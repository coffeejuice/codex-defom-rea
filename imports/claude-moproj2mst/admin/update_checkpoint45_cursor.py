"""Advance the verified44 continuation and exclusive source ownership."""
import json
from pathlib import Path
root = Path(__file__).resolve().parents[1]
path = root/'analysis/takeover-state.json'
state = json.loads(path.read_text(encoding='utf-8'))
assert state['efficiency']['completed_recovery_checkpoints'] == 44
actions = {
    'R300': '45 active: actual parentless editor base cursor on the same WidgetState; source-ordered private values, QObject thread/counter, registry, flags and direction. Continue ChildAdded/counter composition through shared APIs. Preserve precise modal/brush/post dependencies; allocate native UI only after base and wrappers return.',
    'RDOE': '45 active: actual main-window-owned lazy TaskFactoryRuntime and shared return-notifier receiver list. Suppression precedes all lookup. Commit factory/data and same-store pending Report allocation, then guard before unresolved QObject base. Existing parsed projection is only explicit isolated-fixture mode.',
    'RBOOL': '45 active: one shared font/device/Create constructor tail and QObject counter; actual ChildAdded identity and true result. Retain stack Create across posting boundary. Shared real queue/post/wake implementation is selected successor.',
    'RIO': '44 frozen148pass;45 workers implementing. Root integrates actual Session owner/bindings and native base allocation order, investigates application wrappers and source-qualified modal/palette capture. No owned original process or GUI input session currently live.',
    'RORACLE': 'Editor003 retained and cleaned; source-qualified first grid endpoint is not whole lifecycle acceptance. Root prepares bounded private-value/modal/palette/wrapper investigation; no guessed values or public guard bypass.'
}
for item in state['recovery_queue']:
    if item['id'] in actions:
        item['next_action'] = actions[item['id']]
        evidence = 'analysis/wave3-parented-grid-task-suppression-validation.json'
        if evidence not in item['evidence']: item['evidence'].append(evidence)
state['next'] = actions['RIO']
state['resume_cursor'].update(root=actions['RIO'], remesh_native=actions['R300'],
    reference_harness=actions['RDOE'], closure_next=actions['RBOOL'])
extra = {
    'remesh_native': ['include/m2m/engine/editor_base.hpp','src/engine/editor_base.cpp','tests/mdt/test_editor_base.cpp'],
    'reference_harness': ['include/m2m/doe_task_factory.hpp','src/doe/task_factory.cpp','tests/mdt/test_doe_task_factory.cpp',
        'include/m2m/doe_io.hpp','src/doe/io.cpp','include/m2m/doe_session_notifications.hpp','src/doe/session_notifications.cpp'],
    'root': ['include/m2m/engine/conversion_session.hpp']
}
for owner, files in extra.items():
    for p in files:
        if p not in state['active_source_ownership'][owner]: state['active_source_ownership'][owner].append(p)
state['efficiency']['checkpoint_count_qualification'] = '44 frozen checkpoints; count is not recovery percentage'
path.write_text(json.dumps(state,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(checkpoint=44,active=45,programs_passed=148,objective_complete=False)))
