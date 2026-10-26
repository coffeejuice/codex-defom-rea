"""Read PE import identities for IAT addresses used in selected static disassembly.

No target code, analysis database, or runtime session is opened.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=Path)
    parser.add_argument("assembly", type=Path, nargs="+")
    args = parser.parse_args()
    data = args.binary.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    opt = pe + 24
    base = struct.unpack_from("<Q", data, opt + 24)[0]
    sh = opt + struct.unpack_from("<H", data, pe + 20)[0]
    sections = []
    for i in range(struct.unpack_from("<H", data, pe + 6)[0]):
        _, va, raw_size, raw = struct.unpack_from("<IIII", data, sh + 40 * i + 8)
        sections.append((va, raw_size, raw))

    def offset(rva):
        return next(raw + rva - va for va, size, raw in sections if va <= rva < va + size)

    def string(rva):
        pos = offset(rva)
        return data[pos:data.index(0, pos)].decode("ascii")

    imports = {}
    rva, _ = struct.unpack_from("<II", data, opt + 112 + 8)
    desc = offset(rva)
    while True:
        original, _, _, name, first = struct.unpack_from("<IIIII", data, desc)
        if not any((original, name, first)):
            break
        dll = string(name)
        pos = offset(original or first)
        i = 0
        while True:
            thunk = struct.unpack_from("<Q", data, pos + i * 8)[0]
            if not thunk:
                break
            imports[base + first + 8 * i] = {
                "dll": dll,
                "import": "ordinal:" + str(thunk & 0xffff) if thunk >> 63 else string(thunk + 2),
            }
            i += 1
        desc += 20
    selected = {}
    for path in args.assembly:
        for match in re.finditer(r"([0-9A-F]+): call\s+qword ptr \[([0-9A-F]+)h\]",
                                 path.read_text(encoding="utf-8-sig")):
            address = int(match[2], 16)
            if address in imports:
                entry = selected.setdefault(address, dict(imports[address], calls=[]))
                entry["calls"].append({"path": str(path), "instruction": hex(int(match[1], 16))})
    print(json.dumps({"binary": str(args.binary), "sha256": hashlib.sha256(data).hexdigest(),
                      "imports": [{"iat": hex(k), **v} for k, v in sorted(selected.items())]}, indent=2))


if __name__ == "__main__":
    main()
