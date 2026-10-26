# usage: annot.py in.c out.c  -- replaces &UNK_x / _UNK_x with literal comments (strings or doubles) from the exe copy
import sys,struct,re
p='C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'
d=open(p,'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; opt=pe+24
nsec=struct.unpack_from('<H',d,pe+6)[0]; sh=opt+struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(nsec):
    o=sh+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8); secs.append((va,vs,ra,rs))
def off(v):
    r=v-0x140000000
    for va,vs,ra,rs in secs:
        if va<=r<va+rs: return r-va+ra
def lit(v,isdbl):
    o=off(v)
    if o is None: return None
    if isdbl:
        return 'D(%r)'%struct.unpack_from('<d',d,o)[0]
    e=d.find(b'\0',o)
    s=d[o:e]
    if 0<len(s)<200 and all(32<=c<127 or c in (9,10,13) for c in s):
        return 'S'+repr(s.decode('latin1')).replace("'",'"',0)
    if len(s)==0: return 'S""'
    return 'D(%r)'%struct.unpack_from('<d',d,o)[0]
t=open(sys.argv[1]).read()
def rep(m):
    amp,us,a=m.group(1),m.group(2),m.group(3)
    v=int(a,16); l=lit(v,us=='_' and not amp)
    return m.group(0)+('/*%s*/'%l if l else '')
t=re.sub(r'(&?)(_?)UNK_(1[0-9a-f]{8})',rep,t)
open(sys.argv[2],'w').write(t)
# second pass: raw 0x14xxxxxxx constants
t=open(sys.argv[2]).read()
def rep2(m):
    v=int(m.group(1),16); o=off(v)
    if o is None: return m.group(0)
    e=d.find(b'\0',o); s=d[o:e]
    if 0<len(s)<200 and all(32<=c<127 or c in (9,10,13) for c in s):
        return m.group(0)+'/*S%r*/'%s.decode('latin1')
    return m.group(0)
t=re.sub(r'\b0x(14[0-9a-f]{7})\b(?!\s*\))',rep2,t)
t=re.sub(r'\b0x(14[0-9a-f]{7})\b(?=,|\))',rep2,t)
open(sys.argv[2],'w').write(t)
