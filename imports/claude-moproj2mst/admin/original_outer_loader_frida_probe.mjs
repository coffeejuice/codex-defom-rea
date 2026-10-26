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
const CWD = 'C:\\PROBLEM\\p202a';
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
const NATURAL_EXIT_MS = 5000;
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
const EVENT_KINDS = ['outer-enter', 'duplicate-return', 'factory-return', 'cast-return', 'project-return', 'acceptance-return', 'outer-return'];
const EVENT_CAP_PER_KIND = 8;
const EVENT_CAP_TOTAL = 56;

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
        emit('outer-enter', { ...frame });
      } catch (error) { send({ kind: 'hook-error', site: 'outer-enter', pid: Process.id, error: String(error).slice(0, 300) }); }
    },
    onLeave(retval) {
      try {
        const frame = this.outerFrame; if (!frame) return;
        const stack = activeByTid.get(frame.tid) || [];
        if (stack.at(-1) !== frame) { rejected.outerLeaveMismatch++; return; }
        emit('outer-return', { ...frame, rawReturn: text(retval), normalReturn: true });
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
          returnAddress: call.returnAddress, entryRsp: call.entryRsp, rawReturn, lowByte, failed, normalReturn: true });
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
    registrationCallCompleted: true, patchGatePassed, patches: patchReceipts });
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

function completionPrerequisites(result, helperError, helperBuffer, transportOverflow) {
  return result.patchReadinessQualified === true && result.installReadinessFailure == null &&
    result.outerEvidenceQualified === true && result.cp205FaultPath === true &&
    result.firstAvCoreQualified === true && result.exceptionRecordQualified === true &&
    result.commandInvocationQualified === true &&
    result.cleanupVerified === true && result.cleanupReceipt?.exit_code === 0xc0000005 &&
    result.processAbsent === true && result.helperStdoutClosed === true && helperError === null &&
    helperBuffer === '' && transportOverflow === false && result.helperStdinError == null &&
    result.scriptUnloadSettled === true && result.sessionDetached === true &&
    result.fixtureComparison?.unchanged === true && result.rawHelperReceiptsRetained === true &&
    result.failures.length === 0;
}

