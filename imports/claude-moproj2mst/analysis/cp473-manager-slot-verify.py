from pathlib import Path
import hashlib
import json
import re
import struct
import subprocess


ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"
ASM = ROOT / "analysis/cp473-manager-slot-14091bcb0.asm"
RECEIPT = ROOT / "analysis/cp473-manager-slot-verification.json"
DUMPBIN = Path(
    r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe"
)

VTABLE = 0x1421671B8
SLOT_OFFSET = 0x1C0
METHOD = 0x14091BCB0
EXPECTED_END = 0x14091BD9D

image = BINARY.read_bytes()
pe = struct.unpack_from("<I", image, 0x3C)[0]
section_count = struct.unpack_from("<H", image, pe + 6)[0]
optional_size = struct.unpack_from("<H", image, pe + 20)[0]
optional = pe + 24
image_base = struct.unpack_from("<Q", image, optional + 24)[0]
sections = []
for i in range(section_count):
    virtual_size, virtual_address, raw_size, raw_pointer = struct.unpack_from(
        "<IIII", image, optional + optional_size + 40 * i + 8
    )
    sections.append((virtual_address, max(virtual_size, raw_size), raw_pointer))


def file_offset(va: int) -> int:
    rva = va - image_base
    for start, size, raw_pointer in sections:
        if start <= rva < start + size:
            return raw_pointer + rva - start
    raise ValueError(f"VA is not in a PE section: {va:#x}")


def pe_bytes(va: int, count: int) -> bytes:
    return image[file_offset(va) : file_offset(va) + count]


slot_va = VTABLE + SLOT_OFFSET
slot_raw = pe_bytes(slot_va, 8)
target = struct.unpack("<Q", slot_raw)[0]
assert target == METHOD, (hex(target), hex(METHOD))

# Locate the complete method by its PE x64 exception-directory runtime-function entry.
exception_rva, exception_size = struct.unpack_from("<II", image, optional + 112 + 24)
exception_offset = file_offset(image_base + exception_rva)
runtime_functions = [
    struct.unpack_from("<III", image, exception_offset + i)
    for i in range(0, exception_size, 12)
]
function = next(
    entry
    for entry in runtime_functions
    if entry[0] <= METHOD - image_base < entry[1]
)
begin, end = image_base + function[0], image_base + function[1]
assert begin == METHOD and end == EXPECTED_END, (hex(begin), hex(end))

dump_text = subprocess.check_output(
    [str(DUMPBIN), "/disasm", f"/range:{begin:#x},{end - 1:#x}", str(BINARY)],
    text=True,
)
ASM.write_text(dump_text, encoding="utf-8")

chunks = []
current = None
for line in dump_text.splitlines():
    match = re.match(r"^  ([0-9A-F]{16}): ((?:[0-9A-F]{2} )*[0-9A-F]{2})(?:\s{2,}|$)", line)
    if match:
        current = [int(match[1], 16), bytearray.fromhex(match[2])]
        chunks.append(current)
    elif current and re.fullmatch(r"\s{10,}(?:[0-9A-F]{2} ?)+", line):
        current[1].extend(bytes.fromhex(line))

assert chunks and chunks[0][0] == begin
assert chunks[-1][0] + len(chunks[-1][1]) == end
for va, encoded in chunks:
    assert pe_bytes(va, len(encoded)) == encoded, hex(va)
for (va, encoded), (next_va, _) in zip(chunks, chunks[1:]):
    assert va + len(encoded) == next_va

# Pin the ordered state mutation/filter sequence and the value-return copy.
flat = re.sub(r"\s+", " ", dump_text)
ordered_fragments = [
    "call 0000000140259AD0",  # copy manager+38 list into temporary source list
    "call 0000000140004AE0",  # source-list size
    "call 0000000140160C10",  # source-list at(index)
    "mov dword ptr [rcx+18h],eax",  # reindex every source member
    "call 00000001401CC6B0",  # read variable link (+40)
    "test rax,rax",
    "jne 000000014091BD61",  # nonnull link skips append
    "call 000000014172D7E0",  # append unlinked variable to result list
    "call 0000000140259AD0",  # assign temporary result into hidden return storage
]
positions = []
cursor = 0
for fragment in ordered_fragments:
    position = flat.find(fragment, cursor)
    assert position >= 0, fragment
    positions.append(position)
    cursor = position + len(fragment)
