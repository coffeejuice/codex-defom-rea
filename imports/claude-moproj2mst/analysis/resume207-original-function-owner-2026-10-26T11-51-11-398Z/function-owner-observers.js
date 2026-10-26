
  'use strict';
  const processCreationFiletime = "134374890722592589";
  let first = true;
  let sequence = 0;
  const hooks = [{"name":"owner","rva":962400,"bytes":"48894c2408534881ec60010000","callerRva":961146},{"name":"producer","rva":120080,"bytes":"48894c24084883ec28488b4c2430","callerRva":962560}];
  const eventKinds = ['owner-enter', 'producer-enter', 'producer-return'];
  const counts = Object.fromEntries(eventKinds.map(k => [k, 0]));
  const overflow = Object.fromEntries(eventKinds.map(k => [k, 0]));
  const rejected = { ownerCaller: 0, producerCaller: 0, noUniqueOwner: 0, ownerLeaveMismatch: 0 };
  let total = 0;
  let invocationSerial = 0;
  const activeByTid = new Map();
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
  function fields(objectO) {
    const holderH = guarded(asPtr(objectO), 0x30);
    const field = holderH.ok ? guarded(asPtr(holderH.value), 0x38) : { ok: false, error: 'holder-unreadable' };
    return { holderH, field, phase: 'pre-store-context' };
  }
  const app = Process.mainModule;
  if (!app || app.name.toLowerCase() !== 'def_gui_mo_64.exe' || app.path.toLowerCase() !== "c:\\program files\\sftc\\deform\\v11.0.2\\def_gui_mo_64.exe")
    throw new Error('Loaded original module identity mismatch');
  if (app.size !== 0x535f000) throw new Error('Loaded original image size mismatch');
  const actualHooks = [];
  for (const hook of hooks) {
    const address = app.base.add(hook.rva);
    const bytes = Array.from(new Uint8Array(address.readByteArray(hook.bytes.length / 2)), x => x.toString(16).padStart(2, '0')).join('');
    if (bytes !== hook.bytes) throw new Error('Hook instruction bytes mismatch at RVA ' + hook.rva.toString(16));
    actualHooks.push({ name: hook.name, rva: hook.rva, address: text(address), bytes, callerRva: hook.callerRva });
  }
  function emit(kind, body) {
    if (!first) return;
    if (counts[kind] >= 16 || total >= 48) { overflow[kind]++; return; }
    counts[kind]++; total++;
    send({ kind: 'hook', event: kind, sequence: ++sequence, pid: Process.id,
      processCreationFiletime, tid: Process.getCurrentThreadId(), ...body });
  }
  const owner = hooks[0], producer = hooks[1];
  Interceptor.attach(app.base.add(owner.rva), {
    onEnter() {
      try {
        this.ownerFrame = null;
        if (!this.returnAddress.equals(app.base.add(owner.callerRva))) { rejected.ownerCaller++; return; }
        const tid = Process.getCurrentThreadId();
        const stack = activeByTid.get(tid) || [];
        const frame = { invocationId: String(++invocationSerial), tid, objectO: text(this.context.rcx),
          returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp) };
        stack.push(frame); activeByTid.set(tid, stack); this.ownerFrame = frame;
        emit('owner-enter', frame);
      } catch (error) { send({ kind: 'hook-error', site: 'owner-enter', pid: Process.id, error: String(error).slice(0, 300) }); }
    },
    onLeave() {
      try {
        const frame = this.ownerFrame;
        if (!frame) return;
        const stack = activeByTid.get(frame.tid);
        if (!stack || stack.at(-1) !== frame) { rejected.ownerLeaveMismatch++; return; }
        stack.pop(); if (stack.length === 0) activeByTid.delete(frame.tid);
      } catch (error) { send({ kind: 'hook-error', site: 'owner-leave', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
  });
  Interceptor.attach(app.base.add(producer.rva), {
    onEnter() {
      try {
        this.producerFrame = null;
        if (!this.returnAddress.equals(app.base.add(producer.callerRva))) { rejected.producerCaller++; return; }
        const tid = Process.getCurrentThreadId(), stack = activeByTid.get(tid) || [];
        if (stack.length !== 1) { rejected.noUniqueOwner++; return; }
        const frame = stack[0];
        this.producerFrame = { invocationId: frame.invocationId, tid, objectO: frame.objectO,
          inputRcx: text(this.context.rcx), returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp) };
        emit('producer-enter', this.producerFrame);
      } catch (error) { send({ kind: 'hook-error', site: 'producer-enter', pid: Process.id, error: String(error).slice(0, 300) }); }
    },
    onLeave(retval) {
      try {
        const frame = this.producerFrame;
        if (!frame) return;
        const stack = activeByTid.get(frame.tid) || [];
        if (stack.length !== 1 || stack[0].invocationId !== frame.invocationId) { rejected.noUniqueOwner++; return; }
        emit('producer-return', { ...frame, producerP: text(retval), preStore: fields(frame.objectO) });
      } catch (error) { send({ kind: 'hook-error', site: 'producer-return', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
  });
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
          hookSnapshot: { sequence, counts: { ...counts }, overflow: { ...overflow }, rejected: { ...rejected } },
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
    registrationCallCompleted: true });
  