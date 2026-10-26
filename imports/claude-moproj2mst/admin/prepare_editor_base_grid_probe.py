"""Prepare (never launch) a bundled exact-CLI editor base/first-grid observer."""
import hashlib
import json
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[1]
base = root/'build/original-splitter-probe-004/probe.js'
assert hashlib.sha256(base.read_bytes()).hexdigest() == '582b6291dc9abc631048732631bac058f8697d69049bb28a2a4c496e73f394b1'
script = base.read_text().replace('splitter','editor')

def replace_once(old,new):
    global script
    assert script.count(old) == 1, old[:100]
    script = script.replace(old,new,1)

replace_once('let mainView=null,manager=null,editor=null,armed=false,done=false,selectedTailComplete=false;',
'''let mainView=null,manager=null,editor=null,armed=false,done=false,selectedTailComplete=false;
let mainWindow=null,mainReady=false,normalOpen=false,postOpen=false,project=null,modelChild=null;
let operation=null,guiModel=null,ui=null,grid=null,gridPrivate=null,setupEntered=false,qobjectFrame=null;
const EXPECTED_PROJECT='C:\\\\PROBLEM\\\\m2m42\\\\DB_deconstruction_02.moproj';''')
replace_once("record('view_default_return',{selected:editor?wid(editor):null,globals:modules.gui&&modules.core?gid():null});",
"record('selected_editor_first_grid_published',{selected:editor?wid(editor):null,grid:gidGrid(),globals:gid(),mainReady,normalOpen,postOpen,setupEntered});")
replace_once("Only first SplitView constructor tail and exact dependencies; raw unknown queue fields have no invented semantics. No Qt getters/native effect calls, GUI or event pump.",
"Native CLI Open to actual first editor grid UI publication, before spacing/margins; passive source-bound observations only. No full Save/Close comparison or GUI input.")
start=script.index("  attach(m,0xe9400,")
end=script.index(" }else if(name==='qtgui4_64.dll')",start)
script=script[:start]+r'''
  attach(m,0x10490,{onEnter(args){if(!mainWindow)mainWindow=cp(args[0]);this.selected=same(args[0],mainWindow);},
   onLeave(){if(this.selected){mainReady=true;record('mainwindow_return',{owner:str(mainWindow)});}}});
  attach(m,0x1ca30,{onEnter(args){this.selected=false;
   if(!mainReady||!same(args[0],mainWindow)||!this.returnAddress.equals(m.base.add(0x1c94)))return;
   if(normalOpen)throw new Error('Repeated selected CLI Open');
   const path=qtText(args[1]);if(path.toLowerCase()!==EXPECTED_PROJECT.toLowerCase())throw new Error('Unexpected original Open path');
   normalOpen=true;this.selected=true;record('native_open_enter',{path,owner:str(mainWindow)});
  },onLeave(result){if(this.selected)record('native_open_return',{project:str(cp(result)),matches:same(result,project)});}});
  attach(m,0x8a9b0,{onEnter(args){this.selected=normalOpen;if(!this.selected)return;
   if(project&&!same(project,args[0]))throw new Error('Second selected Project');
   project=cp(args[0]);modelChild=pp(project.add(8));record('project_read_enter',{project:str(project),modelChild:str(modelChild)});
  },onLeave(result){if(this.selected)record('project_read_return',{result:cp(result).toInt32()&255});}});
  attach(m,0x2a0e0,{onEnter(args){if(!normalOpen)return;
   if(!same(args[0],mainWindow)||!same(args[1],project))throw new Error('Wrong postOpen ownership');
   postOpen=true;record('post_open_enter',{project:str(project)});}});
  attach(m,0xeb26f0,{onEnter(args){if(!normalOpen)return;
   if(!project||!modelChild||modelChild.isNull()||editor)throw new Error('Unexpected selected editor');
   editor=cp(args[0]);operation=cp(args[1]);if(operation.isNull())throw new Error('Null operation');
   armed=true;record('editor_constructor_enter',{editor:str(editor),operation:str(operation),postOpen});}});
  attach(m,0x5f770,{onEnter(args){this.selected=same(args[0],operation);},onLeave(result){if(this.selected){
   guiModel=cp(result);record('operation_model',{operation:str(operation),model:str(guiModel)});}}});
  attach(m,0xed9830,{onEnter(args){if(!normalOpen)return;
   if(!same(args[1],editor)||!same(args[0],pp(editor.add(0x140)))||!guiModel||guiModel.isNull())throw new Error('Setup identity mismatch');
   ui=cp(args[0]);setupEntered=true;selectedTailComplete=true;record('setup_enter',{ui:str(ui),widget:wid(editor),globals:gid()});}});
  attach(m,0xed9a11,{onEnter(){if(!setupEntered)return;
   if(!grid||!same(pp(ui),grid))throw new Error('First grid publication mismatch');
   complete();}});
  for(const point of [0x377990,0x3779f3,0x1307be])attach(m,point,{onEnter(){
   if(!armed||setupEntered)return;record('wrapper_checkpoint',{site:point,widget:wid(editor),
    qualification:'Initialized QWidget fields only; raw wrapper guards not read before their own constructor.'});}});
''' + script[end:]
replace_once("if(!current('init')||!same(args[1],editor))return;",
    "if((!current('init')&&!setupEntered)||!same(args[1],editor))return;")
