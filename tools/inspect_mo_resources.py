"""Bounded read-only extraction of relevant embedded Qt Designer XML resources."""
import hashlib
import json
import re
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe')
OUT = ROOT/'build/gui-validation/mo-ui-resources'
LIMIT = 2 * 1024 * 1024
WANTED = re.compile(rb'SimRun|Convert2Dto3D|CopyMirror|SfOprPre|RunSimulation', re.I)


def main():
    data = SOURCE.read_bytes()
    OUT.mkdir(parents=True, exist_ok=True)
    records = []
    seen = set()
    candidates = [(m.start(), m.group(0), 'literal-xml') for m in re.finditer(rb'<\?xml[^\x00]*?</ui>', data, re.S)]
    for m in re.finditer(rb'\x78[\x01\x9c\xda]', data):
        pos = m.start()
        if pos < 4:
            continue
        size = struct.unpack_from('>I', data, pos-4)[0]
        if not 20 <= size <= LIMIT:
            continue
        try:
            stream = zlib.decompressobj()
            raw = stream.decompress(data[pos:pos+LIMIT], size+1)
        except zlib.error:
            continue
        if stream.eof and len(raw) == size and b'<ui' in raw[:200]:
            candidates.append((pos, raw, 'qt-length-prefixed-zlib'))
    for pos, raw, kind in candidates:
        if not WANTED.search(raw):
            continue
        digest = hashlib.sha256(raw).hexdigest()
        if digest in seen:
            continue
        seen.add(digest)
        dest = OUT/f'{pos:x}.ui'
        dest.write_bytes(raw)
        records.append(dict(file_offset=hex(pos), kind=kind, sha256=digest,
                            bytes=len(raw), output=str(dest.relative_to(ROOT))))
    report = dict(source=str(SOURCE), sha256=hashlib.sha256(data).hexdigest(),
                  qualification='Embedded XML and strings establish labels, not runtime reachability.',
                  scan_limit_bytes=LIMIT, selected=records)
    (OUT/'manifest.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
