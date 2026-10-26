"""Persist the live shared-owner increment without advancing verified acceptance."""
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
path = root / 'analysis/takeover-state.json'
state = json.loads(path.read_text(encoding='utf-8-sig'))
assert state['efficiency']['completed_recovery_checkpoints'] == 42
actions = {
    'R300': '43 in progress: one actual editor WidgetState bound to Session WidgetRuntime; setup uses shared dynamic-property and retained stack-event dispatch. Preserve UnknownObjectName production guard until earlier base constructor is closed. Next recover parented-grid ChildAdded and raw4c post ordering.',
    'RDOE': '43 in progress: allocation store owns ALL SessionRecords from native allocation, Part/Document lists are nonowning. Preserve records/backlinks through clear and destruction; adapter-scope cleanup remains distinct from native Close. Next actual 17-prototype factory metadata/font/model children.',
    'RBOOL': '43 in progress: sole WidgetRuntime-owned ThreadState, live widget properties and source-closed rawaa event dispatch. Actual posted queue remains unknown. Next implement reviewed bundled constructor-tail passive probe, root-only capture and cleanup.',
    'RIO': 'Integrate frozen43 source, run focused and full converter tests, inspect differences then freeze. Three workers continue read-only successors during root build. No owned original process or GUI input session is live.'
}
for item in state['recovery_queue']:
    if item['id'] in actions:
        item['next_action'] = actions[item['id']]
state['next'] = actions['RIO']
state['resume_cursor'].update(root=actions['RIO'], remesh_native=actions['R300'],
    reference_harness=actions['RDOE'], closure_next=actions['RBOOL'])
state['active_source_ownership'] = {
    'root': ['src/engine/conversion_session.cpp', 'tests/mdt/test_conversion_session.cpp',
        'src/doe/output_data.cpp', 'tests/mdt/test_doe_output_data.cpp',
        'tests/mdt/test_doe_default_session.cpp', 'tests/mdt/test_doe_session_notifications.cpp'],
    'remesh_native': ['include/m2m/engine/editor_setup.hpp', 'src/engine/editor_setup.cpp',
        'include/m2m/engine/simcontrol300_owner.hpp', 'src/engine/simcontrol300_owner.cpp', 'tests/mdt/test_editor_setup.cpp'],
    'closure_next': ['include/m2m/engine/view_manager.hpp', 'src/engine/view_manager.cpp', 'tests/mdt/test_view_manager.cpp',
        'include/m2m/engine/widget_events.hpp', 'src/engine/widget_events.cpp', 'tests/mdt/test_widget_events.cpp'],
    'reference_harness': ['include/m2m/doe_task_runtime.hpp', 'src/doe/task_runtime.cpp', 'tests/mdt/test_doe_task_runtime.cpp',
        'include/m2m/doe_session_records.hpp', 'src/doe/session_records.cpp', 'tests/mdt/test_doe_session.cpp',
        'include/m2m/doe_session.hpp']
}
state['active_source_ownership_path_base'] = 'source/apps/moproj2mst'
path.write_text(json.dumps(state, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'verified_checkpoint': 42, 'active_checkpoint': 43, 'worker_count': 3,
    'build_running': False, 'owned_original_process_live': False}))