assert flat.count("mov dword ptr [rcx+18h],eax") == 1

receipt = {
    "checkpoint": "CP473 DOE manager vtable +0x1c0 byte binding",
    "binary": str(BINARY.relative_to(ROOT)).replace("\\", "/"),
    "binary_sha256": hashlib.sha256(image).hexdigest(),
    "method": "PE exception-directory bounds; bounded dumpbin; compare each addressed instruction byte against PE sections",
    "tool": {
        "path": str(DUMPBIN),
        "reported_version": "14.44.35228.0",
        "mode": "/DISASM /RANGE",
    },
    "vtable": {
        "class": "SfMoDoeManager",
        "base_va": hex(VTABLE),
        "base_plus_offset": hex(slot_va),
        "offset": hex(SLOT_OFFSET),
        "file_offset": hex(file_offset(slot_va)),
        "qword_bytes": slot_raw.hex(" "),
        "target_va": hex(target),
        "qualification": "Constructor 0x1400d1180 installs vtable 0x1421671b8; normal manager branch only.",
    },
    "method_export": {
        "path": str(ASM.relative_to(ROOT)).replace("\\", "/"),
        "start": hex(begin),
        "end_exclusive": hex(end),
        "bytes": end - begin,
        "instructions": len(chunks),
        "raw_sha256": hashlib.sha256(pe_bytes(begin, end - begin)).hexdigest(),
        "text_sha256": hashlib.sha256(ASM.read_bytes()).hexdigest(),
        "mismatches": 0,
    },
    "visible_semantics": {
        "source_list": "QList copy of manager+0x38",
        "source_iteration": "in increasing QList index order from zero to the copied list size",
        "mutation_before_filter": "For each source member, store its loop index in variable+0x18 before reading its link field.",
        "predicate": "Variable+0x40 link getter returns null; only then append the variable pointer to result QList.",
        "membership": "No manager+0x38 erase, append, or reorder is visible in this body.",
        "return": "Copy-assign temporary result QList to caller-provided hidden return storage in RDX, then destroy local lists.",
        "type3_opt_consumer": "Writer uses returned QList size and emits count/number sequence; it does not retrieve individual returned elements.",
    },
    "ordered_instruction_anchors": [
        {"va": "0x14091bcd4", "effect": "copy manager+0x38 list"},
        {"va": "0x14091bd03", "effect": "read source-list size"},
        {"va": "0x14091bd17", "effect": "get current source variable"},
        {"va": "0x14091bd28", "effect": "write variable+0x18 index"},
        {"va": "0x14091bd3c", "effect": "read link pointer from variable+0x40"},
        {"va": "0x14091bd44", "effect": "skip append if link is nonnull"},
        {"va": "0x14091bd5c", "effect": "append unlinked variable to result"},
        {"va": "0x14091bd68", "effect": "copy result QList to hidden return storage"},
    ],
    "limits": [
        "Static PE evidence only; no original application or runtime was executed.",
        "Qt QList helper internals, implicit-sharing refcount transitions, detach/allocation failure behavior, and destruction internals are not independently recovered here.",
        "The method's visible reindex side effect occurs before its null-link filter; downstream behavior after helper failures is not established.",
        "Vtable interpretation is qualified to the normal SfMoDoeManager dynamic type, not arbitrary manager implementations.",
    ],
}
RECEIPT.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(
    json.dumps(
        {
            "receipt": str(RECEIPT),
            "vtable_slot_va": hex(slot_va),
            "qword": slot_raw.hex(" "),
            "target": hex(target),
            "method_range": [hex(begin), hex(end)],
            "method_bytes": end - begin,
            "instructions": len(chunks),
            "raw_sha256": receipt["method_export"]["raw_sha256"],
            "mismatches": 0,
        },
        indent=2,
    )
)
