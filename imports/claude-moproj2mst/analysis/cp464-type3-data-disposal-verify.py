from pathlib import Path
import struct,hashlib,json,re,subprocess
root=Path(__file__).resolve().parents[1];prefix='cp464-type3-data-disposal'
dumpbin=r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe'
class PE:
 def __init__(self,name):
  self.path=root/'evidence/original-binaries'/name;self.b=self.path.read_bytes();b=self.b;p=struct.unpack_from('<I',b,60)[0];o=p+24;self.o=o;self.base=struct.unpack_from('<Q',b,o+24)[0];sh=o+struct.unpack_from('<H',b,p+20)[0];self.ss=[struct.unpack_from('<IIII',b,sh+40*i+8) for i in range(struct.unpack_from('<H',b,p+6)[0])];rv,z=struct.unpack_from('<II',b,o+136);self.fs=[(self.base+s,self.base+e) for s,e,u in (struct.unpack_from('<III',b,x) for x in range(self.off(self.base+rv),self.off(self.base+rv)+z,12))]
 def off(self,v):return next(rp+v-self.base-va for vs,va,rs,rp in self.ss if va<=v-self.base<va+max(vs,rs))
 def raw(self,v,n):return self.b[self.off(v):self.off(v)+n]
 def cs(self,v):return self.b[self.off(v):].split(b'\0',1)[0].decode('ascii')
 def bounds(self,a):return next((s,e) for s,e in self.fs if s==a)
 def imports(self,want):
  result=[];rv,z=struct.unpack_from('<II',self.b,self.o+120)
  for d in range(self.off(self.base+rv),self.off(self.base+rv)+z,20):
   orig,stamp,fwd,nm,first=struct.unpack_from('<IIIII',self.b,d)
   if not(orig or nm or first):break
   i=0
   while True:
    v=struct.unpack_from('<Q',self.b,self.off(self.base+(orig or first))+8*i)[0]
    if not v:break
    slot=self.base+first+8*i
    if slot in want:result.append(dict(iat=hex(slot),dll=self.cs(self.base+nm),symbol=('ordinal:'+str(v&65535)) if v>>63 else self.cs(self.base+v+2)))
    i+=1
  return result
 def exps(self,names):
  rv,z=struct.unpack_from('<II',self.b,self.o+112);ed=struct.unpack_from('<IIHHIIIIIII',self.b,self.off(self.base+rv));nf,nn,fr,nr,orr=ed[6:];out=[]
  for i in range(nn):
   n=self.cs(self.base+struct.unpack_from('<I',self.b,self.off(self.base+nr)+4*i)[0])
   if n in names:
    ix=struct.unpack_from('<H',self.b,self.off(self.base+orr)+2*i)[0];out.append(dict(symbol=n,va=hex(self.base+struct.unpack_from('<I',self.b,self.off(self.base+fr)+4*ix)[0])))
  return out
exe=PE('DEF_GUI_MO_64.exe');qt=PE('QtCore4_64.dll');exports=[];binaries=[]
plans=[(exe,[0x140142620,0x1401426c0,0x140142720,0x1401427b0,0x140142800,0x140142b00,0x140142d90,0x140142e20,0x140142e70,0x1403536a0,0x1403536f0,0x140353750,0x1403537c0,0x1405f8bb0,0x1405f8c30,0x1405f8ca0,0x1405f8d30,0x1400ad260,0x1400ad2a0,0x1400adbc0,0x1400ae510,0x1400ae8a0,0x1400ac4e0,0x1400ac8f0,0x14036f980,0x140375f80,0x140375fc0,0x14146eabc,0x14147489c,0x141471b7c,0x140142d50],{0x140353740:0x140353746,0x1414dfc68:0x1414dfc6d,0x14015d170:0x14015d187}),(qt,[0x6712f870,0x6712d300,0x6712de20,0x671313d0,0x67131160],{})]
for pe,aa,leaves in plans:
 want=set();these=[]
 for a in aa+list(leaves):
  end=leaves.get(a) or pe.bounds(a)[1];txt=subprocess.check_output([dumpbin,'/disasm',f'/range:{a:#x},{end-1:#x}',str(pe.path)],text=True);out=root/f'analysis/{prefix}-{a:x}.asm';out.write_text(txt,encoding='utf-8');chunks=[];cur=None
  for line in txt.splitlines():
   m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
   if m:cur=[int(m[1],16),bytearray.fromhex(m[2])];chunks.append(cur)
   elif cur and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line):cur[1].extend(bytes.fromhex(line))
  assert chunks[0][0]==a and chunks[-1][0]+len(chunks[-1][1])==end,(hex(a),chunks[-1])
  for va,bb in chunks:assert pe.raw(va,len(bb))==bb
  for (va,bb),(nv,_) in zip(chunks,chunks[1:]):assert va+len(bb)==nv
  want.update(int(x,16) for x in re.findall(r'(?:call|jmp)\s+qword ptr \[([0-9A-F]{16})h\]',txt))
  entry=dict(path=str(out.relative_to(root)),start=hex(a),end_exclusive=hex(end),bytes=end-a,instructions=len(chunks),mismatches=0,raw_sha256=hashlib.sha256(pe.raw(a,end-a)).hexdigest(),sha256=hashlib.sha256(out.read_bytes()).hexdigest(),last_bytes=bytes(chunks[-1][1]).hex(),bound='explicit RET/tail JMP leaf' if a in leaves else 'PE exception-function range');exports.append(entry);these.append(entry)
 binaries.append(dict(path=str(pe.path.relative_to(root)),sha256=hashlib.sha256(pe.b).hexdigest(),bytes=len(pe.b),exports=len(these),compared_bytes=sum(x['bytes']for x in these),addressed_instructions=sum(x['instructions']for x in these),imports=pe.imports(want)))
qbindings=qt.exps({'??1QObject@@UEAA@XZ','??0QObject@@QEAA@PEAV0@@Z','?deleteChildren@QObjectPrivate@@QEAAXXZ','?clearGuards@QObjectPrivate@@SAXPEAVQObject@@@Z'})
r=dict(checkpoint='CP464 Type3 +38 data disposal',method='Offline bounded original PE dumpbin exports independently mapped and compared byte for byte; no original process, GUI, debugger or mutable REA session',binaries=binaries,exports=exports,qt_export_bindings=qbindings,mismatches=0,compared_bytes=sum(x['bytes']for x in exports),addressed_instructions=sum(x['instructions']for x in exports),qualifications=['Normal paths and selected static original functions only; C++ exception/unwind paths, OS heap internals and arbitrary callbacks are not closed.','Imported QtCore original file bytes are bound, not the DLL loaded in any process. QObject destructor includes dynamic metaobject/signal/connection/child/private-destructor boundaries; callback effects remain unknown.','Absence of a +38 free in selected paths is a bounded static result, not a process-wide non-free/leak observation.','New CP464 analysis files only; no source/build/deployment/plan/state modifications.'])
r['source_receipts']=[dict(path=n,sha256=hashlib.sha256((root/n).read_bytes()).hexdigest()) for n in ['analysis/cp458-type3-linked-report.md','analysis/cp458-type3-linked-byte-verification.json','analysis/cp461-shapeoptim-init-report.md','analysis/cp461-shapeoptim-init-qt-byte-verification.json']]
(root/f'analysis/{prefix}-byte-verification.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:r[k]for k in ['compared_bytes','addressed_instructions','mismatches','qt_export_bindings']}));print(json.dumps([{k:v for k,v in x.items()if k!='imports'}for x in binaries]))
