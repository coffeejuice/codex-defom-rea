"""Bind CP465 Type3 OPT count and region-section literals to the original PE."""

from pathlib import Path
import hashlib
import json
import struct

ROOT = Path(__file__).resolve().parents[1]
PE_PATH = ROOT / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
PE_DATA = PE_PATH.read_bytes()
PE_SHA256 = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
assert hashlib.sha256(PE_DATA).hexdigest() == PE_SHA256

pe_offset = struct.unpack_from('<I', PE_DATA, 0x3c)[0]
optional = pe_offset + 24
image_base = struct.unpack_from('<Q', PE_DATA, optional + 24)[0]
section_table = optional + struct.unpack_from('<H', PE_DATA, pe_offset + 20)[0]
sections = [
    struct.unpack_from('<IIII', PE_DATA, section_table + 40 * i + 8)
    for i in range(struct.unpack_from('<H', PE_DATA, pe_offset + 6)[0])
]


def mapped_bytes(address: int, length: int) -> tuple[int, bytes]:
    rva = address - image_base
    for virtual_size, virtual_address, raw_size, raw_address in sections:
        if virtual_address <= rva < virtual_address + max(virtual_size, raw_size):
            file_offset = raw_address + rva - virtual_address
            value = PE_DATA[file_offset:file_offset + length]
            assert len(value) == length
            return file_offset, value
    raise ValueError(f'address is not in a PE section: {address:#x}')


# Each byte sequence includes its terminating NUL. Overlapping 0x70/0x74
# literals are distinct insertion operands, as shown by separate call sites.
EXPECTED = [
    (0x1427ba028, b'DOE_PAR    \t\t\x00', 'f0b3ca54302b70ca6222919b8fe7b274abad0423f547dbad55873772f2dc5b82', 'manager_count_label'),
    (0x1427ba038, b'   0.0  1.0  0.5  0.01   0\x00', '676f4c16871c9b73623082d7e6a8745ccd60bca037a6881d32ecc7347237a21a', 'manager_number_iteration_literal'),
    (0x1427ba058, b'REGIONS    \t\t\x00', 'ad5b1e3d6dd2cc8e53e8af8ce3ece6ab3420ea12144144e00043badddbbd4141', 'regions_count_label'),
    (0x1427ba078, b'REGIONS\t\t0\x00', '0ab48ff7fb54071df1b9e00c55cd169229c49fb39c6b738dbd4688a4061ba6a4', 'regions_nonpositive_count_literal'),
    (0x1427ba074, b'Opt\x00', '2ba757c7120c9517ace17328b12a518aafd0c276688451105bdc52800f7f4ef0', 'region_row_suffix_opt'),
    (0x1427ba070, b'/\x00', 'ade0274f84b828b0980112609bb51775d0a4e39bf4728f21dcdd04a627abb92a', 'region_row_separator'),
    (0x1427ba068, b'.GEO\x00', '736692cb080c6a3f73a5f3e33d9954663430c777db220effa67666f735b5e342', 'region_row_suffix_geo'),
]

records = []
for address, expected, expected_sha256, role in EXPECTED:
    file_offset, actual = mapped_bytes(address, len(expected))
    assert actual == expected, (hex(address), actual, expected)
    actual_sha256 = hashlib.sha256(actual).hexdigest()
    assert actual_sha256 == expected_sha256
    records.append({
        'address': hex(address),
        'file_offset': hex(file_offset),
        'bytes': len(actual),
        'literal_ascii': actual[:-1].decode('ascii'),
        'includes_nul': True,
        'sha256': actual_sha256,
        'role': role,
    })

disassembly_path = ROOT / 'analysis/cp465-type3-opt-save-140907a10.asm'
disassembly = disassembly_path.read_text(encoding='utf-8')
disassembly_sha256 = hashlib.sha256(disassembly_path.read_bytes()).hexdigest()
assert disassembly_sha256 == 'c188feb9a5cf880507f6b1fa7b4e78f833e547cc6b8f6540ad613f4b19e9e866'

# The saved disassembly contains the literal references and surrounding
# operations. These checks bind source-order claims to the exact saved export;
# they do not infer Qt's physical output bytes.
ordered_groups = [
    ('manager count then 1..N sequence', [
        '0000000140908111: E8 CA C9 6F FF     call        0000000140004AE0',
        '000000014090811A: 48 8D 15 07 1F EB  lea         rdx,[00000001427BA028h]',
        '0000000140908133: FF 15 AF BC 4F 01  call        qword ptr [0000000141E03DE8h]',
        '000000014090815D: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]',
        '0000000140908176: 48 8D 15 BB 1E EB  lea         rdx,[00000001427BA038h]',
    ]),
    ('region count and row suffix operands', [
        '00000001409081BB: FF 90 C0 00 00 00  call        qword ptr [rax+00000000000000C0h]',
        '00000001409081D0: 48 8D 15 81 1E EB  lea         rdx,[00000001427BA058h]',
        '000000014090829A: 48 8D 15 C7 1D EB  lea         rdx,[00000001427BA068h]',
        '00000001409082C7: 48 8D 15 A2 1D EB  lea         rdx,[00000001427BA070h]',
        '00000001409082F4: 48 8D 15 79 1D EB  lea         rdx,[00000001427BA074h]',
        '0000000140908468: 48 8D 15 09 1C EB  lea         rdx,[00000001427BA078h]',
    ]),
]
for group_name, needles in ordered_groups:
    positions = [disassembly.find(needle) for needle in needles]
    assert all(position >= 0 for position in positions), group_name
    assert positions == sorted(positions), group_name

receipt = {
    'checkpoint': 'CP473 Type3 OPT count and region literals',
    'binary': str(PE_PATH.relative_to(ROOT)),
    'binary_sha256': PE_SHA256,
    'writer_disassembly': str(disassembly_path.relative_to(ROOT)),
    'writer_disassembly_sha256': disassembly_sha256,
    'method': 'Read fixed NUL-terminated bytes by PE section mapping and compare against expected bytes and SHA-256; separately assert ordered call/reference anchors in the complete CP465 bounded writer export.',
    'literals': records,
    'ordered_groups': [
        {
            'name': name,
            'evidence': 'CP465 saved disassembly; byte binding recorded in cp465-type3-opt-save-byte-verification.json',
            'qualification': qualification,
        }
        for name, qualification in [
            ('manager count then 1..N sequence', 'The call to helper 0x140004ae0 precedes the count literal; the returned int is inserted, and the loop starts at 1, compares against that count, inserts the sequence literal, and increments.'),
            ('region count and row suffix operands', 'The +0xc0 count is inserted after the positive-count branch; row building calls +0xd8 and references .GEO, /, Opt in order; the nonpositive arm references its own fixed literal.'),
        ]
    ],
    'physical_file_bytes_established': False,
    'original_executed': False,
    'limits': [
        'This binds static source literals and call/reference order only, not QTextStream encoding, integer formatting, buffering, flush, QFile text translation, write errors, or physical file bytes.',
        'The row suffix concatenation helpers are visible as call sites; their complete implementations and resulting runtime strings are not established by this receipt.',
        'No original process, GUI, build, test, or mutable REA session was used.',
    ],
}
output_path = ROOT / 'analysis/cp473-opt-section-byte-verification.json'
output_path.write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'literals': len(records), 'total_bytes': sum(x['bytes'] for x in records),
                  'mismatches': 0, 'receipt': str(output_path.relative_to(ROOT))}))
