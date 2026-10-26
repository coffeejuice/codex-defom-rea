"""Run the relocated converter suite, preserving skips/refusals and fixture hashes.

Configure/build first. This launcher neither builds nor starts the original app.
Usage: python admin/run_converter_tests.py [--build build/baseline] [--inventory-only]
"""
import argparse
from contextlib import contextmanager
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "evidence"
EVENT = re.compile(r"^\s*(?:SKIP\b|\[skip\]|REFUS(?:E|ED|AL)\b|NOTE\b)|"
                   r"\b[1-9]\d* (?:cases |groups )?skipped\b|\b[1-9]\d* refused\b|"
                   r"refused \(active|not compared|without a finished fresh re-save", re.I)
EMPTY_ACTIVE_COGGING_REFUSAL = re.compile(
    r"^\s*refused \(active cogging operation\):\s*$", re.I)
TRUNCATION = re.compile(r"exceeds the threshold|test output (?:was )?truncated", re.I)
FIXTURES = ("M2M_CORPUS", "M2M_ORACLE_B", "M2M_DB_FIXTURE", "M2M_REFERENCE_EXE")
OUTPUTS = ("TEMP", "TMP", "TMPDIR", "M2M_S_DUMP", "M2M_DB_DUMP", "M2M_MOPROJ_DUMP")


def gap_events(output):
    """Return diagnostic gap lines, ignoring an empty active-cogging summary."""
    return [line for line in output.splitlines()
            if not EMPTY_ACTIVE_COGGING_REFUSAL.fullmatch(line) and EVENT.search(line)]


class LaunchTiming:
    """Record wall time without changing the CTest-only elapsed_seconds field."""

    def __init__(self):
        self.started = time.monotonic()
        self.run = None
        self.phases = {}

    @contextmanager
    def phase(self, name):
        started = time.monotonic()
        try:
            yield
        finally:
            self.phases[name] = round(time.monotonic() - started, 6)

    def write(self, return_code=None, error=None):
        if self.run is None:
            return
        receipt = {
            "run": str(self.run),
            "launcher_elapsed_seconds": round(time.monotonic() - self.started, 6),
            "phase_seconds": self.phases,
            "timing_scope": "main entry through completion of inventory, fixture snapshots, source/binary bindings, CTest, JUnit parsing, and report output; excludes this timing-receipt write and process exit",
            "return_code": return_code,
            "error": error,
        }
        (self.run / "timing.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")


def child(value, parent):
    path, parent = Path(value).resolve(), parent.resolve()
    if path == parent or not path.is_relative_to(parent):
        raise ValueError(f"Path must remain beneath {parent}: {path}")
    return path


def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def source_fingerprint(source):
    """Hash the source tree with the same path/content contract as builds."""
    digest = hashlib.sha256()
    files = 0
    for path in sorted(source.rglob("*")):
        if path.is_file():
            digest.update(path.relative_to(source).as_posix().encode())
            digest.update(b"\0")
            digest.update(bytes.fromhex(sha(path)))
            files += 1
    return {"sha256": digest.hexdigest(), "files": files}


def binary_paths(tests, build=None):
    """Find test executables, converter CLI and the built public ABI artifacts."""
    paths = set()
    for test in tests:
        command = test.get("command", [])
        if command:
            executable = Path(command[0]).resolve()
            if executable.is_file():
                paths.add(executable)
        for prop in test.get("properties", []):
            if prop.get("name") != "ENVIRONMENT":
                continue
            for item in prop.get("value", []):
                key, sep, value = item.partition("=")
                if sep and key == "M2M_CLI":
                    executable = Path(value).resolve()
                    if executable.is_file():
                        paths.add(executable)
    if build is not None:
        for name in ("moproj2mst.exe", "moproj2mst.dll", "deformdb.dll"):
            artifact = (build / "bin" / name).resolve()
            if artifact.is_file():
                paths.add(artifact)
    return sorted(paths)


def binary_snapshot(paths):
    return {str(path): {"bytes": path.stat().st_size, "sha256": sha(path)}
            for path in paths}


def execution_binding(source, binaries):
    return {"source_tree": source_fingerprint(source),
            "binaries": binary_snapshot(binaries)}


def binding_is_stable(before, after):
    return before == after


