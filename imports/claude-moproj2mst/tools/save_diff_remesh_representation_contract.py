"""Derived bounded remesh overwrite vectors; does not execute the original."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-remesh-representation-contract.json"
if TARGET.exists():
    raise SystemExit("Immutable remesh representation contract already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


def vector(token, bits, flag):
    raw = bytearray(bits.to_bytes(8, "little") + bytes([flag]))
    prefix = token.encode("ascii") + b"\0"
    assert len(prefix) <= 9
    raw[:len(prefix)] = prefix
    return {"token": token, "before_double_bits": f"{bits:016x}",
        "before_contact_byte": flag, "after_nine_bytes": raw.hex(),
        "after_double_bits": f"{int.from_bytes(raw[:8], 'little'):016x}",
        "after_contact_byte": raw[8]}


receipt = {
    "status": "read_only_representation_contract_before_source_implementation",
    "lineage": record("analysis/wave2-save-diffs-300-remesh-raw-contract.json"),
    "evidence": [record(path) for path in [
        "analysis/wave2-save-diffs-300-remesh-ctor.asm",
        "analysis/wave2-save-diffs-300-remesh-reader.asm",
        "analysis/wave2-save-diffs-300-remesh-token-cases.json",
        "analysis/wave2-save-diffs-300-remesh-token-dispatch.asm",
        "analysis/wave2-save-diffs-300-remesh-token-copy.asm",
        "analysis/wave2-save-diffs-300-support-1400abcc0.c",
        "analysis/wave2-save-diffs-300-remesh-file-literals.json",
    ]],
    "extension": "Prior strict profile<=7 avoids any spill. A9-byte modeled span extends selectedtoken<=8 safely: bytes0..7 are original uniform double representation+10..17, byte8 is known contact flag+18. Token length8 writes only terminatingNUL to contact flag, which is definedfalse. No arbitrary bool representation or padding write occurs. This covers original writer default0.000000(8chars).",
    "algorithm": "Seed array<byte,9> from current double object bytes and supported bool normalized0/1. Require selectedtoken length<=8 and no embeddedNUL (original TokenReader-selected Cstring profile). Copy exact byte prefix plusNUL. Copy first8bytes back through memcpy/bit_cast. Contact flag is unchanged for lengths<=7 and explicitlyfalse forlength8. No atof/strtod/number formatting or entire-field zeroing. Every other RemeshState field stays untouched.",
    "guards": "Selectedtoken>=9 or embeddedNUL outside source-generated token profile refuses before this helper's writes. Earlier stream/keyword effects must remain in a future reader; do not transactionalize. Longer source effects may be recoverable with a complete byte layout and writer promotion, but are not represented by writing arbitrary bytes into C++bool.",
    "derived_vectors": [
        vector("1", 0x3ff4000000000000, 1),
        vector("0.25", 0, 1),
        vector("0.000000", 0x3ff4000000000000, 1),
        vector("1", int.from_bytes(b"0.000000", "little"), 0),
        vector("", 0x7ff8000000000031, 1),
    ],
    "test_plan": "Bit-exact prefix/suffix and duplicate-token composition; length8 contact clear; no neighboring state changes; unsupported longtoken has no helper writes; signedzero/NaN representations copied without arithmetic. Vectors are derived from proven copy instructions, not captured original outputs.",
    "successor": "After typed helper tests, add nativeCRT remesh reader carrying exact tokenization and earlier effects, then actual root-owned project Open/Save binding. Missing/empty-file success/failure and unit-independent field defaults remain exact source contracts; no numeric fix for the original bug.",
    "limits": "No production/test source changes during root build freeze, no runtime/app control/build/test execution/fixture mutation. No complete static object-size or public Save integration claim.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest()}))
