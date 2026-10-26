"""Retain General widget ancestry/final-enable and MO operation bindings."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
target = ROOT / "analysis/wave2-save-diffs-300-mo-control-bindings.json"
if target.exists():
    raise SystemExit("Immutable control binding note already retained.")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


note = {
    "status": "read_only_control_binding_reconciliation_for_next_mo_increment",
    "correction_to_prior_route": "General's initial TRANS4 enable from constant1412fae90 is overwritten at the end of140eb9630 by1412faeb0. Final effective state must use the latter, not the earlier constant assignment.",
    "setupui_ancestry": [
        {"ui": "40", "kind": "QGroupBox", "parent_ui": "28", "ctor_line": 751},
        {"ui": "60", "kind": "QGroupBox", "parent_ui": "28", "ctor_line": 908},
        {"ui": "70", "model": "TRANS2", "parent_ui": "60", "ctor_line": 983},
        {"ui": "78", "model": "TRANS1", "parent_ui": "60", "ctor_line": 1016},
        {"ui": "80", "kind": "QGroupBox", "parent_ui": "60", "ctor_line": 1049},
        {"ui": "90", "model": "TRANS3", "parent_ui": "80", "ctor_line": 1121},
        {"ui": "a0", "model": "TRANS5", "parent_ui": "80", "ctor_line": 1185},
        {"ui": "a8", "model": "HTMTHD", "parent_ui": "80", "ctor_line": 1221},
        {"ui": "c8 (decimal200)", "model": "TRANS4", "parent_ui": "80", "ctor_line": 1357},
    ],
    "effective_enable_dependency": {
        "order": ["TRANS1", "TRANS2", "TRANS3", "TRANS4", "TRANS5", "HTMTHD"],
        "values": ["default true", "default true", "1412fae10(dimension) OR1412fab10(dimension)",
            "1412faeb0(dimension)", "1412faeb0(dimension)", "1412faeb0(dimension)"],
        "qualification": "Control own enable flags from full General fill. SetupUi places process controls under60/80, not disabled units group40. Remaining original callbacks/ancestor state must still be checked before public whole-widget equivalence; no actual feature values inferred.",
        "mesh": "MO procType5 hides mesh spin158; General Save's visible AND enabled test therefore preserves model meshno.",
    },
    "operation_start_binding": {
        "startsFromKey": "Existing source14005f230= isChainHead(op) OR restartGiven; do not replace with a procType check. isChainHead140050bc0 reads active flow predecessor==-1.",
        "class": "Original140054e90(op) -> class vt+200 corresponds existing OpClass::writesDimNstart(). Missing class is false.",
        "data": "Operation.nstartGiven+18 and startStepNo+1c already represented; no shared model/layout addition needed.",
        "next_interface": "Extend StepCountContext/Page with explicit actual MO operation state/class support; preserve existing context1 aggregate callers. Apply operation nstart effects between SC.nstart and SC.nstep stores, including live startsFromKey decision.",
    },
    "smallest_external_next": [
        "General feature matrix:1412eef80 and1412fab10 plus dimension getter140130e50; fae10/faeb0 bodies already retained.",
        "Pressure record aggregation and source transport remain separate from General feature answers; retain explicit supplied bool until bound.",
        "Remesh absent-both-open-fail branch can initialize per-call ctor state and enabledfalse at actual project Open, then disabled removal at full Save. Populated reader/writer needs exact CRT TokenReader/fprintf path, not Extra3D QTextStream parsing.",
    ],
    "evidence": [record(path) for path in [
        "analysis/wave2-save-diffs-300-setupui.c",
        "analysis/wave2-save-diffs-300-fill-140eb9630.c",
        "analysis/wave2-save-diffs-300-fill-140ebb110.c",
        "analysis/wave2-save-diffs-300-apply-140ebb770.c",
        "source/apps/moproj2mst/src/model/objects.cpp",
        "source/apps/moproj2mst/src/model/opinfo.cpp",
        "source/apps/moproj2mst/src/model/project.cpp",
        "source/apps/moproj2mst/include/m2m/model.hpp",
        "analysis/wave2-save-diffs-300-mo-route-reconciliation.json",
    ]],
    "qualification": "Only retained original/source files were read. No private export, new binary capture, original runtime, source/test edit, build or fixture write during root freeze.",
}
target.write_text(json.dumps(note, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(target.relative_to(ROOT)), "sha256": hashlib.sha256(target.read_bytes()).hexdigest()}))
