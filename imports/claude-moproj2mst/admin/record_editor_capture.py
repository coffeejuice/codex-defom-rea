"""Retain a bounded owned native-Open capture, including failed probe attempts."""
import argparse
import collections
import hashlib
import json
import shutil
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
parser = argparse.ArgumentParser()
parser.add_argument('--attempt', type=int, required=True)
args = parser.parse_args()
assert 1 <= args.attempt <= 99
folder = root / f'build/original-editor-probe-{args.attempt:03d}'
output = root / f'analysis/wave3-original-editor-probe-{args.attempt:03d}-receipt.json'
assert not output.exists()
owned = read(folder / 'owned-process.json')
assert owned['absence_verified'] and owned['session_closed'] and not owned['cleanup_required']
assert not any(p['pid'] == owned['pid'] for p in read(folder/'absence-evidence.json')['structuredContent']['processes'])
evidence = read(folder/'capture-evidence.json')['structuredContent']
capture = evidence['normalized_result']
assert capture['pid'] == owned['pid'] and capture['session_id'] == owned['session_id']
payloads = [m['payload'] for m in capture['messages'] if m.get('type') == 'send']
pauses = [p for p in payloads if p.get('kind') == 'capture_paused']
issues = [m for m in capture['messages'] if m.get('type') == 'error' or m.get('payload', {}).get('kind') in
          ['callback_error','installation_error','native_exception','native_exception_truncated']]
modules = []
expected = read(root/'analysis/wave3-editor-owned-open-probe-v3-bindings.json')['modules']
hashes = {Path(x['path']).name: x['sha256'] for x in expected}
for p in payloads:
    if p.get('kind') != 'module': continue
    actual = sha(Path(p['path']))
    assert actual == hashes[p['name']]
    modules.append(dict(path=p['path'],base=p['base'],sha256=actual))
fixture = read(root/'analysis/wave3-editor-owned-open-fixture-manifest.json')
scratch = Path(fixture['proposed_directory'])
assert str(scratch).lower() == r'c:\problem\m2m42'
snapshot = folder / 'project-after'
assert not snapshot.exists()
snapshot.mkdir()
files = []
for p in sorted(scratch.rglob('*')):
    assert not p.is_symlink(), p
    if not p.is_file(): continue
    relative = p.relative_to(scratch)
    target = snapshot / relative
    target.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(p,target)
    digest = sha(p)
    assert sha(target) == digest
    files.append(dict(path=relative.as_posix(),bytes=p.stat().st_size,sha256=digest))
before = {x['relative_path']:x['sha256'] for x in fixture['files']}
after = {x['path']:x['sha256'] for x in files}
source = Path(fixture['source_directory'])
assert all(sha(source/p) == digest for p,digest in before.items()), 'Protected source changed'
reached = any(p.get('reason') == 'selected_editor_before_first_grid' for p in pauses)
record = dict(utc=datetime.now(timezone.utc).isoformat(),
    status='selected editor endpoint reached' if reached else 'partial capture; endpoint not reached',
    evidence_id=evidence['evidence_id'], pid=owned['pid'], session_id=owned['session_id'],
    script=dict(path=str((folder/'probe.js').relative_to(root)),sha256=sha(folder/'probe.js')),
    modules=modules,messages=len(capture['messages']),messages_truncated=capture['messages_truncated'],
    kinds=dict(collections.Counter(p.get('kind') for p in payloads)),pauses=pauses,issues=issues,
    cleanup=owned,project_after=dict(path=str(snapshot.relative_to(root)),files=files,
        changed=[p for p,h in before.items() if after.get(p)!=h],added=sorted(set(after)-set(before))),
    source_fixture_unchanged=True,raw_files={p.name:sha(p) for p in folder.glob('*.json')},
    qualifications=[
        'Native CLI startup Open precedes main show/event-loop; not interactive File/Open timing parity.',
        'Current user settings remain in effect; only owned process cwd changed before resume.',
        'No GUI input/control acquired. Exact PID termination and separate absence verification completed.',
        'Probe errors and native exceptions are distinct. Partial observations do not prove Open/Save/Close completion.',
        'All target-after files retained; original corpus unchanged. Captured native side effects remain evidence.'
    ])
output.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(receipt=str(output),status=record['status'],messages=record['messages'],
    pauses=[p['reason'] for p in pauses],issues=len(issues),changed=record['project_after']['changed'],added=record['project_after']['added'])))
