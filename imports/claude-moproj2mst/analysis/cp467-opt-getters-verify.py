from pathlib import Path
import hashlib, json, re, struct, subprocess, sys

root = Path(__file__).resolve().parents[1]
binary = root / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
b = binary.read_bytes()
sha = hashlib.sha256(b).hexdigest()
expected_sha = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
assert sha == expected_sha, f'unexpected PE SHA-256: {sha}'
pe = struct.unpack_from('<I', b, 0x3c)[0]
sections_count = struct.unpack_from('<H', b, pe + 6)[0]
optional_size = struct.unpack_from('<H', b, pe + 20)[0]
o = pe + 24
image_base = struct.unpack_from('<Q', b, o + 24)[0]
sections = []
for i in range(sections_count):
    vs, va, rs, rp = struct.unpack_from('<IIII', b, o + optional_size + 40*i + 8)
    sections.append((va, max(vs, rs), rp))
def fileoff(va):
    rva = va - image_base
    for start, size, raw in sections:
        if start <= rva < start + size:
            return raw + rva - start
    raise ValueError(hex(va))
def raw(va, n):
    data = b[fileoff(va):fileoff(va)+n]
    assert len(data) == n
    return data

# Derive complete bounds from the PE exception directory.
exc_rva, exc_size = struct.unpack_from('<II', b, o + 112 + 24)
exc_off = fileoff(image_base + exc_rva)
runtime_functions = [struct.unpack_from('<III', b, exc_off+i) for i in range(0, exc_size, 12)]
def runtime_range(va):
    rva = va - image_base
    entry = next(e for e in runtime_functions if e[0] <= rva < e[1])
    return image_base + entry[0], image_base + entry[1], image_base + entry[2]

vtable = 0x1427bceb8
slot_targets = {0x28:0x140159d00, 0x40:0x14091e3a0, 0x50:0x14091e460,
                0x60:0x14091e520, 0x70:0x14091e5e0, 0x98:0x14091eff0,
                0xc0:0x1400d7ee0, 0xd8:0x14091f380, 0x118:0x140918960,
                0x128:0x1409189a0, 0x138:0x1409189e0, 0x148:0x140918a20,
                0x158:0x140918a70}
for slot, target in slot_targets.items():
    assert struct.unpack('<Q', raw(vtable+slot, 8))[0] == target, hex(slot)
vt_bytes = raw(vtable, 0x168)
vt_hash = hashlib.sha256(vt_bytes).hexdigest()
assert vt_hash == '892092bd77b5abfd5de107fcb554df603875c6f1ac4a312910d9ebb530fe8da2'

dumpbin = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
assert dumpbin.exists()
exports=[]
for slot in (0x98,0x60,0x50,0x70,0x40):
    target=slot_targets[slot]
    start,end,unwind=runtime_range(target)
    assert start == target
    txt=subprocess.check_output([str(dumpbin),'/disasm:bytes',f'/range:{start:#x},{end-1:#x}',str(binary)],text=True)
    chunks=[]; cur=None
    for line in txt.splitlines():
        m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
        if m:
            cur=[int(m[1],16),bytearray.fromhex(m[2])]; chunks.append(cur)
        elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line):
            cur[1].extend(bytes.fromhex(line))
    assert chunks and chunks[0][0]==start and chunks[-1][0]+len(chunks[-1][1])==end, hex(target)
    for va,bb in chunks: assert raw(va,len(bb))==bb,hex(va)
    for (va,bb),(next_va,_) in zip(chunks,chunks[1:]): assert va+len(bb)==next_va
    out=root/f'analysis/cp467-opt-getters-slot{slot:x}-{target:x}.asm'
    out.write_text(txt,encoding='utf-8')
    exports.append({'slot':hex(slot),'target':hex(target),'start':hex(start),'end_exclusive':hex(end),
                    'unwind_info':hex(unwind),'bytes':end-start,'instructions':len(chunks),
                    'raw_sha256':hashlib.sha256(raw(start,end-start)).hexdigest(),
                    'artifact':str(out.relative_to(root)),'artifact_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),
                    'mismatches':0})

source_paths=['analysis/cp450-type3-getters-report.json','analysis/cp450-type3-getters-evidence.json',
              'analysis/cp450-type3-getters-byte-verification.json','analysis/cp465-type3-opt-save-report.md',
              'analysis/cp465-type3-opt-save-byte-verification.json','analysis/cp468-opt-group-report.md',
              'source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp',
              'source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.cpp']
source_hashes={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in source_paths}
# Inputs directly qualify the PE version/build identity and compile-time source crosswalk.
version={'file_version':None,'product_version':None,'timestamp':hex(struct.unpack_from('<I',b,pe+8)[0]),
         'image_version':f'{struct.unpack_from("<H",b,o+44)[0]}.{struct.unpack_from("<H",b,o+46)[0]}',
         'machine':hex(struct.unpack_from('<H',b,pe+4)[0]),'image_base':hex(image_base)}
