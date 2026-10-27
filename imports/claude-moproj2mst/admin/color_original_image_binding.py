"""CP285 DISABLED original-image predicates. No process I/O or launch capability.

Inputs are immutable file bytes, caller-supplied synthetic mapped samples and
caller-supplied identity receipts. A successful predicate grants no run readiness.
"""
import hashlib
import ntpath
import struct
from pathlib import Path

ORIGINAL_RUN_ENABLED = False
ORIGINAL = Path(__file__).resolve().parent.parent / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
SHA256 = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
FILE_SIZE = 67611136
BASE, IMAGE_SIZE, HEADERS, IMAGE_BASE_OFFSET = 0x140000000, 0x535f000, 0x400, 0x140
HEADER_SHA256 = '2cfb564ed35912acda737920e5abec88c24408ac73a3911f2d4af49674e68121'
CALL_RVA, ENTRY_RVA, RETURN_RVA = 0x11f2482, 0x11f26e0, 0x11f2487
CALL = bytes.fromhex('e859020000')
PROLOGUE = bytes.fromhex('48894c2408534881ecc0000000488b053cb996024833c448898424b0000000')
FUNCTION_END, UNWIND_RVA = 0x11f28de, 0x33ff8ac
UNWIND_HEADER = bytes.fromhex('191f0300')
U64_MAX = (1 << 64) - 1


class BindingError(ValueError):
    """Missing, malformed or mismatched supplied evidence; original stays HOLD."""


def require(condition, message):
    if not condition:
        raise BindingError(message)


def span(data, offset, size):
    require(isinstance(data, bytes), 'sample must be bytes')
    require(type(offset) is int and type(size) is int and size > 0 and
            0 <= offset < offset + size <= len(data), 'short read or invalid file bounds')
    return data[offset:offset + size]


def unpack(fmt, data, offset):
    return struct.unpack(fmt, span(data, offset, struct.calcsize(fmt)))


def address(base, rva, size=1):
    require(type(base) is int and type(rva) is int and type(size) is int,
            'address fields must be integers')
    require(base > 0 and base % 0x10000 == 0 and 0 < size and
            0 <= rva < rva + size <= IMAGE_SIZE and
            base + IMAGE_SIZE <= 0x800000000000 and base + IMAGE_SIZE <= U64_MAX,
            'invalid base, overflow or image bounds')
    return base + rva


def parse_pe(blob):
    """Bounded AMD64 PE parser, independently testable before identity pinning."""
    require(span(blob, 0, 2) == b'MZ', 'DOS signature')
    pe, = unpack('<I', blob, 0x3c)
    require(pe >= 64 and span(blob, pe, 4) == b'PE\0\0', 'PE signature')
    machine, count = unpack('<HH', blob, pe + 4)
    opt_size, = unpack('<H', blob, pe + 20)
    require(machine == 0x8664 and 0 < count <= 96 and opt_size == 240, 'AMD64 headers')
    opt = pe + 24
    require(unpack('<H', blob, opt)[0] == 0x20b, 'PE32+ magic')
    image_base, = unpack('<Q', blob, opt + 24)
    image_size, headers = unpack('<II', blob, opt + 56)
    require(0 < headers <= len(blob) and headers <= image_size <= 0xffffffff,
            'image/header size')
    require(unpack('<I', blob, opt + 108)[0] == 16, 'directory count')
    span(blob, opt, opt_size)
    sections = []
    require(opt + opt_size + count * 40 <= headers, 'section table beyond headers')
    for i in range(count):
        s = opt + opt_size + i * 40
        vs, va, rs, raw = unpack('<IIII', blob, s + 8)
        flags, = unpack('<I', blob, s + 36)
        require(vs > 0 and va >= headers and va + max(vs, rs) <= image_size,
                'section virtual bounds')
        if rs:
            require(raw >= headers and raw + rs <= len(blob), 'section raw bounds')
        sections.append(dict(name=span(blob, s, 8), va=va, vs=vs, raw=raw, size=rs, flags=flags))
    for i, a in enumerate(sections):
        for b in sections[i + 1:]:
            require(a['va'] + max(a['vs'], a['size']) <= b['va'] or
                    b['va'] + max(b['vs'], b['size']) <= a['va'], 'overlapping virtual sections')
            if a['size'] and b['size']:
                require(a['raw'] + a['size'] <= b['raw'] or
                        b['raw'] + b['size'] <= a['raw'], 'overlapping raw sections')
    dirs = [unpack('<II', blob, opt + 112 + 8 * i) for i in range(16)]
    return dict(base=image_base, image_size=image_size, headers=headers,
                image_base_offset=opt + 24, sections=sections, directories=dirs)


