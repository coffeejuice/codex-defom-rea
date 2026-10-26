"""Bounded immutable-PE wrapper and solid-brush dependency capture."""
import hashlib
import json
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]
TOOL = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')
EXPECTED = {'DEF_GUI_MO_64.exe':'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
    'QtGui4_64.dll':'c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887'}
captures = []
for name, addresses in [('DEF_GUI_MO_64.exe',[0x140377b80,0x140130d70]),('QtGui4_64.dll',[0x6511e6d0])]:
    binary = ROOT/'evidence/original-binaries'/name
    data = binary.read_bytes()
    assert hashlib.sha256(data).hexdigest() == EXPECTED[name]
    pe = struct.unpack_from('<I', data,0x3c)[0]
    opt = pe+24
    base = struct.unpack_from('<Q',data,opt+24)[0]
    table = opt+struct.unpack_from('<H',data,pe+20)[0]
    sections = [struct.unpack_from('<IIII',data,table+i*40+8) for i in range(struct.unpack_from('<H',data,pe+6)[0])]
    def offset(rva):
        for _, start, length, raw in sections:
            if start <= rva < start+length: return raw+rva-start
        raise ValueError(hex(rva))
    pdata, size = struct.unpack_from('<II',data,opt+112+3*8)
    unwind = [struct.unpack_from('<III',data,offset(pdata)+i) for i in range(0,size,12)]
    for address in addresses:
        matches = [(start,end) for start,end,_ in unwind if start <= address-base < end]
        assert len(matches) <= 1
        end = base+matches[0][1] if matches else address+0x60
        assert end-address < 4096
        target = ROOT/f'analysis/wave3-editor-wrapper-palette-{address:x}.asm'
        assert not target.exists()
        command = [str(TOOL),'/disasm:bytes',f'/range:{address:#x},{end-1:#x}',str(binary)]
        result = subprocess.run(command, capture_output=True, check=True)
        target.write_bytes(result.stdout)
        captures.append(dict(binary=name,binary_sha256=EXPECTED[name],entry=hex(address),end_exclusive=hex(end),
            boundary='unwind region' if matches else 'bounded leaf candidate; inspect RET and branches',
            path=str(target.relative_to(ROOT)),sha256=hashlib.sha256(result.stdout).hexdigest(),command=command))
    if name == 'DEF_GUI_MO_64.exe':
        literals = {}
        for address in [0x1421a8848,0x1421a8858]:
            start = offset(address-base)
            literals[hex(address)] = data[start:start+128].split(b'\0',1)[0].decode('ascii')
record = dict(captures=captures,literals=literals,tool_sha256=hashlib.sha256(TOOL.read_bytes()).hexdigest(),
    qualification='Read-only original binary evidence; no runtime execution or inferred callback completion.')
target = ROOT/'analysis/wave3-editor-wrapper-palette-static.json'
assert not target.exists()
target.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(captures=[x['path'] for x in captures],literals=literals)))
