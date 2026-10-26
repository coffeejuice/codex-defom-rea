"""Prepare passive probe003 from frozen helpers. Does not load or launch it."""
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from doe_static_pe import PE

prior = ROOT / 'build/original-splitter-probe-002/probe-reviewed-v2.js'
old = prior.read_text(encoding='utf-8')
assert hashlib.sha256(prior.read_bytes()).hexdigest() == '39bf80ba3928c626ca798743c945c4a94f2f08ee0ed76e3ee0ffa916b3af3dfc'
out = ROOT / 'build/original-splitter-probe-003/probe.js'
assert not out.exists()
out.parent.mkdir(parents=True, exist_ok=True)
hooks = {
    'DEF_GUI_MO_64.exe': [0xe9400, 0xd34730, 0xd35a80, 0xdab810],
    'QtGui4_64.dll': [0x70d70, 0x402b00, 0x6e4d0, 0x6dbd0, 0x611d0, 0x214d0,
                     0x1f3ca0, 0x68080, 0x20be0, 0x4a2f0, 0x64410, 0x5f940, 0x65ee0],
    'QtCore4_64.dll': [0x119570, 0x11ae90, 0x38f0]
}
manifest = {}
for name, rvas in hooks.items():
    pe = PE(ROOT / 'evidence/original-binaries' / name)
    for rva in rvas:
        offset = pe.off(pe.base + rva)
        manifest[name.lower()+':'+format(rva, 'x')] = pe.data[offset:offset+11].hex()

header = old[:old.index('const MANIFEST =')].replace('probe-002', 'probe-003')
header = header.replace('// Qt getter or event pump. Other NativeFunction calls are GetACP/GetOEMCP.',
                        '// Qt getter or event pump. Only the two process-local cwd functions are called.')