async function run() {
  requireThat(process.platform === 'win32', 'Windows execution host required');
  requireThat(process.argv.length === 3 && process.argv[2] === '--execute-reviewed-probe', 'Prepared only; exact opt-in argument required.');
  requireThat(HOOK_BYTES_REVIEWED === true, 'BLOCKED: independent six-entry relocation/caller and actual-code fixture review required');
  requireThat(ORIGINAL_RUN_REVIEWED === true, 'BLOCKED before any target launch: original-specific review has not approved execution.');

  const stamp = new Date().toISOString().replaceAll(/[:.]/g, '-');
  const out = path.join(ROOT, 'analysis', `resume210-original-outer-loader-frida-${stamp}`);
  fs.mkdirSync(out, { recursive: false });
  const result = { schema: 'original-outer-loader-frida-probe-prepared-v1', startedUtc: new Date().toISOString(), host: os.hostname(),
    target: { exe: EXE, sha256: EXPECTED.exe, cwd: CWD, argv: [EXE, '-m', 'open', '-f', PROJECT] },
    fridaVersion: null, naturalExitMs: NATURAL_EXIT_MS, helperMaxSeconds: HELPER_MAX_SECONDS,
    originalInstallationVersion: '11.0.2', originalVersionResource: null,
    guiInput: false, guiControlAcquired: false, interceptorHooks: 6,
    hookCapPerKind: EVENT_CAP_PER_KIND, hookCapTotal: EVENT_CAP_TOTAL,
    causalClassification: 'indeterminate_pending_qualified_same_frame_review',
    failures: [], stages: [], out };
  let helper = null, device = null, session = null, script = null, childIdentity = null, creationFiletimeExact = null, helperExit = null;
  let helperBuffer = '', helperLines = [], helperError = null, stdoutClosed = false, stderr = '';
  let helperTransportOverflow = false, helperTotalBytes = 0;
  let stdoutEndResolve;
  const helperStdoutEnded = new Promise(resolve => { stdoutEndResolve = resolve; });
  let readyResolve, readyReject, resumeResolve, resumeReject;
  const ready = new Promise((resolve, reject) => { readyResolve = resolve; readyReject = reject; });
  const resumed = new Promise((resolve, reject) => { resumeResolve = resolve; resumeReject = reject; });
  const scriptMessages = [];
  const stage = (name, detail = {}) => result.stages.push({ name, utc: new Date().toISOString(), ...detail });
  const writeRaw = () => fs.writeFileSync(path.join(out, 'helper-raw.jsonl'), helperLines.join('\n') + (helperLines.length ? '\n' : ''), { flag: 'wx' });
  const sendCommand = command => { if (!helper?.stdin.destroyed) helper.stdin.write(JSON.stringify({ command }) + '\n'); };

  try {
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
    save(out, 'runtime-identities.json', { node: { path: nodePath, version: process.version, sha256: fileSha(nodePath) },
      python: { path: PYTHON, sha256: pythonHash }, frida: { entry: FRIDA_URL, packagePath: fridaPackagePath, version: fridaPackage.version, packageSha256: fileSha(fridaPackagePath) },
      helper: { path: HELPER, sha256: fileSha(HELPER) }, target: { path: EXE, sha256: fileSha(EXE) } });
    result.runnerSha256 = fileSha(fileURLToPath(import.meta.url));
    result.helperSha256 = fileSha(HELPER);
    stage('fixed-preflight-passed');

    const frida = await import(FRIDA_URL);
    device = await frida.getLocalDevice();
    result.fridaDeviceId = device.id;
    helper = spawn(PYTHON, ['-I', '-S', HELPER, '--exe', EXE, '--cwd', CWD, '--sha256', EXPECTED.exe,
      '--max-seconds', String(HELPER_MAX_SECONDS), '--', '-m', 'open', '-f', PROJECT],
    { cwd: ROOT, windowsHide: true, stdio: ['pipe', 'pipe', 'pipe'] });
    helper.stdin.on('error', error => { result.helperStdinError = { name: error.name, message: error.message }; });
    result.helperArgv = [PYTHON, '-I', '-S', HELPER, '--exe', EXE, '--cwd', CWD, '--sha256', EXPECTED.exe,
      '--max-seconds', String(HELPER_MAX_SECONDS), '--', '-m', 'open', '-f', PROJECT];
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
      JSON.stringify(childIdentity.argv.slice(1)) === JSON.stringify(['-m', 'open', '-f', PROJECT]),
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

    sendCommand('resume');
    result.resumedReceipt = await bounded(resumed, 3000, 'helper resume receipt');
    const resumeStateValid = result.resumedReceipt.state === 'running' && result.resumedReceipt.exit_verified === false ||
      result.resumedReceipt.state === 'exited' && result.resumedReceipt.exit_verified === true;
    requireThat(result.resumedReceipt.previous_suspend_count === 1 && resumeStateValid, 'Resume receipt invalid');
    stage('resumed-once');
    // Keep the command channel open. The finally block gives natural exit five seconds.
  } catch (error) {
    result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', name: error?.name ?? 'Error', message: String(error?.message ?? error).slice(0, 2000) });
    if (helper && !helper.stdin.destroyed) sendCommand('terminate');
  } finally {
    if (helperExit) {
      // The first AV message can arrive before the original process handles
      // that exception. Keep the command channel open for natural exit first.
      try { await bounded(helperExit, NATURAL_EXIT_MS, 'natural target/helper exit'); stage('natural-exit-window-ended'); }
      catch (error) {
        result.naturalExitWindow = { elapsedMs: NATURAL_EXIT_MS, message: String(error?.message ?? error) };
        if (helper && !helper.stdin.destroyed) { sendCommand('terminate'); helper.stdin.end(); }
        try { await bounded(helperExit, 22000, 'fallback helper exit and watchdog cleanup'); }
        catch (last) { result.failures.push({ stage: 'helper-exit', message: String(last?.message ?? last) }); }
      }
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
    const hookEvents = scriptMessages.filter(message => message.type === 'send' && message.payload?.kind === 'hook').map(message => message.payload);
    const hookErrors = scriptMessages.filter(message => message.type === 'send' && message.payload?.kind === 'hook-error').map(message => message.payload);
    const eventKinds = EVENT_KINDS;
    const counts = Object.fromEntries(eventKinds.map(kind => [kind, hookEvents.filter(event => event.event === kind).length]));
    const snapshot = result.firstAvCore?.hookSnapshot;
    const throughCore = hookEvents.filter(event => Number.isInteger(event.sequence) && event.sequence <= snapshot?.sequence);
    const throughCoreCounts = Object.fromEntries(eventKinds.map(kind => [kind, throughCore.filter(event => event.event === kind).length]));
    const snapshotCountsValid = Number.isInteger(snapshot?.sequence) && snapshot.sequence >= 0 &&
      eventKinds.every(kind => Number.isInteger(snapshot.counts?.[kind]) && snapshot.counts[kind] >= 0 && snapshot.counts[kind] <= EVENT_CAP_PER_KIND &&
        Number.isInteger(snapshot.overflow?.[kind]) && snapshot.overflow[kind] === 0) &&
      ['outerCaller', 'helperCaller', 'noUniqueOwner', 'outerLeaveMismatch', 'order', 'overlap'].every(kind => Number.isInteger(snapshot.rejected?.[kind]) && snapshot.rejected[kind] >= 0);
    const snapshotParity = snapshotCountsValid &&
      eventKinds.every(kind => throughCoreCounts[kind] === snapshot.counts[kind]) &&
      snapshot.sequence === eventKinds.reduce((sum, kind) => sum + snapshot.counts[kind], 0) &&
      throughCore.length === snapshot.sequence &&
      throughCore.every((event, index) => event.sequence === index + 1 && eventKinds.includes(event.event));
    const allCountersUncapped = eventKinds.every(kind => (snapshot?.overflow?.[kind] ?? 1) === 0 &&
      (snapshot?.counts?.[kind] ?? EVENT_CAP_PER_KIND) < EVENT_CAP_PER_KIND && counts[kind] < EVENT_CAP_PER_KIND);
    result.hookCoverage = { counts, snapshot, errors: hookErrors,
      throughCoreCounts, snapshotCountsValid, snapshotParity,
      cappedOrPossiblyCapped: !allCountersUncapped,
      identityValid: hookEvents.every(event => event.pid === childIdentity?.pid && event.processCreationFiletime === creationFiletimeExact),
      sequenceValid: hookEvents.every((event, index) => event.sequence === index + 1),
      boundsValid: hookEvents.length <= EVENT_CAP_TOTAL && eventKinds.every(kind => counts[kind] <= EVENT_CAP_PER_KIND),
      allEventsObserved: eventKinds.every(kind => throughCoreCounts[kind] > 0) };
    result.hookEvidenceQualified = snapshot?.boundedStateInvalid === false && result.hookCoverage.snapshotParity && result.hookCoverage.identityValid && result.hookCoverage.sequenceValid &&
      result.hookCoverage.boundsValid && !result.hookCoverage.cappedOrPossiblyCapped &&
      hookErrors.length === 0 && !result.hostMessageOverflow && result.handlerError == null && result.scriptError == null;
    result.cleanupByTermination = result.cleanupReceipt?.termination_requested === true || (result.cleanupReceipt?.termination_attempts?.length ?? 0) > 0;
    result.cleanupVerified = result.cleanupReceipt?.exit_verified === true && result.cleanupReceipt?.handles_closed === true && result.cleanupReceipt?.wait_result === 0 && result.helperExit?.code === 0 && result.cleanupByTermination === false;
    result.processAbsent = result.windowsAbsent === true && result.fridaAbsent === true;
    const core = result.firstAvCore;
    result.firstAvCoreQualified = core?.kind === 'first-av-core' && core.pid === childIdentity?.pid &&
      core.processCreationFiletime === creationFiletimeExact && core.exceptionType === 'access-violation' &&
      typeof core.rip === 'string' && typeof core.rcx === 'string' && typeof core.exceptionAddress === 'string' &&
      Number.isInteger(core.tid) && Array.isArray(core.activeInvocations) &&
      Number.isInteger(core.hookSnapshot?.sequence) && core.hookSnapshot?.counts && core.hookSnapshot?.overflow &&
      core.disposition === 'return_false_continue_natural_exception';
    const av = result.firstAccessViolation;
    result.exceptionRecordQualified = av?.kind === 'first-access-violation' && av.pid === childIdentity?.pid &&
      av.processCreationFiletime === creationFiletimeExact && av.tid === core?.tid &&
      av.exceptionType === 'access-violation' && typeof av.exceptionAddress?.address === 'string' &&
      typeof av.rip?.address === 'string' && av.faultingInstruction?.module?.name &&
      av.registers && Object.keys(av.registers).length >= 10 && Array.isArray(av.accurateBacktrace) &&
      av.accurateBacktrace.length > 0 && av.accurateBacktrace.every(frame => typeof frame.address === 'string') &&
      av.rip.address?.toLowerCase() === core?.rip?.toLowerCase() &&
      av.registers.rcx?.toLowerCase() === core?.rcx?.toLowerCase() &&
      av.memoryOperation?.operation === core?.memoryOperation?.operation &&
      av.memoryOperation?.address?.toLowerCase() === core?.memoryOperation?.address?.toLowerCase() &&
      av.disposition === 'return_false_continue_natural_exception';
    const frames = av?.accurateBacktrace ?? [];
    const selectedChain = ['0xa7510', '0xeb1ec', '0xeaa7a'];
    const chainAt = frames.findIndex(frame => frame.module?.path?.toLowerCase() === EXE.toLowerCase() &&
      frame.module.offset?.toLowerCase() === selectedChain[0]);
    result.cp205FaultPath = result.firstAvCoreQualified && result.exceptionRecordQualified &&
      core.ripModule?.name?.toLowerCase() === 'qtcore4_64.dll' && core.ripModule?.offset?.toLowerCase() === '0x32c0' &&
      core.ripModule?.path?.toLowerCase() === path.join(path.dirname(EXE), 'QtCore4_64.dll').toLowerCase() &&
      core.memoryOperation?.operation === 'read' && core.memoryOperation?.address?.toLowerCase() === '0x58' &&
      core.rcx?.toLowerCase() === '0x58' && chainAt >= 0 &&
      selectedChain.every((offset, index) => frames[chainAt + index]?.module?.path?.toLowerCase() === EXE.toLowerCase() &&
        frames[chainAt + index]?.module?.offset?.toLowerCase() === offset);
    const entries = throughCore.filter(e => e.event === 'outer-enter');
    const returns = throughCore.filter(e => e.event === 'outer-return');
    const pairs = entries.flatMap(entry => {
      const matched = returns.filter(e => e.invocationId === entry.invocationId && e.tid === entry.tid &&
        e.main === entry.main && e.entryRsp === entry.entryRsp && e.returnAddress === entry.returnAddress && e.sequence > entry.sequence);
      if (matched.length !== 1) return [];
      const returned = matched[0];
      const stages = throughCore.filter(e => e.invocationId === entry.invocationId && e.tid === entry.tid &&
        e.sequence > entry.sequence && e.sequence < returned.sequence && e.event.endsWith('-return'));
      const failure = stages.find(e => e.failed === true);
      const required = failure ? HOOKS.slice(1).findIndex(h => h.name + '-return' === failure.event) + 1 : 5;
      const base = BigInt(result.handlerReady.module.base);
      const valid = entry.returnAddress?.toLowerCase() === '0x' + (base + 0x1c94n).toString(16) &&
        !returned.invalid && !returned.pending && stages.length === required && required > 0 &&
        stages.every((e, index) => e.event === HOOKS[index + 1].name + '-return' && e.normalReturn === true &&
          e.main === entry.main && e.returnAddress?.toLowerCase() === '0x' + (base + BigInt(HOOKS[index + 1].callerRva)).toString(16) &&
          e.failed === (index === required - 1 && !!failure)) &&
        (failure ? returned.rawReturn === '0x0' : returned.rawReturn !== '0x0');
      return [{ entry, returned, stages, valid, firstFailedPrerequisite: failure?.event.replace('-return', '') ?? null }];
    });
    result.outerPairs = pairs;
    const noAmbiguity = ['noUniqueOwner', 'outerLeaveMismatch', 'order', 'overlap'].every(k => snapshot?.rejected?.[k] === 0);
    const unique = entries.length === 1 && returns.length === 1 && pairs.length === 1 && pairs[0].valid && noAmbiguity && pairs[0].entry.tid === core?.tid && pairs[0].entry.tid === childIdentity?.primary_tid &&
      BigInt(pairs[0].entry.pathQString) === BigInt(pairs[0].entry.main) + 0x18d8n;
    result.outerEvidenceQualified = result.hookEvidenceQualified && unique;
    result.causalClassification = result.cp205FaultPath && result.outerEvidenceQualified
      ? pairs[0].firstFailedPrerequisite ? 'same_fault_path_outer_' + pairs[0].firstFailedPrerequisite : 'same_fault_path_outer_nonzero'
      : result.cp205FaultPath ? 'same_fault_path_outer_indeterminate' : 'probe_path_divergence';
    // CP209 main selects f, prepares these two stack locals and directly calls
    // this entry. This identifies the command invocation, not QString content.
    result.commandInvocationQualified = result.outerEvidenceQualified === true;
    result.runtimePathQualification = 'raw_qstring_address_only_content_unverified';
    result.causalClaimScope = 'first_prerequisite_of_fixed_owned_cli_open_invocation';
    result.captureComplete = false;
    result.releaseBlocked = ['independent repaired-runner and harmless-fixture review', 'six-entry live relocation review', 'owned harmless native exception/cleanup qualification'];
    try { if (result.fixtureAfter) save(out, 'fixture-after.json', result.fixtureAfter); }
    catch (error) { result.failures.push({ stage: 'fixture-after-retention', message: String(error?.message ?? error) }); result.captureComplete = false; }
    try { writeRaw(); result.rawHelperReceiptsRetained = true; }
    catch (error) { result.rawHelperReceiptsRetained = false; result.failures.push({ stage: 'raw-helper-retention', message: String(error?.message ?? error) }); }
    result.helperError = helperError ? { name: helperError.name, message: String(helperError.message).slice(0, 2000) } : null;
    result.helperTransport = { overflow: helperTransportOverflow, totalBytes: helperTotalBytes, partialLine: helperBuffer !== '' };
    result.completionPrerequisitesMet = completionPrerequisites(result, helperError, helperBuffer, helperTransportOverflow);
    result.captureComplete = result.captureComplete && result.completionPrerequisitesMet;
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

run().catch(error => { console.error(JSON.stringify({ outcome: 'PRELAUNCH_FAILURE', error: String(error?.message ?? error) })); process.exitCode = 1; });
