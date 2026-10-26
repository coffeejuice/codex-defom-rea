#!/usr/bin/env node
// One-attempt uninstrumented GUI startup harness. Deliberately disabled by default.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { spawn, execFile } from 'node:child_process';
import { promisify } from 'node:util';
import { fileURLToPath } from 'node:url';

const RUN_UNINSTRUMENTED_BASELINE = false;
const OBSERVE_MS = 5_000;
const TIMEOUT_SECONDS = 20;
const HELPER_SHA256 = '8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d';
const PRIOR_RUN = 'resume202-passive-oracle-run-2026-10-26T04-45-03-155Z';
const BASELINE_DISPOSABLE_SHA256 = 'a18fcae942c1a9c1e67094793fc7cc311567c783659c18b56b2e5dd7a88eb6d0';
const BASELINE_SOURCE_SHA256 = '45a238c188488825eabb524b3a450cb8d865a9077b7586078fed2c3d146f3a64';
const EXE_SHA256 = 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc';
const execFileAsync = promisify(execFile);
const HERE = path.dirname(fileURLToPath(import.meta.url));
const CONVERTER = path.resolve(HERE, '..');
const RUNS = path.join(CONVERTER, 'analysis');
const HELPER = path.join(HERE, 'owned_suspended_process.py');
const EXPECTED_DISPOSABLE = path.join(RUNS, PRIOR_RUN, 'disposable-before.json');
const EXPECTED_SOURCE = path.join(RUNS, PRIOR_RUN, 'source-before.json');
const DISPOSABLE = 'C:\\PROBLEM\\p202a';
const SOURCE = path.join(CONVERTER, 'evidence', 'gui_mo', 'oracle_b', '98_B_DS3F_before');
const EXE = 'C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\DEF_GUI_MO_64.exe';
const PROJECT = path.join(DISPOSABLE, 'Gear_blank.moproj');
const ARGS = ['-m', 'open', '-f', PROJECT];

function digest(file) {
  return crypto.createHash('sha256').update(fs.readFileSync(file)).digest('hex');
}

function inventory(root) {
  const entries = [];
  function walk(dir, relative = '') {
    for (const item of fs.readdirSync(dir, { withFileTypes: true })) {
      const rel = path.posix.join(relative, item.name);
      const full = path.join(dir, item.name);
      if (item.isSymbolicLink()) throw new Error(`Reparse/symlink entry is unsupported: ${full}`);
      if (item.isDirectory()) {
        entries.push({ kind: 'directory', path: rel });
        walk(full, rel);
      } else if (item.isFile()) {
        const stat = fs.statSync(full);
        entries.push({ kind: 'file', path: rel, bytes: String(stat.size), sha256: digest(full) });
      } else throw new Error(`Unexpected filesystem entry: ${full}`);
    }
  }
  walk(root);
  entries.sort((a, b) => a.path < b.path ? -1 : a.path > b.path ? 1 : 0);
  return { root, entries };
}

function projection(manifest) {
  return manifest.entries.map(({ kind, path: relative, bytes, sha256 }) =>
    ({ kind, path: relative, ...(bytes ? { bytes, sha256 } : {}) }));
}

function compareInventory(actual, expected) {
  const sort = entries => entries.sort((a, b) => a.path < b.path ? -1 : a.path > b.path ? 1 : 0);
  return JSON.stringify(sort(projection(actual))) === JSON.stringify(sort(projection(expected)));
}

function parseEvent(line) {
  // Preserve Win32 FILETIME's 64-bit integer exactly across JSON.parse.
  return JSON.parse(line.replace(/("creation_filetime"\s*:\s*)(\d+)/g, '$1"$2"'));
}

