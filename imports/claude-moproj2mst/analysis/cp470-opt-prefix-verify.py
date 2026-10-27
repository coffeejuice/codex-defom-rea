"""Bind fixed OPT writer insertion literals to the retained original PE."""

from pathlib import Path
import hashlib
import json
import struct

ROOT = Path(__file__).resolve().parents[1]
PATH = ROOT / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
DATA = PATH.read_bytes()
SHA = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
assert hashlib.sha256(DATA).hexdigest() == SHA
pe = struct.unpack_from('<I', DATA, 0x3c)[0]
optional = pe + 24
base = struct.unpack_from('<Q', DATA, optional + 24)[0]
first_section = optional + struct.unpack_from('<H', DATA, pe + 20)[0]
sections = [struct.unpack_from('<IIII', DATA, first_section + 40 * i + 8)
            for i in range(struct.unpack_from('<H', DATA, pe + 6)[0])]


def mapped(address, length):
    rva = address - base
    for virtual_size, virtual_address, raw_size, raw_address in sections:
        if virtual_address <= rva < virtual_address + max(virtual_size, raw_size):
            offset = raw_address + rva - virtual_address
            value = DATA[offset:offset + length]
            assert len(value) == length
            return offset, value
    raise ValueError(hex(address))


EXPECTED = [
    (0x1427b9f70, b'DEF_OPT       \t1.0\x00', 'dc24431719fdb5ace145e028880c6513e91542ede256aa2d7d81d59efa2b69fc'),
    (0x1427b9f88, b'TYP_OPT       \t1\x00', 'b26a91d23e8c34f4c780547ac2f2ce90702c1a693e66bc28607f0a3bc3b55f73'),
    (0x1427b9fa0, b'GRD_TYP  \t\t10\x00', '2cb125b6cb8d0313daec29a0399ff4b4dd4727a947a4fdc3d75688a4e5ec6190'),
    (0x1427b9fb0, b'KRG_MAX       \t\x00', '5d19f5356be7ca773735a50455c032054b1db9e48ca5b380b5c7c9727bd64026'),
    (0x1427b9fc0, b'OPR_NO        \t\x00', '38c08a02fa0475ddd1be8a1766935764e6a0974d021e2a27f188de28f908bb48'),
]
records = []
for address, expected, expected_sha in EXPECTED:
    offset, actual = mapped(address, len(expected))
    assert actual == expected
    assert hashlib.sha256(actual).hexdigest() == expected_sha
    records.append(dict(address=hex(address), file_offset=hex(offset),
                        bytes=len(actual), literal=actual[:-1].decode('ascii'),
                        includes_nul=True, sha256=expected_sha))

source = ROOT / 'analysis/cp465-type3-opt-save-140907a10.asm'
receipt = dict(checkpoint='CP470 fixed Type3 OPT insertion literals',
               binary=str(PATH.relative_to(ROOT)), binary_sha256=SHA,
               writer_disassembly=str(source.relative_to(ROOT)),
               writer_disassembly_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
               method='Read literal bytes including NUL through original PE section mapping; writer call order is separately qualified by CP465 bounded disassembly',
               literals=records, physical_file_bytes_established=False,
               original_executed=False)
output = ROOT / 'analysis/cp470-opt-prefix-byte-verification.json'
output.write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps(dict(literals=len(records), total_bytes=sum(x['bytes'] for x in records),
                      mismatches=0)))
