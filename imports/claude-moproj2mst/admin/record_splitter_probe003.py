"""Qualify the owned startup value/cache capture and verified cleanup."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
folder = root / 'build/original-splitter-probe-003'
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()

evidence = read(folder / 'capture-evidence.json')['structuredContent']
capture = evidence['normalized_result']
messages = capture['messages']
payloads = [m['payload'] for m in messages if m.get('type') == 'send']
complete, = [p for p in payloads if p.get('kind') == 'capture_complete_paused']
assert not capture['messages_truncated'] and not complete['truncations']
assert not complete['totals']['failedReads'] and not complete['totals']['callbackErrors']
assert not complete['openContexts']
assert not [m for m in messages if m.get('type') == 'error']
owned = read(folder / 'owned-process.json')
assert not owned['cleanup_required'] and owned['absence_verified'] and owned['session_closed']
assert owned['pid'] == capture['pid'] == complete['pid']
assert owned['session_id'] == capture['session_id']
absence = read(folder / 'absence-evidence.json')['structuredContent']
assert not any(p['pid'] == owned['pid'] for p in absence['processes'])
modules = []
for p in payloads:
    if p.get('kind') != 'module': continue
    observed = sha(Path(p['path']))
    assert observed == p['expectedFileSha256']
    modules.append(dict(path=p['path'], base=p['base'], sha256=observed))
assert len(modules) == 3
script = folder / 'probe.js'
assert sha(script) == '8a8d54f8c530c76443d14a9c21fadeb63f95d076fc96536d90dbc0c6c709626d'
record = dict(status='Completed owned startup capture; PID absence verified and Frida closed',
    utc=datetime.now(timezone.utc).isoformat(), evidence_id=evidence['evidence_id'],
    pid=owned['pid'], session_id=owned['session_id'], modules=modules,
    script=dict(path=str(script.relative_to(root)), sha256=sha(script)),
    counts=complete['totals'], messages=len(messages), errors=[], truncations=[],
    installed_hooks=complete['installedHooks'], cleanup=owned,
    raw_files={p.name:sha(p) for p in folder.glob('*.json')},
    qualifications=[
        'Startup through d35a80 return only; no project Open/Save/Close acceptance.',
        'Passive selected cache/value/parent observations; no Qt getter calls or event pump.',
        'No GUI input/control acquired. Target termination is not a GUI-control release.',
        'No full MainView child enumeration or general Qt behavior coverage claim.',
        'Raw QLocale eight-byte capture includes padding; only two uint16 fields are meaningful.',
        'Frida close reports target state unknown; separate process enumeration proves PID absence.',
        'Full inline Evidence retained. Source interpretation and implementation validation remain separate.'
    ])
path = root / 'analysis/wave3-original-splitter-probe-003-receipt.json'
assert not path.exists(), 'Never overwrite a capture receipt'
path.write_text(json.dumps(record, indent=2)+'\n', encoding='utf-8')
print(json.dumps(dict(receipt=str(path), messages=len(messages), totals=complete['totals'], cleanup_required=False)))
