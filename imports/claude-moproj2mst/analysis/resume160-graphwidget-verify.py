"""Read-only PE qualification for CP160 graph widget owner investigation."""
from pathlib import Path
import bisect
import hashlib
import json
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
binary = ROOT / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
data = binary.read_bytes()
u16 = lambda p: struct.unpack_from('<H', data, p)[0]
u32 = lambda p: struct.unpack_from('<I', data, p)[0]
nt = u32(0x3c)
opt = nt + 24
base = struct.unpack_from('<Q', data, opt + 24)[0]
sh = opt + u16(nt + 20)
sections = [struct.unpack_from('<IIII', data, sh + 40*i + 8) for i in range(u16(nt+6))]
def off(rva):
    return next(raw+rva-va for _,va,size,raw in sections if va <= rva < va+size)
def string(rva):
    p = off(rva)
    return data[p:data.index(0,p)].decode('ascii', errors='replace')
imports = {}
p = off(u32(opt+120))
while True:
    original,_,_,name,first = struct.unpack_from('<IIIII',data,p)
    if not any((original,name,first)): break
    i = 0
    while True:
        thunk = struct.unpack_from('<Q',data,off(original or first)+i*8)[0]
        if not thunk: break
        imports[base+first+i*8] = dict(dll=string(name),name=f'ordinal:{thunk&65535}' if thunk>>63 else string(thunk+2))
        i += 1
    p += 20
rva,size = struct.unpack_from('<II',data,opt+136)
records = list(struct.iter_unpack('<III',data[off(rva):off(rva)+size]))
regions = {base+a:base+b for a,b,_ in records}
starts = [a for a,_,_ in records]
leaf_ends = {0x140d41b10:0x140d41b26,0x1411f19f0:0x1411f19ff}
rows = []
used = {}
for path in sorted((ROOT/'analysis').glob('resume160-graphwidget-*.asm')):
    start = int(path.stem.rsplit('-',1)[1],16)
    end = regions.get(start) or leaf_ends[start]
    raw_start,raw_end = off(start-base),off(end-base-1)+1
    text = path.read_text()
    row = dict(artifact=str(path.relative_to(ROOT)), start=hex(start),end_exclusive=hex(end),
               file_start=hex(raw_start),file_end_exclusive=hex(raw_end),machine_sha256=hashlib.sha256(data[raw_start:raw_end]).hexdigest(),
               assembly_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
               boundary='pdata region' if start in regions else 'decoded terminal RET; later captured bytes excluded')
    if start == 0x140d7b3f0:
        row['embedded_jump_table'] = dict(start='0x140d7b474',end_exclusive='0x140d7b48c',
            targets=[hex(base+u32(off(0xd7b474)+4*i)) for i in range(6)])
    rows.append(row)
    for m in re.finditer(r'([0-9A-F]{16}):.*?call\s+qword ptr \[([0-9A-F]+)h\]',text):
        address = int(m[2],16)
        if address in imports:
            used.setdefault(hex(address),dict(**imports[address],sites=[]))['sites'].append(hex(int(m[1],16)))
targets = {0x140d78490,0x140d7a9f0,0x140d41b10,0x140d7c260,0x140d7aae0,0x140d7ab20,0x140d7ab70}
xrefs = []
for _,va,size,raw in sections:
    if va != 0x1000: continue
    for i in range(size-5):
        if data[raw+i] != 0xe8: continue
        site = base+va+i
        target = site+5+struct.unpack_from('<i',data,raw+i+1)[0]
        if target not in targets: continue
        j = bisect.bisect_right(starts,va+i)-1
        owner = base+starts[j] if j>=0 and va+i<records[j][1] else None
        xrefs.append(dict(site=hex(site),target=hex(target),containing_pdata_start=hex(owner) if owner else None))
dumpbin = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
result = dict(binary=str(binary.relative_to(ROOT)),binary_sha256=hashlib.sha256(data).hexdigest(),
    tool=dict(path=str(dumpbin),sha256=hashlib.sha256(dumpbin.read_bytes()).hexdigest(),banner='Microsoft (R) COFF/PE Dumper Version 14.44.35228.0'),
    captures=rows,imports=used,direct_call_candidates=xrefs,
    qualification='Immutable static PE only. E8 byte scan yields candidates until instruction-decode verified; no indirect-call completeness. Pdata spans can include data and do not prove complete cold-path coverage. No target runtime, GUI, mutable REA DB, source edit, build or test.',
    capture_note='Initial bridge capture produced c82cb0/c82d50/1411f1a10 then stopped on the leaf 1411f19f0 lacking pdata; separate explicit-bound capture succeeded. This receipt qualifies those already-written captures. Leaf claims stop at first terminal RET. d7b3f0 includes six switch-table words after its RET; these are data.')
out = ROOT/'analysis/resume160-graphwidget-verification.json'
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(dict(receipt=str(out.relative_to(ROOT)),captures=len(rows),imports=len(used),direct_call_candidates=len(xrefs))))
