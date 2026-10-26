# usage: python -I stores.py disp,disp,... [lo hi]; functions with movsd [reg+disp32],xmm stores for all given disps
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
disps=[int(x,16) for x in sys.argv[1].split(',')]
lo=int(sys.argv[2],16)-B if len(sys.argv)>2 else 0; hi=int(sys.argv[3],16)-B if len(sys.argv)>3 else 1<<40
hits={}
for dsp in disps:
    # F2 [REX] 0F 11 modrm(mod=10) [sib] disp32
    pat=re.compile(rb'\xf2[\x40-\x4f]?\x0f\x11[\x80-\xbf]\x24?'+re.escape(struct.pack('<I',dsp)), re.S)
    for m in pat.finditer(tx):
        r=tva+m.start()
        if not (lo<=r<hi): continue
        f=fn(r); hits.setdefault(f,set()).add(dsp)
for f,s in sorted(hits.items(), key=lambda x:x[0] or 0):
    if len(s)==len(disps): print('%x'%(B+(f or 0)))