replace_once("if(e.add(0x10).readU16()!==0x0f)return;",
    "if(![0x0f,0xaa,0x44].includes(e.add(0x10).readU16()))return;")
replace_once("if(!current('init')||!same(args[0],editor))return;",
    "if((!current('init')&&!setupEntered)||!same(args[0],editor))return;")
replace_once("if(e.add(0x10).readU16()!==0x4a)return;",
    "if(![0x4a,0x4c].includes(e.add(0x10).readU16()))return;")
replace_once("inputs:sid('event-inputs','create-inputs',()=>eventInputs(editor)),globals:gid()",
    "inputs:sid('event-inputs','notify-inputs',()=>eventInputs(editor)),globals:gid(),grid:gidGrid()")
replace_once("inputs:sid('event-inputs','create-return-inputs',()=>eventInputs(editor)),globals:gid()",
    "inputs:sid('event-inputs','notify-return-inputs',()=>eventInputs(editor)),globals:gid(),grid:gidGrid()")
replace_once("thread:tid(this.f.thread)});},", "thread:tid(this.f.thread),grid:gidGrid()});},")
replace_once("thread:tid(f.thread)});}finally{pop(f);}}});", "thread:tid(f.thread),grid:gidGrid()});}finally{pop(f);}}});")
# Source-qualified first-frame getter fields; earlier incidental frame bytes remain raw.
replace_once("function gid(){",r'''
function qtText(a){const d=pp(a),n=d.add(8).readS32();if(n<0||n>512)throw new Error('QString bound');
 return n?pp(d.add(0x10)).readUtf16String(n):'';}
function gridState(){if(!grid||!gridPrivate)return null;const d=gridPrivate;
 return {object:str(grid),private:str(d),vtable:moduleAt(pp(grid)),parent:str(pp(d.add(0x10))),
 thread:str(pp(d.add(0x40))),qobjectFlags:d.add(0x20).readU32(),
 layoutFlagsRaw:d.add(0x9c).readU32(),layoutFlagsKnownMask:15,
 rows:d.add(0xc0).readS32(),columns:d.add(0xc4).readS32(),margins:[0x8c,0x90,0x94,0x98].map(x=>d.add(x).readS32()),
 cachedSize:d.add(0x108).readS32(),dirtyRaw:d.add(0x134).readU32(),dirtyKnownMask:0x17,
 installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:ui?str(pp(ui)):null,
 qobjectCounter:modules.core.base.add(0x2f4344).readU32()};}
function gidGrid(){return grid&&gridPrivate?sid('grid','first-grid',gridState):null;}
function opacityState(){const d=priv(editor);return {widget:wid(editor),privateD8:d.add(0x1d8).readU32(),
 qualification:'Only source-consumed bit17 is a semantic opacity observation; other bits remain raw.'};}
function gid(){''')
extra_gui = r'''
  for(const point of [0x6e6e5,0x6e6ea,0x6e72b,0x6e75b])attach(m,point,{onEnter(){
   if(!current('init')||!same(this.context.rsi,editor))return;
   record('parentless_checkpoint',{site:point,globals:gid(),state:opacityState(),
    opacityDecision:point===0x6e72b?(this.context.rax.toInt32()&255):null});}});
  attach(m,0x64410,{onEnter(args){this.selected=!!current('init')&&same(args[0],editor)&&this.returnAddress.equals(m.base.add(0x6e6f2));
   if(this.selected)record('parentless_palette_enter',{state:opacityState()});},onLeave(result){if(this.selected){const p=cp(result);
   record('parentless_palette_return',{palette:read('returned-palette',()=>palette(p)),state:opacityState()});}}});
  attach(m,0x49bf0,{onEnter(args){this.selected=!!current('init')&&this.returnAddress.equals(m.base.add(0x6e703));
   if(this.selected)record('parentless_brush_enter',{palette:str(cp(args[0])),group:args[1].toInt32(),role:args[2].toInt32()});},
   onLeave(result){if(this.selected){const b=cp(result);record('parentless_brush_return',{brush:str(b),
    value:read('brush-data',()=>({data:str(pp(b)),style:pp(b).add(4).readS32()}))});}}});
  attach(m,0x11c090,{onEnter(args){this.selected=!!current('init')&&this.returnAddress.equals(m.base.add(0x6e721));},
   onLeave(result){if(this.selected)record('parentless_brush_isopaque',{result:(cp(result).toInt32()&255)!==0});}});
  attach(m,0x97940,{onEnter(args){this.selected=!!current('init')&&same(args[0],priv(editor));if(this.selected)record('opacity_effect_enter',{state:opacityState()});},
   onLeave(){if(this.selected)record('opacity_effect_return',{state:opacityState()});}});
  attach(m,0x3f700,{onEnter(args){this.selected=setupEntered&&same(args[1],editor)&&this.returnAddress.equals(modules.app.base.add(0xed99ab));
   if(!this.selected)return;if(grid)throw new Error('Second first-grid allocation');grid=cp(args[0]);
   record('first_grid_constructor_enter',{grid:str(grid),parent:str(editor),installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:str(pp(ui))});},
   onLeave(){if(this.selected)record('first_grid_constructor_return',{grid:gidGrid()});}});
  attach(m,0x46c73,{onEnter(){if(!gridPrivate||!same(this.context.rsi,grid))return;
   record('first_grid_before_install',{grid:gidGrid()});}});
'''
replace_once("  modules.gui=m;", "  modules.gui=m;\n"+extra_gui)
extra_core=r'''
  attach(m,0x12e630,{onEnter(args){this.selected=same(args[0],editor)||same(args[0],grid);if(!this.selected)return;
   if(qobjectFrame)throw new Error('Nested selected QObject constructor');
   this.f={object:cp(args[0]),private:cp(args[1]),parent:cp(args[2])};qobjectFrame=this.f;
   // Object vtable/data are not initialized at entry. Only the already-built private record is read.
   record('selected_qobject_enter',{object:str(this.f.object),private:str(this.f.private),parent:str(this.f.parent),
    counter:m.base.add(0x2f4344).readU32(),privateFlags:this.f.private.add(0x20).readU32(),
    gridPrivate:same(this.f.object,grid)?{rows:this.f.private.add(0xc0).readS32(),columns:this.f.private.add(0xc4).readS32(),
     layoutFlagsRaw:this.f.private.add(0x9c).readU32(),knownMask:15}:null});
  },onLeave(){if(!this.selected)return;record('selected_qobject_return',{object:str(this.f.object),
   counter:m.base.add(0x2f4344).readU32(),thread:tid(pp(this.f.private.add(0x40))),grid:gidGrid()});qobjectFrame=null;}});
  attach(m,0x12e68a,{onEnter(){if(!qobjectFrame)return;if(same(qobjectFrame.object,grid))gridPrivate=cp(qobjectFrame.private);
   record('selected_qobject_thread_bound',{object:str(qobjectFrame.object),thread:tid(pp(qobjectFrame.private.add(0x40))),grid:gidGrid()});}});
  attach(m,0x12e6e0,{onEnter(){if(!qobjectFrame)return;record('selected_qobject_before_counter',{
   object:str(qobjectFrame.object),counter:m.base.add(0x2f4344).readU32(),grid:gidGrid()});}});
'''
replace_once("  modules.core=m;", "  modules.core=m;\n"+extra_core)

