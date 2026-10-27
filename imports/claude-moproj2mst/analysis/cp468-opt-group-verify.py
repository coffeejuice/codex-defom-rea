"""Reproduce bounded original PE bytes for the Type3 OPT group-list identity trace."""

from pathlib import Path
import hashlib
import json
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]
DUMPBIN = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
PREFIX = 'cp468-opt-group'


class PE:
    def __init__(self, path):
        self.path = path
        self.data = path.read_bytes()
        pe = struct.unpack_from('<I', self.data, 0x3c)[0]
        optional = pe + 24
        self.base = struct.unpack_from('<Q', self.data, optional + 24)[0]
        section_count = struct.unpack_from('<H', self.data, pe + 6)[0]
        section_start = optional + struct.unpack_from('<H', self.data, pe + 20)[0]
        self.sections = []
        for index in range(section_count):
            vs, va, rs, rp = struct.unpack_from('<IIII', self.data, section_start + 40 * index + 8)
            self.sections.append((va, max(vs, rs), rp))
        exception_rva, exception_size = struct.unpack_from('<II', self.data, optional + 112 + 24)
        start = self.offset(self.base + exception_rva)
        self.functions = [struct.unpack_from('<III', self.data, start + i)
                          for i in range(0, exception_size, 12)]

    def offset(self, address):
        rva = address - self.base
        for va, size, raw in self.sections:
            if va <= rva < va + size:
                return raw + rva - va
        raise ValueError(hex(address))

    def read(self, address, length):
        start = self.offset(address)
        value = self.data[start:start + length]
        assert len(value) == length
        return value

    def bounds(self, address):
        rva = address - self.base
        for first, last, unwind in self.functions:
            if first == rva:
                return self.base + last, self.base + unwind
        raise ValueError(hex(address))


SPECS = [
    ('DEF_GUI_MO_64.exe', 0x14003f680, 0x14003f6d2, 'e1a4a6c78e88d28a717e828bfee8faea088049af84adcfe6ee6a8d0981dcb850'),
    ('DEF_GUI_MO_64.exe', 0x140025100, 0x1400251d0, '76727d5c0772819f4437ee1ffd800efc9acf3225e8fad266cbeab1c8021c58fd'),
    ('DEF_GUI_MO_64.exe', 0x1400250c0, 0x1400250f8, '6090029f4e592e1dd35485aa4a81e92af942e50e2d9b02effbaa9b8eb5a8828d'),
    ('DEF_GUI_MO_64.exe', 0x1400253c0, 0x1400254df, '373f3083372e6e32ba3054d3fdf552059261a08e12b0b041cb3bbdbd958aea11'),
    ('DEF_GUI_MO_64.exe', 0x140025580, 0x140025643, '95cc6f8ecb1a4832eb0408b7b4f2618f3f268aef5ee38df7e8a7e2b62dcad290'),
    ('QtCore4_64.dll', 0x67048880, 0x67048942, '711f231c483d1be3d27bb1373ba09beebb9e7a17f5037925f5c7889b91514727'),
]


def main():
    assert DUMPBIN.exists()
    pe_files = {name: PE(ROOT / 'evidence/original-binaries' / name)
                for name in {spec[0] for spec in SPECS}}
    assert hashlib.sha256(pe_files['DEF_GUI_MO_64.exe'].data).hexdigest() == 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
    assert hashlib.sha256(pe_files['QtCore4_64.dll'].data).hexdigest() == 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160'
    verified = []
    for name, start, end, expected in SPECS:
        pe = pe_files[name]
        actual_end, unwind = pe.bounds(start)
        assert actual_end == end, hex(start)
        raw = pe.read(start, end - start)
        assert hashlib.sha256(raw).hexdigest() == expected, hex(start)
        output = subprocess.check_output(
            [str(DUMPBIN), '/disasm:bytes', f'/range:{start:#x},{end-1:#x}', str(pe.path)],
            text=True)
        chunks = []
        current = None
        for line in output.splitlines():
            match = re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)', line)
            if match:
                current = [int(match[1], 16), bytearray.fromhex(match[2])]
                chunks.append(current)
            elif current and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+', line):
                current[1].extend(bytes.fromhex(line))
        assert chunks and chunks[0][0] == start
        assert chunks[-1][0] + len(chunks[-1][1]) == end
        for (address, instruction), following in zip(chunks, chunks[1:] + [[end, None]]):
            assert address + len(instruction) == following[0]
            assert pe.read(address, len(instruction)) == instruction
        artifact = ROOT / 'analysis' / f'{PREFIX}-{start:x}.asm'
        artifact.write_text(output, encoding='utf-8')
        verified.append(dict(binary=name, start=hex(start), end_exclusive=hex(end),
                             unwind_info=hex(unwind), bytes=len(raw), instructions=len(chunks),
                             raw_sha256=expected, artifact=str(artifact.relative_to(ROOT)),
                             artifact_sha256=hashlib.sha256(artifact.read_bytes()).hexdigest(),
                             mismatches=0))
    receipt = dict(checkpoint='CP468 Type3 OPT group-list identity',
                   binaries={name: hashlib.sha256(pe.data).hexdigest() for name, pe in pe_files.items()},
                   method='Bounded dumpbin instruction bytes mapped to original PE sections and complete exception ranges',
                   exports=verified, compared_bytes=sum(x['bytes'] for x in verified),
                   addressed_instructions=sum(x['instructions'] for x in verified),
                   mismatches=0, original_executed=False)
    path = ROOT / 'analysis' / f'{PREFIX}-byte-verification.json'
    path.write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: receipt[k] for k in ('compared_bytes', 'addressed_instructions', 'mismatches')}))


if __name__ == '__main__':
    main()
