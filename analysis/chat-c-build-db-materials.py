"""Project saved evidence and validate the G2 contract without target execution."""
import hashlib,json,pathlib,re,subprocess,collections
ROOT=pathlib.Path(__file__).resolve().parent.parent
A=ROOT/'analysis'
OLD=pathlib.Path(r'C:\Users\alext\Projects\codex-deform-rea\analysis')
TARGET='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def save(p,d):p.write_text(json.dumps(d,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
def compact(e):
 n=e.get('normalized_result',{})
 if isinstance(n,str):p=n;asm=[];proc=e.get('procedure',e.get('parameters',{}))
 else:p=e.get('pseudocode',n.get('pseudocode',''));asm=e.get('assembly',n.get('assembly',[]));proc=e.get('procedure',n.get('procedure',{}))
 return dict(evidence_id=e.get('evidence_id'),subject=e.get('subject'),provider=e.get('provider'),analysis_profile=e.get('analysis_profile'),procedure=proc,pseudocode=p,assembly=asm,limitations=e.get('limitations',[])+(n.get('limitations',[]) if isinstance(n,dict) else []))

bundlepath=A/'oracle'/'chat-c-db-materials-evidence-bundle.json'
bundle=read(bundlepath)
addresses='1cff20 1d0180 28f300 336740 1d7090 1cc180 28a740 28b3b0 28b780 28c420 28a950 29ccd0 29cd20 29cd30 29cd50 130290 28b050 28b240 28a4c0 28a580 130200 130160 12fff0 29cd60 28a530'.split()
fresh={}
for e in bundle['records']:
 n=e.get('normalized_result')
 if e.get('operation')=='analyze_function' and isinstance(n,dict):
  addr=n.get('procedure',{}).get('address','').removeprefix('0x140')
  if addr in addresses:fresh[addr]=compact(e)
assert len(fresh)==25
retained=[]; bindings=[]
want='1c77c0 1c7d40 1c7d10 0015e0 0015f0 29c060 29c260 29c2c0 29c310 29e210 28cb70 28c6f0 28cdc0 28b850 339df0 333f60 33a120 33a990 334490 33a040 1d6250 1cb440 1cf4a0 1c8f40 1f4090 12fc70 12d050 12f410 12d2a0 484ab8 324020 324000 323e50 323de0 323ec0 3239f0'.split()
for addr in want:
 candidates=sorted(OLD.glob('*0x140'+addr+'.json'))
 if not candidates:candidates=sorted(A.glob('*0x140'+addr+'.json'))
 assert candidates,addr
 p=candidates[0];e=read(p);q=compact(e)
 b=dict(path=str(p),sha256=sha(p),bytes=p.stat().st_size,evidence_id=q['evidence_id'])
 q['source_binding']=b;retained.append(q);bindings.append(b)
for n in ['material-import-next-contract.json','relation-import-static-contract.json','relation-property-static-contract.json','nested-material-static-contract.json']:
 p=OLD/n;bindings.append(dict(path=str(p),sha256=sha(p),bytes=p.stat().st_size,kind='retained contract; KEY runtime claims do not validate DB'))
g1path=A/'chat-c-db-controls-evidence.json';g1=read(g1path)
g1keys=['g1_table_read','g1_table_typed','g1_typed_rank1','g1_typed_rank2','g1_typed_rank2_copy','g1_coordinator','g1_unit_set']
reused=[x for x in g1['records'] if x['key'] in g1keys]
bindings.append(dict(path=str(g1path),sha256=sha(g1path),bytes=g1path.stat().st_size,kind='G1 retained evidence'))
for n in ['chat-c-db-controls-contract.json','chat-c-db-model-contract.json','README.md']:
 p=A/n if n!='README.md' else ROOT/n
 bindings.append(dict(path=str(p),sha256=sha(p),bytes=p.stat().st_size,kind='prior contract or project scope'))

