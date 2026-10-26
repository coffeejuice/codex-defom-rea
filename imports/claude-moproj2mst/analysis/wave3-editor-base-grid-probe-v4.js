'use strict';
// Prepared only; root owns launch, loaded-file hash verification and cleanup.
// Root-authorized process-local cwd selection only; no file creation, GUI,
// Qt getter or event pump. Only the two process-local cwd functions are called.
if (Process.platform !== 'windows' || Process.arch !== 'x64') throw new Error('Wrong target platform');
// Check the expected image before any process-local setup.
Process.getModuleByName('DEF_GUI_MO_64.exe');
const SCRATCH = 'C:\\PROBLEM\\m2m42';
const kernel = Process.getModuleByName('kernel32.dll');
const setDirectory = new NativeFunction(kernel.getExportByName('SetCurrentDirectoryW'), 'int', ['pointer']);
const getDirectory = new NativeFunction(kernel.getExportByName('GetCurrentDirectoryW'), 'uint32', ['uint32', 'pointer']);
if (!setDirectory(Memory.allocUtf16String(SCRATCH))) throw new Error('Owned process cwd setup failed');
const cwdCapacity = 32768, cwdBuffer = Memory.alloc(cwdCapacity * 2);
const cwdLength = getDirectory(cwdCapacity, cwdBuffer);
if (!cwdLength || cwdLength >= cwdCapacity) throw new Error('Owned process cwd verification failed');
const actualCwd = cwdBuffer.readUtf16String(cwdLength);
if (actualCwd.toLowerCase() !== SCRATCH.toLowerCase()) throw new Error('Owned process cwd differs from requested scratch');
send({ kind: 'working_directory', pid: Process.id, requested: SCRATCH, actual: actualCwd,
  qualification: 'Process-local current directory only; root verified preexisting scratch. No files created by probe.' });
const EXPECTED = {
  'def_gui_mo_64.exe': 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
  'qtgui4_64.dll': 'c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887',
  'qtcore4_64.dll': 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160'
};
const MANIFEST = {
  "def_gui_mo_64.exe:10490": "4c8944241848895424104889",
  "def_gui_mo_64.exe:1ca30": "488954241048894c24084881",
  "def_gui_mo_64.exe:8a9b0": "488954241048894c24084881",
  "def_gui_mo_64.exe:2a0e0": "488954241048894c24085657",
  "def_gui_mo_64.exe:eb26f0": "488954241048894c24085748",
  "def_gui_mo_64.exe:5f770": "48894c24084883ec38488b4c",
  "def_gui_mo_64.exe:ed9830": "488954241048894c240853b8",
  "def_gui_mo_64.exe:ed9a11": "ba03000000488b8c2490c900",
  "def_gui_mo_64.exe:377990": "90488b4c2450488d05bb43fb",
  "qtgui4_64.dll:64410": "40534883ec20488b4120488b",
  "qtgui4_64.dll:49bf0": "48895c2408574883ec2083fa",
  "qtgui4_64.dll:11c090": "488bc4574883ec7048c74424",
  "qtgui4_64.dll:97940": "48895c2408574883ec20488b",
  "qtgui4_64.dll:3f700": "48894c2408574883ec3048c7",
  "qtgui4_64.dll:46c73": "4c89a0a8000000498d4c2410",
  "qtgui4_64.dll:6e4d0": "488bc4565741544155415648",
  "qtgui4_64.dll:1f2e30": "48894c24085556574883ec30",
  "qtgui4_64.dll:99110": "40555641544883ec20488b71",
  "qtgui4_64.dll:99181": "483bc3410f95c54584ed740d",
  "qtgui4_64.dll:7f9d0": "4883ec28488b0555b4940048",
  "qtgui4_64.dll:6e780": "90488d4c2438e89583190041",
  "qtgui4_64.dll:6e7ab": "448b1d96b1950041ffc34489",
  "qtgui4_64.dll:3b2470": "48894c2408534883ec3048c7",
  "qtgui4_64.dll:5e420": "488bc4574881ec8000000048",
  "qtgui4_64.dll:5e4b0": "4c8b1048895c24204c8bcf44",
  "qtgui4_64.dll:5e4cd": "488d4c2438ff15388e630084",
  "qtgui4_64.dll:6db20": "48895c240889542410574883",
  "qtgui4_64.dll:62460": "40565741544883ec3048c744",
  "qtgui4_64.dll:403210": "48894c24085356574883ec30",
  "qtgui4_64.dll:6e6e5": "e836bfffff488bcee81e5dff",
  "qtgui4_64.dll:6e6ea": "488bcee81e5dffffba040000",
  "qtgui4_64.dll:6e72b": "8b97d80100000fb6c88bc2c1",
  "qtgui4_64.dll:6e75b": "4c8d4610488d978001000048",
  "qtcore4_64.dll:12e630": "48894c24085356574883ec30",
  "qtcore4_64.dll:12e68a": "4885db7451488b53084c8b46",
  "qtcore4_64.dll:12e6e0": "f083055c5c1c0001488bc748",
  "qtcore4_64.dll:2acc2": "488bf04885c00f8574010000",
  "qtcore4_64.dll:119570": "40565741544883ec4048c744",
  "qtcore4_64.dll:11ae90": "40555657415441554883ec40",
  "qtcore4_64.dll:11afc6": "48897424704c896424284889",
  "qtcore4_64.dll:11afe8": "33ed48896c247066834e1201",
  "qtcore4_64.dll:11b015": "488b4f204885c9740a488b01",
  "qtcore4_64.dll:141b70": "4883ec28488b4908f08381a8",
  "def_gui_mo_64.exe:377950": "488954241048894c24084883",
  "def_gui_mo_64.exe:130740": "448944241848895424104889",
  "qtgui4_64.dll:70d70": "48894c2408565741544883ec",
  "qtgui4_64.dll:5e780": "48894c24085556574883ec30",
  "def_gui_mo_64.exe:2cf3c0": "4c894c24204c894424184889"
};
const LIMIT = { states:128, records:192, queueEntries:1024, queueRecords:1152,
 filters:16, gestures:64, ancestors:16, errors:32, metricDepth:16, postContexts:4 };
