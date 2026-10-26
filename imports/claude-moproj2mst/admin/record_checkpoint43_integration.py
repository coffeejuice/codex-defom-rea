"""Bind root integration to frozen worker files; validation follows separately."""
import argparse
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument('--doe-receipt', required=True)
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
def sha(p): return hashlib.sha256((root/p).read_bytes()).hexdigest()
workers = [
    'analysis/wave3-editor-shared-widget-property-increment.json',
    'analysis/wave3-close-widget-shared-events-landing.json',
    args.doe_receipt,
]
worker_files = []
for name in workers:
    data = json.loads((root/name).read_text(encoding='utf-8-sig'))
    files = data.get('source', data.get('source_files', data.get('files', data.get('sources'))))
    pairs = files.items() if isinstance(files, dict) else ((x['path'], x['sha256']) for x in files)
    for path, expected in pairs:
        assert sha(path) == expected, path
        assert path not in worker_files, path
        worker_files.append(path)
prefix = 'source/apps/moproj2mst/'
files = ['src/engine/conversion_session.cpp', 'tests/mdt/test_conversion_session.cpp',
    'src/doe/output_data.cpp', 'tests/mdt/test_doe_output_data.cpp',
    'tests/mdt/test_doe_default_session.cpp', 'tests/mdt/test_doe_session_notifications.cpp']
record = dict(utc=datetime.now(timezone.utc).isoformat(),
    status='Source integration only; build, behavioral tests and frozen candidate recorded separately',
    worker_receipts={p:sha(p) for p in workers}, verified_worker_file_count=len(worker_files),
    root_sources={prefix+p:sha(prefix+p) for p in files},
    integration=[
        'ConversionSession binds actual parentless editor component to its existing WidgetRuntime immediately after factory allocation and before setup allocation. No synthetic QWidget base completion or allWidgets insertion.',
        'Setup diagnostic access and test assertions now observe the sole live component; old objectName/event copies removed. Earlier unknown entry state remains guarded at the same public loader boundary.',
        'Actual shared event dispatch retains application/thread/receiver/event identities, partial notification depth and exact continuation. Unknown queue population is explicit, never fabricated empty.',
        'DOE output scans ordered nonowning SessionRecord pointers. Null pointer consumption is an explicit unresolved native invalid-access guard, never silently skipped.',
        'External DOE fixtures use explicit allocation-store owners and pointer access. The notifier-only test now has empty SessionRoots so it tests its intended later notification boundary.',
        'Session integration regression binds a Task to an actual store-owned record, destroys its document/list, releases Project storage and verifies the same record/backlink survives. Adapter-scope end invalidates both despite an escaped store lease.'
    ],
    qualifications=[
        'No new public path is enabled merely by eliminating an unsupported result.',
        'Adapter end cleanup is not an implementation or proof of original normal Close.',
        'Full editor, native base/post queue, active DOE lazy factory and fresh whole-lifecycle acceptance remain incomplete.'
    ],
    successors_started=[
        'Closure worker is implementing the reviewed passive constructor-tail capture script, root-only launch.',
        'Editor worker is closing caller QVariant cleanup and parentless/parented-grid dependencies read-only.',
        'DOE worker resumes actual lazy Task factory metadata/model/font initialization after source freeze.'
    ])
target = root/'analysis/wave3-shared-events-session-records-root-integration.json'
assert not target.exists()
target.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'receipt':str(target),'worker_files':len(worker_files),'root_files':len(files)}))