def snapshot(roots):
    result = {}
    for root in sorted(roots):
        files = [root] if root.is_file() else sorted(p for p in root.rglob("*") if p.is_file())
        for path in files:
            path = child(path, EVIDENCE)
            st = path.stat()
            result[str(path.relative_to(EVIDENCE))] = [st.st_size, st.st_mtime_ns, sha(path)]
    return result


def hash_workers(value):
    """Argparse validation, shared by both engines for reproducible commands."""
    workers = int(value)
    if not 1 <= workers <= 32:
        raise argparse.ArgumentTypeError("hash workers must be from 1 through 32")
    return workers


def fixture_snapshot(roots, engine="serial", workers=4, path_engine="exact"):
    """Select an exact, uncached SHA-256 engine; keep legacy serial as default."""
    if engine == "serial":
        return snapshot(roots)
    if engine == "threaded":
        from fixture_hash_fast import snapshot as threaded_snapshot
        return threaded_snapshot(roots, evidence=EVIDENCE, workers=workers, path_engine=path_engine)
    raise ValueError(f"Unknown fixture hash engine: {engine}")


def folder_sweep(output):
    """Parse diagnostic case rows and reconcile them with both printed tallies."""
    cases, tallies, notes, skipped = [], [], [], []
    row = re.compile(r"^(SAME|DIFF) (.+): (\d+) differ, (\d+) missing, (\d+) extra(.*)$")
    refused = re.compile(r"^REFUSED (.+) \((\d+)\): (.*)$")
    summary = re.compile(r"^(.+): (\d+) cases, (\d+) identical \((\d+) with an expected older-window "
                         r"\.moproj view profile\), (\d+) refused, (\d+) file diffs, "
                         r"(\d+) files with another version banner$")
    for line in output.splitlines():
        match = row.match(line)
        if match:
            kind, name, differ, missing, extra, adjustment = match.groups()
            cases.append(dict(result=kind, case=name, differ=int(differ), missing=int(missing),
                              extra=int(extra), comparator_adjustments=adjustment.strip()))
            continue
        match = refused.match(line)
        if match:
            cases.append(dict(result="REFUSED", case=match[1], code=int(match[2]), message=match[3]))
            continue
        match = summary.match(line)
        if match:
            tallies.append(dict(zip(("set", "cases", "identical", "view_profiles", "refused", "file_diffs", "banner_files"),
                                    [match[1], *(int(v) for v in match.groups()[1:])])) )
        elif line.startswith("NOTE ") or "without a finished fresh re-save" in line:
            notes.append(line)
        elif line.startswith("SKIP "):
            skipped.append(line)
    issues = []
    if len(tallies) != 2 or {t["set"] for t in tallies} != {"corpus (fresh re-saves RS_*)", "oracle B"}:
        issues.append("Missing one or both folder-sweep summary tallies")
    if sum(t["cases"] for t in tallies) != len(cases) + len(skipped):
        issues.append("Folder-sweep denominator does not match captured case rows and copy skips")
    if sum(t["identical"] for t in tallies) != sum(c["result"] == "SAME" for c in cases):
        issues.append("SAME row count differs from summary identical count")
    if sum(t["refused"] for t in tallies) != sum(c["result"] == "REFUSED" for c in cases):
        issues.append("REFUSED row count differs from summary refusal count")
    if sum(t["file_diffs"] for t in tallies) != sum(c.get("differ", 0) + c.get("missing", 0) + c.get("extra", 0) for c in cases):
        issues.append("File difference totals differ from captured case rows")
    return dict(cases=cases, tallies=tallies, notes=notes, copy_skips=skipped,
                complete=not issues and not TRUNCATION.search(output), completeness_issues=issues)


