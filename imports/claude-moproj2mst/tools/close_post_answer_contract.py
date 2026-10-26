"""Freeze proven Close scene inputs while preserving the shared DOE render gap."""
import hashlib
import json
from pathlib import Path
from doe_static_pe import PE, ROOT


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    pe = PE()
    files = ['analysis/wave3-close-save-return-contract.json',
             'analysis/wave2-doe-statevar-postparse-route.json',
             'analysis/wave2-doe-ghidra-text-verification.json',
             'analysis/wave2-doe-stepdata-statevar-control-order-receipt.json',
             'analysis/wave2-doe-stepdata-statevar-control-order-decompile.json',
             'analysis/wave2-doe-stepdata-object-statevar-postparse-receipt.json',
             'analysis/wave2-doe-stepdata-object-statevar-postparse-decompile.json',
             'analysis/wave2-doe-session-statevar-view-readers-receipt.json',
             'analysis/wave2-doe-session-statevar-view-readers-decompile.json']
    labels = ['close-post-answer', 'close-scene-getters', 'close-scene-mutations',
              'close-display-state', 'close-display-zero']
    files += [f'analysis/wave3-closure-next-{n}-static.json' for n in labels]
    for n in labels:
        r = json.loads((ROOT / f'analysis/wave3-closure-next-{n}-static.json').read_text())
        assert r['binary_sha256'] == sha(pe.data)
        for c in r['captures']:
            a, b = int(c['entry'], 0), int(c['end_exclusive'], 0)
            assert sha(pe.data[pe.off(a):pe.off(b)]) == c['code_sha256']
            assert sha((ROOT / c['artifact']).read_bytes()) == c['artifact_sha256']
    signal_address = 0x142a01168
    at = pe.off(signal_address)
    signal = pe.data[at:pe.data.index(0, at)].decode()
    assert signal == '2signalChangePtrkGraph()'
    result = {
        'schema': 'wave3-close-post-answer-inputs-v1',
        'status': 'caller_and_state_inputs_source_bound; shared_render_legend_and_notification_closure_pending',
        'method': 'Offline PE and bounded DUMPBIN, reusing byte-qualified private decompile receipts. No original runtime or GUI; no production changes.',
        'binary_sha256': sha(pe.data),
        'entry': '140d0e540(SfGLDisplay*, &variable0, &component0, &notifyTrue)',
        'position': 'After current-operation Save answer is accepted by044110, before signalRemoveGraphItems, manager Close, project destruction and diagram null attachment.',
        'proven_inputs_and_dispatch': [
            'Main-window+0x120 pointer, projected by main virtual+0x288 as pointer+0x10, supplies the display receiver. Zero/null pointer skips this callback. Exact virtual slots are in close-save-return-contract.',
            'cfac50 selects a displayed GuiModel: shown main+0x1a0 first, shown third+0x1c8 next, last shown named model from+0x1d8 next, then shown second+0x1b8 fallback. It can substitute the second under its final display/type condition. These are existing display links, not the diagram receiver+0x2c8.',
            'd0e540 also reads main+0x1a0, second+0x1b8 and d0cb50(display,true). The latter can lazily allocate the 0x130-byte display controller through d41bd0 when display+0x840 is null; this call is not a pure getter.',
            'For the selected nonnull GuiModel, c905b0 obtains its StateVar object and d15b80 runs with variable0, component0, notifyTrue. d15b80 brackets d16530(...,-1,-1,null) with state+0x450=true/false.',
            'd0e540 computes ranges through its dimension-dependent helpers, then visits shown named models from display+0x1d8. For each eligible named model it calls d30090(range), then d161f0(state,&variable0,&component0,&notifyTrue,-1,2). d161f0 similarly brackets d16530 with+0x450=true/false.',
            'At the tail, if the main-window pointer exists and the first integer remains0, invoke main virtual+0x360. Exact primary table1420fb108+0x360 contains140cc54a0. This callback is not elided.'
        ],
        'statevar_transition': [
            'd16530 runs d15bf0 on current object views, d302f0 cached-data handling, then stores variable to state+0x4c and component to+0x50. For these supplied inputs both stored values are0.',
            'Because notifyTrue, d16530 invokes notifier2cf3c0 with literal2signalChangePtrkGraph() before render updates. Fresh XML postparse uses notifyFalse; its skipped-direct-notification inference does not apply to Close.',
            'd16ae0 checks variable first: variable0 callsd16340 to clear per-view plots. Its nonzero-variable global rendering mutations are not made eligible merely by+0x450=true. Thus this receipt supersedes the intermediate message that the Close bracket alone activates those global writes.',
            'Then d30480(state,&true), skip var20000-only d30170, d17b90, d18980->d189b0, d15fb0, d16a80->display cff380. This is the shared DOE postparse dependency chain, with Close notification/mode inputs retained.',
            'd30090 looks up the requested variable id in143d76360 via2d7a80 and calls virtual+0x28 only for nonnull lookup. No assumption that registry id0 is absent is made.',
            'Neither the displayed-model selector nor the StateVar update is proof that all transitive notifications are harmless or that no pending timer can affect current attachment. Keep these effects separate from the proven diagram null-rebind contract.'
        ],
        'notification_literal': {'address': hex(signal_address), 'text': signal,
                                 'raw_sha256': sha(signal.encode()+b'\0')},
        'integration_boundary': {
            'required_profile_inputs': ['main display pointer availability',
                                        'shown main/second/third/named GuiModel links and types',
                                        'display controller+0x840 availability/state',
                                        'StateVar current object views and cached-data state',
                                        'registry lookup and notification recipients'],
            'safe_projection': 'Represent an explicit callback invocation with the exact (0,0,true) inputs and order. Source-proven StateVar writes/notifications can be projected when the shared render dependencies are supported. Do not default unavailable display state to an empty scene or declare this a no-op.',
            'shared_unresolved_work': 'DOE render/legend ownership and global/notification receiver state remain an existing gate; this worker does not duplicate that investigation or claim its completion.'
        },
        'evidence': [{'path': f, 'sha256': sha((ROOT/f).read_bytes())} for f in files],
        'tool_sha256': sha(Path(__file__).read_bytes())
    }
    out = ROOT/'analysis/wave3-close-post-answer-contract.json'
    assert not out.exists()
    out.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'path': out.relative_to(ROOT).as_posix(), 'sha256': sha(out.read_bytes()),
                      'linked_files_verified': len(files)}))


if __name__ == '__main__':
    main()
