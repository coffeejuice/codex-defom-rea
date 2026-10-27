"""Bind selected CP445 Ghidra instruction and literal bytes to the imported PE."""

from __future__ import annotations

import hashlib
import json
import pathlib
import re
import struct


ROOT = pathlib.Path(__file__).resolve().parents[1]
PE = ROOT / "evidence/gui_mo/DEF_GUI_MO_64.exe"
ASM = ROOT / "analysis/cp445-mirrormerge-writer-range.asm"
INSTRUCTIONS = {
    0x140E24413,  # first +e8 record load
    0x140E2442C,  # distinct +f0 record load
    0x140E2471C,  # first +e8 formatting argument
    0x140E2479D,  # repeated +e8 formatting argument
}
LITERALS = {
    0x142ABC114: b"\x09\x00",
    0x142ABC274: b"\x09\x00",
    0x142ABC48C: b"\x09\x00",
    0x142ABCB1C: b"\x09\x00",
    0x142ABDAE4: b"\x20\x00",
    0x142ABDB94: b"\x20\x00",
    0x142ABDF2C: b"\x20\x00",
    0x142ABE6BC: b"\x20\x00",
    0x142ABE87C: b"\x20\x00",
}


def main() -> None:
    data = PE.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    assert data[pe : pe + 4] == b"PE\x00\x00"
    sections = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    assert struct.unpack_from("<H", data, optional)[0] == 0x20B
    base = struct.unpack_from("<Q", data, optional + 24)[0]
    section_table = optional + optional_size
    spans = []
    for i in range(sections):
        s = section_table + i * 40
        virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, s + 8)
        spans.append((rva, raw_offset, min(virtual_size, raw_size)))

    def file_offset(va: int, count: int) -> int:
        rva = va - base
        for start, raw, length in spans:
            if start <= rva and rva + count <= start + length:
                return raw + rva - start
        raise ValueError(f"VA outside PE raw section: {va:#x}")

    seen: dict[int, str] = {}
    for line in ASM.read_text(encoding="utf-8").splitlines():
        match = re.match(r"^([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s", line)
        if not match:
            continue
        va = int(match.group(1), 16)
        if va not in INSTRUCTIONS:
            continue
        expected = bytes.fromhex(match.group(2))
        actual = data[file_offset(va, len(expected)) : file_offset(va, len(expected)) + len(expected)]
        assert actual == expected, f"instruction mismatch at {va:#x}"
        seen[va] = expected.hex()
    assert set(seen) == INSTRUCTIONS, "missing decisive instruction(s)"
    for va, expected in LITERALS.items():
        actual = data[file_offset(va, len(expected)) : file_offset(va, len(expected)) + len(expected)]
        assert actual == expected, f"literal mismatch at {va:#x}"
    print(json.dumps({
        "pe_sha256": hashlib.sha256(data).hexdigest(),
        "assembly_sha256": hashlib.sha256(ASM.read_bytes()).hexdigest(),
        "image_base": hex(base),
        "matched_instructions": {hex(k): v for k, v in sorted(seen.items())},
        "matched_literals": {hex(k): v.hex() for k, v in sorted(LITERALS.items())},
    }, indent=2))


if __name__ == "__main__":
    main()
