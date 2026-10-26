// PREPARED ONLY. Keep the false guard until the exact one-run probe is reviewed.
// Owns one exact B98 converter child through owned_suspended_process.py.
// Prepared six-entry contingency; normal-return scalars only, plus unchanged first-AV observer.
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
import { spawn, execFileSync } from 'node:child_process';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const EXE = 'C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\DEF_GUI_MO_64.exe';
const CWD = 'C:\\PROBLEM\\p213a';
const PROJECT = path.join(CWD, 'Gear_blank.moproj');
const SOURCE = path.join(ROOT, 'evidence', 'gui_mo', 'oracle_b', '98_B_DS3F_before');
const PYTHON = 'C:\\Program Files\\Python312\\python.exe';
const HELPER = path.join(ROOT, 'admin', 'owned_suspended_process.py');
const FRIDA_URL = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const EXPECTED = {
  exe: 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
  project: 'e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd',
  fixtureManifest: '55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd',
  helper: '8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d',
};
// Launch requires this constant AND the exact CLI argument below. Keep false.
const ORIGINAL_RUN_REVIEWED = false;
const OBSERVATION_MS = 5000;
const QSTRING_LAYOUT_REVIEWED = false;
const QT_PATH = path.join(path.dirname(EXE), 'QtCore4_64.dll');
const QT_SHA = 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160';
const CLI_ARG = '0043003a005c00500052004f0042004c0045004d005c00700032003100330061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a';
const HELPER_MAX_SECONDS = 20;
// Prefixes read directly from pinned PE; relocation/patchability NOT qualified.
const HOOK_BYTES_REVIEWED = false;
const HOOKS = [
  { name: 'outer', rva: 0x1ca30, bytes: '488954241048894c24084881ecd8010000', callerRva: 0x1c94 },
  { name: 'duplicate', rva: 0xc78c10, bytes: '488954241048894c24084881ecb8000000', callerRva: 0x1cf11 },
  { name: 'factory', rva: 0xc79390, bytes: '488954241048894c24084883ec48', callerRva: 0x1d08a },
  { name: 'cast', rva: 0x146f8fc, bytes: '48895c241048897424185741544155415641574883ec60', callerRva: 0x1d0bd },
  { name: 'project', rva: 0x44a90, bytes: '48894c24084881ec8800000048c7442448feffffff', callerRva: 0x1d0d8 },
  { name: 'acceptance', rva: 0x8a9b0, bytes: '488954241048894c24084881ec68010000', callerRva: 0x1d0fb },
];
const EVENT_KINDS = ['outer-enter', 'duplicate-return', 'factory-return', 'cast-return', 'project-return', 'acceptance-enter', 'acceptance-return', 'outer-return'];
const EVENT_CAP_PER_KIND = 8;
const EVENT_CAP_TOTAL = 64;

const sha = bytes => crypto.createHash('sha256').update(bytes).digest('hex');
const fileSha = filename => sha(fs.readFileSync(filename));
const canonical = filename => fs.realpathSync.native(filename).toLowerCase();
function requireThat(ok, message) { if (!ok) throw new Error(message); }
function bounded(promise, ms, label) {
  let timer;
  return Promise.race([
    promise,
    new Promise((_, reject) => { timer = setTimeout(() => reject(new Error(`${label} timed out; outcome uncertain`)), ms); }),
  ]).finally(() => clearTimeout(timer));
}
function save(out, name, value) {
  fs.writeFileSync(path.join(out, name), JSON.stringify(value, null, 2) + '\n', { flag: 'wx' });
}

