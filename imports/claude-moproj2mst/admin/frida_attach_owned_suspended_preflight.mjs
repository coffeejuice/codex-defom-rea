// One diagnostic attempt. The Python helper alone owns/resumes/terminates its child.
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { createHash } from 'node:crypto';
import { spawn, execFileSync } from 'node:child_process';
import { createInterface } from 'node:readline';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const helperPath = path.join(root, 'admin', 'owned_suspended_process.py');
const reportPath = path.join(root, 'analysis', 'resume202-direct-attach-suspended.md');
const cwd = path.join(root, 'scratch', 'resume202-frida-selftest');
const exe = 'C:\\Program Files\\Python312\\python.exe';
const moduleUrl = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const digest = filename => createHash('sha256').update(fs.readFileSync(filename)).digest('hex');
const result = { startedUtc: new Date().toISOString(), moduleUrl, exe, cwd, maxSeconds: 20, helperPath, receipts: [], stages: [], failures: [], readiness: null };
let helper, device, session, script, helperExit, lines;
const waiters = [];
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
function localPresence() {
  const pid = result.created?.identity?.pid;
  if (!Number.isInteger(pid) || pid <= 0) throw new Error('No validated child PID');
  const raw = execFileSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', `Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' | Select-Object ProcessId,ExecutablePath,CommandLine | ConvertTo-Json -Compress`], { encoding: 'utf8', windowsHide: true, timeout: 10000 }).trim();
  return raw ? JSON.parse(raw) : null;
}
try {
  if (!fs.statSync(cwd).isDirectory()) throw new Error('Fixed scratch cwd does not exist');
  result.pythonSha256 = digest(exe);
  result.helperSha256 = digest(helperPath);
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
  script = await bounded(session.createScript(`send({kind:'ready',pid:Process.id});`), 2000, 'createScript');
  let readyResolve;
  const ready = new Promise(resolve => { readyResolve = resolve; });
  script.message.connect(message => { if (message.type === 'send' && message.payload?.kind === 'ready') { result.readiness = message.payload; readyResolve(); } else result.scriptMessages = [...(result.scriptMessages ?? []), message]; });
  await bounded(script.load(), 2000, 'script load'); stage('script-loaded');
  await bounded(ready, 2000, 'readiness');
  if (result.readiness.pid !== identity.pid) throw new Error('Readiness PID mismatch');
  stage('script-ready');
  command('resume');
  result.resumed = await receipt('resumed', 2000); stage('helper-resumed');
} catch (error) { result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', ...rawError(error) }); }
finally {
  // No PID-based kill: EOF/terminate/deadline all use the helper's retained handle.
  if (helper) {
    command('terminate');
    if (!helper.stdin.destroyed) helper.stdin.end();
    try { await bounded(helperExit, 26000, 'helper exit including watchdog'); } catch (error) { result.failures.push({ stage: 'helper-exit', ...rawError(error) }); }
    result.cleanup = result.receipts.find(item => item.event === 'cleanup') ?? null;
  }
  try { if (script && session && !session.isDetached) await bounded(script.unload(), 2000, 'unload'); } catch (error) { result.failures.push({ stage: 'unload', ...rawError(error) }); }
  try { if (session && !session.isDetached) await bounded(session.detach(), 2000, 'detach'); } catch (error) { result.failures.push({ stage: 'detach', ...rawError(error) }); }
  if (result.created?.identity?.pid) {
    try { result.localProcessAfter = localPresence(); } catch (error) { result.localVerificationError = rawError(error); }
    try { result.fridaProcessAfter = (await bounded(device.enumerateProcesses(), 5000, 'Frida process enumeration')).find(item => item.pid === result.created.identity.pid) ?? null; } catch (error) { result.fridaVerificationError = rawError(error); }
  }
  result.finishedUtc = new Date().toISOString();
  result.outcome = result.stages.some(s => s.name === 'helper-resumed') && result.cleanup?.exit_verified && result.cleanup?.handles_closed && result.localProcessAfter === null && result.fridaProcessAfter === null ? 'PASS' : 'FAIL / incomplete';
  fs.writeFileSync(reportPath, '# Direct Frida attach to an owned CREATE_SUSPENDED child\n\n' +
    'One attempt against an isolated Python sleep child created by the existing helper. The helper retains the original process/thread handles and enforces its 20 second watchdog. No GUI or existing process was targeted. Cleanup uses helper terminate/EOF and its exact held handle; no PID-based kill is used.\n\n' +
    'This tests attachment before helper ResumeThread; it does not establish attachment to the reference application or REA guardian integration. A success would support this helper as a candidate ownership path, subject to integration checks.\n\n```json\n' + JSON.stringify(result, null, 2) + '\n```\n');
}
console.log(JSON.stringify(result));
process.exit(result.outcome === 'PASS' ? 0 : 1);
