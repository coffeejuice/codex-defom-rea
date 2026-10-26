"""Read-only RIP-relative LEA/.pdata hints for explicit original ASCII strings.

These are instruction-pattern hints, not a control-flow graph or a decompiler.
No cache/database is opened or written and no target code is executed.
"""
import argparse
import bisect
import hashlib
import json
from pathlib import Path
import re
import struct


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=Path)
    parser.add_argument("strings", nargs="*")
    parser.add_argument("--call-target", action="append", default=[], type=lambda s: int(s, 0))
    parser.add_argument("--lea-target", action="append", default=[], type=lambda s: int(s, 0))
    parser.add_argument("--write-target", action="append", default=[], type=lambda s: int(s, 0))
    args = parser.parse_args()
    data = args.binary.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    opt = pe + 24
    sh = opt + struct.unpack_from("<H", data, pe + 20)[0]
    base = struct.unpack_from("<Q", data, opt + 24)[0]
    sections = []
    for i in range(struct.unpack_from("<H", data, pe + 6)[0]):
        pos = sh + 40 * i
        vs, va, rs, ra = struct.unpack_from("<IIII", data, pos + 8)
        sections.append((data[pos:pos + 8].rstrip(b"\0").decode(), va, vs, ra, rs))
    text = next(s for s in sections if s[0] == ".text")
    pdata = next(s for s in sections if s[0] == ".pdata")
    functions = []
    for pos in range(pdata[3], pdata[3] + pdata[4], 12):
        start, end, _ = struct.unpack_from("<III", data, pos)
        if start:
            functions.append((base + start, base + end))
    starts = [f[0] for f in functions]
    refs = {}
    for match in re.finditer(rb"[\x48\x4c]\x8d[\x05\x0d\x15\x1d\x25\x2d\x35\x3d]",
                             data[text[3]:text[3] + text[4]], re.S):
        pos = match.start()
        va = base + text[1] + pos
        target = va + 7 + struct.unpack_from("<i", data, text[3] + pos + 3)[0]
        index = bisect.bisect_right(starts, va) - 1
        fn = functions[index] if index >= 0 and va < functions[index][1] else None
        refs.setdefault(target, []).append(dict(instruction=hex(va), function=[hex(v) for v in fn] if fn else None))
    results = []
    for value in args.strings:
        offset = data.find(value.encode("ascii"))
        target = None
        if offset >= 0:
            for _, va, _, ra, rs in sections:
                if ra <= offset < ra + rs:
                    target = base + va + offset - ra
                    break
        results.append(dict(string=value, file_offset=hex(offset) if offset >= 0 else None,
                            va=hex(target) if target else None, lea_hints=refs.get(target, [])))
    calls = []
    for target in args.call_target:
        hints = []
        for match in re.finditer(b"\xe8", data[text[3]:text[3] + text[4]]):
            pos = match.start()
            va = base + text[1] + pos
            destination = va + 5 + struct.unpack_from("<i", data, text[3] + pos + 1)[0]
            if destination != target:
                continue
            index = bisect.bisect_right(starts, va) - 1
            fn = functions[index] if index >= 0 and va < functions[index][1] else None
            hints.append(dict(instruction=hex(va), function=[hex(v) for v in fn] if fn else None))
        own = functions[starts.index(target)] if target in starts else None
        calls.append(dict(target=hex(target), function=[hex(v) for v in own] if own else None,
                          direct_call_hints=hints))
    writes = []
    for target in args.write_target:
        hints = []
        raw = data[text[3]:text[3] + text[4]]
        for match in re.finditer(rb"[\x88\x89\xc6\xc7][\x05\x0d\x15\x1d\x25\x2d\x35\x3d]", raw):
            pos = match.start()
            opcode = raw[pos]
            size = 7 if opcode == 0xc6 else 10 if opcode == 0xc7 else 6
            va = base + text[1] + pos
            destination = va + size + struct.unpack_from("<i", raw, pos + 2)[0]
            if destination != target:
                continue
            index = bisect.bisect_right(starts, va) - 1
            fn = functions[index] if index >= 0 and va < functions[index][1] else None
            hints.append(dict(instruction=hex(va), function=[hex(v) for v in fn] if fn else None,
                              opcode=hex(opcode), immediate=raw[pos+6] if opcode == 0xc6 else None))
        writes.append(dict(target=hex(target), rip_store_hints=hints))
    print(json.dumps(dict(binary=str(args.binary.resolve()), sha256=hashlib.sha256(data).hexdigest(),
                          limitation="RIP-relative LEA and direct CALL pattern hints only; no target execution",
                          results=results, calls=calls,
                          explicit_lea_targets=[dict(target=hex(t), lea_hints=refs.get(t, []))
                                                for t in args.lea_target],
                          explicit_write_targets=writes), indent=2))


if __name__ == "__main__":
    main()
