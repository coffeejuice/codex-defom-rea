"""Freeze source-bound diagram fit effects that precede callback model resolution."""
import hashlib
import json
import struct
from pathlib import Path
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import imports


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    pe = PE()
    evidence = ['analysis/wave3-boolean-timer-contract.json',
                'analysis/wave3-normal-close-diagram-lifetime-contract.json']
    for label in ['diagram-fit-prefix', 'diagram-fit-zoom']:
        path = f'analysis/wave3-closure-next-{label}-static.json'
        receipt = json.loads((ROOT / path).read_text())
        assert receipt['binary_sha256'] == sha(pe.data)
        for c in receipt['captures']:
            begin, end = int(c['entry'], 0), int(c['end_exclusive'], 0)
            assert sha(pe.data[pe.off(begin):pe.off(end)]) == c['code_sha256']
            assert sha((ROOT / c['artifact']).read_bytes()) == c['artifact_sha256']
        evidence.append(path)
    assert pe.u64(pe.off(0x1421cded8 + 0x388)) == 0x140174120
    base = pe.off(0x1421de840)
    method_offset = pe.u32(base + 20)
    signals = []
    for index in range(2):
        va = 0x1421def20 + pe.u32(base + 4*(method_offset + 5*index))
        at = pe.off(va)
        signals.append({'local_id': index, 'address': hex(va), 'signature': pe.data[at:pe.data.index(0, at)].decode()})
    assert [s['signature'] for s in signals] == ['zoomLevelChanged(int)', 'zoomScaleChanged(double)']
    constants = {hex(address): struct.unpack('<d', pe.data[pe.off(address):pe.off(address)+8])[0]
                 for address in [0x1421ce520, 0x1421d94d8, 0x1421d3d18, 0x1421d9538,
                                 0x1421249e0, 0x142142078]}
    im = imports(pe)
    import_addresses = [0x141e07c40, 0x141e055f8, 0x141e05780, 0x141e07c38,
                        0x141e07690, 0x141e07c30, 0x141e07c28, 0x141e07c20,
                        0x141e07c18, 0x141e04668, 0x141e0b568, 0x141e06e10, 0x141e07dd8]
    source = 'source/apps/moproj2mst/src/moproj/write.cpp'
    result = {
        'schema': 'wave3-diagram-modelupdated-fit-prefix-v1',
        'status': 'direct_original_fit_effects_bound; live_view_state_and_signal_recipients_pending',
        'method': 'Offline PE and bounded DUMPBIN; no original/candidate runtime, GUI, build, shared Ghidra or production changes.',
        'binary_sha256': sha(pe.data),
        'callback_boundary': '183160 modelUpdated invokes diagram virtual+0x388 =174120 BEFORE reading its current attached model/item. A null-bound receiver therefore still performs this prefix; it skips the later click only after prefix returns.',
        'ordered_effects': [
            'Read scene from view.private(+0x28)+0x08; QGraphicsScene::itemsBoundingRect into a QRectF. If QRectF::isValid is false, replace with(0,0,double(private+0x128),double(private+0x12c)). These signed integer fallback dimensions are state, not assumed constants.',
            'QRectF::adjust(-40,-40,20,20), then QGraphicsView::fitInView(rect, aspect mode1/KeepAspectRatio). The resulting transform depends on actual viewport, scrollbar/layout and prior transform state.',
            'Call QGraphicsScene::invalidate(sceneRect, SceneLayers0xffff), then QGraphicsScene::update(default empty QRectF). These occur even for a null-bound diagram model.',
            'Read QGraphicsView::matrix().m11() into scale. Call180fb0(private,scale), store its integer result into private+0x54.',
            'Emit local signal0 zoomLevelChanged(int) through18d2c0 / QMetaObject::activate using stored private+0x54; then emit local signal1 zoomScaleChanged(double) through18d310 with scale. Both activate the same metaobject143d821c0, in that order.',
            'Only when all prefix calls and signal callbacks return does183160 resolve the then-current model/item and run or skip selection. Signal callbacks are therefore before the model lookup, not after it.'
        ],
        'zoom_level_calculation': {
            'entry': '140180fb0',
            'finite_normal_domain': [
                'Read width and height of the full sceneRect, not the adjusted item rectangle.',
                'If width>height, extent=width and viewportExtent=viewport.width(); otherwise extent=height and viewportExtent=viewport.height().',
                'minimumScale = double(viewportExtent) * 0.949999988079071 / extent.',
                'If scale<1, level = truncateTowardZero((scale-1)*10/(1-minimumScale)); otherwise level = truncateTowardZero((scale-1)/0.30000001192092896).',
                'No clamp appears in this body. The constants are exact doubles promoted from their original float values; do not replace with decimal0.95 or0.3 when matching bit-sensitive arithmetic.'
            ],
            'exceptional_domain': 'Native SSE comisd branch behavior and cvttsd2si are retained in assembly. This finite formula does not claim NaN, infinite, zero-extent, singular-transform or overflow reconstruction.'
        },
        'adapter_implication': [
            'A delivered timer cannot be reduced to Boolean apply or an inert fit counter. The callback needs actual diagram-view state or an explicit qualified fit projection and signal receiver profile.',
            'moproj::viewCentre currently calculates a fixed fresh-Open serialization transform using Project.diagramViewHeight and imported geometry assumptions. It does not own or update arbitrary session transform, selection, invalidation, zoom state or callback delivery. Do not silently reuse it as a generic modelUpdated fit implementation.',
            'No inference that post-Close scene contents are empty, fallback dimensions are unchanged, or zoom recipients are absent is made here.'
        ],
        'signals': signals,
        'constants': constants,
        'imports': {hex(address): im[address] for address in import_addresses},
        'evidence': [{'path': path, 'sha256': sha((ROOT / path).read_bytes())} for path in evidence],
        'candidate_source_at_audit': {'path': source, 'sha256': sha((ROOT / source).read_bytes())},
        'tool_sha256': sha(Path(__file__).read_bytes()),
    }
    out = ROOT / 'analysis/wave3-diagram-modelupdated-fit-contract.json'
    assert not out.exists()
    out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'path': out.relative_to(ROOT).as_posix(), 'sha256': sha(out.read_bytes()), 'linked_files_verified': len(evidence)}))


if __name__ == '__main__':
    main()
