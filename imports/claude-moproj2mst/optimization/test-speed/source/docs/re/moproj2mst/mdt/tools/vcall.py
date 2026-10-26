# usage: python -I vcall.py <slot hex> ; functions containing call qword [reg+slot] (FF 90..97 disp32)
import bisect, struct, sys, re
EXE='C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'; B=0x140000000
d=open(EXE,'rb').read(); pe=struct.unpack_from('<I',d,0x3c)[0]
ns=struct.unpack_from('<H',d,pe+6)[0]; sh=pe+24+struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(ns):
    o=sh+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8); secs.append((d[o:o+8].rstrip(b'\0').decode(),va,vs,ra,rs))
t=secs[0]; tx=d[t[3]:t[3]+t[4]]; tva=t[1]
pd=[s for s in secs if s[0]=='.pdata'][0]; fs=[]; fe=[]
for k in range(0,pd[4],12):
    a,b,_=struct.unpack_from('<III',d,pd[3]+k)
    if a==0: break
    fs.append(a); fe.append(b)
def fn(r):
    k=bisect.bisect_right(fs,r)-1
    return fs[k] if k>=0 and r<fe[k] else None
slot=int(sys.argv[1],16)
pat=re.compile(rb'\xff[\x90-\x93\x95-\x97]'+re.escape(struct.pack('<I',slot)))
out={}
for m in pat.finditer(tx):
    f=fn(tva+m.start()); out.setdefault(f,[]).append(tva+m.start()+B)
for f in sorted(out,key=lambda x:x or 0): print('%x'%(B+(f or 0)), ' '.join('%x'%a for a in out[f]))
