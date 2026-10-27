from pathlib import Path
import struct, hashlib, json, re, subprocess

root = Path(__file__).resolve().parents[1]
binary = root / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
b = binary.read_bytes()
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
def raw(va, n): return b[fileoff(va):fileoff(va)+n]

# Exception-directory runtime-function boundaries from the PE, not guessed RET windows.
exc_rva, exc_size = struct.unpack_from('<II', b, o + 112 + 24)
exc_off = fileoff(image_base + exc_rva)
functions = [struct.unpack_from('<III', b, exc_off+i)[:2] for i in range(0, exc_size, 12)]
runtime_functions = [struct.unpack_from('<III', b, exc_off+i) for i in range(0, exc_size, 12)]
start = 0x140907a10
rva = start - image_base
begin, end = next((a, z) for a, z in functions if a <= rva < z)
begin += image_base; end += image_base
assert begin == start
dumpbin = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
txt = subprocess.check_output([str(dumpbin), '/disasm', f'/range:{start:#x},{end-1:#x}', str(binary)], text=True)
out = root / 'analysis/cp465-type3-opt-save-140907a10.asm'
out.write_text(txt, encoding='utf-8')
chunks=[]; cur=None
for line in txt.splitlines():
    m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)', line)
    if m:
        cur=[int(m[1],16),bytearray.fromhex(m[2])]; chunks.append(cur)
    elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+', line): cur[1].extend(bytes.fromhex(line))
assert chunks and chunks[0][0] == start and chunks[-1][0]+len(chunks[-1][1]) == end
for va, bb in chunks: assert raw(va,len(bb)) == bb, hex(va)
for (va,bb),(nv,_) in zip(chunks,chunks[1:]): assert va+len(bb)==nv
flat = re.sub(r'\s+', ' ', txt)
assert flat.count('[rsp+38h]') == 3
assert flat.count('mov byte ptr [rsp+38h],0') == 1
assert flat.count('movzx eax,byte ptr [rsp+38h]') == 2
assert raw(0x140907a3e,5) == bytes.fromhex('c6 44 24 38 00')
assert raw(0x140907b5e,5) == bytes.fromhex('0f b6 44 24 38')
assert raw(0x1409085a8,5) == bytes.fromhex('0f b6 44 24 38')
assert raw(0x1409085ad,7) == bytes.fromhex('88 84 24 40 01 00 00')
assert raw(0x14090860c,7) == bytes.fromhex('8a 84 24 40 01 00 00')
rf = next(e for e in runtime_functions if e[0] == start-image_base and e[1] == end-image_base)
unwind_va = image_base + rf[2]
unwind_off = fileoff(unwind_va)
unwind_header = b[unwind_off:unwind_off+4]
unwind_version = unwind_header[0] & 7
unwind_flags = unwind_header[0] >> 3
unwind_codes = unwind_header[2]
handler_pos = unwind_off + 4 + (((unwind_codes+1)&~1)*2)
handler_rva = struct.unpack_from('<I',b,handler_pos)[0] if unwind_flags & 3 else None
assert unwind_version == 1 and unwind_flags == 2 and image_base+handler_rva == 0x14146e20c

# Bind the helper called after each +0x98 indexed collection result; its body is
# needed to distinguish an empty collection from a first-member +0x60 read.
helper_start = 0x140307410
helper_rva = helper_start - image_base
helper_begin, helper_end = next((a,z) for a,z in functions if a <= helper_rva < z)
helper_begin += image_base; helper_end += image_base
assert helper_begin == helper_start
helper_txt = subprocess.check_output([str(dumpbin), '/disasm', f'/range:{helper_start:#x},{helper_end-1:#x}', str(binary)], text=True)
helper_out = root / 'analysis/cp465-type3-opt-save-140307410.asm'
helper_out.write_text(helper_txt, encoding='utf-8')
helper_chunks=[]; cur=None
for line in helper_txt.splitlines():
    m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)', line)
    if m:
        cur=[int(m[1],16),bytearray.fromhex(m[2])]; helper_chunks.append(cur)
    elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+', line): cur[1].extend(bytes.fromhex(line))
