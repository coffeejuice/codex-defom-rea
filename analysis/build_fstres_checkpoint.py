"""Aggregate four completed FSTRES batches; never execute a replay or original.

Prepared before capture. Final independent report schemas and registration must
be inspected before first use; missing or inconsistent evidence stops output.
"""
import argparse
from collections import Counter
import datetime
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FREEZE = "analysis/fstres-candidate-freeze.json"
COMPARATOR = "tools/compare_fstres_import.py"
VERIFIER = "tools/verify_fstres_observations.py"
PROBE = "tools/observe_fstres_import.js"
REGISTRATION = "analysis/fstres-test-registration.json"
STATIC = ["analysis/fstres-static-contract.json", "analysis/fstres-byte-audit.json",
          "analysis/fstres-model-inventory.json", "analysis/fstres-implementation-review.json",
          "analysis/fstres-comparator-precapture-review.json", "analysis/fstres-follow-on-scope.json"]
BATCHES = {
    name: {"capture": f"analysis/fstres-runtime-{name}.json",
           "cases": f"analysis/fstres-{name}-cases.json",
           "input_freeze": f"analysis/fstres-{name}-input-freeze.json",
           "run": f"analysis/oracle/fstres-{name}{'-v2' if name in ('training-a', 'held-out-b') else ''}/run.json",
           "comparison": f"analysis/fstres-{name}-comparison.json",
           "observations": f"analysis/fstres-{name}-observations.json"}
    for name in ("training-a", "training-b", "held-out-a", "held-out-b")
}


def require(value, message):
    if not value:
        raise ValueError(message)


def local(path):
    p = Path(path)
    return p if p.is_absolute() else ROOT / p


def read(path):
    return json.loads(local(path).read_text(encoding="utf-8-sig"))


def sha(path):
    return hashlib.sha256(local(path).read_bytes()).hexdigest()


def binding(path):
    p = local(path).resolve(strict=True)
    value = {"path": p.relative_to(ROOT).as_posix(), "sha256": sha(p)}
    if p.suffix == ".json":
        document = read(p)
        if "evidence_id" in document:
            value["evidence_id"] = document["evidence_id"]
    return value


def check_binding(value, expected=None):
    require(sha(value["path"]) == value["sha256"], "Stale binding: " + value["path"])
    if expected is not None:
        require(local(value["path"]).resolve() == local(expected).resolve(), "Unexpected bound path")


def check_lists(value):
    for key in ("bindings", "source_bindings", "static_source_bindings", "dependencies"):
        entries = value.get(key, [])
        if isinstance(entries, dict):
            continue
        for entry in entries:
            check_binding(entry)


def batch(name, paths):
    comparison, audit = read(paths["comparison"]), read(paths["observations"])
    require(comparison["status"] == "pass" and audit["status"] == "PASS", name + " needs both passing reports")
    check_lists(comparison)
    check_lists(audit)
    for key, expected in (("capture", paths["capture"]), ("run_record", paths["run"]), ("probe_template", PROBE)):
        check_binding(comparison["provenance"][key], expected)
    for key, expected in (("cases", paths["cases"]), ("input_freeze", paths["input_freeze"]), ("comparator", COMPARATOR)):
        check_binding(comparison[key], expected)
    candidate = comparison["candidate"]
    check_binding(candidate["manifest"], FREEZE)
    require(candidate["current_files_match_freeze"] and candidate["frozen_candidate_required"], name + " must use the frozen candidate")
    audited = {local(b["path"]).resolve() for b in audit["bindings"]}
    for expected in (paths["capture"], paths["run"], paths["cases"], paths["input_freeze"], FREEZE, VERIFIER):
        require(local(expected).resolve() in audited, name + " independent audit misses " + expected)
    imports = comparison["imports"]
    require(len(imports) == audit["result"]["imports"] == len(read(paths["cases"])["cases"]), name + " import count mismatch")
    require(all(item["matched"] for item in imports), name + " import differences remain")
    handlers = [h["snapshot"] for item in imports for h in item["handlers"]]
    require(len(handlers) == comparison["coverage"]["complete_post_handler_snapshots"], name + " handler count differs from actual snapshots")
    selected = [h for h in handlers if h["id"] == 94]
    native = [h for h in audit["result"]["handlers"] if h["id"] == 94]
    observed = comparison["fstres_handlers"]
    require(Counter(h["status"] for h in selected) == Counter(h["status"] for h in native)
            == Counter(h["status"] for h in observed), name + " FSTRES status/count mismatch")
    readers = {key: value for key, value in comparison.items()
               if key.endswith("_comparison") and isinstance(value, dict) and "status" in value}
    require(all(value["status"] == "pass" for value in readers.values()), name + " reader differences remain")
    rejected, skipped = audit["tampering_rejected"], audit["tampering_not_applied"]
    require(rejected and len(rejected) == len(set(rejected)), name + " corruption checks missing or duplicated")
    evidence, run = read(paths["capture"]), read(paths["run"])
    cap = evidence["normalized_result"]
    require(cap["messages_truncated"] is False and cap["cleanup_error"] is None
            and cap["state"] == "detached" and cap["pid"] == run["pid"]
            and run["status"] == "completed" and run["exit_code"] == 0, name + " incomplete original run")
    require(all(m["type"] == "send" and m["payload"].get("operation") != "probe-error"
                for m in cap["messages"]), name + " invalid observer messages")
    lifecycle = comparison["fstres_lifecycle"]
    modes = Counter(h["mode"] for h in observed if "mode" in h and h["outer_status"] == 0)
    counts = {"imports": len(imports), "parsed_tokens": comparison["tokens"],
              "complete_handler_snapshots": len(handlers), "FSTRES_handlers": len(selected),
              "independently_audited_selected_handlers": len(audit["result"]["handlers"]),
              "GRNDAT_handlers": sum(h["id"] == 108 for h in audit["result"]["handlers"]),
              "failed_FSTRES_handlers": sum(h["status"] != 0 for h in selected),
              "FSTRES_line_reader_calls": comparison["fstres_reader_comparison"]["count"],
              "FSTRES_direct_payload_calls": len(lifecycle["payloads"]),
              "FSTRES_typed_resets": len(lifecycle["resets"]),
              "FSTRES_constructor_calls": len(lifecycle["constructors"]),
              "property_scalar_scan_calls": comparison["property_scalar_comparison"]["count"],
              "final_standard_descriptor_snapshots": comparison["descriptor_snapshots"],
              "independent_tamper_rejections": len(rejected), "independent_tamper_checks_skipped": len(skipped)}
    return {"counts": counts, "modes": dict(modes), "pid": cap["pid"], "messages": len(cap["messages"]),
            "reader_counts": {key: value["count"] for key, value in readers.items() if "count" in value},
            "skipped_corruption_checks": skipped,
            "bindings": [binding(path) for path in paths.values()]}


