"""Bind checkpoint42 root integration to released worker source/evidence."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
def sha(path): return hashlib.sha256((root / path).read_bytes()).hexdigest()
receipts = [
    'analysis/wave3-editor-native-setup-prefix-implementation.json',
    'analysis/wave2-doe-type2-task-retention-implementation.json',
    'analysis/wave3-close-widget-values-prefix-landing.json',
    'analysis/wave3-original-splitter-probe-003-receipt.json',
    'analysis/wave3-close-widget-values-probe003-contract.json',
]
for name in receipts[:3]:
    data = json.loads((root / name).read_text(encoding='utf-8'))
    files = data.get('source', data.get('source_files', data.get('files')))
    pairs = files.items() if isinstance(files, dict) else ((x['path'], x['sha256']) for x in files)
    for path, expected in pairs:
        assert sha(path) == expected, path
prefix = 'source/apps/moproj2mst/'
files = [
    'include/m2m/doe_io.hpp', 'src/doe/io.cpp',
    'include/m2m/engine/conversion_session.hpp', 'src/engine/conversion_session.cpp',
    'src/core/convert.cpp', 'tests/mdt/test_conversion_session.cpp',
    'tests/mdt/test_doe_default_session.cpp', 'tests/mdt/test_doe_output_data.cpp',
    'tests/mdt/test_doe_session_notifications.cpp',
]
record = dict(utc=datetime.now(timezone.utc).isoformat(),
    status='Root source integration; build/regression/freeze are separate validation receipts',
    sources={prefix+p:sha(prefix+p) for p in files}, evidence={p:sha(p) for p in receipts},
    integration=[
        'Actual ConversionSession retains one TaskAllocationStore; StudyIo binds it weakly before Open. Same pointer/control is idempotent; null, replacement and expired rebinding refuse.',
        'Type2 Task and cleared Session-record allocation retention is separate from Project storage. Explicit adapter-scope release invalidates Task observers despite escaped store leases; this is not native Close deletion.',
        'Existing public editor guard now retains the same factory/setup native-prefix dependency. Unknown editor-time objectName stops before native stores. No startup defaults or later page effects are applied early.',
        'Actual WidgetRuntime receives probe003 values after grid/focus setup and before SplitView allocation. Shared font/palette aliases, native cache order and actual manager pending entry are preserved.',
        'MainView gesture-map exclusivity is a source-and-capture inference, separately qualified in the contract. Later constructor/font-device and public manager aliases remain guarded.',
        'Session integration tests cover actual owner identity, weak lifetime, foreign control blocks, escaped-store invalidation, shared value identity, separate masks/groups and ordered pending children.',
        'External DOE fixtures now retain explicit test allocation stores. No local production fallback owner is introduced.'
    ],
    acceptance='Public editor, active DOE, original full Open/Save/Close comparison, native failure/Close and complete architecture remain unfinished.',
    successor='Native command-line Open/editor capture, 17-prototype Task factory initialization and QWidget font paint-device binding are already assigned and active.')
path = root / 'analysis/wave3-widget-task-ownership-root-integration.json'
assert not path.exists()
path.write_text(json.dumps(record, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'receipt':str(path), 'root_files':len(files), 'released_worker_hashes':'matched'}))
