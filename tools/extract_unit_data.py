"""Decode exact unit presentation data from retained REA read_bytes evidence."""
import argparse
import hashlib
import json
from pathlib import Path
import struct

TARGET = 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'


def extract(path):
    raw = path.read_bytes()
    evidence = json.loads(raw)
    records = [r for r in evidence['records'] if r['operation'] == 'read_bytes']
    bindings = []
    regions = {}
    for r in records:
        n = r['normalized_result']
        assert r['subject']['digest']['sha256'] == TARGET
        data = bytes.fromhex(n['bytes_hex'])
        assert n['complete'] and len(data) == n['requested_bytes'] == n['returned_bytes']
        regions[int(n['address'], 16)] = data
        bindings.append(r['evidence_id'])

    def read(address, size):
        for start, data in regions.items():
            if start <= address and address + size <= start + len(data):
                return data[address-start:address-start+size]
        raise AssertionError(f'No complete retained read at {address:#x}+{size}')

    def text(data):
        assert b'\0' in data
        return data.split(b'\0', 1)[0].decode('ascii')

    addresses = [0x1406212e4, 0x14062139c, *range(0x140625aa0, 0x140625ac0, 4)]
    formats = {hex(a): text(read(a, 4)) for a in addresses}
    assert set(formats.values()) == {'%s'}
    a, b = [], []
    for i in range(28):
        r = read(0x1406214a0 + 136*i, 136)
        a.append(dict(id=struct.unpack_from('<i', r)[0], text=text(r[4:104]),
                      raw64=[r[j:j+8].hex() for j in [104,112,120,128]]))
        r = read(0x140622380 + 504*i, 504)
        b.append(dict(id=struct.unpack_from('<i', r)[0],
                      text=[text(r[j:j+100]) for j in [4,104,204,304,404]]))
    return dict(target_sha256=TARGET, method='Complete retained REA read_bytes decoded at exact source offsets; every formatting literal is %s.',
                source=dict(path=path.resolve().relative_to(Path(__file__).resolve().parents[1]).as_posix(), sha256=hashlib.sha256(raw).hexdigest()),
                rea_byte_evidence_ids=bindings, formats=formats, table_a=a, table_b=b)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--evidence', type=Path, default=Path('analysis/unit-static-evidence.json'))
    p.add_argument('--output', type=Path, required=True)
    args = p.parse_args()
    data = extract(args.evidence)
    args.output.write_text(json.dumps(data, indent=2)+'\n', encoding='utf-8', newline='\n')
    print(json.dumps({'status':'DECODED_SOURCE_UNIT_DATA','rows_per_table':len(data['table_a']), 'format_count':len(data['formats'])}))


if __name__ == '__main__': main()
