"""Bounded offline selections for Close ownership and directory-dialog recovery."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import DUMPBIN, imports


def sha(data):
    return hashlib.sha256(data).hexdigest()


def exports(pe):
    o = pe.off(pe.base + pe.u32(pe.optional + 112))
    funcs = pe.off(pe.base + pe.u32(o + 28))
    names = pe.off(pe.base + pe.u32(o + 32))
    ords = pe.off(pe.base + pe.u32(o + 36))
    result = {}
    for i in range(pe.u32(o + 24)):
        n = pe.off(pe.base + pe.u32(names + 4*i))
        s = pe.data[n:pe.data.find(b'\0', n)].decode()
        result[s] = pe.base + pe.u32(funcs + 4*pe.u16(ords + 2*i))
    return result


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--binary', choices=['DEF_GUI_MO_64.exe', 'QtGui4_64.dll', 'QtCore4_64.dll'], required=True)
    p.add_argument('--label', required=True)
    p.add_argument('addresses', nargs='+')
    a = p.parse_args()
    assert a.label.replace('-', '').isalnum()
    target = ROOT / 'evidence/original-binaries' / a.binary
    pe = PE(target)
    caps = []
    for text in a.addresses:
        parts = text.split(':')
        entry = int(parts[0], 0)
        bounds = pe.bounds(entry)
        if len(parts) == 2:
            end = int(parts[1], 0)
            bound_qualification = 'Explicit bounded interval; whole logical procedure not assumed.'
        else:
            assert bounds and int(bounds['begin'], 0) == entry
            end = int(bounds['end_exclusive'], 0)
            bound_qualification = bounds['qualification']
        cmd = [str(DUMPBIN), '/disasm:bytes', f'/range:0x{entry:x},0x{end-1:x}', str(target)]
        raw = subprocess.run(cmd, capture_output=True, check=True).stdout
        path = ROOT / f'analysis/wave3-closure-next-{entry:x}.asm'
        if path.exists():
            assert path.read_bytes() == raw
        else:
            path.write_bytes(raw)
        caps.append({'entry': hex(entry), 'end_exclusive': hex(end),
                     'bounds_qualification': bound_qualification,
                     'code_sha256': sha(pe.data[pe.off(entry):pe.off(end)]),
                     'artifact': path.relative_to(ROOT).as_posix(), 'artifact_sha256': sha(raw),
                     'command': cmd})
    result = {'method': 'Offline direct PE and DUMPBIN only; no original runtime, DLL loading, GUI, shared database or builds.',
              'binary': target.relative_to(ROOT).as_posix(), 'binary_sha256': sha(pe.data),
              'captures': caps, 'imports': {hex(k): v for k, v in imports(pe).items()},
              'exports': {k: hex(v) for k, v in exports(pe).items()} if a.binary.endswith('.dll') else {},
              'tool_sha256': sha(Path(__file__).read_bytes())}
    out = ROOT / f'analysis/wave3-closure-next-{a.label}-static.json'
    assert not out.exists()
    out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'receipt': out.relative_to(ROOT).as_posix(), 'sha256': sha(out.read_bytes()),
                      'captures': len(caps)}))


if __name__ == '__main__':
    main()
