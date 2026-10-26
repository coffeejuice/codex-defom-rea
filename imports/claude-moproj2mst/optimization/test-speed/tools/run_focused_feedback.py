"""Plan or run limited optimization feedback; never a full acceptance gate."""

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import time
import xml.etree.ElementTree as ET

EXPERIMENT = Path(__file__).resolve().parents[1]
ROOT = EXPERIMENT.parents[1]
BUILD = ROOT / "build/test-speed-candidate"
BUILD_DRIVER = EXPERIMENT / "build_isolated.py"


def registrations():
    """Read only generated registrations, without executing tests or fixture reads."""
    result = {}
    for line in (BUILD / "CTestTestfile.cmake").read_text(encoding="utf-8").splitlines():
        if line.startswith('add_test("m2m.'):
            fields = re.findall(r'"([^"\n]*)"', line)
            result[fields[0]] = fields[1:]
    if not result:
        raise ValueError("No converter test registrations in existing build")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--label", required=True)
    parser.add_argument("--test", action="append", required=True,
                        help="Exact CTest name; repeat for several tests")
    parser.add_argument("--last-full-run", type=Path, required=True,
                        help="Receipt JSON from the last successful full validation")
    parser.add_argument("--jobs", type=int, default=4, help="Build parallelism")
    parser.add_argument("--execute", action="store_true",
                        help="Execute after coordinating exclusive build/test access")
    args = parser.parse_args()
    if not re.fullmatch(r"[A-Za-z0-9_-]+", args.label):
        parser.error("Use a filename-safe label")
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    full_path = args.last_full_run.resolve()
    if not full_path.is_relative_to(ROOT.resolve()):
        parser.error("--last-full-run must be a receipt within the converter checkout")
    full = json.loads(full_path.read_text(encoding="utf-8"))
    if (full.get("exit_code") != 0 or full.get("fixture_changes") != []
            or not full.get("registered_tests")):
        parser.error("Linked receipt must record successful full tests and unchanged fixtures")
    available = registrations()
    if (full["registered_tests"] != len(available)
            or full.get("unreported_tests", []) or full.get("incomplete_output_tests", [])):
        parser.error("Linked full receipt does not cover the current registered test count")
    selected = sorted(set(args.test))
    unknown = set(selected) - available.keys()
    if unknown:
        parser.error("Unknown exact test names: " + ", ".join(sorted(unknown)))
    # Always include both runtime products: tests can invoke the CLI or load the
    # DLL indirectly without expressing those relationships as CMake dependencies.
    targets = {"moproj2mst", "moproj2mst_cli"}
    for name in selected:
        executable = Path(available[name][0])
        if executable.parent.resolve() == (BUILD / "bin").resolve():
            targets.add("moproj2mst_cli" if executable.stem == "moproj2mst"
                        else executable.stem)
    destination = EXPERIMENT / "analysis" / (args.label + "-focused-feedback")
    build_label = args.label + "-focused"
    build_receipt = EXPERIMENT / "analysis" / (build_label + "-build.json")
    command_build = [sys.executable, str(BUILD_DRIVER), "--label", build_label,
                     "--skip-configure", "--jobs", str(args.jobs)]
    for target in sorted(targets):
        command_build.extend(["--target", target])
    # CTest's regex engine supports escaped dots, grouping and anchors. Restrict
    # names to the existing naming alphabet instead of accepting raw regex input.
    if any(not re.fullmatch(r"[A-Za-z0-9_.-]+", name) for name in selected):
        parser.error("Unsupported test name alphabet")
    expression = "^(" + "|".join(name.replace(".", r"\.") for name in selected) + ")$"
    command_test = ["ctest", "--test-dir", str(BUILD), "-R", expression,
                    "--no-tests=error", "--verbose", "--output-on-failure", "-j", "1",
                    "--test-output-size-passed", "33554432",
                    "--test-output-size-failed", "33554432",
                    "--output-junit", str(destination / "junit.xml"),
                    "--output-log", str(destination / "ctest.log")]
    receipt = {
        "mode": "focused-feedback-only", "full_acceptance": False,
        "full_suite_coverage": False, "fixture_integrity_coverage": False,
        "qualification": "Selected tests only; no before/after fixture hash gate, no full-suite equivalence claim. Full acceptance remains required.",
        "last_full_run": str(full_path),
        "last_full_run_sha256": hashlib.sha256(full_path.read_bytes()).hexdigest(),
        "last_full_registered_tests": full["registered_tests"],
        "selected_tests": selected, "targets": sorted(targets),
        "build_receipt": str(build_receipt),
        "commands": [command_build, command_test], "steps": [],
        "status": "planned" if not args.execute else "started",
    }
    if not args.execute:
        print(json.dumps(receipt, indent=2))
        return 0
    if destination.exists() or build_receipt.exists():
        parser.error("Choose a fresh label; existing evidence will not be overwritten")
    destination.mkdir(parents=True)
    output = destination / "receipt.json"
    output.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    spec = importlib.util.spec_from_file_location("isolated_build", BUILD_DRIVER)
    driver = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(driver)
    began = time.perf_counter()
    try:
        env = driver.vc_environment()
        for kind, command in zip(("build", "ctest"), (command_build, command_test)):
            started = time.perf_counter()
            with (destination / (kind + "-console.log")).open("w", encoding="utf-8") as log:
                run = subprocess.run(command, cwd=ROOT, env=env, stdout=log,
                                     stderr=subprocess.STDOUT, text=True)
            receipt["steps"].append({"kind": kind, "seconds": round(time.perf_counter() - started, 3),
                                     "exit_code": run.returncode})
            if run.returncode:
                raise RuntimeError(kind + " failed; inspect retained console log")
        cases = ET.parse(destination / "junit.xml").getroot().findall(".//testcase")
        names = sorted(case.attrib["name"] for case in cases)
        if names != selected or any(case.find("skipped") is not None or
                                   case.find("failure") is not None or
                                   case.find("error") is not None for case in cases):
            raise RuntimeError("JUnit test identity/status does not match requested tests")
        receipt["status"] = "focused-tests-passed-not-full-acceptance"
    except Exception as error:
        receipt["status"] = "failed"
        receipt["error"] = str(error)
    finally:
        receipt["elapsed_seconds"] = round(time.perf_counter() - began, 3)
        output.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"receipt": str(output), "status": receipt["status"],
                      "elapsed_seconds": receipt["elapsed_seconds"]}))
    return 1 if receipt["status"] == "failed" else 0


if __name__ == "__main__":
    raise SystemExit(main())
