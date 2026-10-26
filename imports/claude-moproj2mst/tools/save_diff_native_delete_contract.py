"""Retain immutable original narrow CRT delete evidence and bounded repair."""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
from doe_static_pe import PE

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-native-io-repair-contract.json"
ASM = ROOT / "analysis/wave2-save-diffs-300-native-delete-original.asm"
if TARGET.exists() or ASM.exists():
    raise SystemExit("Immutable repair evidence already exists; choose a new checkpoint name.")
pe = PE()
dumpbin = Path("C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe")
binary = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"
ranges = [
    ("delete helper", 0x1407fbc10, 0x1407fbc6d),
    ("fopen sets sharing0x40", 0x14146fcd8, 0x14146fce3),
    ("fsopen narrow byte checks", 0x14146fba0, 0x14146fcd5),
    ("fclose", 0x14146fb28, 0x14146fb9e),
    ("remove and forwarding entry", 0x14146fd48, 0x14146fd7e),
    ("rt parser -> narrow sopen", 0x1414820ac, 0x141482374),
    ("sopen wrapper", 0x1414883ec, 0x14148841e),
    ("sopen dispatcher", 0x1414882b4, 0x1414883ea),
    ("sopen leaf -> CreateFileA", 0x1414879c4, 0x1414881c2),
]
captures = []
for label, begin, end in ranges:
    result = subprocess.run([str(dumpbin), "/DISASM", f"/RANGE:0x{begin:x},0x{end:x}", str(binary)],
        cwd=ROOT, capture_output=True, text=True, check=True)
    captures.append(f"; {label}; bounded range {begin:x}..{end:x}\n{result.stdout}\n")
ASM.write_text("\n".join(captures), encoding="utf-8")

imports = []
offset = pe.off(pe.base + pe.u32(pe.optional + 120))
while pe.u32(offset):
    ilt, _, _, name, iat = struct.unpack_from("<IIIII", pe.data, offset)
    dll = pe.data[pe.off(pe.base + name):].split(b"\0", 1)[0].decode()
    index = 0
    while pe.u64(pe.off(pe.base + (ilt or iat)) + 8 * index):
        value = pe.u64(pe.off(pe.base + (ilt or iat)) + 8 * index)
        va = pe.base + iat + 8 * index
        if va in (0x141e03210, 0x141e03280, 0x141e035e8):
            symbol = pe.data[pe.off(pe.base + value) + 2:].split(b"\0", 1)[0].decode()
            imports.append({"iat_va": hex(va), "dll": dll, "symbol": symbol})
        index += 1
    offset += 20


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300_io.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp",
]
cases = re.findall(r"TEST_CASE\(([^)]+)\)", (ROOT / sources[-1]).read_text())
mode = pe.data[pe.off(0x14385aee4):].split(b"\0", 1)[0].decode("ascii")
receipt = {
    "status": "focused_native_delete_repair_frozen_for_root_build",
    "original_sha256": hashlib.sha256(pe.data).hexdigest(),
    "tool": record("tools/save_diff_native_delete_contract.py"),
    "original_evidence": record(str(ASM.relative_to(ROOT)).replace("\\", "/")),
    "bounded_functions": [{"name": name, "begin": hex(begin), "end": hex(end),
        "unwind": pe.bounds(begin)} for name, begin, end in ranges],
    "imports": imports,
    "mode_literal": {"va": "0x14385aee4", "ascii": mode},
    "original_contract": [
        "1407fbc10 passes its char* argument unchanged to static fopen14146fcd8 with narrow mode rt, then fclose14146fb28 and remove14146fd78.",
        "fopen forwarding entry sets sharing0x40 (_SH_DENYNO); fsopen checks filename and mode as individual bytes, not UTF16.",
        "For rt, parsed file flags are text0x4000, read-only0. Native sopen leaf calls imported CreateFileA with GENERIC_READ, FILE_SHARE_READ|WRITE3 (no DELETE), OPEN_EXISTING3, FILE_ATTRIBUTE_NORMAL0x80, SECURITY_ATTRIBUTES length24/null descriptor/inheritTRUE.",
        "remove14146fd78 forwards to14146fd48 and imported DeleteFileA, using the same narrow char* filename unchanged.",
        "fopen failure returns true; successful fopen is closed before remove; remove0 returns true, any remove failure returns false. No pre-delete QFile exists test.",
        "Extra3D callers pass fixed ASCII relative sidecar literals. No QString-to-narrow directory encoding is established by this helper; original CWD is part of its lifecycle.",
    ],
    "repair": [
        "Replace _wfopen/_wremove and extended win32Path substitution with std::fopen(nativeASCII,rt), std::fclose and std::remove.",
        "Factory guard requires fully absolute ordinary drive or UNC directory, ASCII without control/NUL, no extended/device namespace, and length including slash plus longest fixed sidecar below MAX_PATH. No file mutation precedes this guard.",
        "Delete also guards its resolved filename to prevent arbitrary supplied Unicode helper paths from silently using different semantics.",
        "The ASCII/ordinary-path guard is this adapter's supported profile. It does not claim the original rejects ASCII relative sidecars under Unicode or long CWD; that original lifecycle mapping remains a recovery dependency.",
    ],
    "source_v2": [record(path) for path in sources],
    "prior_v1": record("analysis/wave2-save-diffs-300-native-io-increment.json"),
    "validation": {"v1": "Parent reports buildv1 and10 focused CTests passed, native backend7 cases passed; superseded source checkpoint, not accepted final validation.",
        "v2": "Not run by worker; root owns rebuild/tests. git diff --check passed before freeze.",
        "native_cases": cases, "native_cases_count": len(cases), "expected_editor_helper_cases": 40 + len(cases)},
    "qualification": "Static PE reads and bounded DUMPBIN only; no original runtime, DLL loading, app control or fixture mutations. Current runtime CRT resource-limit behavior is not proved identical to the original static CRT. Public editor300 Save remains refused, and other numeric/codec/spring profile guards remain explicit.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": str(TARGET.relative_to(ROOT)), "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(),
    "asm_sha256": hashlib.sha256(ASM.read_bytes()).hexdigest(), "native_cases": len(cases), "imports": imports}))
