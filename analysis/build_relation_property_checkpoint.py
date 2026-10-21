"""Build the relation-property checkpoint only from completed, bound reports.

Preparing or importing this module does not write a checkpoint. Run explicitly
after both valid comparisons, independent audits and test registration finish.
No native or replacement process is executed by this documentation builder.
"""
import argparse
from collections import Counter
import datetime
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FREEZE = "analysis/relation-property-candidate-freeze.json"
COHORTS = {
    "training": {
        "capture": "analysis/relation-property-runtime-training-v2.json",
        "cases": "analysis/relation-property-evaluation-cases-v2.json",
        "input_freeze": "analysis/relation-property-v2-input-freeze.json",
        "run": "analysis/oracle/relation-property-training-b/run.json",
        "comparison": "analysis/relation-property-training-comparison.json",
        "observations": "analysis/relation-property-observations.json",
    },
    "held_out": {
        "capture": "analysis/relation-property-runtime-held-out.json",
        "cases": "analysis/relation-property-held-out-cases-v2.json",
        "input_freeze": "analysis/relation-property-held-out-input-freeze.json",
        "run": "analysis/oracle/relation-property-held-out/run.json",
        "comparison": "analysis/relation-property-held-out-comparison.json",
        "observations": "analysis/relation-property-held-out-observations.json",
    },
}
STATIC = [
    "analysis/relation-property-static-contract.json",
    "analysis/relation-property-byte-audit.json",
    "analysis/relation-property-model-inventory.json",
    "analysis/relation-property-implementation-review.json",
]
REPAIR = [
    "analysis/relation-property-probe-initial-failure.json",
    "analysis/relation-property-probe-v2-diagnostic.json",
    "analysis/relation-property-probe-v2-repair-audit.json",
]
TOOLS = [
    "tools/compare_relation_property_import.py",
    "tools/verify_relation_property_observations.py",
    "tools/observe_relation_property_import.js",
]
VERIFIER_DIAGNOSTIC = "analysis/relation-property-initial-verifier-diagnostic.json"
KEYWORDS = {135: "TTTD", 136: "TTTN", 137: "PHASLH", 138: "PHASVL",
            139: "TRNSFP", 140: "PHEQVF", 141: "COARSN", 142: "IPSNRG", 143: "VARSEL"}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def local(path):
    p = Path(path)
    return p if p.is_absolute() else ROOT / p


def read(path):
    return json.loads(local(path).read_text(encoding="utf-8-sig"))


def digest(path):
    return hashlib.sha256(local(path).read_bytes()).hexdigest()


def binding(path):
    p = local(path).resolve(strict=True)
    try:
        name = p.relative_to(ROOT).as_posix()
    except ValueError:
        name = p.as_posix()
    result = {"path": name, "sha256": digest(p)}
    if p.suffix == ".json":
        record = read(p)
        if "evidence_id" in record:
            result["evidence_id"] = record["evidence_id"]
    return result


def check_binding(record, expected=None):
    require(isinstance(record, dict) and "path" in record and "sha256" in record,
            "Missing explicit path/hash binding")
    require(digest(record["path"]) == record["sha256"], "Stale binding: " + record["path"])
    if expected is not None:
        require(local(record["path"]).resolve() == local(expected).resolve(),
                "Unexpected bound artifact: " + record["path"])


def check_report_bindings(report):
    """Check current report/tool/source lists without rewriting historical freezes."""
    for key in ("bindings", "dependencies", "static_source_bindings"):
        for item in report.get(key, []):
            check_binding(item)


