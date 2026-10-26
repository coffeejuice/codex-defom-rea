// PREPARED ONLY. Do not remove the false guard without independent review.
// Owns one exact B98 converter child through owned_suspended_process.py.
// Installs only Frida Process.setExceptionHandler; no Interceptor hooks or GUI input.
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
const OBSERVE_MS = 5000;
const HELPER_MAX_SECONDS = 20;

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
function injectedSource() {
  // This callback records only the first access violation and returns false,
  // explicitly leaving normal application exception handling in control.
  return `
  'use strict';
  let first = true;
  const text = value => { try { return value === undefined || value === null ? null : String(value); } catch (_) { return '<unprintable>'; } };
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
        let frames = [];
        try { frames = Thread.backtrace(context, Backtracer.ACCURATE).map(describe); }
        catch (e) { frames = [{ error: String(e) }]; }
        const rip = context.rip || context.pc || null;
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
  send({ kind: 'handler-ready', pid: Process.id, platform: Process.platform, arch: Process.arch, registrationCallCompleted: true });
  `;
}

async function run() {
  requireThat(process.platform === 'win32', 'Windows execution host required');
  requireThat(process.argv.length === 3 && process.argv[2] === '--execute-reviewed-probe', 'Prepared only; exact opt-in argument required.');
  requireThat(ORIGINAL_RUN_REVIEWED === true, 'BLOCKED before any target launch: original-specific review has not approved execution.');

  const stamp = new Date().toISOString().replaceAll(/[:.]/g, '-');
  const out = path.join(ROOT, 'analysis', `resume205-original-crash-stack-${stamp}`);
  fs.mkdirSync(out, { recursive: false });
  const result = { schema: 'original-crash-stack-probe-v1', startedUtc: new Date().toISOString(), host: os.hostname(),
    target: { exe: EXE, sha256: EXPECTED.exe, cwd: CWD, argv: [EXE, '-m', 'open', '-f', PROJECT] },
    fridaVersion: null, observerMs: OBSERVE_MS, helperMaxSeconds: HELPER_MAX_SECONDS,
    guiInput: false, guiControlAcquired: false, interceptorHooks: 0, failures: [], stages: [], out };
  let helper = null, device = null, session = null, script = null, childIdentity = null, helperExit = null;
  let helperBuffer = '', helperLines = [], helperError = null, stdoutClosed = false, stderr = '';
  let stdoutEndResolve;
  const helperStdoutEnded = new Promise(resolve => { stdoutEndResolve = resolve; });
  let readyResolve, readyReject, resumeResolve, resumeReject;
  const ready = new Promise((resolve, reject) => { readyResolve = resolve; readyReject = reject; });
  const resumed = new Promise((resolve, reject) => { resumeResolve = resolve; resumeReject = reject; });
  let captureEventResolve;
  const captureEvent = new Promise(resolve => { captureEventResolve = resolve; });
  const scriptMessages = [];
  const stage = (name, detail = {}) => result.stages.push({ name, utc: new Date().toISOString(), ...detail });
  const writeRaw = () => fs.writeFileSync(path.join(out, 'helper-raw.jsonl'), helperLines.join('\n') + (helperLines.length ? '\n' : ''), { flag: 'w' });
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
    const source = injectedSource();
    fs.writeFileSync(path.join(out, 'exception-handler.js'), source, { flag: 'wx' });
    result.runnerSha256 = fileSha(fileURLToPath(import.meta.url));
    result.injectedScriptSha256 = sha(source);
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
      helperBuffer += chunk;
      if (Buffer.byteLength(helperBuffer) > 65536) { helperError = new Error('Helper stdout exceeds 64 KiB'); readyReject(helperError); resumeReject(helperError); return; }
      let index;
      while ((index = helperBuffer.indexOf('\n')) >= 0) {
        const line = helperBuffer.slice(0, index); helperBuffer = helperBuffer.slice(index + 1);
        if (Buffer.byteLength(line) > 8192 || helperLines.length >= 32) { helperError = new Error('Helper receipt count/line bound exceeded'); readyReject(helperError); resumeReject(helperError); return; }
        helperLines.push(line);
        try {
          const row = JSON.parse(line);
          if (!row || Array.isArray(row) || !['created', 'resumed', 'status', 'endpoint', 'cleanup', 'error', 'cleanup_error'].includes(row.event)) throw new Error('Unexpected helper receipt');
          if (row.event === 'created') { childIdentity = row.identity; result.createdReceipt = row; readyResolve(row); }
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
      Array.isArray(childIdentity.argv) && childIdentity.argv.length === 5 &&
      childIdentity.argv[0].toLowerCase() === EXE.toLowerCase() &&
      JSON.stringify(childIdentity.argv.slice(1)) === JSON.stringify(['-m', 'open', '-f', PROJECT]),
    'Owned child identity/command receipt mismatch');
    result.ownedIdentity = childIdentity;
    stage('created-suspended-identity-validated');

    session = await bounded(device.attach(childIdentity.pid), 12000, 'Frida attach');
    script = await bounded(session.createScript(source), 3000, 'Frida script creation');
    let handlerReadyResolve, handlerReadyReject;
    const handlerReady = new Promise((resolve, reject) => { handlerReadyResolve = resolve; handlerReadyReject = reject; });
    script.message.connect(message => {
      scriptMessages.push(message);
      if (message.type === 'send' && message.payload?.kind === 'handler-ready' && message.payload.pid === childIdentity.pid) handlerReadyResolve(message.payload);
      else if (message.type === 'send' && message.payload?.kind === 'first-access-violation' && message.payload.pid === childIdentity.pid) { result.firstAccessViolation ??= message.payload; captureEventResolve(message.payload); }
      else if (message.type === 'send' && message.payload?.kind === 'exception-handler-error') { result.handlerError ??= message.payload; captureEventResolve(message.payload); }
      else if (message.type === 'error') { result.scriptError ??= { description: message.description ?? null, stack: message.stack ?? null }; handlerReadyReject(new Error('Injected script error')); captureEventResolve(result.scriptError); }
    });
    await bounded(script.load(), 3000, 'Frida script load');
    result.handlerReady = await bounded(handlerReady, 3000, 'exception-handler installation receipt');
    requireThat(result.handlerReady.pid === childIdentity.pid && result.handlerReady.arch === 'x64' && result.handlerReady.platform === 'windows', 'Handler readiness identity/platform mismatch');
    stage('handler-ready-before-resume');

    sendCommand('resume');
    result.resumedReceipt = await bounded(resumed, 3000, 'helper resume receipt');
    const resumeStateValid = result.resumedReceipt.state === 'running' && result.resumedReceipt.exit_verified === false ||
      result.resumedReceipt.state === 'exited' && result.resumedReceipt.exit_verified === true;
    requireThat(result.resumedReceipt.previous_suspend_count === 1 && resumeStateValid, 'Resume receipt invalid');
    stage('resumed-once');
    result.observation = await Promise.race([captureEvent.then(event => ({ endedEarly: true, event })), new Promise(resolve => setTimeout(() => resolve({ endedEarly: false, event: null }), OBSERVE_MS))]);
    stage('observation-window-ended', { observedEvent: result.observation.event?.kind ?? null });
    if (!result.observation.endedEarly) sendCommand('terminate');
  } catch (error) {
    result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', name: error?.name ?? 'Error', message: String(error?.message ?? error).slice(0, 2000) });
    if (helper && !helper.stdin.destroyed) sendCommand('terminate');
  } finally {
    if (helperExit) {
      // The first AV message can arrive before the original process handles
      // that exception. Keep the command channel open for natural exit first.
      try { await bounded(helperExit, 5000, 'natural target/helper exit'); stage('natural-exit-window-ended'); }
      catch (error) {
        result.naturalExitWindow = { elapsedMs: 5000, message: String(error?.message ?? error) };
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
    result.cleanupByTermination = result.cleanupReceipt?.termination_requested === true || (result.cleanupReceipt?.termination_attempts?.length ?? 0) > 0;
    result.cleanupVerified = result.cleanupReceipt?.exit_verified === true && result.cleanupReceipt?.handles_closed === true && result.cleanupReceipt?.wait_result === 0 && result.helperExit?.code === 0 && result.cleanupByTermination === false;
    result.processAbsent = result.windowsAbsent === true && result.fridaAbsent === true;
    const av = result.firstAccessViolation;
    result.exceptionRecordQualified = av?.kind === 'first-access-violation' && av.pid === childIdentity?.pid &&
      av.exceptionType === 'access-violation' && typeof av.exceptionAddress?.address === 'string' &&
      typeof av.rip?.address === 'string' && av.faultingInstruction?.module?.name &&
      av.registers && Object.keys(av.registers).length >= 10 && Array.isArray(av.accurateBacktrace) &&
      av.accurateBacktrace.length > 0 && av.accurateBacktrace.every(frame => typeof frame.address === 'string') &&
      av.disposition === 'return_false_continue_natural_exception';
    result.captureComplete = result.exceptionRecordQualified === true && result.cleanupVerified && result.processAbsent &&
      result.sessionDetached === true && result.fixtureComparison?.unchanged === true && result.failures.length === 0;
    try { if (result.fixtureAfter) save(out, 'fixture-after.json', result.fixtureAfter); }
    catch (error) { result.failures.push({ stage: 'fixture-after-retention', message: String(error?.message ?? error) }); result.captureComplete = false; }
    try { writeRaw(); result.rawHelperReceiptsRetained = true; }
    catch (error) { result.rawHelperReceiptsRetained = false; result.failures.push({ stage: 'raw-helper-retention', message: String(error?.message ?? error) }); }
    result.captureComplete = result.captureComplete && result.rawHelperReceiptsRetained === true && result.failures.length === 0;
    result.outcome = result.captureComplete ? 'CAPTURE_COMPLETE' : 'INCOMPLETE';
    try { save(out, 'summary.json', result); }
    catch (error) { result.captureComplete = false; result.outcome = 'INCOMPLETE'; result.retentionError = String(error?.message ?? error); }
  }
  console.log(JSON.stringify({ outcome: result.outcome ?? 'INCOMPLETE', out, captureComplete: result.captureComplete === true }));
  process.exitCode = result.captureComplete ? 0 : 1;
}

run().catch(error => { console.error(JSON.stringify({ outcome: 'PRELAUNCH_FAILURE', error: String(error?.message ?? error) })); process.exitCode = 1; });
