"""Compare two preserved converter test runs without changing either run.

Usage: python admin/compare_test_speed_runs.py BASE_RUN BASE_RECEIPT CAND_RUN CAND_RECEIPT
Exit 0 means equivalent, 1 means a semantic difference, and 2 means evidence is
missing or inconsistent. The runner's shared results file must be copied after
each run because later runs overwrite it.
"""

import argparse
from collections import Counter
import hashlib
import json
import re
from pathlib import Path
import sys
import xml.etree.ElementTree as ET


REQUIRED_RUN_FILES = ("inventory.json", "fixtures-before.json", "fixtures-after.json",
                      "junit.xml", "console.log", "ctest.log")
REQUIRED_RECEIPT_FIELDS = ("run", "build", "registered_tests", "inventory", "exit_code",
                           "fixture_changes", "tests", "counts", "unreported_tests",
                           "incomplete_output_tests", "status")
TIMING_FIELDS = {"elapsed_seconds", "launcher_elapsed_seconds", "phase_seconds",
                 "timing_scope", "timing_receipt", "seconds"}
PATH_FIELDS = {"run", "build", "inventory", "log"}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def read_json(path):
    return json.loads(path.read_text(encoding="utf-8"))


def same_path(a, b):
    return Path(a).resolve() == Path(b).resolve()


def fixture_hash_settings_valid(settings):
    if not isinstance(settings, dict):
        return False
    legacy = {"algorithm", "cache", "engine", "workers"}
    path_fields = {"path_engine", "requested_path_engine"}
    if set(settings) not in (legacy, legacy | path_fields):
        return False
    if (settings.get("algorithm") != "sha256" or settings.get("cache") is not False or
            settings.get("engine") not in ("serial", "threaded") or
            type(settings.get("workers")) is not int or not 1 <= settings["workers"] <= 32):
        return False
    if settings["engine"] == "serial" and settings["workers"] != 1:
        return False
    if path_fields <= set(settings):
        effective, requested = settings["path_engine"], settings["requested_path_engine"]
        if effective not in ("exact", "parent-cache") or requested not in ("exact", "parent-cache"):
            return False
        if effective != (requested if settings["engine"] == "threaded" else "exact"):
            return False
    return True


def canonical(value, run, build, source=None):
    """Replace declared roots at path boundaries; retain all other text."""
    if isinstance(value, str):
        roots = [(str(run), "<RUN>"), (str(build), "<BUILD>")]
        if source is not None:
            roots.append((str(source), "<SOURCE>"))
        for root, token in sorted(roots,
                                  key=lambda pair: len(pair[0]), reverse=True):
            # Windows accepts either separator and ignores path case. Do not
            # consume a longer sibling name such as build/candidate-other.
            pattern = "[/\\\\]".join(re.escape(part) for part in re.split(r"[/\\]", root.rstrip("/\\")))
            value = re.sub(pattern + r"(?=$|[/\\\s\"'<>;)])", lambda _: token, value,
                           flags=re.IGNORECASE)
        return value
    if isinstance(value, list):
        return [canonical(item, run, build, source) for item in value]
    if isinstance(value, dict):
        return {key: canonical(item, run, build, source) for key, item in value.items()}
    return value


