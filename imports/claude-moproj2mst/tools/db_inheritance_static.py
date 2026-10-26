"""Inspect inherited-object DB helpers offline; never execute the target."""
from pathlib import Path
import hashlib,json,struct,subprocess

ROOT=Path(__file__).resolve().parents[1]
TARGET=ROOT/'evidence/original-binaries/DEF_GUI_MO_64.exe'
DUMPBIN=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')

def main():
    d=TARGET.read_bytes();digest=hashlib.sha256(d).hexdigest()
    assert digest=='fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
    pe=struct.unpack_from('<I',d,60)[0];opt=pe+24;base=struct.unpack_from('<Q',d,opt+24)[0]
    st=opt+struct.unpack_from('<H',d,pe+20)[0]
    sections=[struct.unpack_from('<IIII',d,st+i*40+8) for i in range(struct.unpack_from('<H',d,pe+6)[0])]
    def off(va):
        for vs,rv,size,raw in sections:
            if rv<=va-base<rv+min(vs,size):return raw+va-base-rv
        raise ValueError(hex(va))
    rv,size=struct.unpack_from('<II',d,opt+112+24);p=off(base+rv)
    ranges={base+s:base+e for s,e,_ in [struct.unpack_from('<III',d,i) for i in range(p,p+size,12)]}
    functions=[]
    for entry in [0x14005cc00,0x14005c890,0x14005b2f0,0x14080eb30,0x14080ea20]:
        end=ranges[entry]
        out=ROOT/'analysis'/f'wave2-db-inheritance-{entry:x}-disassembly.txt'
        result=subprocess.run([str(DUMPBIN),'/DISASM:BYTES',f'/RANGE:{entry:#x},{end-1:#x}',str(TARGET)],capture_output=True,check=True)
        out.write_bytes(result.stdout)
        functions.append({'entry':hex(entry),'end_exclusive':hex(end),'code_sha256':hashlib.sha256(d[off(entry):off(end-1)+1]).hexdigest(),'disassembly':out.relative_to(ROOT).as_posix()})
    receipt={'kind':'offline_static_disassembly','target_sha256':digest,'functions':functions,
             'qualification':'Exact pdata-bounded copied original function bytes; DUMPBIN only. No original process/function execution. Decompilation frame artifacts require these byte checks.'}
    (ROOT/'analysis/wave2-db-inheritance-static-evidence.json').write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({'receipt':'analysis/wave2-db-inheritance-static-evidence.json','functions':len(functions),'target_sha256':digest}))

if __name__=='__main__':main()