function waitForEvent(events, name, child, deadline) {
  return new Promise((resolve, reject) => {
    const poll = () => {
      const found = events.find(e => e.event === name);
      if (found) return resolve(found);
      const failure = events.find(e => e.event === 'error' || e.event === 'cleanup_error');
      if (failure) return reject(new Error(JSON.stringify(failure)));
      if (child.exitCode !== null) return reject(new Error(`helper exited early (${child.exitCode})`));
      if (Date.now() >= deadline) return reject(new Error(`helper did not emit ${name} before deadline`));
      setTimeout(poll, 25);
    };
    poll();
  });
}

function waitForClose(child, maxMs) {
  if (child.exitCode !== null) return Promise.resolve({ code: child.exitCode, signal: child.signalCode });
  return new Promise((resolve, reject) => {
    const timer = setTimeout(() => reject(new Error('Helper launcher close was not observed before bounded deadline.')), maxMs);
    child.once('error', error => { clearTimeout(timer); reject(error); });
    child.once('close', (code, signal) => { clearTimeout(timer); resolve({ code, signal }); });
  });
}

async function windowsIdentityAbsent(identity) {
  const ps = `$ErrorActionPreference='Stop'; try {$p=Get-CimInstance Win32_Process -Filter 'ProcessId = ${Number(identity.pid)}' -ErrorAction Stop; if ($null -eq $p) {$result=[ordered]@{ok=$true;absent=$true}} else {$creation=$null; if ($null -ne $p.CreationDate -and [string]$p.CreationDate) {$creation=([datetime]$p.CreationDate).ToUniversalTime().ToFileTimeUtc().ToString()}; $result=[ordered]@{ok=$true;absent=$false;ProcessId=[int]$p.ProcessId;CreationFiletime=$creation;ExecutablePath=[string]$p.ExecutablePath}}} catch {$result=[ordered]@{ok=$false;error=$_.Exception.Message}}; ConvertTo-Json -InputObject $result -Depth 4 -Compress`;
  const { stdout } = await execFileAsync('C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe',
    ['-NoProfile', '-NonInteractive', '-Command', ps], { windowsHide: true, timeout: 5000 });
  const found = normalizeCimEnvelope(stdout);
  if (found.ok !== true) throw new Error(`Windows process query failed: ${found.error ?? 'invalid response'}`);
  if (found.absent === true) return { checked: true, originalIdentityAbsent: true, observed: null };
  if (found.absent !== false || found.ProcessId !== identity.pid ||
      typeof found.ExecutablePath !== 'string' || !found.ExecutablePath.trim() ||
      typeof found.CreationFiletime !== 'string' || !/^\d{18,}$/.test(found.CreationFiletime)) {
    throw new Error('Windows process query returned an incomplete identity; absence is unknown.');
  }
  // CIM DateTime can lose 100 ns FILETIME precision. A successful row for the
  // exact PID is therefore always treated as present; PID reuse is not inferred.
  return { checked: true, originalIdentityAbsent: false, observed: found,
    exactImageMatch: path.resolve(found.ExecutablePath).toLowerCase() === path.resolve(identity.canonical_image).toLowerCase(),
    exactCreationFiletimeMatch: String(found.CreationFiletime) === String(identity.creation_filetime),
    qualification: 'any successful CIM row for the original PID fails absence; CIM time precision is not used to infer PID reuse' };
}

function normalizeCimEnvelope(stdout) {
  const text = String(stdout).trim();
  if (!text.startsWith('{') || !text.endsWith('}')) throw new Error(`CIM query did not return a JSON object: ${JSON.stringify(text)}`);
  const found = JSON.parse(text);
  if (!found || typeof found !== 'object' || Array.isArray(found)) throw new Error('CIM result is not an object.');
  return found;
}

