
  'use strict';
  const processCreationFiletime = "134374951243032343";
  let first = true;
  let sequence = 0;
  const hooks = [{"name":"outer","rva":4691,"bytes":"488954241048894c24084881ecd8010000","callerRva":4576},{"name":"duplicate","rva":4784,"bytes":"488954241048894c24084881ecb8000000","callerRva":4713},{"name":"factory","rva":4832,"bytes":"488954241048894c24084883ec48","callerRva":4723},{"name":"cast","rva":4876,"bytes":"48895c241048897424185741544155415641574883ec60","callerRva":4733},{"name":"project","rva":4948,"bytes":"48894c24084881ec8800000048c7442448feffffff","callerRva":4743},{"name":"acceptance","rva":5002,"bytes":"488954241048894c24084881ec68010000","callerRva":4760},{"name":"postopen","rva":5050,"bytes":"488954241048894c240856574881ec88040000","callerRva":4669},{"name":"current","rva":5130,"bytes":"48894c24084883ec48","callerRva":5093},{"name":"activation","rva":5162,"bytes":"8954241048894c24084883ec38","callerRva":5108},{"name":"viewbind","rva":5201,"bytes":"8854241048894c24084881ecc8000000","callerRva":5125},{"name":"viewaccess","rva":5266,"bytes":"48894c24084883ec28","callerRva":5243},{"name":"modelbind","rva":5287,"bytes":"488954241048894c2408","callerRva":5258}];
  const eventKinds = ["outer-enter","duplicate-return","factory-return","cast-return","project-return","acceptance-enter","acceptance-return","outer-return","postopen-enter","current-enter","current-return","activation-enter","activation-return","postopen-return","viewbind-enter","viewcurrent-enter","viewcurrent-return","viewaccess-enter","viewaccess-return","modelbind-enter","modelbind-return","viewbind-return"];
  const counts = Object.fromEntries(eventKinds.map(k => [k, 0]));
  const overflow = Object.fromEntries(eventKinds.map(k => [k, 0]));
  const rejected = { outerCaller: 0, helperCaller: 0, noUniqueOwner: 0, outerLeaveMismatch: 0, order: 0, overlap: 0 };
  let total = 0;
  let invocationSerial = 0;
  const activeByTid = new Map();
  let boundedStateInvalid = false;
  const text = value => { try { return value === undefined || value === null ? null : String(value); } catch (_) { return '<unprintable>'; } };
  const read = address => {
    try { return { ok: true, value: text(address.readPointer()) }; }
    catch (error) { return { ok: false, error: String(error).slice(0, 160) }; }
  };
  const asPtr = value => { try { return value === null || value === undefined ? null : ptr(value); } catch (_) { return null; } };
  function guarded(base, offset) {
    try { return base === null ? { ok: false, error: 'base-unreadable' } : read(base.add(offset)); }
    catch (error) { return { ok: false, error: String(error).slice(0, 160) }; }
  }
  const app = Process.mainModule;
  if (!app || app.name.toLowerCase() !== 'fixture.exe' || app.path.toLowerCase() !== "c:\\users\\alext\\projects\\codex-deform-rea\\imports\\claude-moproj2mst\\scratch\\cp215v01\\fixture.exe")
    throw new Error('Loaded original module identity mismatch');
  if (app.size !== 135168) throw new Error('Loaded original image size mismatch');
  // Qualified against exact QtCore getters; never calls into the target.
  const qt = Process.mainModule;
  // Synthetic fixture has no loaded QtCore; metadata qualification remains covered by actual-agent mocks.
  const hexBytes = bytes => Array.from(new Uint8Array(bytes), x => x.toString(16).padStart(2, '0')).join('');
  const qtGetters = [];
  for (const getter of qtGetters) if (hexBytes(qt.base.add(getter.rva).readByteArray(getter.bytes.length/2)) !== getter.bytes) throw new Error('Loaded QtCore getter bytes mismatch');
  function readable(address, bytes) {
    if (!Number.isSafeInteger(bytes) || bytes < 0 || bytes > 256 || address.isNull()) throw new Error('invalid read bounds');
    const end = address.add(bytes);
    if (end.compare(address) < 0) throw new Error('pointer wrap');
    let cursor = address, ranges = 0;
    while (cursor.compare(end) < 0) {
      const range = Process.findRangeByAddress(cursor);
      if (++ranges > 256 || !range || !range.protection.includes('r')) throw new Error('unreadable range');
      const next = range.base.add(range.size);
      if (next.compare(cursor) <= 0) throw new Error('invalid mapped range');
      cursor = next.compare(end) < 0 ? next : end;
    }
    const buffer = address.readByteArray(bytes);
    if (!buffer || buffer.byteLength !== bytes) throw new Error('partial range read');
    return buffer;
  }
  function qstring(object) {
    const out = {object: text(object), ok: false, units:null, text:null, rawReadStatus: 'unread', recheckStatus:'not_reached', truncated: false, allocationValidity:'structural-and-readable-only'};
    try {
      const objectBuffer = readable(object, 8), objectBefore = hexBytes(objectBuffer);
      out.objectBytes = objectBefore;
      if (ptr(object).and(7).toUInt32() !== 0) throw new Error('unaligned QString object');
      const d = ptr('0x'+new DataView(objectBuffer).getBigUint64(0,true).toString(16)); out.header = text(d);
      if (d.and(7).toUInt32() !== 0) throw new Error('unaligned QString header');
      const headerBuffer = readable(d, 26), view = new DataView(headerBuffer);
      const before = hexBytes(headerBuffer); out.headerBytes = before;
      out.refcount = view.getInt32(0,true); out.capacity = view.getInt32(4,true); out.length = view.getInt32(8,true);
      out.data = '0x'+view.getBigUint64(16,true).toString(16);
      out.storageKind = ptr(out.data).equals(d.add(26)) ? 'inline' : 'external';
      out.flags = view.getUint16(24,true);
      if (ptr(out.data).isNull()) throw new Error('null QString data');
      if (ptr(out.data).and(1).toUInt32() !== 0) throw new Error('unaligned QString data');
      if (out.refcount <= 0 || out.capacity < 0 || out.capacity < out.length || out.length < 0 || out.length > 128) {out.truncated = out.length > 128; throw new Error('invalid or excessive QString length/refcount/capacity');}
      const bytes = out.length ? new Uint8Array(readable(ptr(out.data), out.length * 2)) : new Uint8Array(0);
      if (bytes.length !== out.length * 2) throw new Error('partial content read');
      out.units = [];
      for (let i = 0; i < bytes.length; i += 2) out.units.push(bytes[i] | bytes[i + 1] << 8);
      out.text = String.fromCharCode(...out.units);
      out.utf16leHex = hexBytes(bytes.buffer);
      out.headerAfter = hexBytes(readable(d, 26));
      out.objectAfter = hexBytes(readable(object, 8));
      out.recheckStatus = out.headerAfter === before && out.objectAfter === objectBefore ? 'matched' : 'mismatch';
      if (out.recheckStatus !== 'matched') throw new Error('QString changed during snapshot');
      out.ok = true; out.rawReadStatus = 'exact_length_stable_reread';
    } catch (error) {out.units=null; out.text=null; delete out.utf16leHex; out.error = String(error).slice(0, 180); out.rawReadStatus = 'failed';}
    return out;
  }

  const actualHooks = [];
  for (const hook of hooks) {
    const address = app.base.add(hook.rva);
    const bytes = Array.from(new Uint8Array(address.readByteArray(hook.bytes.length / 2)), x => x.toString(16).padStart(2, '0')).join('');
    if (bytes !== hook.bytes) throw new Error('Hook instruction bytes mismatch at RVA ' + hook.rva.toString(16));
    actualHooks.push({ name: hook.name, rva: hook.rva, address: text(address), bytes, callerRva: hook.callerRva });
  }
  function emit(kind, body) {
    if (!first) return;
    if (counts[kind] >= 8 || total >= 176) { overflow[kind]++; return; }
    counts[kind]++; total++;
    send({ kind: 'hook', event: kind, sequence: ++sequence, pid: Process.id,
      processCreationFiletime, tid: Process.getCurrentThreadId(), ...body });
  }
  const outer = hooks[0];
  let lastOuter = null;
  const steps = hooks.slice(1,6).map(h => h.name);
  // Keep all accepted frames, including nested ones, so they can never silently
  // overwrite attribution. Any overlap invalidates causal qualification.
  Interceptor.attach(app.base.add(outer.rva), {
    onEnter() {
      try {
        this.outerFrame = null;
        if (!first) return;
        const tid = Process.getCurrentThreadId(), stack = activeByTid.get(tid) || [];
        // Every outer entry can reenter the selected body, regardless of caller.
        if (stack.length) { rejected.overlap++; for (const f of stack) f.invalid = true; }
        if (!this.returnAddress.equals(app.base.add(outer.callerRva))) { rejected.outerCaller++; return; }
        if (activeByTid.size >= 16 && !activeByTid.has(tid) || stack.length >= 8 ||
          Array.from(activeByTid.values()).reduce((n, frames) => n + frames.length, 0) >= 64) {
          boundedStateInvalid = true; for (const frames of activeByTid.values()) for (const f of frames) f.invalid = true; return;
        }
        const frame = { invocationId: String(++invocationSerial), tid, main: text(this.context.rcx),
          pathQString: text(this.context.rdx), entryRsp: text(this.context.rsp),
          returnAddress: text(this.returnAddress), next: 0, stopped: false, invalid: stack.length !== 0 };
        stack.push(frame); activeByTid.set(tid, stack); this.outerFrame = frame;
        emit('outer-enter', { ...frame, pathWitness: qstring(ptr(frame.pathQString)) });
      } catch (error) { send({ kind: 'hook-error', site: 'outer-enter', pid: Process.id, error: String(error).slice(0, 300) }); }
    },
    onLeave(retval) {
      try {
        const frame = this.outerFrame; if (!frame) return;
        const stack = activeByTid.get(frame.tid) || [];
        if (stack.at(-1) !== frame) { rejected.outerLeaveMismatch++; return; }
        emit('outer-return', { ...frame, rawReturn: text(retval), normalReturn: true, pathWitness: qstring(ptr(frame.pathQString)), hookSnapshot: {boundedStateInvalid, sequence: sequence + 1, counts: {...counts, 'outer-return': counts['outer-return'] + 1}, overflow: {...overflow}, rejected: {...rejected}} });
        lastOuter = {...frame, rawReturn:text(retval), outerSequence:sequence};
        stack.pop(); if (!stack.length) activeByTid.delete(frame.tid);
      } catch (error) { send({ kind: 'hook-error', site: 'outer-return', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
  });
  for (const hook of hooks.slice(1,6)) Interceptor.attach(app.base.add(hook.rva), {
    onEnter() {
      try {
        this.selectedCall = null;
        if (!this.returnAddress.equals(app.base.add(hook.callerRva))) { rejected.helperCaller++; return; }
        const tid = Process.getCurrentThreadId(), stack = activeByTid.get(tid) || [];
        if (stack.length !== 1) { rejected.noUniqueOwner++; return; }
        const frame = stack[0];
        if (frame.invalid || frame.stopped || steps[frame.next] !== hook.name || frame.pending) {
          frame.invalid = true; rejected.order++; return;
        }
        // Outer prologue subtracts 0x1d8 without pushes; CALL adds 8.
        if (!this.context.rsp.equals(ptr(frame.entryRsp).sub(0x1e0))) { frame.invalid = true; rejected.order++; return; }
        if (hook.name === 'acceptance') emit('acceptance-enter', {invocationId: frame.invocationId, main: frame.main, returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp), rawRCX: text(this.context.rcx), rawRDX: text(this.context.rdx), pathWitness: qstring(this.context.rdx)});
        frame.pending = hook.name;
        this.selectedCall = { frame, rawRCX:text(this.context.rcx), returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp) };
      } catch (error) { send({ kind: 'hook-error', site: hook.name + '-enter', pid: Process.id, error: String(error).slice(0, 300) }); }
    },
    onLeave(retval) {
      try {
        const call = this.selectedCall; if (!call) return;
        const frame = call.frame, stack = activeByTid.get(frame.tid) || [];
        if (stack.length !== 1 || stack[0] !== frame || frame.pending !== hook.name || frame.invalid) {
          rejected.noUniqueOwner++; frame.invalid = true; return;
        }
        const rawReturn = text(retval), lowByte = retval.toUInt32() & 255;
        const failed = hook.name === 'duplicate' ? !retval.isNull() : hook.name === 'acceptance' ? lowByte === 0 : retval.isNull();
        frame.next++; frame.pending = null; frame.stopped = failed;
        emit(hook.name + '-return', { invocationId: frame.invocationId, main: frame.main,
          returnAddress: call.returnAddress, entryRsp: call.entryRsp, rawReturn, lowByte, failed, normalReturn: true, ...(hook.name === 'acceptance' ? {pathWitness: qstring(ptr(frame.pathQString)),parserWitness: projectByte(ptr(call.rawRCX))} : {}) });
      } catch (error) { send({ kind: 'hook-error', site: hook.name + '-return', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
  });
  let postFrame = null, postSerial = 0, postStateInvalid = false;
  const postRejected = {caller:0, helperCaller:0, overlap:0, order:0, identity:0, frame:0};
  const postHooks = hooks.slice(6,9);
  function postSnapshot(nextKind) {return {viewStateInvalid,viewSerial,viewRejected:{...viewRejected},postStateInvalid,postSerial,postRejected:{...postRejected},boundedStateInvalid,sequence:sequence+1,
    counts:{...counts,[nextKind]:counts[nextKind]+1},overflow:{...overflow},rejected:{...rejected}};}
  Interceptor.attach(app.base.add(postHooks[0].rva), {
    onEnter() {
      this.post = null;
      try {
        // Poison the selected frame before caller filtering on any nested postOpen.
        if(postFrame) {postFrame.invalid=true;postStateInvalid=true;postRejected.overlap++;}
        if(!this.returnAddress.equals(app.base.add(postHooks[0].callerRva))) {postRejected.caller++;return;}
        if(++postSerial!==1 || !lastOuter || invocationSerial!==1 || activeByTid.size!==0 || postFrame || lastOuter.invalid || ptr(lastOuter.rawReturn).isNull() || Process.getCurrentThreadId()!==lastOuter.tid || !this.context.rcx.equals(ptr(lastOuter.main)) || !this.context.rdx.equals(ptr(lastOuter.rawReturn))) {postStateInvalid=true;postRejected.identity++;send({kind:'hook-error',site:'postopen-identity',pid:Process.id,error:'duplicate or mismatched postOpen'});return;}
        postFrame={invocationId:lastOuter.invocationId,postId:String(postSerial),tid:Process.getCurrentThreadId(),main:text(this.context.rcx),project:text(this.context.rdx),entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),next:0,pending:null,invalid:false};
        this.post=postFrame;emit('postopen-enter',{...postFrame,outerSequence:lastOuter.outerSequence});
      }catch(error){send({kind:'hook-error',site:'postopen-enter',pid:Process.id,error:String(error)});}
    },
    onLeave() {
      try {const frame=this.post;if(!frame)return;
        if(postFrame!==frame || frame.pending) {frame.invalid=true;postRejected.frame++;}
        emit('postopen-return',{...frame,normalReturn:true,voidReturn:true,hookSnapshot:postSnapshot('postopen-return')});
        if(postFrame===frame)postFrame=null;
      }catch(error){send({kind:'hook-error',site:'postopen-return',pid:Process.id,error:String(error)});}
    }
  });
  for(const hook of postHooks.slice(1)) Interceptor.attach(app.base.add(hook.rva), {
    onEnter() {
      this.postCall=null;this.viewCurrentCall=null;
      try {
        if(hook.name==='current' && this.returnAddress.equals(app.base.add(5222))) { viewCurrentEnter.call(this); return; }
        if(postFrame?.pending===hook.name) {postFrame.invalid=true;postStateInvalid=true;postRejected.overlap++;}
        if(!this.returnAddress.equals(app.base.add(hook.callerRva))) {postRejected.helperCaller++;return;}
        const f=postFrame;
        if(!f || f.invalid || f.pending || Process.getCurrentThreadId()!==f.tid || (hook.name==='current'?f.next!==0:f.next!==1) || !this.context.rsp.equals(ptr(f.entryRsp).sub(0x4e0))) {if(f)f.invalid=true;postStateInvalid=true;postRejected.order++;return;}
        f.pending=hook.name;
        this.postCall={frame:f,returnAddress:text(this.returnAddress),entryRsp:text(this.context.rsp),rawRCX:text(this.context.rcx),rawRDX:text(this.context.rdx),rawEDX:this.context.rdx.toUInt32()};
        emit(hook.name+'-enter',{invocationId:f.invocationId,postId:f.postId,...this.postCall,frame:undefined});
      }catch(error){send({kind:'hook-error',site:hook.name+'-enter',pid:Process.id,error:String(error)});}
    },
    onLeave(retval) {
      try {if(this.viewCurrentCall){viewCurrentLeave.call(this,retval);return;} const call=this.postCall;if(!call)return;const f=call.frame;
        if(postFrame!==f || f.invalid || f.pending!==hook.name) {f.invalid=true;postStateInvalid=true;postRejected.frame++;return;}
        f.next++;f.pending=null;
        if(hook.name==='current')f.current=text(retval);
        emit(hook.name+'-return',{invocationId:f.invocationId,postId:f.postId,...call,frame:undefined,normalReturn:true,...(hook.name==='current'?{rawReturn:text(retval)}:{voidReturn:true})});
      }catch(error){send({kind:'hook-error',site:hook.name+'-return',pid:Process.id,error:String(error)});}
    }
  });

  // Reads only at live normal-return/entry boundaries, never after child cleanup.
  function projectByte(project) {
    const out={project:text(project),address:null,ok:false,stable:false,rawByte:null,first:null,second:null};
    try {
      const address=project.add(0x55);out.address=text(address);
      if(project.isNull()||address.compare(project)<0)throw Error('invalid Project field address');
      out.first=new Uint8Array(readable(address,1))[0];out.second=new Uint8Array(readable(address,1))[0];
      out.stable=out.first===out.second;
      if(!out.stable)throw Error('Project status changed during read');
      out.rawByte=out.first;out.ok=true;
    }catch(error){out.error=String(error);}
    return out;
  }
  function receiverSlot(receiver) {
    const out={receiver:text(receiver),ok:false,stable:false,vtable:null,slot:null,target:null};
    try {
      if(receiver.isNull()||receiver.and(7).toUInt32()!==0)throw Error('invalid view receiver');
      const object=readable(receiver,8),vtable=ptr('0x'+new DataView(object).getBigUint64(0,true).toString(16));out.vtable=text(vtable);
      const slot=vtable.add(0x1b8);out.slot=text(slot);
      if(vtable.isNull()||vtable.and(7).toUInt32()!==0||slot.compare(vtable)<0)throw Error('invalid vtable');
      const target=readable(slot,8);out.target='0x'+new DataView(target).getBigUint64(0,true).toString(16);
      out.stable=hexBytes(object)===hexBytes(readable(receiver,8))&&hexBytes(target)===hexBytes(readable(slot,8));
      if(!out.stable)throw Error('view receiver slot changed during read');
      out.matchesConcreteTarget=ptr(out.target).equals(app.base.add(5287));out.ok=true;
    }catch(error){out.error=String(error);}
    return out;
  }
  let viewFrame=null,viewSerial=0,viewStateInvalid=false;
  const viewRejected={caller:0,helperCaller:0,overlap:0,identity:0,order:0,frame:0};
  const viewHooks=hooks.slice(9);
  function viewEvent(kind,call,extra={}){const f=viewFrame;emit(kind,{invocationId:f.invocationId,postId:f.postId,viewId:f.viewId,...call,frame:undefined,...extra});}
  function viewCurrentEnter(){
    this.viewCurrentCall=null;
    if(viewFrame?.pending==='viewcurrent'){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
    const f=viewFrame;
    if(!f||f.invalid||f.pending||f.next!==0||Process.getCurrentThreadId()!==f.tid||!this.context.rsp.equals(ptr(f.entryRsp).sub(0xd0))){if(f)f.invalid=true;viewStateInvalid=true;viewRejected.order++;return;}
    f.pending='viewcurrent';this.viewCurrentCall={frame:f,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),rawRCX:text(this.context.rcx)};
    viewEvent('viewcurrent-enter',this.viewCurrentCall);
  }
  function viewCurrentLeave(retval){
    const c=this.viewCurrentCall,f=c.frame;
    if(viewFrame!==f||f.invalid||f.pending!=='viewcurrent'){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;return;}
    f.pending=null;f.next=1;f.current=text(retval);viewEvent('viewcurrent-return',c,{normalReturn:true,rawReturn:text(retval)});
  }
  Interceptor.attach(app.base.add(viewHooks[0].rva),{
    onEnter(){this.view=null;try{
      if(viewFrame){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
      if(!this.returnAddress.equals(app.base.add(viewHooks[0].callerRva))){viewRejected.caller++;return;}
      const p=postFrame;
      if(++viewSerial!==1||viewFrame||!p||p.invalid||p.pending||p.next!==2||Process.getCurrentThreadId()!==p.tid||!this.context.rcx.equals(ptr(p.main))||!this.context.rsp.equals(ptr(p.entryRsp).sub(0x4e0))){viewStateInvalid=true;viewRejected.identity++;send({kind:'hook-error',site:'viewbind-identity',pid:Process.id,error:'duplicate/mismatched viewbind'});return;}
      viewFrame={invocationId:p.invocationId,postId:p.postId,viewId:String(viewSerial),tid:p.tid,main:text(this.context.rcx),project:p.project,rawRDX:text(this.context.rdx),rawDL:this.context.rdx.toUInt32()&255,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),next:0,pending:null,invalid:false};
      this.view=viewFrame;viewEvent('viewbind-enter',{...viewFrame});
    }catch(error){send({kind:'hook-error',site:'viewbind-enter',pid:Process.id,error:String(error)});}},
    onLeave(){try{const f=this.view;if(!f)return;if(viewFrame!==f||f.pending){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;}viewEvent('viewbind-return',{...f},{normalReturn:true,voidReturn:true});if(viewFrame===f)viewFrame=null;}catch(error){send({kind:'hook-error',site:'viewbind-return',pid:Process.id,error:String(error)});}}
  });
  for(const hook of viewHooks.slice(1))Interceptor.attach(app.base.add(hook.rva),{
    onEnter(){this.viewCall=null;try{
      if(viewFrame?.pending===hook.name){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
      if(!this.returnAddress.equals(app.base.add(hook.callerRva))){viewRejected.helperCaller++;return;}
      const f=viewFrame;
      if(!f||f.invalid||f.pending||Process.getCurrentThreadId()!==f.tid||(hook.name==='viewaccess'?f.next!==1:f.next!==2)||!this.context.rsp.equals(ptr(f.entryRsp).sub(0xd0))){if(f)f.invalid=true;viewStateInvalid=true;viewRejected.order++;return;}
      f.pending=hook.name;this.viewCall={frame:f,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),rawRCX:text(this.context.rcx),rawRDX:text(this.context.rdx)};
      viewEvent(hook.name+'-enter',this.viewCall,hook.name==='modelbind'?{receiverSlot:receiverSlot(this.context.rcx)}:{});
    }catch(error){send({kind:'hook-error',site:hook.name+'-enter',pid:Process.id,error:String(error)});}},
    onLeave(retval){try{const c=this.viewCall;if(!c)return;const f=c.frame;
      if(viewFrame!==f||f.invalid||f.pending!==hook.name){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;return;}
      f.next++;f.pending=null;if(hook.name==='viewaccess')f.view=text(retval);
      viewEvent(hook.name+'-return',c,{normalReturn:true,...(hook.name==='viewaccess'?{rawReturn:text(retval)}:{voidReturn:true,receiverSlot:receiverSlot(ptr(c.rawRCX))})});
    }catch(error){send({kind:'hook-error',site:hook.name+'-return',pid:Process.id,error:String(error)});}}
  });

  // Installation gate runs while the exact owned child remains suspended.
  // Only the observed five-byte redirect mode is eligible for a first trial.
  Interceptor.flush();
  const patchReceipts = [];
  let patchGatePassed = true;
  for (const hook of hooks) {
    const address = app.base.add(hook.rva);
    const receipt = { name: hook.name, rva: hook.rva, address: text(address), before: hook.bytes };
    try {
      const buffer = address.readByteArray(hook.bytes.length / 2);
      const raw = new Uint8Array(buffer);
      receipt.after = Array.from(raw, x => x.toString(16).padStart(2, '0')).join('');
      receipt.redirectKind = raw[0] === 0xe9 ? 'e9-rel32' : 'unsupported';
      receipt.redirectSize = raw[0] === 0xe9 ? 5 : null;
      receipt.displacedLength = (hook.name === 'activation' || hook.name === 'viewbind') ? 9 : 5;
      receipt.padding = (hook.name === 'activation' || hook.name === 'viewbind') ? receipt.after.slice(10,18) : '';
      receipt.suffixUnchanged = receipt.after.slice(receipt.displacedLength*2) === hook.bytes.slice(receipt.displacedLength*2);
      if (raw[0] === 0xe9) {
        receipt.displacement = new DataView(buffer).getInt32(1, true);
        receipt.redirectDestination = text(address.add(5 + receipt.displacement));
      }
      receipt.gatePassed = receipt.redirectKind === 'e9-rel32' && receipt.redirectSize === 5 && receipt.suffixUnchanged && ((hook.name !== 'activation' && hook.name !== 'viewbind') || receipt.padding === '0f1f4000');
    } catch (error) { receipt.gatePassed = false; receipt.error = String(error).slice(0, 300); }
    if (!receipt.gatePassed) patchGatePassed = false;
    patchReceipts.push(receipt);
  }
  if (!patchGatePassed) {
    send({ kind: 'install-readiness-failure', pid: Process.id, processCreationFiletime,
      reason: 'requires_e9_rel32_strict5_or_activation_exact9_nop_window', patches: patchReceipts });
    throw new Error('Installed hook redirect failed pre-resume five-byte gate');
  }
  function describe(address) {
    const module = address ? Process.findModuleByAddress(address) : null;
    let symbol = null;
    try { symbol = DebugSymbol.fromAddress(address).toString(); } catch (_) {}
    return { address: text(address), symbol, module: module ? { name: module.name, path: module.path, base: text(module.base), size: module.size, offset: text(address.sub(module.base)) } : null };
  }
  function plainRegisters(context) {
    const out = {};
    const names = new Set(['rip','rsp','rbp','rax','rbx','rcx','rdx','rsi','rdi','r8','r9','r10','r11','r12','r13','r14','r15','eflags','rflags', ...Object.keys(context)]);
    for (const name of names) {
      try { const value = context[name]; if (value && typeof value.toString === 'function') out[name] = value.toString(); } catch (_) {}
    }
    return out;
  }
  Process.setExceptionHandler(details => {
    try {
      if (first && details && details.type === 'access-violation') {
        first = false;
        const context = details.context || {};
        const rip = context.rip || context.pc || null;
        let ripModule = null, ripModuleError = null;
        try {
          const module = rip ? Process.findModuleByAddress(rip) : null;
          if (module) ripModule = { name: module.name, path: module.path, base: text(module.base), offset: text(rip.sub(module.base)) };
        } catch (error) { ripModuleError = String(error).slice(0, 160); }
        let memoryOperation = null, memoryOperationError = null;
        try {
          if (details.memory) memoryOperation = { operation: text(details.memory.operation), address: text(details.memory.address) };
        } catch (error) { memoryOperationError = String(error).slice(0, 160); }
        const tid = Process.getCurrentThreadId();
        const activeInvocations = (activeByTid.get(tid) || []).map(frame => frame.invocationId);
        send({ kind: 'first-av-core', pid: Process.id, processCreationFiletime, tid, activeInvocations,
          exceptionType: details.type, rip: text(rip), rcx: text(context.rcx),
          exceptionAddress: text(details.address),
          memoryOperation, memoryOperationError,
          ripModule, ripModuleError,
          hookSnapshot: { boundedStateInvalid, sequence, counts: { ...counts }, overflow: { ...overflow }, rejected: { ...rejected } },
          disposition: 'return_false_continue_natural_exception' });
        let frames = [];
        try { frames = Thread.backtrace(context, Backtracer.ACCURATE).map(describe); }
        catch (e) { frames = [{ error: String(e) }]; }
        send({ kind: 'first-access-violation', utc: new Date().toISOString(), pid: Process.id, processCreationFiletime, tid,
          exceptionType: details.type, exceptionAddress: describe(details.address),
          rip: describe(rip), faultingInstruction: describe(rip), registers: plainRegisters(context), accurateBacktrace: frames,
          memoryOperation: details.memory ? { operation: text(details.memory.operation), address: text(details.memory.address) } : null,
          disposition: 'return_false_continue_natural_exception' });
      }
    } catch (e) {
      send({ kind: 'exception-handler-error', pid: Process.id, error: String(e).slice(0, 1200), disposition: 'return_false_continue_natural_exception' });
    }
    return false;
  });
  send({ kind: 'handler-ready', pid: Process.id, platform: Process.platform, arch: Process.arch,
    module: { name: app.name, path: app.path, base: text(app.base), size: app.size }, hooks: actualHooks,
    qtLoadedIdentity: {path: qt.path, base:text(qt.base), size:qt.size, boundedMetadataAndGetterChecks:true}, registrationCallCompleted: true, patchGatePassed, patches: patchReceipts });
  