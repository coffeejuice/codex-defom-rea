"""Retain constructor-tail capture qualifications and verify owned cleanup."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
folder = root/'build/original-splitter-probe-004'
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
evidence = read(folder/'capture-evidence.json')['structuredContent']
capture = evidence['normalized_result']
messages = capture['messages']
payloads = [m['payload'] for m in messages if m.get('type') == 'send']
complete, = [p for p in payloads if p.get('kind') == 'capture_complete_paused']
assert not capture['messages_truncated'] and not complete['truncations'] and not complete['openContexts']
assert not complete['totals']['failedReads'] and not complete['totals']['callbackErrors']
assert not [m for m in messages if m.get('type') == 'error']
owned = read(folder/'owned-process.json')
assert owned['pid'] == capture['pid'] == complete['pid']
assert owned['session_id'] == capture['session_id']
absence = read(folder/'absence-evidence.json')['structuredContent']
assert not any(p['pid'] == owned['pid'] for p in absence['processes'])
closed = read(folder/'close-evidence.json')
assert not closed.get('isError') and closed['structuredContent']['target_state'] == 'unknown'
owned.update(cleanup_required=False,absence_verified=True,session_closed=True,
    observed_endpoint='d35a80 return paused in probe; exact owned PID subsequently terminated')
(folder/'owned-process.json').write_text(json.dumps(owned,indent=2)+'\n',encoding='utf-8')
preflight = read(folder/'root-preflight.json')
scratch = Path(preflight['cwd'])
after = {p.relative_to(scratch).as_posix():sha(p) for p in sorted(scratch.rglob('*')) if p.is_file()}
assert after == preflight['cwd_files'], 'Retain and inspect unexpected original directory effects'
modules = []
for p in payloads:
    if p.get('kind') == 'module':
        observed = sha(Path(p['path']))
        assert observed == p['expectedFileSha256']
        modules.append(dict(path=p['path'],base=p['base'],sha256=observed))
assert len(modules) == 3
assert sha(folder/'probe.js') == preflight['script_sha256']
markers = []
def scan(v,path):
    if isinstance(v,dict):
        for k,value in v.items():
            if k in ('unavailable','malformed','incomplete') and value:
                markers.append(dict(path=path+'/'+k,value=value))
            scan(value,path+'/'+k)
    elif isinstance(v,list):
        for i,value in enumerate(v): scan(value,path+'/'+str(i))
scan(payloads,'payloads')
record = dict(status='Completed owned startup capture; absence verified and Frida closed',
    utc=datetime.now(timezone.utc).isoformat(),evidence_id=evidence['evidence_id'],
    pid=owned['pid'],session_id=owned['session_id'],modules=modules,
    script=dict(path=str((folder/'probe.js').relative_to(root)),sha256=sha(folder/'probe.js')),
    counts=complete['totals'],messages=len(messages),errors=[],truncations=[],
    nested_qualification_markers=markers,installed_hooks=complete['installedHooks'],cleanup=owned,
    directory_effects=dict(cwd=str(scratch),files=len(after),changed=[],added=[],removed=[]),
    raw_files={p.name:sha(p) for p in folder.glob('*.json')},
    qualifications=[
        'First actual SplitView constructor tail through d35a80 return; no project Open/Save/Close comparison.',
        'Passive metric/DC/font/count/Create/post/dispatcher/frame observations; no Qt calls or event pump.',
        'No GUI input/control acquired. Exact owned-process termination is not a GUI-control release.',
        'Raw frame/policy/queue/event bytes are usable only at source-qualified initialization points; incidental earlier bytes do not establish initialized state.',
        'Opaque queued receivers remain evidence aliases, not fabricated live widget owners or supported dispatch.',
        'Current startup observations do not prove later editor-time population or values.',
        'Frida close reports unknown target state; separate enumeration establishes PID absence.',
        'Full inline Evidence retained; source interpretation, integration and acceptance remain separate.'
    ])
target = root/'analysis/wave3-original-splitter-probe-004-receipt.json'
assert not target.exists()
target.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'receipt':str(target),'messages':len(messages),'counts':complete['totals'],
    'nested_qualification_markers':markers,'cleanup_required':False,'directory_files_unchanged':len(after)}))
