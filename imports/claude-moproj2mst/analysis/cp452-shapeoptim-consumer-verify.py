from pathlib import Path
import struct, hashlib, json, re
root=Path(__file__).resolve().parents[1]
p=root/'evidence/original-binaries/DEF_GUI_MO_64.exe'
b=p.read_bytes(); pe=struct.unpack_from('<I',b,0x3c)[0]; ns=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]; base=struct.unpack_from('<Q',b,pe+24+24)[0]
sections=[]
for i in range(ns):
 o=pe+24+opt+40*i
 vs,va,rs,rp=struct.unpack_from('<IIII',b,o+8); sections.append((va,max(vs,rs),rp))
def raw(va,n):
 rva=va-base
 for s,sz,rp in sections:
  if s<=rva<s+sz: return b[rp+rva-s:rp+rva-s+n]
 raise ValueError(hex(va))
exports=[]
for p in sorted((root/'analysis').glob('cp452-shapeoptim-consumer-*.asm')):
 chunks=[]; current=None
 for line in p.read_text().splitlines():
  m=re.match(r'^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)',line)
  if m:
   current=[int(m[1],16),bytearray.fromhex(m[2])]; chunks.append(current)
  elif current and re.fullmatch(r'\s{10,}(?:[0-9A-F]{2} ?)+',line): current[1].extend(bytes.fromhex(line))
 compared=0
 for va,bb in chunks:
  assert raw(va,len(bb))==bb,(str(p),hex(va)); compared+=len(bb)
 for (va,bb),(nv,nb) in zip(chunks,chunks[1:]): assert va+len(bb)==nv,(str(p),hex(va))
 start=chunks[0][0]; end=chunks[-1][0]+len(chunks[-1][1]); rb=raw(start,end-start)
 exports.append(dict(path=str(p.relative_to(root)),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),start=hex(start),end_exclusive=hex(end),bytes=compared,instructions=len(chunks),raw_sha256=hashlib.sha256(rb).hexdigest(),mismatches=0,last_instruction=bytes(chunks[-1][1]).hex()))
sources=[]
for name in ['evidence/gui_mo/s1/mdt/L7/d0.c','analysis/resume149-type3-owner-next.md','analysis/cp450-type3-getters-report.json','analysis/cp450-type3-getters-byte-verification.json','analysis/cp450-type3-getters-imports-all.json','analysis/resume451-type3-private-getter-adapter.md','source/apps/moproj2mst/src/ops/type3_optimizer_defopt_view.hpp','source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp','source/apps/moproj2mst/src/ops/l7.hpp','source/apps/moproj2mst/src/ops/report.cpp','source/apps/moproj2mst/src/engine/shapeoptim_load.cpp']:
 p=root/name; bb=p.read_bytes(); sources.append(dict(path=name,bytes=len(bb),sha256=hashlib.sha256(bb).hexdigest()))
result=dict(checkpoint='CP452 ShapeOptim consumer static evidence',binary='evidence/original-binaries/DEF_GUI_MO_64.exe',binary_bytes=len(b),binary_sha256=hashlib.sha256(b).hexdigest(),method='Read PE file, export bounded dumpbin bytes, independently map addresses through PE section table and compare every exported byte; no original execution or mutable REA project',exports=exports,compared_bytes=sum(x['bytes'] for x in exports),addressed_instructions=sum(x['instructions'] for x in exports),mismatches=0,sources=sources,qualifications=['Native consumer export stops after OB+e9 store, before property traversal. Full consumer tail is retained decompile evidence, not newly assembly-verified.','Helper normal-path element sizes and native call arguments recovered; Qt detach, allocation and unwind implementations are outside private value projection.','Original native owner not executed; no GUI/app control acquired; no source/build/plan/state edits.'])
out=root/'analysis/cp452-shapeoptim-consumer-byte-verification.json'; out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['binary_sha256','compared_bytes','addressed_instructions','mismatches']}))
