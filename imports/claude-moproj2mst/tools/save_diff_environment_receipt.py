"""Retain editor300 environment/default-PDIE implementation source hashes.

Reads retained evidence and source; no build, target runtime or fixture writes.
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
    report = {
        "status": "environment_and_pdie_default_helpers_implemented_public_save_still_refused",
        "sources": [record(path) for path in source],
        "evidence_contracts": [record("analysis/wave2-save-diffs-300-environment-contract.json"),
                               record("analysis/wave2-save-diffs-300-pdie-default.json")],
        "original_sha256": "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc",
        "environment": {
            "profile": "3D/KnownC, four constant curves without arrays/owned objects, finite populated values, furnace off, empty TIMEID, VFACTR mode0, ACVCOF var0=0",
            "feature_state": "Pressure fields populated boolean is explicit. Primary1412e6910 is constant1, so ENVATM/ACVCOF are always populated; no unnecessary primary-license input was added.",
            "state": "Four deep const temporary curves and g6 ValidatedText constants, seven ordered scalar widgets, optional shared General heat-method combo overwrite",
            "copy_order": "ENVTMP,CNVCOF,ENVATM,ACVCOF copies occur before any scalar validation; Invalid constant retains remembered raw value but still copies. Scalars validate sequentially; only Invalid stops.",
            "empty_pressure": "Feature-disabled pressure fields remain original empty text with source-proven Intermediate state; Apply converts them to0 and continues.",
            "shared_control": "fillEnvironmentSharedControls propagates the actual shared HTMTHD combo index into GeneralPage before its earlier Save apply; no independent-page scalar shortcut.",
        },
        "pdie_default": {
            "change": "Replaced the previous UnsupportedSnapshot at the PDIE warning with source-proven default-No behavior.",
            "behavior": "For movement kinds3,4,7,8,9,10 and differing limits, keep current six VMIN/LMAX fields; continue EMAX/MDS/rotation tail. TMAX/SMAX preceding stores survive. No chosen-Yes branch is added.",
            "proof": "OriginalNo|Default0x10100; exact retained QtGui old-question button setup selects its bit8 marker and strips0x100 to No0x10000. See PDIE default evidence contract.",
        },
        "tests": {
            "status": "not_run_by_worker_coordinator_owns_build_and_execution",
            "cases": re.findall(r"TEST_CASE\(([^)]+)\)", (ROOT / source[-1]).read_text()),
            "new_cases": 6,
            "modified_existing_case": "stopping_partial_validation_and_pdie_default_no_continues_tail now checks the proven No branch instead of the former unsupported boundary",
            "prior_validation": "Root reported full108 checks passed and21 snapshot cases before this increment; analysis/wave3-native-save-validation.json",
        },
        "successor": "Advanced-page recovery started. Always-called time callback1408bfe60 updates object state; it must be recovered and ported, not dropped as UI-only.",
        "remaining": ["Advanced/extra3D/recovery-tail composition", "Native project remesh reader/writer binding", "External capability and pressure-feature record state binding", "Constructor-wide persistent snapshot and public Save dispatch", "Fresh original comparison and RS55 full mode1 raw comparison"],
        "qualification": "No original runtime/app control, fixture mutation, build or test execution by this worker. Original static evidence remains oracle; native code-byte identity is retained. Token usage unavailable.",
    }
    path = ROOT / "analysis/wave2-save-diffs-300-environment-increment.json"
    path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"receipt": str(path.relative_to(ROOT)),
                      "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                      "snapshot_cases": len(report["tests"]["cases"])}))


if __name__ == "__main__":
    main()
