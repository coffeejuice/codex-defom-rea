"""Retain editor300 persistent page composition and remaining public route."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300_lifecycle.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp",
]
evidence = [
    "analysis/wave2-save-diffs-300-snapshot-increment.json",
    "analysis/wave2-save-diffs-300-step-stop-increment.json",
    "analysis/wave2-save-diffs-300-meshing-contract.json",
    "analysis/wave2-save-diffs-300-environment-contract.json",
    "analysis/wave2-save-diffs-300-advanced-contract.json",
    "analysis/wave2-save-diffs-300-extra3d-increment.json",
    "analysis/wave2-save-diffs-300-native-io-repair-contract.json",
    "analysis/wave2-save-diffs-300-support-140eb26f0.c",
    "analysis/wave2-save-diffs-300-fill-boundary.asm",
    "analysis/wave2-save-diffs-300-fill-dispatch.asm",
    "analysis/wave2-save-diffs-300-save.asm",
    "analysis/wave2-save-diffs-300-support-140ecb340.c",
    "analysis/wave2-save-diffs-300-support-140eca080.c",
    "analysis/wave2-save-diffs-300-support-140eb5180.c",
    "analysis/wave2-save-diffs-300-support-140eb52f0.c",
    "analysis/wave2-save-diffs-300-support-140eb5250.c",
    "analysis/wave2-save-diffs-300-support-140ed4b00.c",
    "analysis/wave2-save-diffs-300-support-140ed5dd0.c",
    "analysis/wave2-save-diffs-300-ecf340.asm",
    "source/port/defcmd/src/p8_08/model_copy.cpp",
    "source/port/defmodel/src/step/sim_ctrl.cpp",
    "analysis/wave3-wizard-native-validation.json",
]
cases = re.findall(r"TEST_CASE\(([^)]+)\)", (ROOT / sources[-1]).read_text())
receipt = {
    "status": "persistent_initial_page_composer_implemented_public_guard_retained",
    "sources": [record(path) for path in sources],
    "evidence": [record(path) for path in evidence],
    "source_qualification": "Retained original binary selections remain oracle. Historical helper receipts describe their then-current scope; later verified helpers supersede only their explicitly closed pending items. No new runtime, static export/capture or build performed for this increment.",
    "reconciliation": {
        "settings": "Existing per-call MoInputMode default0/env mapping remains unchanged. The initial composer requires applicationMode1, dimension3, KnownC and explicit feature/control context; none is inferred from the setting.",
        "open": [
            "140eb26f0 initializes page/selection0 and invokes140eb5630(0,0).",
            "140eb80c0(page0) fills General, StepCount, StepSize, Stopping, Meshing, Iteration, Environment, Advanced, Extra3D in order. General/StepSize/Iteration/Environment callbacks can mutate model state.",
            "140eb5630 copies ENVTMP,CNVCOF,ENVATM,ACVCOF,DTTYPE,VFACTR and VFACTR count into editor temporaries after initial dispatch.",
            "140eb50f0 repeats Stopping and Meshing, then invokes140ecb340(0).",
            "140ecb340 constructs/initializes SC backup with actual owning step, field-aware deep-copies model SC; refills General/StepCount; copies DTTYPE then refills StepSize/Iteration; copies four environment curves plus VFACTR/count then refills Environment/Advanced; overwrites NBC75/105, restores SC through original copy routine.",
            "The composed shared General heat combo follows each Environment refill. Extra3D files are not re-read during this final refresh.",
        ],
        "apply": [
            "140eca080(page0) calls each page apply in the same nine-page order. ValidationStopped returns only from that page; it does not skip subsequent page calls.",
            "An UnsupportedSnapshot stops this adapter only at the reached boundary; earlier model stores and file effects remain. Reports distinguish uncalled pages from completed/validation-stopped pages.",
            "140eb52f0 releases the mesh temporary array after all page blocks; session clears those entries so a repeated apply cannot reuse freed object snapshots.",
            "Verified empty recovery tail140ed5dd0 clears NRECID, reapplies remembered Advanced, then resizes TIMERC from TIMEID count and ERECVL from ERECID count even after validation stop.",
        ],
        "bindings": "Backup uses existing GUI1408df7b0/native1401f3660 def::cmd::sim_ctrl_copy binding already used by L5, with native sim_ctrl_set_owner initialization. This is not a whole-struct byte snapshot; original field-aware copy omissions remain. No new native copy equivalence audit is claimed here.",
    },
    "minimal_shared_interfaces": {
        "ownership": "Root owns eventual family.cpp and loader_l3.hpp hooks. Worker makes no shared source/model/header changes.",
        "context": "InitialEditorContext{GeneralContext general;StepCountContext stepCount;NumericLocale numericLocale;bool pressureFieldsPopulated}",
        "open": "optional<InitialEditorSession> openInitialEditor(GuiModel&,RemeshState&,Extra3DIo&,const InitialEditorContext&)",
        "apply": "InitialApplyReport applyInitialEditorPageState(InitialEditorSession&,GuiModel&,RemeshState&,Extra3DIo&,const vector<RecoveryEntry>* rootRecovery,const vector<RecoveryEntry>* operationRecovery)",
        "persistence": "FormingClass should own an opaque per-shown-editor session and I/O/runtime state, constructed at actual editor activation and retained until replacement/Close. Save must not call fill again against latest model.",
        "abi": "Internal C++ hooks only; no m2m C signature, flags or existing mode0 branch changes.",
    },
    "public_route_remaining": [
        {"item": "General control enable state", "known": "Normal app mode1 enables General parent group;140ecf360 reads base editor+49. TRANS3 enables on1412fae10(mode) OR1412fab10(mode), TRANS4 getter1412fae90 is constant1 through primary getter1412e6910; HTMTHD getter1412faeb0 still reads feature data via1412eef80.", "next": "Trace base+49 setter/default and1412eef80 inputs/call result; do not manufacture all-enabled context."},
        {"item": "General process override", "known": "Save obtains manager1400a7390 item; if either140010330 or140010360 is true, original preserves TRANS/HTMTHD before later shared combo logic.", "next": "Bind actual project manager state at Save, and confirm fresh single-project branch; no forced false default."},
        {"item": "Pressure feature", "known": "Original1446ee586 is startup feature record aggregate+6=OR(+1c,+1d), not a plain INI setting; absent filled pressure text is Intermediate and Save writes0.", "next": "Recover exact record source/initialization supplying those flags or explicit qualified per-call state; do not infer default0 from BSS."},
        {"item": "Remesh lifecycle", "known": "Fresh singleton ctor state is source-proven. Project Open14008a9b0 after CWD set invokes reader1400ab170; enabled=reader result. Reader tries LOCAL_REMESH.DAT KEYWORD_DRIVEN then LOC_DEN_CTRL.DAT; absent both returnsfalse without changing scalar state. Project Save1400980d0 removes LOCAL_REMESH when disabled or writer1400abcc0 when enabled.", "next": "First integrate absent-file disabled branch at actual project Open/Save point, preserving CRT open failure behavior; then port scalar reader/writer via original TokenReader contract."},
        {"item": "Recovery UI and populated records", "known": "Constructor after refresh calls140ed4b00 ->140ed4bb0/select row0/140ed6460. Populated records require missing+4/+8/+10 layout fields and remain excluded.", "next": "Prove empty-row guards of140ed4bb0/140ed6460 before claiming full constructor completion; retain nonempty guard."},
        {"item": "Intermediate view callbacks", "known": "After page apply original calls140ecd2f0(0),140ecd430(0),140ecf340 getter before recovery tail. Getter140ecf340 is base+48; secondary toggle invokes viewer class+150 and140ec0a00, not a proven generic no-op.", "next": "Bind/recover their exact non-UI model consequences; composer marks public completeness false rather than silently claim those effects complete."},
        {"item": "Outer Save", "known": "140eb5180 invokes140131110 refresh/dirty, copies SC.oprnam to operation via140050a90,140eb5250 stores current model in project root,140ece7b0(0)/140eb5500 restore view; caller then propsLoad140054350.", "next": "Root shared hook must preserve this call order and existing project/model ownership, after page-state lifecycle acceptance."},
        {"item": "Existing profile guards", "known": "Movement6 curves, populated springs/recovery, nonconstant page curves, furnace/TIMEID/VFACTR active states, non-C/Unicode/radix sidecar branches remain explicit guards.", "next": "Integrate supported initial branch first; subsequent branch recovery remains queued, not called completion."},
    ],
    "tests": {"status": "not_run_worker_root_owns_build_and_execution", "new_cases": cases,
        "new_cases_count": len(cases), "prior_checkpoint": "analysis/wave3-wizard-native-validation.json:112 CTests,48 editor helper cases passed, no fixture/diagnostic changes",
        "expected_editor_helper_cases": 48 + len(cases),
        "method": "Use actual GuiModel/SC plus typed I/O instrumentation. Open->mutate source->apply proves remembered state. Instrument final initial I/O to distinguish repeat/refill state; instrument Extra3D Save to prove earlier page state and tail Advanced reapply. Tests also cover reached unsupported boundaries and model identity."},
    "qualification": "Internal page composer is not full public Open/Save lifecycle acceptance. Public300 Save remains refused. No fresh original run, app control, fixture writes or worker test execution; no token/speed claim.",
}
target = ROOT / "analysis/wave2-save-diffs-300-lifecycle-increment.json"
target.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(target.relative_to(ROOT)), "sha256": hashlib.sha256(target.read_bytes()).hexdigest(), "new_cases": len(cases)}))
