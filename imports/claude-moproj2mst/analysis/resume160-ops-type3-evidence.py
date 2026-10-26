"""CP160 immutable FRCROT/Type3 byte qualifications; no target execution."""
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from doe_static_pe import PE

p = PE()
def span(start, end):
    raw = p.data[p.off(start):p.off(end - 1) + 1]
    return dict(start=hex(start), end_exclusive=hex(end),
                file_start=hex(p.off(start)), bytes=len(raw),
                sha256=hashlib.sha256(raw).hexdigest())

ranges = {
    'FRCROT_case': (0x140b67701, 0x140b67843),
    'group3_common_tail': (0x140b68e6f, 0x140b68f82),
    'table_reader': (0x140af73f0, 0x140af77fa),
    'table_reset_preserving_metadata': (0x14089b9e0, 0x14089ba35),
    'table_initializer_code_before_embedded_tables': (0x140899010, 0x140899f35),
    'initializer_jump_targets': (0x140899f38, 0x14089a0ac),
    'initializer_case22_selector': (0x14089a0bb, 0x14089a0bc),
    'table_release_buffers': (0x14089b1f0, 0x14089b371),
    'table_allocate_and_clear': (0x14089b840, 0x14089b95f),
    'table_clamp_leaf_through_RET': (0x14089b970, 0x14089b9d3),
    'table_count': (0x14089bc90, 0x14089bd5a),
    'small_byte_fill_path': (0x141471850, 0x1414718bc),
    'mirror_identity_preservation': (0x1408e5d60, 0x1408e5ddd),
    'mirror_delegated_copy': (0x1408e57e0, 0x1408e5bde),
    'formatted_scan_wrapper': (0x141472ac8, 0x141472afd),
    'SUB_destructor_revalidation_only': (0x1400b58d0, 0x1400b59de),
    'SUB_deleting_wrapper_revalidation_only': (0x1400ac330, 0x1400ac366),
    'optimizer_deleting_wrapper': (0x140918fc0, 0x140918ff6),
    'optimizer_vtable_first45': (0x1427bceb8, 0x1427bd020),
}
selector = p.data[p.off(0x14089a0ac) + (0x16 - 7)]
target = p.base + p.u32(p.off(0x140899f38) + selector * 4)
formats = []
for a in (0x1427e89dc, 0x1427e89e4, 0x1427e89ec, 0x1427e89f4, 0x1427e89fc):
    o = p.off(a)
    end = p.data.index(0, o) + 1
    formats.append({**span(a, a + end-o), 'ascii':p.data[o:end-1].decode('ascii')})
captures = []
for path in sorted((ROOT/'analysis').glob('resume160-*.asm')):
    if any(x in path.name for x in ('frcrot-', 'type3-')):
        captures.append(dict(artifact=str(path.relative_to(ROOT)), bytes=path.stat().st_size,
                             sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
out = dict(binary='evidence/original-binaries/DEF_GUI_MO_64.exe',
           binary_sha256=hashlib.sha256(p.data).hexdigest(),
           ranges={name:span(*bounds) for name,bounds in ranges.items()},
           template22_dispatch=dict(selector=selector, target=hex(target),
                                    note='default/common tail, no template-specific stores'),
           formats=formats,
           vtable=[dict(slot=hex(i*8),target=hex(p.u64(p.off(0x1427bceb8)+i*8))) for i in range(45)],
           captures=captures,
           qualification='Immutable PE/.pdata and bounded dumpbin bytes only. No runtime, GUI, live REA, database or source/build change. Initial reset capture completed two bodies then stopped on missing leaf unwind; both retained. The 16-byte fill probe is incomplete and supports no behavioral claim. The 160-byte fill capture ends partially, but the selected <=56-byte fill path returns at 0x1414718bb without reaching its larger-fill branch. Initializer/group3 unwind spans include embedded jump data after their RET; those data are not executable instructions. SUB captures duplicate CP81/142 solely to reject stale missing-destructor claims; no new owner closure is claimed.')
path = ROOT/'analysis/resume160-ops-type3-evidence.json'
path.write_text(json.dumps(out, indent=2)+'\n')
print(json.dumps(dict(receipt=str(path.relative_to(ROOT)), range_count=len(ranges),
                     template22=out['template22_dispatch'], captures=len(captures))))
