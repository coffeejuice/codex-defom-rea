"""Prepare passive private-value, modal, brush and wrapper capture; no launch."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[1]
old = ROOT/'analysis/wave3-editor-base-grid-probe-v3.js'
assert hashlib.sha256(old.read_bytes()).hexdigest() == 'e860e7ca70b0ea24a5846b6d2f8a04f0e47cd55a8b83d37bdb7ea9dd4bc9ee41'
s = old.read_text(encoding='utf-8')
def sub(a,b):
    global s
    assert s.count(a) == 1, a[:100]
    s = s.replace(a,b)
sub('let operation=null,guiModel=null,ui=null,grid=null,gridPrivate=null,setupEntered=false,qobjectFrame=null;',
    'let operation=null,guiModel=null,ui=null,grid=null,gridPrivate=null,setupEntered=false,qobjectFrame=null,widgetCtor=null;')
sub('applicationLayoutDirection:g.add(0x9c96b4).readS32(),',
    'applicationLayoutDirection:g.add(0x9c96b4).readS32(),applicationModalRaw:g.add(0x9cae3c).readU8(),')
sub("this.selected=!!current('init')&&same(args[0],editor)&&this.returnAddress.equals(m.base.add(0x6e6f2));",
    "this.selected=!!current('init')&&same(args[0],editor);this.callsite=moduleAt(this.returnAddress);")
sub("record('parentless_palette_enter',{state:opacityState()});",
    "record('parentless_palette_enter',{state:opacityState(),globals:gid(),actualReturnAddress:this.callsite});")
sub("value:read('brush-data',()=>({data:str(pp(b)),style:pp(b).add(4).readS32()}))",
    "value:read('brush-data',()=>({data:str(pp(b)),style:pp(b).add(4).readS32(),alphaHighByte:pp(b).add(0x0d).readU8(),colorRaw16:hex(pp(b).add(8).readByteArray(16))}))")
sub("initializedGuardStorage:read('wrapper-guard-storage',()=>({modelRaw:hex(editor.add(0x28).readByteArray(16)),",
    "initializedGuardStorage:read('wrapper-guard-storage',()=>({modelPointer:str(pp(editor.add(0x28))),modelType:editor.add(0x30).readS32(),enabled34:editor.add(0x34).readU8(),flag35:editor.add(0x35).readU8(),operationPointer:point===0x130740?str(pp(editor.add(0x38))):null,procedureType:point===0x130740?editor.add(0x40).readS32():null,modelRaw:hex(editor.add(0x28).readByteArray(16)),")
sub(" }else if(name==='qtgui4_64.dll'){",r'''
  attach(m,0x2cf3c0,{onEnter(args){this.selected=armed&&!setupEntered&&same(args[1],m.base.add(0x21a8858));
   if(!this.selected)return;this.notifier=cp(args[0]);
   if(!same(this.notifier,m.base.add(0x3d76700)))throw new Error('Wrong selected notifier');
   record('wrapper_active_step_notifier_enter',{notifier:str(this.notifier),key:'2signalChangeActiveStepData()',
    suppressed:this.notifier.add(0x141).readU8(),flag90:this.notifier.add(0x90).readU8(),widget:wid(editor)});},
   onLeave(){if(this.selected)record('wrapper_active_step_notifier_return',{
    suppressed:this.notifier.add(0x141).readU8(),flag90:this.notifier.add(0x90).readU8(),widget:wid(editor)});}});
 }else if(name==='qtgui4_64.dll'){''')
sub('  modules.gui=m;',r'''  modules.gui=m;
  attach(m,0x70d70,{onEnter(args){this.selected=armed&&!setupEntered&&same(args[0],editor);
   if(!this.selected)return;if(widgetCtor)throw new Error('Nested selected QWidget constructor');
   this.frame={widget:cp(args[0]),parent:cp(args[1]),tid:Process.getCurrentThreadId(),private:null};widgetCtor=this.frame;
   record('selected_qwidget_constructor_enter',{widget:str(editor),parent:str(this.frame.parent),flags:args[2].readU32()});},
   onLeave(){if(this.selected){if(widgetCtor!==this.frame)throw new Error('Selected QWidget context lost');
    record('selected_qwidget_constructor_return',{widget:wid(editor),private:str(this.frame.private)});widgetCtor=null;}}});
  attach(m,0x5e780,{onEnter(args){this.selected=!!widgetCtor&&widgetCtor.tid===Process.getCurrentThreadId()&&this.returnAddress.equals(m.base.add(0x70db5));
   if(!this.selected)return;this.private=cp(args[0]);widgetCtor.private=this.private;
   record('selected_widget_private_enter',{private:str(this.private),version:args[1].toUInt32()});},
   onLeave(result){if(!this.selected)return;const d=this.private;
    if(!same(result,d))throw new Error('QWidgetPrivate return identity changed');
    record('selected_widget_private_return',{private:str(d),font:read('private-font',()=>font(d.add(0x180))),
     palette:read('private-palette',()=>palette(d.add(0x170))),localeRaw4:hex(d.add(0x1a8).readByteArray(4)),
     threadData:str(pp(d.add(0x40))),qobjectFlags:d.add(0x20).readU32(),globals:gid()});}});
''')

def bytes_at(name,rva):
    data=(ROOT/'evidence/original-binaries'/name).read_bytes()
    pe=struct.unpack_from('<I',data,0x3c)[0]
    table=pe+24+struct.unpack_from('<H',data,pe+20)[0]
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        _,start,length,raw=struct.unpack_from('<IIII',data,table+40*i+8)
        if start<=rva and rva+12<=start+length:return data[raw+rva-start:raw+rva-start+12].hex()
    raise ValueError(hex(rva))
start=s.index('const MANIFEST = ')+len('const MANIFEST = ')
end=s.index(';\nconst LIMIT',start)
manifest=json.loads(s[start:end])
for name,rva in [('QtGui4_64.dll',0x70d70),('QtGui4_64.dll',0x5e780),('DEF_GUI_MO_64.exe',0x2cf3c0)]:
    manifest[name.lower()+':'+format(rva,'x')]=bytes_at(name,rva)
s=s[:start]+json.dumps(manifest,indent=2)+s[end:]
target=ROOT/'analysis/wave3-editor-base-grid-probe-v4.js'
assert not target.exists()
target.write_text(s,encoding='utf-8')
subprocess.run(['node','--check',str(target)],check=True,capture_output=True)
digest=hashlib.sha256(target.read_bytes()).hexdigest()
pre=json.loads((ROOT/'build/original-editor-probe-003/root-preflight.json').read_text())
for item in pre['modules']:
    assert hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256']
scratch=Path(pre['cwd'])
assert str(scratch.resolve()).lower()==r'c:\problem\m2m42'
actual={}
for path in sorted(scratch.rglob('*')):
    assert not path.is_symlink() and not path.is_junction()
    if path.is_file():actual[path.relative_to(scratch).as_posix()]=hashlib.sha256(path.read_bytes()).hexdigest()
assert actual==pre['cwd_files']
folder=ROOT/'build/original-editor-probe-004'
assert not folder.exists()
folder.mkdir()
(folder/'probe.js').write_bytes(target.read_bytes())
pre.update(utc=datetime.now(timezone.utc).isoformat(),script_sha256=digest,
    source_script=str(target.relative_to(ROOT)),checked_sites=len(manifest),
    review='Passive exact-owner/thread constructor and modal/brush/wrapper observations. No GUI input, Qt getter/event pump, target memory writes or file writes. Inherited process-local cwd setup only.',
    new_qualification='Palette callsite may be relocated by the adjacent instruction hook; receiver/init identity is primary selector and actual return address is retained.')
(folder/'root-preflight.json').write_text(json.dumps(pre,indent=2)+'\n',encoding='utf-8')
record=dict(status='Prepared and syntax/preflight verified; not launched',source_sha256=hashlib.sha256(old.read_bytes()).hexdigest(),
    script=str(target.relative_to(ROOT)),sha256=digest,checked_sites=len(manifest),cwd_files=len(actual),
    changes=['Actual modal byte captured at surrounding source points.',
        'Palette selector uses actual selected init/receiver rather than possibly relocated return address; return address retained.',
        'Capture initialized private font/palette/locale and thread-null interval on exact selected QWidget constructor.',
        'Record role10 solid brush alpha source byte and raw QColor separately, preserving actual palette/brush identities.',
        'Capture typed wrapper fields and selected ChangeActiveStepData notifier entry/return; no callback success inference.'])
(ROOT/'analysis/wave3-editor-base-grid-probe-v4-freeze.json').write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(folder=str(folder),sha256=digest,checked_sites=len(manifest),launched=False)))
