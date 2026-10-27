from pathlib import Path
import struct,hashlib,json,re,subprocess
root=Path(__file__).resolve().parents[1]
binpath=root/'evidence/original-binaries/DEF_GUI_MO_64.exe';b=binpath.read_bytes();pe=struct.unpack_from('<I',b,60)[0];o=pe+24;optsz=struct.unpack_from('<H',b,pe+20)[0];base=struct.unpack_from('<Q',b,o+24)[0];sections=[]
for i in range(struct.unpack_from('<H',b,pe+6)[0]):
 x=o+optsz+40*i;vs,va,rs,rp=struct.unpack_from('<IIII',b,x+8);sections.append((va,max(vs,rs),rp))
def off(v):
 for a,z,r in sections:
  if a<=v-base<a+z:return r+v-base-a
 raise ValueError(hex(v))
def raw(v,n):return b[off(v):off(v)+n]
def cstr(v):return b[off(v):].split(b'\0',1)[0].decode('ascii')
rva,z=struct.unpack_from('<II',b,o+112+3*8);pd=off(base+rva);functions=[(base+s,base+e) for s,e,u in (struct.unpack_from('<III',b,q) for q in range(pd,pd+z,12))]
def bounds(a):
 if a in [0x140142ca0,0x140142d00]:return a,a+0x4e
 for s,e in functions:
  if s==a:return s,e
 raise ValueError(hex(a))
addresses=[0x140046cc0,0x1400511f0,0x1405f8d60,0x14013a9a0,0x14013aa60,0x14013ab00,0x14013af20,0x14013be10,0x140142620,0x1401426c0,0x140142720,0x1401427b0,0x140142800,0x140142b00,0x140142ca0,0x140142d00,0x140142d50,0x140142d90,0x1405f8bb0,0x1405f8c30,0x1405f8d30,0x140142e20,0x1400ac4e0,0x140cbc900,0x140cbc920,0x1405ef2b0,0x1405ef280,0x1400652a0,0x140050440,0x140050690,0x1400512c0]
ranges={f'{a:x}':bounds(a) for a in addresses};ranges.update({'140051230':(0x140051230,0x14005123f),'140c1aef0':(0x140c1aef0,0x140c1aefe),'14013ab70':(0x14013ab70,0x14013ab76),'14015d170':(0x14015d170,0x14015d187)})
dumpbin=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe');exports=[];wanted=set()
for name,(start,end) in ranges.items():
 txt=subprocess.check_output([str(dumpbin),'/disasm',f'/range:{start:#x},{end-1:#x}',str(binpath)],text=True);p=root/f'analysis/cp458-type3-linked-{name}.asm';p.write_text(txt,encoding='utf-8');chunks=[];cur=None
 for line in txt.splitlines():
  m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
  if m:cur=[int(m[1],16),bytearray.fromhex(m[2])];chunks.append(cur)
  elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line):cur[1].extend(bytes.fromhex(line))
 assert chunks[0][0]==start and chunks[-1][0]+len(chunks[-1][1])==end,(name,hex(chunks[-1][0]))
 for va,bb in chunks:assert raw(va,len(bb))==bb,(name,hex(va))
 for (va,bb),(nv,_) in zip(chunks,chunks[1:]):assert va+len(bb)==nv,(name,hex(va))
 wanted.update(int(v,16) for v in re.findall(r'call\s+qword ptr \[([0-9A-F]{16})h\]',txt))
 exports.append(dict(path=str(p.relative_to(root)),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),start=hex(start),end_exclusive=hex(end),bytes=end-start,instructions=len(chunks),raw_sha256=hashlib.sha256(raw(start,end-start)).hexdigest(),mismatches=0,last_instruction=bytes(chunks[-1][1]).hex()))
imports=[];rva,size=struct.unpack_from('<II',b,o+112+8)
for d in range(off(base+rva),off(base+rva)+size,20):
 orig,stamp,fwd,nm,first=struct.unpack_from('<IIIII',b,d)
 if not(orig or nm or first):break
 dll=cstr(base+nm);th=orig or first;i=0
 while True:
  v=struct.unpack_from('<Q',b,off(base+th)+8*i)[0]
  if not v:break
  slot=base+first+8*i
  if slot in wanted:imports.append(dict(iat=hex(slot),dll=dll,symbol=('ordinal:'+str(v&65535)) if v>>63 else cstr(base+v+2)))
  i+=1
tables=[]
for v,slots in [(0x1421b4d98,[0x18,0x78,0x98,0x188]),(0x1421bfcd8,[0x18,0x58,0x60]),(0x1421bfca0,[0x20]),(0x1421247d8,[0x18])]:
 col=struct.unpack_from('<Q',b,off(v-8))[0];td=base+struct.unpack_from('<I',b,off(col)+12)[0];tables.append(dict(vtable=hex(v),col=hex(col),type_descriptor=hex(td),type_name=cstr(td+16),slots=[dict(offset=hex(s),target=hex(struct.unpack_from('<Q',b,off(v+s))[0]),raw_hex=raw(v+s,8).hex()) for s in slots],col_raw_hex=raw(col,24).hex(),type_descriptor_raw_hex=raw(td,len(cstr(td+16))+17).hex()))
sources=[]
for name in ['analysis/cp454-shapeoptim-tail-report.md','analysis/cp454-shapeoptim-tail-byte-verification.json','evidence/gui_mo/s1/mdt/L4/so/ctor.c','evidence/gui_mo/s1/mdt/L4/so/pctor.c','evidence/gui_mo/s1/p17/b4.c','evidence/gui_mo/s1/s1-01/dc/open3.c','evidence/gui_mo/s1/s1-01/dc/acc0.c','evidence/gui_mo/s1/s1-07/dc/dim0.c','evidence/gui_mo/s1/mdt/L7/d0.c','evidence/gui_mo/s1/vtables.tsv']:
 p=root/name;sources.append(dict(path=name,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
r=dict(checkpoint='CP458 Type3 ShapeOptim linked-property identity',binary='evidence/original-binaries/DEF_GUI_MO_64.exe',binary_bytes=len(b),binary_sha256=hashlib.sha256(b).hexdigest(),method='Offline bounded dumpbin disassembly; PE section mapping and every addressed instruction byte comparison; original never executed; no REA database opened',exports=exports,compared_bytes=sum(x['bytes'] for x in exports),addressed_instructions=sum(x['instructions'] for x in exports),mismatches=0,imports=imports,vtables=tables,defaults=dict(double_va=hex(0x1421b5050),raw_hex=raw(0x1421b5050,8).hex(),double=struct.unpack('<d',raw(0x1421b5050,8))[0]),sources=sources,qualifications=['Static selected functions and conditional creation/publication/teardown only. No occurrence witness for current-operation P versus most-recent linked B at Type3 load.','Qt changeGuard/removeGuard/QObject constructor/destructor implementation not inspected; symbols bind guard calls, not native thread/reentrant lifetime behavior.','Original vtable RTTI is PE-bound. Legacy strings.txt offsets are file offsets and must not be used as image RVAs.','No source/test/build/deployment/plan/state edits, mutable shared REA access, original process execution or GUI control.'])
(root/'analysis/cp458-type3-linked-byte-verification.json').write_text(json.dumps(r,indent=2)+'\n')
print(json.dumps({k:r[k] for k in ['binary_sha256','compared_bytes','addressed_instructions','mismatches']}));print(json.dumps([x for x in imports if any(q in x['symbol'] for q in ['Guard','QObject'])]));print(json.dumps(tables));print(json.dumps(r['defaults']))

