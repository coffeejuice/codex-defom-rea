"""Freeze the offline normal-Close guard and fresh Windows dialog evidence."""
import hashlib
import json
import struct
from pathlib import Path
from doe_static_pe import PE, ROOT


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    exe = PE()
    gui = PE(ROOT / 'evidence/original-binaries/QtGui4_64.dll')
    files = [
        'analysis/wave3-normal-close-diagram-lifetime-contract.json',
        'analysis/wave3-directory-failure-dialog-contract.json',
        'analysis/wave3-boolean-open-sequence-contract.json',
        'analysis/wave3-boolean-save-pump-14008a9b0.asm',
        'analysis/wave3-boolean-external-callback-14002a0e0.asm',
    ]
    labels = ['can-close', 'can-close-model', 'can-close-gates', 'can-close-mode-tail',
              'can-close-mode-ret', 'question', 'question-helper', 'close-question-focus',
              'close-proctype', 'question-layout', 'question-style', 'question-style-init',
              'close-native-open-mode', 'close-reset-signals']
    files += [f'analysis/wave3-closure-next-{label}-static.json' for label in labels]
    for path in files[5:]:
        receipt = json.loads((ROOT / path).read_text())
        target = PE(ROOT / receipt['binary'])
        assert receipt['binary_sha256'] == sha(target.data)
        for capture in receipt['captures']:
            a, b = int(capture['entry'], 0), int(capture['end_exclusive'], 0)
            assert sha(target.data[target.off(a):target.off(b)]) == capture['code_sha256']
            assert sha((ROOT / capture['artifact']).read_bytes()) == capture['artifact_sha256']
    links = [{'path': f, 'sha256': sha((ROOT / f).read_bytes())} for f in files]
    vtables = []
    for table, offset, target in [(0x142115f68, 0x58, 0x140c7a100),
                                   (0x142112878, 0x68, 0x140044110)]:
        raw = exe.data[exe.off(table + offset):exe.off(table + offset) + 8]
        assert struct.unpack('<Q', raw)[0] == target
        vtables.append({'table': hex(table), 'offset': hex(offset),
                        'target': hex(target), 'raw_sha256': sha(raw)})
    style = []
    for name, lookup, table, index, expected in [
        ('QWindowsStyle', 0x6535af9c, 0x6535af74, 0x45, 0x6535af45),
        ('QWindowsXPStyle', 0x6530fdc0, 0x6530fd9c, 0x45, 0x6530fd5c),
        ('QCommonStyle', 0x652b59f4, 0x652b5934, 0x45-4, 0x652b5915)]:
        n = gui.data[gui.off(lookup + index)]
        raw = gui.data[gui.off(table + n*4):gui.off(table + n*4) + 4]
        branch = gui.base + struct.unpack('<I', raw)[0]
        assert branch == expected
        style.append({'style': name, 'hint': '0x45', 'lookup': hex(lookup + index),
                      'lookup_byte': n, 'table_entry': hex(table + n*4),
                      'branch': hex(branch), 'raw_sha256': sha(bytes([n]) + raw)})
    layout_raw = gui.data[gui.off(0x656fc580):gui.off(0x656fc580) + 56]
    layout = list(struct.unpack('<14I', layout_raw))
    assert [x for x in layout if x in (1, 5, 6)] == [5, 6, 1]
    out = {
        'schema': 'wave3-normal-close-guard-v1',
        'status': 'process_type_and_nested_save_before_detach_source_bound; default_Yes_for_shipped_fresh_Windows_style',
        'method': 'Offline retained PE, bounded DUMPBIN and prior qualified evidence; no application runtime, GUI, DLL load, build or shared database.',
        'binaries': {'DEF_GUI_MO_64.exe': sha(exe.data), 'QtGui4_64.dll': sha(gui.data)},
        'vtables': vtables,
        'process_type': [
            '1402cb780 stores *int_arg to global143d766b8+0xc; 1402cb7a0 reads that exact field. 1402cb7c0 tests 1 or 5, and 1402cb870 tests 5.',
            'This is the application process type, separate from settings Setting/MoInputMode at settings+0xc90. No equality between those fields is assumed.',
            'Normal Open registration 14002a0e0 calls 140013d40(main,0,0) at 14002a11b. For mode argument 0, 013d40 resolves current project through0133a0/045780; null project selects5, project+0x55 true selects5, false selects1.',
            'Native .moproj loader14008a9b0 stores 14008afa0 return byte into project+0x55 at14008ae1d. 08afa0 returns native reader08b410 success byte. Thus successful native Open selects process type5.',
            '140010a70 also writes5 through2cb780; its startup caller is not needed for the post-Open binding and is not asserted here.'
        ],
        'guard': [
            'Normal Close0299a0 calls manager can-close before busy=true, refresh resets, manager deletion or diagram detach. Manager vtable+0x58 is c7a100; it calls each owned model vtable+0x68 and aborts on false. SfMoModel slot is044110.',
            '044110 immediately allows Close if project+0x19c0 is null or process type is neither1 nor5. Otherwise it asks a question; no modified/dirty flag is tested in this body.',
            'When project+0x55 is true and process type5, the question is current-operation Save; otherwise it asks whether to save a Keyword file. Both use Yes0x4000 | No0x10000 | Cancel0x400000, combined0x414000, and explicit defaultNoButton0.',
            'Current-operation question returns at0444dc. Yes calls project Save0980d0 at044507, while project and diagram are still attached; replace answer with its integer result. If result equals0x4000, remap toCancel0x400000. Other return values are not treated as failure by this guard merely because they are zero.',
            'Keyword question Yes invokes main-window virtual+0x3a0 at04479a; this receipt does not resolve that alternate branch.',
            'When final answer differs fromCancel, invoke main-window virtual+0x288 and signalRemoveGraphItems before returning true. Actual detach follows only after all guards return true. The nested Save and callback bodies require their own contracts.'
        ],
        'question_default': {
            'answer': 'Yes (0x4000)',
            'qualification': 'Static fresh dialog with the shipped Windows/Common, WindowsXP or WindowsVista style and unchanged initial focus. No original runtime observation, user focus movement, alternate/custom style, environment style override or external event-filter behavior is claimed.',
            'chain': [
                'QtGui654777a0 question delegates65477580 with icon4. Helper adds the ascending standard-button bits and, for default0, explicitly chooses only AcceptRole0. Yes role5, No role6 and Cancel role1 therefore leave the explicit default unset.',
                'QDialogButtonBox private init6539b500 queries styleHint0x45. WindowsVista delegates this hint toXP; XP delegates toWindows; Windows delegates toCommon. Common starts result0 and jumps to its return for this hint.',
                'Horizontal layout0 raw role table656fc580 puts roles5,6,1 in that order: Yes,No,Cancel. Layout6539c680 sets focus proxy to first widget and calls QWidget::setTabOrder for subsequent widgets at6539cb1b.',
                'QPushButton ctor653f8270 initializes autoDefault state2. autoDefault653f7410/742c resolves true for a QDialog ancestor. QDialog::setVisible6545ac70 selects the first focus-chain autoDefault button when no default exists; focusInEvent653f75c0 also promotes the initially focused autoDefault button.',
                'QDialog::keyPressEvent6545b1d0 handles Return/Enter by finding the visible enabled default QPushButton and calling click65371ee0. Helper65477580 returns the clicked standard button.'
            ],
            'style_dispatch': style,
            'layout_table': {'address': '0x656fc580', 'roles': layout, 'raw_sha256': sha(layout_raw)}
        },
        'event_delivery': 'The question uses QDialog::exec and therefore the modal event loop already source-bound by the directory-failure-dialog contract. Pending eligible timers may run before the default answer, and nested0980d0 contains additional Save delivery points. A fresh default answer does not imply zero event delivery. Preserve explicit due-event schedules; do not infer wall-clock arrival.',
        'integration': 'For successful native MO Open -> Save -> normal Close under the recovered default: question modal delivery, answerYes, nested0980d0 Save, non-Cancel post-answer callbacks, then existing Close cleanup/null attachment contract. Do not implement normal public Close as detach alone or suppress its nested Save based on a reconstructed dirty flag.',
        'remaining': ['Bind nested0980d0 return values and post-answer main virtual+0x288.',
                      'Source-selected shipped Windows style is established here; a full original fresh runtime comparison remains pending under the parent validation gate.'],
        'evidence': links,
        'tool_sha256': sha(Path(__file__).read_bytes())
    }
    path = ROOT / 'analysis/wave3-normal-close-guard-contract.json'
    assert not path.exists()
    path.write_text(json.dumps(out, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'path': path.relative_to(ROOT).as_posix(), 'sha256': sha(path.read_bytes()),
                      'linked_files_verified': len(files)}))


if __name__ == '__main__':
    main()