assert helper_chunks and helper_chunks[0][0] == helper_start and helper_chunks[-1][0]+len(helper_chunks[-1][1]) == helper_end
for va,bb in helper_chunks: assert raw(va,len(bb)) == bb, hex(va)
for (va,bb),(nv,_) in zip(helper_chunks,helper_chunks[1:]): assert va+len(bb)==nv

primary = {'path':str(out.relative_to(root)),'sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'start':hex(start),'end_exclusive':hex(end),'bytes':end-start,'instructions':len(chunks),'raw_sha256':hashlib.sha256(raw(start,end-start)).hexdigest(),'mismatches':0,'last_instruction':bytes(chunks[-1][1]).hex()}
helper = {'path':str(helper_out.relative_to(root)),'sha256':hashlib.sha256(helper_out.read_bytes()).hexdigest(),'start':hex(helper_start),'end_exclusive':hex(helper_end),'bytes':helper_end-helper_start,'instructions':len(helper_chunks),'raw_sha256':hashlib.sha256(raw(helper_start,helper_end-helper_start)).hexdigest(),'mismatches':0,'last_instruction':bytes(helper_chunks[-1][1]).hex()}
source_paths = ['analysis/resume449-type3-published-owner-edge-audit.md','analysis/resume63-doe-public-save-order.md','analysis/resume143-type3-file-parser.md','evidence/gui_mo/s1/mdt/DB/d3.c','evidence/gui_mo/s1/mdt/DOE/d6.c','source/docs/re/moproj2mst/ledger.tsv']
report = {
  'checkpoint':'CP465 Type3 .OPT Save consumer static trace',
  'binary':'evidence/original-binaries/DEF_GUI_MO_64.exe',
  'binary_sha256':hashlib.sha256(b).hexdigest(),
  'binary_bytes':len(b),
  'method':'PE exception-directory function bounds; bounded dumpbin exports; independently map every addressed byte through PE sections; original not executed',
  'exports':[primary,helper],
  'return_contract':{'slot':'[rsp+0x38]','initializer_va':'0x140907a3e','initializer':'zero','writes':['0x140907a3e: zero initialization'],'reads':['0x140907b5e: open-failure path','0x1409085a8: post-close path'],'ordinary_returns':{'open_failure':False,'normal_post_close':False},'verification':'Complete bound function contains exactly one byte-slot write (zero) and two reads; no other slot references.'},
  'exception_metadata':{'runtime_function_start':hex(image_base+rf[0]),'runtime_function_end_exclusive':hex(image_base+rf[1]),'unwind_info_va':hex(unwind_va),'version':unwind_version,'flags':unwind_flags,'handler_va':hex(image_base+handler_rva) if handler_rva is not None else None,'qualification':'The UHANDLER target and exception/cleanup behavior are not traced in this checkpoint.'},
  'ordered_collection_contract':{'collection_getter_sites':['0x140907d22','0x140907dc8','0x140907e6b'],'nonempty_helper':'0x140307410','conditional_first_value_getter_sites':['0x140907d64','0x140907e04','0x140907ea7'],'qualification':'QListData::isEmpty returns true for empty; that branch skips +0x60. False means nonempty and reaches +0x60. Helper body is separately byte-bound.'},
  'source_qualifications':[{'path':p,'sha256':hashlib.sha256((root/p).read_bytes()).hexdigest()} for p in source_paths],
  'qualifications':['The original PE is static evidence only.','Qt QFile/QTextStream codec, buffering, flush/close error reporting are not independently characterized by this export.','The writer has UHANDLER unwind metadata; the handler target and exception/cleanup behavior are not traced here.','No source, build, test, plan, state, GUI, original process, or mutable REA database changes.']
}
(root/'analysis/cp465-type3-opt-save-byte-verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'binary_sha256':report['binary_sha256'],'exports':[{'start':e['start'],'end_exclusive':e['end_exclusive'],'bytes':e['bytes'],'instructions':e['instructions'],'mismatches':e['mismatches']} for e in report['exports']],'return_contract':report['return_contract'],'mismatches':0}))
