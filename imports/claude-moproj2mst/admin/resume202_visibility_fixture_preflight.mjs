// One diagnostic attempt. The Python helper alone owns/resumes/terminates its child.
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { createHash } from 'node:crypto';
import { spawn, execFileSync } from 'node:child_process';
import { createInterface } from 'node:readline';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const helperPath = path.join(root, 'admin', 'owned_suspended_process.py');
const reportPath = path.join(root, 'analysis', 'resume202-visibility-fixture-preflight.md');
const lifecycleReportPath = path.join(root, 'analysis', 'resume202-visibility-fixture-lifecycle-repair.md');
const cwd = path.join(root, 'admin');
const agentPath = path.join(root, 'admin', 'resume202_visibility_fixture_agent.js');
const exe = 'C:\\Program Files\\Python312\\python.exe';
const moduleUrl = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const digest = filename => createHash('sha256').update(fs.readFileSync(filename)).digest('hex');
const result = { startedUtc: new Date().toISOString(), moduleUrl, exe, cwd, maxSeconds: 20, helperPath, receipts: [], stages: [], failures: [], readiness: null };
let helper, device, session, script, helperExit, lines;
const waiters = [];
const teardown = { methodShape: null, attempts: [], sessionDetachedAtEnd: null, scriptUnloadSettled: false };
const rawError = error => Object.fromEntries([...new Set(['name', 'message', 'code', 'details', ...Object.getOwnPropertyNames(error ?? {})])].map(key => [key, error?.[key] ?? null]));
const stage = name => result.stages.push({ name, utc: new Date().toISOString() });
async function bounded(promise, ms, label) {
  let timer;
  try { return await Promise.race([promise, new Promise((_, reject) => { timer = setTimeout(() => reject(new Error(`${label} timed out after ${ms} ms`)), ms); })]); }
  finally { clearTimeout(timer); }
}
function receipt(event, ms = 5000) {
  const existing = result.receipts.find(item => item.event === event);
  if (existing) return Promise.resolve(existing);
  return bounded(new Promise(resolve => waiters.push({ event, resolve })), ms, `helper ${event}`);
}
function command(value) { if (helper && !helper.stdin.destroyed) helper.stdin.write(JSON.stringify({ command: value }) + '\n'); }
function sessionDetached() {
  if (!session) return true;
  if (typeof session.isDetached !== 'function') throw new Error('Frida Session.isDetached method unavailable');
  const detached = session.isDetached();
  if (typeof detached !== 'boolean') throw new Error('Frida Session.isDetached returned nonboolean');
  return detached;
}
function localPresence() {
  const pid = result.created?.identity?.pid;
  if (!Number.isInteger(pid) || pid <= 0) throw new Error('No validated child PID');
  const query = `$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' | Select-Object ProcessId,ExecutablePath,CommandLine); [pscustomobject]@{schema='owned-pid-presence-v1';querySucceeded=$true;pid=${pid};present=($rows.Count -eq 1);processes=@($rows)} | ConvertTo-Json -Compress -Depth 4`;
  const raw = execFileSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', query], { encoding: 'utf8', windowsHide: true, timeout: 10000 }).trim();
  if (!raw) throw new Error('Windows CIM presence query returned empty output');
  const value = JSON.parse(raw);
  if (!value || value.schema !== 'owned-pid-presence-v1' || value.querySucceeded !== true || value.pid !== pid || typeof value.present !== 'boolean' || !Array.isArray(value.processes) || value.processes.length > 1 || value.processes.some(row => row.ProcessId !== pid) || value.present !== (value.processes.length === 1)) throw new Error('Invalid Windows CIM presence receipt');
  return value;
}
try {
  if (!fs.statSync(cwd).isDirectory()) throw new Error('Fixed scratch cwd does not exist');
  result.pythonSha256 = digest(exe);
  result.helperSha256 = digest(helperPath);
  result.agentSha256 = digest(agentPath);
  result.runnerSha256 = digest(fileURLToPath(import.meta.url));
  result.fridaVersion = JSON.parse(fs.readFileSync(fileURLToPath(new URL('../../package.json', moduleUrl)), 'utf8')).version;
  if (result.fridaVersion !== '17.23.2') throw new Error('Unexpected Frida version');
  const frida = await import(moduleUrl);
  device = await frida.getLocalDevice(); stage('local-device');
  result.helperArgv = ['-I', '-S', helperPath, '--exe', exe, '--cwd', cwd, '--sha256', result.pythonSha256, '--max-seconds', '20', '--', '-I', '-S', '-c', 'import time; time.sleep(60)'];
  helper = spawn(exe, result.helperArgv, { cwd, windowsHide: true, stdio: ['pipe', 'pipe', 'pipe'] });
  helper.stdin.on('error', error => result.failures.push({ stage: 'helper-stdin', ...rawError(error) }));
  helperExit = new Promise((resolve, reject) => { helper.once('error', reject); helper.once('exit', (code, signal) => { result.helperExit = { code, signal }; resolve(); }); });
  helper.stderr.on('data', chunk => { result.helperStderr = (result.helperStderr ?? '') + chunk.toString(); });
  lines = createInterface({ input: helper.stdout });
  lines.on('line', line => {
    try {
      const item = JSON.parse(line); result.receipts.push(item);
      for (const waiter of waiters) if (waiter.event === item.event) waiter.resolve(item);
    } catch (error) { result.failures.push({ stage: 'helper-json', raw: line, ...rawError(error) }); }
  });
  result.created = await receipt('created');
  const identity = result.created.identity;
  if (result.created.state !== 'suspended' || identity.launcher_pid !== helper.pid || !Number.isInteger(identity.pid) || identity.pid <= 0 || identity.sha256 !== result.pythonSha256 || identity.canonical_image.toLowerCase() !== exe.toLowerCase() || identity.cwd.toLowerCase() !== cwd.toLowerCase()) throw new Error('Created receipt identity mismatch');
  stage('validated-owned-create-suspended');
  session = await bounded(device.attach(identity.pid), 12000, 'direct attach'); stage('attached');
  script = await bounded(session.createScript(fs.readFileSync(agentPath, 'utf8')), 2000, 'createScript');
  let readyResolve;
  const ready = new Promise(resolve => { readyResolve = resolve; });
  script.message.connect(message => { if (message.type === 'send' && message.payload?.kind === 'fixture-result') { result.readiness = message.payload; readyResolve(); } else result.scriptMessages = [...(result.scriptMessages ?? []), message]; });
  await bounded(script.load(), 10000, 'script load'); stage('script-loaded');
  await bounded(ready, 5000, 'readiness');
  if (result.readiness.pid !== identity.pid) throw new Error('Readiness PID mismatch');
  stage('script-ready');
  if (!result.readiness.passed || result.readiness.fridaRuntimeVersion !== '17.23.2') throw new Error('Fixture failed or unexpected Frida runtime');
  teardown.methodShape = { isDetached: typeof session.isDetached };
} catch (error) { result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', ...rawError(error) }); }
finally {
  // No PID-based kill: EOF/terminate/deadline all use the helper's retained handle.
  if (helper) {
    command('terminate');
    if (!helper.stdin.destroyed) helper.stdin.end();
    try { await bounded(helperExit, 26000, 'helper exit including watchdog'); } catch (error) { result.failures.push({ stage: 'helper-exit', ...rawError(error) }); }
    result.cleanup = result.receipts.find(item => item.event === 'cleanup') ?? null;
  }
  // The helper terminates through its retained creation handle before Frida teardown.
  // A timeout records uncertainty; it never implies that the operation settled.
  if (session) {
    let detached = false;
    try { detached = sessionDetached(); teardown.methodShape = { isDetached: 'function', afterChildTermination: detached }; }
    catch (error) { teardown.stateError = rawError(error); result.failures.push({ stage: 'detached-state', ...rawError(error) }); }
    if (detached) {
      teardown.scriptCleanupDisposition = 'not_applicable_session_already_detached_after_target_exit';
      teardown.attempts.push({ operation: 'script.cleanup', settled: true, notApplicable: true, disposition: teardown.scriptCleanupDisposition });
    } else if (script && typeof script.unload === 'function') {
      try { await bounded(script.unload(), 10000, 'final script unload'); teardown.scriptUnloadSettled = true; script = null; teardown.attempts.push({ operation: 'script.unload', settled: true }); }
      catch (error) { teardown.attempts.push({ operation: 'script.unload', settled: false, ...rawError(error) }); result.failures.push({ stage: 'final-unload', ...rawError(error) }); }
    }
    if (!sessionDetached()) {
      try { await bounded(session.detach(), 10000, 'final session detach'); teardown.attempts.push({ operation: 'session.detach', settled: true, detached: sessionDetached() }); }
      catch (error) { teardown.attempts.push({ operation: 'session.detach', settled: false, ...rawError(error) }); result.failures.push({ stage: 'final-detach', ...rawError(error) }); }
    }
  }
  try { teardown.sessionDetachedAtEnd = sessionDetached(); } catch (error) { teardown.sessionDetachedAtEnd = null; teardown.stateError = rawError(error); }
  if (result.created?.identity?.pid) {
    try { result.localProcessAfter = localPresence(); } catch (error) { result.localVerificationError = rawError(error); }
    try { result.fridaProcessAfter = (await bounded(device.enumerateProcesses(), 5000, 'Frida process enumeration')).find(item => item.pid === result.created.identity.pid) ?? null; } catch (error) { result.fridaVerificationError = rawError(error); }
  }
  result.finishedUtc = new Date().toISOString();
  result.teardown = teardown;
  const scriptCleanupComplete = teardown.scriptUnloadSettled || teardown.scriptCleanupDisposition === 'not_applicable_session_already_detached_after_target_exit';
  const windowsAbsent = result.localProcessAfter?.schema === 'owned-pid-presence-v1' && result.localProcessAfter.querySucceeded === true && result.localProcessAfter.present === false && result.localProcessAfter.processes.length === 0;
  result.outcome = result.readiness?.passed && result.failures.length === 0 && scriptCleanupComplete && teardown.sessionDetachedAtEnd === true && result.cleanup?.exit_verified && result.cleanup?.handles_closed && windowsAbsent && result.fridaProcessAfter === null ? 'PASS' : 'FAIL / incomplete';
  const resultPath = path.join(root, 'analysis', `resume202-visibility-fixture-lifecycle-${result.startedUtc.replaceAll(':', '-')}.json`);
  fs.writeFileSync(resultPath, JSON.stringify(result, null, 2) + '\n');
  result.evidencePath = resultPath;
  fs.writeFileSync(lifecycleReportPath, '# CP202 synthetic visibility fixture lifecycle repair\n\n' +
    `Outcome: **${result.outcome}**. Raw JSON receipt: \`${path.basename(resultPath)}\`.\n\n` +
    'The run uses only a newly created, converter-owned suspended Python child. It does not start the original application or acquire GUI control. Cleanup terminates the child through its retained process handle first; only then does the runner call Frida 17.23.2 Session.isDetached(). When the session is already detached, script cleanup is recorded as not applicable after target exit; no unload call is made or represented as successful. Independent CIM and Frida same-PID absence checks follow. Timeouts remain uncertain.\n\n' +
    'Historical attempts remain preserved in the original preflight report and attempt JSON. This report makes no claim about the original executable, Qt behavior, GUI behavior, or original interception.\n\n' +
    '```json\n' + JSON.stringify(result, null, 2) + '\n```\n');
  fs.writeFileSync(reportPath, '# CP202 synthetic visibility hook preflight\n\n' +
    'Outcome: **' + result.outcome + '**. Direct Frida 17.23.2 tested an aligned synthetic instruction hook in one newly created, held-suspended, converter-owned x64 Python child. The child main thread was never resumed. Agent execution allocated its own executable memory; no original executable, Qt code, GUI, or existing user process was used. No reconstructed DEF_PRE instance is involved.\n\n' +
    'The wrapper reserves 0x278 stack bytes, stores the known bool at RSP+0x74 and the synthetic main pointer at RSP+0x250, and executes the original candidate first 38 instruction bytes at page+0x40. Synthetic main+0x420 points to dock; dock points to an allocated vtable. The following instructions store post-block RAX/RCX/EDX for verification and return EDX. Baseline, hooked and restored calls exercise 0 and 1. The hook callback reads only the requested byte and pointer chain.\n\n' +
    'This establishes only the synthetic x64 allocation/layout case. It does not establish interception at original RVA 0x1547e, explain the earlier installation failure, reproduce original unwind metadata/branches/ASLR/module placement, or prove the original virtual callee executes. Changed byte offsets and span below describe observed differences, not an inferred internal Gum overwrite/trampoline length. Frida runtime/package identities are retained; separate Gum build identity is not exposed by this test.\n\n' +
  'The listener is detached, restored bytes compared and restored calls checked before child termination. See analysis/resume202-visibility-fixture-lifecycle-repair.md for bounded unload/detach outcomes and the retained-handle cleanup receipts. Historical failures remain preserved. No GUI-control session was acquired.\n\n' +
    '```json\n' + JSON.stringify(result, null, 2) + '\n```\n');
}
console.log(JSON.stringify({ outcome: result.outcome, evidencePath: result.evidencePath, teardown, cleanup: result.cleanup, localProcessAfter: result.localProcessAfter, fridaProcessAfter: result.fridaProcessAfter }));
process.exit(result.outcome === 'PASS' ? 0 : 1);
