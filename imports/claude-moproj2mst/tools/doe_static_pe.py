"""Offline PE selections for DOE recovery; no target execution or engine connection.

RIP/call scans produce byte-pattern candidates, not disassembler-proven instruction
boundaries. Confirm candidate sites with a bounded DUMPBIN view before behavioral use.
"""
import argparse
import bisect
import hashlib
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"


class PE:
    def __init__(self, binary=BINARY):
        self.data = binary.read_bytes()
        pe = self.u32(0x3c)
        assert self.data[pe:pe+4] == b"PE\0\0"
        self.optional = pe+24
        assert self.u16(self.optional) == 0x20b
        self.base = self.u64(self.optional+24)
        start = self.optional+self.u16(pe+20)
        self.sections = []
        for i in range(self.u16(pe+6)):
            p = start+40*i
            self.sections.append({"name": self.data[p:p+8].rstrip(b"\0").decode(),
                "rva": self.u32(p+12), "size": self.u32(p+16), "offset": self.u32(p+20),
                "executable": bool(self.u32(p+36) & 0x20000000)})
        p, size = self.u32(self.optional+112+24), self.u32(self.optional+112+28)
        self.unwind = []
        for at in range(self.off(self.base+p), self.off(self.base+p)+size, 12):
            a, b, u = struct.unpack_from("<III", self.data, at)
            self.unwind.append((self.base+a, self.base+b, self.base+u))
        self.unwind.sort()
        self.starts = [x[0] for x in self.unwind]

    def u16(self, p): return struct.unpack_from("<H", self.data, p)[0]
    def u32(self, p): return struct.unpack_from("<I", self.data, p)[0]
    def u64(self, p): return struct.unpack_from("<Q", self.data, p)[0]

    def off(self, va):
        for s in self.sections:
            if s["rva"] <= va-self.base < s["rva"]+s["size"]:
                return s["offset"]+va-self.base-s["rva"]
        raise ValueError(f"VA not file-backed: {va:x}")

    def va(self, offset):
        for s in self.sections:
            if s["offset"] <= offset < s["offset"]+s["size"]:
                return self.base+s["rva"]+offset-s["offset"]
        raise ValueError("file offset outside sections")

    def bounds(self, va):
        i = bisect.bisect_right(self.starts, va)-1
        if i >= 0 and self.unwind[i][0] <= va < self.unwind[i][1]:
            a, b, u = self.unwind[i]
            return {"begin": hex(a), "end_exclusive": hex(b), "unwind_info": hex(u),
                    "qualification": "runtime unwind region, not proof of whole logical procedure"}
        return None

    def strings(self, literals):
        out = []
        for literal in literals:
            for encoding in ("ascii", "utf-16-le"):
                raw = literal.encode(encoding)+(b"\0" if encoding == "ascii" else b"\0\0")
                pos = self.data.find(raw)
                while pos >= 0:
                    try:
                        out.append({"text": literal, "encoding": encoding, "va": hex(self.va(pos)), "offset": pos})
                    except ValueError:
                        pass
                    pos = self.data.find(raw, pos+1)
        return out

    def references(self, targets):
        targets = set(targets)
        out = []
        for s in self.sections:
            if not s["executable"]: continue
            raw = self.data[s["offset"]:s["offset"]+s["size"]]
            for match in re.finditer(b"[\x40-\x4f][\x8d\x8b][\x05\x0d\x15\x1d\x25\x2d\x35\x3d]", raw):
                i = match.start()
                if i+7 > len(raw): continue
                va = self.base+s["rva"]+i
                target = va+7+struct.unpack_from("<i", raw, i+3)[0]
                if target in targets:
                    out.append({"candidate_site": hex(va), "target": hex(target), "kind": "RIP-LEA/MOV-pattern",
                                "bytes": raw[i:i+7].hex(), "unwind_region": self.bounds(va)})
            for match in re.finditer(b"[\xe8\xe9]", raw):
                i = match.start()
                if i+5 > len(raw): continue
                va = self.base+s["rva"]+i
                target = va+5+struct.unpack_from("<i", raw, i+1)[0]
                if target in targets:
                    out.append({"candidate_site": hex(va), "target": hex(target), "kind": "CALL/JMP-rel32-pattern",
                                "bytes": raw[i:i+5].hex(), "unwind_region": self.bounds(va)})
        return out


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--string", action="append", default=[])
    parser.add_argument("--address", action="append", default=[])
    parser.add_argument("--references", action="store_true")
    parser.add_argument("--output", required=True, help="analysis/wave2-doe*.json only")
    args = parser.parse_args()
    output = Path(args.output).resolve()
    assert output.parent == ROOT / "analysis" and output.name.startswith("wave2-doe") and output.suffix == ".json"
    pe = PE()
    strings = pe.strings(args.string)
    addresses = [int(x, 0) for x in args.address]
    result = {"target": str(BINARY), "target_sha256": hashlib.sha256(pe.data).hexdigest(),
              "tool": str(Path(__file__).relative_to(ROOT)), "tool_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "strings": strings, "unwind_regions": {hex(x): pe.bounds(x) for x in addresses},
              "references": pe.references(addresses+[int(x["va"],0) for x in strings]) if args.references else [],
              "qualification": "offline byte-pattern candidates; validate instruction boundaries/semantics with disassembly; no runtime or REA connection"}
    output.write_text(json.dumps(result, indent=2)+"\n", encoding="utf-8")
    print(json.dumps({"output": str(output), "sha256": hashlib.sha256(output.read_bytes()).hexdigest(),
                      "strings": strings, "unwind_regions": result["unwind_regions"], "reference_count": len(result["references"])}))


if __name__ == "__main__":
    main()