// Deterministic recursive hash manifest. Reject links and paths at/over the
// established DEFORM test-path bound; report all file names, lengths and hashes.
function manifest(directory, enforceShortPaths = false) {
  const base = fs.realpathSync.native(directory);
  const entries = [];
  function walk(dir) {
    const ds = fs.lstatSync(dir);
    requireThat(!ds.isSymbolicLink(), `Symlink directory rejected: ${dir}`);
    for (const name of fs.readdirSync(dir).sort()) {
      const item = path.join(dir, name), st = fs.lstatSync(item);
      requireThat(!st.isSymbolicLink(), `Symlink entry rejected: ${item}`);
      if (enforceShortPaths) requireThat(item.length < 100, `Path length is not below 100: ${item}`);
      requireThat(canonical(item).startsWith(base.toLowerCase() + path.sep), `Manifest escape: ${item}`);
      if (st.isDirectory()) {
        entries.push({ kind: 'directory', path: path.relative(base, item).replaceAll('\\', '/') });
        walk(item);
      } else {
        requireThat(st.isFile(), `Non-regular entry rejected: ${item}`);
        const bytes = fs.readFileSync(item), after = fs.statSync(item);
        requireThat(st.size === after.size && st.mtimeMs === after.mtimeMs, `File changed while hashing: ${item}`);
        entries.push({ kind: 'file', path: path.relative(base, item).replaceAll('\\', '/'), bytes: String(st.size), sha256: sha(bytes) });
      }
    }
  }
  walk(base);
  entries.sort((a, b) => a.path < b.path ? -1 : a.path > b.path ? 1 : 0);
  return { root: base, entries, contentDigest: sha(JSON.stringify(entries)) };
}
function windowsPresence(pid) {
  requireThat(Number.isInteger(pid) && pid > 0, 'Invalid owned PID for independent Windows check');
  const command = `$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' -ErrorAction Stop | Select-Object ProcessId,CreationDate,ExecutablePath,CommandLine); [pscustomobject]@{schema='owned-pid-presence-v1';querySucceeded=$true;pid=${pid};present=[bool]($rows.Count -gt 0);processes=@($rows)} | ConvertTo-Json -Depth 4 -Compress`;
  const raw = execFileSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', command], { encoding: 'utf8', windowsHide: true, timeout: 5000 }).trim();
  const value = JSON.parse(raw);
  requireThat(value?.schema === 'owned-pid-presence-v1' && value.querySucceeded === true && value.pid === pid && typeof value.present === 'boolean' && Array.isArray(value.processes), 'Invalid Windows presence receipt');
  requireThat(value.processes.length <= 1 && value.processes.every(row => row.ProcessId === pid) && value.present === (value.processes.length === 1), 'Contradictory Windows presence receipt');
  return { raw, value };
}
function injectedSource(creationFiletimeExact) {
  // These are function callbacks. A producer result exists only on normal onLeave.
  return `
  'use strict';
  const processCreationFiletime = ${JSON.stringify(creationFiletimeExact)};
  let first = true;
  let sequence = 0;
  const hooks = ${JSON.stringify(HOOKS)};
  const eventKinds = ${JSON.stringify(EVENT_KINDS)};
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
  if (!app || app.name.toLowerCase() !== 'def_gui_mo_64.exe' || app.path.toLowerCase() !== ${JSON.stringify(EXE.toLowerCase())})
    throw new Error('Loaded original module identity mismatch');
  if (app.size !== 0x535f000) throw new Error('Loaded original image size mismatch');
  // Qualified against exact QtCore getters; never calls into the target.
  const qt = Process.findModuleByName('QtCore4_64.dll');
  if (!qt || qt.path.toLowerCase() !== ${JSON.stringify(QT_PATH.toLowerCase())}) throw new Error('Loaded QtCore identity mismatch');
  const qtMatches = Process.enumerateModules().filter(m => m.name.toLowerCase() === 'qtcore4_64.dll');
  if (qtMatches.length !== 1 || !qtMatches[0].base.equals(qt.base) || qt.size !== 0x31f000) throw new Error('Loaded QtCore uniqueness/size mismatch');
  const qtPe = qt.base.add(qt.base.add(0x3c).readU32());
  if (qt.base.readU16() !== 0x5a4d || qtPe.readU32() !== 0x4550 || qtPe.add(4).readU16() !== 0x8664 || qtPe.add(8).readU32() !== 0x517180cf || qtPe.add(24).readU16() !== 0x20b || qtPe.add(80).readU32() !== 0x31f000) throw new Error('Loaded QtCore PE metadata mismatch');
  const hexBytes = bytes => Array.from(new Uint8Array(bytes), x => x.toString(16).padStart(2, '0')).join('');
  const qtGetters = [{rva:0xfec0,bytes:'488b018b4008c3'},{rva:0x2e90,bytes:'488b018b4004c3'},{rva:0x47940,bytes:'488b01488b4010c3'}];
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
    if (counts[kind] >= ${EVENT_CAP_PER_KIND} || total >= ${EVENT_CAP_TOTAL}) { overflow[kind]++; return; }
    counts[kind]++; total++;
    send({ kind: 'hook', event: kind, sequence: ++sequence, pid: Process.id,
      processCreationFiletime, tid: Process.getCurrentThreadId(), ...body });
  }
  const outer = hooks[0];
  const steps = hooks.slice(1).map(h => h.name);
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
        stack.pop(); if (!stack.length) activeByTid.delete(frame.tid);
      } catch (error) { send({ kind: 'hook-error', site: 'outer-return', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
  });
  for (const hook of hooks.slice(1)) Interceptor.attach(app.base.add(hook.rva), {
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
        this.selectedCall = { frame, returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp) };
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
          returnAddress: call.returnAddress, entryRsp: call.entryRsp, rawReturn, lowByte, failed, normalReturn: true, ...(hook.name === 'acceptance' ? {pathWitness: qstring(ptr(frame.pathQString))} : {}) });
      } catch (error) { send({ kind: 'hook-error', site: hook.name + '-return', pid: Process.id, error: String(error).slice(0, 300) }); }
    }
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
      receipt.suffixUnchanged = receipt.after.slice(10) === hook.bytes.slice(10);
      if (raw[0] === 0xe9) {
        receipt.displacement = new DataView(buffer).getInt32(1, true);
        receipt.redirectDestination = text(address.add(5 + receipt.displacement));
      }
      receipt.gatePassed = receipt.redirectKind === 'e9-rel32' && receipt.redirectSize === 5 && receipt.suffixUnchanged;
    } catch (error) { receipt.gatePassed = false; receipt.error = String(error).slice(0, 300); }
    if (!receipt.gatePassed) patchGatePassed = false;
    patchReceipts.push(receipt);
  }
  if (!patchGatePassed) {
    send({ kind: 'install-readiness-failure', pid: Process.id, processCreationFiletime,
      reason: 'requires_e9_rel32_and_unchanged_full_prefix_suffix', patches: patchReceipts });
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
  `;
}

