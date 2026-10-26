"""Freeze independently observed actual MO constructor contexts and stage guards."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-observed-composition-increment.json"
if TARGET.exists():
    raise SystemExit("Immutable composition receipt already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300.cpp",
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300_lifecycle.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_mo.cpp",
]
text = {path: (ROOT / path).read_text() for path in sources}
header, cpp, lifecycle_header, lifecycle_cpp, tests = [text[path] for path in sources]
checks = {
    "pressure_helper_declared": "pressureFieldsForStartupHeating(const StartupHeatingFeatures&);" in header,
    "pressure_helper_defined": "pressureFieldsForStartupHeating(const StartupHeatingFeatures& features)" in cpp,
    "composer_context_declared": "std::optional<MoInitialObservations> moObservations;" in lifecycle_header,
    "distinct_general_calls": "fillGeneralPage(0)" in lifecycle_cpp and "fillGeneralPage(1)" in lifecycle_cpp,
    "both_environment_fills_use_resolver": lifecycle_cpp.count("if (!fillEnvironmentPage())") == 2,
    "test_main": "int main() { return testkit::run_all(); }" in tests,
    "test_string_method_declared": "bool isEmpty() const" in (ROOT / "source/apps/moproj2mst/include/m2m/qt4.hpp").read_text(),
    "no_trailing_whitespace": all(line.rstrip() == line for value in text.values() for line in value.splitlines()),
}
if not all(checks.values()):
    raise SystemExit(f"Prebuild handoff check failed: {checks}")
receipt = {
    "status": "actual_mo_observed_composition_frozen_for_root_build",
    "sources": [record(path) for path in sources],
    "contracts": [record(path) for path in [
        "analysis/wave2-save-diffs-300-observed-composition-contract.json",
        "analysis/wave2-save-diffs-300-feature-context-increment.json",
        "analysis/wave2-save-diffs-300-pressure-context-contract.json",
        "analysis/wave2-save-diffs-300-backup-unit-repair.json",
    ]],
    "implementation": {
        "startup_input": "StartupHeatingFeatures contains optional<uint8_t>twoD,threeD; pressureFieldsForStartupHeating returns true for any knownnonzero, false for bothknownzero, unknown otherwise. It computes only the source-proven OR; it does not infer missing feature values or current-dimension aliases.",
        "context": "MoInitialObservations contains two optional MoFeatureAvailability General-fill sets plus independent startupHeating bytes; optional appended to InitialEditorContext. Actual procType5 requires observations; old processEnabled/pressure bool cannot supply missing actualMO answers. ProcType1 isolated helper profile stays unchanged.",
        "initial_general": "Resolve observation0 at the first original General fill, preserving original getter>0/OR semantics through verified builder. Missing set/query rejects before General's model side effects.",
        "environment": "Resolve startupHeating at each actual Environment fill after preceding pages. Missing aggregate rejects at that reached point, not in advance of all constructor effects. Known false is engaged and supported: fresh empty pressure edits remain Intermediate and later write0.",
        "backup_refill": "Resolve independent observation1 inside the existing backed-up refresh, after initial pageI/O and repeatedStopping/Meshing. Failure still reaches the existing field-awareSCrestore; it does not undo earlier constructor page effects. Source UNIT initialization repair remains intact.",
        "apply": "Save page helpers consume remembered form controls; they do not re-read current observation inputs. Shared root properties-to-model overwrite remains later and separate. Public300 guard stays false.",
    },
    "shared_interface": "No family/loader/common/model changes. Root eventual activation hook supplies project-bearing GeneralContext, actualMO StepCountContext, KnownC and MoInitialObservations. Two General observations are separate invocations; three HEATING answers inside each remain separate. Startup2D/3D bits are distinct evidence from all General availability results.",
    "tests": {
        "new_cases": [name for name in re.findall(r"TEST_CASE\(([^)]+)\)", tests) if name in {
            "mo_pressure_features_keep_unknown_distinct_and_combine_both_startup_dimensions",
            "mo_composer_remembers_independent_refill_observations_and_two_dimensional_pressure",
            "mo_composer_observed_zero_pressure_is_supported_and_keeps_fresh_empty_controls",
            "mo_composer_refuses_missing_observations_at_their_actual_constructor_stage"}],
        "mo_cases_count": len(re.findall(r"TEST_CASE\(([^)]+)\)", tests)),
        "existing_case_update": "Prior actualMO persistent-session case now supplies both original observed General sets and startup bytes. Existing assertion expectations remain; no fallback to assumed all-enabled state.",
        "method": "Pure startup unknown/zero/nonzero truth states; real GuiModel/Project/Operation, two distinct General sets,2D-only pressure in3D despiteGeneral3Dunavailable, source/input mutation afterOpen, zero-pressure fresh forms/apply0; smode mutation and NBC read count distinguish initial/Environment/refill refusal boundaries.",
        "cheap_handoff_checks": checks,
        "prebuild_review": "Verified actual EnvironmentPage scalars API and qt::String::isEmpty declaration; removed guessed nonexistent pressurePopulated/empty members before handoff. Existing NoFiles callback contract remains no filesystem writes, false reads, seven removal effects on completeSave.",
        "status": "not_run_worker_root_owns_build_and_execution",
        "prior_checkpoint": "Parent reports source-stable119 passing registered checks in frozenwave3-directories-colorbar; this adds four cases within existing MO executable, no new registration.",
    },
    "next": "Bounded remesh byte representation under original TokenReader4 overwrite contract, then native file Open/Save binding. RepeatedGeneral public activation/provider, original recovery/view/outerSave and guarded page profiles remain unfinished; no full application completion claim.",
    "limits": "No worker builds/tests, new private exports, original runtime, app control or fixture mutation for this implementation. Static evidence retained in prior pressure contract. No public env/ABI/default license policy changes.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "mo_cases": receipt["tests"]["mo_cases_count"], "checks": checks}))
