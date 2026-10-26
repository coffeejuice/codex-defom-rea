# usage: vt.py exe vtVA[,vtVA...] [slot,slot...] -> class name via RTTI COL and slot targets
import sys,struct
p=sys.argv[1]; d=open(p,'rb').read(); B=0x140000000
pe=struct.unpack_from('<I',d,0x3c)[0]; opt=pe+24
nsec=struct.unpack_from('<H',d,pe+6)[0]; sh=opt+struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(nsec):
    o=sh+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8); secs.append((va,vs,ra,rs))
def off(va):
    r=va-B
    for sva,vs,ra,rs in secs:
        if sva<=r<sva+rs: return ra+r-sva
def q(va): return struct.unpack_from('<Q',d,off(va))[0]
def dd(va): return struct.unpack_from('<I',d,off(va))[0]
def cls(vt):
    col=q(vt-8)
    try:
        td=B+dd(col+12); o=off(td+16); e=d.index(b'\0',o); return d[o:e].decode()
    except Exception as ex: return '?'
slots=[int(x,16) for x in sys.argv[3].split(',')] if len(sys.argv)>3 else []
for v in sys.argv[2].split(','):
    vt=int(v,16); print('%x %s'%(vt,cls(vt)), ' '.join('+%x=%x'%(s,q(vt+s)) for s in slots))
