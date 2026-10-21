"""Bind retained evidence and validate the G1 static contract; never execute the target."""
import collections, hashlib, json, pathlib, re, subprocess

ROOT = pathlib.Path(__file__).resolve().parent.parent
ANALYSIS = ROOT / 'analysis'
RETAINED = pathlib.Path(r'C:\Users\alext\Projects\codex-deform-rea\analysis')
TARGET = 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def save(p, x): p.write_text(json.dumps(x, indent=2, ensure_ascii=False)+'\n', encoding='utf-8')
def compact(d):
    n=d.get('normalized_result', {})
    if isinstance(n,str): pseudo=n; assembly=[]; proc=d.get('parameters')
    else:
        pseudo=d.get('pseudocode',n.get('pseudocode',''))
        assembly=d.get('assembly',n.get('assembly',[]))
        proc=d.get('procedure',n.get('procedure'))
    return dict(evidence_id=d.get('evidence_id'),subject=d.get('subject'),provider=d.get('provider'),analysis_profile=d.get('analysis_profile'),procedure=proc,pseudocode=pseudo,assembly=assembly,limitations=d.get('limitations',[]))

cpath=ANALYSIS/'chat-c-db-controls-contract.json'
epath=ANALYSIS/'chat-c-db-controls-evidence.json'
c=read(cpath); e=read(epath)
names=set()
def refs(x):
    if isinstance(x,dict):
        for k,v in x.items():
            if k=='retained_sources': names.update(v)
            if k=='retained_source': names.add(v)
            refs(v)
    elif isinstance(x,list):
        for v in x:refs(v)
refs(c)
names.update(['mesh-0x1400015e0.json','import-0x1401c7080.json','import-0x1401c77c0.json','import-0x1401c7d40.json','root-layout-0x1401c8ba0.json','import-0x1403239f0.json','import-0x140324000.json','import-0x140323e50.json','import-0x140323de0.json','import-0x140323ec0.json'])
names.update('startup-0x140'+a+'.json' for a in ['1e7e90','1e8490','1e8650','1e8fb0','1e96b0','1ea6b0','1ead60'])
bindings=[]; projections=[]
for name in sorted(names):
    p=ANALYSIS/name
    if not p.exists():p=RETAINED/name
    assert p.exists(), name
    d=read(p); q=compact(d)
    b=dict(name=name,path=str(p),sha256=sha(p),bytes=p.stat().st_size,evidence_id=q['evidence_id'])
    if q['subject']:
        digest=q['subject'].get('digest',{}).get('sha256')
        assert digest==TARGET,(name,digest)
        b['target_sha256']=digest
    bindings.append(b)
    if q['pseudocode']:
        q['source']=b; projections.append(q)
e['retained_source_bindings']=bindings
e['retained_records']=projections
save(epath,e)
c['retained_source_bindings']=bindings
c['text_contract']['retained_sources']=[n for n in sorted(names) if any(a in n for a in ['3239f0','324020','324000','323e50','323de0','323ec0'])]
c['reset_and_defaults']['retained_sources'].append('root-layout-0x1401c8ba0.json') if 'root-layout-0x1401c8ba0.json' not in c['reset_and_defaults']['retained_sources'] else None

fresh={r['key']:r for r in e['records']}
main=fresh['g1_main_e']; code=main['pseudocode'].replace('\r',''); lines=code.split('\n')
checks=[]
def check(name,condition,details=None):
    assert condition,name
    checks.append(dict(check=name,result='pass',details=details))
check('all twenty fresh projections bound to target',len(e['records'])==20 and all(r['subject']['digest']['sha256']==TARGET for r in e['records']))
check('reference executable hash',sha(RETAINED.parent/'source'/'DEF_PRE_64.exe')==TARGET)
for row in c['direct_record_writes']:
    check('source statement line '+str(row['pseudocode_line']),lines[row['pseudocode_line']-1].strip()==row['statement'])
    assert int(row['root_offset'],16)==0x750+int(row['control_offset'],16)
    assert row['record_byte_offset']==row['index']*(4 if row['input']=='S' else 8)
