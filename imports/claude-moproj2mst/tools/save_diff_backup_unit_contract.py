"""Retain the bounded editor backup initialization repair without overwriting evidence."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-backup-unit-repair.json"
if TARGET.exists():
    raise SystemExit("Immutable repair receipt already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


source = "source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp"
test = "source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp"
native = [
    "source/port/defmodel/src/step/sim_ctrl.cpp",
    "source/port/defmodel/src/linked/dynary_lifecycle.cpp",
    "source/port/defmodel/src/table/dynary_template.cpp",
    "source/port/defmodel/src/layout/cls_1403753f0.cpp",
    "source/port/defmodel/src/llobj/dynary_table.cpp",
    "source/port/defmodel/src/members/furnace.cpp",
    "source/port/defmodel/src/members/ext_deps_wiring.cpp",
    "source/port/defrt/include/defrt/ftn/units.hpp",
    "source/port/defrt/src/ftn/units.cpp",
    "source/port/defcmd/src/p8_08/model_copy.cpp",
    "source/port/defmodel/src/window/inter_object_ops.cpp",
]
cpp = (ROOT / source).read_text()
tests = (ROOT / test).read_text()
checks = {
    "unit_before_owner_init_before_copy": cpp.index("backup.unit = sc.unit;")
        < cpp.index("sim_ctrl_set_owner(backup") < cpp.index("sim_ctrl_copy(&backup"),
    "declaring_init_header": "#include <defmodel/step/db_step.hpp>" in cpp,
    "declaring_copy_header": "#include <defcmd/interp.hpp>" in cpp,
    "test_main": "int main() { return testkit::run_all(); }" in tests,
    "both_unit_profiles": "for (const int unit : {1, 2})" in tests,
}
if not all(checks.values()):
    raise SystemExit(f"Prebuild handoff check failed: {checks}")
receipt = {
    "status": "initialized_backup_unit_repair_frozen_for_root_build",
    "cause": "Automatic SimControl backup had indeterminate UNIT. Native sim_ctrl_set_owner calls sim_ctrl_init_u3a8, which reads UNIT twice before sim_ctrl_copy overwrites the backup's copied fields. Passing tests cannot establish absence of this C++ undefined behavior.",
    "repair": "Assign backup.unit = sc.unit immediately after construction and before owner/init. Existing fillGeneral has already required source UNIT1 or2 on this supported path. Retain all field-aware deep copy, repeated fill and restore ordering.",
    "original_qualification": "Retained GUI140ecb340 calls GUI1408d3bc0 constructor, GUI1408d3cd0 owner/init, then GUI1408df7b0 copy. That selection does not establish the original stack UNIT value. This repair supplies defined adapter/native initialization; it is not a claim that original GUI constructor bytes or incidental stack contents match it. Shared native model sources remain untouched.",
    "sources": [record(source), record(test)],
    "evidence": [record(path) for path in native] + [
        record("analysis/wave2-save-diffs-300-support-140ecb340.c"),
        record("analysis/wave2-save-diffs-300-lifecycle-increment.json"),
        record("analysis/wave2-save-diffs-300-lifecycle-compile-repair.json"),
        record("analysis/wave2-save-diffs-300-mo-fixture-repair.json"),
    ],
    "bounded_initialization_audit": {
        "owner": "Constructor assigns owner=null; set_owner installs actual already initialized 3D DB_STEP_DATA before all dimension queries.",
        "scalar_reads": "Names/step counts/limits/iteration/misc init write their target fields before dependent use. update_u240 reads DSMAX and DTTYPE.ndim after their assignments. FRQNCY dimensions are assigned before multiplication/allocation. Environment init's UNIT branch is the only preexisting SimControl scalar dependency identified before deep copy.",
        "owned_members": "DYNARY constructor sets counts, arrays and owned pointer before dynary_init/free; templates set shape before calloc. Furnace constructor initializes list head/tail/count and name bytes before clear/setup; newly appended DYNARY nodes are constructed, and shape/value allocations are initialized before default fill/conversion. Furnace conversion's unit singleton is inline UnitGlobals g_units{} and has defined initialization, separate from backup UNIT.",
        "untouched_storage": "DYNARY padding+5c/+70 and 3D-excluded 2D-only SimControl fields are not initialized wholesale, and are not read by this bounded ctor/init path. No memset/whole-struct assignment introduced. No claim of every native model field or arbitrary later operation being initialized.",
        "copy_boundary": "First sim_ctrl_copy reads destination ownership, constructed table storage and source fields; destination scalars are stores. Native dynary_copy/alloc only read constructed destination arrays/counts/shape; original field-aware omissions remain. Source model completeness remains the existing loaded-state/profile requirement.",
    },
    "test": {
        "new_case": "initial_editor_backup_initialization_preserves_both_loaded_unit_profiles",
        "method": "UNIT1 and2 each Open with distinct loaded environment values, verify loaded values/deep temporary snapshots survive backup initialization, mutate source UNIT/curve values, then Apply and verify remembered UNIT and six-significant-digit form conversion. Ordinary assertions do not detect all undefined reads; source ordering audit establishes repair.",
        "lifecycle_cases": len(re.findall(r"TEST_CASE\(([^)]+)\)", tests)),
        "status": "not_run_worker_root_owns_build_and_execution",
        "root_reported_prior": "Full118 CTests passed before this repair; parent retained v2 results, not evidence that the UB was harmless.",
        "cheap_handoff_checks": checks,
    },
    "scope": "Only composer/test plus this receipt tool changed. No builds/runtime/static exports/fixture writes. Public300 Save guard and m2m ABI remain unchanged; full lifecycle recovery remains incomplete.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "checks": checks}))
