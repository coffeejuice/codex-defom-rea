
  'use strict';
  const hooks=[["DEF_GUI_MO_64.exe",87204,"requested_visibility","ff5058488d0d0a12d603e80d632b000fb6c085c0745d4533"],["DEF_GUI_MO_64.exe",240016,"slot_entry","8854241048894c240856574881ecd8010000c64424600048"],["DEF_GUI_MO_64.exe",240262,"busy_return","884424620fb644246285c07407c644246301eb3e488b8c24"],["DEF_GUI_MO_64.exe",240290,"auto_save_call","e8d960fdff884424640fb644246485c0750dc78424c00100"],["QtGui4_64.dll",3800992,"dock_event","405355565741544881eca000000048c7442440feffffff4c"],["QtGui4_64.dll",3801220,"show_signal_call","e877d8ffffe946020000488bcee88ae2ffffe93902000032"]], expectedSizes={"DEF_GUI_MO_64.exe":87420928,"QtGui4_64.dll":10887168,"QtCore4_64.dll":3272704}, expectedDirectory="C:\\Program Files\\SFTC\\DEFORM\\v11.0.2";
  const max=512, windowMs=15000;
  let seq=0, observations=0, ended=false, timer=null, observer=null, slotSerial=0, armed=false, continuity=true;
  const start=Date.now(), listeners=[], installed=new Set(), modules=new Map(), stacks=new Map(), readySeq=new Map();
  const names=['DEF_GUI_MO_64.exe','QtGui4_64.dll','QtCore4_64.dll'];
  const canonicalName=name=>names.find(n=>n.toLowerCase()===name.toLowerCase());
  function coverage(){return{installed:[...installed],modules:[...modules.keys()],readySeq:Object.fromEntries(readySeq),continuity};}
  function preReady(){return observer!==null&&modules.has('DEF_GUI_MO_64.exe')&&hooks.filter(h=>h[0]==='DEF_GUI_MO_64.exe').every(h=>installed.has(h[2]));}
  function emit(kind,data={}) {
    const record={kind,seq:++seq,ms:Date.now()-start,tid:Process.getCurrentThreadId(),...data};
    if(JSON.stringify(record).length>1024) {finish('record_size_exceeded'); return;}
    send(record);
  }
  function finish(reason,detail=null) {
    if(ended)return; ended=true; if(timer!==null)clearTimeout(timer);
    const detachErrors=[];let observerStopped=false;
    for(const listener of listeners)try{listener.detach();}catch(e){detachErrors.push(String(e).slice(0,60));}
    if(observer)try{observer.detach();observerStopped=true;}catch(e){detachErrors.push(String(e).slice(0,60));}
    send({kind:'terminal',seq:++seq,ms:Date.now()-start,reason,detail,observations,detachErrors,observerStopped,...coverage()});
  }
  function record(kind,data) {
    if(ended||!armed)return;
    if(observations>=max){finish('budget_exhausted');return;}
    observations++;emit(kind,{...data,hookReadySeq:readySeq.get(kind)??null});
  }
  function top(tid){const s=stacks.get(tid);return s&&s.length?s[s.length-1]:null;}
  function safe(fn){try{return fn();}catch(e){finish('read_or_callback_error',String(e).slice(0,180));return null;}}
  function install(module) {
    const name=canonicalName(module.name);
    if(ended||!name)return;
    try {
      if(modules.has(name)){continuity=false;emit('duplicate_module',{name,path:module.path,base:String(module.base),size:module.size});throw new Error('duplicate_or_substituted_module:'+name);}
      const expected=(expectedDirectory+'\\'+name).toLowerCase();
      if(module.path.toLowerCase()!==expected)throw new Error('module_path_mismatch:'+name);
      if(module.size!==expectedSizes[name])throw new Error('module_size_mismatch:'+name);
      const own=hooks.filter(h=>h[0]===name);
      const prefixes=own.map(h=>({rva:'0x'+h[1].toString(16),bytes:Array.from(new Uint8Array(module.base.add(h[1]).readByteArray(24))).map(b=>b.toString(16).padStart(2,'0')).join('')}));
      // Compare EVERY prefix in the module before Interceptor changes any of them.
      for(let i=0;i<own.length;i++)if(prefixes[i].bytes!==own[i][3])throw new Error('prefix_mismatch:'+own[i][2]);
      if(name==='QtCore4_64.dll') {
        const q=Array.from(new Uint8Array(module.base.add(0x11730).readByteArray(5))).map(b=>b.toString(16).padStart(2,'0')).join('');
        if(q!=='0fb74110c3')throw new Error('QEvent_type_prefix_mismatch');
      }
      modules.set(name,{name,path:module.path,base:String(module.base),size:module.size,prefixes});
      emit('module_identity',modules.get(name));
      for(const h of own){
        const kind=h[2];
        const callbacks={onEnter(){safe(()=>{
          const c=this.context,tid=Process.getCurrentThreadId();
          const active=top(tid),common={slotId:active?active.id:null};
          if(kind==='requested_visibility')record(kind,{dock:String(c.rcx),requested:c.rdx.toUInt32()&255,...common});
          if(kind==='dock_event'){if(!modules.has('QtCore4_64.dll')){record('unqualified_dock_event',{dock:String(c.rcx),event:String(c.rdx),reason:'QtCore_type_bytes_not_yet_qualified'});finish('event_type_unqualified');return;}const type=c.rdx.add(0x10).readU16();record(kind,{dock:String(c.rcx),event:String(c.rdx),type,typeLabel:type===17?'Show':type===18?'Hide':'other',...common});}
          if(kind==='show_signal_call')record(kind,{dock:String(c.rcx),emitted:c.rdx.toUInt32()&255,...common});
          if(kind==='slot_entry'){
            const dock=c.rcx.add(0x420).readPointer(),s=stacks.get(tid)||[];
            this.slotId=++slotSerial;s.push({id:this.slotId,main:String(c.rcx),dock:String(dock)});stacks.set(tid,s);
            record(kind,{main:String(c.rcx),dock:String(dock),value:c.rdx.toUInt32()&255,slotId:this.slotId,parentSlotId:common.slotId});
          }
          if(kind==='busy_return')record(kind,{busy:c.rax.toUInt32()&255,...common});
          if(kind==='auto_save_call')record(kind,{main:String(c.rcx),...common});
        });}};
        if(kind==='slot_entry')callbacks.onLeave=function(){safe(()=>{const tid=Process.getCurrentThreadId(),s=stacks.get(tid)||[];if(!s.length||s.at(-1).id!==this.slotId)throw new Error('slot_stack_mismatch');s.pop();});};
        listeners.push(Interceptor.attach(module.base.add(h[1]),callbacks));installed.add(kind);
      }
      const next=seq+1;for(const h of own)readySeq.set(h[2],next);
      emit('module_ready',{name,path:module.path,base:String(module.base),size:module.size,hooks:own.map(h=>h[2]),armed});
    }catch(e){finish('hook_error',String(e).slice(0,180));}
  }
  function removed(module){const name=canonicalName(module.name);if(ended||!name)return;continuity=false;emit('module_removed',{name,path:module.path,base:String(module.base),size:module.size});for(const h of hooks.filter(h=>h[0]===name))installed.delete(h[2]);finish('required_module_removed');}
  rpc.exports={
    status(){return{pid:Process.id,platform:Process.platform,arch:Process.arch,ended,installed:[...installed],modules:[...modules.values()],observerInstalled:observer!==null,preResumeReady:preReady(),continuity};},
    arm(){if(ended||!preReady()||!continuity)throw new Error('required_EXE_and_observer_pre_resume_readiness_missing');armed=true;timer=setTimeout(()=>finish('startup_window_elapsed'),windowMs);emit('armed',{pid:Process.id});return true;},
    stop(){finish('external_stop');}
  };
  if(Process.platform!=='windows'||Process.arch!=='x64')finish('unsupported_platform');
  else if(typeof Process.attachModuleObserver!=='function')finish('module_observer_unavailable');
  else {observer=Process.attachModuleObserver({onAdded:install,onRemoved:removed});if(ended)observer.detach();else emit('observer_ready',{pid:Process.id});}
  