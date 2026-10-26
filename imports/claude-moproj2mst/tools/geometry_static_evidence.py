"""Retain bounded offline geometry disassembly; never execute original code."""
from pathlib import Path
import hashlib
import json
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT/'evidence/original-binaries/DEF_GUI_MO_64.exe'
DUMPBIN = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe')

def main():
    data=TARGET.read_bytes()
    digest=hashlib.sha256(data).hexdigest()
    assert digest=='fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
    pe=struct.unpack_from('<I',data,60)[0]; opt=pe+24
    base=struct.unpack_from('<Q',data,opt+24)[0]
    st=opt+struct.unpack_from('<H',data,pe+20)[0]
    sections=[struct.unpack_from('<IIII',data,st+40*i+8) for i in range(struct.unpack_from('<H',data,pe+6)[0])]
    def offset(va):
        rv=va-base
        for vs,start,size,raw in sections:
            if start<=rv<start+min(vs,size):return raw+rv-start
        raise ValueError(hex(va))
    pr,ps=struct.unpack_from('<II',data,opt+112+3*8)
    pa=offset(base+pr)
    ranges={base+s:base+e for s,e,_ in [struct.unpack_from('<III',data,o) for o in range(pa,pa+ps,12)]}
    functions=[]
    for entry,fallback in [(0x140c8e0f0,None),(0x14126c540,None),(0x14126d0c0,None),
                           (0x14126cae0,None),(0x14121a310,32),(0x141470a78,16),
                           (0x141219af0,None),(0x1412510a0,None),(0x141250f70,None),
                           (0x141251140,64),(0x141251000,32),(0x14126c6a0,40),
                           (0x14121c200,None),(0x14121ca10,None),
                           (0x1400c3570,32),(0x1400c3590,32),(0x140514f00,None),
                           (0x140894330,None),(0x140856a60,32),(0x140859710,32)]:
        end=ranges.get(entry,entry+fallback if fallback else None)
        if end is None:raise ValueError('Missing exact function range '+hex(entry))
        p=ROOT/'analysis'/f'wave2-geometry-{entry:x}-disassembly.txt'
        result=subprocess.run([str(DUMPBIN),'/DISASM:BYTES',f'/RANGE:{entry:#x},{end-1:#x}',str(TARGET)],capture_output=True,check=True)
        p.write_bytes(result.stdout)
        functions.append({'entry':hex(entry),'end_exclusive':hex(end),'range_kind':'pdata exact' if entry in ranges else 'bounded leaf/thunk window; tail is not claimed function body',
                          'code_sha256':hashlib.sha256(data[offset(entry):offset(end-1)+1]).hexdigest(),
                          'disassembly':p.relative_to(ROOT).as_posix()})
    constants=[]
    for va in [0x142157420,0x142203f60]:
        off=offset(va)
        constants.append({'va':hex(va),'file_offset':off,'bytes_hex':data[off:off+8].hex(),'double':struct.unpack_from('<d',data,off)[0]})
    vtable=[{'slot':hex(i*8),'target':hex(struct.unpack_from('<Q',data,offset(0x142d36608)+i*8)[0])} for i in range(10)]
    assert vtable[9]['target']=='0x14126d0c0'
    # IAT identities are metadata, not a loaded-process address claim.
    imports=[]
    ir,_=struct.unpack_from('<II',data,opt+112+8)
    io=offset(base+ir)
    while True:
        oft,stamp,chain,name,ft=struct.unpack_from('<IIIII',data,io)
        if not any((oft,stamp,chain,name,ft)):break
        no=offset(base+name);dll=data[no:data.index(0,no)].decode('ascii')
        j=0
        while True:
            thunk=struct.unpack_from('<Q',data,offset(base+(oft or ft))+j*8)[0]
            if not thunk:break
            if not thunk>>63:
                at=offset(base+thunk)+2;sym=data[at:data.index(0,at)].decode('ascii')
                if sym in ['fabs','sqrt','_fabs','_sqrt'] or base+ft+j*8==0x141e0a758:
                    imports.append({'dll':dll,'symbol':sym,'iat_va':hex(base+ft+j*8)})
            j+=1
        io+=20
    qt=ROOT/'evidence/original-binaries/QtGui4_64.dll'; qdata=qt.read_bytes()
    qp=struct.unpack_from('<I',qdata,60)[0];qo=qp+24;qb=struct.unpack_from('<Q',qdata,qo+24)[0]
    qs=qo+struct.unpack_from('<H',qdata,qp+20)[0]
    qsecs=[struct.unpack_from('<IIII',qdata,qs+40*i+8) for i in range(struct.unpack_from('<H',qdata,qp+6)[0])]
    def qoff(rv):
        for vs,start,size,raw in qsecs:
            if start<=rv<start+min(vs,size):return raw+rv-start
        raise ValueError(hex(rv))
    er,_=struct.unpack_from('<II',qdata,qo+112)
    export=qoff(er)
    _,_,_,_,_,_,nf,nn,af,an,ao=struct.unpack_from('<IIHHIIIIIII',qdata,export)
    qentry=None
    for i in range(nn):
        nr=struct.unpack_from('<I',qdata,qoff(an)+4*i)[0];no=qoff(nr)
        sym=qdata[no:qdata.index(0,no)].decode('ascii')
        if sym=='?length@QVector3D@@QEBANXZ':
            ordinal=struct.unpack_from('<H',qdata,qoff(ao)+2*i)[0]
            qentry=struct.unpack_from('<I',qdata,qoff(af)+4*ordinal)[0];break
    assert qentry is not None
    qpr,qps=struct.unpack_from('<II',qdata,qo+112+3*8)
    qranges={s:e for s,e,_ in [struct.unpack_from('<III',qdata,o) for o in range(qoff(qpr),qoff(qpr)+qps,12)]}
    qend=qranges.get(qentry,qentry+160)
    qout=ROOT/'analysis/wave2-geometry-qt-vector-length-disassembly.txt'
    result=subprocess.run([str(DUMPBIN),'/DISASM:BYTES',f'/RANGE:{qb+qentry:#x},{qb+qend-1:#x}',str(qt)],capture_output=True,check=True)
    qout.write_bytes(result.stdout)
    qt_receipt={'path':qt.relative_to(ROOT).as_posix(),'sha256':hashlib.sha256(qdata).hexdigest(),
                'symbol':'?length@QVector3D@@QEBANXZ','entry':hex(qb+qentry),'end_exclusive':hex(qb+qend),
                'range_kind':'pdata exact' if qentry in qranges else 'bounded leaf window; tail not claimed',
                'disassembly':qout.relative_to(ROOT).as_posix()}
    receipt={'kind':'offline_static_geometry_evidence','target_sha256':digest,'functions':functions,
             'constants':constants,'item_vtable':{'va':'0x142d36608','slots':vtable},'math_imports':imports,
             'qt_vector_length':qt_receipt,
             'qualification':'DUMPBIN disassembly and raw PE metadata only. Original code not executed; retained decompilation still requires control/data-flow qualification.'}
    out=ROOT/'analysis/wave2-geometry-static-evidence.json';out.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({'receipt':str(out),'functions':len(functions),'constants':constants,'vt48':vtable[9],'math_imports':imports},indent=2))

if __name__=='__main__':main()
