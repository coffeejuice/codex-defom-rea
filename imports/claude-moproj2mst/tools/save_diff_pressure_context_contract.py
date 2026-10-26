"""Record startup HEATING feature bits supplying the Environment pressure aggregate."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-pressure-context-contract.json"
if TARGET.exists():
    raise SystemExit("Immutable pressure contract already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


files = [
    "analysis/wave2-save-diffs-300-pressure-global-init.asm",
    "analysis/wave2-doe-sim300-pressure-startup-decompile.json",
    "analysis/wave2-doe-sim300-pressure-feature-setter-decompile.json",
    "analysis/wave2-doe-sim300-pressure-feature-setter-receipt.json",
    "analysis/wave2-save-diffs-300-pressure-feature-flags.asm",
    "analysis/wave2-save-diffs-300-pressure-feature-map-entry.asm",
    "analysis/wave2-save-diffs-300-pressure-feature-map-fields.asm",
    "analysis/wave2-save-diffs-300-pressure-feature-map-3d.asm",
    "analysis/wave2-save-diffs-300-pressure-feature-names.json",
    "analysis/wave2-save-diffs-300-pressure-feature-aggregation.asm",
    "analysis/wave2-save-diffs-300-fill-140ec3fa0.c",
    "analysis/wave2-save-diffs-300-remesh-raw-contract.json",
]
receipt = {
    "status": "read_only_pressure_observed_input_binding_source_closed",
    "evidence": [record(path) for path in files],
    "qualification": "Fixed original PE/decompile selections only, no original runtime or default-license claim. Ghidra single pressure setter export completed20.101s on byte-verified private copy; shared lock released. Raw flag function is one bounded0x17ad-byte function region; mapping fragments supply exact target offsets. No source edits since feature component freeze.",
    "startup": [
        "Main140001410 at1705 calls1412ef8b0(global1446ee580,false,false); it runs1412ef050 for dimension2/3 when corresponding product main flags are absent.",
        "Primary1412e6910 is source-proven1. In1412ef050, product client obtains actual record via1412e5df0. Failed product record returns before1412eb000; successful branch retains record pointer at+1450 and calls1412eb000(record,legacy flags,dimension). No availability result can be manufactured from this branch structure alone.",
        "1412eb000 uses product feature record count/index/value array, name-to-index lookups and nonzero comparisons; it obtains destination byte through1412e76c0, then stores normalized nonzero status. Missing mapping/out-of-range branches call original error path; no generic safe default is inferred for those paths.",
        "1412e76c0 compares exact2D_DEF_HEATING at142db5438 and returns global+1c (140...7800); exact3D_DEF_HEATING at142db5310 returns global+1d (140...7a2c). These are two distinct product feature bits.",
        "Main separately calls packet handler1412eae20 then aggregation1412e73d0 at17fd. The packet leaf1412e75b0 only updates+2f/+44, as corrected in prior remesh raw contract; it does not supply pressure bits.",
    ],
    "environment": "1412e73d0 computes global byte+6 as OR(nonzero byte+1c,nonzero byte+1d). fill140ec3fa0 reads1446ee586 to decide whether ENVMPR/ENVMPT text is filled. Therefore a2D-only HEATING feature still populates pressure fields in a3D editor. This startup feature aggregate is distinct from General's later per-fill3D license availability transactions.",
    "next_owned_component": {
        "input": "Explicit observed startup2D_DEF_HEATING and3D_DEF_HEATING byte states, optional to retain unknown. No license service/settings/INI reads and no reuse of General's three HEATING results.",
        "aggregation": "Any observed nonzero bit proves aggregate true; both observed zero prove false; otherwise unknown remains refusal. A caller may instead supply the directly observed global aggregate with explicit provenance. Per-call state, no global cache or forced default.",
        "composition": "Bind resolved bool at actual Environment fills (same startup snapshot unless separately observed lifecycle change). Keep General query observations separate for each General refill. Existing InitialEditorContext pressureFieldsPopulated is explicit but not yet provenance-bound by typed startup observations.",
        "tests": "Bothzero,2D-only,3D-only,noncanonical nonzero bytes, unknown combinations, and Environment filled-vs-empty remembered text/apply distinction. No assertion weakening or publicguard removal.",
    },
    "remaining": "Startup observations/provider remain external input to reconstruction; runtime-derived provenance is not available from historical RS55. Root owns eventual shared settings/family hooks. Remesh byte overwrite and actual file lifecycle remain separate successors. Publicmode300 Save still guarded.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest()}))