def main(timing=None):
    if timing is None:
        timing = LaunchTiming()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, default=ROOT / "build/baseline")
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--hash-engine", choices=("serial", "threaded"), default="serial",
                        help="fixture SHA-256 engine (default: legacy serial)")
    parser.add_argument("--hash-workers", type=hash_workers, default=4,
                        help="threaded hash workers, 1..32 (default: 4; ignored by serial)")
    parser.add_argument("--hash-path-engine", choices=("exact", "parent-cache"), default="exact",
                        help="threaded path resolution (default: exact; parent-cache requires stable "
                             "directory/reparse topology; ignored by serial)")
    parser.add_argument("--inventory-only", action="store_true")
    args = parser.parse_args()
    build = child(args.build, ROOT / "build")
    run = ROOT / "build/test-runs" / str(time.time_ns())
    run.mkdir(parents=True, exist_ok=False)
    timing.run = run
    env = dict(os.environ)
    # Optional externally supplied fixture/output overrides must never leak into
    # these research runs. The registered CTest properties set all supported paths.
    for name in ("M2M_INSTALL_FIXTURE", "M2M_DEFLATE_VECTORS", "M2M_S_CASES",
                 "M2M_S_DUMP", "M2M_DB_DUMP", "M2M_MOPROJ_DUMP"):
        env.pop(name, None)
    for name in ("TEMP", "TMP", "TMPDIR"):
        env[name] = str(run)
    command = ["ctest", "--test-dir", str(build), "-L", "m2m"]
    with timing.phase("inventory"):
        listed = subprocess.run(command + ["--show-only=json-v1"], env=env, cwd=run,
                                text=True, capture_output=True, check=True)
        inventory = json.loads(listed.stdout)
        tests = inventory.get("tests", [])
        if not tests:
            raise RuntimeError("No m2m tests registered")
        roots = set()
        for test in tests:
            properties = {p["name"]: p["value"] for p in test.get("properties", [])}
            settings = dict(pair.split("=", 1) for pair in properties.get("ENVIRONMENT", []))
            scratch = child(settings["M2M_TEST_SCRATCH"], ROOT / "build")
            child(properties["WORKING_DIRECTORY"], ROOT / "build")
            for name in OUTPUTS:
                value = Path(settings[name]).resolve()
                if value != scratch:
                    child(value, scratch)
            for name in FIXTURES:
                fixture = child(settings[name], EVIDENCE)
                if not fixture.exists():
                    raise FileNotFoundError(f"{name}: {fixture}")
                roots.add(fixture)
            if settings.get("M2M_FOLDER_SWEEP") != "1":
                raise RuntimeError("Full Save folder sweep is not enabled")
            test["audited_settings"] = settings
        (run / "inventory.json").write_text(json.dumps(inventory, indent=2), encoding="utf-8")
    receipt = {
        "run": str(run), "build": str(build), "registered_tests": len(tests),
        "scope": "Imported converter CTest suite, including historical full Save folder sweep",
        "qualifications": [
            "Historical oracles are not fresh original-session acceptance evidence",
            "CTest success can contain case skips, expected refusals or permissive assertions",
            "g0.abi real-project assertion permits M2M_OK or M2M_E_UNSUPPORTED",
            "Deflate external vectors and imported PACK fixture absent unless separately acquired",
            "s.save_folder is diagnostic: it logs differences/refusals without failing assertions",
            "Full folder comparator normalizes declared view/banner profiles; inspect raw differences",
            "No original executable is launched; keyword reference is read as a PE file only",
        ],
        "inventory": str(run / "inventory.json"), "token_measurement": "unavailable",
        "timing_receipt": str(run / "timing.json"),
        "fixture_hash": {"engine": args.hash_engine,
                         "workers": args.hash_workers if args.hash_engine == "threaded" else 1,
                         "path_engine": args.hash_path_engine if args.hash_engine == "threaded" else "exact",
                         "requested_path_engine": args.hash_path_engine,
                         "algorithm": "sha256", "cache": False},
    }
    if args.inventory_only:
        with timing.phase("inventory_report"):
            receipt["status"] = "inventory-only; tests not executed"
            rendered = json.dumps(receipt, indent=2)
            (ROOT / "analysis/test-relocation-inventory.json").write_text(rendered, encoding="utf-8")
            (run / "receipt.json").write_text(rendered, encoding="utf-8")
            print(rendered)
        return 0
    with timing.phase("fixture_before_sha"):
        before = fixture_snapshot(roots, args.hash_engine, args.hash_workers, args.hash_path_engine)
        (run / "fixtures-before.json").write_text(json.dumps(before, indent=2), encoding="utf-8")
    junit = run / "junit.xml"
    with timing.phase("execution_binding_before"):
        binaries = binary_paths(tests, build)
        binding_before = execution_binding(ROOT / "source", binaries)
    started = time.monotonic()
    command += ["--verbose", "--output-on-failure", "--no-tests=error", "-j", str(max(1, args.jobs)),
                "--test-output-size-passed", "33554432", "--test-output-size-failed", "33554432",
                "--output-junit", str(junit), "--output-log", str(run / "ctest.log")]
    with timing.phase("ctest"):
        with (run / "console.log").open("w", encoding="utf-8") as log:
            result = subprocess.run(command, env=env, cwd=run, stdout=log, stderr=subprocess.STDOUT)
    receipt.update(command=command, ctest_exit_code=result.returncode,
                   elapsed_seconds=round(time.monotonic() - started, 3))
    with timing.phase("execution_binding_after"):
        try:
            binding_after = execution_binding(ROOT / "source", binaries)
            binding_error = None
            binding_stable = binding_is_stable(binding_before, binding_after)
        except OSError as exc:
            binding_after = None
            binding_error = f"Cannot capture end-of-test binding: {type(exc).__name__}: {exc}"
            binding_stable = False
    receipt["exit_code"] = result.returncode if binding_stable else 1
    receipt["execution_binding"] = {
        "algorithm": "sha256",
        "source_tree_before": binding_before["source_tree"],
        "source_tree_after": binding_after["source_tree"] if binding_after else None,
        "binaries_before": binding_before["binaries"],
        "binaries_after": binding_after["binaries"] if binding_after else None,
        "stable_during_test": binding_stable,
    }
    if binding_error:
        receipt["execution_binding"]["error"] = binding_error
    with timing.phase("fixture_after_sha"):
        after = fixture_snapshot(roots, args.hash_engine, args.hash_workers, args.hash_path_engine)
        (run / "fixtures-after.json").write_text(json.dumps(after, indent=2), encoding="utf-8")
        changed = sorted(k for k in before.keys() | after.keys() if before.get(k) != after.get(k))
    receipt["fixture_changes"] = changed
    with timing.phase("junit_parsing_report"):
        rows = []
        if junit.exists():
            for case in ET.parse(junit).getroot().iter("testcase"):
                output = case.findtext("system-out", "")
                events = gap_events(output)
                sweep = folder_sweep(output) if case.attrib["name"] == "m2m.s.save_folder" else None
                truncated = bool(TRUNCATION.search(output))
                incomplete = truncated or (sweep is not None and not sweep["complete"])
                differences = [c for c in sweep["cases"] if c["result"] == "DIFF"] if sweep else []
                status = ("failed" if case.find("failure") is not None else
                          "skipped" if case.find("skipped") is not None else
                          "incomplete-output" if incomplete else
                          "passed-with-logged-differences" if differences else
                          "passed-with-logged-gaps" if events else "passed")
                log = run / (re.sub(r"[^\w.-]", "_", case.attrib["name"]) + ".log")
                log.write_text(output, encoding="utf-8")
                rows.append({"name": case.attrib["name"], "status": status,
                             "seconds": case.attrib.get("time"), "gap_events": events,
                             "diagnostic_differences": differences, "output_truncated": truncated,
                             "output_incomplete": incomplete, "folder_sweep": sweep, "log": str(log)})
        receipt["tests"] = rows
        receipt["counts"] = {status: sum(r["status"] == status for r in rows) for status in
                             ("passed", "passed-with-logged-gaps", "passed-with-logged-differences", "incomplete-output", "skipped", "failed")}
        receipt["unreported_tests"] = sorted(set(t["name"] for t in tests) - set(r["name"] for r in rows))
        receipt["incomplete_output_tests"] = [r["name"] for r in rows if r["output_incomplete"]]
        rc = 1 if (changed or receipt["unreported_tests"] or receipt["incomplete_output_tests"]
                   or not binding_stable) else result.returncode
        gaps = any(r["status"] != "passed" for r in rows)
        receipt["status"] = ("completed-with-recorded-gaps" if gaps else "completed") if rc == 0 else "failed-or-incomplete"
        if not binding_stable:
            receipt["execution_binding"]["status"] = "incomplete: source or test binary drift during CTest"
        rendered = json.dumps(receipt, indent=2)
        (ROOT / "analysis/test-relocation-results.json").write_text(rendered, encoding="utf-8")
        (run / "receipt.json").write_text(rendered, encoding="utf-8")
        print(json.dumps({k: v for k, v in receipt.items() if k != "tests"}, indent=2))
    return rc


if __name__ == "__main__":
    timing = LaunchTiming()
    return_code = None
    error = None
    try:
        return_code = main(timing)
    except BaseException as exc:
        error = {"type": type(exc).__name__, "message": str(exc)}
        raise
    finally:
        # Timing output must not hide the launcher's original failure.
        try:
            timing.write(return_code=return_code, error=error)
        except OSError as exc:
            print(f"Could not write launcher timing receipt: {exc}", file=sys.stderr)
    raise SystemExit(return_code)
