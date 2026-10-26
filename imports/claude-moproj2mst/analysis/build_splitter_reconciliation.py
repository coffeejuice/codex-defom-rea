"""Compact, read-only reconciliation of the parent-owned frozen capture."""
import collections
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / 'build/original-splitter-probe-001'
def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def evidence(name):
    p = BASE / name
    data = json.loads(p.read_text())
    return data['structuredContent'], {'path': p.relative_to(ROOT).as_posix(), 'sha256': digest(p), 'evidence_id': data['structuredContent'].get('evidence_id')}

capture, binding = evidence('capture-evidence.json')
messages = [m.get('payload', {}) for m in capture['normalized_result']['messages']]
states = {m['stateId']: m['data'] for m in messages if m.get('kind') == 'state'}
phases = [m for m in messages if m.get('kind') == 'phase']
complete = next(m for m in messages if m.get('kind') == 'capture_complete_paused')
roles = {r['object']: r['role'] for r in complete['roles']}
def identity(pointer):
    return roles.get(pointer, pointer)
def phase_roles(phase):
    return {states[i].get('role'): states[i] for i in phase['objects']}
def compact_object(v):
    w = v.get('widget', {})
    return {'role': v.get('role'), 'parent': identity(v.get('parent')),
            'children': [identity(p) for p in v.get('children', {}).get('entries', [])],
            'pendingChildren': [identity(x['child']) for x in v.get('pendingChildren', {}).get('entries', [])],
            'attributesLow': w.get('attributesLow'), 'attributesExtended': w.get('attributesExtended'),
            'windowFlags': w.get('windowFlags'), 'cachedWidgetItem': w.get('cachedWidgetItem')}

event_rows, active_phase = [], None
results = {m['eventId']: m for m in messages if m.get('kind') == 'notify_leave'}
for m in messages:
    if m.get('kind') == 'phase': active_phase = m['ordinal']
    if m.get('kind') in ('notify_enter', 'post_event'):
        event_rows.append({'id': m['eventId'], 'afterPhase': active_phase, 'kind': m['kind'],
                           'role': identity(m['receiver']), 'type': hex(m['type']),
                           'child': identity(m.get('child')), 'result': results.get(m['eventId'], {}).get('result')})

supp, supp_binding = evidence('final-children-supplement.json')
supp_data = next(m['payload'] for m in supp['normalized_result']['messages'] if m.get('payload', {}).get('kind') == 'final_mainwindow_children_supplement')
bindings = [binding, supp_binding]
for name in ('script-load.json', 'session-start.json', 'termination-evidence.json', 'process-absence-evidence.json', 'session-close-evidence.json'):
    _, b = evidence(name)
    bindings.append(b)
for rel in ('analysis/wave3-original-splitter-probe-receipt.json', 'build/original-splitter-probe-001/probe.js', 'build/original-splitter-probe-001/probe-reviewed.js'):
    p = ROOT / rel
    bindings.append({'path': rel, 'sha256': digest(p)})

final = phase_roles(phases[-1])
shared = [m for m in messages if m.get('kind') == 'state' and m.get('stateType') == 'shared']
assert len(shared) == 1
visibility = []
for p in phases:
    if p['stage'] == 'set_visible_enter':
        obj = p['extra']['object']
        v = next(v for v in phase_roles(p).values() if v.get('object') == obj)
        visibility.append({'phase': p['ordinal'], 'role': identity(obj), 'requested': p['extra']['requested'],
                           'beforeLow': v['widget']['attributesLow'], 'beforeExtended': v['widget']['attributesExtended']})