const modules={}, listeners=[], installed=new Map(), states=new Map(), errors=new Map(), truncations=new Map();
const stacks=new Map(), queueRecords=new Map();
const totals={states:0,records:0,failedReads:0,callbackErrors:0,queueRecords:0,init:0,font:0,metric:0,post:0,create:0,frame:0,geometry:0};
let mainView=null,manager=null,editor=null,armed=false,done=false,selectedTailComplete=false;
let mainWindow=null,mainReady=false,normalOpen=false,postOpen=false,project=null,modelChild=null;
let operation=null,guiModel=null,ui=null,grid=null,gridPrivate=null,setupEntered=false,qobjectFrame=null,widgetCtor=null;
const EXPECTED_PROJECT='C:\\PROBLEM\\m2m42\\DB_deconstruction_02.moproj';
function cp(p) { return p === null || p === undefined ? null : ptr(p.toString()); }
function pp(address) { return cp(address.readPointer()); }
function str(p) { return p === null || p === undefined ? null : p.toString(); }
function same(a, b) { return !!(a && b && a.equals(b)); }
function hex(b) { return Array.from(new Uint8Array(b)).map(x => x.toString(16).padStart(2, '0')).join(''); }
function moduleAt(p) {
  if (!p || p.isNull()) return null;
  const m = Process.findModuleByAddress(p);
  return m ? { name: m.name, base: str(m.base), rva: str(p.sub(m.base)) } : { unknown: str(p) };
}
function truncate(where, count, limit) {
  const existing = truncations.get(where);
  if (existing) existing.observed = count;
  else { const v = { where, observed: count, limit }; truncations.set(where, v); send({ kind: 'truncation', ...v }); }
  return { incomplete: true, where, observed: count, limit };
}
function read(where, fn) {
  try { return fn(); } catch (e) {
    totals.failedReads++;
    const key = where + '\n' + String(e);
    let id = errors.get(key);
    if (id === undefined) {
      if (errors.size >= LIMIT.errors) return truncate('errors', totals.failedReads, LIMIT.errors);
      id = errors.size + 1; errors.set(key, id);
      send({ kind: 'read_error', errorId: id, where, message: String(e) });
    }
    return { unavailable: true, errorId: id };
  }
}
function intern(type, data) {
  const key = type + ':' + JSON.stringify(data);
  if (states.has(key)) return states.get(key);
  if (totals.states >= LIMIT.states) return truncate('states', totals.states + 1, LIMIT.states);
  const id = ++totals.states; states.set(key, id);
  send({ kind: 'state', stateId: id, stateType: type, data }); return id;
}
function record(stage, data) {
  if (done) return;
  const ordinal = ++totals.records;
  if (ordinal > LIMIT.records) { truncate('records', ordinal, LIMIT.records); return; }
  send({ kind: 'observation', ordinal, stage, tid: Process.getCurrentThreadId(), data });
}
function priv(p){return pp(p.add(8));}
function role(p){if(same(p,mainView))return 'mainView';if(same(p,manager))return 'manager';if(same(p,editor))return 'editor';return null;}
function sid(type,where,fn){return intern(type,read(where,fn));}
function key(kind){return kind+':'+Process.getCurrentThreadId();}
function current(kind){const a=stacks.get(key(kind));return a&&a.length?a[a.length-1]:null;}
function push(kind,extra={}){const k=key(kind);let a=stacks.get(k);if(!a){a=[];stacks.set(k,a);}
 const limit=kind==='metric'?LIMIT.metricDepth:kind==='post'?LIMIT.postContexts:2;
 if(a.length>=limit){truncate('context-depth-'+kind,a.length+1,limit);return null;}
 const c={kind,key:k,id:++totals[kind],...extra};a.push(c);return c;}
function pop(c){if(!c)return;const a=stacks.get(c.key);if(!a||a[a.length-1]!==c)throw new Error('Unbalanced '+c.key);a.pop();}
function font(a){const d=pp(a);return {object:str(a),data:str(d),mask:a.add(8).readU32(),
 field38:d.isNull()?null:d.add(0x38).readS32(),field3c:d.isNull()?null:d.add(0x3c).readS32()};}
