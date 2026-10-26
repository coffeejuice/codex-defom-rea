"""Read bounded main argv literals from immutable PE and retained disassembly."""
import hashlib
import json
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[1]
binary = root / 'evidence/original-binaries/DEF_GUI_MO_64.exe'
assembly = root / 'analysis/wave3-editor-owned-open-entry-140001410.asm'
data = binary.read_bytes()
assert hashlib.sha256(data).hexdigest() == 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
pe = struct.unpack_from('<I', data, 0x3c)[0]
assert data[pe:pe+4] == b'PE\0\0'
count = struct.unpack_from('<H', data, pe+6)[0]
opt_size = struct.unpack_from('<H', data, pe+20)[0]
opt = pe+24
assert struct.unpack_from('<H', data, opt)[0] == 0x20b
base = struct.unpack_from('<Q', data, opt+24)[0]
sections = []
for i in range(count):
    pos = opt+opt_size+i*40
    virtual_size, rva, raw_size, raw = struct.unpack_from('<IIII', data, pos+8)
    sections.append((rva, raw_size, raw))

def read_ascii(va):
    address = va-base
    for rva, length, raw in sections:
        if rva <= address < rva+length:
            offset = raw+address-rva
            chunk = data[offset:offset+min(160, length-(address-rva))]
            if b'\0' not in chunk:
                return None
            value = chunk.split(b'\0', 1)[0]
            if value and all(32 <= b < 127 for b in value):
                return value.decode('ascii')
    return None

rows = []
for line in assembly.read_text().splitlines():
    match = re.search(r'^\s*([0-9A-F]{16}):.*lea\s+(rdx|r8),\[([0-9A-F]+)h\]', line)
    if not match:
        continue
    instruction, register, address = int(match[1],16), match[2], int(match[3],16)
    if not 0x140001b56 <= instruction < 0x140001fb6:
        continue
    value = read_ascii(address)
    if value is not None:
        rows.append(dict(instruction=hex(instruction), register=register, literal_va=hex(address), ascii=value))
result = dict(binary=str(binary.relative_to(root)), binary_sha256=hashlib.sha256(data).hexdigest(),
    assembly=str(assembly.relative_to(root)), assembly_sha256=hashlib.sha256(assembly.read_bytes()).hexdigest(),
    range=['0x140001b56','0x140001fb6'], literals=rows,
    qualification='Read-only literal/call-site inventory; literals alone do not prove dispatched behavior. No target execution.')
target = root / 'analysis/wave3-native-cli-option-literals.json'
assert not target.exists()
target.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps(rows))