def extract(q,scale):
 lines=q['pseudocode'].replace('\r','').split('\n');writes=[];calls=[];stack=[];pending='';aliases={}
 for ix,line in enumerate(lines,1):
  t=line.strip()
  if t.startswith('}') and stack:stack.pop()
  if re.match(r'(if |else|switch|for |while)',t):pending=t
  if '{' in t:stack.append(dict(header=pending or '{',line=ix,cases=[]));pending=''
  if re.match(r'case |default:',t):
   for frame in reversed(stack):
    if frame['header'].startswith('switch'):
     if frame['cases'] and not lines[ix-2].strip().startswith('case '):frame['cases']=[]
     frame['cases'].append(t);break
  context=[dict(header=x['header'],line=x['line'],cases=list(x['cases'])) for x in stack if x['header']!='{']
  m=re.match(r'(\w+) = (.+);$',t)
  if m:aliases[m[1]]=dict(line=ix,expression=m[2])
  dest=None;storage=None;rhs=None
  m=re.match(r'param_1\[(0x[0-9a-f]+|\d+)\] = (.+);$',t)
  if m:dest=int(m[1],0)*scale;storage='int32';rhs=m[2]
  m2=re.match(r'\*\((\w+) \*\)\(param_1 \+ (0x[0-9a-f]+|\d+)\) = (.+);$',t)
  if m2:dest=int(m2[2],0)*scale;storage=m2[1];rhs=m2[3]
  if t.startswith('*param_1 = '):dest=0;storage='int32';rhs=t[len('*param_1 = '):-1]
  if dest is not None:
   src=re.search(r'(?:param_2\[([^]]+)\]|\*param_2)',rhs)
   index=src[1] if src and src[1] else ('0' if src else None)
   row=dict(destination_byte_offset=hex(dest),storage=storage,rhs=rhs,source_record_index=index,source_line=ix,statement=t,guard_context=context,evidence_id=q['evidence_id'])
   if src and re.fullmatch(r'0x[0-9a-f]+|\d+',index):row['source_record_byte_offset']=int(index,0)*8
   if re.search(r'\b(?:dVar1|iVar3|uVar4|local_\w+)\b',rhs):row['nearest_alias_assignments']={a:v for a,v in aliases.items() if re.search(r'\b'+re.escape(a)+r'\b',rhs)}
   writes.append(row)
  if 'FUN_140' in t and not t.startswith('void ') and re.search(r'FUN_140(?:1309b0|130dc0|131ea0|131e30|28a740|28b3b0|28c420)',t):
   text=line;j=ix
   while ';' not in text and j<len(lines):text+='\n'+lines[j];j+=1
   expr=re.search(r'param_1 \+ (0x[0-9a-f]+|\d+)',text)
   if not expr:
    first=re.search(r'FUN_140[0-9a-f]+\((\w+),',text)
    if first and first[1] in aliases:expr=re.search(r'param_1 \+ (0x[0-9a-f]+|\d+)',aliases[first[1]]['expression'])
   calls.append(dict(source_line=ix,callee=re.search(r'FUN_(0x)?(140[0-9a-f]+)',text)[2],destination_byte_offset=hex(int(expr[1],0)*scale) if expr else None,statement=text.strip(),guard_context=context,nearest_alias_assignments={a:v for a,v in aliases.items() if re.search(r'\b'+re.escape(a)+r'\b',text)},evidence_id=q['evidence_id']))
 return dict(writes=writes,calls=calls,source_lines=len(lines),note='Guard context is a source navigation aid, not a compiled predicate evaluator. Else depends on preceding branch; nearest aliases require the surrounding source path. Full pseudocode governs order and branch meaning.')

maps={'material':extract(fresh['28f300'],4),'relation':extract(fresh['336740'],1)}
epath=A/'chat-c-db-materials-evidence.json'
save(epath,dict(schema_version=1,kind='Compact evidence projection; full pseudocode and assembly retained, other dossier facets remain in full bundle',target_sha256=TARGET,fresh=list(fresh.values()),retained=retained,reused_g1=reused,source_bindings=bindings,transport=dict(material_parent='ev_9e614bb965773892ecb0d438f8b142f89527c0f6c234a9206f890277efec9ec6',complete_pseudocode_code_units=39776,complete_assembly_rows=3301,pages=[dict(offset=0,examined=16000),dict(offset=16000,examined=14000),dict(offset=30000,examined=9776)],high_pcode_truncated=True)))
mpath=A/'chat-c-db-materials-field-map.json';save(mpath,dict(schema_version=1,notation='Material decompiler param_1 is int*: multiply destination index by4. Relation param_1 is a byte address. Both input records are float64 slots.',**maps))
print(json.dumps(dict(fresh=len(fresh),retained=len(retained),bindings=len(bindings),maps={k:{x:len(v[x]) for x in ['writes','calls']} for k,v in maps.items()},evidence_bytes=epath.stat().st_size)))