function palette(a){return {object:str(a),data:str(pp(a)),word8:a.add(8).readU32()};}
function rect(a){return [0,4,8,12].map(n=>a.add(n).readS32());}
function method(vt,offset){return vt.isNull()?null:moduleAt(pp(vt.add(offset)));}
function widget(o){
 if(!o||o.isNull())return {object:str(o),unavailable:'null widget'};
 const d=priv(o),data=pp(o.add(0x20));
 if(d.isNull()||!data.equals(d.add(0x148))||!(d.add(0x20).readU32()&1))
  return {object:str(o),private:str(d),unavailable:'uninitialized QWidgetData'};
 const e=pp(d.add(0x88)),pdvt=pp(o.add(0x10));
 return {object:str(o),role:role(o),private:str(d),vtable:moduleAt(pp(o)),paintVtable:moduleAt(pdvt),
  metricMethod:method(pdvt,0x28),devTypeMethod:method(pdvt,8),parent:str(pp(d.add(0x10))),
  threadData:str(pp(d.add(0x40))),qobjectFlags:d.add(0x20).readU32(),postedCount:d.add(0x24).readS32(),
  dynamicMeta:str(pp(d.add(0x28))),hookData:str(pp(d.add(0x78))),
  attributesLow:data.add(8).readU32(),windowFlags:data.add(12).readU32(),
  extended:[0x1c8,0x1cc,0x1d0].map(x=>d.add(x).readU32()),geometry:rect(data.add(0x14)),
  layout:str(pp(d.add(0xa8))),layoutItem:str(pp(d.add(0xc0))),fieldC8:str(pp(d.add(0xc8))),
  frameBytes:hex(d.add(0x138).readByteArray(4)),sizePolicy:d.add(0x1a0).readU32(),
  font:font(d.add(0x180)),palette:palette(d.add(0x170)),localeRaw4:hex(d.add(0x1a8).readByteArray(4)),
  extra:e.isNull()?{pointer:str(e)}:{pointer:str(e),dpiY:e.add(0x52).readU16(),
   proxy:str(pp(e.add(0x10))),style:str(pp(e.add(0x20))),minimum:[e.add(0x40).readS32(),e.add(0x44).readS32()],
   maximum:[e.add(0x48).readS32(),e.add(0x4c).readS32()]}};
}
function wid(o){return sid('widget','widget:'+str(o),()=>widget(o));}
function globals(){const g=modules.gui.base,c=modules.core.base,app=pp(c.add(0x2f4220)),style=pp(g.add(0x9c9688));
 return {cachedDC:str(pp(g.add(0x9cae30))),currentWidgets:g.add(0x9c9948).readS32(),
 maximumWidgets:g.add(0x9c994c).readS32(),compileVersion:c.add(0x2f1da4).readS32(),applicationAttributes:c.add(0x2f41f8).readU32(),
 applicationLayoutDirection:g.add(0x9c96b4).readS32(),applicationModalRaw:g.add(0x9cae3c).readU8(),
 application:str(app),appThreadData:app.isNull()?null:str(pp(priv(app).add(0x40))),
 defaultStyle:{object:str(style),vtable:style.isNull()?null:moduleAt(pp(style)),subElementRect:style.isNull()?null:method(pp(style),0xb8)}};}

function qtText(a){const d=pp(a),n=d.add(8).readS32();if(n<0||n>512)throw new Error('QString bound');
 return n?pp(d.add(0x10)).readUtf16String(n):'';}
function gridState(){if(!grid||!gridPrivate)return null;const d=gridPrivate;
 return {object:str(grid),private:str(d),vtable:moduleAt(pp(grid)),parent:str(pp(d.add(0x10))),
 thread:str(pp(d.add(0x40))),qobjectFlags:d.add(0x20).readU32(),
 layoutFlagsRaw:d.add(0x9c).readU32(),layoutFlagsKnownMask:15,
 rows:d.add(0xc0).readS32(),columns:d.add(0xc4).readS32(),margins:[0x8c,0x90,0x94,0x98].map(x=>d.add(x).readS32()),
 cachedSize:d.add(0x108).readS32(),dirtyRaw:d.add(0x134).readU32(),dirtyKnownMask:0x17,
 installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:ui?str(pp(ui)):null,
 parentChildren:list(pp(priv(editor).add(0x18)),LIMIT.queueEntries,'editor-children',p=>str(p)),
 qobjectCounter:modules.core.base.add(0x2f4344).readU32()};}
function gidGrid(){return grid&&gridPrivate?sid('grid','first-grid',gridState):null;}
function opacityState(){const d=priv(editor);return {widget:wid(editor),privateD8:d.add(0x1d8).readU32(),
 qualification:'Only source-consumed bit17 is a semantic opacity observation; other bits remain raw.'};}
