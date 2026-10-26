"""Read explicit-address PE constants without loading or executing the target."""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=Path)
    for kind in ("ascii", "double", "int32"):
        parser.add_argument("--" + kind, action="append", default=[], type=lambda v: int(v, 0))
    args = parser.parse_args()
    data = args.binary.read_bytes()
    pe = struct.unpack_from("<I", data, 60)[0]
    opt = pe + 24
    base = struct.unpack_from("<Q", data, opt + 24)[0]
    sh = opt + struct.unpack_from("<H", data, pe + 20)[0]
    sections = []
    for i in range(struct.unpack_from("<H", data, pe + 6)[0]):
        _, va, size, raw = struct.unpack_from("<IIII", data, sh + 40 * i + 8)
        sections.append((va, size, raw))
    result = []
    for kind in ("ascii", "double", "int32"):
        for va in getattr(args, kind):
            rva = va - base
            pos = next(raw + rva - start for start, size, raw in sections if start <= rva < start + size)
            if kind == "ascii":
                end = data.index(0, pos, pos + 4096)
                value = data[pos:end].decode("ascii")
            else:
                value = struct.unpack_from("<d" if kind == "double" else "<i", data, pos)[0]
            result.append({"va": hex(va), "file_offset": hex(pos), "kind": kind, "value": value})
    print(json.dumps({"binary": str(args.binary), "sha256": hashlib.sha256(data).hexdigest(), "values": result}, indent=2))


if __name__ == "__main__":
    main()
