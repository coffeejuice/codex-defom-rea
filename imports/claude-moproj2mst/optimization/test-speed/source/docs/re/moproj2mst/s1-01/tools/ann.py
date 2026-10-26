# usage: ann.py exe in.c [out.c] -> replaces &UNK_/&DAT_/PTR_ to printable C strings with the string as comment
import sys,struct,re
p=sys.argv[1]; d=open(p,'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; opt=pe+24
nsec=struct.unpack_from('<H',d,pe+6)[0]; sh=opt+struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(nsec):
    o=sh+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8); secs.append((va,vs,ra,rs))
def rd(va,n):
    r=va-0x140000000
    for sva,vs,ra,rs in secs:
        if sva<=r<sva+rs: return d[ra+r-sva:ra+r-sva+n]
    return None
def s(va):
    b=rd(va,400)
    if not b: return None
    e=b.find(b'\0')
    if e<=0: return None
    t=b[:e]
    if all(32<=c<127 or c in (9,10,13) for c in t): return t.decode()
    return None
src=open(sys.argv[2],encoding='utf-8',errors='replace').read()
def rep(m):
    va=int(m.group(2),16); t=s(va)
    return m.group(0)+('/*"%s"*/'%t.replace('\n','\n').replace('\r','\r').replace('*/','*_/') if t is not None else '')
out=re.sub(r'(&?(?:UNK|DAT|s)_)(14[0-9a-f]{7})',rep,src)
open(sys.argv[3] if len(sys.argv)>3 else sys.argv[2],'w',encoding='utf-8').write(out)
