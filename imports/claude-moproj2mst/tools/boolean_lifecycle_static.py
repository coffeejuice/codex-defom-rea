"""Bounded offline evidence for Boolean selection wiring and Save failure gates."""
import hashlib
import json
from pathlib import Path
import struct
import subprocess
from doe_static_pe import PE, ROOT

DUMPBIN = Path("C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def imports(pe):
    result = {}
    at = pe.off(pe.base + pe.u32(pe.optional + 120))
    while pe.u32(at + 12):
        lookup, _, _, name, iat = struct.unpack_from('<IIIII', pe.data, at)
        i = 0
        while (value := pe.u64(pe.off(pe.base + lookup) + i*8)):
            if value >> 63:
                symbol = f'ordinal:{value & 0xffff}'
            else:
                start = pe.off(pe.base + value) + 2
                symbol = pe.data[start:pe.data.find(b'\0', start)].decode()
            result[pe.base + iat + i*8] = symbol
            i += 1
        at += 20
    return result


def main():
    out = ROOT / 'analysis/wave3-boolean-lifecycle-failure-static.json'
    assert not out.exists()
    pe = PE()
    assert sha(pe.data) == 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
    gui = PE(ROOT / 'evidence/original-binaries/QtGui4_64.dll')
    captures = []
    for label, target, addresses in [
        ('original', pe, [0x140d39aa0, 0x140d3d590, 0x1401313c0, 0x1400a20d0,
                          0x1411e1820, 0x1411e1970, 0x1411e0e40]),
        ('qtgui', gui, [0x654dfa70])]:
        binary = ROOT / ('evidence/original-binaries/' + ('DEF_GUI_MO_64.exe' if label == 'original' else 'QtGui4_64.dll'))
        for entry in addresses:
            bounds = target.bounds(entry)
            assert int(bounds['begin'], 0) == entry
            end = int(bounds['end_exclusive'], 0)
            command = [str(DUMPBIN), '/disasm:bytes', f'/range:0x{entry:x},0x{end-1:x}', str(binary)]
            raw = subprocess.run(command, capture_output=True, check=True).stdout
            artifact = ROOT / f'analysis/wave3-boolean-lifecycle-{entry:x}.asm'
            assert not artifact.exists()
            artifact.write_bytes(raw)
            captures.append({'binary': str(binary.relative_to(ROOT)), 'binary_sha256': sha(target.data),
                             'entry': hex(entry), 'end_exclusive': hex(end),
                             'bounds_qualification': bounds['qualification'],
                             'code_sha256': sha(target.data[target.off(entry):target.off(end)]),
                             'artifact': str(artifact.relative_to(ROOT)), 'artifact_sha256': sha(raw), 'command': command})
    imp = imports(pe)
    table = 0x142a0b158
    slots = []
    for slot in [0x1a8, 0x220, 0x228]:
        entry = pe.u64(pe.off(table)+slot)
        offset = pe.off(entry)
        assert pe.data[offset:offset+2] == b'\xff\x25'
        iat = entry + 6 + struct.unpack_from('<i', pe.data, offset+2)[0]
        slots.append({'offset': hex(slot), 'entry': hex(entry), 'stub_bytes': pe.data[offset:offset+6].hex(),
                      'iat': hex(iat), 'symbol': imp[iat]})
    literals = {}
    for address in [0x142a0a740,0x142a0a768,0x142a0a788,0x142a0a7b0,0x142a0a7d0,0x142a0a7f8,
                    0x142a0ea18,0x142a0e9f0]:
        offset = pe.off(address)
        literals[hex(address)] = pe.data[offset:pe.data.find(b'\0',offset)].decode()
    evidence = [
        'evidence/gui_mo/s1/s1-04/d1c.c', 'evidence/gui_mo/s1/s1-04/d2c.c',
        'evidence/gui_mo/s1/s1-02/a1s.c', 'evidence/gui_mo/s1/mdt/save_d0.c',
        'evidence/gui_mo/s1/mdt/L3/save0.c',
        'analysis/wave2-doe-lazy-empty-finalizer-tree-decompile.json',
        'analysis/wave2-doe-ghidra-text-verification.json',
        'analysis/wave2-db133-open-lifecycle.json',
        'source/apps/moproj2mst/src/core/convert.cpp',
        'source/apps/moproj2mst/src/engine/boolean_editor.cpp',
        'source/apps/moproj2mst/include/m2m/engine.hpp']
    result = {'schema': 'wave3-boolean-lifecycle-failure-static-v1',
              'method': 'Offline PE reads and DUMPBIN only. No target execution, DLL loading, UI, builds, or shared REA.',
              'captures': captures, 'tree_vtable': hex(table), 'tree_imported_slots': slots,
              'constructor_literals': literals,
              'retained_private_export': {'entry': '140d3d590', 'qualification': 'Private Ghidra project .text identity is linked by wave2-doe-ghidra-text-verification.json; small constructor also independently disassembled here.'},
              'findings': [
                  {'id': 'BT01', 'status': 'unknown', 'contract': 'No currentChanged/selectionChanged connection is installed by inspected tree-view or tree-model constructors. Imported base virtual slots are not custom factory callbacks.', 'limit': 'Does not exclude externally installed callbacks or prove all Qt transitive behavior. First Boolean editor creation/order remains unclosed.'},
                  {'id': 'BF01', 'status': 'proven_mismatch', 'priority': 0, 'original': 'MST destination is opened before generation at 0x1401313c0; failed open skips generation and its Boolean writers.', 'candidate': 'core/convert.cpp generates MST and applies side files before final writeAtomically(outPath,mst).'},
                  {'id': 'BF02', 'status': 'proven_mismatch', 'priority': 1, 'original': '0x1411e1820 attempts BWZ then INI regardless BWZ result; returns BWZ result. 0x1411df6d0 ignores it and emits subsequent MST content.', 'candidate': 'applyEffects throws on first failed file and stops subsequent file writes and final MST.'},
                  {'id': 'BF03', 'status': 'proven_mismatch', 'priority': 1, 'original': 'Boolean files open directly at destination with mode2 and no mkdir. Successful-open path returns1 with no explicit stream-status check.', 'candidate': 'writeAtomically writes a sibling temporary and replaces destination; applyEffects creates parent directories and raises write errors.'},
                  {'id': 'BF04', 'status': 'proven_mismatch', 'priority': 1, 'original': 'Top Save writes .moproj then properties (including Cogging.MDT), then MST. MOPROJ failure does not skip later stages. MST failure does not skip permitted DOE post-MST calls in0x1400a20d0.', 'candidate': 'Physical Cogging.MDT files are applied after all generic step5 effects; shared applyEffects exceptions abort all later stages.'}],
              'qualifications': ['WriteOnly mode2 and direct-destination opens are proven; exact Windows Qt buffering/flush/partial-write bytes remain a separate dependency.', 'Original top-level GUI Save return is not automatically an m2m adapter status; preserve ABI and qualify status mapping.', 'No production edits are justified for early Open timing by this evidence.'],
              'files_sha256': {s: sha((ROOT/s).read_bytes()) for s in evidence},
              'reproducer': str(Path(__file__).relative_to(ROOT)), 'reproducer_sha256': sha(Path(__file__).read_bytes())}
    out.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'receipt': str(out), 'sha256': sha(out.read_bytes()), 'captures': len(captures), 'production_edits': False}))


if __name__ == '__main__':
    main()