async function selfTestCimMocks() {
  const identity = { pid: 1234, canonical_image: 'C:\\x.exe', creation_filetime: '134000000000000000' };
  const ps = `$ErrorActionPreference='Stop'; function New-Envelope($p) {if ($null -eq $p) {[ordered]@{ok=$true;absent=$true}} else {$created=$null; if ($null -ne $p.CreationDate -and [string]$p.CreationDate) {$created=([datetime]$p.CreationDate).ToUniversalTime().ToFileTimeUtc().ToString()}; [ordered]@{ok=$true;absent=$false;ProcessId=[int]$p.ProcessId;CreationFiletime=$created;ExecutablePath=[string]$p.ExecutablePath}}}; $zero=New-Envelope $null; $row=[pscustomobject]@{ProcessId=1234;CreationDate='2026-10-26T10:00:00.0000000Z';ExecutablePath='C:\\x.exe'}; $present=New-Envelope $row; $errorEnvelope=[ordered]@{ok=$false;error='access denied'}; ConvertTo-Json -InputObject ([ordered]@{zero=$zero;present=$present;error=$errorEnvelope}) -Depth 5 -Compress`;
  const { stdout } = await execFileAsync('C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe',
    ['-NoProfile', '-NonInteractive', '-Command', ps], { windowsHide: true, timeout: 5000 });
  const mocks = JSON.parse(stdout.trim());
  const absent = normalizeCimEnvelope(JSON.stringify(mocks.zero));
  if (absent.ok !== true || absent.absent !== true) throw new Error('zero-row mock did not serialize as an explicit absence envelope');
  const present = normalizeCimEnvelope(JSON.stringify(mocks.present));
  if (present.ok !== true || present.absent !== false || present.ProcessId !== identity.pid) throw new Error('present-row mock failed envelope validation');
  let errorRejected = false;
  try { const response = normalizeCimEnvelope(JSON.stringify(mocks.error)); if (response.ok !== true) throw new Error(response.error); }
  catch { errorRejected = true; }
  if (!errorRejected) throw new Error('query-error mock was accepted');
  let legacyRejected = false;
  try { normalizeCimEnvelope('ok absent'); } catch { legacyRejected = true; }
  if (!legacyRejected) throw new Error('legacy plain-text output was accepted');
  return { passed: true, cases: ['zero_rows_explicit_json', 'present_pid_row', 'query_error_rejected', 'legacy_text_rejected'], serializer: 'Windows PowerShell ConvertTo-Json mock envelopes' };
}

function identityMatches(identity, launcherPid) {
  return identity && Number.isInteger(identity.pid) && identity.pid > 0 &&
    identity.launcher_pid === launcherPid && Number.isInteger(identity.primary_tid) && identity.primary_tid > 0 &&
    typeof identity.creation_filetime === 'string' && /^\d{18,}$/.test(identity.creation_filetime) &&
    path.resolve(identity.canonical_image).toLowerCase() === path.resolve(EXE).toLowerCase() &&
    identity.sha256 === EXE_SHA256 && path.resolve(identity.cwd).toLowerCase() === path.resolve(DISPOSABLE).toLowerCase();
}

