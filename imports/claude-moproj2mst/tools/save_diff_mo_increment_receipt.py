"""Capture actual MO editor300 General/StepCount and cheap source handoff checks."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_mo.cpp",
]
evidence = [
    "analysis/wave2-save-diffs-300-mo-route-reconciliation.json",
    "analysis/wave2-save-diffs-300-mo-control-bindings.json",
    "analysis/wave2-save-diffs-300-fill-140eb9630.c",
    "analysis/wave2-save-diffs-300-apply-140ebaa30.c",
    "analysis/wave2-save-diffs-300-fill-140ebb110.c",
    "analysis/wave2-save-diffs-300-apply-140ebb770.c",
    "analysis/wave2-save-diffs-300-setupui.c",
    "analysis/wave2-doe-type2-session-read-failures-decompile.json",
    "source/apps/moproj2mst/src/model/registry_rescue.cpp",
    "source/apps/moproj2mst/src/model/project.cpp",
    "source/apps/moproj2mst/src/model/opinfo.cpp",
    "source/apps/moproj2mst/src/model/objects.cpp",
    "source/apps/moproj2mst/include/m2m/model.hpp",
    "source/apps/moproj2mst/include/m2m/mst/writer.hpp",
    "analysis/wave2-save-diffs-300-lifecycle-increment.json",
    "analysis/wave2-save-diffs-300-lifecycle-compile-repair.json",
]
test = (ROOT / sources[-1]).read_text()
cpp = (ROOT / sources[1]).read_text()
model = (ROOT / "source/apps/moproj2mst/include/m2m/model.hpp").read_text()
writer = (ROOT / "source/apps/moproj2mst/include/m2m/mst/writer.hpp").read_text()
cases = re.findall(r"TEST_CASE\(([^)]+)\)", test)
checks = {
    "model_declarations": all(name in model for name in ("opBase(const Operation&", "startsFromKey(const Operation&", "activeSubProject(const Project&")),
    "class_virtual_declaration": "virtual bool writesDimNstart() const;" in writer,
    "source_has_declaring_includes": '#include "m2m/engine.hpp"' in cpp and '#include "m2m/mst/writer.hpp"' in cpp,
    "engine_includes_model_header": '#include "m2m/model.hpp"' in (ROOT / "source/apps/moproj2mst/include/m2m/engine.hpp").read_text(),
    "new_test_main": bool(re.search(r'int main\(\).*testkit::run_all', test)),
    "project_destruction_class_complete": '#include "m2m/mst/writer.hpp"' in test,
    "fixture_asserts_real_registry_resolution": "REQUIRE(opBase(*operation) != nullptr)" in test,
    "owned_trailing_whitespace": not any(line.rstrip() != line for path in sources for line in (ROOT / path).read_text().splitlines()),
}
assert all(checks.values()), checks
receipt = {
    "status": "actual_mo_procType5_general_stepcount_implemented_public_guard_retained",
    "sources": [record(path) for path in sources],
    "evidence": [record(path) for path in evidence],
    "route": "procType5 is actual MO, separately from SubProject.type2/3 DOE/optimisation. Prior procType1 helpers/composer tests remain valid only as their isolated guarded profile; they were not actual MO lifecycle acceptance.",
    "general": {
        "context": "generalContextForMo(const Project&,dimension,GeneralCapabilityState{trans3Enabled,trans4Trans5HeatEnabled}) -> procType5 context. Answers represent exact original feature/control results, not inferred license defaults.",
        "enable_mapping": "Final six control enables {true,true,x,y,y,y}: x=1412fae10 OR1412fab10; y=1412faeb0. The earlier TRANS4 constant1412fae90 assignment is overwritten at fill end. Process parent groups60/80 are distinct from units group40.",
        "mesh": "MO hides mesh spin158, leaves fresh setupUi value1, and General Save's visibility check never writes model meshno. Existing procType1 clamp/store remains unchanged.",
        "override": "Remember actual Project pointer. At Save call existing activeSubProject1400a7390 and preserve TRANS/HTMTHD iff actual active type2 or3, then retain later original heat-method/shared-combo behavior. This is live state, not Open's remembered false flag.",
    },
    "step_count": {
        "context": "StepCountContext procType5 requires explicit optional MoStepCountContext{Operation*}; null operation is a source-supported branch, missing context still refuses. Existing two-field procType1 aggregates and rejection tests stay intact.",
        "class": "Fill and Apply both call opBase(actual operation)->writesDimNstart(), null class=false. Existing opBase140054e90 reads Project.registry[classKey]; it does not use the cached Operation.opClass field. No class-support boolean is frozen or guessed.",
        "fill": "Head/null operation uses SC source and fresh unchecked checkbox. Nonhead supporting class reads actual nstartGiven; checked state selects operation.startStepNo. Non-supporting/missing class unchecks. If selected source>0, negate SC.nstart+1 with original32-bit wrap, even when selection came from different operation value. Preserve original spin ranges and qualified primary-die list.",
        "apply_order": "Write SC.nstart first. Re-evaluate live startsFromKey (actual flow predecessor OR restartGiven) and class vt+200: head forces operation given1/value; nonhead unsupported clears flag only; supported writes remembered checkbox and value only if checked. Then SC.nstep,stpinc,pdie. Unwritten operation.startStepNo remains current.",
    },
    "shared_interface": "No family.cpp/loader_l3/shared model edits. Existing persistent composer accepts new contexts directly. Root eventual hook can build General context from explicit original capabilities and actual Project, StepCount context{5,qualified die list,MoStepCountContext{&op}}. No ABI/flag changes.",
    "phase_boundary": "Page-state effects precede root's source-verified shared Save chain: shown Apply -> propsLoad -> propsToEngine, while index0 propsToEngine executes twice. Tests here assert page-apply state and operation effects; they are not claims of final engine state after properties overwrite or full Save output parity.",
    "tests": {"status": "not_run_worker_root_owns_build_and_execution", "new_cases": cases, "new_cases_count": len(cases),
        "prior_validation": "Root reports wave3-ht-stepdata candidate115 CTests/composer6 passed; previous editor helper count54, now expected61",
        "expected_editor_helper_cases": 54 + len(cases),
        "fixture_review": "Real Project/SubProject/FlowItem head and predecessor state is used. Registry-backed test classes assert opBase and actual virtual support before helper calls. Negative-source/positive-override fixture distinguishes original SC negation from tempting override negation. Typed absent-file I/O has openfalse/remove true and no real filesystem writes.",
        "handoff_checks": checks, "checks_limit": "Cheap declaration/main/fixture/whitespace inspection, not compilation or tests."},
    "efficiency_review": {
        "checkpoint": "Safe source boundary after Extra3D/native backend/composer checkpoints; source freeze respected during root builds.",
        "measured_events": "One prior lifecycle build failed for missing dynary_copy declaration; separate compile repair receipt retained. Current prebuild fixture review caught cached-opClass assumption before execution and replaced it with verified registry ownership/assertions.",
        "adopted_rule": "For each new helper call check declaring header and for each new test executable check main; inspect reused source helper return/resolution contract instead of relying on field names or comments.",
        "limits": "Model token usage and comparable wall-time baseline unavailable. No speed/token savings claim; no new private exports/runtime during this increment.",
    },
    "remaining": ["General feature result provider and pressure aggregate source", "Remesh actual project Open/Save file lifecycle", "Recovery UI empty guards and populated records", "Intermediate view and outer widget/root-storage effects", "Existing unsupported numeric/curve/spring profiles and independent original comparison"],
    "qualification": "Public mode300 Save still refuses; no completion claim. Existing mode0 unchanged. No worker builds/tests, original runtime/app control, captures or fixture mutation.",
}
target = ROOT / "analysis/wave2-save-diffs-300-mo-increment.json"
target.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(target.relative_to(ROOT)), "sha256": hashlib.sha256(target.read_bytes()).hexdigest(), "new_cases": len(cases), "checks": checks}))
