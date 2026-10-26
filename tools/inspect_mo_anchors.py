"""Read-only PE string anchors and candidate RTTI owners; no behavior inferred."""
import re, struct, json, sys
from pathlib import Path
p=Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe')
d=p.read_bytes(); pe=struct.unpack_from('<I',d,0x3c)[0]; sh=pe+24+struct.unpack_from('<H',d,pe+20)[0]
sections=[]
for i in range(struct.unpack_from('<H',d,pe+6)[0]):
 o=sh+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8);sections.append((va,rs,ra))
def off(va):
 r=va-0x140000000
 return next((ra+r-s for s,n,ra in sections if s<=r<s+n),None)
def string(va):
 o=off(va)
 if o is None:return None
 b=d[o:o+500].split(b'\0')[0]
 return b.decode() if len(b)>=4 and all(32<=x<127 or x in (9,10,13) for x in b) else None
code=Path(sys.argv[1] if len(sys.argv)>1 else 'build/gui-validation/mo-launch-focused.c').read_text()
refs=set(re.findall(r'0x(14[0-9a-f]{7})',code)+re.findall(r'UNK_(14[0-9a-f]{7})',code))
anchors={a:string(int(a,16)) for a in refs}
print(json.dumps({a:v for a,v in sorted(anchors.items()) if v},indent=2))
for target in [0x14012bcc0,0x1401cae90,0x140e29270,0x140eaf030]:
 owners=set()
 for m in re.finditer(re.escape(struct.pack('<Q',target)),d):
  for delta in range(8,4096,8):
   o=m.start()-delta
   if o<0:break
   col=struct.unpack_from('<Q',d,o)[0];c=off(col)
   if c is None or c+24>len(d):continue
   sig,_,_,td,_,selfr=struct.unpack_from('<IIIIII',d,c)
   if sig==1 and col==0x140000000+selfr:
    s=string(0x140000000+td+16)
    if s:owners.add(s)
    break
 print(hex(target),sorted(owners))
for meta in [0x143d7e400]:
 o=off(meta)
 if o is None or o+40>len(d):
  print('QMetaObject',hex(meta),'not file-backed; runtime initialization unresolved')
  continue
 ptrs=struct.unpack_from('<5Q',d,o)
 print('QMetaObject',hex(meta),[hex(x) for x in ptrs],string(ptrs[1]))
for target in [0x1400df2b0,0x1401cc740,0x140e2b560]:
 for m in re.finditer(re.escape(struct.pack('<Q',target)),d):
  if m.start()>=16:
   sp=struct.unpack_from('<Q',d,m.start()-16)[0];s=string(sp)
   if s:print('meta-method-owner',hex(target),s)