function gid(){return sid('globals','globals',globals);}
function list(data, limit, where, mapper) {
  if (data.isNull()) return { data: str(data), unavailable: 'null list data' };
  const begin = data.add(8).readS32(), end = data.add(12).readS32();
  if (begin < 0 || end < begin) return { data: str(data), begin, end, malformed: true };
  const entries = [], count = end-begin;
  for (let i=0; i<Math.min(count,limit); i++) {
    const p = pp(data.add(0x18+8*(begin+i))); entries.push(mapper ? mapper(p) : str(p));
  }
  return { data: str(data), begin, end, count, entries,
    coverage: count > limit ? truncate(where,count,limit) : 'complete' };
}
function filters(o) {
  return list(pp(priv(o).add(0x70)), LIMIT.filters, 'filters:'+str(o), wrapper => {
    if (wrapper.isNull()) return { wrapper: str(wrapper), unavailable: 'null guard' };
    const f = pp(wrapper); if (f.isNull()) return { wrapper: str(wrapper), object: str(f) };
    const vt = pp(f), method = pp(vt.add(0x28));
    return { wrapper: str(wrapper), object: str(f), vtable: moduleAt(vt), method: moduleAt(method),
      threadData: str(pp(priv(f).add(0x40))) };
  });
}
function gestureMap(data) {
  if (data.isNull()) return { data:str(data), unavailable:'null gesture data' };
  const refs=data.add(0x68).readS32(), size=data.add(0x70).readS32(), entries=[], seen=new Set(); let node=pp(data.add(8));
  if(size<0) return {data:str(data),size,malformed:true};
  while(!node.equals(data) && entries.length<LIMIT.gestures) {
    if(node.isNull()||seen.has(str(node))) return {data:str(data),size,entries,malformed:'null/cycle'};
    seen.add(str(node)); entries.push({node:str(node),key:node.sub(8).readS32(),flags:node.sub(4).readS32()}); node=pp(node.add(8));
  }
  return {data:str(data),refs,size,entries,countMatches:node.equals(data)&&entries.length===size,
    coverage:node.equals(data)?'complete':truncate('gestures',size,LIMIT.gestures)};
}
function eventInputs(o) {
  const c=modules.core.base, app=pp(c.add(0x2f4220)), owner=pp(c.add(0x2f3c68));
  let internal={owner:str(owner),shutdown:c.add(0x2f3c70).readU8()};
  if(!owner.isNull()) { const d=pp(owner);internal.data=str(d);
    if(d.isNull())internal.unavailable='null internal registry data';
    else {const count=d.add(8).readS32();internal.count=count;
      internal.type3=count<0?{malformed:true}:count<=3?{indexAbsent:true}:list(pp(d.add(0x28)),LIMIT.filters,'internal-type3',p=>({pointer:str(p),module:moduleAt(p)}));} }
  const chain=[], seen=new Set(); let w=cp(o);
  while(w&&!w.isNull()&&chain.length<LIMIT.ancestors) {
    if(seen.has(str(w))) {chain.push({malformed:'cycle'});break;} seen.add(str(w));
    const d=priv(w), data=pp(w.add(0x20));
    if(!(d.add(0x20).readU32()&1)||!data.equals(d.add(0x148))) {chain.push({unavailable:'not initialized widget',object:str(w)});break;}
    const flags=data.add(12).readU32();chain.push({object:str(w),windowFlags:flags,contexts:gestureMap(pp(d.add(0x1c0)))});
    if(flags&1) {w=null;break;}w=pp(d.add(0x10));
  }
  if(w&&!w.isNull()&&chain.length>=LIMIT.ancestors)truncate('gesture-ancestors',chain.length+1,LIMIT.ancestors);
  const layout=pp(priv(o).add(0xa8));
  return {receiver:str(o),receiverFilters:filters(o),threadData:str(pp(priv(o).add(0x40))),
    layout:layout.isNull()?{object:str(layout)}:{object:str(layout),vtable:moduleAt(pp(layout)),flags:priv(layout).add(0x9c).readU32()},
    app:str(app),appClosing:c.add(0x2f41fd).readU8(),appThread:app.isNull()?null:str(pp(priv(app).add(0x40))),
    appFilters:app.isNull()?null:filters(app),gestureManager:app.isNull()?null:str(pp(priv(app).add(0x148))),
    internal,declarative:c.add(0x2f1da8).readU8(),parentHook:str(pp(c.add(0x2f4358))),gestures:chain};
}
function dispatcher(o){if(!o||o.isNull())return {object:str(o)};
 const vt=pp(o),wake=pp(vt.add(0x98)),loc=moduleAt(wake),d=priv(o),r={object:str(o),private:str(d),vtable:moduleAt(vt),wake:loc};
 
 const direct=loc&&loc.name.toLowerCase()==='qtcore4_64.dll'&&loc.rva==='0x141b70';
 const guiThunk=loc&&loc.name.toLowerCase()==='qtgui4_64.dll'&&loc.rva==='0x5e0ca0'&&
  hex(wake.readByteArray(6))==='ff2572690b00'&&pp(modules.gui.base.add(0x697618)).equals(modules.core.base.add(0x141b70));
 if((direct||guiThunk)&&!d.isNull())
  Object.assign(r,{win32:{window:str(pp(d.add(0x98))),serial:d.add(0xa8).readU32(),postedWake:d.add(0xb4).readU32()}});
 else r.specificFields='unqualified dispatcher type; not read';return r;}
function thread(d){if(!d||d.isNull())return {object:str(d),unavailable:'null threadData'};
 return {object:str(d),qthread:str(pp(d.add(8))),notifyDepth:d.add(0x1c).readU32(),canWait:d.add(0x50).readU8(),dispatcher:dispatcher(pp(d.add(0x20)))};}
function tid(d){return sid('thread','thread:'+str(d),()=>thread(d));}
function event(e){if(!e||e.isNull())return {object:str(e)};return {object:str(e),vtable:moduleAt(pp(e)),
 type:e.add(0x10).readU16(),flags:e.add(0x12).readU16(),child:e.add(0x10).readU16()===0x44?str(pp(e.add(0x18))):null};}
