"""Retain shipped wizard open gates and QIODevice Text write ordering offline."""
import json
import subprocess
from boolean_lifecycle_static import DUMPBIN, imports, sha
from doe_static_pe import PE, ROOT


def main():
    output = ROOT / 'analysis/wave3-wizard-save-static.json'
    assert not output.exists()
    captures = []
    for name, entries in [
        ('DEF_GUI_MO_64.exe', [0x1402a16c0, 0x1402b9050, 0x1411b3b20, 0x1411b3c40]),
        ('QtCore4_64.dll', [0x670bb750]),
    ]:
        binary = ROOT / 'evidence/original-binaries' / name
        pe = PE(binary)
        for entry in entries:
            bounds = pe.bounds(entry)
            assert int(bounds['begin'], 0) == entry
            end = int(bounds['end_exclusive'], 0)
            command = [str(DUMPBIN), '/disasm:bytes', f'/range:{hex(entry)},{hex(end-1)}', str(binary)]
            raw = subprocess.run(command, capture_output=True, check=True).stdout
            path = ROOT / f'analysis/wave3-wizard-save-{entry:x}.asm'
            assert not path.exists()
            path.write_bytes(raw)
            captures.append(dict(binary=name, binary_sha256=sha(pe.data), entry=hex(entry),
                end_exclusive=hex(end), qualification=bounds['qualification'],
                code_sha256=sha(pe.data[pe.off(entry):pe.off(end)]),
                artifact=str(path.relative_to(ROOT)), artifact_sha256=sha(raw), command=command))
    result = dict(method='Offline PE and DUMPBIN only; no original execution or DLL loading',
                  captures=captures, contract_status='Pending bounded instruction review',
                  reproducer=str(__file__), reproducer_sha256=sha(open(__file__, 'rb').read()))
    output.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(receipt=str(output), captures=len(captures), sha256=sha(output.read_bytes()))))


if __name__ == '__main__':
    main()
