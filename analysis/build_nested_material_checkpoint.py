"""Build the selected nested-material checkpoint from completed bound reports.

This tool executes neither original nor replacement binaries. It writes only
after the comparisons, independent audits, registration and hashes validate.
"""
import argparse
from collections import Counter
import datetime
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FREEZE = "analysis/nested-material-candidate-freeze.json"
COMPARATOR = "tools/compare_nested_material_import.py"
VERIFIER = "tools/verify_nested_material_observations.py"
PROBE = "tools/observe_nested_material_import.js"
DIAGNOSTIC = "analysis/nested-material-initial-verifier-diagnostic.json"
REGISTRATION = "analysis/nested-material-test-registration.json"
STATIC = ["analysis/nested-material-static-contract.json",
          "analysis/nested-material-byte-audit.json",
          "analysis/nested-material-model-inventory.json",
          "analysis/nested-material-implementation-review.json",
          "analysis/nested-material-precapture-audit.json",
          "analysis/nested-material-held-out-precapture-audit.json"]
COHORTS = {
    name: {
        "capture": f"analysis/nested-material-runtime-{suffix}.json",
        "cases": "analysis/nested-material-evaluation-cases.json" if name == "training"
                 else "analysis/nested-material-held-out-cases.json",
        "input_freeze": "analysis/nested-material-input-freeze.json" if name == "training"
                        else "analysis/nested-material-held-out-input-freeze.json",
        "run": f"analysis/oracle/nested-material-{suffix}/run.json",
        "comparison": f"analysis/nested-material-{suffix}-comparison.json",
        "observations": "analysis/nested-material-observations.json" if name == "training"
                        else "analysis/nested-material-held-out-observations.json",
    }
    for name, suffix in (("training", "training"), ("held_out", "held-out"))
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def local(path):
    path = Path(path)
    return path if path.is_absolute() else ROOT / path


def read(path):
    return json.loads(local(path).read_text(encoding="utf-8-sig"))


def sha(path):
    return hashlib.sha256(local(path).read_bytes()).hexdigest()


def binding(path):
    path = local(path).resolve(strict=True)
    result = {"path": path.relative_to(ROOT).as_posix(), "sha256": sha(path)}
    if path.suffix == ".json":
        value = read(path)
        if "evidence_id" in value:
            result["evidence_id"] = value["evidence_id"]
    return result


def validate_binding(value, expected=None):
    require(isinstance(value, dict) and "path" in value and "sha256" in value,
            "Expected explicit path and SHA256 binding")
    require(sha(value["path"]) == value["sha256"], "Stale binding: " + value["path"])
    if expected is not None:
        require(local(value["path"]).resolve() == local(expected).resolve(),
                "Unexpected artifact: " + value["path"])


def validate_lists(value):
    for key in ("source_bindings", "static_source_bindings", "bindings", "dependencies"):
        items = value.get(key, [])
        # The model inventory uses a dependency narrative object, while audit
        # reports use this key for an explicit list of tool hash bindings.
        if key == "dependencies" and isinstance(items, dict):
            continue
        for item in items:
            validate_binding(item)


def cohort(name, paths):
    comparison, audit = read(paths["comparison"]), read(paths["observations"])
    require(comparison["status"] == "pass" and audit["status"] == "PASS",
            name + " needs passing comparison and independent audit")
    validate_lists(comparison)
    validate_lists(audit)
    for key, expected in (("capture", paths["capture"]), ("run_record", paths["run"]),
                          ("probe_template", PROBE)):
        validate_binding(comparison["provenance"][key], expected)
    for key, expected in (("cases", paths["cases"]), ("input_freeze", paths["input_freeze"]),
                          ("comparator", COMPARATOR)):
        validate_binding(comparison[key], expected)
    validate_binding(comparison["candidate"]["manifest"], FREEZE)
    require(comparison["candidate"]["current_files_match_freeze"] is True
            and comparison["candidate"]["frozen_candidate_required"] is True,
            name + " must be a strict frozen comparison")
    audited_paths = {local(b["path"]).resolve() for b in audit["bindings"]}
    for path in (paths["capture"], paths["run"], paths["cases"], paths["input_freeze"], FREEZE, VERIFIER):
        require(local(path).resolve() in audited_paths, name + " audit misses " + path)
    imports = comparison["imports"]
    require(len(imports) == audit["result"]["imports"] == len(read(paths["cases"])["cases"]),
            name + " import counts disagree")
    require(all(item["matched"] for item in imports), name + " unmatched imports")
    handlers = [h["snapshot"] for item in imports for h in item["handlers"]]
    require(len(handlers) == comparison["coverage"]["complete_post_handler_snapshots"],
            name + " complete handler count must come from actual snapshots, not tokens")
    selected = [h for h in handlers if h["id"] in (94, 108)]
    observations = comparison["nested_material_handlers"]
    native = audit["result"]["handlers"]
    require(len(selected) == len(observations) == len(native), name + " nested handler counts disagree")
    require(Counter((h["id"], h["status"]) for h in selected)
            == Counter((h["id"], h["status"]) for h in native)
            == Counter((h["id"], h["status"]) for h in observations),
            name + " native selected ID/status counts disagree")
    failed = sum(h["status"] != 0 for h in selected)
    require(failed == audit["result"]["coverage"]["failures"], name + " native failure count mismatch")
    ignored_failures = sum(h.get("coefficient_status") == 1 and h["status"] == 0 for h in observations)
    refresh_failures = sum(h["id"] == 108 and h.get("property_status") == 1
                           and 0 in h.get("refresh_options", []) for h in observations)
    require(ignored_failures == audit["result"]["coverage"]["ignored_failed_coefficients"],
            name + " ignored coefficient failure coverage mismatch")
    require(refresh_failures == audit["result"]["coverage"]["GRNDAT_failed_table_with_refresh"],
            name + " GRNDAT failed-table refresh coverage mismatch")
    operations = comparison["original_operations"]
    require(audit["result"]["refreshes"] == operations.get("material-refresh-after", 0),
            name + " refresh count mismatch")
    require(audit["result"]["disabled_feature53_queries"] == operations.get("nested-material-feature", 0),
            name + " disabled feature-query count mismatch")
    rejected, skipped = audit["tampering_rejected"], audit["tampering_not_applied"]
    require(rejected and len(rejected) == len(set(rejected)), "Independent checks missing or duplicated")
    for key in ("nested_material_reader_comparison", "reader_comparison", "property_reader_comparison",
                "property_scalar_comparison"):
        require(comparison[key]["status"] == "pass", name + " reader replay failed: " + key)
    evidence, run = read(paths["capture"]), read(paths["run"])
    cap = evidence["normalized_result"]
    require(cap["messages_truncated"] is False and cap["cleanup_error"] is None
            and cap["state"] == "detached" and cap["pid"] == run["pid"]
            and run["status"] == "completed" and run["exit_code"] == 0,
            name + " capture/run lifecycle incomplete")
    require(all(m["type"] == "send" and m["payload"].get("operation") != "probe-error"
                for m in cap["messages"]), name + " observer error")
    wrappers = [row for row in comparison["property_lifecycle"]["calls"] if row["id"] in (94, 108)]
    counts = {
        "imports": len(imports), "parsed_tokens": comparison["tokens"],
        "complete_handler_snapshots": len(handlers), "selected_nested_handlers": len(selected),
        "failed_selected_handlers": failed, "nested_line_reader_calls": comparison["nested_material_reader_comparison"]["count"],
        "nested_property_wrapper_calls": len(wrappers),
        "property_header_and_coarse_reader_calls": comparison["property_reader_comparison"]["count"],
        "property_scalar_scan_calls": comparison["property_scalar_comparison"]["count"],
        "base_reader_calls": comparison["reader_comparison"]["count"],
        "final_standard_descriptor_snapshots": comparison["descriptor_snapshots"],
        "ignored_failed_coefficients_followed_by_success": ignored_failures,
        "GRNDAT_failed_table_followed_by_refresh": refresh_failures,
        "material_refreshes": audit["result"]["refreshes"],
        "disabled_feature53_queries": audit["result"]["disabled_feature53_queries"],
        "independent_tamper_rejections": len(rejected), "independent_tamper_checks_skipped": len(skipped),
    }
    return {"counts": counts, "capture": binding(paths["capture"]),
            "messages": len(cap["messages"]), "pid": cap["pid"],
            "keyword_counts": dict(Counter(h["id"] for h in selected)),
            "id_dimensions": audit["result"]["coverage"]["id_dimensions"],
            "property_selectors": sorted({r["selector"] for r in wrappers}),
            "failed_cases": [{"file": h["file"], "id": h["id"], "status": h["status"]}
                             for h in observations if h["status"] != 0],
            "skipped_corruption_checks": skipped}


def build():
    freeze = read(FREEZE)
    require(len(freeze["files"]) == 33, "Expected selected 33-file candidate")
    for entry in freeze["files"]:
        require(sha(entry["path"]) == sha(entry["archive"]) == entry["sha256"],
                "Runtime or archive changed: " + entry["path"])
    validate_binding(freeze["baseline_log"])
    validate_binding(freeze["static_review"])
    for path in STATIC:
        validate_lists(read(path))
    cohorts = {name: cohort(name, paths) for name, paths in COHORTS.items()}
    registration = read(REGISTRATION)
    require(registration["status"] == "pass" and registration["candidate_current_all_match"] is True,
            "Regression registration did not pass unchanged")
    for key in ("baseline_log", "registration_log", "cmake"):
        validate_binding(registration[key])
    baseline = freeze["baseline_tests_passed"]
    require(registration["existing_tests_passed_before_freeze"] == baseline
            and registration["new_tests_passed"] == len(registration["new_tests"]) == 2
            and registration["registered_tests"] == baseline + 2, "Regression counts disagree")
    diagnostic = read(DIAGNOSTIC)
    for key in ("capture", "verifier", "initial_training_receipt"):
        validate_binding(diagnostic[key])
    totals = Counter()
    for value in cohorts.values():
        totals.update(value["counts"])
    totals.update(baseline_tests_passed=baseline, new_tests_passed=2,
                  tests_registered=registration["registered_tests"], frozen_runtime_files_unchanged=33)
    paths = [FREEZE, COMPARATOR, VERIFIER, PROBE, DIAGNOSTIC, REGISTRATION, *STATIC,
             "analysis/nested-material-follow-on-plan.json", Path(__file__).resolve()]
    for paths_for_cohort in COHORTS.values():
        paths.extend(paths_for_cohort.values())
    return {
        "schema_version": 1, "created_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "status": "selected_nested_material_import_profile_verified",
        "target": binding("source/DEF_PRE_64.exe"),
        "implemented_profile": {"FSTRES": {"id": 94, "outer_mode": 14},
                                "GRNDAT": {"id": 108, "outer_selector": 4}, "root_0x864": 0},
        "behavior": [
            "Constructor-derived FSTRES mode 1, four zero coefficients and scalar table value 1; GRNDAT selector 0, secondary 0 and empty rank-two table with selectors 3/8 are preserved until imported.",
            "Material selection and missing-record creation precede outer headers. Outer input failures preserve selected fields after any creation; unrelated valid outer branches remain explicit unsupported outcomes.",
            "FSTRES mode 14 stores mode 11 and initializes four live coefficients. Its coefficient-helper status is overwritten by the subsequent property reader, including observed failure followed by success.",
            "GRNDAT selector 4 preserves secondary field 0x17ec, imports table 0x1740 and refreshes components even after table failure. Creation option 1 and later preserving option 0 refreshes remain distinct.",
            "The original disabled grain-feature gate is observed in primary and linked roots and query 53 calls. Enabled scientific component widths are not inferred.",
            "Both nested tables, all selected material fields, physical buffers, descriptors, meshes and retained relation/custom state are compared at the reported full snapshots.",
        ],
        "verification": dict(totals), "cohorts": cohorts,
        "validation_order": f"{baseline} baseline tests passed before the unchanged 33-file freeze; both captured cohorts passed strict comparison. Two new regressions then passed, giving {baseline + 2} registered tests. This is {baseline}+2, not a claim of one combined test invocation.",
        "development_history": [
            {"kind": "unchanged_candidate_and_observer", "statement": "Both native comparisons passed on their first attempts. No C++ or probe repair was required in this milestone. Candidate files and their archives are unchanged at checkpoint creation."},
            {"kind": "independent_verifier_scope_correction", "record": binding(DIAGNOSTIC),
             "statement": "The initial independent verifier reused a historical helper lexer that excluded an incomplete sign after an assigned coefficient. The local verifier was corrected using the already recovered finite decimal grammar, with its original script and training receipt preserved. Both final audits bind the same current verifier; no candidate or native mismatch is implied."},
        ],
        "source_bindings": [binding(path) for path in dict.fromkeys(paths)],
        "limits": [
            "Only FSTRES mode 14 and GRNDAT selector 4 are recovered here; neither complete keyword family nor the complete application is claimed.",
            "The explicit profile with root 0x864 disabled excludes enabled grain-dependent scientific widths, interpolation, constitutive evaluation and simulation behavior.",
            "Native padding, shared external table ownership, allocation failure, nonfinite values and undefined scratch reads are not fabricated. Owned cleanup does not reproduce native temporary leaks.",
            "Exact captured floating bytes and stream semantics establish the tested finite domain, not every legacy decimal rounding case.",
        ],
        "next_recovery": {"plan": binding("analysis/nested-material-follow-on-plan.json"),
                          "scope": "Complete bounded FSTRES94 import family: all modes, ten table owners, 42 initialized doubles, derived vector extents, failure sequencing and lifecycle."},
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", default="analysis/nested-material-contract.json")
    args = parser.parse_args()
    result = build()
    output = local(args.output)
    output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"output": binding(output), "verification": result["verification"]}, indent=2))


if __name__ == "__main__":
    main()
