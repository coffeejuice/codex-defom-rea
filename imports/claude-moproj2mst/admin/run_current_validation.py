"""Run the current converter build, full CTest and source-bound m2m audit.

Only operates on an existing build directory beneath this converter's build/.
It never launches the original application.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from run_converter_tests import source_fingerprint

ROOT = Path(__file__).resolve().parents[1]
VCVARS = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat")


def msvc_environment():
    if not VCVARS.is_file():
        raise FileNotFoundError(VCVARS)
    command = f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"'
    result = subprocess.run(command, capture_output=True, text=True, check=True)
    env = dict(os.environ)
    for line in result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if sep and key:
            env[key] = value
    return env


def run_step(tag, name, command, env, build, receipt):
    log = build / f"{tag}-{name}.log"
    started = time.monotonic()
    with log.open("w", encoding="utf-8") as output:
        result = subprocess.run(command, cwd=ROOT, env=env,
                                stdout=output, stderr=subprocess.STDOUT)
    step = {"name": name, "command": command, "exit_code": result.returncode,
            "elapsed_seconds": round(time.monotonic() - started, 3),
            "log": str(log)}
    receipt["steps"].append(step)
    return result.returncode


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--tag", required=True,
                        help="lowercase checkpoint name used for receipt and logs")
    args = parser.parse_args()
    if not re.fullmatch(r"cp[0-9]+(?:-[0-9]+)?", args.tag):
        raise ValueError("checkpoint tag must look like cp294 or cp292-293")
    build = (ROOT / args.build).resolve()
    if not build.is_relative_to((ROOT / "build").resolve()) or not (build / "CMakeCache.txt").is_file():
        raise ValueError("existing converter build directory required")
    report = ROOT / "analysis" / f"{args.tag}-full-validation.json"
    env = msvc_environment()
    receipt = {"schema": f"{args.tag}-full-validation-v1", "build": str(build),
               "msvc_environment": str(VCVARS), "steps": [],
               "source_before": source_fingerprint(ROOT / "source"),
               "original_run_ready": False}
    commands = [
        ("build", ["cmake", "--build", str(build), "--parallel", "4"]),
        ("ctest", ["ctest", "--test-dir", str(build), "--parallel", "4",
                   "--output-on-failure", "--no-tests=error"]),
        ("bound-launcher", [sys.executable, "admin/run_converter_tests.py",
                            "--build", str(build), "--jobs", "4",
                            "--hash-engine", "threaded", "--hash-workers", "4",
                            "--hash-path-engine", "exact"]),
    ]
    exit_code = 0
    try:
        for name, command in commands:
            if run_step(args.tag, name, command, env, build, receipt):
                receipt["status"] = f"failed:{name}"
                exit_code = 1
                break
        if exit_code == 0:
            bound = json.loads((ROOT / "analysis/test-relocation-results.json").read_text(encoding="utf-8"))
            receipt["bound_launcher"] = {
                "run": bound["run"], "registered_tests": bound["registered_tests"],
                "counts": bound["counts"], "status": bound["status"],
                "source_binary_stable": bound["execution_binding"]["stable_during_test"],
                "fixture_changes": bound["fixture_changes"],
                "unreported_tests": bound["unreported_tests"],
                "incomplete_output_tests": bound["incomplete_output_tests"]}
            receipt["status"] = "passed"
    finally:
        receipt["source_after"] = source_fingerprint(ROOT / "source")
        receipt["source_stable"] = receipt["source_before"] == receipt["source_after"]
        if receipt.get("status") == "passed" and not receipt["source_stable"]:
            receipt["status"] = "failed:source-drift"
            exit_code = 1
        report.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
        print(json.dumps({"status": receipt.get("status"), "steps": receipt["steps"],
                          "source_stable": receipt["source_stable"],
                          "bound_launcher": receipt.get("bound_launcher")}, separators=(",", ":")))
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main())
