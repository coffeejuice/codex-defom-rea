"""Freeze the bounded HeatTreat property writer contract; no original execution."""
import hashlib
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
target = root / 'analysis/wave3-ht-native-contract.json'
assert not target.exists()
evidence = [
    'analysis/wave3-wizard-save-static.json',
    'analysis/wave3-wizard-save-1411b3b20.asm',
    'analysis/wave3-wizard-save-1411b3c40.asm',
    'analysis/wave3-ht-path-local8.asm',
    'analysis/wave3-cogging-native-io-contract.json',
    'evidence/gui_mo/s1/mdt/L6/s1.c',
]
result = {
    'method': 'Bounded shipped PE instruction and retained decompilation review; no original execution',
    'evidence': [{'path': p, 'sha256': hashlib.sha256((root / p).read_bytes()).hexdigest()} for p in evidence],
    'contracts': [
        {'address': '1411b3b20', 'behavior': 'Owner checks and task-directory lookup precede fillFromModel(mode1), which precedes HTWZ open. Failed open retains those model/schedule mutations.'},
        {'address': '1411b3d87..1411b3dbd', 'behavior': 'HeatTreatWizard.HTWZ path uses QString::toLocal8Bit then fopen wt; failed open returns0 and skips printf body. Successful open emits the existing schedule fragments, ignores printf/fclose results and returns1.'},
        {'address': '6706e4b0..6706e50b', 'behavior': 'QString::toLocal8Bit uses codecForLocale/fromUnicode when configured; null codec falls back to Latin1. Retained capture includes following function bytes which are not attributed to this method.'},
        {'address': '1411b3c40', 'behavior': 'Media names use local8Bit; userKey explicitly uses Latin1. Preserve separate encoding decisions and first-NUL printf string termination.'},
    ],
    'implementation': [
        'Native property HeatTreat calls fillFromModel before opening byte-path CRT output',
        'Shared CoggingFile accepts explicit path bytes; original Cogging constructor still uses toAscii',
        'Ctx codepage supplies the explicit locale-codec profile for HeatTreat path and media names',
        'Native property effects are immediate and omitted from deferred replay; output-alias adapter check remains before mutation',
    ],
    'focused_cases': [
        'Failed open preserves 2D preopen schedule fill and prior file',
        'No-delete-sharing permits direct write; UTF8 locale media versus Latin1 userKey; closed handle',
        'Byte-range-locked output retains ignored flush failure without replay',
        'Public output-alias rejection preserves project and existing HTWZ bytes',
    ],
    'qualifications': [
        'No fresh original injected-failure capture; source-derived contract tests are not independent acceptance',
        'Earlier Save-current HeatTreat MOKEY/HTWZ remains recording/deferred and queued separately',
        'Full process locale-codec discovery and null-codec fallback profile remain separate dependencies',
        'CRT buffer allocation fallback, descriptor exhaustion and full static-CRT partial-write internals retain Cogging contract qualifications',
        'Existing schedule arithmetic, legacy printf formatting and table payload reconstruction are reused, not newly accepted here',
    ],
    'next': 'Validate combined candidate, then remove earlier Save-current deferred effects and integrate evidence-qualified Boolean event delivery',
}
target.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'receipt': str(target), 'sha256': hashlib.sha256(target.read_bytes()).hexdigest()}))