def raw_span(blob, pe, rva, size, executable=False):
    require(type(rva) is int and type(size) is int and 0 < size and
            0 <= rva < rva + size <= pe['image_size'], 'RVA bounds')
    matches = [s for s in pe['sections'] if s['va'] <= rva and
               rva + size <= s['va'] + s['size']]
    require(len(matches) == 1, 'span must be uniquely raw-backed')
    s = matches[0]
    require(not executable or s['flags'] & 0x20000000, 'span is not executable')
    return span(blob, s['raw'] + rva - s['va'], size)


def audit_relocations(blob, pe, protected):
    """Inspect every relocation, including writes beginning before a span.

    AMD64 ABSOLUTE padding and DIR64 are accepted. Unknown nonpadding types
    refuse rather than guessing write widths and accidentally proving absence.
    """
    rva, size = pe['directories'][5]
    require(rva > 0 and size > 0, 'relocation directory absent')
    data = raw_span(blob, pe, rva, size)
    cursor, entries = 0, 0
    while cursor < len(data):
        page, block = unpack('<II', data, cursor)
        require(page % 0x1000 == 0 and page < pe['image_size'] and
                block >= 8 and block % 2 == 0 and cursor + block <= len(data),
                'malformed relocation block')
        for offset in range(cursor + 8, cursor + block, 2):
            item, = unpack('<H', data, offset)
            kind, delta = item >> 12, item & 0xfff
            if kind == 0:
                continue
            require(kind == 10, 'unknown AMD64 relocation type')
            site = page + delta
            require(site + 8 <= pe['image_size'], 'relocation outside image')
            for start, length in protected:
                require(site + 8 <= start or start + length <= site,
                        'relocation overlaps protected bytes')
            entries += 1
        cursor += block
    return entries


def validate_file_binding(blob):
    require(type(blob) is bytes and len(blob) == FILE_SIZE, 'original size mismatch')
    require(hashlib.sha256(blob).hexdigest() == SHA256, 'original SHA-256 mismatch')
    pe = parse_pe(blob)
    require((pe['base'], pe['image_size'], pe['headers'], pe['image_base_offset']) ==
            (BASE, IMAGE_SIZE, HEADERS, IMAGE_BASE_OFFSET), 'pinned PE layout mismatch')
    require(hashlib.sha256(span(blob, 0, HEADERS)).hexdigest() == HEADER_SHA256, 'header hash mismatch')
    require(raw_span(blob, pe, CALL_RVA, 5, True) == CALL, 'CALL mismatch')
    require(CALL_RVA + 5 + struct.unpack('<i', CALL[1:])[0] == ENTRY_RVA and
            CALL_RVA + 5 == RETURN_RVA, 'CALL target/return mismatch')
    require(raw_span(blob, pe, ENTRY_RVA, len(PROLOGUE), True) == PROLOGUE, 'complete prologue mismatch')
    pdata_rva, pdata_size = pe['directories'][3]
    require(pdata_size > 0 and pdata_size % 12 == 0, 'malformed pdata')
    pdata = raw_span(blob, pe, pdata_rva, pdata_size)
    matches = [unpack('<III', pdata, i) for i in range(0, len(pdata), 12)
               if unpack('<III', pdata, i)[0] <= ENTRY_RVA < unpack('<III', pdata, i)[1]]
    require(matches == [(ENTRY_RVA, FUNCTION_END, UNWIND_RVA)], 'runtime-function binding')
    require(raw_span(blob, pe, UNWIND_RVA, 4) == UNWIND_HEADER, 'unwind header mismatch')
    count = audit_relocations(blob, pe, [(CALL_RVA, len(CALL)), (ENTRY_RVA, len(PROLOGUE))])
    return dict(status='DISABLED_STATIC_BINDING_ONLY', original_run_ready=False,
                sha256=SHA256, size=FILE_SIZE, relocation_entries_checked=count,
                full_prologue_bytes_checked=len(PROLOGUE))


