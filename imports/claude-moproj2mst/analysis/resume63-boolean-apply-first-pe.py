"""CP63 bounded immutable PE reads; never loads or executes target code."""
from pathlib import Path
import hashlib
import json
import struct

root = Path(__file__).resolve().parents[1]
path = root / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
b = path.read_bytes()
u16 = lambda o: struct.unpack_from('<H', b, o)[0]
u32 = lambda o: struct.unpack_from('<I', b, o)[0]
u64 = lambda o: struct.unpack_from('<Q', b, o)[0]
nt = u32(0x3c)
opt = nt + 24
base = u64(opt + 24)
st = opt + u16(nt + 20)
sections = []
for i in range(u16(nt + 6)):
    p = st + i * 40
    vs, va, sz, raw = struct.unpack_from('<IIII', b, p + 8)
    sections.append(dict(name=b[p:p+8].rstrip(b'\0').decode(), va=va,
                         raw=raw, size=sz, characteristics=u32(p+36)))
def off(va):
    rva = va - base
    return next(s['raw'] + rva - s['va'] for s in sections
                if s['va'] <= rva < s['va'] + s['size'])
def string(va, limit=160):
    return b[off(va):off(va)+limit].split(b'\0', 1)[0].decode('ascii', 'replace')
def span(va, size):
    raw = b[off(va):off(va)+size]
    return dict(va=hex(va), size=size, hex=raw.hex(), sha256=hashlib.sha256(raw).hexdigest())

# Ordinary import-table carrier only (not delay imports or runtime resolution).
imports = {}
irva, isize = struct.unpack_from('<II', b, opt + 120)
ip = off(base + irva)
while any(struct.unpack_from('<IIIII', b, ip)):
    oft, stamp, forward, name, first = struct.unpack_from('<IIIII', b, ip)
    dll = string(base + name)
    p = off(base + (oft or first))
    i = 0
    while (value := u64(p + i * 8)):
        symbol = f'ordinal:{value & 0xffff}' if value >> 63 else string(base + value + 2)
        imports[base + first + i * 8] = dict(dll=dll, symbol=symbol)
        i += 1
    ip += 20

target = 0x1411e2870
absolute = []
needle = struct.pack('<Q', target)
p = b.find(needle)
while p >= 0:
    for s in sections:
        if s['raw'] <= p < s['raw'] + s['size']:
            absolute.append(dict(va=hex(base+s['va']+p-s['raw']), file_offset=hex(p)))
    p = b.find(needle, p+1)
direct_candidates = []
scanned = []
for s in sections:
    if not s['characteristics'] & 0x20000000:
        continue
    scanned.append(dict(name=s['name'], va=hex(base+s['va']), raw_size=s['size']))
    for i in range(s['raw'], s['raw']+s['size']-4):
        if b[i] == 0xe8:
            va = base+s['va']+i-s['raw']
            if va+5+struct.unpack_from('<i', b, i+1)[0] == target:
                direct_candidates.append(hex(va))

meta = 0x142cde640
mp = [u64(off(meta)+i*8) for i in range(4)]
result = dict(binary=str(path.relative_to(root)), binary_sha256=hashlib.sha256(b).hexdigest(),
    method='PE32+ file-backed VA reads, ordinary import directory, carrier byte scans; no runtime',
    static_metaobject=dict(raw=span(meta,32), pointers=[hex(x) for x in mp],
                          stringdata=string(mp[1]), metadata_u32=list(struct.unpack_from('<15I',b,off(mp[2]))),
                          metadata_raw=span(mp[2],60)),
    literals=[dict(va=hex(va), text=string(va), raw=span(va, min(len(string(va))+1,160)))
              for va in [0x142cde618,0x1429fcfb8,0x1429fcfd0,0x1429fcff0,0x1429fd000,
                         0x1429fd020,0x1429fd038,0x1429fd058,0x1429fd070,0x1429fd098,0x1429fd0b0]],
    selected_imports=[dict(iat=hex(va), **imports[va]) for va in [0x141e04120,0x141e04380,0x141e06d70]],
    apply_carrier_scan=dict(target=hex(target), absolute_pointer_hits=absolute,
                            executable_sections=scanned, direct_E8_candidates=direct_candidates,
                            limitation='Only encoded absolute VA and E8 rel32 carriers; not decoded instructions or a complete indirect/cross-module callgraph. Absence does not exclude calls.'),
    vtable=[dict(slot=hex(slot), va=hex(0x142cd9b18+slot), value=hex(u64(off(0x142cd9b18+slot))))
            for slot in [0,8,0x10,0x1b8,0x1c0]])
out = root / 'analysis/resume63-boolean-apply-first-pe.json'
assert not out.exists()
out.write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps(result, indent=2))
