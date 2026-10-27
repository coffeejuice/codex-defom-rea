from pathlib import Path
import struct,hashlib,json,re,subprocess,bisect
root=Path.cwd(); prefix='cp461-shapeoptim-init'; path=root/'evidence/original-binaries/DEF_GUI_MO_64.exe'; b=path.read_bytes()
p=struct.unpack_from('<I',b,60)[0];o=p+24;base=struct.unpack_from('<Q',b,o+24)[0];sh=o+struct.unpack_from('<H',b,p+20)[0]
sections=[struct.unpack_from('<IIII',b,sh+40*i+8) for i in range(struct.unpack_from('<H',b,p+6)[0])]
def off(v):return next(rp+v-base-va for vs,va,rs,rp in sections if va<=v-base<va+max(vs,rs))
def raw(v,n):return b[off(v):off(v)+n]
def cstr(v):return b[off(v):].split(b'\0',1)[0].decode('ascii')
rv,z=struct.unpack_from('<II',b,o+112+24); funcs=[(base+s,base+e) for s,e,u in (struct.unpack_from('<III',b,x) for x in range(off(base+rv),off(base+rv)+z,12))];starts=[s for s,e in funcs]
addresses=[0x1405e0d50,0x1400424b0,0x1405e0e40,0x1400aced0,0x14018f9e0,0x140088ac0,0x140004b00,0x140042500,0x1402081a0,0x1405e0e90,0x140004e90,0x140004c60,0x140025650,0x140005240,0x140004df0,0x140025320,0x140140ba0,0x140025840,0x140002f80]
dumpbin=r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe';exports=[];want={0x141e04218}
for a in addresses:
 end=next(e for s,e in funcs if s==a); txt=subprocess.check_output([dumpbin,'/disasm',f'/range:{a:#x},{end-1:#x}',str(path)],text=True);out=root/f'analysis/{prefix}-{a:x}.asm';out.write_text(txt,encoding='utf-8');chunks=[];cur=None
 for line in txt.splitlines():
  m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
  if m:cur=[int(m[1],16),bytearray.fromhex(m[2])];chunks.append(cur)
  elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line):cur[1].extend(bytes.fromhex(line))
 assert chunks[0][0]==a and chunks[-1][0]+len(chunks[-1][1])==end
 for va,bb in chunks:assert raw(va,len(bb))==bb
 for (va,bb),(nv,_) in zip(chunks,chunks[1:]):assert va+len(bb)==nv
 want.update(int(x,16) for x in re.findall(r'call\s+qword ptr \[([0-9A-F]{16})h\]',txt))
 exports.append(dict(path=str(out.relative_to(root)),start=hex(a),end_exclusive=hex(end),bytes=end-a,instructions=len(chunks),mismatches=0,raw_sha256=hashlib.sha256(raw(a,end-a)).hexdigest(),sha256=hashlib.sha256(out.read_bytes()).hexdigest(),last_bytes=bytes(chunks[-1][1]).hex()))
imports=[];rv,z=struct.unpack_from('<II',b,o+112+8)
for d in range(off(base+rv),off(base+rv)+z,20):
 orig,stamp,fwd,nm,first=struct.unpack_from('<IIIII',b,d)
 if not(orig or nm or first):break
 i=0
 while True:
  v=struct.unpack_from('<Q',b,off(base+(orig or first))+8*i)[0]
  if not v:break
  slot=base+first+8*i
  if slot in want:imports.append(dict(iat=hex(slot),dll=cstr(base+nm),symbol=('ordinal:'+str(v&65535)) if v>>63 else cstr(base+v+2)))
  i+=1

srcs=['analysis/cp460-type3-open-identity-report.md','analysis/cp460-type3-open-identity-byte-verification.json','analysis/cp460-type3-open-identity-14013f5a0.asm','evidence/gui_mo/s1/mdt/L5/v4.c','evidence/gui_mo/s1/mdt/L7/d7.c','evidence/gui_mo/out_p07/lists.c']
sources=[dict(path=s,bytes=(root/s).stat().st_size,sha256=hashlib.sha256((root/s).read_bytes()).hexdigest()) for s in srcs]
r=dict(checkpoint='CP461 ShapeOptim scalar initializer helpers',binary=str(path.relative_to(root)),binary_sha256=hashlib.sha256(b).hexdigest(),exports=exports,compared_bytes=sum(x['bytes'] for x in exports),addressed_instructions=sum(x['instructions'] for x in exports),mismatches=0,imports=imports,sources=sources,qualifications=['Immutable original PE reads and selected complete pdata-function exports only; original never executed, GUI/app-control not acquired, native REA/Ghidra not opened.','Retained decompiles guide claims; every new instruction export compared bytewise. CP460/CP454/CP458 qualifications remain.','Qt DLL implementation, callback/reentrant effects, faults and exception/unwind semantics remain outside this source evidence.','No source, tests, build, deployment, plan or state edits.'])
(root/f'analysis/{prefix}-byte-verification.json').write_text(json.dumps(r,indent=2)+'\n')
print(json.dumps({k:r[k] for k in ['binary_sha256','compared_bytes','addressed_instructions','mismatches','imports']}))