function queue(td){
 const q=td.add(0x30),d=pp(q),r={owner:str(q),threadData:str(td),data:str(d),rawOwnerFields:[8,12,16].map(n=>q.add(n).readS32())};
 if(d.isNull())return {...r,unavailable:'null QList data'};
 const begin=d.add(8).readS32(),end=d.add(12).readS32(),capacity=d.add(4).readS32();
 Object.assign(r,{refs:d.readS32(),capacity,begin,end});
 if(begin<0||end<begin||capacity<end)return {...r,malformed:'invalid list bounds'};
 const n=end-begin,order=[],newRecords=[];r.count=n;
 for(let i=0;i<Math.min(n,LIMIT.queueEntries);i++){
  const at=pp(d.add(0x18+8*(begin+i)));
  const desc=at.isNull()?{record:str(at),unavailable:'null record'}:{record:str(at),receiver:str(pp(at)),receiverRole:role(pp(at)),event:event(pp(at.add(8))),priority:at.add(0x10).readS32()};
  const k=JSON.stringify(desc);let alias=queueRecords.get(k);
  if(alias===undefined){if(queueRecords.size>=LIMIT.queueRecords){order.push(truncate('queue-records',queueRecords.size+1,LIMIT.queueRecords));break;}
   alias=++totals.queueRecords;queueRecords.set(k,alias);newRecords.push({alias,...desc});}
  order.push(alias);
 }
 if(newRecords.length)send({kind:'queue_records',records:newRecords});
 r.order=order;r.coverage=n>LIMIT.queueEntries?truncate('queue-entries',n,LIMIT.queueEntries):order.length===n?'complete':'incomplete records';return r;
}
function qid(td){return sid('post-queue','post-queue:'+str(td),()=>queue(td));}
function checked(m, rva) {
  const key = m.name.toLowerCase() + ':' + rva.toString(16), expected = MANIFEST[key];
  if (!expected) throw new Error('Missing immutable prologue: ' + key);
  const address = cp(m.base.add(rva)), actual = hex(address.readByteArray(expected.length / 2));
  if (actual !== expected) throw new Error('Prologue mismatch: ' + key);
  send({ kind: 'hook_verified', key, address: str(address), bytes: actual }); return address;
}
function attach(m, rva, callbacks) {
  const key = m.name.toLowerCase() + ':' + rva.toString(16);
  if (installed.has(key)) throw new Error('Duplicate hook ' + key);
  installed.set(key, true);
  const wrapped = {};
  for (const name of ['onEnter', 'onLeave']) if (callbacks[name]) wrapped[name] = function(...args) {
    if (done) return;
    try { callbacks[name].apply(this, args); }
    catch (e) { totals.callbackErrors++; read('callback:' + key + ':' + name, () => { throw e; }); }
  };
  // Hit statistics count selected records separately in totals; global native
  // invocation volume is deliberately not traced or represented as coverage.
  listeners.push(Interceptor.attach(checked(m, rva), wrapped));
}

function complete(){
 record('selected_editor_first_grid_published',{selected:editor?wid(editor):null,grid:gidGrid(),globals:gid(),mainReady,normalOpen,postOpen,setupEntered});
 done=true;send({kind:'capture_complete_paused',pid:Process.id,totals,limits:LIMIT,installedHooks:Array.from(installed.keys()),
 truncations:Array.from(truncations.values()),errorKinds:errors.size,
 openContexts:Array.from(stacks.entries()).filter(([,v])=>v.length).map(([key,v])=>({key,count:v.length})),
 qualification:'Native CLI Open to actual first editor grid UI publication, before spacing/margins; passive source-bound observations only. No full Save/Close comparison or GUI input.'});
 recv('coordinator_release').wait();
}