cpath=A/'chat-c-db-materials-contract.json'
if not cpath.exists():raise SystemExit(0)
c=read(cpath);c['source_bindings']=bindings
c['evidence_ids']={a:q['evidence_id'] for a,q in fresh.items()}
checks=[]
def ck(name,ok,detail=None):
 assert ok,name
 checks.append(dict(check=name,result='pass',detail=detail))
ck('target executable hash',sha(OLD.parent/'source'/'DEF_PRE_64.exe')==TARGET)
for q in list(fresh.values())+retained+reused:
 if q.get('subject'):ck('target bound '+q['evidence_id'],q['subject']['digest']['sha256']==TARGET)
for kind,addr in [('material','28f300'),('relation','336740')]:
 lines=fresh[addr]['pseudocode'].replace('\r','').split('\n')
 for row in maps[kind]['writes']:ck(kind+' write source line '+str(row['source_line']),lines[row['source_line']-1].strip()==row['statement'])
 for row in maps[kind]['calls']:ck(kind+' call source line '+str(row['source_line']),'\n'.join(lines[row['source_line']-1:row['source_line']-1+len(row['statement'].split('\n'))]).strip()==row['statement'])
m=fresh['1cff20']['pseudocode'];r=fresh['1d0180']['pseudocode']
ck('material append precedes deserialization',m.index('FUN_14029c060')<m.index('FUN_14028f300'))
ck('rank remapping precedes reconciliation',m.index('FUN_14029c260')<m.index('free(_Memory)')<m.index('FUN_1401d6250'))
ck('relation loops use material count',r.count('local_48 < *(int *)(param_1 + 0x1320)')==1 and r.count('local_18 < *(int *)(param_1 + 0x1320)')==1 and '0x13f4' not in r)
ck('relation record matrix stride', 'local_18 * *(int *)(param_1 + 0x1320) + local_48' in r)
ck('rank wrapper forwards RAX',fresh['1d7090']['assembly'][-3:]==['0x1401d70ad: CALL 0x14029c260','0x1401d70b2: ADD RSP, 0x28','0x1401d70b6: RET'])
ck('material full body retained',len(fresh['28f300']['pseudocode'])==39776 and len(fresh['28f300']['assembly'])==3301)
ck('typed version tail has no key positivity gate','FUN_140130dc0(param_1 + 0x858,param_3,param_4,(int)param_2[0x99]);' in fresh['28f300']['pseudocode'])
pair_example=[dict(list_ordinal=i*3+j,storage_ordinal=j*3+i,keys=[ids[i],ids[j]]) for i in range(3) for j in range(3) for ids in [[17,-4,90]]]
ck('ordered matrix formula example', [x['storage_ordinal'] for x in pair_example]==[0,3,6,1,4,7,2,5,8])
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
ck('isolated base',head=='d546058bde8c7c245485959dca8608b507be5ce4')
ck('no tracked edits',subprocess.check_output(['git','diff','--name-only','HEAD'],cwd=ROOT,text=True).strip()=='')
session=read(A/'chat-c-db-materials-session.json')
ck('export receipt bytes',bundlepath.stat().st_size==session['export']['bytes'])
ck('own session closed',session['close']=={'result':None})
if session.get('after') is not None:ck('post-close session empty',session['after']=={'open':False,'cleanup_pending':False})
c['validation_summary']=dict(static_checks=len(checks),fresh_dossiers=25,retained_functions=len(retained),maps={k:{x:len(v[x]) for x in ['writes','calls']} for k,v in maps.items()},runtime_runs=0)
save(cpath,c)
artifacts=[dict(path=str(p),bytes=p.stat().st_size,sha256=sha(p)) for p in [cpath,epath,mpath,bundlepath,A/'chat-c-db-materials-session.json',pathlib.Path(__file__)]]
save(A/'chat-c-db-materials-validation.json',dict(schema_version=1,status='static consistency checks passed; no runtime equivalence claim',checks=checks,counts=c['validation_summary'],artifacts=artifacts,session=session,ordered_pair_formula_example=pair_example,not_run=['original runtime','build','production tests','scientific validation']))
print(json.dumps(dict(counts=c['validation_summary'],artifacts=artifacts),indent=2))