def load_cohort(name, paths):
    comparison, audit = read(paths["comparison"]), read(paths["observations"])
    manifest, capture = read(paths["cases"]), read(paths["capture"])
    require(comparison["status"] == "pass" and audit["status"] == "PASS",
            name + " comparison and original-only audit must both pass")
    for key, expected in (("capture", paths["capture"]), ("run_record", paths["run"])):
        check_binding(comparison["provenance"][key], expected)
    check_binding(comparison["cases"], paths["cases"])
    check_binding(comparison["input_freeze"], paths["input_freeze"])
    check_binding(comparison["candidate"]["manifest"], FREEZE)
    check_binding(comparison["comparator"], TOOLS[0])
    candidate = comparison["candidate"]
    require(candidate["frozen_candidate_required"] is True
            and candidate["current_files_match_freeze"] is True,
            name + " must be an immediate strict frozen comparison")
    check_report_bindings(comparison)
    check_report_bindings(audit)
    audit_paths = {local(item["path"]).resolve() for item in audit["bindings"]}
    for path in (paths["capture"], paths["run"], paths["cases"], paths["input_freeze"], FREEZE, TOOLS[1]):
        require(local(path).resolve() in audit_paths, name + " audit misses required input " + path)
    result = audit["result"]
    imports = comparison["imports"]
    require(len(imports) == result["imports"] == len(manifest["cases"]),
            name + " import counts disagree")
    require(all(item["matched"] for item in imports), name + " has unmatched import")
    selected = [h for item in imports for h in item["handlers"] if h["snapshot"]["id"] in KEYWORDS]
    relation_handlers = [h for item in imports for h in item["handlers"]
                         if h["snapshot"]["id"] in {*KEYWORDS, 134}]
    require(len(relation_handlers) == len(result["handlers"]), name + " relation handler counts disagree")
    observed_counts = Counter(h["snapshot"]["id"] for h in selected)
    audited_counts = Counter(h["id"] for h in result["handlers"] if h["id"] in KEYWORDS)
    require(observed_counts == audited_counts, name + " audited keyword coverage disagrees")
    selected_failures = sum(h["snapshot"]["status"] != 0 for h in selected)
    relation_failures = sum(h["snapshot"]["status"] != 0 for h in relation_handlers)
    require(relation_failures == result["coverage"]["failures"], name + " failure counts disagree")
    rejections, skips = audit["tampering_rejected"], audit["tampering_not_applied"]
    require(isinstance(rejections, list) and rejections and len(set(rejections)) == len(rejections),
            name + " lacks actual independent corruption checks")
    lifecycle = comparison["relation_property_lifecycle"]
    readers = comparison["relation_property_reader_comparison"]
    require(readers["status"] == "pass", name + " relation reader replay failed")
    require(lifecycle["payload_count"] == len(lifecycle["payloads"]), "Payload count mismatch")
    require(capture["evidence_id"] == comparison["provenance"]["evidence_id"], "Evidence identity mismatch")
    cap = capture["normalized_result"]
    require(not cap.get("messages_truncated", False), "Truncated accepted capture")
    require(not any(m.get("type") == "error" or m.get("payload", {}).get("operation") == "probe-error"
                    for m in cap["messages"]), "Accepted capture contains observer errors")
    handler_count = sum(len(item["handlers"]) for item in imports)
    require(handler_count == comparison["coverage"]["complete_post_handler_snapshots"],
            name + " complete handler snapshot counts disagree")
    counts = {
        "imports": len(imports), "complete_handler_snapshots": handler_count,
        "selected_property_handlers": len(selected), "failed_selected_handlers": selected_failures,
        "relation_handlers_including_PHASTF": len(relation_handlers),
        "failed_relation_handlers_including_PHASTF": relation_failures,
        "relation_header_and_combined_reader_calls": readers["count"],
        "table_payload_calls": lifecycle["payload_count"], "typed_reset_calls": len(lifecycle["resets"]),
        "property_scalar_scan_calls": comparison["property_scalar_comparison"]["count"],
        "base_reader_calls": comparison["reader_comparison"]["count"],
        "final_standard_descriptor_snapshots": comparison["descriptor_snapshots"],
        "independent_tamper_rejections": len(rejections), "independent_tamper_checks_skipped": len(skips),
    }
    return {
        "counts": counts, "capture": binding(paths["capture"]),
        "keyword_counts": {KEYWORDS[k]: v for k, v in sorted(observed_counts.items())},
        "TTTD_modes": lifecycle["TTTD_modes"],
        "table_rank_width_coverage": sorted({(p["rank"], p["width"]) for p in lifecycle["payloads"]}),
        "expected_failure_cases": [{"file": item["file"], "id": h["snapshot"]["id"],
                                    "status": h["snapshot"]["status"]}
                                   for item in imports for h in item["handlers"]
                                   if h["snapshot"]["id"] in KEYWORDS and h["snapshot"]["status"] != 0],
        "skipped_corruption_checks": skips,
    }