function patchReadinessQualified(ready, hooks) {
  if (ready?.patchGatePassed !== true || !Array.isArray(ready.patches) || ready.patches.length !== hooks.length ||
      !/^0x[0-9a-f]+$/i.test(ready.module?.base ?? '')) return false;
  return hooks.every((hook, index) => {
    const receipt = ready.patches[index];
    if (!receipt || receipt.name !== hook.name || receipt.rva !== hook.rva || receipt.before !== hook.bytes ||
      receipt.gatePassed !== true || receipt.redirectKind !== 'e9-rel32' || receipt.redirectSize !== 5 ||
      receipt.suffixUnchanged !== true || typeof receipt.after !== 'string' || receipt.after.length !== hook.bytes.length ||
      !/^e9[0-9a-f]+$/i.test(receipt.after) || receipt.after.slice(10) !== hook.bytes.slice(10) ||
      !/^0x[0-9a-f]+$/i.test(receipt.address ?? '') || !/^0x[0-9a-f]+$/i.test(receipt.redirectDestination ?? '')) return false;
    const raw = Uint8Array.from(receipt.after.match(/../g), x => parseInt(x, 16));
    const displacement = new DataView(raw.buffer).getInt32(1, true);
    return BigInt(receipt.address) === BigInt(ready.module.base) + BigInt(hook.rva) &&
      receipt.displacement === displacement &&
      BigInt(receipt.redirectDestination) === BigInt(receipt.address) + 5n + BigInt(displacement);
  });
}