def load_run(run_arg, receipt_arg, source=None):
    run, receipt_path = run_arg.resolve(), receipt_arg.resolve()
    issues = []
    if not run.is_dir():
        return None, [f"Missing run directory: {run}"]
    for name in REQUIRED_RUN_FILES:
        if not (run / name).is_file():
            issues.append(f"Missing run artifact: {run / name}")
    for name in ("console.log", "ctest.log", "junit.xml"):
        path = run / name
        if path.is_file() and path.stat().st_size == 0:
            issues.append(f"Empty run artifact: {path}")
    if not receipt_path.is_file():
        issues.append(f"Missing preserved receipt: {receipt_path}")
    if issues:
        return None, issues
    try:
        receipt = read_json(receipt_path)
        inventory = read_json(run / "inventory.json")
        before = read_json(run / "fixtures-before.json")
        after = read_json(run / "fixtures-after.json")
        root = ET.parse(run / "junit.xml").getroot()
    except (ValueError, OSError, ET.ParseError) as exc:
        return None, [f"Could not parse {run}: {exc}"]
    missing = set(REQUIRED_RECEIPT_FIELDS) - receipt.keys()
    if missing:
        issues.append(f"Receipt lacks fields: {', '.join(sorted(missing))}")
        return None, issues
    if not same_path(receipt["run"], run):
        issues.append(f"Receipt belongs to {receipt['run']}, not {run}")
    if not same_path(receipt["inventory"], run / "inventory.json"):
        issues.append("Receipt inventory path does not match run inventory")
    tests = inventory.get("tests", [])
    if not isinstance(tests, list) or not tests:
        issues.append("Inventory contains no tests")
        tests = []
    names = [test.get("name") for test in tests]
    if len(names) != len(set(names)):
        issues.append("Inventory contains duplicate test names")
    if receipt["registered_tests"] != len(names):
        issues.append("Receipt registered_tests differs from inventory size")
    rows = receipt["tests"]
    if not isinstance(rows, list):
        issues.append("Receipt tests is not a list")
        rows = []
    row_names = [row.get("name") for row in rows]
    if len(row_names) != len(set(row_names)):
        issues.append("Receipt contains duplicate test names")
    if set(receipt["unreported_tests"]) != set(names) - set(row_names):
        issues.append("Receipt unreported_tests does not match inventory minus test rows")
    if set(row_names) != set(names):
        issues.append("Receipt rows do not cover exactly the registered tests")
    if receipt["unreported_tests"] or receipt["incomplete_output_tests"]:
        issues.append("Run has missing tests or incomplete output")
    if receipt["exit_code"] != 0 or receipt["fixture_changes"]:
        issues.append("Run failed or changed fixture files")
    if receipt["status"] not in {"completed", "completed-with-recorded-gaps"}:
        issues.append("Run did not complete")
    hash_settings = receipt.get("fixture_hash")
    if hash_settings is not None and not fixture_hash_settings_valid(hash_settings):
        issues.append("Unsupported fixture hashing settings")
    if set(receipt["incomplete_output_tests"]) != {row["name"] for row in rows
                                                        if row.get("output_incomplete")}:
        issues.append("Receipt incomplete_output_tests does not match test rows")
    if receipt["counts"] != dict(Counter(row.get("status") for row in rows),
                                 **{key: 0 for key in receipt["counts"]
                                    if key not in {row.get("status") for row in rows}}):
        issues.append("Receipt status counts do not match test rows")
    calculated_changes = sorted(set(before) ^ set(after) |
                                {key for key in set(before) & set(after) if before[key] != after[key]})
    if calculated_changes != receipt["fixture_changes"]:
        issues.append("Receipt fixture_changes does not match before/after manifests")
    junit_cases = list(root.iter("testcase"))
    junit_names = [case.attrib.get("name") for case in junit_cases]
    if len(junit_names) != len(set(junit_names)):
        issues.append("JUnit contains duplicate test names")
    if set(junit_names) != set(row_names):
        issues.append("JUnit test inventory differs from receipt rows")
    row_by_name = {row["name"]: row for row in rows}
    for name, row in row_by_name.items():
        sweep = row.get("folder_sweep")
        should_be_incomplete = bool(row.get("output_truncated") or
                                    (sweep is not None and not sweep.get("complete")))
        if row.get("output_incomplete") != should_be_incomplete:
            issues.append(f"Output completeness flags disagree: {name}")
    logs = {}
    for case in junit_cases:
        name = case.attrib.get("name")
        if name not in row_by_name:
            continue
        row = row_by_name[name]
        log = Path(row.get("log", ""))
        if not same_path(log.parent, run):
            issues.append(f"Case log is outside run: {name}")
            continue
        if not log.is_file():
            issues.append(f"Missing case log: {name}")
            continue
        raw = log.read_bytes()
        logs[name] = raw
        # Path.read_text uses universal-newline conversion, matching the
        # runner's text write on Windows. Raw bytes remain intact for diffing.
        if log.read_text(encoding="utf-8") != case.findtext("system-out", ""):
            issues.append(f"Case log differs from JUnit system-out: {name}")
        if (case.find("failure") is not None) != (row.get("status") == "failed"):
            issues.append(f"JUnit failure disagrees with receipt: {name}")
        if (case.find("skipped") is not None) != (row.get("status") == "skipped"):
            issues.append(f"JUnit skip disagrees with receipt: {name}")
    return dict(run=run, receipt=receipt, inventory=inventory, before=before,
                after=after, rows=row_by_name, logs=logs, source=source,
                receipt_path=receipt_path), issues