receipt={'checkpoint':'CP467 Type3 OPT writer getter contracts','binary':'evidence/original-binaries/DEF_GUI_MO_64.exe',
         'binary_sha256':sha,'binary_bytes':len(b),'pe_version_identity':version,
         'method':'PE exception-directory runtime-function bounds; bounded dumpbin /disasm:bytes exports; compare every decoded addressed instruction byte and continuation byte with PE section mapping; no original execution',
         'vtable':{'address':hex(vtable),'bytes_through_slot160':len(vt_bytes),'sha256':vt_hash,
                   'selected_slot_targets':{hex(k):hex(v) for k,v in slot_targets.items()}},
         'exports':exports,'source_qualifications':[{'path':p,'sha256':source_hashes[p]} for p in source_paths],
         'byte_mismatches':0,'original_executed':False,'source_build_plan_state_changed':False}
(root/'analysis/cp467-opt-getters-byte-verification.json').write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')

report='''# CP467 Type3 `.OPT` writer getter contract\n\n## Evidence identity and scope\n\nThis is a static trace of the `DefOptModel` vtable calls in the CP465 writer. The retained PE is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `'''+sha+'''` (67,611,136 bytes). Its file-version resource is absent; the PE timestamp and image-version fields are recorded in `cp467-opt-getters-byte-verification.json`. Every new getter range is bounded by the PE exception directory, exported with bounded `dumpbin /disasm:bytes`, and compared byte-for-byte against the mapped PE. All five ranges have zero mismatches. No original executable was run.\n\n## Writer call to recovered field crosswalk\n\n| Writer vtable slot | PE target | Getter result | Backing value / behavior |\n|---|---:|---|---|\n| `+0x98(owner, group, out)` | `0x14091eff0` | Temporary field1-value count | Filters NUM_CON rows by classifier, appends matching row field1 values to its by-value temporary, returns the count and destroys the temporary. CP468 proves the writer tests a distinct original empty list, so preliminary +0x60 calls are skipped in the stable normal profile. |\n| `+0x60(owner, i)` | `0x14091e520` | signed 32-bit | NUM_CON field at node `+0x0c` (field4); invalid signed index returns `-1`. |\n| `+0x50(owner, i)` | `0x14091e460` | double | NUM_CON field at node `+0x18` (field3); invalid signed index returns `0.0`. |\n| `+0x70(owner, i)` | `0x14091e5e0` | signed 32-bit | NUM_CON field at node `+0x08` (field2); invalid signed index returns `-1`. |\n| `+0x40(owner, i)` | `0x14091e3a0` | signed 32-bit | NUM_CON field at node `+0x04` (field1); invalid signed index returns `-1`. |\n\nThese map to the physical row layout already qualified by CP450: classifierResult `+0`, field1 `+4`, field2 `+8`, field4 `+0xc`, uninitialized double `+0x10`, field3 `+0x18`, formatted QString `+0x20`. The new getter exports confirm that the writer’s row calls are projections over that layout; they do not make the uninitialized `+0x10` deterministic or writable.\n\n## Existing getters and source crosswalk\n\nCP450 already binds writer slots `+0x118/+0x128/+0x138` to `owner+0x30/+0x34/+0x38` (`int32`), `+0x148` to `owner+0x40` (`double`), `+0x158` to `owner+0x48` (`byte`), direct helper `0x14091e0b0` to `owner+0x0c`, and `0x14091e0f0` to `owner+0x10`. Slot `+0x28` gives the NUM_CON list size at `owner+0x18`; `+0xc0` gives the region list size at `owner+0x20`; `+0xd8` returns a region name in list order or `Selected Object` for an invalid index. Those prior contracts and the CP465 writer call order were not recaptured.\n\nThe checked source crosswalk is `Type3OptimizerScalarState` and its existing prepared row vectors in `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp/.cpp`. Scalars and row member order exist. The existing private DefOpt view is the integration point for four CP467 scalar row getters with native invalid-index defaults. The +0x98 filter mutates a copied temporary and must not be exposed as a writer-visible matching list; CP468 closes that identity in the stable normal profile. Existing scalar and region helpers remain available. This does not model native QList copy-on-write, QString ownership, allocator failure, exceptions, or the owner’s liveness.\n\nCP468 supersedes the earlier writer-visible group-list inference. Its byte-bound copy, append, destruction and Qt detach trace establishes why the separate writer lists remain empty under stable normal calls.\n\n## Remaining limits\n\nThe byte evidence establishes getter load/return behavior and the writer’s chosen getter order. It does not establish exact Qt text/codec bytes, stream buffering, QFile write/close effects, or native owner construction/lifetime. Do not describe the projection as a Qt output serializer or claim byte-for-byte `.OPT` parity.\n\n## Reproduction\n\nFrom this checkout run `python analysis/cp467-opt-getters-verify.py`. It verifies the immutable PE SHA, vtable targets and prior table hash; derives complete getter bounds from the PE exception directory; regenerates the five bounded disassemblies; compares every addressed byte; and refreshes the JSON receipt and this report.\n'''
(root/'analysis/cp467-opt-getters-report.md').write_text(report,encoding='utf-8')
print(json.dumps({'binary_sha256':sha,'exports':[{'slot':e['slot'],'start':e['start'],'end_exclusive':e['end_exclusive'],'bytes':e['bytes'],'mismatches':e['mismatches']} for e in exports],'vtable_sha256':vt_hash,'source_hashes':source_hashes},indent=2))
