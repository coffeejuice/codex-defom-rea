"""Freeze the observed original MO feature-result adapter and its evidence."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-feature-context-increment.json"
if TARGET.exists():
    raise SystemExit("Immutable feature-context receipt already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_mo.cpp",
]
evidence = [
    "analysis/wave2-doe-sim300-mo-general-bindings-decompile.json",
    "analysis/wave2-doe-sim300-mo-feature-query-decompile.json",
    "analysis/wave2-doe-sim300-mo-feature-query-receipt.json",
    "analysis/wave2-save-diffs-300-feature-result.asm",
    "analysis/wave2-save-diffs-300-feature-query-wrapper.asm",
    "analysis/wave2-save-diffs-300-feature-literals.json",
    "analysis/wave2-save-diffs-300-fill-140eb9630.c",
    "analysis/wave2-save-diffs-300-support-140eb26f0.c",
    "analysis/wave2-save-diffs-300-apply-140ebaa30.c",
    "analysis/wave2-save-diffs-300-mo-control-bindings.json",
    "analysis/wave3-save-dispatch-validation.json",
]
header, cpp, tests = [(ROOT / path).read_text() for path in sources]
checks = {
    "int32_declaring_header": "#include <cstdint>" in header,
    "optional_declaring_header": "#include <optional>" in header,
    "helper_declared": "generalContextForMoAvailability(const Project&" in header,
    "helper_defined": "generalContextForMoAvailability(const Project&" in cpp,
    "test_main": "int main() { return testkit::run_all(); }" in tests,
    "test_registry_type_header": '#include "m2m/mst/writer.hpp"' in tests,
}
if not all(checks.values()):
    raise SystemExit(f"Prebuild checks failed: {checks}")
receipt = {
    "status": "observed_feature_context_component_frozen_for_root_build",
    "source_qualification": "Original fixed PE remains oracle. New private read-only three-function export on byte-verified project completed16.848s, no runtime/shared REA. Ghidra void return on1412e4b90 is incorrect; bounded raw bytes establish EAX return and caller preservation. Entry-only known_callees are not coverage proof.",
    "sources": [record(path) for path in sources],
    "evidence": [record(path) for path in evidence],
    "original_contract": {
        "query": "1412eef80 allocates product client, ctor1412ee6a0, selects feature1412e67d0, checks availability1412e4b90, saves EAX, unregisters/destroys client and returns saved signed integer. Each dimension3 getter returns result>0.",
        "identity": "Literal DEFORM-3D product; TRANS3 is3D_DEF_GUI_MIC3(fae10) OR3D_DEF_WZD_HEAT_TREAT3(fab10). Final TRANS4/TRANS5/HTMTHD enables use3D_DEF_HEATING(faeb0). Logs name CLManagerClientForProduct::check_license_availability; DEF_MODULE_NO_CHECKING ctor string does not establish successful result.",
        "result": "Availability requires four query string pointers, product record and feature record; missing pointers/records or transaction failure returns0. Virtual+48,+20,+30 transactions expect sizes18h,b8h,18h, then success returns output record+14h (initial b0h). This is an external transaction, not a constant getter. No server/availability default is inferred.",
        "general_order": "MIC3 first, WZD only if MIC3 unavailable. Then HEATING queries separately in source order: UI+a8(HTMTHD), UI+a0(TRANS5), UI+c8(final TRANS4), the last overwriting earlier constant TRANS4 enable. Constructor control array maps indices0..5 to UI78,70,90,c8,a0,a8; Apply writes engine TRANS0..4/HTMTHD from these controls.",
    },
    "adapter_contract": {
        "type": "MoFeatureAvailability{optional<int32_t> microstructure3D,wizardHeatTreat3D;array<optional<int32_t>,3> heating}",
        "method": "generalContextForMoAvailability(Project&,dimension,const MoFeatureAvailability&) ->optional<GeneralContext>; supports actual MO3D only, no settings/license reads or global caching.",
        "unknown": "nullopt is unknown and refuses if that original query is reached. Numeric0 andnegative are observed unavailable, positive is observed available. MIC3positive permits missing skippedWZD; missingMIC3 always refuses.",
        "heating": "All three reached observations required, independently mapped [2],[1],[0] to process enables[3],[4],[5]. Differing answers are retained rather than imposing stability. Existing bool helper remains an explicitly supplied isolated control-state convenience; it does not recover repeated queries.",
        "scope": "One General fill invocation only. No claim observations remain stable across constructor refills. Existing persistent composer still takes explicit GeneralContext and public300 Save remains refused. Next owned composition binding must accept two independent observation sets at the two actual General fills; root eventual shared hook supplies observed per-call state, not license guesses.",
    },
    "tests": {
        "new_cases": [name for name in re.findall(r"TEST_CASE\(([^)]+)\)", tests) if name.startswith("mo_feature_observations_")],
        "mo_cases_count": len(re.findall(r"TEST_CASE\(([^)]+)\)", tests)),
        "method": "Missing/short-circuited observations, signed result semantics, each mandatory HEATING observation, dimension guard, independent HEATING call order and fill->change observed input->Apply remembered controls. Real Project is used; no fixture files are written.",
        "status": "not_run_worker_root_owns_build_and_execution",
        "prior_checkpoint": "Parent reports repaired118 CTests passing in analysis/wave3-save-dispatch-validation.json; no new executable registration, two new MO cases in existing group.",
        "cheap_handoff_checks": checks,
    },
    "remaining": ["Two independent constructor General-fill observation bindings", "Pressure feature record provenance/aggregation inputs", "Remesh native type4 field layout/reader contract", "Existing recovery/view/outer Save dependencies and guarded profiles"],
    "limits": "No worker builds/tests, app control, original runtime or fixture mutation. No new public settings/ABI/env surface, license client implementation, default availability answer or full integration claim.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "checks": checks}))
