"""Bind checkpoint44 sources and record the due efficiency review before building."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
def read(p): return json.loads((ROOT / p).read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256((ROOT / p).read_bytes()).hexdigest()
def save_new(p, value):
    target = ROOT / p
    assert not target.exists(), p
    target.write_text(json.dumps(value, indent=2) + '\n', encoding='utf-8')

state_path = ROOT / 'analysis/takeover-state.json'
state = read('analysis/takeover-state.json')
assert state['efficiency']['completed_recovery_checkpoints'] == 43
receipts = [
    'analysis/wave3-editor-first-grid-thread-owner-amendment.json',
    'analysis/wave3-close-shared-parented-grid-landing.json',
    'analysis/wave2-doe-session-task-suppression-implementation.json',
]
files = {}
for receipt in receipts:
    record = read(receipt)
    entries = record.get('files', record.get('sources', record.get('source')))
    if isinstance(entries, dict):
        entries = [dict(path=p, **v) for p, v in entries.items()]
    for entry in entries:
        path, expected = entry['path'], entry['sha256']
        assert path not in files, path
        assert sha(path) == expected, path
        files[path] = expected
capture_path = 'analysis/wave3-original-editor-probe-003-receipt.json'
capture = read(capture_path)
assert capture['cleanup']['absence_verified'] and capture['cleanup']['session_closed']
assert not capture['cleanup']['cleanup_required']
assert not capture['errors'] and not capture['truncations']
assert not capture['directory_effects']['changed'] and capture['source_fixture_unchanged']
now = datetime.now(timezone.utc)
integration_path = 'analysis/wave3-parented-grid-task-suppression-root-integration.json'
save_new(integration_path, dict(
    utc=now.isoformat(), status='Source integration bound; candidate build/tests still required',
    worker_receipts={p: sha(p) for p in receipts}, source_files=files,
    root_production_edits=[],
    integration=[
        'The existing actual editor owner composes caller bool-variant teardown and first pending GridLayoutState. Actual thread control-block identity, parent membership and partial native flags are preserved.',
        'All installed-layout reads share the WidgetState alias; event dispatch cannot bypass a newly installed layout through stale observations.',
        'Root review found same-address foreign shared_ptr ownership could pass thread checks. The amendment closes that hole and adds initial/resumed adversarial coverage before this build.',
        'Parsed DOE Session import now reads the actual shared suppression gate before task type/factory lookup. Suppressed null Task preserves record allocation, append, named-model clear and independent payload failures.',
        'Existing root ConversionSession integration and m2m ABI remain unchanged; public editor base and active DOE completion remain guarded.'
    ],
    original_capture=dict(path=capture_path, sha256=sha(capture_path), records=65,
        endpoint='first editor grid UI publication before spacing', cleanup_verified=True),
    qualifications=[
        'Source-backed components do not constitute full public lifecycle integration.',
        'Fresh CLI Open timing is qualified separately from interactive Open; this capture does not exercise Save or Close.',
        'Historical 93/98 comparisons and passing checkpoint43 are baselines, not validation of checkpoint44.'
    ],
    successors_started=[
        'Shared constructor-tail font/device/Create/post and QObject counter interface design.',
        'Parentless editor-base cursor design using the actual shared WidgetState and new capture.',
        'Actual lazy17-prototype task factory constructor and preference ownership investigation.'
    ]))

prior_path = state['efficiency']['last_review']
prior = read(prior_path)
build = read('analysis/wave3-shared-events-session-records-v1-build.json')
review_path = 'analysis/wave3-checkpoint43-hourly-efficiency-review.json'
save_new(review_path, dict(
    at_utc=now.isoformat(), trigger='One hour elapsed; safe boundary after capture cleanup and source freeze',
    prior_review=prior_path, prior_checkpoint=42, checkpoint=43,
    elapsed_clock_seconds=(now - datetime.fromisoformat(prior['at_utc'])).total_seconds(),
    verified_builds=1, configure_build_seconds=sum(x['elapsed_seconds'] for x in build['steps']),
    latest_ctest_seconds=state['efficiency']['latest_ctest_seconds'],
    captures=['analysis/wave3-original-splitter-probe-004-receipt.json', capture_path],
    token_measurement=None, speedup_claim=None,
    interpretation='Elapsed time includes research, integration and tool latency; it is not measured active coding time or agent speedup.',
    decisions=[
        'Keep three workers on exclusive source lanes; root alone builds, integrates and runs reference processes.',
        'Use bundled source-derived constructor captures; the latest capture covered parentless base, shared events, first grid, queue and wake in one disposable process.',
        'Intern native event/queue identities independently of snapshot descriptors; preserve raw captures and corrections.',
        'Move validated source contracts into actual ownership and public lifecycle integration; isolated helpers remain explicitly incomplete.',
        'No percentage increase from this narrow increment; use the existing qualified estimate until acceptance coverage changes materially.',
        'Continue candidate44 integration immediately; perform the next review after three checkpoints or one hour at a safe boundary.'
    ]))
state['efficiency'].update(last_review=review_path, last_review_checkpoint=43,
    last_review_utc=now.isoformat(), next_review='After checkpoint46 or one hour at next safe boundary',
    checkpoint_count_qualification='43 frozen checkpoints; count is not recovery percentage')
state['latest_owned_capture'] = capture_path
state['next'] = 'Candidate44 source frozen and reviewed; bind/build/test first-grid/shared-layout and DOE suppression. Successor constructor-tail, parentless-base and actual lazy Task factory investigations are active.'
state['resume_cursor']['root'] = state['next']
for item in state['recovery_queue']:
    if item['id'] in ['R300', 'RBOOL', 'RDOE', 'RIO', 'RORACLE']:
        for path in [integration_path, capture_path]:
            if path not in item['evidence']: item['evidence'].append(path)
    if item['id'] == 'RIO': item['next_action'] = state['next']
    if item['id'] == 'RORACLE':
        item['next_action'] = 'Editor003 capture completed and cleaned at first grid UI publication. Source-bind constructor/event/queue effects; whole fresh Open-Save-Close acceptance remains outstanding.'
state_path.write_text(json.dumps(state, indent=2) + '\n', encoding='utf-8')
print(json.dumps(dict(integration=integration_path, verified_files=len(files), review=review_path,
    checkpoint=43, next='Build/test candidate44', objective_complete=False)))
