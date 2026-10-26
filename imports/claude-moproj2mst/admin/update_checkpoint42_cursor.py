"""Reconcile verified39-41 work and the active42 integration queue."""
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
path = root / 'analysis/takeover-state.json'
state = json.loads(path.read_text(encoding='utf-8'))
assert state['efficiency']['completed_recovery_checkpoints'] == 41
updates = {
    'R300': ('39 setup-to-page owner handoff and40 pending picking owner verified. Active42 integrates released native setup name/resize/property prefix on same owner; recover editor-time base-widget/filter context next. Preserve setup, picking and public Save guards.', ['analysis/wave3-editor-setup-page-handoff-implementation.json', 'analysis/wave3-picking-object-writer-validation.json', 'analysis/wave3-editor-native-setup-prefix-implementation.json']),
    'RDOE': ('39 actual property-table finalization and40 actual Object writer verified. Active42 repairs Type2 Task and Session-record allocation retention with shared Session-owned store and stable identities; native prototype initialization/rebinding remains queued.', ['analysis/wave3-owner-table-focus-validation.json', 'analysis/wave3-picking-object-writer-validation.json', 'analysis/wave2-doe-live-task-registry-ownership-contract.json']),
    'RBOOL': ('39 parent/focus prefix verified. Probe003 completed and owned PID absence verified. Active42 binds native cache/value evidence to actual WidgetRuntime and advances source-ordered parent font/palette/locale/event tail without fake success gates.', ['analysis/wave3-owner-table-focus-validation.json', 'analysis/wave3-original-splitter-probe-003-receipt.json']),
    'RIO': ('41 allocator tail discrepancy repaired and verified. Root integrates42 shared Task allocation owner and actual editor setup dependency, then builds/regresses/freezes and immediately advances remaining lifecycle dependencies.', ['analysis/wave3-allocator-tail-fidelity-validation.json', 'analysis/wave3-allocator-low-level-tests.json'])
}
for item in state['recovery_queue']:
    if item['id'] not in updates: continue
    action, evidence = updates[item['id']]
    item['next_action'] = action
    for receipt in evidence:
        assert (root / receipt).is_file(), receipt
        if receipt not in item['evidence']: item['evidence'].append(receipt)
state['next'] = updates['RIO'][0]
state['resume_cursor'].update({
    'root': '41 frozen145pass plus17lowlevel cases; 93/98 historical unchanged. Active42 editor prefix released and root integrating; DOE allocationstore in progress; closure probe003 captured/cleaned and source implementation next. No GUI control or live owned original process.',
    'remesh_native': 'Released five native setup files; root integrating. Recover exact editor-time base constructor and notification context next.',
    'reference_harness': 'Implement TaskAllocationStore and stable Task/Session identity retention. Type2 list clear retains raw Session allocations as well as Tasks; source amendment required. Root owns actual Session/StudyIo binding.',
    'closure_next': 'Probe003 accepted:254messages,165records,56states,zero errors/truncation. Implement cohesive typed WidgetRuntime cache/value and parent-tail prefix in owned files; no native full startup claim.'
})
path.write_text(json.dumps(state, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'checkpoint':41,'active':42,'queue_reconciled':list(updates),'live_owned_process':False}))