const observer=Process.attachModuleObserver({onAdded(m){
 const name=m.name.toLowerCase();if(!EXPECTED[name])return;
 send({kind:'module',name:m.name,path:m.path,base:str(m.base),size:m.size,
  expectedFileSha256:EXPECTED[name],loadedFileHashRequiresRootVerification:true});
 if(name==='def_gui_mo_64.exe'){
  modules.app=m;

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

  attach(m,0x377990,{onEnter(){if(!armed||setupEntered||!same(pp(cp(this.context.rsp).add(0x50)),editor))return;
   record('wrapper_qwidget_return',{widget:wid(editor)});}});
  for(const point of [0x377950,0x130740])attach(m,point,{onEnter(args){this.selected=armed&&!setupEntered&&same(args[0],editor);},
   onLeave(){if(this.selected)record('wrapper_constructor_return',{site:point,widget:wid(editor),
    initializedGuardStorage:read('wrapper-guard-storage',()=>({modelPointer:str(pp(editor.add(0x28))),modelType:editor.add(0x30).readS32(),enabled34:editor.add(0x34).readU8(),flag35:editor.add(0x35).readU8(),operationPointer:point===0x130740?str(pp(editor.add(0x38))):null,procedureType:point===0x130740?editor.add(0x40).readS32():null,modelRaw:hex(editor.add(0x28).readByteArray(16)),
     operationRaw:point===0x130740?hex(editor.add(0x38).readByteArray(16)):null}))});}});

  attach(m,0x2cf3c0,{onEnter(args){this.selected=armed&&!setupEntered&&same(args[1],m.base.add(0x21a8858));
   if(!this.selected)return;this.notifier=cp(args[0]);
   if(!same(this.notifier,m.base.add(0x3d76700)))throw new Error('Wrong selected notifier');
   record('wrapper_active_step_notifier_enter',{notifier:str(this.notifier),key:'2signalChangeActiveStepData()',
    suppressed:this.notifier.add(0x141).readU8(),flag90:this.notifier.add(0x90).readU8(),widget:wid(editor)});},
   onLeave(){if(this.selected)record('wrapper_active_step_notifier_return',{
    suppressed:this.notifier.add(0x141).readU8(),flag90:this.notifier.add(0x90).readU8(),widget:wid(editor)});}});
 }else if(name==='qtgui4_64.dll'){
  modules.gui=m;
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


  for(const point of [0x6e6e5,0x6e6ea,0x6e72b,0x6e75b])attach(m,point,{onEnter(){
   if(!current('init')||!same(this.context.rsi,editor))return;
   record('parentless_checkpoint',{site:point,globals:gid(),state:opacityState(),
    opacityDecision:point===0x6e72b?(this.context.rax.toInt32()&255):null});}});
  attach(m,0x64410,{onEnter(args){this.selected=!!current('init')&&same(args[0],editor);this.callsite=moduleAt(this.returnAddress);
   if(this.selected)record('parentless_palette_enter',{state:opacityState(),globals:gid(),actualReturnAddress:this.callsite});},onLeave(result){if(this.selected){const p=cp(result);
   record('parentless_palette_return',{palette:read('returned-palette',()=>palette(p)),state:opacityState()});}}});
  attach(m,0x49bf0,{onEnter(args){this.selected=!!current('init')&&this.returnAddress.equals(m.base.add(0x6e703));
   if(this.selected)record('parentless_brush_enter',{palette:str(cp(args[0])),group:args[1].toInt32(),role:args[2].toInt32()});},
   onLeave(result){if(this.selected){const b=cp(result);record('parentless_brush_return',{brush:str(b),
    value:read('brush-data',()=>({data:str(pp(b)),style:pp(b).add(4).readS32(),alphaHighByte:pp(b).add(0x0d).readU8(),colorRaw16:hex(pp(b).add(8).readByteArray(16))}))});}}});
  attach(m,0x11c090,{onEnter(args){this.selected=!!current('init')&&this.returnAddress.equals(m.base.add(0x6e721));},
   onLeave(result){if(this.selected)record('parentless_brush_isopaque',{result:(cp(result).toInt32()&255)!==0});}});
  attach(m,0x97940,{onEnter(args){this.selected=!!current('init')&&same(args[0],priv(editor));if(this.selected)record('opacity_effect_enter',{state:opacityState()});},
   onLeave(){if(this.selected)record('opacity_effect_return',{state:opacityState()});}});
  attach(m,0x3f700,{onEnter(args){this.selected=setupEntered&&same(args[1],editor)&&this.returnAddress.equals(modules.app.base.add(0xed99ab));
   if(!this.selected)return;if(grid)throw new Error('Second first-grid allocation');grid=cp(args[0]);
   record('first_grid_constructor_enter',{grid:str(grid),parent:str(editor),installedLayout:str(pp(priv(editor).add(0xa8))),uiGrid:str(pp(ui))});},
   onLeave(){if(this.selected)record('first_grid_constructor_return',{grid:gidGrid()});}});
  attach(m,0x46c73,{onEnter(){if(!gridPrivate||!same(this.context.r12,grid))return;
   record('first_grid_before_install',{grid:gidGrid()});}});

  attach(m,0x6e4d0,{onEnter(args){this.f=null;if(!armed||!editor||selectedTailComplete)return;
   const d=cp(args[0]);if(same(pp(d.add(8)),editor)){
    this.f=push('init',{private:d});record('widget_init_enter',{context:this.f?.id,private:str(d),widget:str(editor),parent:str(cp(args[1])),flags:args[2].readU32(),initialFont:read('initial-font',()=>font(d.add(0x180))),initialPalette:read('initial-palette',()=>palette(d.add(0x170))),privateD8:d.add(0x1d8).readU32()});}},
   onLeave(){const f=this.f;if(!f)return;try{record('widget_init_return',{context:f.id,widget:wid(editor),globals:gid(),thread:tid(pp(priv(editor).add(0x40)))});}finally{pop(f);}}});
  attach(m,0x1f2e30,{onEnter(args){this.f=null;if(!current('init')||!same(args[2],editor.add(0x10)))return;
   this.f=push('font',{output:cp(args[0]),source:cp(args[1]),device:cp(args[2])});if(!this.f)return;
   record('font_device_enter',{context:this.f.id,widget:wid(editor),source:read('font-source',()=>font(this.f.source)),device:str(this.f.device),globals:gid()});},
   onLeave(result){const f=this.f;if(!f)return;const returned=cp(result);try{record('font_device_return',{
    context:f.id,returned:str(returned),output:read('font-output',()=>font(f.output)),source:read('font-source-after',()=>font(f.source)),globals:gid()});}finally{pop(f);}}});
  attach(m,0x99110,{onEnter(args){this.f=null;if(!current('font')||args[1].toInt32()!==8)return;
   const device=cp(args[0]),owner=device.sub(0x10);this.f=push('metric',{device,owner});if(!this.f)return;
   record('metric_enter',{context:this.f.id,paintDevice:str(device),widget:wid(owner),globals:gid()});},
   onLeave(result){const f=this.f;if(!f)return;const returned=cp(result);try{record('metric_return',{
    context:f.id,result:returned.toInt32(),widget:wid(f.owner),globals:gid()});}finally{pop(f);}}});
  attach(m,0x99181,{onEnter(){const f=current('metric');if(!f)return;
   const actual=cp(this.context.rax),application=cp(this.context.rbx);
   record('metric_thread_comparison',{context:f.id,currentQThread:str(actual),applicationQThread:str(application),equal:actual.equals(application)});}});
  attach(m,0x7f9d0,{onEnter(){this.selected=!!current('metric');if(this.selected)record('dc_cache_enter',{metric:current('metric').id,globals:gid()});},
   onLeave(result){if(this.selected){const returned=cp(result);record('dc_cache_return',{returned:str(returned),globals:gid()});}}});
  attach(m,0x6e780,{onEnter(){if(current('init')&&same(this.context.rsi,editor))record('font_assigned',{widget:wid(editor),globals:gid()});}});
  attach(m,0x6e7ab,{onEnter(){if(current('init')&&same(this.context.rsi,editor))record('before_widget_counts',{widget:wid(editor),globals:gid()});}});
  attach(m,0x3b2470,{onEnter(args){this.selected=armed&&!selectedTailComplete&&same(args[0],editor);},
   onLeave(){if(this.selected)record('qframe_return',{widget:wid(editor),globals:gid()});}});
  attach(m,0x5e420,{onEnter(args){this.f=null;if(!armed||!editor||selectedTailComplete)return;const d=cp(args[0]);
   if(same(pp(d.add(8)),editor)){this.f=push('frame',{private:d,subElement:args[1].toInt32()});
    if(this.f)record('frame_margins_enter',{context:this.f.id,subElement:this.f.subElement,widget:wid(editor),globals:gid()});}},
   onLeave(){const f=this.f;if(!f)return;try{record('frame_margins_return',{context:f.id,widget:wid(editor),globals:gid()});}finally{pop(f);}}});
  attach(m,0x5e4b0,{onEnter(){const f=current('frame');if(!f)return;
   const style=cp(this.context.rax),option=cp(this.context.rdi);
   record('frame_style_call',{context:f.id,style:str(style),vtable:style.isNull()?null:moduleAt(pp(style)),
    target:style.isNull()?null:method(pp(style),0xb8),option:str(option),
    // Only QRect+10 is read:6505e487 initialized it for the null-option ctor path.
    optionRect:read('style-option-rect',()=>rect(option.add(0x10))),widget:wid(editor),globals:gid()});}});
  attach(m,0x5e4cd,{onEnter(){const f=current('frame');if(!f)return;const resultAddress=cp(this.context.rsp).add(0x38);
   record('frame_style_return',{context:f.id,rect:read('style-result-rect',()=>rect(resultAddress)),widget:wid(editor),globals:gid()});}});
  attach(m,0x6db20,{onEnter(args){this.selected=armed&&!selectedTailComplete&&same(args[0],editor);if(this.selected)record('size_policy_enter',{
   requested:args[1].toInt32(),widget:wid(editor)});},onLeave(){if(this.selected)record('size_policy_return',{widget:wid(editor)});}});
  attach(m,0x62460,{onEnter(args){this.f=null;if(!armed||!editor||selectedTailComplete)return;const d=cp(args[0]);
   if(same(pp(d.add(8)),editor)){this.f=push('geometry',{private:d});if(this.f){const parent=pp(d.add(0x10));
    record('geometry_update_enter',{context:this.f.id,force:args[1].toInt32()&255,widget:wid(editor),parent:parent.isNull()?null:wid(parent)});}}},
   onLeave(){const f=this.f;if(!f)return;try{record('geometry_update_return',{context:f.id,widget:wid(editor)});}finally{pop(f);}}});
  attach(m,0x403210,{onEnter(args){this.selected=armed&&!selectedTailComplete&&same(args[0],editor);},
   onLeave(){if(this.selected)record('qeditor_return',{widget:wid(editor),orientation:read('orientation',()=>priv(editor).add(0x218).readS32()),globals:gid()});}});
 }else if(name==='qtcore4_64.dll'){
  modules.core=m;

  attach(m,0x12e630,{onEnter(args){this.selected=same(args[0],editor)||same(args[0],grid);if(!this.selected)return;
   if(qobjectFrame)throw new Error('Nested selected QObject constructor');
   this.f={object:cp(args[0]),private:cp(args[1]),parent:cp(args[2]),tid:Process.getCurrentThreadId()};qobjectFrame=this.f;
   // Object vtable/data are not initialized at entry. Only the already-built private record is read.
   record('selected_qobject_enter',{object:str(this.f.object),private:str(this.f.private),parent:str(this.f.parent),
    counter:m.base.add(0x2f4344).readU32(),privateFlags:this.f.private.add(0x20).readU32(),
    gridPrivate:same(this.f.object,grid)?{rows:this.f.private.add(0xc0).readS32(),columns:this.f.private.add(0xc4).readS32(),
     layoutFlagsRaw:this.f.private.add(0x9c).readU32(),knownMask:15}:null});
  },onLeave(){if(!this.selected)return;record('selected_qobject_return',{object:str(this.f.object),
   counter:m.base.add(0x2f4344).readU32(),thread:tid(pp(this.f.private.add(0x40))),grid:gidGrid()});qobjectFrame=null;}});
  attach(m,0x12e68a,{onEnter(){if(!qobjectFrame||qobjectFrame.tid!==Process.getCurrentThreadId())return;if(same(qobjectFrame.object,grid))gridPrivate=cp(qobjectFrame.private);
   record('selected_qobject_thread_bound',{object:str(qobjectFrame.object),thread:tid(pp(qobjectFrame.private.add(0x40))),grid:gidGrid()});}});
  attach(m,0x12e6e0,{onEnter(){if(!qobjectFrame||qobjectFrame.tid!==Process.getCurrentThreadId())return;record('selected_qobject_before_counter',{
   object:str(qobjectFrame.object),counter:m.base.add(0x2f4344).readU32(),grid:gidGrid()});}});

  attach(m,0x2acc2,{onEnter(){const f=current('metric');if(!f)return;const td=cp(this.context.rax);
   record('metric_existing_tls',{context:f.id,nativeTlsGetValueResult:str(td),thread:td.isNull()?null:tid(td),
    qualification:'Natural TLS result; null retains adoption branch, not assumed absent.'});}});
  attach(m,0x119570,{onEnter(args){this.f=null;if((!current('init')&&!setupEntered)||!same(args[1],editor))return;
   const e=cp(args[2]);if(![0x0f,0xaa,0x44].includes(e.add(0x10).readU16()))return;
   this.f=push('create',{event:e,thread:pp(priv(editor).add(0x40))});if(!this.f)return;
   record('create_enter',{context:this.f.id,event:event(e),widget:wid(editor),thread:tid(this.f.thread),
    inputs:sid('event-inputs','notify-inputs',()=>eventInputs(editor)),globals:gid(),grid:gidGrid()});},
   onLeave(result){const f=this.f;if(!f)return;const returned=cp(result);try{record('create_return',{
    context:f.id,result:(returned.toUInt32()&255)!==0,event:event(f.event),widget:wid(editor),thread:tid(f.thread),
    inputs:sid('event-inputs','notify-return-inputs',()=>eventInputs(editor)),globals:gid(),grid:gidGrid()});}finally{pop(f);}}});
  attach(m,0x11ae90,{onEnter(args){this.f=null;if((!current('init')&&!setupEntered)||!same(args[0],editor))return;
   const e=cp(args[1]);if(![0x4a,0x4c].includes(e.add(0x10).readU16()))return;
   this.f=push('post',{event:e,thread:pp(priv(editor).add(0x40))});if(!this.f)return;
   record('post_enter',{context:this.f.id,event:event(e),priority:args[2].toInt32(),receiver:wid(editor),thread:tid(this.f.thread),grid:gidGrid()});},
   onLeave(){const f=this.f;if(!f)return;try{record('post_return',{context:f.id,event:event(f.event),receiver:wid(editor),thread:tid(f.thread),grid:gidGrid()});}finally{pop(f);}}});
  attach(m,0x11afc6,{onEnter(){const f=current('post');if(!f)return;const td=cp(this.context.rdi);
   record('post_before_insert_locked',{context:f.id,receiver:str(cp(this.context.r12)),event:event(cp(this.context.rsi)),thread:tid(td),queue:qid(td)});}});
  attach(m,0x11afe8,{onEnter(){const f=current('post');if(!f)return;const td=cp(this.context.rdi);
   record('post_after_insert_locked',{context:f.id,event:event(f.event),thread:tid(td),queue:qid(td)});}});
  attach(m,0x11b015,{onEnter(){const f=current('post');if(!f)return;const td=cp(this.context.rdi);
   record('post_before_wake',{context:f.id,event:event(f.event),receiver:wid(editor),thread:tid(td)});}});
  attach(m,0x141b70,{onEnter(args){this.selected=!!current('post');this.dispatcher=this.selected?cp(args[0]):null;
   if(this.selected)record('wake_enter',{post:current('post').id,state:sid('dispatcher','wake-enter',()=>dispatcher(this.dispatcher))});},
   onLeave(){if(this.selected)record('wake_return',{state:sid('dispatcher','wake-return',()=>dispatcher(this.dispatcher))});}});
 }
}});
let exceptionCount = 0;
Process.setExceptionHandler(details => {
  if (++exceptionCount > 8) {
    if (exceptionCount === 9) send({ kind: 'native_exception_truncated', pid: Process.id, limit: 8,
      mainReady, normalOpen, postOpen, setupEntered, qualification: 'Further native exceptions are omitted; original handling continues unchanged.' });
    return false;
  }
  const registers = {};
  for (const name of ['rip', 'rsp', 'rbp', 'rax', 'rbx', 'rcx', 'rdx', 'rsi', 'rdi', 'r8', 'r9', 'r10', 'r11', 'r12', 'r13', 'r14', 'r15']) registers[name] = str(details.context[name]);
  send({ kind: 'native_exception', pid: Process.id, tid: Process.getCurrentThreadId(), type: details.type,
    address: str(details.address), module: moduleAt(details.address), memory: details.memory, registers,
    stackRaw: read('exception stack', () => hex(details.context.rsp.readByteArray(256))),
    backtrace: read('exception backtrace', () => Thread.backtrace(details.context, Backtracer.ACCURATE).slice(0, 24).map(moduleAt)),
    mainReady, normalOpen, postOpen, setupEntered, editor: str(editor), operation: str(operation), project: str(project) });
  return false; // original exception handling/crash remains untouched
});

send({kind:'probe_ready',pid:Process.id,qualification:'Prepared native CLI editor base and first-grid observation; root review required before launch.'});
