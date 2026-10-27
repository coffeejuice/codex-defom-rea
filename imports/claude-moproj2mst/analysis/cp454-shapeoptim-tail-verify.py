from pathlib import Path
import struct,hashlib,json,re,subprocess
root=Path(__file__).resolve().parents[1]
p=root/'evidence/original-binaries/DEF_GUI_MO_64.exe';b=p.read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0];ns=struct.unpack_from('<H',b,pe+6)[0];opt=struct.unpack_from('<H',b,pe+20)[0];o=pe+24;base=struct.unpack_from('<Q',b,o+24)[0]
sections=[]
for i in range(ns):
 x=o+opt+40*i;vs,va,rs,rp=struct.unpack_from('<IIII',b,x+8);sections.append((va,max(vs,rs),rp))
def off(va):
 rva=va-base
 for s,sz,rp in sections:
  if s<=rva<s+sz:return rp+rva-s
 raise ValueError(hex(va))
def raw(va,n):return b[off(va):off(va)+n]
def cstring(va):return b[off(va):].split(b'\0',1)[0].decode('ascii')
ranges={'consumer':(0x14013de2c,0x14013e4e0),'iterator-ctor':(0x1401408e0,0x14014091a),'iterator-test':(0x140dfc080,0x140dfc0e1),'iterator-self':(0x140dfc380,0x140dfc390),'iterator-deref':(0x140dd75b0,0x140dd75cb),'iterator-next':(0x140de9800,0x140de981f),'iterator-neq':(0x140126470,0x1401264a8),'top-list':(0x140353770,0x1403537ae),'child-list':(0x140370f60,0x140370fa2),'display-name':(0x14036fcc0,0x14036fe44),'set-maxsims':(0x1401424e0,0x140142529),'set-minobjective':(0x140142580,0x1401425c7),'refresh':(0x140142ca0,0x140142cee),'pointer-get':(0x140c1aef0,0x140c1aefe)}
ranges.update({'iterator-init':(0x140140a10,0x140140a74),'slot-value':(0x140619750,0x140619780),'list-begin':(0x140307560,0x140307590),'list-end':(0x140136ff0,0x140137020),'iterator-pointer-ctor':(0x140c628c0,0x140c628dd)})
dumpbin=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
exports=[];wanted=set()
for name,(start,end) in ranges.items():
 p=root/f'analysis/cp454-shapeoptim-tail-{name}.asm'
 txt=subprocess.check_output([str(dumpbin),'/disasm',f'/range:{start:#x},{end-1:#x}',str(root/'evidence/original-binaries/DEF_GUI_MO_64.exe')],text=True)
 p.write_text(txt,encoding='utf-8');wanted.update(int(v,16) for v in re.findall(r'call\s+qword ptr \[([0-9A-F]{16})h\]',txt));chunks=[];cur=None
 for line in txt.splitlines():
  m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
  if m:cur=[int(m[1],16),bytearray.fromhex(m[2])];chunks.append(cur)
  elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line):cur[1].extend(bytes.fromhex(line))
 assert chunks[0][0]==start and chunks[-1][0]+len(chunks[-1][1])==end,(name,hex(chunks[-1][0]))
 for va,bb in chunks:assert raw(va,len(bb))==bb,(name,hex(va))
 for (va,bb),(nv,_) in zip(chunks,chunks[1:]):assert va+len(bb)==nv,(name,hex(va))
 exports.append(dict(path=str(p.relative_to(root)),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),start=hex(start),end_exclusive=hex(end),bytes=end-start,instructions=len(chunks),raw_sha256=hashlib.sha256(raw(start,end-start)).hexdigest(),mismatches=0,last_instruction=bytes(chunks[-1][1]).hex()))
# Bind only the called imports, retaining exact IAT slot and DLL/symbol, not a full import listing.
# wanted collected only from exact indirect IAT calls in the selected exports
imports=[];rva,size=struct.unpack_from('<II',b,o+112+8)
for d in range(off(base+rva),off(base+rva)+size,20):
 orig,stamp,fwd,nm,first=struct.unpack_from('<IIIII',b,d)
 if not(orig or nm or first):break
 dll=cstring(base+nm);th=orig or first;i=0
 while True:
  v=struct.unpack_from('<Q',b,off(base+th)+8*i)[0]
  if not v:break
  slot=base+first+8*i
  if slot in wanted:imports.append(dict(iat=hex(slot),dll=dll,symbol=('ordinal:'+str(v&65535)) if v>>63 else cstring(base+v+2)))
  i+=1
strings=[dict(va=hex(a),text=cstring(a),raw_hex=raw(a,len(cstring(a))+1).hex()) for a in [0x1421b4a30,0x1421b4a40,0x1421b4a48,0x1423275e4]]
sources=[]
for name in ['evidence/gui_mo/s1/mdt/L7/d0.c','evidence/gui_mo/s1/mdt/L7/d4.c','evidence/gui_mo/s1/s1-04/d1.c','analysis/cp452-shapeoptim-consumer-report.md','analysis/cp452-shapeoptim-consumer-byte-verification.json','source/apps/moproj2mst/src/ops/report.cpp','source/apps/moproj2mst/src/engine/shapeoptim_load.cpp']:
 p=root/name;sources.append(dict(path=name,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
result=dict(checkpoint='CP454 ShapeOptim consumer tail',binary='evidence/original-binaries/DEF_GUI_MO_64.exe',binary_bytes=len(b),binary_sha256=hashlib.sha256(b).hexdigest(),method='Bounded dumpbin disassembly followed by independent PE section mapping and every exported byte comparison; original not executed; no REA database opened',exports=exports,compared_bytes=sum(x['bytes'] for x in exports),addressed_instructions=sum(x['instructions'] for x in exports),mismatches=0,imports=imports,strings=strings,sources=sources,qualifications=['Consumer tail from CP452 exclusive end 0x14013de2c through RET is bound. Direct selected helper ranges terminate at RET.','Static normal control flow and fault prefixes only; Qt allocation/detach/log formatting, exception unwinding, RTTI implementation, property identity and native ownership are not independently reproduced.','No original process execution, GUI input, native session, shared REA mutation, source/test/build/plan/state edits.'])
(root/'analysis/cp454-shapeoptim-tail-byte-verification.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['binary_sha256','compared_bytes','addressed_instructions','mismatches']}));print(json.dumps(imports))