check('201 direct writes across both branches',collections.Counter(x['branch'] for x in c['direct_record_writes'])=={'dimension != 2':102,'dimension == 2':99})
check('finite truncation instruction exists',any('CVTTSD2SI' in x for x in main['assembly']))
a=fresh['g1_coordinator']['assembly']
slots=[x for x in a if 'RSP + 0x20]' in x]
check('getter stack store and no coordinator reload',slots==['0x1401cc420: MOV byte ptr [RSP + 0x20], R9B','0x1401cc74c: MOV dword ptr [RSP + 0x20], EAX'])
check('single unit setter call with imported selector',sum('CALL 0x140235c30' in x for x in a)==1 and '0x1401cc775: MOV ECX, dword ptr [R11 + 0x84c]' in a and '0x1401cc434: SUB RSP, 0x48' in a)
keyasm=[x for x in main['assembly'] if 0x1401edc00<=int(x.split(':')[0],16)<=0x1401edc48]
check('nested key is a low32 stack argument','0x1401edc25: MOV dword ptr [RSP + 0x20], EAX' in keyasm)
c['units']['abi_audit']['nested_key']=dict(rule='T58 converts to int32, written as dword fifth argument. CONCAT44 upper-half noise in pseudocode is not a 64-bit property key.',sources=[main['evidence_id']],assembly=keyasm)
check('nested count wrapper forwards EAX',fresh['g1_nested_count']['assembly'][-3:]==['0x140375962: CALL 0x14013c1b0','0x140375967: ADD RSP, 0x28','0x14037596b: RET'])
check('nested fixed minimum21 tables',len(re.findall(r'FUN_14012d2a0\(uVar1,0x(?:8[e-f]|9[0-9a-f]|a[0-2])\)',fresh['g1_nested_resize']['pseudocode']))==21)
check('C538 value slot is table key in both branches','param_1 + 0x538,param_5,param_6,(int)(double)param_3[0x2a]' in code and 'param_1 + 0x538,param_5,param_6,(int)(double)param_3[0x24]' in code)
check('both text paths use fourth arg1',code.count('0x748),10,1);')==2)
check('stored reset has no default chain call','FUN_1401e7c50' not in fresh['g1_reset']['pseudocode'])
allids={r['evidence_id'] for r in e['records']}|{r['evidence_id'] for r in projections}
def ids(x):
    if isinstance(x,str) and x.startswith('ev_'):yield x
    elif isinstance(x,list):
        for v in x:yield from ids(v)
    elif isinstance(x,dict):
        for v in x.values():yield from ids(v)
check('contract evidence IDs resolve',all(x in allids for x in ids(c)))
session=read(ANALYSIS/'chat-c-db-controls-session.json')
check('owned session released',session['before']=={'open':False,'cleanup_pending':False} and session['after']=={'open':False,'cleanup_pending':False})
bundlepath=ANALYSIS/'oracle'/'chat-c-db-controls-evidence-bundle.json'
bundle=read(bundlepath)
check('full export contains every fresh evidence ID',all(r['evidence_id'] in set(ids(bundle)) for r in e['records']))
check('export byte receipt matches',bundlepath.stat().st_size==session['export']['bytes'])
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
check('isolated worktree base commit',head==c['base_commit'])
diff=subprocess.check_output(['git','diff','--name-only','HEAD'],cwd=ROOT,text=True).strip()
check('no tracked production or shared changes',diff=='',dict(tracked_diff=diff))
c['validation_summary']=dict(direct_statement_checks=201,fresh_static_dossiers=20,retained_source_files=len(bindings),retained_pseudocode_records=len(projections),proposed_tests=19,original_runtime_runs=0,tests_executed='Static artifact/source checks only; no implementation or original-runtime tests')
save(cpath,c)
artifacts=[]
for p in [cpath,epath,ANALYSIS/'chat-c-db-controls-session.json',bundlepath,pathlib.Path(__file__)]:
    artifacts.append(dict(path=str(p),sha256=sha(p),bytes=p.stat().st_size))
receipt=dict(schema_version=1,status='passed static consistency checks only',base_commit=head,target_sha256=TARGET,checks=checks,counts=c['validation_summary'],artifacts=artifacts,session=session,not_run=['build','unit tests','original executable','runtime probes','differential comparisons'],limitations=['Statement checks are self-consistency checks against retained Ghidra output, not independent semantic validation.','Full export unknowns=0 means no recorded REA unknown objects; unresolved scientific and integration questions remain in the contract.','Retained files remain in their original checkout and are content-hash bound; compact pseudocode projections are included for review.'])
save(ANALYSIS/'chat-c-db-controls-validation.json',receipt)
print(json.dumps(dict(status=receipt['status'],checks=len(checks),counts=receipt['counts'],artifacts=artifacts),indent=2))
