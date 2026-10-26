"""Retain the Close guard's nested Save answer and warning-overload contract."""
import hashlib
import json
import struct
from pathlib import Path
from doe_static_pe import PE, ROOT


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    pe = PE()
    files = ['analysis/wave3-normal-close-guard-contract.json',
             'analysis/wave3-boolean-open-sequence-contract.json']
    files += [f'analysis/wave3-closure-next-{n}-static.json' for n in
              ['close-nested-save', 'close-post-answer', 'close-scene-owner',
               'close-scene-getters', 'close-lastop-warning', 'close-warning-forward']]
    for name in files[2:]:
        r = json.loads((ROOT / name).read_text())
        p = PE(ROOT / r['binary'])
        assert sha(p.data) == r['binary_sha256']
        for c in r['captures']:
            a, b = int(c['entry'], 0), int(c['end_exclusive'], 0)
            assert sha(p.data[p.off(a):p.off(b)]) == c['code_sha256']
            assert sha((ROOT / c['artifact']).read_bytes()) == c['artifact_sha256']
    table = 0x1420fb108
    slots = []
    for offset, expected in [(0x288, 0x140c9c1d0), (0x778, 0x140c9bbd0), (0x3a0, 0x140029300)]:
        raw = pe.data[pe.off(table+offset):pe.off(table+offset)+8]
        assert struct.unpack('<Q', raw)[0] == expected
        slots.append({'table': hex(table), 'offset': hex(offset), 'target': hex(expected),
                      'raw_sha256': sha(raw)})
    result = {
        'schema': 'wave3-close-nested-save-return-v1',
        'status': 'nested_Save_answer_and_warning_overload_closed; post_answer_scene_inputs_bound; scene_effects_pending',
        'method': 'Offline original PE and bounded DUMPBIN; no runtime, application control, DLL loading or builds.',
        'binary_sha256': sha(pe.data),
        'save_return': [
            '0980d0 initializes answer=0x4000 at0980e5 and an independent write-success byte=true. If project+0x55 is false it sets answer=0x10000 and returns it.',
            'For each nonnull subproject, call0b8fb0 and replace answer with its result. Any result other than0x10000 returns immediately before current-operation Save and output writes.',
            'After checks, call current operation052ec0 if present, project write0983c0(false), properties0982e0, MST0a20d0, then timer/settings bookkeeping. Project-write false does not skip properties or MST. Combined write success is stored in a local byte and is not returned.',
            '0982a0 returns the answer integer. With at least one nonnull subproject whose checks pass, answer is0x10000 regardless of the later project/MST boolean write success. With no nonnull subproject it remains0x4000.',
            'Caller044110 replaces Yes with this returned answer. Exactly0x4000 is remapped toCancel0x400000. Therefore ordinary checked Save permits Close despite a boolean output-write failure; no-nonnull-subproject Save cancels Close. This is original control flow, not a proposed error-handling improvement.'
        ],
        'last_operation_warning': {
            'precondition': '0b8fb0 detects an operation marked must-be-last that precedes another connector-chain operation; the existing must-be-last structural contract is not expanded here.',
            'normal_return': '0b8fb0 initializes answer=0x10000 and returns that unchanged if no warning is required.',
            'call': 'At0b94b3 import141e0b7b8 is ?warning@QMessageBox@@SAHPEAVQWidget@@AEBVQString@@1W4StandardButton@1@2@Z; r9=0x4000 and fifth argument=0x10000.',
            'overload': 'QtGui65018420 wraps ONLY first enum0x4000 in a QFlags object, and forwards second enum0x10000 as default to654777e0. That wrapper passes icon2 to shared65477580. Thus available buttons=Yes only, requested default=No (absent). This is not Yes|No.',
            'default_answer': 'Shared helper creates the sole Yes button and leaves explicit default unset because No is absent. Fresh dialog focus/autoDefault selects that sole button; answer0x4000. This default does not depend on relative Yes/No layout order.',
            'consequence': '0b8fb0 returns0x4000, 0980d0 stops before Save writes, and normal Close044110 maps it toCancel and leaves project attached. The warning modal loop remains an event delivery opportunity.',
            'source_comment_correction': 'source/apps/moproj2mst/src/engine/save_project.cpp currently describes a Yes/No choice and throws unsupported. Exact shipped overload has only Yes; root owns any implementation change.'
        },
        'post_answer': {
            'slots': slots,
            'order': [
                'If final answer is notCancel, guard044110 calls main virtual+0x288 at0447e1.',
                'c9c1d0 calls main virtual+0x778, bound to c9bbd0. c9bbd0 passes main+0x120 to c1aef0, a direct pointer load. If nonnull, c9c1d0 returns that object+0x10; otherwise null.',
                'For nonnull return,0447fa..044838 builds integers0,0 and booltrue and calls d0e540(pointer,&int0,&int0,&boolTrue). Then emit signalRemoveGraphItems through notifier2cf3c0. Guard returns true only after these calls.',
                'd0e540 resolves primary scene item cfac50, secondary cf9960 (+0x1b8), primary cf94f0 (+0x1a0), and enabled cached scene controller d0cb50. It can mutate item display state and walks collection at scene+0x1d8. Full fresh effects are not declared a no-op by this receipt.'
            ],
            'remaining': 'Bind d0e540 mutation callees and scene-model provenance with DOE worker before representing this callback as an exact Close state transition. The input arguments and call position are proven.'
        },
        'qualification_addendum': 'In wave3-normal-close-guard-contract.json, shipped Windows style means the captured Qt style implementation path. Application startup selection of that style was not separately recovered; no runtime/custom-style/environment override assertion is intended.',
        'evidence': [{'path': f, 'sha256': sha((ROOT/f).read_bytes())} for f in files],
        'tool_sha256': sha(Path(__file__).read_bytes())
    }
    path = ROOT / 'analysis/wave3-close-save-return-contract.json'
    assert not path.exists()
    path.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'path': path.relative_to(ROOT).as_posix(), 'sha256': sha(path.read_bytes()),
                      'linked_files_verified': len(files)}))


if __name__ == '__main__':
    main()