# Regenerate exact-site manifest from the three immutable module images. Every
# added site comes from retained native contracts/source, never a guessed offset.
images={
 'def_gui_mo_64.exe':('DEF_GUI_MO_64.exe','fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'),
 'qtgui4_64.dll':('QtGui4_64.dll','c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887'),
 'qtcore4_64.dll':('QtCore4_64.dll','bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160')}
def pe_bytes(name,rva,size=12):
    file,digest=images[name]; data=(root/'evidence/original-binaries'/file).read_bytes()
    assert hashlib.sha256(data).hexdigest()==digest
    pe=struct.unpack_from('<I',data,0x3c)[0]; count=struct.unpack_from('<H',data,pe+6)[0]
    start=pe+24+struct.unpack_from('<H',data,pe+20)[0]
    for i in range(count):
        vs,rv,rs,raw=struct.unpack_from('<IIII',data,start+40*i+8)
        if rv<=rva and rva+size<=rv+rs:return data[raw+rva-rv:raw+rva-rv+size].hex()
    raise ValueError(hex(rva))
manifest={}
for name,part in [('def_gui_mo_64.exe',script.split("if(name==='def_gui_mo_64.exe'){")[1].split(" }else if(name==='qtgui4_64.dll')")[0]),
 ('qtgui4_64.dll',script.split(" }else if(name==='qtgui4_64.dll'){")[1].split(" }else if(name==='qtcore4_64.dll')")[0]),
 ('qtcore4_64.dll',script.split(" }else if(name==='qtcore4_64.dll'){")[1])]:
    offsets=[int(x,16) for x in re.findall(r'attach\(m,(0x[0-9a-f]+),',part)]
    for group in re.findall(r'for\(const point of \[([^]]+)\]\)',part):
        offsets += [int(x,16) for x in group.split(',')]
    assert len(offsets)==len(set(offsets)),name
    for rva in offsets:manifest[name+':'+format(rva,'x')]=pe_bytes(name,rva)
