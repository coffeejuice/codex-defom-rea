"""Retain exact editor base/first-grid observation and independently verify cleanup."""
import hashlib
import json
import shutil
from datetime import datetime, timezone
from pathlib import Path
root=Path(__file__).resolve().parents[1]
folder=root/'build/original-editor-probe-004'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
evidence=read(folder/'capture-evidence.json');c=evidence['normalized_result']
p=[m['payload'] for m in c['messages'] if m.get('type')=='send']
complete,=[v for v in p if v.get('kind')=='capture_complete_paused']
endpoint,=[v for v in p if v.get('stage')=='selected_editor_first_grid_published']
assert all(endpoint['data'][k] for k in ('mainReady','normalOpen','postOpen','setupEntered'))
assert not c['messages_truncated'] and not complete['truncations'] and not complete['openContexts']
assert not complete['totals']['failedReads'] and not complete['totals']['callbackErrors']
assert not [m for m in c['messages'] if m.get('type')=='error']
assert not [v for v in p if v.get('kind') in ('native_exception','native_exception_truncated')]
owned=read(folder/'owned-process.json')
assert c['pid']==owned['pid']==complete['pid'] and c['session_id']==owned['session_id']
assert not any(v['pid']==owned['pid'] for v in read(folder/'absence-evidence.json')['processes'])
closed=read(folder/'close-evidence.json');assert not closed.get('isError')
assert closed['target_state']=='unknown'
owned.update(cleanup_required=False,absence_verified=True,session_closed=True,
    observed_endpoint='140ed9a11 paused; exact owned PID subsequently terminated')
(folder/'owned-process.json').write_text(json.dumps(owned,indent=2)+'\n',encoding='utf-8')
pre=read(folder/'root-preflight.json');assert sha(folder/'probe.js')==pre['script_sha256']
modules=[]
for v in p:
    if v.get('kind')=='module':
        h=sha(Path(v['path']));assert h==v['expectedFileSha256']
        modules.append(dict(path=v['path'],base=v['base'],sha256=h))
assert len(modules)==3
scratch=Path(pre['cwd']);snapshot=folder/'project-after';assert not snapshot.exists();snapshot.mkdir()
after={}
for file in sorted(scratch.rglob('*')):
    assert not file.is_symlink() and not file.is_junction()
    if not file.is_file():continue
    rel=file.relative_to(scratch);dest=snapshot/rel;dest.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(file,dest);digest=sha(file);assert sha(dest)==digest;after[rel.as_posix()]=digest
before=pre['cwd_files'];assert after==before
fixture=read(root/'analysis/wave3-editor-owned-open-fixture-manifest.json')
assert all(sha(Path(fixture['source_directory'])/x['relative_path'])==x['sha256'] for x in fixture['files'])
markers=[]
def scan(v,path):
    if isinstance(v,dict):
        for k,x in v.items():
            if k in ('unavailable','malformed','incomplete') and x:markers.append(dict(path=path+'/'+k,value=x))
            scan(x,path+'/'+k)
    elif isinstance(v,list):
        for i,x in enumerate(v):scan(x,path+'/'+str(i))
scan(p,'payloads')
record=dict(status='Native CLI Open to first editor grid publication reached; owned process cleaned',
    utc=datetime.now(timezone.utc).isoformat(),evidence_id=evidence['evidence_id'],pid=owned['pid'],
    session_id=owned['session_id'],script=dict(path=str((folder/'probe.js').relative_to(root)),sha256=sha(folder/'probe.js')),
    modules=modules,messages=len(c['messages']),counts=complete['totals'],errors=[],truncations=[],
    nested_qualification_markers=markers,endpoint=endpoint,cleanup=owned,
    directory_effects=dict(path=str(snapshot.relative_to(root)),files=after,changed=[],added=[],removed=[]),
    source_fixture_unchanged=True,raw_files={x.name:sha(x) for x in folder.glob('*.json')},
    qualifications=['Native CLI startup Open precedes window show/event-loop; not interactive File/Open timing parity.',
        'Stops after140ed9a0e UI publication and before spacing/margins; no full editor or Save/Close acceptance.',
        'No GUI input, Qt calls, event pump or target-memory assignment; owned-process cwd selected explicitly.',
        'Current user settings remain in effect; this is not a pristine-settings/environment claim.',
        'Raw high layout/dirty/event bits and incidental earlier widget fields remain qualified by source initialization.',
        'Queue records retain actual order/aliases; opaque receivers are not implemented live widget owners.',
        'Exact PID absence independently verified before Frida close; close itself reports unknown target state.'])
out=root/'analysis/wave3-original-editor-probe-004-receipt.json';assert not out.exists()
out.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'receipt':str(out),'messages':len(c['messages']),'counts':complete['totals'],
    'nested_qualification_markers':markers,'files_unchanged':len(after),'cleanup_required':False}))
