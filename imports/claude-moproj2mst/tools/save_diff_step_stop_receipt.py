"""Retain source/evidence hashes for editor300 step-size and stopping recovery.

No build, target execution, fixture write, or analysis-project mutation.
"""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]


def record(path):
    content = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(content),
            "sha256": hashlib.sha256(content).hexdigest()}


def main():
    source = ["source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
              "source/apps/moproj2mst/src/engine/simcontrol300.cpp",
              "source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp"]
    evidence = ["analysis/wave2-sim300-pages3-4-root-review.json",
                "analysis/wave2-save-diffs-300-fill-140ebb950.c",
                "analysis/wave2-save-diffs-300-apply-140ebc1f0.c",
                "analysis/wave2-save-diffs-300-support-140ecc770.c",
                "analysis/wave2-save-diffs-300-support-140eccaa0.c",
                "analysis/wave2-save-diffs-300-support-140ecc540.c",
                "analysis/wave2-save-diffs-300-support-140ecb340.c",
                "analysis/wave2-save-diffs-300-fill-140ebd180.c",
                "analysis/wave2-save-diffs-300-apply-140ebf8f0.c",
                "analysis/wave2-save-diffs-300-support-140eb6310.c",
                "analysis/wave2-save-diffs-300-support-140ebe800.c",
                "analysis/wave2-save-diffs-300-support-140eb71e0.c",
                "analysis/wave2-save-diffs-300-step-stop-formats.json",
                "analysis/wave2-save-diffs-300-rotation-formats.json",
                "source/port/defmodel/src/window/inter_object_ops.cpp"]
    report = {
        "status": "isolated_helpers_ready_public_save_still_refused",
        "sources": [record(p) for p in source],
        "evidence": [record(p) for p in evidence],
        "original_sha256": "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc",
        "qualification": "Retained original static decompiles from the byte-verified private project. No new Ghidra export or target runtime. Evidence contains source addresses; source completeness is bounded to the explicit guards below.",
        "step_size": {
            "fill": "eb b950 -> ecc770/eccaa0/ecc540; constructor final ecb340 copies model DTTYPE into editor+e8 before refilling step size",
            "guards": ["dimension3", "explicit KnownC numeric locale", "DTTYPE type0x80 / ndim0 / no owned object / no table pointers or counts / ncomp0or1", "DTTYPE mode0or1 and STPDEF1,2,3", "finite filled scalar text inputs"],
            "snapshot": "Each line edit retains Qt g6 text. The temporary DYNARY is deep-copied through the existing recovered dynary_copy and held const; no shallow owning copy.",
            "open_effect": "ecc770 writes model DTTYPE mode before any Save; STPDEF3 radio2 stores mode1",
            "apply_order": ["DTTYPE mode", "DTMAX1 then zero DSMAX, or DSMAX then zero DTMAX1", "STPDEF", "DEMAX", "DPLEN", "DTSUB", "radio2 alternate DTMAX1", "DTPMAX1,2,3 from selected page", "SLDERR", "temporary DTTYPE curve deep-copy then restore mode"],
            "partial_validation": "Only Invalid stops. Each preceding write survives. Invalid before final curve copy leaves that copy unapplied. Intermediate text is converted using the same QString numeric seam.",
            "excluded": "2D fields, tabulated curves and unrecovered curve-form callbacks are explicit unsupported profiles, not defaulted.",
        },
        "stopping": {
            "guards": ["model dimension3", "explicit KnownC numeric locale", "object0 absent", "neither stored MDS selector names an eligible existing object (kind!=7)", "finite filled scalar and selected rotation-vector values"],
            "snapshot": "TMAX, SMAX, VMIN, LMAX, EMAX use Qt g6 text; MDSOBJ4 uses width10/fixed5. Absent/undefined MDS selections become none; invalid mode index becomes-1. Rotation vector/center text is retained from the fill-time selector.",
            "apply_order": ["TMAX", "SMAX1,2,3", "PDIE movement warning predicate", "VMIN1,2,3", "LMAX1,2,3", "EMAX", "MDSOBJ4 (Invalid only logs; still converts)", "MDSOBJ1,2 from none and mode from combo", "both eb6da0 no-ops because selected object0 absent", "ec09a0(&0) no-op", "fresh rotation selector then vector and center writes"],
            "rotation": "eb6310 scans object positions1..count for the first nonzero vector, returns position+1, fallback1; the caller then looks up that number. Apply deliberately repeats the scan against current model state. Initial editor+11c0 does not mean object0 or no writes.",
            "prompt_boundary": "If current PDIE movement kind3,4,7,8,9,10 and any VMIN/LMAX parsed difference>1e-6, helper returns UnsupportedSnapshot at that point after TMAX/SMAX changes. No dialog answer is invented.",
            "excluded": "Existing selected MDS objects, their node/spin callbacks, and PDIE warning answers remain unsupported.",
        },
        "constructor_contract_correction": {
            "immediate_repeat": "eb50f0 first repeats stopping and meshing",
            "then": "ecb340(0) backs up SC; refills general, step count; copies DTTYPE to editor+e8; refills step size, iteration; copies environment temporary curves and VFACTR; refills environment, advanced; restores SC from backup",
            "consequence": "Final widget and curve snapshots must reflect this complete order. The original two-element repeat array alone was not the complete constructor tail; the new constructorBackedUpRefreshOrder records the extra six page fills.",
        },
        "tests": {"status": "not_run_by_worker_root_builds_and_executes",
                  "all_snapshot_cases": re.findall(r"TEST_CASE\(([^)]+)\)", (ROOT / source[-1]).read_text()),
                  "new_cases": 8,
                  "prior_validation": "Root reported combined103 CTests passed before this increment, including snapshot8/8; analysis/wave2-snapshot-tasklists-validation.json"},
        "successor_started": "Retained meshing ec0a40/ec1350 review started: deep per-object forms and first selected form's text/spin roundtrip, separately from global remesh singleton. No meshing implementation claim yet.",
        "not_claimed": ["Public mode300 Save integration", "Fresh original acceptance or RS55 equality", "Complete model/license/locale settings binding", "Token efficiency gain; token usage unavailable"],
    }
    path = ROOT / "analysis/wave2-save-diffs-300-step-stop-increment.json"
    path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"receipt": str(path.relative_to(ROOT)),
                      "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                      "snapshot_cases": len(report["tests"]["all_snapshot_cases"])}))


if __name__ == "__main__":
    main()
