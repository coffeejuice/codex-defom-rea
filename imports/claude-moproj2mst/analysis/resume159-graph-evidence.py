"""CP159 immutable-PE range/import qualification; no target execution."""
import hashlib
import json
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[1]
binary = root / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
data = binary.read_bytes()
u16 = lambda p: struct.unpack_from('<H', data, p)[0]
u32 = lambda p: struct.unpack_from('<I', data, p)[0]
pe = u32(0x3c)
opt = pe + 24
base = struct.unpack_from('<Q', data, opt + 24)[0]
sh = opt + u16(pe + 20)
sections = [struct.unpack_from('<IIII', data, sh + i*40 + 8) for i in range(u16(pe+6))]
def off(rva):
    return next(raw+rva-va for _, va, size, raw in sections if va <= rva < va+size)
def string(rva):
    p = off(rva)
    return data[p:data.index(0, p)].decode('ascii', errors='replace')
imports = {}
p = off(u32(opt+112+8))
while True:
    original, _, _, name, first = struct.unpack_from('<IIIII', data, p)
    if not any((original, name, first)):
        break
    i = 0
    while True:
        thunk = struct.unpack_from('<Q', data, off(original or first)+i*8)[0]
        if not thunk:
            break
        imports[base+first+i*8] = {'dll':string(name), 'name':f'ordinal:{thunk&65535}' if thunk>>63 else string(thunk+2)}
        i += 1
    p += 20
pdata_rva, pdata_size = struct.unpack_from('<II', data, opt+112+3*8)
regions = {base+a:base+b for a,b,_ in struct.iter_unpack('<III',data[off(pdata_rva):off(pdata_rva)+pdata_size])}
captures = []
used = {}
literals = {}
leaf_ends = {0x140d41b30:0x140d41b3e, 0x1411eebd0:0x1411eec02}
for path in sorted((root/'analysis').glob('resume159-graph-*.asm')):
    text = path.read_text()
    start = int(path.stem.rsplit('-',1)[1],16)
    end = regions.get(start) or leaf_ends.get(start)
    if end is None:
        continue
    raw = data[off(start-base):off(end-base-1)+1]
    captures.append({'artifact':str(path.relative_to(root)), 'start':hex(start), 'end_exclusive':hex(end),
                     'file_start':hex(off(start-base)), 'file_end_exclusive':hex(off(end-base-1)+1),
                     'machine_bytes':len(raw), 'machine_sha256':hashlib.sha256(raw).hexdigest(),
                     'assembly_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
                     'boundary':'pdata region' if start in regions else 'decoded leaf through first terminal RET; capture includes later bytes',
                     'retained_body_completed':bool(re.search(r'\bret\b',text))})
    for m in re.finditer(r'([0-9A-F]{16}):.*?call\s+qword ptr \[([0-9A-F]+)h\]',text):
        addr = int(m[2],16)
        if addr in imports:
            entry = used.setdefault(hex(addr), {**imports[addr], 'sites':[]})
            entry['sites'].append(hex(int(m[1],16)))
    if start == 0x140d78490:
        for m in re.finditer(r'lea\s+rdx,\[([0-9A-F]+)h\]',text):
            addr = int(m[1],16)
            if 0x142000000 <= addr < 0x143000000:
                literals[hex(addr)] = string(addr-base)
vtable_address = 0x14210eb18
vtable_offset = off(vtable_address-base)
vtable = []
for i in range(5):
    target = struct.unpack_from('<Q', data, vtable_offset+i*8)[0]
    entry = {'slot':hex(i*8),'target':hex(target)}
    t = off(target-base)
    entry['target_first6_hex'] = data[t:t+6].hex()
    if data[t:t+2] == b'\xff\x25':
        iat = target+6+struct.unpack_from('<i',data,t+2)[0]
        entry['iat'] = hex(iat)
        entry['import'] = imports.get(iat)
    vtable.append(entry)
result = {'binary':str(binary.relative_to(root)), 'binary_sha256':hashlib.sha256(data).hexdigest(),
          'dumpbin':{'path':r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe',
                     'banner':'Microsoft (R) COFF/PE Dumper Version 14.44.35228.0',
                     'sha256':hashlib.sha256(Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe').read_bytes()).hexdigest()},
          'method':'Read-only standard-library PE parsing plus retained dumpbin /disasm:bytes output',
          'captures':captures, 'imports':used, 'd78490_ascii_literals':literals,
          'item_vtable':{'va':hex(vtable_address),'file_start':hex(vtable_offset),'bytes':40,'sha256':hashlib.sha256(data[vtable_offset:vtable_offset+40]).hexdigest(),'entries':vtable},
          'qualification':'Static instruction/import evidence only; a terminal RET in each unwind region does not prove all logical-function cold paths. No target runtime, GUI or mutable analysis database.',
          'capture_note':'Initial three-body capture exited 0. Delegate capture produced two complete bodies then exited 1 because d41b30 has no unwind record. Role capture produced d7ad70 then exited 1 because 1411eebd0 has no unwind record. Both leaves were subsequently captured successfully with explicit bounds; interpreted leaf spans stop at their decoded terminal RET, excluding following functions and partial trailing instructions.'}
out = root/'analysis/resume159-graph-evidence.json'
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'receipt':str(out.relative_to(root)), 'captures':len(captures), 'imports':len(used), 'literals':len(literals)}))
