"""Retain bounded fresh disassembly and verify inherited connector assembly."""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"
DUMPBIN = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe")


def main():
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    assert digest == "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc"
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    opt = pe + 24
    count = struct.unpack_from("<H", data, pe + 6)[0]
    opt_size = struct.unpack_from("<H", data, pe + 20)[0]
    base = struct.unpack_from("<Q", data, opt + 24)[0]
    sections = [struct.unpack_from("<IIII", data, opt + opt_size + i * 40 + 8) for i in range(count)]

    def offset(va):
        rva = va - base
        for vs, start, size, raw in sections:
            if start <= rva < start + min(vs, size):
                return raw + rva - start
        raise ValueError(hex(va))

    pdata_rva, pdata_size = struct.unpack_from("<II", data, opt + 112 + 3 * 8)
    pdata = offset(base + pdata_rva)
    ranges = {}
    for at in range(pdata, pdata + pdata_size, 12):
        start, end, unwind = struct.unpack_from("<III", data, at)
        ranges[base + start] = base + end
    functions = []
    for entry in (0x140074F50, 0x14015EF80, 0x140161470):
        end = ranges[entry]
        output = ROOT / "analysis" / f"connector-{entry:x}-disassembly.txt"
        if output.exists():
            raise FileExistsError(output)
        command = [str(DUMPBIN), "/DISASM:BYTES", f"/RANGE:{entry:#x},{end - 1:#x}", str(TARGET)]
        with output.open("w", encoding="utf-8") as f:
            subprocess.run(command, stdout=f, stderr=subprocess.STDOUT, check=True)
        functions.append(dict(entry=hex(entry), end_exclusive=hex(end),
                              code_sha256=hashlib.sha256(data[offset(entry):offset(end - 1) + 1]).hexdigest(),
                              disassembly=str(output.relative_to(ROOT))))
    inherited = []
    for name in ("conn.asm", "conn3.asm"):
        path = ROOT / "evidence/gui_mo/L5d" / name
        checked = 0
        for line in path.read_text().splitlines():
            match = re.match(r"^([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s", line)
            if match:
                at = offset(int(match[1], 16))
                expected = bytes.fromhex(match[2])
                assert data[at:at + len(expected)] == expected, line
                checked += 1
        inherited.append(dict(path=str(path.relative_to(ROOT)), records_checked=checked,
                              mismatches=0, sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    strings = {}
    for va in (0x1421D2CE8, 0x1421D2CF8, 0x1421D2D08, 0x1421D2D18, 0x1421D2D28, 0x1421D2D38):
        at = offset(va)
        strings[hex(va)] = data[at:data.index(b"\0", at)].decode("ascii")
    receipt = dict(kind="static_byte_verification", target_sha256=digest,
                   functions=functions, inherited_assembly=inherited, strings=strings)
    out = ROOT / "analysis/connector-static-evidence.json"
    out.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(receipt, indent=2))


if __name__ == "__main__":
    main()
