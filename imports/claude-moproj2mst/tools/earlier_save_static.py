"""Retain bounded original earlier-Save file gates without executing the target."""
import json
import subprocess
from boolean_lifecycle_static import DUMPBIN, sha
from doe_static_pe import PE, ROOT

out = ROOT / 'analysis/wave3-earlier-save-static.json'
assert not out.exists()
binary = ROOT / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
pe = PE(binary)
captures = []
for entry in [0x140051190, 0x14008c630, 0x1411b73c0, 0x1411b9080]:
    bounds = pe.bounds(entry)
    assert int(bounds['begin'], 0) == entry
    end = int(bounds['end_exclusive'], 0)
    command = [str(DUMPBIN), '/disasm:bytes', f'/range:{hex(entry)},{hex(end-1)}', str(binary)]
    raw = subprocess.run(command, capture_output=True, check=True).stdout
    path = ROOT / f'analysis/wave3-earlier-save-{entry:x}.asm'
    assert not path.exists()
    path.write_bytes(raw)
    captures.append(dict(entry=hex(entry), end_exclusive=hex(end), qualification=bounds['qualification'],
        code_sha256=sha(pe.data[pe.off(entry):pe.off(end)]), artifact=str(path.relative_to(ROOT)),
        artifact_sha256=sha(raw), command=command))
literals = {}
for va in [0x142ca22bc, 0x142ca281c, 0x142ca298c]:
    offset = pe.off(va)
    literals[hex(va)] = pe.data[offset:offset+24].split(b'\0')[0].decode('ascii')
result = dict(binary_sha256=sha(pe.data), captures=captures, mode_literals=literals,
    method='Offline bounded PE/DUMPBIN; instruction review pending; unwind bounds do not guarantee logical function completeness')
out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
print(json.dumps(dict(receipt=str(out), captures=len(captures), mode_literals=literals)))