// One helper stdin lifecycle. write(false) means queued with backpressure, not failure.
// Keep termination and end idempotent even during the gap before stream destruction.
function createCommandChannel(getStream) {
  let terminationSent = false, ended = false;
  function writable(stream) {
    return stream && !ended && !stream.destroyed && !stream.writableEnded && !stream.writableFinished && stream.writable !== false;
  }
  function send(command) {
    const stream = getStream();
    if (!writable(stream) || (command === 'terminate' && terminationSent)) return false;
    stream.write(JSON.stringify({command}) + '\n');
    if (command === 'terminate') terminationSent = true;
    return true;
  }
  return {send, terminateAndEnd() {
    const stream = getStream();
    send('terminate');
    if (writable(stream)) {ended = true; stream.end();}
    return {terminationSent, ended};
  }};
}
function assertEncodedArgument(token, project) {
  const fixed = '0043003a005c00500052004f0042004c0045004d005c00700032003100330061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a';
  requireThat(token === fixed && token.length === 136 && /^[0-9a-f]+$/.test(token) && token.length % 4 === 0, 'Exact canonical 136-hex argument required');
  const units = token.match(/.{4}/g).map(x => parseInt(x, 16));
  requireThat(units.length === 34 && String.fromCharCode(...units) === project, 'Encoded argument roundtrip mismatch');
}
function createEndpoint(ms, now = () => performance.now()) {
  let finish, timer, started = null, settled = false;
  const wait = new Promise(resolve => {finish = resolve;});
  const end = reason => {if (!settled) {settled = true; clearTimeout(timer); finish({reason, elapsedMs: started === null ? null : now() - started});}};
  return {wait, start() {if (started !== null) throw Error('Observation resumed twice'); started = now(); timer = setTimeout(() => end('deadline'), ms);},
    accept(message) {
      if (settled || started === null) return;
      if (now() - started >= ms) {end('deadline'); return;}
      if (message.type === 'error' || ['hook-error', 'exception-handler-error'].includes(message.payload?.kind)) end('error');
      else if (message.payload?.kind === 'first-av-core') end('first-av');
      else if (message.payload?.kind === 'hook' && message.payload.event === 'outer-return') end('outer-return');
    }, cancel() {clearTimeout(timer);}};
}
function qualifyObservation(messages, identity, filetime, ready) {
  const out = {openObservationComplete: false, encodedPathQualified: false, postOpenRoute: {observed: false, status:'not_instrumented', basis:'CP209 static main forwards outer RAX to 2a0e0 unconditionally; no added hook'}};
  try {
    const events = messages.filter(m => m.type === 'send' && m.payload?.kind === 'hook').map(m => m.payload);
    const returns = events.filter(e => e.event === 'outer-return'), entries = events.filter(e => e.event === 'outer-enter');
    requireThat(returns.length === 1 && entries.length === 1, 'Unique outer pair required');
    const entry = entries[0], returned = returns[0], snapshot = returned.hookSnapshot;
    requireThat(events.length <= EVENT_CAP_TOTAL && events.every((e,i) => e.sequence === i+1 && e.pid === identity.pid && e.processCreationFiletime === filetime && e.tid === identity.primary_tid && e.invocationId === entry.invocationId), 'Sequence/identity mismatch');
    requireThat(events[0] === entry && events.at(-1) === returned && returned.normalReturn === true && !returned.invalid && !returned.pending && snapshot?.boundedStateInvalid === false, 'Outer frame incomplete');
    requireThat(snapshot.sequence === events.length && EVENT_KINDS.every(k => snapshot.counts[k] === events.filter(e => e.event === k).length && snapshot.counts[k] < EVENT_CAP_PER_KIND && snapshot.overflow[k] === 0), 'Counter parity failed');
    requireThat(['noUniqueOwner','outerLeaveMismatch','order','overlap'].every(k => snapshot.rejected[k] === 0), 'Frame attribution rejected');
    requireThat(!messages.some(m => m.type === 'error' || ['hook-error','exception-handler-error','install-readiness-failure'].includes(m.payload?.kind)), 'Observer errors');
    const base = BigInt(ready.module.base);
    requireThat(BigInt(entry.returnAddress) === base + BigInt(HOOKS[0].callerRva) && BigInt(entry.pathQString) === BigInt(entry.main) + 0x18d8n && returned.main === entry.main && returned.pathQString === entry.pathQString && returned.entryRsp === entry.entryRsp && returned.returnAddress === entry.returnAddress, 'Outer identity mismatch');
    const stages = events.filter(e => e.event.endsWith('-return') && e.event !== 'outer-return');
    let failed = null;
    for (let i=0; i<stages.length; i++) {
      const e=stages[i], hook=HOOKS[i+1];
      requireThat(hook && e.event === hook.name+'-return' && e.normalReturn === true && e.main === entry.main && BigInt(e.returnAddress) === base + BigInt(hook.callerRva) && BigInt(e.entryRsp) === BigInt(entry.entryRsp)-0x1e0n && failed === null, 'Helper order/frame mismatch');
      const raw=BigInt(e.rawReturn), low=Number(raw & 255n);
      const failure=hook.name === 'duplicate' ? raw !== 0n : hook.name === 'acceptance' ? low === 0 : raw === 0n;
      requireThat(e.lowByte === low && e.failed === failure, 'Raw scalar disagreement');
      if(failure) failed=hook.name;
    }
    requireThat(stages.length > 0 && (failed !== null || stages.length === 5) && (failed === null ? BigInt(returned.rawReturn)!==0n : BigInt(returned.rawReturn)===0n), 'Incomplete prerequisite prefix');
    const accepts = events.filter(e=>e.event==='acceptance-enter');
    requireThat(accepts.length === (stages.length === 5 ? 1 : 0), 'Acceptance entry count mismatch');
    const witness = w => w?.ok === true && w.rawReadStatus === 'exact_length_stable_reread' && w.truncated === false && w.object === entry.pathQString && w.length === 34 && Array.isArray(w.units) && w.units.length === 34 && w.units.every((n,i)=>n===PROJECT.charCodeAt(i)) && w.text === PROJECT;
    requireThat(witness(entry.pathWitness) && witness(returned.pathWitness), 'Outer path witness incomplete');
    if(accepts.length) {
      const a=accepts[0], project=stages[3], acceptance=stages[4];
      requireThat(a.sequence === project.sequence+1 && acceptance.sequence === a.sequence+1 && a.rawRCX === project.rawReturn && a.rawRDX === entry.pathQString && a.main === entry.main && BigInt(a.returnAddress)===base+BigInt(HOOKS[5].callerRva) && BigInt(a.entryRsp)===BigInt(entry.entryRsp)-0x1e0n && witness(a.pathWitness) && witness(acceptance.pathWitness), 'Reader path/receiver attribution incomplete');
      out.encodedPathQualified=true;
    }
    out.outerObservationQualified=true; out.openObservationComplete=true; out.firstFailedPrerequisite=failed; out.rawOuterReturn=returned.rawReturn;
    out.postOpenRoute.inferredArm=BigInt(returned.rawReturn)===0n ? 'null_refresh_arm' : 'nonnull_arm';
  } catch(error) {out.qualificationError=String(error);}
  return out;
}
function cleanupComplete(r, error, buffer, overflow) {
  return r.cleanupVerified === true && r.processAbsent === true && r.helperStdoutClosed === true && error === null && buffer === '' && !overflow && r.helperStdinError == null && r.scriptUnloadSettled === true && r.sessionDetached === true && r.rawHelperReceiptsRetained === true && r.failures.length === 0;
}
function completionPrerequisites(r,error,buffer,overflow) {
  return r.patchReadinessQualified === true && r.installReadinessFailure == null && r.openObservationComplete === true && r.encodedPathQualified === true && cleanupComplete(r,error,buffer,overflow) && r.sourcePreserved === true && !!r.fixtureAfter && r.hostMessageOverflow == null && r.resumedReceipt?.previous_suspend_count === 1 && ['running','exited'].includes(r.resumedReceipt.state) && r.observationEndpoint?.reason === 'outer-return' && r.observationEndpoint.elapsedMs < OBSERVATION_MS && r.intentionalTerminationRequested === true && !(r.cleanupReceipt?.termination_attempts??[]).some(a=>a.source==='watchdog_deadline');
}

