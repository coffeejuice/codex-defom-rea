"""Preserve v1, prepare source-corrected v2 without target execution."""
import hashlib
import json
from pathlib import Path
import struct

root=Path(__file__).resolve().parents[1]
old=root/'analysis/wave3-editor-base-grid-probe-v1.js'
assert hashlib.sha256(old.read_bytes()).hexdigest()=='9fcd4ce07f32ca0acf0c968ba0913fabccb44cc8e608427907dad963c4228122'
s=old.read_text()
changes=[]
def sub(a,b,n=1):
    global s
    assert s.count(a)==n,a[:90]
    s=s.replace(a,b)
sub('const refs=data.readS32(), size=data.add(0x70).readS32()',
    'const refs=data.add(0x68).readS32(), size=data.add(0x70).readS32()')
changes.append('QMap refcount reads +0x68; probe004 data+0 value is not a refcount. Map size/order observations remain distinct.')
sub("if(loc&&loc.name.toLowerCase()==='qtcore4_64.dll'&&loc.rva==='0x141b70'&&!d.isNull())",r'''
 const direct=loc&&loc.name.toLowerCase()==='qtcore4_64.dll'&&loc.rva==='0x141b70';
 const guiThunk=loc&&loc.name.toLowerCase()==='qtgui4_64.dll'&&loc.rva==='0x5e0ca0'&&
  hex(wake.readByteArray(6))==='ff2572690b00'&&pp(modules.gui.base.add(0x697618)).equals(modules.core.base.add(0x141b70));
 if((direct||guiThunk)&&!d.isNull())''')
changes.append('Admit exact QtGui wake thunk only with verified six bytes and actual IAT target, then read source-bound Win32 private fields.')
sub("if(!gridPrivate||!same(this.context.rsi,grid))return;", "if(!gridPrivate||!same(this.context.r12,grid))return;")
changes.append('At65046c73 R12 is actual layout, RSI is private storage. Fix hook selection before installed-layout store.')
a=s.index('  for(const point of [0x377990,0x3779f3,0x1307be])')
b=s.index(" }else if(name==='qtgui4_64.dll')",a)
s=s[:a]+r'''
  attach(m,0x377990,{onEnter(){if(!armed||setupEntered||!same(pp(cp(this.context.rsp).add(0x50)),editor))return;
   record('wrapper_qwidget_return',{widget:wid(editor)});}});
  for(const point of [0x377950,0x130740])attach(m,point,{onEnter(args){this.selected=armed&&!setupEntered&&same(args[0],editor);},
   onLeave(){if(this.selected)record('wrapper_constructor_return',{site:point,widget:wid(editor),
    initializedGuardStorage:read('wrapper-guard-storage',()=>({modelRaw:hex(editor.add(0x28).readByteArray(16)),
     operationRaw:point===0x130740?hex(editor.add(0x38).readByteArray(16)):null}))});}});
''' + s[b:]
changes.append('Use exact wrapper owner checks and entry/onLeave pairs rather than RET-site hooks across procedure boundaries.')
sub("if(type==='unused')", "",n=0) if False else None
sub("type:e.add(0x10).readU16(),flags:e.add(0x12).readU16()};}",
    "type:e.add(0x10).readU16(),flags:e.add(0x12).readU16(),child:e.add(0x10).readU16()===0x44?str(pp(e.add(0x18))):null};}")
sub("installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:ui?str(pp(ui)):null,",
    "installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:ui?str(pp(ui)):null,\n parentChildren:list(pp(priv(editor).add(0x18)),LIMIT.queueEntries,'editor-children',p=>str(p)),")
changes.append('Record actual QChildEvent child pointer and ordered editor QObject child list at same grid states.')
sub("this.f=push('init',{private:d});record('widget_init_enter',{context:this.f?.id,private:str(d),widget:str(editor)});",
    "this.f=push('init',{private:d});record('widget_init_enter',{context:this.f?.id,private:str(d),widget:str(editor),parent:str(cp(args[1])),flags:args[2].readU32(),initialFont:read('initial-font',()=>font(d.add(0x180))),initialPalette:read('initial-palette',()=>palette(d.add(0x170))),privateD8:d.add(0x1d8).readU32()});")
changes.append('Capture initialized private font/palette and actual null parent/flags before QWidget private-init mutations.')
# Reuse the prior bounded native exception observer, preserving original handling.
v4=(root/'analysis/wave3-editor-owned-open-probe-v4.js').read_text()
exception=v4[v4.index('let exceptionCount = 0;'):v4.index("send({ kind: 'probe_ready'")]
s=s.replace("send({kind:'probe_ready'",exception+"\nsend({kind:'probe_ready'",1)
changes.append('Retain bounded native exception diagnostics; handler returns false and preserves original handling.')

def bytes_at(name,rva):
    data=(root/'evidence/original-binaries'/name).read_bytes()
    pe=struct.unpack_from('<I',data,0x3c)[0];n=struct.unpack_from('<H',data,pe+6)[0]
    start=pe+24+struct.unpack_from('<H',data,pe+20)[0]
    for i in range(n):
        _,rv,size,raw=struct.unpack_from('<IIII',data,start+40*i+8)
        if rv<=rva and rva+12<=rv+size:return data[raw+rva-rv:raw+rva-rv+12].hex()
    raise ValueError(hex(rva))
start=s.index('const MANIFEST = ')+len('const MANIFEST = ')
end=s.index(';\nconst LIMIT',start)
manifest=json.loads(s[start:end])
for rva in (0x3779f3,0x1307be):del manifest['def_gui_mo_64.exe:'+format(rva,'x')]
for rva in (0x377950,0x130740):manifest['def_gui_mo_64.exe:'+format(rva,'x')]=bytes_at('DEF_GUI_MO_64.exe',rva)
s=s[:start]+json.dumps(manifest,indent=2)+s[end:]
# Immutable original proof for the new thunk recognizer.
assert bytes_at('QtGui4_64.dll',0x5e0ca0)[:12]=='ff2572690b00'
target=root/'analysis/wave3-editor-base-grid-probe-v2.js'
assert not target.exists()
target.write_text(s,encoding='utf-8')
record=dict(status='Prepared v2; not launched',source=str(old.relative_to(root)),
 source_sha256=hashlib.sha256(old.read_bytes()).hexdigest(),script=str(target.relative_to(root)),
 sha256=hashlib.sha256(target.read_bytes()).hexdigest(),checked_sites=len(manifest),changes=changes,
 qualifications=['V1 and probe004 remain immutable. Probe004 Win32 fields were not captured; its equal opaque snapshots do not prove no wake effects.',
  'Probe004 mislabeled refs field is unusable. Source-backed unchanged-map inference remains a separate qualified argument.',
  'Normal raw44 child-event returns true per6712cb5f/6712cedb; capture retains actual result without imposing property-event false.'])
(root/'analysis/wave3-editor-base-grid-probe-v2-amendment.json').write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'script':record['script'],'sha256':record['sha256'],'checked_sites':len(manifest),'launched':False}))
