"""Retain the bounded editor300 snapshot increment and its evidence qualifications.

Reads sources/evidence only; writes this increment's analysis receipt. It neither
builds nor executes the converter, original program, or an analysis database.
"""
from pathlib import Path
import hashlib
import json
import re


ROOT = Path(__file__).resolve().parents[1]


def record(relative):
    data = (ROOT / relative).read_bytes()
    return {"path": relative, "bytes": len(data),
            "sha256": hashlib.sha256(data).hexdigest()}


def main():
    sources = [
        "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
        "source/apps/moproj2mst/src/engine/simcontrol300.cpp",
        "source/apps/moproj2mst/src/engine/loader_l3.cpp",
        "source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp",
    ]
    evidence = [
        "analysis/wave2-doe-ghidra-text-verification.json",
        "analysis/wave2-save-diffs-300-fill-140eb9630.c",
        "analysis/wave2-save-diffs-300-apply-140ebaa30.c",
        "analysis/wave2-save-diffs-300-fill-140ebb110.c",
        "analysis/wave2-save-diffs-300-support-140ebb3f0.c",
        "analysis/wave2-save-diffs-300-apply-140ebb770.c",
        "analysis/wave2-save-diffs-300-fill-140ec19f0.c",
        "analysis/wave2-save-diffs-300-apply-140ec3130.c",
        "analysis/wave2-save-diffs-300-support-140eb26f0.c",
        "analysis/wave2-save-diffs-300-setupui-selected.json",
        "analysis/wave2-save-diffs-original-ui-solver-state.asm",
        "analysis/wave2-save-diffs-qt-sprintf-format.asm",
        "analysis/wave2-save-diffs-qt-number-format.asm",
        "analysis/wave2-save-diffs-qt-number-set-format.asm",
        "analysis/wave2-sim300-pages3-4-root-review.json",
    ]
    test = (ROOT / sources[-1]).read_text()
    report = {
        "status": "internal_page_helpers_implemented_public_save_still_refused",
        "original_sha256": "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc",
        "scope": "Three isolated initial editor300 page snapshots; not the complete initial editor",
        "sources": [record(p) for p in sources],
        "evidence": [record(p) for p in evidence],
        "implemented": [
            "General: guarded application mode1 / dimension3 / valid radio state / printable ASCII title and operation; explicit supplied enable and override state; ordered SC writes and SMODE recomputation",
            "Step count: guarded mode1; original signed start conversion, exact setupUi clamps, caller-qualified primary-die combo objects; remembered values at Apply",
            "Iteration: guarded dimension3 / explicit KnownC locale / finite CVGERR / SOLMTD0,1,2,4,5 / valid method radios; real CG callback and remembered text/combos; ordered partial validation writes",
            "ValidatorState preserves original Invalid0 / Intermediate1 / Acceptable2. Edited text requires explicit validator results; no replacement Qt validator is claimed",
            "Existing active-editor CG Open hook delegates to the shared callback, preserving its behavior",
        ],
        "formatting": {
            "implementation": "qt::number(value, 'g', 6); no UCRT printf seam",
            "binary": record("evidence/original-binaries/QtCore4_64.dll"),
            "proof": "QtCore QString::vsprintf67074f90 constructs QLocale(C=1,country0) at67074fc6..fd2 and floating branch67075837 calls QLocalePrivate::doubleToString6704ee70. QString::number67070fd0 delegates setNum67070e30, which constructs the same C locale67070e96..ea2 and calls the same doubleToString67070ecf. Lowercase g selects form2 with precision6 and default width/no extra format flags.",
            "qualification": "Bounded retained static disassembly; no target DLL executed. Numeric validator locale remains separately explicit.",
        },
        "lifecycle_obligations": [
            "Initial fill order: general, step count, step size, stopping, meshing, iteration, environment, advanced, extra3D",
            "Constructor then fills stopping and meshing again; these are not default/unvisited forms",
            "General fill blocks title and operation-name textChanged signals while setting those texts",
            "Stopping Save re-runs rotation selector against then-current model vectors; first object with any nonzero rotation component, otherwise object1; never infer object0 from editor+11c0",
        ],
        "remaining_before_public_save": [
            "Bind explicit GeneralContext enable/override and primary-die predicate state to recovered per-call original settings/license/project state; no ambient license defaults are inferred",
            "Compose step-size temporary curve fill/callback/apply and sequential validation, preserving DTTYPE mutation during Open",
            "Compose both stopping fills and selected-object writes, sequential validation, default dialog branch and Save-time rotation reselection",
            "Compose both meshing fills, first selected mesh form roundtrip, unvisited deep copies and global remesh state",
            "Resolve environment feature/license-dependent population and exact default/blank widget state; it affects ENVMPR/ENVMPT and must not be guessed",
            "Compose advanced/frequency callbacks and extra3D sidefiles with explicit supported-profile guards",
            "Guard nonempty recovery/PPVar lists until native entry fields and parsing fidelity are recovered; compose empty-list tail cleanup",
        ],
        "tests": {
            "new_registered_test": "m2m.mdt.simcontrol300_snapshot",
            "cases": re.findall(r"TEST_CASE\(([^)]+)\)", test),
            "status": "not_run_by_worker_coordinator_owns_build_and_execution",
            "coverage": ["Open snapshot then mutate model then Apply", "exact spin clamp boundaries", "CG Open preserves accepted fractions but Apply canonicalizes remembered integer text and combos", "Invalid first and second convergence partial writes", "Intermediate conversion is not Invalid", "invalid integer limits default200", "explicit unsupported guards and unrecovered locale"],
            "cmake": "Existing CONFIGURE_DEPENDS globs discover source and test; no CMake edit",
        },
        "limitations": [
            "Public mode300 Save still deliberately refuses at0x140eb5180; no full Save or RS55 equality claim",
            "These helpers are not wired to a constructor-wide snapshot; the aggregate contains only three recovered pages and declares completeForPublicSave=false",
            "No original runtime, GUI control, fixture mutation, build or test execution performed for this worker checkpoint",
            "Static decompiles are qualified by private project byte-identity receipt; empty decompiler callee metadata is not a coverage claim",
            "Token usage unavailable; no token/time efficiency improvement claim",
        ],
    }
    destination = ROOT / "analysis/wave2-save-diffs-300-snapshot-increment.json"
    destination.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"receipt": str(destination.relative_to(ROOT)),
                      "sha256": hashlib.sha256(destination.read_bytes()).hexdigest(),
                      "cases": len(report["tests"]["cases"])}))


if __name__ == "__main__":
    main()
