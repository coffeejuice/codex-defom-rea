"""CP64 bounded immutable import/export and notifier-key reads."""
from pathlib import Path
import struct, json, hashlib

root = Path(__file__).resolve().parents[1]
class PE:
    def __init__(self,name):
        self.path=root/'evidence/original-binaries'/name
        self.b=self.path.read_bytes()
        self.n=self.u32(60); self.o=self.n+24
        self.base=self.u64(self.o+24)
        s=self.o+self.u16(self.n+20)
        self.secs=[struct.unpack_from('<IIII',self.b,s+40*i+8) for i in range(self.u16(self.n+6))]
    def u16(self,o): return struct.unpack_from('<H',self.b,o)[0]
    def u32(self,o): return struct.unpack_from('<I',self.b,o)[0]
    def u64(self,o): return struct.unpack_from('<Q',self.b,o)[0]
    def off(self,va):
        r=va-self.base
        return next(raw+r-start for vs,start,size,raw in self.secs if start<=r<start+size)
    def string(self,va): return self.b[self.off(va):self.off(va)+256].split(b'\0',1)[0].decode('ascii')
    def directory(self,index): return struct.unpack_from('<II',self.b,self.o+112+index*8)
    def imports(self):
        r,z=self.directory(1); p=self.off(self.base+r); out={}
        while any(struct.unpack_from('<IIIII',self.b,p)):
            oft,stamp,fwd,name,first=struct.unpack_from('<IIIII',self.b,p)
            q=self.off(self.base+(oft or first)); i=0
            while (v:=self.u64(q+i*8)):
                out[self.base+first+i*8]=dict(dll=self.string(self.base+name),symbol=self.string(self.base+v+2) if not v>>63 else f'ordinal:{v&65535}')
                i+=1
            p+=20
        return out
    def exports(self):
        r,z=self.directory(0); p=self.off(self.base+r)
        nf,nn,fa,na,oa=struct.unpack_from('<IIIII',self.b,p+20)
        funcs=self.off(self.base+fa); names=self.off(self.base+na); ords=self.off(self.base+oa)
        return {self.string(self.base+self.u32(names+i*4)):self.base+self.u32(funcs+self.u16(ords+i*2)*4) for i in range(nn)}

app=PE('DEF_GUI_MO_64.exe'); qt=PE('QtCore4_64.dll'); imp=app.imports(); exp=qt.exports()
imports=[dict(iat=hex(a),**imp[a], export_va=hex(exp[imp[a]['symbol']])) for a in [0x141e05658,0x141e05348]]
addresses=[0x142497750,0x142497798,0x1424977b8,0x1429fd020,0x1429fd038]
result=dict(method='PE32+ file-backed reads of ordinary imports/exports and five literal spans; target not executed',
            binaries=[dict(path=str(p.path.relative_to(root)),sha256=hashlib.sha256(p.b).hexdigest()) for p in [app,qt]],
            imports=imports,literals=[dict(va=hex(a),text=app.string(a),hex=app.b[app.off(a):app.off(a)+len(app.string(a))+1].hex()) for a in addresses])
out=root/'analysis/resume64-boolean-event-route-pe.json'
assert not out.exists()
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