common = old[old.index('function cp('):old.index('function value(')]
attach = old[old.index('function checked('):old.index('function captureCodepages(')]
body = r'''
const LIMIT = { roles: 7, states: 160, records: 240, errors: 32, contexts: 7,
  resolvePerContext: 2, hashBuckets: 256, hashNodes: 64, keyBytes: 128,
  list: 64, filters: 16, ancestors: 16, gestures: 32, events: 32 };
const modules = {}, listeners = [], roles = new Map(), installed = new Map();
const states = new Map(), errors = new Map(), truncations = new Map(), contexts = new Map();
const totals = { records: 0, states: 0, failedReads: 0, callbackErrors: 0,
  parent: 0, font: 0, palette: 0, locale: 0, events: 0, fontResolve: 0,
  paletteResolve: 0, fontLookup: 0, paletteLookup: 0, widgetPalette: 0, opacity: 0 };
let mainView = null, manager = null, splitter = null, handleCount = 0, armed = false, done = false;
'''
body += common
body += r'''
function sid(type, where, fn) { return intern(type, read(where, fn)); }
function contextKey(kind) { return kind + ':' + Process.getCurrentThreadId(); }
function current(kind) { const s = contexts.get(contextKey(kind)); return s && s.length ? s[s.length-1] : null; }
function push(kind, r) {
  const count = ++totals[kind];
  if (count > LIMIT.contexts) { truncate(kind, count, LIMIT.contexts); return null; }
  const f = { kind, role: r, id: count, key: contextKey(kind), resolveCalls: 0, lookupCalls: 0 };
  let s = contexts.get(f.key); if (!s) { s = []; contexts.set(f.key, s); } s.push(f); return f;
}
function pop(f) {
  if (!f) return;
  const s = contexts.get(f.key);
  if (!s || s[s.length-1] !== f) throw new Error('Unbalanced selected context '+f.key);
  s.pop();
}
function qstring(address) {
  const data = pp(address);
  return { object: str(address), data: str(data), length: data.isNull() ? null : data.add(8).readS32() };
}
function value(address, kind, refs) {
  if (!address || address.isNull()) return { object: str(address), unavailable: 'null value object' };
  if (kind === 'locale') return { object: str(address), raw8: hex(address.readByteArray(8)) };
  const data = pp(address), result = { object: str(address), data: str(data), word8: address.add(8).readU32() };
  if (data.isNull()) result.unavailable = 'null shared data';
  else { if (refs) result.dataRefs = data.readS32(); if (kind === 'font') result.dataField38 = data.add(0x38).readU32(); }
  return result;
}
function byteArray(address) {
  const data = pp(address);
  if (data.isNull()) return { object: str(address), unavailable: 'null QByteArray data' };
  const length = data.add(8).readS32(), bytes = pp(data.add(0x10));
  if (length < 0 || (length && bytes.isNull())) return { data: str(data), length, malformed: true };
  const n = Math.min(length, LIMIT.keyBytes);
  return { object: str(address), data: str(data), buffer: str(bytes), length,
    bytesHex: n ? hex(bytes.readByteArray(n)) : '', coverage: length > n ? truncate('cache-key-bytes', length, n) : 'complete' };
}
function classCache(rva, kind) {
  const g = modules.gui.base, wrapper = pp(g.add(rva));
  const result = { wrapper: str(wrapper), destroyed: g.add(rva+8).readU8(),
    cleanupGuard: g.add(rva-8).readU32(), cleanupReference: str(pp(g.add(rva-16))), kind };
  if (wrapper.isNull()) return result;
  const data = pp(wrapper); result.data = str(data);
  if (data.isNull()) { result.unavailable = 'null hash data'; return result; }
  const size = data.add(0x14).readS32(), n = data.add(0x20).readS32(), buckets = pp(data.add(8));
  Object.assign(result, { size, bucketCount: n, buckets: str(buckets), entries: [] });
  if (size < 0 || n < 0 || (n && buckets.isNull())) { result.malformed = true; return result; }
  if (n > LIMIT.hashBuckets || size > LIMIT.hashNodes) {
    result.coverage = truncate('hash-'+kind, Math.max(n, size), n > LIMIT.hashBuckets ? LIMIT.hashBuckets : LIMIT.hashNodes); return result;
  }
  const seen = new Set();
  for (let b = 0; b < n; b++) {
    let node = pp(buckets.add(8*b));
    while (!node.equals(data)) {
      if (node.isNull() || seen.has(str(node)) || seen.size >= LIMIT.hashNodes) {
        result.malformed = 'null/cycle/duplicate/limit before sentinel'; result.coverage = 'incomplete'; return result;
      }
      seen.add(str(node));
      result.entries.push({ bucket: b, node: str(node), next: str(pp(node)), hash: node.add(8).readU32(),
        key: byteArray(node.add(0x10)), value: value(node.add(0x18), kind, false) });
      node = pp(node);
    }
  }
  result.countMatches = result.entries.length === size;
  result.coverage = result.countMatches ? 'complete' : 'count mismatch'; return result;
}
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
function pending(d) {
  const data = pp(d.add(0x68)); if (data.isNull()) return { data: str(data), unavailable: 'null vector data' };
  const count = data.add(8).readS32(), capacity = data.add(4).readS32(), entries = [];
  if (count < 0 || capacity < count) return { data: str(data), count, capacity, malformed: true };
  for (let i=0; i<Math.min(count,LIMIT.list); i++) { const at=data.add(0x10+16*i);
    entries.push({ control: str(pp(at)), child: str(pp(at.add(8))) }); }
  return { data: str(data), count, capacity, entries, coverage: count > LIMIT.list ? truncate('pending',count,LIMIT.list) : 'complete' };
}
function widget(o, includePending) {
  if (!o || o.isNull()) return { object: str(o), unavailable: 'null widget' };
  const d = priv(o), data = pp(o.add(0x20));
  if (d.isNull() || !data.equals(d.add(0x148))) return { object: str(o), private: str(d), unavailable: 'QWidgetData not initialized/bound' };
  const e = pp(d.add(0x88)), vt = pp(o), dynamicMeta = pp(d.add(0x28));
  const result = { object: str(o), private: str(d), role: role(o)?.name || null, vtable: moduleAt(vt),
    dynamicMeta: str(dynamicMeta), parent: str(pp(d.add(0x10))), qobjectFlags: d.add(0x20).readU32(),
    threadData: str(pp(d.add(0x40))), hookData: str(pp(d.add(0x78))), widgetData: str(data),
    attributesLow: data.add(8).readU32(), windowFlags: data.add(12).readU32(),
    extended: [0x1c8,0x1cc,0x1d0].map(x=>d.add(x).readU32()), internalFlags1d8: d.add(0x1d8).readU32(),
    graphicsEffect: str(pp(d.add(0xd8))), fieldD0: str(pp(d.add(0xd0))),
    fontInherited: d.add(0x128).readU32(), paletteInherited: d.add(0x12c).readU32(),
    font: value(d.add(0x180),'font',false), palette: value(d.add(0x170),'palette',false), locale: value(d.add(0x1a8),'locale',false),
    extra: e.isNull() ? { pointer: str(e) } : { pointer: str(e), flags5c: e.add(0x5c).readU32(),
      topExtra: str(pp(e.add(8))), proxy: str(pp(e.add(0x10))), style: str(pp(e.add(0x20))), stylesheet: qstring(e.add(0x38)) } };
  if (includePending) result.pending = pending(d);
  return result;
}
function widgetId(o, includePending=false) { return sid('widget','widget:'+str(o),()=>widget(o,includePending)); }
function ancestors(o) {
  const chain=[], seen=new Set();
  while (o && !o.isNull() && chain.length<LIMIT.ancestors) {
    if (seen.has(str(o))) return { chain, malformed: 'ancestor cycle' }; seen.add(str(o));
    const d=priv(o);
    if (d.isNull() || !(d.add(0x20).readU32()&1)) return { chain, unavailable: 'non-widget ancestor', object: str(o) };
    chain.push(widgetId(o)); o=pp(d.add(0x10));
  }
  return { chain, coverage: o && !o.isNull() ? truncate('ancestors',chain.length+1,LIMIT.ancestors) : 'complete to null' };
}
function cacheState() {
  if(!modules.gui||!modules.core)return {unavailable:'Qt modules not both available'};
  const g=modules.gui.base, c=modules.core.base, app=pp(c.add(0x2f4220)),
    f=pp(g.add(0x9c96b8)), p=pp(g.add(0x9c9690));
  return { fontDefault: value(f,'font',false), paletteDefault: value(p,'palette',false),
    fontMutex: str(pp(g.add(0x9c9768))), fontMutexDestroyed: g.add(0x9c9770).readU8(),
    fontClasses: sid('font-classes','font-classes',()=>classCache(0x9c97b0,'font')),
    paletteClasses: sid('palette-classes','palette-classes',()=>classCache(0x9c9790,'palette')),
    applicationStylesheet: qstring(g.add(0x9c9738)), modalState: g.add(0x9cae3c).readU8(),
    modalListOwner: str(pp(g.add(0x9c9718))), activeWindow: str(pp(g.add(0x9c96f0))),
    application: str(app), applicationThread: app.isNull()?null:str(pp(priv(app).add(0x40))),
    applicationType:g.add(0x9c9670).readS32(),compileVersion:c.add(0x2f1da4).readS32(),
    applicationAttributes:c.add(0x2f41f8).readU32() };
}
function cacheId() { return sid('application-values','application-values',cacheState); }
function gestureMap(data) {
  if (data.isNull()) return { data:str(data), unavailable:'null gesture data' };
  const size=data.add(0x70).readS32(), entries=[], seen=new Set(); let node=pp(data.add(8));
  if(size<0) return {data:str(data),size,malformed:true};
  while(!node.equals(data) && entries.length<LIMIT.gestures) {
    if(node.isNull()||seen.has(str(node))) return {data:str(data),size,entries,malformed:'null/cycle'};
    seen.add(str(node)); entries.push({node:str(node),key:node.sub(8).readS32(),flags:node.sub(4).readS32()}); node=pp(node.add(8));
  }
  return {data:str(data),size,entries,countMatches:node.equals(data)&&entries.length===size,
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
function eventEnter(args, posted, caller) {
  const f=current('parent');if(!f)return null;
  const o=cp(args[posted?0:1]), e=cp(args[posted?1:2]), type=e.add(0x10).readU16();
  const child=[0x44,0x45].includes(type)?pp(e.add(0x18)):null;
  if(posted ? !(type===0x43&&same(o,f.newParent)) :
      !((type===0x15&&same(o,f.role.object))||([0x44,0x45].includes(type)&&same(o,f.newParent)&&same(child,f.role.object))))return null;
  if(++totals.events>LIMIT.events){truncate('events',totals.events,LIMIT.events);return null;}
  const r={id:totals.events,receiver:o,posted};record(posted?'selected_post':'selected_event_enter',{
    id:r.id,parentContext:f.id,type,receiver:str(o),child:str(child),caller:moduleAt(caller),
    widget:widgetId(o,true),inputs:sid('event-inputs','event-inputs',()=>eventInputs(o))});return r;
}
'''
body += attach
body += r'''
function complete() {
  record('view_default_return',{roles:Array.from(roles.values()).filter(r=>r.qobjectReady).map(r=>({role:r.name,widget:widgetId(r.object)})),caches:cacheId()});
  done=true;send({kind:'capture_complete_paused',pid:Process.id,totals,limits:LIMIT,
    installedHooks:Array.from(installed.keys()),truncations:Array.from(truncations.values()),errorKinds:errors.size,
    openContexts:Array.from(contexts.entries()).filter(([,s])=>s.length).map(([key,s])=>({key,count:s.length})),
    qualification:'Passive selected setParent/value/cache inputs only. No full mainwindow children, Qt getter calls, event pump, GUI input or Primary extension.'});
  recv('coordinator_release').wait();
}
const observer=Process.attachModuleObserver({onAdded(m){
  const name=m.name.toLowerCase();if(!EXPECTED[name])return;
  send({kind:'module',name:m.name,path:m.path,base:str(m.base),size:m.size,
    expectedFileSha256:EXPECTED[name],loadedFileHashRequiresRootVerification:true});
  if(name==='def_gui_mo_64.exe'){
    modules.app=m;
    attach(m,0xe9400,{onEnter(args){if(!mainView){mainView=cp(args[0]);track(mainView,'mainView');}}});
    attach(m,0xd34730,{onEnter(args){this.r=null;if(!manager&&same(args[1],mainView)){manager=cp(args[0]);this.r=track(manager,'manager');}},
      onLeave(){if(this.r)this.r.qobjectReady=this.r.widgetReady=true;}});
    attach(m,0xd35a80,{onEnter(args){this.selected=same(args[0],manager)&&this.returnAddress.equals(m.base.add(0xe9c42));if(this.selected)armed=true;},
      onLeave(){if(this.selected)complete();}});
    attach(m,0xdab810,{onEnter(args){if(armed&&!splitter&&same(args[2],mainView)){splitter=cp(args[0]);track(splitter,'splitter');}}});
  }else if(name==='qtgui4_64.dll'){
    modules.gui=m;
    attach(m,0x70d70,{onEnter(args){if(!armed||!splitter||!modules.app)return;
      if(this.returnAddress.equals(modules.app.base.add(0xdab952)))track(args[0],'placeholder0');
      else if(this.returnAddress.equals(modules.app.base.add(0xdab9e0)))track(args[0],'placeholder1');}});
    attach(m,0x402b00,{onEnter(args){if(armed&&same(args[2],splitter)&&handleCount<2)track(args[0],'handle'+handleCount++);}});
    attach(m,0x6e4d0,{onEnter(args){this.r=privateRole(cp(args[0]));if(this.r)this.r.qobjectReady=true;},
      onLeave(){if(this.r)this.r.widgetReady=true;}});
    attach(m,0x6dbd0,{onEnter(args){this.f=null;const r=role(args[0]);if(!armed||!target(r))return;
      const f=push('parent',r);if(!f)return;this.f=f;f.newParent=cp(args[1]);r.widgetReady=true;
      record('parent_enter',{context:f.id,widget:widgetId(r.object),newParent:str(f.newParent),parent:widgetId(f.newParent,true),
        oldAncestors:read('old-ancestors',()=>ancestors(r.object)),newAncestors:read('new-ancestors',()=>ancestors(f.newParent)),caches:cacheId()});},
      onLeave(){const f=this.f;if(!f)return;record('parent_return',{context:f.id,widget:widgetId(f.role.object),parent:widgetId(f.newParent,true),
        ancestors:read('parent-return-ancestors',()=>ancestors(f.role.object)),caches:cacheId()});pop(f);}});
    for(const [kind,rva]of [['font',0x611d0],['palette',0x68080],['locale',0x5f940]])attach(m,rva,{
      onEnter(args){this.f=null;const p=current('parent'),r=privateRole(cp(args[0]));if(!p||r!==p.role)return;
        this.f=push(kind,r);if(this.f)record(kind+'_enter',{context:this.f.id,parentContext:p.id,widget:widgetId(r.object),parent:widgetId(p.newParent),caches:cacheId()});},
      onLeave(){const f=this.f;if(!f)return;record(kind+'_return',{context:f.id,widget:widgetId(f.role.object),caches:cacheId()});pop(f);}});
    for(const [kind,rva,returnRva]of [['font',0x214d0,0x5f38a],['palette',0x20be0,0x644cb]])attach(m,rva,{
      onEnter(args){this.f=null;const f=current(kind);if(!f||!this.returnAddress.equals(m.base.add(returnRva)))return;
        if(++f.lookupCalls>1){truncate(kind+'-lookup-per-context',f.lookupCalls,1);return;}this.f=f;this.output=cp(args[0]);
        record(kind+'_lookup_enter',{context:f.id,receiver:widgetId(cp(args[1])),output:str(this.output),caches:cacheId()});},
      onLeave(result){if(!this.f)return;totals[this.f.kind+'Lookup']++;const resultCopy=cp(result);
        record(this.f.kind+'_lookup_return',{context:this.f.id,returned:str(resultCopy),output:value(this.output,this.f.kind,true),caches:cacheId()});}});
    for(const [kind,rva]of [['font',0x1f3ca0],['palette',0x4a2f0]])attach(m,rva,{
      onEnter(args){this.f=null;const f=current(kind);if(!f)return;
        if(++f.resolveCalls>LIMIT.resolvePerContext){truncate(kind+'-resolve-per-context',f.resolveCalls,LIMIT.resolvePerContext);return;}
        this.f=f;this.output=cp(args[1]);this.call=f.resolveCalls;
        record(kind+'_resolve_enter',{context:f.id,call:this.call,caller:moduleAt(cp(this.returnAddress)),
          current:value(cp(args[0]),kind,true),other:value(cp(args[2]),kind,true),output:str(this.output)});},
      onLeave(result){if(!this.f)return;totals[this.f.kind+'Resolve']++;const resultCopy=cp(result);
        record(this.f.kind+'_resolve_return',{context:this.f.id,call:this.call,returned:str(resultCopy),value:value(this.output,this.f.kind,true)});}});
    attach(m,0x64410,{onEnter(args){this.o=null;const f=current('parent');if(!f)return;
      const caller=cp(this.returnAddress);if(![0x645d5,0x64577,0x65f55,0x65fb7].some(r=>caller.equals(m.base.add(r))))return;
      if(++totals.widgetPalette>21){truncate('widget-palette',totals.widgetPalette,21);return;}
      this.o=cp(args[0]);this.parentContext=f.id;record('widget_palette_enter',{parentContext:f.id,caller:moduleAt(caller),widget:widgetId(this.o),caches:cacheId()});},
      onLeave(result){if(!this.o)return;const resultCopy=cp(result);record('widget_palette_return',{
        parentContext:this.parentContext,widget:widgetId(this.o),returned:str(resultCopy),value:value(resultCopy,'palette',true)});}});
    attach(m,0x65ee0,{onEnter(args){this.f=null;const f=current('parent');if(!f||!this.returnAddress.equals(m.base.add(0x6e219)))return;
      this.f=f;totals.opacity++;record('opacity_enter',{parentContext:f.id,ancestors:read('opacity-ancestors',()=>ancestors(f.role.object)),caches:cacheId()});},
      onLeave(){if(this.f)record('opacity_return',{parentContext:this.f.id,ancestors:read('opacity-ancestors-after',()=>ancestors(this.f.role.object))});}});
  }else{
    modules.core=m;
    attach(m,0x119570,{onEnter(args){this.event=eventEnter(args,false,cp(this.returnAddress));},
      onLeave(result){if(!this.event)return;record('selected_event_return',{id:this.event.id,nativeReturnAL:result.toInt32()&255,
        widget:widgetId(this.event.receiver,true),inputs:sid('event-inputs','event-inputs-return',()=>eventInputs(this.event.receiver))});}});
    attach(m,0x11ae90,{onEnter(args){eventEnter(args,true,cp(this.returnAddress));}});
    attach(m,0x38f0,{onEnter(args){this.f=null;const f=current('locale');if(!f||!modules.gui||!this.returnAddress.equals(modules.gui.base.add(0x5f9c5)))return;
      this.f=f;this.left=value(cp(args[0]),'locale');this.right=value(cp(args[1]),'locale');},
      onLeave(result){if(this.f)record('locale_native_equality',{context:this.f.id,left:this.left,right:this.right,nativeReturnAL:result.toInt32()&255});}});
  }
}});
send({kind:'probe_ready',pid:Process.id,platform:Process.platform,arch:Process.arch,
  expectedModuleHashes:EXPECTED,limits:LIMIT,installedHooks:Array.from(installed.keys()),
  qualification:'Prepared passive cache/value and selected-parent tail capture. Owned cwd verified. Root owns launch, loaded hashes and cleanup.'});
'''
script = header + 'const MANIFEST = ' + json.dumps(manifest, indent=2) + ';\n' + body
out.write_text(script, encoding='utf-8')
print(json.dumps({'script':out.relative_to(ROOT).as_posix(),'sha256':hashlib.sha256(out.read_bytes()).hexdigest(),
                  'bytes':len(out.read_bytes()),'checked_hooks':len(manifest)}))