begin=script.index('const MANIFEST = {');end=script.index('\nconst LIMIT',begin)
script=script[:begin]+'const MANIFEST = '+json.dumps(manifest,indent=2)+';'+script[end:]
script=script.replace('Passive approved first-split constructor tail; no target launch by worker.',
 'Prepared native CLI editor base and first-grid observation; root review required before launch.')
target=root/'analysis/wave3-editor-base-grid-probe-v1.js'
assert not target.exists()
target.write_text(script,encoding='utf-8')
record=dict(status='Prepared, NOT yet reviewed or launched',script=str(target.relative_to(root)),
 sha256=hashlib.sha256(target.read_bytes()).hexdigest(),checked_sites=len(manifest),
 source_template=str(base.relative_to(root)),template_sha256=hashlib.sha256(base.read_bytes()).hexdigest(),
 prior_cli_selection='analysis/wave3-editor-owned-open-probe-v4.js',
 evidence=['analysis/wave3-editor-parentless-base-grid-dependencies.json',
  'analysis/wave3-editor-first-parented-grid-contract.json','analysis/wave3-editor-caller-bool-variant-cleanup-contract.json'],
 endpoint='140ed9a11 after actual first grid UI[0] publication, before spacing/margins',
 qualifications=['No GUI input/event pump/Qt calls. Only same owned-process cwd calls from reviewed template.',
  'Exact original CLI Open caller/path/owner selection. No synthetic constructor receivers.',
  'Dispatcher virtual-slot qualification must be reconciled before interpreting native wake snapshots.',
  'Raw high layout/dirty/event bits and incidental early widget fields remain uninitialized unless source binds them.'])
(root/'analysis/wave3-editor-base-grid-probe-v1-preparation.json').write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'script':record['script'],'sha256':record['sha256'],'checked_sites':len(manifest),'launched':False}))
