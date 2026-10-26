"""Read original Qt getReal transition data; no DLL loading or execution."""
import hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=ROOT/'evidence/original-binaries/QtCore4_64.dll';b=p.read_bytes()
pe=struct.unpack_from('<I',b,60)[0];opt=pe+24;base=struct.unpack_from('<Q',b,opt+24)[0];sh=opt+struct.unpack_from('<H',b,pe+20)[0]
sections=[]
for i in range(struct.unpack_from('<H',b,pe+6)[0]):
 vs,va,rs,raw=struct.unpack_from('<IIII',b,sh+40*i+8);sections.append((va,rs,raw))
def read(va,n):
 rva=va-base;off=next(raw+rva-v for v,s,raw in sections if v<=rva and rva+n<=v+s)
 return b[off:off+n]
table=list(read(0x6722c880,130));dispatch=list(read(0x670c5ff4,69));targets=struct.unpack('<7I',read(0x670c5fd8,28))
class_targets={0x670c5bbf:2,0x670c5bc9:5,0x670c5bd3:6,0x670c5bdd:7,0x670c5be7:8,0x670c5bf1:9}
classes={}
for code in range(32,127):
 target=base+targets[dispatch[code-48]] if 48<=code<=116 else 0x670c5bfb
 group=class_targets.get(target)
 if group is None:group=1 if chr(code) in '+-' else 3 if chr(code)=='.' else 4 if chr(code).lower()=='e' else 0
 classes[chr(code)]=group
out={'binary':str(p.relative_to(ROOT)),'sha256':hashlib.sha256(b).hexdigest(),'function':'670c59b0','qualification':'Direct PE constants plus bounded original instruction mapping. C-locale ASCII character classification only; non-C locale and Unicode categories not generalized. No runtime validation. Stop states0/13 are not appended; state table does not prove numeric conversion succeeds. Function .pdata ends670c6039;670c5fd8 onward is embedded jump data, not code.','transition_table_va':'0x6722c880','rows':[table[i*10:i*10+10] for i in range(13)],'ascii_character_classes':classes,'dispatch_targets':[hex(base+t) for t in targets],'next':'Use table with exact pushback/124-byte cap and recovered C-locale toDouble only after qualifying an input branch; separately recover integer automatic-base, decoder/BOM and read failure behavior.'}
q=ROOT/'analysis/wave2-save-diffs-qt-textstream-real-tables.json';q.write_text(json.dumps(out,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'output':str(q.relative_to(ROOT)),'sha256':hashlib.sha256(q.read_bytes()).hexdigest(),'rows':out['rows'],'digit_classes':{c:classes[c] for c in '0+-.eEnNaAiIfF,'}}))
