// Single owned-child diagnostic. Never accepts a caller-selected target or PID.
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { execFileSync } from 'node:child_process';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const reportPath = path.join(root, 'analysis', 'resume202-direct-frida-smoketest.md');
const cwd = path.join(root, 'scratch', 'resume202-frida-selftest');
const exe = 'C:\\Program Files\\Python312\\python.exe';
const moduleUrl = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const argv = [exe, '-I', '-S', '-c', 'import time; time.sleep(60)'];
const result = { startedUtc: new Date().toISOString(), moduleUrl, target: exe, argv, cwd, stages: [], failures: [], pid: null, readiness: null };
let device, session, script, watchdog;
function errorRecord(error) {
  const record = { name: error?.name ?? null, message: error?.message ?? String(error), code: error?.code ?? null, details: error?.details ?? null };
  for (const key of Object.getOwnPropertyNames(error ?? {})) if (!(key in record)) record[key] = error[key];
  return record;
}
function stage(name, details = {}) { result.stages.push({ name, utc: new Date().toISOString(), ...details }); }
function localPresence() {
  const query = `Get-CimInstance Win32_Process -Filter 'ProcessId = ${result.pid}' | Select-Object ProcessId,ExecutablePath,CommandLine | ConvertTo-Json -Compress`;
  const raw = execFileSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', query], { encoding: 'utf8', windowsHide: true, timeout: 10000 }).trim();
  return raw ? JSON.parse(raw) : null;
}
function writeReport() {
  result.finishedUtc = new Date().toISOString();
  const passed = result.stages.some(s => s.name === 'resumed') && result.readiness?.cwd === cwd && result.localProcessAfter === null && result.fridaProcessAfter === null;
  result.outcome = passed ? 'PASS' : 'FAIL / incomplete';
  fs.writeFileSync(reportPath, '# Direct Frida owned-child smoke test\n\n' +
    'One direct Node/Frida attempt against a newly spawned isolated Python sleep process. No GUI application or existing user process was attached.\n\n' +
    'This diagnoses the direct binding path only; it does not prove that REA wrapper attachment or a reference-application capture works. Explicit cwd is established only if the injected GetCurrentDirectoryW result matches the requested directory.\n\n' +
    '```json\n' + JSON.stringify(result, null, 2) + '\n```\n');
}
async function cleanup() {
  if (result.pid === null) return;
  try { await device.kill(result.pid); stage('owned-child-killed'); }
  catch (error) {
    result.failures.push({ stage: 'kill', ...errorRecord(error) });
    // A fallback can act only if an independent query still identifies this exact test command.
    try {
      const current = localPresence();
      if (current && current.ExecutablePath?.toLowerCase() === exe.toLowerCase() && current.CommandLine?.includes('time.sleep(60)')) {
        process.kill(result.pid); stage('owned-child-killed-local-fallback');
      }
    } catch (error) { result.failures.push({ stage: 'fallback-kill', ...errorRecord(error) }); }
  }
  try { if (script) await script.unload(); } catch (error) { result.failures.push({ stage: 'unload-after-kill', ...errorRecord(error) }); }
  try { if (session && !session.isDetached) await session.detach(); } catch (error) { result.failures.push({ stage: 'detach', ...errorRecord(error) }); }
  for (let i = 0; i < 10; i++) {
    try { result.localProcessAfter = localPresence(); } catch (error) { result.localVerificationError = errorRecord(error); }
    try { result.fridaProcessAfter = (await device.enumerateProcesses()).find(p => p.pid === result.pid) ?? null; } catch (error) { result.fridaVerificationError = errorRecord(error); }
    if (result.localProcessAfter === null && result.fridaProcessAfter === null) break;
    await new Promise(resolve => setTimeout(resolve, 100));
  }
}
try {
  if (!fs.statSync(cwd).isDirectory() || !fs.statSync(exe).isFile()) throw new Error('Fixed target/cwd preflight failed');
  const packagePath = fileURLToPath(new URL('../../package.json', moduleUrl));
  result.fridaPackageVersion = JSON.parse(fs.readFileSync(packagePath, 'utf8')).version;
  if (result.fridaPackageVersion !== '17.23.2') throw new Error('Unexpected installed Frida version');
  const frida = await import(moduleUrl);
  device = await frida.getLocalDevice(); stage('local-device');
  result.pid = await device.spawn(argv, { cwd, stdio: 'pipe' }); stage('spawned-paused', { pid: result.pid });
  watchdog = setTimeout(async () => { result.failures.push({ stage: 'watchdog', message: '45 second limit exceeded' }); await cleanup(); writeReport(); process.exit(2); }, 45000);
  session = await device.attach(result.pid); stage('attached');
  script = await session.createScript(`
    const fn = new NativeFunction(Process.getModuleByName('kernel32.dll').getExportByName('GetCurrentDirectoryW'), 'uint32', ['uint32', 'pointer']);
    const out = Memory.alloc(65536);
    const n = fn(32768, out);
    send({kind: 'ready', pid: Process.id, cwd: n > 0 && n < 32768 ? out.readUtf16String(n) : null, cwdLength: n});
  `); stage('script-created');
  let readyResolve;
  const ready = new Promise(resolve => { readyResolve = resolve; });
  script.message.connect(message => { if (message.type === 'send' && message.payload?.kind === 'ready') { result.readiness = message.payload; readyResolve(); } else result.scriptMessages = [...(result.scriptMessages ?? []), message]; });
  await script.load(); stage('script-loaded');
  let readinessTimer;
  await Promise.race([ready, new Promise((_, reject) => { readinessTimer = setTimeout(() => reject(new Error('No readiness within five seconds')), 5000); })]).finally(() => clearTimeout(readinessTimer));
  stage('script-ready');
  if (result.readiness.pid !== result.pid || result.readiness.cwd !== cwd) throw new Error('Readiness PID/cwd mismatch');
  await device.resume(result.pid); stage('resumed');
  await new Promise(resolve => setTimeout(resolve, 250));
} catch (error) {
  result.failures.push({ stage: result.stages.at(-1)?.name ?? 'preflight', ...errorRecord(error) });
} finally {
  clearTimeout(watchdog);
  await cleanup();
  writeReport();
}
console.log(JSON.stringify(result));
process.exitCode = result.outcome === 'PASS' ? 0 : 1;