def build(diagnostics):
    freeze = read(FREEZE)
    require(len(freeze["files"]) == 35, "Expected 35 frozen runtime files")
    for entry in freeze["files"]:
        require(sha(entry["path"]) == sha(entry["archive"]) == entry["sha256"], "Runtime/archive changed: " + entry["path"])
    for path in STATIC:
        check_lists(read(path))
    cohorts = {name: batch(name, paths) for name, paths in BATCHES.items()}
    registration = read(REGISTRATION)
    require(registration["status"] == "pass" and registration["candidate_current_all_match"], "Regression registration is incomplete")
    for key in ("baseline_log", "registration_log", "cmake"):
        check_binding(registration[key])
    require(registration["existing_tests_passed_before_freeze"] == freeze["baseline_tests_passed"]
            and registration["new_tests_passed"] == len(registration["new_tests"]) == 4
            and registration["registered_tests"] == freeze["baseline_tests_passed"] + 4, "Regression counts mismatch")
    totals = Counter()
    modes = Counter()
    for cohort in cohorts.values():
        totals.update(cohort["counts"])
        modes.update(cohort["modes"])
    require(set(range(1, 17)) <= set(modes), "Not all positive mode branches were observed")
    require(0 in modes and any(m < 0 for m in modes) and any(m > 16 for m in modes), "Zero/negative/unknown positive routes missing")
    for path in diagnostics:
        diagnostic = read(path)
        check_lists(diagnostic)
        for attempt in diagnostic.get("attempts", []):
            check_binding(attempt["initial_run"])
            check_binding(attempt["retry_run"])
    return {"schema_version": 1, "status": "VERIFIED_BOUNDED_COMPLETE_FSTRES_IMPORT_FAMILY",
            "created_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
            "purpose": "Original-to-C++ import and storage behavior across the complete FSTRES dispatcher family.",
            "totals": dict(totals), "mode_counts": dict(modes), "batches": cohorts,
            "regressions": {"baseline_passed_before_freeze": freeze["baseline_tests_passed"],
                            "new_tests_passed": 4, "registered": registration["registered_tests"],
                            "single_full_suite_run_claimed": False},
            "correction_history": [{"artifact": binding(path), "status": read(path)["status"],
                                    "explanation": read(path).get("cause", read(path).get("scope", "See retained diagnostic."))}
                                   for path in diagnostics],
            "candidate_changed_after_freeze": False,
            "limitations": ["This establishes bounded import/state behavior, not constitutive stress-law evaluation or a complete application replacement.",
                            "Negative table counts normalize during allocation. Rank0 widths at or below1 preserve prior physical buffers and consume no payload; those documented cases are supported.",
                            "Derived count conversion and intermediate sum/product are limited to finite signed32-representable values without overflow. Arbitrary allocation failure and undefined scratch matching failures remain excluded.",
                            "Raw pointer addresses, unknown padding and native temporary leaks are not reproduced as a C++ ABI or ownership requirement."],
            "next_scope": binding("analysis/fstres-follow-on-scope.json"),
            "bindings": [binding(path) for path in [FREEZE, COMPARATOR, VERIFIER, PROBE, REGISTRATION,
                         *STATIC, *diagnostics, Path(__file__)]]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "analysis/fstres-contract.json")
    parser.add_argument("--diagnostic", action="append", default=[])
    args = parser.parse_args()
    result = build(args.diagnostic)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["totals"]))


if __name__ == "__main__":
    main()
