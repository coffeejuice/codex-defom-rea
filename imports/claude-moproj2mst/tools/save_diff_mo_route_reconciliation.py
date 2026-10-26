"""Retain the procType5 MO versus SubProject.type2 DOE correction."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
target = ROOT / "analysis/wave2-save-diffs-300-mo-route-reconciliation.json"
if target.exists():
    raise SystemExit("Immutable route reconciliation already retained.")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


evidence = [
    "analysis/wave2-save-diffs-300-fill-140eb9630.c",
    "analysis/wave2-save-diffs-300-apply-140ebaa30.c",
    "analysis/wave2-save-diffs-300-fill-140ebb110.c",
    "analysis/wave2-save-diffs-300-apply-140ebb770.c",
    "analysis/wave2-doe-type2-session-read-failures-decompile.json",
    "source/apps/moproj2mst/src/engine/cogging_save.cpp",
    "source/apps/moproj2mst/src/engine/db_load.cpp",
    "source/apps/moproj2mst/src/model/project.cpp",
    "source/apps/moproj2mst/include/m2m/model.hpp",
    "analysis/wave2-save-diffs-300-lifecycle-increment.json",
    "analysis/wave2-save-diffs-300-lifecycle-compile-repair.json",
]
receipt = {
    "status": "material_route_correction_retained_actual_mo_branches_queued_after_root_build",
    "correction": "applicationMode/procType5 from1402cb7a0 is the MO application path, not DOE. Existing helper context1 is a guarded isolated PRE-style profile and must not be presented as actual MO initial-editor lifecycle. DOE/optimisation are separately active SubProject.type2/3.",
    "qualification": "Original retained fill/apply decompiles prove distinct procType5 branches. The identification of5 as DEF_GUI_MO is already used throughout inherited converter source (baseAfterSave and DB load comments); no new original runtime or binary capture was performed. Raw branch bodies remain authoritative and will be ported rather than merely relabelled.",
    "general_mode5": [
        "General fill hides mesh number label/widget ui+150/+158 instead of setting the spin from SC.meshno; Save tests widget visible AND enabled and therefore does not assign SC.meshno on this branch.",
        "General simulation-number spin ui+148 and group ui+40 are enabled only for procType1; MO's effective process enable states still require setupUi ancestry plus feature/control state reconciliation.",
        "Feature getters remain explicit:1412fae90 is constant1 through retained primary1412e6910;1412fae10/faeb0 call1412eef80 and cannot be treated as all-enabled merely because procType5.",
    ],
    "step_count_mode5": [
        "Start source initially SC.nstart. If actual operation is absent or startsFromKey, hide nstart checkbox, choose stack0, retain SC source.",
        "For nonhead operation: class absent or vt+200 false hides checkbox, selects stack1 and blocked-unchecks it. A supporting class shows checkbox, chooses stack0 iff nstartGiven, blocked-checks actual flag, and uses operation.startStepNo iff flag true.",
        "If chosen start source>0, original converts using NEG(SC.nstart+1), even when chosen source came from operation.startStepNo; retain this non-obvious dependency and 32-bit wrap.",
        "All branches clamp start to[-999999,999999], steps[0,999999] and increment[1,999999], then fill primary die selector.",
        "Save writes SC.nstart first. For actual nonhead op: absent/non-supporting class clears nstartGiven; supporting class writes remembered checkbox and writes startStepNo only if checked. For head op force nstartGiven1 and startStepNo=remembered spin. Then write SC.nstep,stpinc,pdie.",
    ],
    "general_override_closed_existing_binding": {
        "leaves": "Retained original140010330/360 are *(int*)(sub+18)==2/3, respectively.",
        "project_binding": "Existing activeSubProject(Project&) is source-labelled1400a7390, subs[activeSub] with bounds/null check in model/project.cpp. Thus preserve flags iff active subproject exists and type2/3; main type0 false.",
        "live_save": "Original General evaluates this at Save. A public composer should source the actual active subproject at that boundary, not assume a remembered false capability.",
    },
    "next_owned_increment": {
        "general": "Add procType5 branch to existing guarded fill, derive hidden mesh no-store behavior from procType5; preserve existing context1 tests/profile.",
        "step_count": "Represent actual operation pointer/state and class vt+200 support explicitly. Port exact fill and apply ordering, including chosen-positive versus SC.nstart negation and writes before later fields.",
        "tests": "Actual MO head/nonhead, supporting/non-supporting/null operation, given/unchecked start, positive operation start with different SC source, live operation side effects and unchanged hidden mesh number. Existing isolated context1 remains separately labelled.",
        "public": "Keep public mode300 refusal until feature/pressure/remesh and outer lifecycle dependencies close. Root owns shared family/loader hooks; no C ABI change.",
    },
    "frozen_state": "Current candidate remains frozen for root combined build; this artifact changes no production/test sources. Current lifecycle increment receipt and compile repair retain exact source lineage; no claim those tests validate actual MO procType5.",
    "evidence": [record(path) for path in evidence],
}
target.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(target.relative_to(ROOT)), "sha256": hashlib.sha256(target.read_bytes()).hexdigest()}))
