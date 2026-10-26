"""Bind the completed owned startup observation to files, hashes, and cleanup."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
folder = root / 'build/original-splitter-probe-002'
def read(p): return json.loads(p.read_text(encoding='utf-8'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
evidence = read(folder / 'capture-evidence.json')['structuredContent']
capture = evidence['normalized_result']
messages = capture['messages']
payloads = [m['payload'] for m in messages if m.get('type') == 'send']
complete, = [p for p in payloads if p.get('kind') == 'capture_complete_paused']
assert not capture['messages_truncated'] and not complete['truncations']
assert not complete['totals']['failedReads'] and not complete['totals']['callbackErrors']
assert not [m for m in messages if m.get('type') == 'error']
owned = read(folder / 'owned-process.json')
assert not owned['cleanup_required'] and owned['absence_verified'] and owned['session_closed']
modules = []
for p in payloads:
    if p.get('kind') != 'module': continue
    observed = sha(Path(p['path']))
    assert observed == p['expectedFileSha256']
    modules.append(dict(path=p['path'], base=p['base'], sha256=observed))
record = dict(status='Completed owned startup capture; process absence verified and Frida session closed',
              utc=datetime.now(timezone.utc).isoformat(), evidence_id=evidence['evidence_id'],
              pid=owned['pid'], session_id=owned['session_id'], modules=modules,
              script=dict(path='build/original-splitter-probe-002/probe-reviewed-v2.js',
                          sha256=sha(folder/'probe-reviewed-v2.js')),
              counts=complete['totals'], messages=len(messages),
              errors=[], truncations=[], coverage=complete['coverage'],
              codepages=[p for p in payloads if p.get('kind')=='codepages'],
              cleanup=owned, raw_files={p.name:sha(p) for p in folder.glob('*.json')},
              qualifications=['Only startup through d35a80 return; no Primary extension or Open/Save/Close acceptance.',
                              'Focus entry predecessor is pre-state; no exact post-partition operand claim.',
                              'Code pages apply to this owned process/environment, not arbitrary hosts.',
                              'No mouse/keyboard control was acquired; terminating owned target is not a GUI control release.',
                              'Full inline Evidence retained; bounded observations do not establish full Qt behavior.'])
path=root/'analysis/wave3-original-splitter-probe-002-receipt.json'
path.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(receipt=str(path), messages=len(messages), totals=complete['totals'],
                     codepages=[(p['acp'],p['oemcp']) for p in record['codepages']], cleanup_required=False)))
