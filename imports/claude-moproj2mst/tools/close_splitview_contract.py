"""Bind the final Close scene callback's secondary-view effects to original bytes."""
import hashlib
import json
from pathlib import Path
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import imports


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    pe = PE()
    files = ['analysis/wave3-close-post-answer-contract.json']
    labels = ['close-tail-notification', 'close-tail-getters', 'close-tail-splitview',
              'close-tail-layout', 'close-tail-layout-lazy', 'close-tail-layout-cleanup',
              'close-scene-getters']
    for label in labels:
        name = f'analysis/wave3-closure-next-{label}-static.json'
        receipt = json.loads((ROOT / name).read_text())
        assert receipt['binary_sha256'] == sha(pe.data)
        for capture in receipt['captures']:
            begin, end = int(capture['entry'], 0), int(capture['end_exclusive'], 0)
            assert sha(pe.data[pe.off(begin):pe.off(end)]) == capture['code_sha256']
            assert sha((ROOT / capture['artifact']).read_bytes()) == capture['artifact_sha256']
        files.append(name)
    assert pe.u64(pe.off(0x1420fb108 + 0x360)) == 0x140cc54a0
    assert pe.u64(pe.off(0x1420fb108 + 0x7b8)) == 0x140ca6210
    literals = {}
    for address in [0x142911e30, 0x142911e40, 0x142911e58, 0x142911e68, 0x14395b8d8,
                    0x143706c20, 0x1436362f0]:
        at = pe.off(address)
        data = pe.data[at:pe.data.index(0, at)]
        literals[hex(address)] = {'value': data.decode(), 'sha256_with_nul': sha(data + b'\0')}
    im = imports(pe)
    used_imports = {hex(address): im[address] for address in [
        0x141e0b858, 0x141e07698, 0x141e08388, 0x141e07ca8, 0x141e09388,
        0x141e04140, 0x141e07f28, 0x141e09da8, 0x141e0a200, 0x141e0a1f8,
        0x141e08190, 0x141e0b3a8, 0x141e0b948, 0x141e058b0]}
    result = {
        'schema': 'wave3-close-secondary-splitview-v1',
        'status': 'direct_decisions_and_order_bound; native_widget_state_and_notification_recipients_qualified',
        'method': 'Offline PE plus bounded DUMPBIN only; no original runtime, application control, shared Ghidra or production changes.',
        'binary_sha256': sha(pe.data),
        'caller': 'At the end of d0e540, main virtual+0x360 = cc54a0 is invoked when the main-window pointer exists and the first integer is0. This remains before signalRemoveGraphItems and project teardown.',
        'ordered_behavior': [
            'cc54a0 calls main virtual+0x7b8 = ca6210, which reads the guarded pointer at main+0x148 through c1aef0. A null result returns directly.',
            'For a nonnull manager, call d34cd0(manager,1,QString::fromLatin1("SplitView_Main")). d34bd0 obtains the manager layout through d34d50, calls QGridLayout::itemAtPosition(0,0), then item virtual+0x60 without an intervening null check; d39490 uses QMetaObject::cast with the application metaobject at1446b8440. The exact runtime metaobject class name was not extracted from its uninitialized-data address.',
            'd34d50 is lazy: absent manager+0x28 calls d34da0, then stores the result through cbc920 / QMetaObject::changeGuard. d34da0 first runs d34920, allocates0x20 bytes, calls QGridLayout constructor, installs original class tables, sets all content margins0, and QWidget::setLayout(manager,layout). The d34920 cleanup deletes a preexisting layout via virtual+0x18; the absent pointer branch skips it.',
            'For the requested SplitView_Main name, d34bd0 returns the successfully cast layout widget. d34cd0 then calls dabcb0 with index1; negative or index>=QSplitter::count returns null, otherwise QSplitter::widget(1). The SplitView_Sub branch is not selected here.',
            'cc54a0 returns on null child or QWidget::isVisible(child)==false. A visible child is cast using QGraphicsView::staticMetaObject, and null cast or null QGraphicsView::scene returns. The preliminary138b2a0 ->100e20 helper returns0 without object mutation.',
            'Once a nonnull QGraphicsScene is obtained, QGraphicsScene::clearSelection is called BEFORE checking that it dynamically casts to SfScene. A failed SfScene cast returns after that selection change.',
            'For an SfScene, cd5490(scene,"SfGraphHistogramItem") is tried first. Only if null is cd5490(scene,"SfChartPtrkItem") tried. cd5490 enumerates QGraphicsScene::items, skips entries with nonnull parentItem, and tests the corresponding RTTI dynamic cast. It returns the first matching top-level item or null.',
            'If either lookup succeeds, invoke notifier2cf3c0 on global143d76700 with the literal2signalCloseSecondaryWidgetInSplitView() and ten empty QGenericArguments. If both return null, return without that signal. The signal is not unconditional.'
        ],
        'integration_inputs': [
            'Actual main+0x148 manager availability.',
            'Existing layout and its(0,0) item/type, or the lazy layout allocation prefix if absent.',
            'Splitter child1 availability and QWidget visibility.',
            'QGraphicsView and SfScene cast outcomes, scene selection and top-level histogram/particle-track item presence.',
            'Signal recipients for signalCloseSecondaryWidgetInSplitView().'
        ],
        'qualifications': [
            'Do not infer null manager, hidden child, empty scene, or missing chart from a fresh-project label. These are state predicates requiring startup/lifecycle evidence or explicit observations.',
            'A missing secondary child is not proof of no earlier effects: manager layout resolution can allocate/cache/install a layout.',
            'The unguarded layout-item dereference is retained; this receipt does not introduce a safe fallback or claim a runtime crash was observed.',
            'Qt layout construction, selection-change notification recipients and the final application signal recipients are not transitively expanded here. Raw widget/rendering parity is not claimed.',
            'The old c1aef0 capture ended immediately before RET; close-tail-layout-lazy includes its exact RET at140c1aefd. Adjacent padding/next function in the explicit100e20 capture is not part of that leaf claim.'
        ],
        'literals': literals,
        'imports': used_imports,
        'evidence': [{'path': name, 'sha256': sha((ROOT / name).read_bytes())} for name in files],
        'tool_sha256': sha(Path(__file__).read_bytes()),
    }
    out = ROOT / 'analysis/wave3-close-secondary-splitview-contract.json'
    assert not out.exists()
    out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'path': out.relative_to(ROOT).as_posix(), 'sha256': sha(out.read_bytes()), 'linked_files_verified': len(files)}))


if __name__ == '__main__':
    main()
