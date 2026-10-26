
  'use strict';
  const processCreationFiletime = "134374880095732594";
  let first = true;
  let sequence = 0;
  const hooks = [{"rva":962575,"bytes":"4c895838"},{"rva":962579,"bytes":"c744245400000000"},{"rva":963043,"bytes":"488b4938"},{"rva":963047,"bytes":"e804c3fbff"}];
  const counts = Object.fromEntries(hooks.map(h => [h.rva, 0]));
  const overflow = Object.fromEntries(hooks.map(h => [h.rva, 0]));
  let total = 0;
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
  if (!app || app.name.toLowerCase() !== 'def_gui_mo_64.exe' || app.path.toLowerCase() !== "c:\\program files\\sftc\\deform\\v11.0.2\\def_gui_mo_64.exe")
    throw new Error('Loaded original module identity mismatch');
  const actualHooks = [];
  for (const hook of hooks) {
    const address = app.base.add(hook.rva);
    const bytes = Array.from(new Uint8Array(address.readByteArray(hook.bytes.length / 2)), x => x.toString(16).padStart(2, '0')).join('');
    if (bytes !== hook.bytes) throw new Error('Hook instruction bytes mismatch at RVA ' + hook.rva.toString(16));
    actualHooks.push({ rva: hook.rva, address: text(address), bytes });
  }
  function observe(rva, context) {
    const seq = ++sequence;
    if (counts[rva] >= 16 || total >= 64) { overflow[rva]++; return; }
    counts[rva]++; total++;
    const s = context.rsp;
    const object = guarded(s, 0x170);
    const h = rva === 0xeb00f || rva === 0xeb013 ? context.rax : rva === 0xeb1e3 ? context.rcx : null;
    const holder = rva === 0xeb1e7 ? (object.ok ? guarded(asPtr(object.value), 0x30) : { ok: false, error: 'object-unreadable' }) : null;
    const field = rva === 0xeb1e7 ? (holder.ok ? guarded(asPtr(holder.value), 0x38) : { ok: false, error: 'holder-unreadable' }) : guarded(h, 0x38);
    send({ kind: 'hook', rva, utc: new Date().toISOString(), pid: Process.id, processCreationFiletime,
      tid: Process.getCurrentThreadId(), sequence: seq, frameS: text(s), objectO: object,
      holderH: rva === 0xeb1e7 ? holder : text(h),
      producerP: rva === 0xeb00f || rva === 0xeb013 ? text(context.r11) : null,
      consumedP: rva === 0xeb1e7 ? text(context.rcx) : null,
      indexEdx: rva === 0xeb1e3 || rva === 0xeb1e7 ? context.rdx.toUInt32() : null,
      fieldAddress: h ? text(h.add(0x38)) : holder.ok && asPtr(holder.value) ? text(asPtr(holder.value).add(0x38)) : null,
      fieldRead: field });
  }
  for (const hook of hooks) {
    Interceptor.attach(app.base.add(hook.rva), { onEnter() {
      try { observe(hook.rva, this.context); }
      catch (error) { send({ kind: 'hook-error', rva: hook.rva, pid: Process.id, error: String(error).slice(0, 300) }); }
    } });
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
        send({ kind: 'first-av-core', pid: Process.id, processCreationFiletime,
          exceptionType: details.type, rip: text(rip), rcx: text(context.rcx),
          exceptionAddress: text(details.address),
          memoryOperation, memoryOperationError,
          ripModule, ripModuleError,
          hookSnapshot: { sequence, counts: { ...counts }, overflow: { ...overflow } },
          disposition: 'return_false_continue_natural_exception' });
        let frames = [];
        try { frames = Thread.backtrace(context, Backtracer.ACCURATE).map(describe); }
        catch (e) { frames = [{ error: String(e) }]; }
        send({ kind: 'first-access-violation', utc: new Date().toISOString(), pid: Process.id,
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
  