def compare(a, b):
    blockers, permitted = [], []
    qualifications = []

    def folder_cost_properties(run):
        tests = [test for test in run["inventory"]["tests"] if test["name"] == "m2m.s.save_folder"]
        if len(tests) != 1:
            return None
        return [prop for prop in tests[0].get("properties", []) if prop.get("name") == "COST"]

    # Only this approved directional addition is scheduler metadata. Unknown
    # values, duplicate properties and COST on other tests remain compared.
    old_cost, new_cost = folder_cost_properties(a), folder_cost_properties(b)
    folder_cost_added = old_cost == [] and new_cost == [{"name": "COST", "value": 25}]
    if folder_cost_added:
        permitted.append("m2m.s.save_folder: explicit CTest COST=25 added (scheduling metadata)")
        qualifications.append("CTest COST=25 may change scheduling order and measured contention; acceptance still requires identical test membership, environment, all other properties, fixtures and semantic diagnostics.")

    def normalized(value, run):
        return canonical(value, run["run"], run["receipt"]["build"], run.get("source"))

    def case_output(value, run, name):
        value = normalized(value, run)
        if name != "m2m.mdt.db_inheritance":
            return value
        test = next((item for item in run["inventory"]["tests"] if item["name"] == name), {})
        scratch = test.get("audited_settings", {}).get("M2M_TEST_SCRATCH")
        if not scratch:
            return value
        # This test names its owned temporary directory with the process ID.
        # Admit only the complete missing-step diagnostic under its registered
        # scratch root. The step, missing_step_4, filename and all other output
        # remain compared. Never erase arbitrary numbers or inherited paths.
        scratch = normalized(scratch, run).rstrip("/\\")
        pattern = (r"^(Step [0-9]+ not found in Database " + re.escape(scratch) +
                   r"[/\\]m2m_inherited_)[0-9]+([/\\]missing_step_4[/\\]Inherited\.DB\.)(?=\r?$)")
        return re.sub(pattern, lambda match: match[1] + "<PID>" + match[2], value, flags=re.MULTILINE)

    def inventory_semantics(run):
        value = normalized(run["inventory"], run)
        if run is b and folder_cost_added:
            test = next(item for item in value["tests"] if item["name"] == "m2m.s.save_folder")
            test["properties"] = [prop for prop in test["properties"] if prop.get("name") != "COST"]
        # Source insertion moves add_test line numbers. Keep every graph edge,
        # file, command, property and test binding; only source line is provenance.
        for node in value.get("backtraceGraph", {}).get("nodes", []):
            node.pop("line", None)
        return value

    def check(label, old, new):
        if old != new:
            blockers.append(label)

    check("registered test names", sorted(t["name"] for t in a["inventory"]["tests"]),
          sorted(t["name"] for t in b["inventory"]["tests"]))
    old_inv, new_inv = a["inventory"], b["inventory"]
    if old_inv != new_inv:
        if inventory_semantics(a) == inventory_semantics(b):
            permitted.append("Inventory differs only by declared roots, CMake backtrace source lines or approved folder COST=25 addition")
        else:
            blockers.append("Inventory properties, commands or backtrace graph differ beyond declared roots/source lines")
    for name in ("before", "after"):
        old, new = a[name], b[name]
        check(f"fixtures-{name} keys", sorted(old), sorted(new))
        check(f"fixtures-{name} key order", list(old), list(new))
        if (a["run"] / f"fixtures-{name}.json").read_bytes() != (b["run"] / f"fixtures-{name}.json").read_bytes():
            blockers.append(f"fixtures-{name} raw manifest bytes differ")
        changed = [key for key in sorted(old.keys() & new.keys()) if old[key] != new[key]]
        if changed:
            blockers.append(f"fixtures-{name} manifest entries differ: {changed}")
    receipt_fields = set(a["receipt"]) | set(b["receipt"])
    fields = receipt_fields - TIMING_FIELDS - PATH_FIELDS - {"tests", "command", "fixture_hash"}
    for field in sorted(fields):
        check(f"receipt.{field}", a["receipt"].get(field), b["receipt"].get(field))
    for field in sorted(TIMING_FIELDS & receipt_fields):
        if a["receipt"].get(field) != b["receipt"].get(field):
            permitted.append(f"Timing field differs: receipt.{field}")
    for field in ("run", "build", "inventory", "timing_receipt"):
        if a["receipt"].get(field) != b["receipt"].get(field):
            permitted.append(f"Path field differs: receipt.{field}")
    if a["receipt"].get("fixture_hash") != b["receipt"].get("fixture_hash"):
        permitted.append("Fixture hash execution metadata differs (validated uncached SHA-256; manifests compared exactly)")
    if a["receipt"].get("command") != b["receipt"].get("command"):
        old = normalized(a["receipt"].get("command"), a)
        new = normalized(b["receipt"].get("command"), b)
        def scheduling_command(command):
            if not isinstance(command, list) or command.count("-j") != 1:
                return None
            index = command.index("-j")
            if index + 1 >= len(command) or not isinstance(command[index + 1], str) or not re.fullmatch(r"[1-9][0-9]*", command[index + 1]):
                return None
            result = list(command)
            result[index + 1] = "<POSITIVE_CTEST_JOBS>"
            return result
        old_scheduled, new_scheduled = scheduling_command(old), scheduling_command(new)
        if old == new:
            permitted.append("CTest command differs only by known run/build roots")
        elif old_scheduled is not None and new_scheduled is not None and old_scheduled == new_scheduled:
            permitted.append(f"CTest -j scheduling parameter differs: {old[old.index('-j') + 1]} -> {new[new.index('-j') + 1]}; all other arguments agree after declared root substitution")
            qualifications.append("Different positive CTest -j values change scheduling and resource contention; matching complete test/fixture/diagnostic evidence is still required independently of timing.")
        else:
            blockers.append("CTest command differs beyond known roots or one positive -j scheduling value")
    names = sorted(set(a["rows"]) | set(b["rows"]))
    for name in names:
        old, new = a["rows"].get(name), b["rows"].get(name)
        if old is None or new is None:
            blockers.append(f"Missing test receipt row: {name}")
            continue
        semantic_keys = set(old) | set(new)
        semantic_keys -= {"seconds", "log"}
        for key in sorted(semantic_keys):
            if normalized(old.get(key), a) != normalized(new.get(key), b):
                blockers.append(f"{name}: {key} differs")
            elif old.get(key) != new.get(key):
                permitted.append(f"{name}: {key} differs only by declared roots")
        if old.get("seconds") != new.get("seconds"):
            permitted.append(f"{name}: JUnit test duration differs")
        old_log, new_log = a["logs"].get(name), b["logs"].get(name)
        if old_log is None or new_log is None:
            blockers.append(f"{name}: case log unavailable")
        elif old_log != new_log:
            old_text = old_log.decode("utf-8", errors="replace")
            new_text = new_log.decode("utf-8", errors="replace")
            if case_output(old_text, a, name) == case_output(new_text, b, name):
                permitted.append(f"{name}: raw log declared path/test-owned PID change, sha256 {digest(old_log)} -> {digest(new_log)}")
            else:
                blockers.append(f"{name}: raw log changed, sha256 {digest(old_log)} -> {digest(new_log)}")
    abi = sorted(name for name in names if name.startswith("m2m.g0.abi"))
    if not abi:
        blockers.append("No ABI tests found")
    raw_differences = []
    artifacts = [(name, a["run"] / name, b["run"] / name) for name in REQUIRED_RUN_FILES]
    artifacts.append(("receipt", a["receipt_path"], b["receipt_path"]))
    for name in sorted(set(a["logs"]) & set(b["logs"])):
        artifacts.append((f"case:{name}", Path(a["rows"][name]["log"]), Path(b["rows"][name]["log"])))
    for name, old_path, new_path in artifacts:
        old_bytes, new_bytes = old_path.read_bytes(), new_path.read_bytes()
        if old_bytes != new_bytes:
            raw_differences.append({"artifact": name, "baseline": str(old_path), "candidate": str(new_path),
                                    "baseline_sha256": digest(old_bytes), "candidate_sha256": digest(new_bytes)})
    return {"equivalent": not blockers, "blockers": blockers, "permitted_differences": permitted,
            "qualifications": qualifications,
            "raw_differences": raw_differences,
            "ctest_commands": {"baseline": a["receipt"].get("command"), "candidate": b["receipt"].get("command")},
            "source_roots": {"baseline": str(a["source"]) if a.get("source") else None,
                             "candidate": str(b["source"]) if b.get("source") else None},
            "test_count": len(names), "abi_tests": abi,
            "baseline": str(a["run"]), "candidate": str(b["run"])}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline_run", type=Path)
    parser.add_argument("baseline_receipt", type=Path)
    parser.add_argument("candidate_run", type=Path)
    parser.add_argument("candidate_receipt", type=Path)
    parser.add_argument("--baseline-source", type=Path)
    parser.add_argument("--candidate-source", type=Path)
    args = parser.parse_args()
    if bool(args.baseline_source) != bool(args.candidate_source):
        parser.error("Source roots must be supplied as a pair")
    for source in (args.baseline_source, args.candidate_source):
        if source is not None and not source.is_dir():
            parser.error(f"Source root does not exist: {source}")
    baseline, b_issues = load_run(args.baseline_run, args.baseline_receipt,
                                args.baseline_source.resolve() if args.baseline_source else None)
    candidate, c_issues = load_run(args.candidate_run, args.candidate_receipt,
                                 args.candidate_source.resolve() if args.candidate_source else None)
    if b_issues or c_issues:
        print(json.dumps({"equivalent": False, "evidence_issues": b_issues + c_issues}, indent=2))
        return 2
    result = compare(baseline, candidate)
    print(json.dumps(result, indent=2))
    return 0 if result["equivalent"] else 1


if __name__ == "__main__":
    sys.exit(main())