async function runOnce() {
  if (!RUN_UNINSTRUMENTED_BASELINE) throw new Error('Guard is false; execution is disabled.');
  if (process.platform !== 'win32') throw new Error('Windows is required.');
  const priorDir = path.join(RUNS, PRIOR_RUN);
  const expectedDisposable = JSON.parse(fs.readFileSync(EXPECTED_DISPOSABLE, 'utf8'));
  const expectedSource = JSON.parse(fs.readFileSync(EXPECTED_SOURCE, 'utf8'));
  if (digest(EXPECTED_DISPOSABLE) !== BASELINE_DISPOSABLE_SHA256) throw new Error('Pinned disposable manifest changed.');
  if (digest(EXPECTED_SOURCE) !== BASELINE_SOURCE_SHA256) throw new Error('Pinned B98 source manifest changed.');
  if (path.resolve(expectedDisposable.root).toLowerCase() !== path.resolve(DISPOSABLE).toLowerCase()) throw new Error('Disposable manifest root mismatch.');
  if (path.resolve(expectedSource.root).toLowerCase() !== path.resolve(SOURCE).toLowerCase()) throw new Error('Source manifest root mismatch.');
  const initialDisposable = inventory(DISPOSABLE);
  const initialSource = inventory(SOURCE);
  if (!compareInventory(initialDisposable, expectedDisposable)) throw new Error('C:\\PROBLEM\\p202a differs from the retained B98 disposable baseline.');
  if (!compareInventory(initialSource, expectedSource)) throw new Error('B98 source differs from its retained source manifest.');
  if (initialDisposable.entries.filter(e => e.kind === 'file').length !== 27) throw new Error('Expected exactly 27 disposable files.');
  if (digest(EXE) !== EXE_SHA256) throw new Error('Original GUI executable hash mismatch.');
  const helperHash = digest(HELPER);
  if (helperHash !== HELPER_SHA256) throw new Error('Owned-process helper hash mismatch.');
  if (!fs.statSync(PROJECT).isFile()) throw new Error('B98 project file is missing.');

  const stamp = new Date().toISOString().replaceAll(':', '-').replaceAll('.', '-');
  const outDir = path.join(RUNS, `resume202-uninstrumented-baseline-${stamp}`);
  fs.mkdirSync(outDir, { recursive: false });
  const before = { executable: { path: EXE, sha256: digest(EXE) }, helper: { path: HELPER, sha256: helperHash },
    disposable: inventory(DISPOSABLE), source: inventory(SOURCE), startedUtc: new Date().toISOString() };
  fs.writeFileSync(path.join(outDir, 'before.json'), JSON.stringify(before, null, 2), { flag: 'wx' });

  const python = 'C:\\Program Files\\Python312\\python.exe';
  const child = spawn(python, [HELPER, '--exe', EXE, '--cwd', DISPOSABLE, '--sha256', EXE_SHA256,
    '--max-seconds', String(TIMEOUT_SECONDS), '--', ...ARGS], { cwd: DISPOSABLE, windowsHide: true, stdio: ['pipe', 'pipe', 'pipe'] });
  let rawStdout = '', stdoutPending = '', rawStderr = '';
  const events = [], parseErrors = [];
  child.stdout.setEncoding('utf8').on('data', chunk => {
    rawStdout += chunk;
    stdoutPending += chunk;
    let newline;
    while ((newline = stdoutPending.indexOf('\n')) >= 0) {
      const line = stdoutPending.slice(0, newline).trim();
      stdoutPending = stdoutPending.slice(newline + 1);
      if (line) {
        try { events.push(parseEvent(line)); }
        catch (error) { parseErrors.push({ line, error: String(error) }); }
      }
    }
  });
  child.stderr.setEncoding('utf8').on('data', chunk => { rawStderr += chunk; });
  const deadline = Date.now() + (TIMEOUT_SECONDS + 10) * 1000;
  let created = null, resume = null, exit = null, flowError = null, absent = null;
  try {
    created = await waitForEvent(events, 'created', child, deadline);
    if (created.state !== 'suspended' || created.exit_verified !== false) throw new Error('Created receipt was not a live suspended process.');
    if (!identityMatches(created.identity, child.pid)) throw new Error('Created receipt identity did not match requested PID/image/hash/cwd/launcher.');
    child.stdin.write('{"command":"resume"}\n');
    resume = await waitForEvent(events, 'resumed', child, deadline);
    if (resume.previous_suspend_count !== 1) throw new Error('Unexpected primary-thread suspend count.');
    const until = Date.now() + OBSERVE_MS;
    while (Date.now() < until && child.exitCode === null && !events.some(e => e.event === 'endpoint')) {
      await new Promise(resolve => setTimeout(resolve, 25));
    }
    if (child.exitCode === null && !events.some(e => e.event === 'endpoint')) child.stdin.write('{"command":"terminate"}\n');
    child.stdin.end(); // EOF also requests held-handle cleanup if endpoint races the command.
  } catch (error) {
    flowError = String(error);
    try { child.stdin.end(); } catch { /* retain helper state and collect its receipts */ }
  }
  try { exit = await waitForClose(child, TIMEOUT_SECONDS * 1000 + 10_000); }
  catch (error) { flowError = [flowError, String(error)].filter(Boolean).join('; '); }

  if (stdoutPending.trim()) {
    try { events.push(parseEvent(stdoutPending.trim())); }
    catch (error) { parseErrors.push({ line: stdoutPending, error: String(error) }); }
  }
  fs.writeFileSync(path.join(outDir, 'helper.stdout.raw'), rawStdout, { flag: 'wx' });
  fs.writeFileSync(path.join(outDir, 'helper.stderr.raw'), rawStderr, { flag: 'wx' });
  fs.writeFileSync(path.join(outDir, 'helper.events.json'), JSON.stringify(events, null, 2), { flag: 'wx' });
  fs.writeFileSync(path.join(outDir, 'helper.parse-errors.json'), JSON.stringify(parseErrors, null, 2), { flag: 'wx' });

  const cleanup = events.find(e => e.event === 'cleanup');
  let absenceError = null;
  if (created?.identity) {
    try { absent = await windowsIdentityAbsent(created.identity); }
    catch (error) { absenceError = String(error); }
  }
  const captureAfter = (name, fn) => {
    try { return { status: 'captured', value: fn() }; }
    catch (error) { return { status: 'error', error: String(error) }; }
  };
  const after = { executable: captureAfter('executable', () => ({ path: EXE, sha256: digest(EXE) })),
    disposable: captureAfter('disposable', () => inventory(DISPOSABLE)),
    source: captureAfter('source', () => inventory(SOURCE)), processAbsence: absent,
    completedUtc: new Date().toISOString() };
  const afterErrors = Object.fromEntries(Object.entries(after).filter(([, item]) => item?.status === 'error').map(([name, item]) => [name, item.error]));
  fs.writeFileSync(path.join(outDir, 'after.json'), JSON.stringify(after, null, 2), { flag: 'wx' });
  const report = { oneAttempt: true, instrumentation: false, frida: false, guiInput: false,
    observationWindowMs: OBSERVE_MS, watchdogSeconds: TIMEOUT_SECONDS,
    expectedDisposableManifestSha256: BASELINE_DISPOSABLE_SHA256, helperSha256: helperHash,
    executableSha256: EXE_SHA256, created, resume, helperExit: exit, cleanup,
    processAbsence: absent, absenceError, afterErrors, flowError, parseErrors,
    executableUnchanged: after.executable.status === 'captured' && after.executable.value.sha256 === before.executable.sha256,
    disposableUnchanged: after.disposable.status === 'captured' && compareInventory(after.disposable.value, before.disposable),
    sourceUnchanged: after.source.status === 'captured' && compareInventory(after.source.value, before.source), outDir };
  fs.writeFileSync(path.join(outDir, 'result.json'), JSON.stringify(report, null, 2), { flag: 'wx' });
  if (flowError || parseErrors.length || exit?.code !== 0 || !cleanup?.exit_verified || !cleanup?.handles_closed ||
      !absent?.originalIdentityAbsent || !report.executableUnchanged || !report.disposableUnchanged || !report.sourceUnchanged || Object.keys(afterErrors).length) {
    throw new Error(`Baseline incomplete or integrity/cleanup gate failed; see ${path.join(outDir, 'result.json')}`);
  }
  return report;
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url) && process.argv[2] === '--self-test-cim') {
  try { process.stdout.write(JSON.stringify(await selfTestCimMocks()) + '\n'); }
  catch (error) { process.stderr.write(`${error.stack ?? error}\n`); process.exitCode = 1; }
} else if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  runOnce().then(report => process.stdout.write(JSON.stringify(report, null, 2) + '\n'))
    .catch(error => { process.stderr.write(`${error.stack ?? error}\n`); process.exitCode = 2; });
}