out = {
    'scope': 'Independent reconciliation of one owned original View_Default startup capture; no worker runtime or GUI input',
    'bindings': bindings,
    'counts': {'messages': len(messages), **complete['counts'], 'roles': len(complete['roles'])},
    'runtime_coverage': {'readErrors': complete['counts']['readsFailed'], 'callbackErrors': complete['counts']['callbackErrors'],
        'truncations': complete['truncations'], 'finalSupplementChildCount': len(supp_data['children']),
        'qualification': 'Final mainwindow supplement supplies205 identities only at stopped endpoint. Earlier first64 truncation remains. Captured entry/return states do not prove all intervening callback behavior.'},
    'roles': complete['roles'],
    'shared_observation': shared[0]['data'],
    'language': [m for m in messages if m.get('kind') == 'language'],
    'ordered_event_rows': event_rows,
    'selected_transitions': [{'phase': p['ordinal'], 'stage': p['stage'], 'objects': [compact_object(v) for v in phase_roles(p).values() if v.get('role') in ('mainView', 'manager', 'splitter')]} for p in phases if p['ordinal'] in (8, 11, 26, 43, 45, 58, 59)],
    'final_splitter_records': final['splitter']['splitter']['records'],
    'visibility_calls': visibility,
    'observed_corrections': [
        'QObject children finally order placeholder1,placeholder0,handle0,handle1; logical records stay placeholder0+handle0 then placeholder1+handle1. Pending-child vector preserves insertion order, a third distinct order.',
        'After each parentless placeholder is reparented, its own hidden bit is clear. Handle1 receives setVisible(true) in phase46. The all-hidden recalc shortcut does not describe either full constructor insertion.',
        'Constructor HideSecondary reaches setVisible(false) with placeholder1 hidden bit clear and extended explicit-hide bit clear, then changes low/extended fields and emits raw0x1b. The previously qualified early-return branch is not selected here.',
        'SplitView parent starts actual MainView and becomes actual manager at phases57/58. MainView pending vector still contains splitter afterward, while its QObject child list removes it.',
        'Handle-specific fields omitted before derivedReady remain unobserved. Source establishes pre-base private defaults; omission does not establish uninitialized native memory.',
        'Nonzero QWidget init optional object is constant in recorded states. Exact650776c0 is pointer-key hash insertion of actual QWidget before QWidgetData/parent setup; it cannot be treated as absent.'
    ],
    'probe_defect': {
        'classification': 'Worker script defect preserved in reviewed script; explicit investigation rework',
        'site': 'probe-reviewed.js:414 grid=result retains onLeave return-value wrapper beyond callback; selectors391/395 compare against that wrapper',
        'api_source': 'https://frida.re/docs/javascript-api/#interceptor',
        'api_contract': 'Frida documents that onLeave retval is recycled across calls and must be copied with ptr(retval.toString()) before storage.',
        'binding': 'Original IAT141e08000 is QGridLayout::addWidget(widget,int,int,QFlags), exported at65040290. Both addWidget and factory65043930 hook bytes verified. The missed phases cannot be used as absence evidence.',
        'causal_qualification': 'Stored grid value was not logged at missed selectors; recycled-wrapper misuse is source/API-proven and explains both selection misses, rather than a directly observed comparison value.',
        'lost_coverage': ['grid_add_splitter_enter/return', 'grid_item_factory_enter/return', 'final concrete grid item/box vector and factory result'],
        'still_valid': ['grid baseline captured immediately in getter callback', 'actual manager cached grid pointer', 'actual splitter reparent events and parent/children snapshots', 'final splitter cachedWidgetItem pointer'],
        'repair': 'Frozen scripts unchanged. Only when these results are needed, use a separate narrow successor probe with copied retval and final grid state; no broad duplicate trace.'
    },
    'conclusions': [
        'Fresh observed profile supports actual role/parent/child and visibility decisions; it does not supply a replacement for native constructor effects.',
        'Keep same cached grid and resume source d349d4; do not replay pre-grid clear calls.',
        'Next cohesive target is actual SplitView QWidget construction/parenting plus first placeholder progress, with one shared runtime widget identity registry. Ordered constructor blockers are documented separately before production release.',
        'The missing grid box/factory evidence is not essential before that constructor boundary; no new original capture is requested by this reconciliation.'
    ]
}
path = ROOT / 'analysis/wave3-close-splitter-probe-reconciliation.json'
assert not path.exists()
path.write_text(json.dumps(out, indent=2) + '\n')
print(json.dumps({'path': path.relative_to(ROOT).as_posix(), 'sha256': digest(path), 'bytes': path.stat().st_size, 'counts': out['counts']}))