async function run() {
  requireThat(process.platform === 'win32', 'Windows execution host required');
  requireThat(process.argv.length === 3 && process.argv[2] === '--execute-reviewed-probe', 'Prepared only; exact opt-in argument required.');
  requireThat(QSTRING_LAYOUT_REVIEWED === true, 'HOLD: independent QString layout and generated reader review required');
  requireThat(HOOK_BYTES_REVIEWED === true, 'BLOCKED: independent six-entry relocation/caller and actual-code fixture review required');
  requireThat(ORIGINAL_RUN_REVIEWED === true, 'BLOCKED before any target launch: original-specific review has not approved execution.');

  const stamp = new Date().toISOString().replaceAll(/[:.]/g, '-');
  const out = path.join(ROOT, 'analysis', `resume213-original-encoded-open-${stamp}`);
  fs.mkdirSync(out, { recursive: false });
  const result = { schema: 'original-encoded-open-prepared-v1', startedUtc: new Date().toISOString(), host: os.hostname(),
    target: { exe: EXE, sha256: EXPECTED.exe, cwd: CWD, argv: [EXE, '-m', 'open', '-f', CLI_ARG] },
    fridaVersion: null, observationMs: OBSERVATION_MS, helperMaxSeconds: HELPER_MAX_SECONDS,
    originalInstallationVersion: '11.0.2', originalVersionResource: null,
    guiInput: false, guiControlAcquired: false, interceptorHooks: 6,
    hookCapPerKind: EVENT_CAP_PER_KIND, hookCapTotal: EVENT_CAP_TOTAL,
    causalClassification: 'indeterminate_pending_qualified_same_frame_review',
    failures: [], stages: [], out };
  let helper = null, device = null, session = null, script = null, childIdentity = null, creationFiletimeExact = null, helperExit = null;
  let helperBuffer = '', helperLines = [], helperError = null, stdoutClosed = false, stderr = '';
  let helperTransportOverflow = false, helperTotalBytes = 0;
  const endpoint = createEndpoint(OBSERVATION_MS);

  let stdoutEndResolve;
  const helperStdoutEnded = new Promise(resolve => { stdoutEndResolve = resolve; });
  let readyResolve, readyReject, resumeResolve, resumeReject;
  const ready = new Promise((resolve, reject) => { readyResolve = resolve; readyReject = reject; });
  const resumed = new Promise((resolve, reject) => { resumeResolve = resolve; resumeReject = reject; });
  const scriptMessages = [];
  const stage = (name, detail = {}) => result.stages.push({ name, utc: new Date().toISOString(), ...detail });
  const writeRaw = () => fs.writeFileSync(path.join(out, 'helper-raw.jsonl'), helperLines.join('\n') + (helperLines.length ? '\n' : ''), { flag: 'wx' });
  const commandChannel = createCommandChannel(() => helper?.stdin);
  const sendCommand = command => commandChannel.send(command);

  try {
    assertEncodedArgument(CLI_ARG, PROJECT);
    requireThat(fileSha(QT_PATH) === QT_SHA, 'QtCore4 DLL identity mismatch');
    requireThat(canonical(EXE) === EXE.toLowerCase(), 'Original canonical path mismatch');
    requireThat(fileSha(EXE) === EXPECTED.exe, 'Original executable SHA-256 mismatch');
    requireThat(fileSha(HELPER) === EXPECTED.helper, 'Owned helper SHA-256 mismatch');
    requireThat(fileSha(PROJECT) === EXPECTED.project, 'B98 project SHA-256 mismatch');
    requireThat(fs.statSync(SOURCE).isDirectory() && fs.statSync(CWD).isDirectory(), 'Required source or disposable directory missing');
    const sourceBefore = manifest(SOURCE), fixtureBefore = manifest(CWD, true);
    requireThat(sourceBefore.entries.filter(item => item.kind === 'file').length === 27, 'Unexpected B98 source file count');
    requireThat(sourceBefore.contentDigest === EXPECTED.fixtureManifest && fixtureBefore.contentDigest === EXPECTED.fixtureManifest,
      'B98 source or disposable content does not match the retained qualified manifest');
    save(out, 'fixture-before.json', fixtureBefore);
    save(out, 'source-fixture-reference.json', sourceBefore);
    result.fixtureBeforeDigest = fixtureBefore.contentDigest;

    const fridaPackagePath = fileURLToPath(new URL('../../package.json', FRIDA_URL));
    const fridaPackage = JSON.parse(fs.readFileSync(fridaPackagePath, 'utf8'));
    requireThat(fridaPackage.version === '17.23.2', 'Installed Frida version must be exactly 17.23.2');
    result.fridaVersion = fridaPackage.version;
    const nodePath = process.execPath;
    const pythonHash = fileSha(PYTHON);
    requireThat(process.version === 'v24.19.0' && fileSha(nodePath) === '3602f2bb1a10f2cbab4c36886218a33c1ab3db87290e73b033c46c77147d0237', 'Node identity mismatch');
    requireThat(pythonHash === '4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a', 'Python identity mismatch');
    requireThat(fileSha(fridaPackagePath) === '7efb59d2050b7b6304e1617f463c4eb7b2f7cb7e3e80e677976825a76ee31e8d', 'Frida package identity mismatch');
    save(out, 'runtime-identities.json', { node: { path: nodePath, version: process.version, sha256: fileSha(nodePath) },
      python: { path: PYTHON, sha256: pythonHash }, frida: { entry: FRIDA_URL, packagePath: fridaPackagePath, version: fridaPackage.version, packageSha256: fileSha(fridaPackagePath) },
      helper: { path: HELPER, sha256: fileSha(HELPER) }, target: { path: EXE, sha256: fileSha(EXE) } });
    result.qtCoreIdentity = {path: QT_PATH, sha256: fileSha(QT_PATH)};
    result.runnerSha256 = fileSha(fileURLToPath(import.meta.url));
    result.helperSha256 = fileSha(HELPER);
    stage('fixed-preflight-passed');

    const frida = await import(FRIDA_URL);
    device = await frida.getLocalDevice();
    result.fridaDeviceId = device.id;
    helper = spawn(PYTHON, ['-I', '-S', HELPER, '--exe', EXE, '--cwd', CWD, '--sha256', EXPECTED.exe,
      '--max-seconds', String(HELPER_MAX_SECONDS), '--', '-m', 'open', '-f', CLI_ARG],
    { cwd: ROOT, windowsHide: true, stdio: ['pipe', 'pipe', 'pipe'] });
    helper.stdin.on('error', error => { result.helperStdinError = { name: error.name, message: error.message }; });
    result.helperArgv = [PYTHON, '-I', '-S', HELPER, '--exe', EXE, '--cwd', CWD, '--sha256', EXPECTED.exe,
      '--max-seconds', String(HELPER_MAX_SECONDS), '--', '-m', 'open', '-f', CLI_ARG];
    result.helperPid = helper.pid;
    helper.stdout.setEncoding('utf8');
    helper.stdout.on('data', chunk => {
      if (helperTransportOverflow) return;
      helperTotalBytes += Buffer.byteLength(chunk);
      if (helperTotalBytes > 65536) {
        helperTransportOverflow = true; helperBuffer = '';
        helperError = new Error('Helper cumulative stdout exceeds 64 KiB'); readyReject(helperError); resumeReject(helperError); return;
      }
      helperBuffer += chunk;
      if (Buffer.byteLength(helperBuffer) > 65536) { helperTransportOverflow = true; helperBuffer = ''; helperError = new Error('Helper stdout exceeds 64 KiB'); readyReject(helperError); resumeReject(helperError); return; }
      let index;
      while ((index = helperBuffer.indexOf('\n')) >= 0) {
        const line = helperBuffer.slice(0, index); helperBuffer = helperBuffer.slice(index + 1);
        if (Buffer.byteLength(line) > 8192 || helperLines.length >= 32) { helperTransportOverflow = true; helperBuffer = ''; helperError = new Error('Helper receipt count/line bound exceeded'); readyReject(helperError); resumeReject(helperError); return; }
        helperLines.push(line);
        try {
          const row = JSON.parse(line);
          if (!row || Array.isArray(row) || !['created', 'resumed', 'status', 'endpoint', 'cleanup', 'error', 'cleanup_error'].includes(row.event)) throw new Error('Unexpected helper receipt');
          if (row.event === 'created') {
            childIdentity = row.identity;
            const exact = line.match(/"creation_filetime"\s*:\s*(\d+)/);
            creationFiletimeExact = exact?.[1] ?? null;
            result.createdReceipt = row; readyResolve(row);
          }
          if (row.event === 'resumed') { result.resumedReceipt = row; resumeResolve(row); }
          if (row.event === 'error' || row.event === 'cleanup_error') { helperError = new Error(`Helper ${row.event}: ${JSON.stringify(row)}`); readyReject(helperError); resumeReject(helperError); }
          if (row.event === 'cleanup') result.cleanupReceipt = row;
        } catch (error) { helperError = error; readyReject(error); resumeReject(error); }
      }
    });
    helper.stdout.on('end', () => { stdoutClosed = true; stdoutEndResolve(); });
    helper.stderr.setEncoding('utf8');
    helper.stderr.on('data', chunk => { if (stderr.length < 4096) stderr += chunk.slice(0, 4096 - stderr.length); });
    helperExit = new Promise((resolve, reject) => {
      helper.once('error', reject);
      helper.once('exit', (code, signal) => { result.helperExit = { code, signal }; resolve(); });
    });
    ready.catch(() => {}); resumed.catch(() => {});
    stage('owned-helper-started');
    const created = await bounded(ready, 5000, 'owned child creation');
    requireThat(created.state === 'suspended' && created.gui_control_acquired === false, 'Child was not created suspended or GUI flag unexpected');
    requireThat(childIdentity && childIdentity.launcher_pid === helper.pid && Number.isInteger(childIdentity.pid) && childIdentity.pid > 0 &&
      Number.isInteger(childIdentity.primary_tid) && childIdentity.primary_tid > 0 && childIdentity.creation_filetime > 0 &&
      childIdentity.sha256 === EXPECTED.exe && childIdentity.canonical_image.toLowerCase() === EXE.toLowerCase() &&
      childIdentity.cwd.toLowerCase() === CWD.toLowerCase() &&
      typeof creationFiletimeExact === 'string' && /^\d+$/.test(creationFiletimeExact) &&
      Array.isArray(childIdentity.argv) && childIdentity.argv.length === 5 &&
      childIdentity.argv[0].toLowerCase() === EXE.toLowerCase() &&
      JSON.stringify(childIdentity.argv.slice(1)) === JSON.stringify(['-m', 'open', '-f', CLI_ARG]),
    'Owned child identity/command receipt mismatch');
    result.ownedIdentity = childIdentity;
    result.creationFiletimeExact = creationFiletimeExact;
    stage('created-suspended-identity-validated');

    const source = injectedSource(creationFiletimeExact);
    fs.writeFileSync(path.join(out, 'outer-loader-observers.js'), source, { flag: 'wx' });
    result.injectedScriptSha256 = sha(source);
    session = await bounded(device.attach(childIdentity.pid), 12000, 'Frida attach');
    script = await bounded(session.createScript(source), 3000, 'Frida script creation');
    let handlerReadyResolve, handlerReadyReject;
    const handlerReady = new Promise((resolve, reject) => { handlerReadyResolve = resolve; handlerReadyReject = reject; });
    handlerReady.catch(() => {}); // Can reject during script.load before the await below.
    script.message.connect(message => {
      if (scriptMessages.length < 128) scriptMessages.push(message);
      else result.hostMessageOverflow = (result.hostMessageOverflow ?? 0) + 1;
      endpoint.accept(message);
      if (message.type === 'send' && message.payload?.kind === 'install-readiness-failure') {
        result.installReadinessFailure ??= message.payload;
        sendCommand('terminate');
        handlerReadyReject(new Error('Installed hook redirect failed pre-resume five-byte gate'));
      }
      else if (message.type === 'send' && message.payload?.kind === 'handler-ready' && message.payload.pid === childIdentity.pid) handlerReadyResolve(message.payload);
      else if (message.type === 'send' && message.payload?.kind === 'first-av-core' && message.payload.pid === childIdentity.pid) result.firstAvCore ??= message.payload;
      else if (message.type === 'send' && message.payload?.kind === 'first-access-violation' && message.payload.pid === childIdentity.pid) result.firstAccessViolation ??= message.payload;
      else if (message.type === 'send' && message.payload?.kind === 'exception-handler-error') result.handlerError ??= message.payload;
      else if (message.type === 'send' && message.payload?.kind === 'hook-error') result.hookError ??= message.payload;
      else if (message.type === 'error') { result.scriptError ??= { description: message.description ?? null, stack: message.stack ?? null }; handlerReadyReject(new Error('Injected script error')); }
    });
    await bounded(script.load(), 3000, 'Frida script load');
    result.handlerReady = await bounded(handlerReady, 3000, 'exception-handler installation receipt');
    requireThat(result.handlerReady.pid === childIdentity.pid && result.handlerReady.arch === 'x64' && result.handlerReady.platform === 'windows' &&
      result.handlerReady.module?.path?.toLowerCase() === EXE.toLowerCase() &&
      result.handlerReady.module?.name?.toLowerCase() === path.basename(EXE).toLowerCase() &&
      result.handlerReady.module?.size === 0x535f000 && result.handlerReady.registrationCallCompleted === true &&
      Array.isArray(result.handlerReady.hooks) && result.handlerReady.hooks.length === HOOKS.length &&
      HOOKS.every((hook, index) => result.handlerReady.hooks[index]?.rva === hook.rva &&
        result.handlerReady.hooks[index]?.bytes === hook.bytes && result.handlerReady.hooks[index]?.callerRva === hook.callerRva),
    'Hook readiness identity, image, or bytes mismatch');
    result.patchReadinessQualified = patchReadinessQualified(result.handlerReady, HOOKS);
    requireThat(result.patchReadinessQualified && result.installReadinessFailure == null, 'Installed patch receipts failed independent host gate; held child must not resume');
    stage('handler-ready-before-resume');

    endpoint.start();
    sendCommand('resume');
    result.resumedReceipt = await bounded(resumed, 3000, 'helper resume receipt');
    const resumeStateValid = result.resumedReceipt.state === 'running' && result.resumedReceipt.exit_verified === false ||
      result.resumedReceipt.state === 'exited' && result.resumedReceipt.exit_verified === true;
    requireThat(result.resumedReceipt.previous_suspend_count === 1 && resumeStateValid, 'Resume receipt invalid');
    stage('resumed-once');
    result.observationEndpoint = await endpoint.wait;
    stage('observation-ended', result.observationEndpoint);
    result.intentionalTerminationRequested = true;
    commandChannel.terminateAndEnd();
  } catch (error) {
    result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', name: error?.name ?? 'Error', message: String(error?.message ?? error).slice(0, 2000) });
    if (helper && !helper.stdin.destroyed) sendCommand('terminate');
  } finally {
    if (helperExit) {
      endpoint.cancel();
      if (helper) { result.intentionalTerminationRequested = true; commandChannel.terminateAndEnd(); }
      try { await bounded(helperExit, 22000, 'owned cleanup and watchdog'); }
      catch (error) { result.failures.push({stage: 'helper-exit', message: String(error)}); }
      try { await bounded(helperStdoutEnded, 2000, 'helper stdout drain'); }
      catch (error) { result.failures.push({ stage: 'helper-stdout-drain', message: String(error?.message ?? error) }); }
    }
    result.helperRawReceipts = helperLines.slice();
    result.helperStdoutClosed = stdoutClosed;
    result.helperStderr = stderr;
    if (helperBuffer) result.helperPartialLine = helperBuffer;
    try {
      if (script && session) {
        requireThat(typeof session.isDetached === 'function', 'Frida Session.isDetached method unavailable');
        if (session.isDetached()) result.scriptUnloadDisposition = 'not_applicable_session_already_detached_after_target_exit';
        else { await bounded(script.unload(), 2000, 'script unload'); result.scriptUnloadDisposition = 'unload_completed'; }
        result.scriptUnloadSettled = true;
      } else { result.scriptUnloadDisposition = 'not_applicable_no_script'; result.scriptUnloadSettled = true; }
    } catch (error) { result.scriptUnloadSettled = false; result.failures.push({ stage: 'script-unload', message: String(error?.message ?? error) }); }
    try {
      if (session) {
        requireThat(typeof session.isDetached === 'function', 'Frida Session.isDetached method unavailable');
        if (!session.isDetached()) await bounded(session.detach(), 2000, 'session detach');
        result.sessionDetached = session.isDetached();
      } else result.sessionDetached = true;
    } catch (error) { result.sessionDetached = false; result.failures.push({ stage: 'session-detach', message: String(error?.message ?? error) }); }
    if (childIdentity?.pid) {
      try { result.windowsAfter = windowsPresence(childIdentity.pid); result.windowsAbsent = result.windowsAfter.value.present === false; }
      catch (error) { result.windowsAbsent = false; result.failures.push({ stage: 'windows-absence', message: String(error?.message ?? error) }); }
      try {
        const rows = await bounded(device.enumerateProcesses(), 3000, 'Frida same-device enumeration');
        requireThat(Array.isArray(rows), 'Frida process enumeration was not an array');
        const matches = rows.filter(row => row?.pid === childIdentity.pid);
        requireThat(matches.length <= 1, 'Duplicate target PID in Frida enumeration');
        result.fridaAfter = { deviceId: device.id, pid: childIdentity.pid, present: matches.length === 1, process: matches[0] ? { pid: matches[0].pid, name: matches[0].name, identifier: matches[0].identifier } : null };
        result.fridaAbsent = matches.length === 0;
      } catch (error) { result.fridaAbsent = false; result.failures.push({ stage: 'frida-absence', message: String(error?.message ?? error) }); }
    } else { result.windowsAbsent = false; result.fridaAbsent = false; }
    try { result.fixtureAfter = manifest(CWD, true); result.fixtureComparison = { beforeDigest: result.fixtureBeforeDigest ?? null,
      afterDigest: result.fixtureAfter.contentDigest, unchanged: result.fixtureBeforeDigest === result.fixtureAfter.contentDigest }; }
    catch (error) { result.fixtureAfterError = String(error?.message ?? error); result.failures.push({ stage: 'fixture-after', message: result.fixtureAfterError }); }
    result.scriptMessages = scriptMessages;
    Object.assign(result, qualifyObservation(scriptMessages, childIdentity, creationFiletimeExact, result.handlerReady));
    result.cleanupVerified = result.cleanupReceipt?.exit_verified === true && result.cleanupReceipt?.handles_closed === true && result.cleanupReceipt?.wait_result === 0 && result.helperExit?.code === 0;
    result.processAbsent = result.windowsAbsent === true && result.fridaAbsent === true;
    try { const after = manifest(SOURCE); result.sourceAfterDigest = after.contentDigest; result.sourcePreserved = after.contentDigest === EXPECTED.fixtureManifest; }
    catch (error) { result.sourcePreserved = false; result.failures.push({stage:'source-after', message:String(error)}); }
    result.lifecycleAcceptance = {save: false, acceptedClose: false, complete: false};
    result.releaseBlocked = ['independent exact runner/generated agent review', 'QString layout/reader review', 'native ownership/endpoint review'];
    try { if (result.fixtureAfter) save(out, 'fixture-after.json', result.fixtureAfter); }
    catch (error) { result.failures.push({ stage: 'fixture-after-retention', message: String(error?.message ?? error) }); result.captureComplete = false; }
    try { writeRaw(); result.rawHelperReceiptsRetained = true; }
    catch (error) { result.rawHelperReceiptsRetained = false; result.failures.push({ stage: 'raw-helper-retention', message: String(error?.message ?? error) }); }
    result.helperError = helperError ? { name: helperError.name, message: String(helperError.message).slice(0, 2000) } : null;
    result.helperTransport = { overflow: helperTransportOverflow, totalBytes: helperTotalBytes, partialLine: helperBuffer !== '' };
    result.completionPrerequisitesMet = completionPrerequisites(result, helperError, helperBuffer, helperTransportOverflow);
    result.cleanupComplete = cleanupComplete(result, helperError, helperBuffer, helperTransportOverflow);
    result.captureComplete = result.completionPrerequisitesMet;
    result.technicalStatus = 'HOLD_PENDING_INDEPENDENT_REVIEW';
    result.outcome = result.captureComplete ? 'CAPTURE_COMPLETE' : 'INCOMPLETE';
    if (!result.captureComplete) {
      result.tentativeCausalClassification = result.causalClassification;
      result.causalClassification = 'incomplete';
    }
    try { save(out, 'summary.json', result); }
    catch (error) { result.captureComplete = false; result.outcome = 'INCOMPLETE'; result.retentionError = String(error?.message ?? error); }
  }
  console.log(JSON.stringify({ outcome: result.outcome ?? 'INCOMPLETE', out, captureComplete: result.captureComplete === true }));
  process.exitCode = result.captureComplete ? 0 : 1;
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) run().catch(error => { console.error(JSON.stringify({ outcome: 'PRELAUNCH_FAILURE', error: String(error?.message ?? error) })); process.exitCode = 1; });

export { injectedSource, patchReadinessQualified, assertEncodedArgument, qualifyObservation, completionPrerequisites, cleanupComplete, createEndpoint, createCommandChannel, HOOKS, EVENT_KINDS, EXE, PROJECT, CLI_ARG };
