"""Retain bounded original PE unwind-region disassembly without executing it."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prefix', required=True)
    parser.add_argument('--entry', action='append', required=True, type=lambda x: int(x, 0))
    parser.add_argument('--binary', default='DEF_GUI_MO_64.exe',
                        help='Filename in the immutable original-binaries evidence directory')
    parser.add_argument('--leaf-bytes', type=int,
                        help='Explicit bounded candidate span when an entry has no unwind record (1..256)')
    args = parser.parse_args()
    assert re.fullmatch(r'[a-z0-9_-]+', args.prefix)
    assert args.leaf_bytes is None or 1 <= args.leaf_bytes <= 256
    evidence = (ROOT / 'evidence/original-binaries').resolve()
    binary = (evidence / args.binary).resolve()
    assert binary.parent == evidence and binary.is_file()
    data = binary.read_bytes()
    u16 = lambda off: struct.unpack_from('<H', data, off)[0]
    u32 = lambda off: struct.unpack_from('<I', data, off)[0]
    nt = u32(0x3c)
    opt = nt + 24
    assert data[nt:nt+4] == b'PE\0\0' and u16(opt) == 0x20b
    base = struct.unpack_from('<Q', data, opt + 24)[0]
    section_start = opt + u16(nt + 20)
    sections = [struct.unpack_from('<IIII', data, section_start + 40*i + 8)
                for i in range(u16(nt + 6))]
    def offset(rva):
        for virtual_size, start, raw_size, raw in sections:
            if start <= rva < start + raw_size:
                return raw + rva - start
        raise ValueError(f'RVA {rva:x} has no file-backed section')
    rva, size = struct.unpack_from('<II', data, opt + 112 + 3*8)
    off = offset(rva)
    regions = {start: (end, unwind) for start, end, unwind in
               struct.iter_unpack('<III', data[off:off+size])}
    compiler_root = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC')
    versions = sorted(compiler_root.glob('*/bin/Hostx64/x64/dumpbin.exe'),
                      key=lambda p: tuple(int(x) for x in p.parents[3].name.split('.')))
    dumpbin = versions[-1]
    receipt_path = ROOT / f'analysis/{args.prefix}-static.json'
    assert not receipt_path.exists()
    rows = []
    for entry in args.entry:
        region = regions.get(entry - base)
        if region:
            end = base + region[0]
            boundary = 'unwind region'
        else:
            if args.leaf_bytes is None:
                raise ValueError(f'{entry:x} has no unwind entry; supply an explicit leaf candidate bound')
            end = entry + args.leaf_bytes
            boundary = 'explicit bounded leaf candidate; inspect RET/branch boundaries before claims'
        path = ROOT / f'analysis/{args.prefix}-{entry:x}.asm'
        assert not path.exists()
        command = [str(dumpbin), '/disasm:bytes', f'/range:0x{entry:x},0x{end-1:x}', str(binary)]
        result = subprocess.run(command, capture_output=True, check=True)
        path.write_bytes(result.stdout)
        rows.append(dict(entry=hex(entry), end_exclusive=hex(end), boundary=boundary,
                         artifact=str(path.relative_to(ROOT)), bytes=len(result.stdout),
                         sha256=hashlib.sha256(result.stdout).hexdigest(), command=command))
    receipt = dict(binary=str(binary.relative_to(ROOT)),
                   binary_sha256=hashlib.sha256(data).hexdigest(), captures=rows,
                   qualification='Unwind regions or explicit bounded leaf candidates as marked per capture; '
                   'neither alone proves the full logical function. Read-only original binary, no runtime execution.',
                   tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    receipt_path.write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps(dict(receipt=str(receipt_path.relative_to(ROOT)),
                          captures=[{k:r[k] for k in ['entry','end_exclusive','bytes']} for r in rows])))


if __name__ == '__main__':
    main()