def validate_mapped_sample(blob, *, base, header, call, prologue, runtime_function,
                           unwind_header, caller_return):
    """Validate supplied samples only; never reads a process or establishes origin."""
    validate_file_binding(blob)
    address(base, 0, IMAGE_SIZE)
    expected = bytearray(span(blob, 0, HEADERS))
    struct.pack_into('<Q', expected, IMAGE_BASE_OFFSET, base)
    require(type(header) is bytes and header == bytes(expected), 'mapped header mismatch/short read')
    require(type(call) is bytes and call == CALL, 'mapped CALL mismatch/short read')
    require(type(prologue) is bytes and prologue == PROLOGUE, 'mapped complete prologue mismatch/short read')
    expected_function = struct.pack('<III', ENTRY_RVA, FUNCTION_END, UNWIND_RVA)
    require(type(runtime_function) is bytes and runtime_function == expected_function, 'mapped pdata mismatch')
    require(type(unwind_header) is bytes and unwind_header == UNWIND_HEADER, 'mapped unwind mismatch')
    require(type(caller_return) is int and caller_return == address(base, RETURN_RVA, 1), 'sampled return mismatch')
    require(address(base, CALL_RVA, 5) + 5 + struct.unpack('<i', call[1:])[0] ==
            address(base, ENTRY_RVA, len(PROLOGUE)), 'mapped CALL target mismatch')
    return dict(status='SYNTHETIC_PREDICATE_ONLY', original_run_ready=False,
                base=base, entry=base + ENTRY_RVA, caller_return=caller_return)


def canonical_path(value):
    require(type(value) is str and value and '\x00' not in value, 'missing/invalid path')
    if value.startswith('\\\\?\\'):
        value = value[4:]
    drive, tail = ntpath.splitdrive(value)
    require(len(drive) == 2 and drive[1] == ':' and tail.startswith('\\') and
            '..' not in tail.split('\\') and '/' not in value, 'path must be absolute canonical Windows path')
    return ntpath.normcase(ntpath.normpath(value))


def validate_identity_receipt(receipt, contract):
    """Compare supplied evidence with a separately frozen installed-file contract.

    Does not acquire or authenticate either mapping. Caller provenance is a
    future prerequisite. No installed path is inferred from the copied evidence.
    """
    require(type(receipt) is dict and type(contract) is dict, 'identity mapping absent')
    keys = ('installed_path', 'size', 'sha256', 'volume_serial', 'file_index')
    require(all(k in contract for k in keys), 'frozen installed contract incomplete')
    expected_path = canonical_path(contract['installed_path'])
    require(expected_path != canonical_path(str(ORIGINAL).replace('/', '\\')), 'evidence copy is not an installed launch contract')
    require(type(contract['size']) is int and contract['size'] == FILE_SIZE and contract['sha256'] == SHA256,
            'contract does not identify pinned original')
    require(type(contract['volume_serial']) is int and 0 <= contract['volume_serial'] <= 0xffffffff and
            type(contract['file_index']) is str and contract['file_index'].isdigit() and
            str(int(contract['file_index'])) == contract['file_index'] and
            0 < int(contract['file_index']) <= U64_MAX, 'invalid contract file identity')
    required = ('creation_image', 'queried_process_path', 'pid', 'primary_tid', 'retained_pid', 'retained_primary_tid')
    require(all(k in receipt for k in required), 'creation identity receipt incomplete')
    image = receipt['creation_image']
    require(type(image) is dict and all(k in image for k in ('final_path', 'size', 'sha256', 'volume_serial', 'file_index')),
            'creation image receipt incomplete')
    require(canonical_path(image['final_path']) == expected_path == canonical_path(receipt['queried_process_path']), 'image path disagreement')
    for k in ('size', 'sha256', 'volume_serial', 'file_index'):
        require(type(image[k]) is type(contract[k]) and image[k] == contract[k], 'creation file identity disagreement: ' + k)
    for k in ('pid', 'primary_tid', 'retained_pid', 'retained_primary_tid'):
        require(type(receipt[k]) is int and 0 < receipt[k] <= 0xffffffff, 'invalid retained process/thread identity')
    require(receipt['pid'] == receipt['retained_pid'] and receipt['primary_tid'] == receipt['retained_primary_tid'], 'retained process/thread disagreement')
    return dict(status='SUPPLIED_IDENTITY_PREDICATE_ONLY', original_run_ready=False)
