"""Offline retained-PE table extraction; never loads or executes the DLL."""
from pathlib import Path
import hashlib
import json
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "evidence/original-binaries/QtCore4_64.dll"
image = TARGET.read_bytes()
sha = lambda data: hashlib.sha256(data).hexdigest()
u16 = lambda offset: struct.unpack_from("<H", image, offset)[0]
u32 = lambda offset: struct.unpack_from("<I", image, offset)[0]
pe = u32(0x3C)
optional = pe + 24
sections = []
for index in range(u16(pe + 6)):
    position = optional + u16(pe + 20) + index * 40
    sections.append((u32(position + 12), u32(position + 8),
                     u32(position + 20), u32(position + 16)))


def raw(rva):
    for address, virtual_size, offset, raw_size in sections:
        if address <= rva < address + max(virtual_size, raw_size):
            result = offset + rva - address
            if result >= offset + raw_size:
                raise ValueError("RVA lacks retained file bytes")
            return result
    raise ValueError(hex(rva))


def word(rva):
    return u16(raw(rva))


def properties(code_unit):
    first = word(0x1F4050 + (code_unit >> 5) * 2)
    record_index = word(0x1F4050 + (first + (code_unit & 31)) * 2)
    record_rva = 0x1FF7B0 + 20 * record_index
    packed = word(record_rva + 4)
    shifted = (packed << 8) & 0xFFFF
    if shifted & 0x8000:
        shifted -= 0x10000
    return image[raw(record_rva)], shifted >> 10, record_index


digits = []
maximum_record = 0
for code in range(0x10000):
    category, value, record = properties(code)
    maximum_record = max(maximum_record, record)
    if category == 4:
        digits.append((code, value))

ranges = []
for code, value in digits:
    if not ranges or code != ranges[-1][1] + 1:
        ranges.append([code, code])
    else:
        ranges[-1][1] = code
for start, end in ranges:
    if end - start != 9 or [properties(c)[1] for c in range(start, end + 1)] != list(range(10)):
        raise ValueError("Digit block is not a ten-value zero-through-nine sequence")

regions = []
for name, start, end in [
    ("QChar_isDigit", 0x69E70, 0x69EAA),
    ("QChar_digitValue", 0x69F00, 0x69F3F),
    ("Qt_BMP_property_index_envelope", 0x1F4050, 0x1FF7B0),
    ("Qt_BMP_property_record_envelope", 0x1FF7B0, 0x1FF7B0 + 20 * (maximum_record + 1)),
    ("codec_registration_and_endian", 0x14C840, 0x14CB17),
    ("UTF16_decode", 0x14F1C0, 0x14F389),
    ("UTF32_decode", 0x14F570, 0x14F804),
]:
    payload = image[raw(start):raw(end - 1) + 1]
    if len(payload) != end - start:
        raise ValueError("Region is not file-contiguous")
    regions.append(dict(name=name, start_rva=hex(start), end_rva_exclusive=hex(end),
                        file_offset=hex(raw(start)), bytes=len(payload), sha256=sha(payload)))

fixtures = []
for name, text in [("unicode_zero_010", "\u0660\u0661\u0660\r\n"),
                   ("mixed_decimal_12", "\u0661\uff12\r\n"),
                   ("two_flags", "\u0661 -\u0968\r\n")]:
    for encoding, bom in [("utf-16-le", b"\xff\xfe"), ("utf-16-be", b"\xfe\xff"),
                          ("utf-32-le", b"\xff\xfe\x00\x00"), ("utf-32-be", b"\x00\x00\xfe\xff")]:
        payload = bom + text.encode(encoding)
        fixtures.append(dict(name=name, encoding=encoding, hex=payload.hex(),
                             sha256=sha(payload)))

capture_validation = []
for capture in sorted((ROOT / "analysis").glob("resume162-spring-codec*.asm")):
    records = 0
    prefixes = 0
    for line in capture.read_text(encoding="utf-8").splitlines():
        match = re.match(r"\s*([0-9A-F]{16}):\s+((?:[0-9A-F]{2}[ \t]+)+)", line)
        if match:
            address = int(match.group(1), 16)
            payload = bytes.fromhex(match.group(2))
            position = raw(address - 0x67000000)
            if image[position:position + len(payload)] != payload:
                raise ValueError(f"Capture byte mismatch: {capture.name} at {address:x}")
            records += 1
            prefixes += len(payload)
    if not records:
        raise ValueError(f"No address-prefixed instruction records: {capture.name}")
    capture_validation.append(dict(file=capture.name, sha256=sha(capture.read_bytes()),
                                   address_prefixed_instruction_records=records,
                                   emitted_prefix_bytes_verified=prefixes, mismatches=0))

receipt = dict(
    target=str(TARGET), target_sha256=sha(image), method="Offline PE bytes and retained QChar instruction emulation; no DLL execution",
    qchar_capture="analysis/resume63-qchar-number-class.asm",
    nd_count=len(digits), non_ascii_nd_count=sum(c > 127 for c, _ in digits),
    ranges=[[f"U+{start:04X}", f"U+{end:04X}"] for start, end in ranges],
    all_blocks_values_zero_through_nine=True,
    digit_membership_sha256=sha(b"".join(struct.pack("<HB", c, v) for c, v in digits)),
    raw_regions=regions,
    capture_validation=capture_validation,
    codec_vtables=[dict(name=name, vtable_va=hex(address),
                        slots=[hex(struct.unpack_from("<Q", image, raw(address - 0x67000000) + i * 8)[0])
                               for i in range(6)],
                        sha256=sha(image[raw(address - 0x67000000):raw(address - 0x67000000) + 48]))
                   for name, address in [("UTF16BE", 0x67245A90), ("UTF16LE", 0x67245AC8),
                                         ("UTF32BE", 0x67245B38), ("UTF32LE", 0x67245B70)]],
    exclusion_samples=[dict(code_unit=f"U+{c:04X}", category=properties(c)[0], digit_value=properties(c)[1])
                       for c in [0x00B2, 0x2160, 0x2212, 0xD835, 0xDFCE]],
    fixture_byte_identities=fixtures,
    qualification="Property results are original-PE deductions. Fixture byte construction is standard encoding; original codec bodies are statically captured separately, with no original runtime execution.",
)
destination = ROOT / "analysis/resume162-spring-unicode-digit-evidence.json"
destination.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps(dict(receipt=str(destination), sha256=sha(destination.read_bytes()),
                      nd_count=len(digits), block_count=len(ranges), raw_regions=regions), indent=2))
