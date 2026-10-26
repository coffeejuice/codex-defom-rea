"""Record the original remesh raw-token overwrite, not a numeric parse repair."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-remesh-raw-contract.json"
if TARGET.exists():
    raise SystemExit("Immutable remesh contract already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


evidence = [
    "analysis/wave2-save-diffs-300-remesh-ctor.asm",
    "analysis/wave2-save-diffs-300-remesh-static-init.asm",
    "analysis/wave2-save-diffs-300-remesh-global-xrefs.json",
    "analysis/wave2-save-diffs-300-remesh-reader.asm",
    "analysis/wave2-save-diffs-300-support-1400ab170.c",
    "analysis/wave2-save-diffs-300-support-1400abcc0.c",
    "analysis/wave2-save-diffs-300-remesh-file-literals.json",
    "analysis/wave2-save-diffs-300-remesh-token-dispatch.asm",
    "analysis/wave2-save-diffs-300-remesh-token-cases.json",
    "analysis/wave2-save-diffs-300-remesh-token-copy.asm",
    "source/apps/moproj2mst/include/m2m/datafiles.hpp",
    "source/apps/moproj2mst/src/datafiles/token.cpp",
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp",
]
receipt = {
    "status": "read_only_original_raw_overwrite_contract_no_implementation",
    "evidence": [record(path) for path in evidence],
    "original": {
        "singleton": "Getter1402c7ed0 returns static global143d91238; static init141cd9ba0 calls ctor1402cc4e0. Constructor proves scalar/member accesses through+29; observed field span is at least0x2a, not proof of sizeof0x30 or any larger object extent.",
        "layout": [
            {"offset": "0x00", "width": 4, "meaning": "method", "default": 1},
            {"offset": "0x04", "width": 1, "meaning": "good internal changeable", "default": 1},
            {"offset": "0x08", "width": 8, "meaning": "filter distance double", "default": 0},
            {"offset": "0x10", "width": 8, "meaning": "uniform internal edge double", "default": 0},
            {"offset": "0x18", "width": 1, "meaning": "improve contact byte", "default": 0},
            {"offset": "0x20", "width": 8, "meaning": "scaling factor double", "default": 1.2},
            {"offset": "0x28", "width": 1, "meaning": "good surface changeable byte", "default": 1},
            {"offset": "0x29", "width": 1, "meaning": "enabled byte", "default": 0},
        ],
        "reader": "At1400ab47e..4a8 UNIFORM_INTERNAL_EDGE_LENGTH passes destination singleton+10 and type4 to1409068f0. Dispatch uses(type-1), tableVA140906e0c storesRVA906ca6. At140906ca6, original calls14146fec0 with destination and selected token. Raw copier writes bytes through NUL. This is not Double10 conversion; type10's table entry is independently different.",
        "token": "For pos0, type4 consumes the next existing token if available. If it must read a fresh physical line, Raw tokenizes only CR/LF for the first token; it does not universally mean substring after the keyword. EOF leaves non-Upper output untouched. Adapter profile must bound the selected token after original tokenization, not merely the original line length.",
        "writer": "1400abcc0 reads qword+10 and formats %s %lf newline for UNIFORM_INTERNAL_EDGE_LENGTH. Thus original text bytes become later double bits; a numeric parse would alter original behavior.",
    },
    "safe_reconstruction_proposal": {
        "first_profile": "Selected token<=7 bytes plus NUL stays wholly in known double storage+10..17. Copy current double representation to array<byte,8>, overwrite prefix with exact token plusNUL, decode bits back through memcpy/bit_cast. Untouched suffix remains current state, including a preceding duplicate keyword's effect. No floating-point parse, no clearing entire field, no whole-struct write.",
        "extent": "Token length8 writes its NUL into+18 improveContact. Length16 reaches+20 scaling, length24 reaches+28 goodSurface, length25 reaches+29 enabled. No claim of safe larger object extent. Refuse before first spill in initial bounded implementation, recording earlier reader effects rather than transactional rollback.",
        "longer_profile": "To preserve spills, use modeled byte storage and exact raw-byte field loads, not arbitrary writes into C++ bool objects. Existing RemeshState bool fields cannot safely represent arbitrary character bytes or preserve writer promotion. Future support needs writer/consumer byte semantics and actual field/extent qualifications; do not assume sizeof from natural alignment.",
        "other_inputs": "Native rt open/fseek/close flags and exact TokenReader numeric/locale conversions remain separate contracts. Existing memory TextFile is not proof of native path/sharing semantics. Missing integer token can leave original shared local unassigned; exclude indeterminate first-use cases rather than invent0. Empty readable file succeeds with retained state; both open failures returnfalse without scalar writes.",
    },
    "pressure_provenance_correction": {
        "aggregate": "1412e73d0 proves byte+6 = OR(byte+1c,byte+1d); environment reads global1446ee586.",
        "not_closed": "1412eae20 packet decoder calls1412e75b0; bounded leaf now proves that handler only sets+2f if payload+34>1 and maxes+44. It does not populate pressure flags+1c/+1d. Next trace their actual setters from global-record initialization/callers; no BSS/default0 inference.",
        "evidence": [record("analysis/wave2-save-diffs-300-pressure-record-loader.asm"),
            record("analysis/wave2-save-diffs-300-pressure-payload-decode.asm"),
            record("analysis/wave2-save-diffs-300-pressure-feature-aggregation.asm")],
        "decode_limit": "Payload decoder selection is entry-only; not whole logical procedure coverage.",
    },
    "validation": "Read-only fixed-PE selections and source reconciliation only. No source/test implementation, build, runtime, app control or fixture mutation. Public300 guard stays; next source changes require coordinator release.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest()}))
