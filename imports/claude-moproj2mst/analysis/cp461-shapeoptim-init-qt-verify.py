from pathlib import Path
import struct,hashlib,json,re,subprocess,bisect
root=Path.cwd(); prefix='cp461-shapeoptim-init-qt'; path=root/'evidence/original-binaries/QtCore4_64.dll'; b=path.read_bytes()
p=struct.unpack_from('<I',b,60)[0];o=p+24;base=struct.unpack_from('<Q',b,o+24)[0];sh=o+struct.unpack_from('<H',b,p+20)[0]
sections=[struct.unpack_from('<IIII',b,sh+40*i+8) for i in range(struct.unpack_from('<H',b,p+6)[0])]
def off(v):return next(rp+v-base-va for vs,va,rs,rp in sections if va<=v-base<va+max(vs,rs))
def raw(v,n):return b[off(v):off(v)+n]
def cstr(v):return b[off(v):].split(b'\0',1)[0].decode('ascii')
rv,z=struct.unpack_from('<II',b,o+112+24); funcs=[(base+s,base+e) for s,e,u in (struct.unpack_from('<III',b,x) for x in range(off(base+rv),off(base+rv)+z,12))];starts=[s for s,e in funcs]
addresses=[0x67001aa0,0x67001ac0,0x670203b0,0x67020c70,0x670700d0]
dumpbin=r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe';exports=[];want={0x671a8618}
for a in addresses:
 end={0x67001aa0:0x67001aa8,0x67001ac0:0x67001ac8,0x670203b0:0x670203b7}.get(a) or next(e for s,e in funcs if s==a); txt=subprocess.check_output([dumpbin,'/disasm',f'/range:{a:#x},{end-1:#x}',str(path)],text=True);out=root/f'analysis/{prefix}-{a:x}.asm';out.write_text(txt,encoding='utf-8');chunks=[];cur=None
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


export_rva,export_size=struct.unpack_from('<II',b,o+112)
ed=struct.unpack_from('<IIHHIIIIIII',b,off(base+export_rva)); nfunc,nnames,fr, nr,orr=ed[6:]
export_bindings=[]
expected={'?shared_null@QListData@@2UData@1@A':0x672f0a50,'?ref@QBasicAtomicInt@@QEAA_NXZ':0x67001aa0,'?deref@QBasicAtomicInt@@QEAA_NXZ':0x67001ac0,'??1QString@@QEAA@XZ':0x67020c70,'?qFree@@YAXPEAX@Z':0x670203b0}
for i in range(nnames):
 name=cstr(base+struct.unpack_from('<I',b,off(base+nr)+4*i)[0])
 if name in expected:
  ix=struct.unpack_from('<H',b,off(base+orr)+2*i)[0]; va=base+struct.unpack_from('<I',b,off(base+fr)+4*ix)[0]
  assert va==expected[name]; export_bindings.append(dict(symbol=name,va=hex(va)))
assert len(export_bindings)==len(expected)
shared=0x672f0a50;shared_bytes=raw(shared,24)
assert struct.unpack('<6I',shared_bytes)==(1,0,0,0,1,0)
r=dict(binary=str(path.relative_to(root)),binary_sha256=hashlib.sha256(b).hexdigest(),exports=exports,export_bindings=export_bindings,compared_bytes=sum(x['bytes'] for x in exports),addressed_instructions=sum(x['instructions'] for x in exports),mismatches=0,imports=imports,shared_null=dict(va=hex(shared),raw_hex=shared_bytes.hex(),file_offset=off(shared),dwords=list(struct.unpack('<6I',shared_bytes))),qualifications=['Original imported QtCore static bytes only. Runtime module identity and state are not observed. Leaf ref/deref ranges end at RET; qFree range ends at unconditional imported JMP. QString ranges use pdata bounds.','Original DLL metadata establishes initial shared_null refcount=1, alloc=0, begin=0, end=0, sharable-bit=1. Refcount mutates in a process; no runtime snapshot was acquired.','CRT free boundary, invalid memory, heap corruption, concurrent mutation and C++ unwind behavior are not reproduced.'])
(root/f'analysis/{prefix}-byte-verification.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:r[k]for k in ['binary_sha256','compared_bytes','addressed_instructions','mismatches','export_bindings']}))