def build(registration_path):
    freeze = read(FREEZE)
    require(len(freeze["files"]) == 33, "Expected the selected 33-file candidate")
    for entry in freeze["files"]:
        require(digest(entry["path"]) == digest(entry["archive"]) == entry["sha256"],
                "Candidate or frozen archive changed: " + entry["path"])
    check_binding(freeze["baseline_log"])
    check_binding(freeze["static_review"])
    cohorts = {name: load_cohort(name, paths) for name, paths in COHORTS.items()}
    combined_ids = set().union(*(c["keyword_counts"] for c in cohorts.values()))
    require(combined_ids == set(KEYWORDS.values()), "Not all nine relation keywords were observed")
    registration = read(registration_path)
    require(registration["status"] == "pass", "Two new regressions must pass")
    check_report_bindings(registration)
    for key in ("baseline_log", "registration_log", "cmake"):
        check_binding(registration[key])
    registered = registration["registered_tests"]
    baseline = freeze["baseline_tests_passed"]
    new_tests = len(registration["new_tests"])
    require(new_tests == registration["new_tests_passed"] == 2, "Expected two passing new tests")
    require(registration["existing_tests_passed_before_freeze"] == baseline, "Baseline count mismatch")
    require(registered == baseline + new_tests, "Regression registration count disagrees")
    require(registration["candidate_current_all_match"] is True, "Registration changed runtime files")
    initial, diagnostic, repair = (read(path) for path in REPAIR)
    require(initial["status"].startswith("invalid_"), "Invalid initial capture must remain classified invalid")
    require(repair["status"].startswith("PASS"), "V2 observer repair audit must pass")
    check_binding(initial["capture"])
    check_binding(diagnostic["original_probe"])
    check_binding(diagnostic["corrected_probe"], TOOLS[2])
    check_report_bindings(repair)
    verifier_diagnostic = read(VERIFIER_DIAGNOSTIC)
    check_binding(verifier_diagnostic["capture"], COHORTS["training"]["capture"])
    check_binding(verifier_diagnostic["verifier_archive"])
    counters = Counter()
    for cohort in cohorts.values():
        counters.update(cohort["counts"])
    counters.update(baseline_tests_passed=baseline, new_tests_passed=new_tests,
                    tests_registered=registered, frozen_runtime_files_unchanged=len(freeze["files"]))
    paths = [FREEZE, *STATIC, *REPAIR, *TOOLS, VERIFIER_DIAGNOSTIC, str(registration_path),
             "analysis/relation-property-follow-on-plan.json", str(Path(__file__))]
    for cohort in COHORTS.values():
        paths.extend(cohort.values())
    return {
        "schema_version": 1,
        "created_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "status": "selected_relation_property_import_profile_verified_with_independent_captures",
        "target": binding("source/DEF_PRE_64.exe"),
        "implemented_keywords": [{"id": k, "name": v} for k, v in KEYWORDS.items()],
        "behavior": [
            "All nine property routes and observed TTTD modes preserve native mutation order, status and stream position, including partial or failed reads.",
            "Relation state includes twelve owned tables, all explicitly initialized scalar fields, name and ordered keys. This is an explicit field model, not an invented native byte image.",
            "Table storage remains separate from logical metadata. Rank-zero vector short-circuit and TRNSFP scalar changes can retain physical arrays; staged dependent payloads commit only after completed reads.",
            "Typed reset preserves table metadata54/60; TTTD and COARSN live-output failures remain visible. These property edits do not imply PHASTF recount or component refresh.",
            "Primary and linked root lifecycles, existing materials/objects and full selected post-handler state are compared within the declared captured domain.",
        ],
        "verification": dict(counters), "cohorts": cohorts,
        "validation_order": (
            f"Release baseline of {baseline} tests passed before the 33-file freeze. Valid training and held-out reports "
            f"enforce that same unchanged candidate. Then {new_tests} newly registered regressions passed; this is "
            f"{baseline}+{new_tests}, not one {registered}-test invocation. Refer to individual comparison/audit records for checker development history."
        ),
        "correction_history": [
            {"kind": "observer_failure", "record": binding(REPAIR[0]),
             "statement": "The initial original run completed but its probe produced reset-continuation errors. It remains preserved and invalid for comparison; no candidate defect follows from it."},
            {"kind": "observer_repair", "record": binding(REPAIR[1]), "audit": binding(REPAIR[2]),
             "statement": "The repaired observer tracks the common reset join while retaining the actual call-return identity. V2 input manifests/freezes preserve the observation change separately from the candidate."},
            {"kind": "independent_verifier_correction", "record": binding(VERIFIER_DIAGNOSTIC),
             "statement": "The initial independent verifier omitted two initialized int32 constructor stores in its override switch. The existing retained source established those stores; the verifier was corrected before both final original-only audits. The initial diagnostic and verifier are preserved. This was a verifier omission, not a candidate mismatch."},
            {"kind": "candidate_identity", "record": binding(FREEZE),
             "statement": "All 33 runtime files and their archived originals match at checkpoint creation. No C++ correction was required between the frozen candidate and the accepted captures."},
        ],
        "source_bindings": [binding(p) for p in dict.fromkeys(paths)],
        "limits": [
            "This verifies selected import behavior, not the complete original application or scientific relation evaluation/interpolation.",
            "Native padding, uninitialized name tails, external shared ownership, allocation failure, nonfinite inputs and unsafe extents are not fabricated.",
            "Native temporary-buffer leakage on EOF is recorded; owned C++ cleanup is an intentional resource-lifetime boundary, not byte-for-byte allocator equivalence.",
            "Captured cases establish their reported ranks, widths and error paths. They do not prove all possible numeric rounding or arbitrary property combinations.",
        ],
        "next_recovery": {
            "plan": binding("analysis/relation-property-follow-on-plan.json"),
            "proposal": "Recover nested FSTRES94 mode14 and GRNDAT108 selector4 constructor/ownership contracts, then validate outer failure semantics and GRNDAT refresh before broader scientific consumers.",
        },
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--registration", default="analysis/relation-property-test-registration.json")
    parser.add_argument("--output", default="analysis/relation-property-contract.json")
    args = parser.parse_args()
    result = build(args.registration)
    output = local(args.output)
    # Write only after every required report, binding and candidate check passes.
    output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"output": binding(output), "verification": result["verification"]}, indent=2))


if __name__ == "__main__":
    main()
