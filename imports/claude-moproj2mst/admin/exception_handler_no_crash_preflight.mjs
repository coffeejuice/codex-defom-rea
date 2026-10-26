// Harmless Frida exception-handler registration preflight.
// Native execution stays disabled until separately reviewed by the coordinator.
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
import { spawn, execFileSync } from 'node:child_process';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const HELPER = path.join(ROOT, 'admin', 'owned_suspended_process.py');
const REPORT = path.join(ROOT, 'analysis', 'resume203-exception-handler-no-crash-preflight.md');
const CWD = 'C:\\PROBLEM';
const PYTHON = 'C:\\Program Files\\Python312\\python.exe';
const EXPECTED_HELPER_SHA256 = '8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d';
const FRIDA_URL = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const CHILD_CODE = 'import time; time.sleep(60)';
const MAX_SECONDS = 20;
const RUN_NATIVE = false; // Keep false until root review explicitly enables the run.
const sha = x => crypto.createHash('sha256').update(x).digest('hex');
const fileSha = p => sha(fs.readFileSync(p));
const requireThat = (ok, message) => { if (!ok) throw new Error(message); };
const result = { schema: 'owned-python-exception-handler-no-crash-preflight-v1', startedUtc: new Date().toISOString(),
  host: os.hostname(), target: { python: PYTHON, cwd: CWD, childCode: CHILD_CODE },
  guiInput: false, guiControlAcquired: false, interceptorHooks: 0, maxSeconds: MAX_SECONDS,
  nativeExecutionEnabled: RUN_NATIVE, stages: [], failures: [], helperRawReceipts: [], scriptMessages: [] };
let device, session, script, helper, helperExit, helperBuffer = '', stdoutClosed = false;
let childIdentity = null, readyResolve, readyReject, handlerReadyResolve, handlerReadyReject, stderr = '';
const ready = new Promise((resolve, reject) => { readyResolve = resolve; readyReject = reject; });
const handlerReadyPromise = new Promise((resolve, reject) => { handlerReadyResolve = resolve; handlerReadyReject = reject; });
handlerReadyPromise.catch(() => {}); // Prevent an early script-error rejection from becoming unhandled before it is awaited.
let stdoutEndResolve;
const stdoutEnded = new Promise(resolve => { stdoutEndResolve = resolve; });
const stage = name => result.stages.push({ name, utc: new Date().toISOString() });
function bounded(p, ms, label) {
  let timer;
  return Promise.race([p, new Promise((_, reject) => { timer = setTimeout(() => reject(new Error(`${label} timeout (${ms} ms)`)), ms); })])
    .finally(() => clearTimeout(timer));
}
function helperCommand(command) { if (helper && !helper.stdin.destroyed) helper.stdin.write(JSON.stringify({ command }) + '\n'); }
function presence(pid) {
  const q = `$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' -ErrorAction Stop | Select-Object ProcessId,ExecutablePath,CommandLine); [pscustomobject]@{pid=${pid};present=[bool]($rows.Count -gt 0);processes=@($rows)} | ConvertTo-Json -Depth 3 -Compress`;
  const s = execFileSync('powershell.exe', ['-NoProfile','-NonInteractive','-Command',q], { encoding:'utf8', windowsHide:true, timeout:8000 }).trim();
  const v = JSON.parse(s);
  requireThat(v.pid === pid && Array.isArray(v.processes) && v.processes.length <= 1 &&
    v.processes.every(row => row && row.ProcessId === pid) && v.present === (v.processes.length === 1), 'Invalid independent Windows process presence receipt');
  return v;
}
function injected() {
  return `
  Process.setExceptionHandler(_details => false);
  send({kind:'handler-ready',pid:Process.id,arch:Process.arch,platform:Process.platform,registrationCallCompleted:true});
  `;
}

async function run() {
  if (!RUN_NATIVE) {
    result.outcome = 'GUARDED';
    result.guardReason = 'Native execution disabled pending root review';
    result.finishedUtc = new Date().toISOString();
    fs.writeFileSync(REPORT, renderReport(result), { flag: 'w' });
    console.log(JSON.stringify({ outcome: result.outcome, report: REPORT, nativeExecutionEnabled: false }));
    return;
  }
  try {
    requireThat(process.platform === 'win32', 'Windows only');
    requireThat(fs.statSync(PYTHON).isFile() && fs.statSync(HELPER).isFile() && fs.statSync(CWD).isDirectory(), 'Fixed runtime/helper/cwd missing');
    result.pythonSha256 = fileSha(PYTHON); result.helperSha256 = fileSha(HELPER);
    requireThat(result.helperSha256 === EXPECTED_HELPER_SHA256, 'Owned-process helper SHA-256 does not match reviewed pin');
    const fridaPkg = fileURLToPath(new URL('../../package.json', FRIDA_URL));
    result.fridaVersion = JSON.parse(fs.readFileSync(fridaPkg,'utf8')).version;
    requireThat(result.fridaVersion === '17.23.2', 'Required direct Frida version 17.23.2 unavailable');
    result.fridaPackagePath = fridaPkg; result.fridaPackageSha256 = fileSha(fridaPkg);
    const frida = await import(FRIDA_URL);
    device = await frida.getLocalDevice(); result.fridaDeviceId = device.id; stage('frida-local-device');
    const argv = ['-I','-S',HELPER,'--exe',PYTHON,'--cwd',CWD,'--sha256',result.pythonSha256,'--max-seconds',String(MAX_SECONDS),'--','-I','-S','-c',CHILD_CODE];
    result.helperArgv = [PYTHON,...argv];
    helper = spawn(PYTHON, argv, { cwd:CWD, windowsHide:true, stdio:['pipe','pipe','pipe'] });
    helper.stdin.on('error', e => result.failures.push({stage:'helper-stdin',message:e.message}));
    helper.stderr.on('data', b => { if (stderr.length<4096) stderr += b.toString().slice(0,4096-stderr.length); });
    helper.stdout.setEncoding('utf8');
    helperExit = new Promise((resolve,reject) => { helper.once('error',reject); helper.once('exit',(code,signal)=>{ result.helperExit={code,signal}; resolve(); }); });
    const counts = { created:0, resumed:0, endpoint:0, cleanup:0 };
    let protocolFailed = false;
    function protocolFailure(message) {
      if (protocolFailed) return;
      protocolFailed = true;
      const e = new Error(`Helper JSONL protocol: ${message}`);
      result.failures.push({stage:'helper-protocol',message:e.message});
      readyReject(e); handlerReadyReject(e);
    }
    helper.stdout.on('data', chunk => {
      helperBuffer += chunk;
      if (Buffer.byteLength(helperBuffer)>65536) { const e=new Error('Helper stdout exceeded bound'); readyReject(e); return; }
      let i; while ((i=helperBuffer.indexOf('\n'))>=0) {
        const line=helperBuffer.slice(0,i); helperBuffer=helperBuffer.slice(i+1);
        try {
          const row=JSON.parse(line); result.helperRawReceipts.push(row);
          if (!row || typeof row !== 'object' || Array.isArray(row) || !['created','resumed','endpoint','cleanup','error','cleanup_error'].includes(row.event)) throw new Error('unexpected event/schema');
          if (row.event==='error'||row.event==='cleanup_error') throw new Error(`helper ${row.event}: ${JSON.stringify(row)}`);
          if (row.event==='created') {
            counts.created++;
            const keys=['event','identity','state','wait_result','exit_code','exit_verified','gui_control_acquired'];
            if(counts.created!==1 || result.helperRawReceipts.length!==1 || Object.keys(row).sort().join('|')!==keys.sort().join('|')) throw new Error('duplicate, out-of-order, or malformed created receipt');
            childIdentity=row.identity; result.createdReceipt=row; readyResolve(row);
          } else if (row.event==='resumed') {
            counts.resumed++;
            const keys=['event','state','wait_result','exit_code','exit_verified','previous_suspend_count'];
            if(counts.created!==1 || counts.resumed!==1 || counts.endpoint || counts.cleanup || Object.keys(row).sort().join('|')!==keys.sort().join('|')) throw new Error('duplicate, out-of-order, or malformed resumed receipt');
            result.resumedReceipt=row;
          } else if (row.event==='endpoint') {
            counts.endpoint++;
            if(counts.created!==1 || counts.resumed!==1 || counts.endpoint!==1 || counts.cleanup || Object.keys(row).sort().join('|')!=='event|reason' || row.reason!=='terminate_command') throw new Error('invalid endpoint receipt');
            result.endpointReceipt=row;
          } else if (row.event==='cleanup') {
            counts.cleanup++;
            const keys=['event','state','wait_result','exit_code','exit_verified','termination_requested','termination_attempts','handles_closed'];
            if(counts.created!==1 || counts.cleanup!==1 || counts.endpoint!==1 || Object.keys(row).sort().join('|')!==keys.sort().join('|')) throw new Error('duplicate, out-of-order, or malformed cleanup receipt');
            result.cleanupReceipt=row;
          }
        } catch(e) { result.failures.push({stage:'helper-json',message:e.message}); readyReject(e); }
      }
    });
    helper.stdout.on('end',()=>{stdoutClosed=true; stdoutEndResolve(); if(helperBuffer) protocolFailure('unterminated trailing JSONL data');});
    stage('helper-started');
    const created=await bounded(ready,5000,'created receipt');
    const expectedArgv=[PYTHON,'-I','-S','-c',CHILD_CODE];
    const argvMatches=Array.isArray(childIdentity?.argv) && childIdentity.argv.length===expectedArgv.length &&
      childIdentity.argv[0]?.toLowerCase()===expectedArgv[0].toLowerCase() && expectedArgv.slice(1).every((arg,i)=>childIdentity.argv[i+1]===arg);
    requireThat(created.state==='suspended' && created.wait_result===258 && created.exit_verified===false && created.gui_control_acquired===false &&
      Number.isInteger(childIdentity?.pid) && childIdentity.pid>0 && Number.isInteger(childIdentity?.primary_tid) && childIdentity.primary_tid>0 &&
      Number.isFinite(childIdentity?.creation_filetime) && Number.isInteger(childIdentity.creation_filetime) && childIdentity.creation_filetime>0 && childIdentity.launcher_pid===helper.pid &&
      childIdentity.host===os.hostname() && childIdentity.sha256===result.pythonSha256 && childIdentity.canonical_image.toLowerCase()===PYTHON.toLowerCase() &&
      childIdentity.cwd.toLowerCase()===CWD.toLowerCase() && argvMatches, 'Owned suspended child identity mismatch');
    const identityKeys=['pid','primary_tid','creation_filetime','canonical_image','sha256','cwd','argv','host','launcher_pid'];
    requireThat(Object.keys(childIdentity).sort().join('|')===identityKeys.sort().join('|'), 'Created identity schema mismatch');
    result.ownedIdentity=childIdentity; stage('owned-harmless-python-child-created-suspended');
    session=await bounded(device.attach(childIdentity.pid),12000,'direct Frida attach'); stage('frida-attached-before-resume');
    script=await bounded(session.createScript(injected()),3000,'create exception handler script');
    script.message.connect(message=>{
      result.scriptMessages.push(message);
      if (message.type==='send' && message.payload?.kind==='handler-ready' && message.payload.pid===childIdentity.pid) { result.handlerReady=message.payload; handlerReadyResolve(message.payload); }
      if (message.type==='error') { result.scriptError={description:message.description??null,stack:message.stack??null}; const e=new Error('Frida injected script error'); result.failures.push({stage:'frida-script',message:e.message,details:result.scriptError}); handlerReadyReject(e); }
    });
    await bounded(script.load(),3000,'script load');
    result.handlerReady=await bounded(handlerReadyPromise,3000,'exception handler registration');
    requireThat(result.handlerReady.arch==='x64' && result.handlerReady.platform==='windows' && result.handlerReady.registrationCallCompleted===true,'Handler readiness mismatch');
    stage('handler-installed-before-resume');
    helperCommand('resume');
    const resumeLine=await waitReceipt('resumed',4000);
    requireThat(resumeLine.previous_suspend_count===1 && ['running','exited'].includes(resumeLine.state),'Resume receipt mismatch');
    result.resumedReceipt=resumeLine; stage('resumed-harmless-sleeper');
    // The only termination request is sent to the owner helper, which uses its retained process handle.
    helperCommand('terminate');
    stage('exact-held-handle-termination-requested');
  } catch(e) { result.failures.push({stage:result.stages.at(-1)?.name??'preflight',name:e?.name??'Error',message:String(e?.message??e).slice(0,2000)}); }
  finally {
    if (helper && !helper.stdin.destroyed) helper.stdin.end();
    if (helperExit) try { await bounded(helperExit,22000,'helper exit and exact-handle cleanup'); await bounded(stdoutEnded,2000,'helper stdout close'); } catch(e) { result.failures.push({stage:'helper-exit',message:String(e?.message??e)}); }
    result.helperStderr=stderr; result.helperStdoutClosed=stdoutClosed; if(helperBuffer) result.helperPartialLine=helperBuffer;
    try {
      if (script && session) {
        if (typeof session.isDetached!=='function') throw new Error('Frida Session.isDetached unavailable');
        if (!session.isDetached()) await bounded(script.unload(),2000,'script unload');
        result.scriptUnloadSettled=true;
      } else result.scriptUnloadSettled=true;
    } catch(e) { result.failures.push({stage:'script-unload',message:String(e?.message??e)}); }
    try {
      if(session) { if(!session.isDetached()) await bounded(session.detach(),2000,'session detach'); result.sessionDetached=session.isDetached(); }
      else result.sessionDetached=true;
    } catch(e) { result.sessionDetached=false; result.failures.push({stage:'session-detach',message:String(e?.message??e)}); }
    if(childIdentity?.pid) {
      try { result.windowsAfter=presence(childIdentity.pid); result.windowsAbsent=result.windowsAfter.present===false; }
      catch(e) { result.windowsAbsent=false; result.failures.push({stage:'windows-absence',message:String(e?.message??e)}); }
      try { const rows=await bounded(device.enumerateProcesses(),4000,'Frida process absence'); const matches=rows.filter(p=>p.pid===childIdentity.pid); result.fridaAfter={deviceId:device.id,matches}; result.fridaAbsent=matches.length===0; }
      catch(e) { result.fridaAbsent=false; result.failures.push({stage:'frida-absence',message:String(e?.message??e)}); }
    } else { result.windowsAbsent=false; result.fridaAbsent=false; }
    result.cleanupVerified=result.cleanupReceipt?.exit_verified===true && result.cleanupReceipt?.handles_closed===true && result.cleanupReceipt?.wait_result===0 && result.cleanupReceipt?.termination_requested===true && result.cleanupReceipt?.termination_attempts?.length===1 && result.cleanupReceipt?.termination_attempts?.[0]?.source==='final_cleanup' && result.cleanupReceipt?.termination_attempts?.[0]?.requested_exit_code===0xe201 && result.cleanupReceipt?.termination_attempts?.[0]?.exit_verified===true && result.helperExit?.code===0 && stdoutClosed && helperBuffer==='';
    result.processAbsent=result.windowsAbsent===true && result.fridaAbsent===true;
    result.registrationVerified=result.handlerReady?.registrationCallCompleted===true && result.handlerReady?.pid===childIdentity?.pid;
    result.noCrashCoverage=true;
    result.resumeVerified=result.resumedReceipt?.previous_suspend_count===1 && ['running','exited'].includes(result.resumedReceipt?.state) && result.stages.some(x=>x.name==='resumed-harmless-sleeper');
    result.terminationPathVerified=result.endpointReceipt?.reason==='terminate_command' && result.resumeVerified && result.cleanupReceipt?.termination_attempts?.[0]?.source==='final_cleanup';
    result.captureComplete=result.registrationVerified===true && result.resumeVerified && result.terminationPathVerified && result.cleanupVerified && result.processAbsent && result.sessionDetached===true && result.failures.length===0;
    result.finishedUtc=new Date().toISOString(); result.outcome=result.captureComplete?'PASS':'INCOMPLETE';
    fs.writeFileSync(REPORT,renderReport(result),{flag:'w'});
  }
  console.log(JSON.stringify({outcome:result.outcome,report:REPORT,captureComplete:result.captureComplete===true}));
  process.exitCode=result.captureComplete?0:1;
}
async function waitReceipt(event, ms) {
  const until=Date.now()+ms;
  while(Date.now()<=until) {
    const row=result.helperRawReceipts.find(x=>x.event===event);
    if(row) return row;
    await new Promise(resolve=>setTimeout(resolve,20));
  }
  throw new Error(`helper ${event} receipt timeout`);
}
function renderReport(v) {
  return '# Owned Python child Frida exception-handler no-crash preflight\n\n' +
    'This converter-only preflight creates a harmless Python sleeper in a suspended state, attaches directly with Frida, installs `Process.setExceptionHandler` with a callback that returns `false`, records the callback registration readiness, and resumes the sleeper. Cleanup asks the owning helper to terminate through its retained process handle, waits for process exit and both handles to close, then requires strict PID absence in independent Windows and Frida process views. It does not generate an exception or crash. A pass establishes only that the callback registration and owned-child cleanup path work; it does not test callback invocation, access-violation payloads, or converter exception behavior. Native execution is guarded off pending root review.\n\n```json\n' + JSON.stringify(v,null,2) + '\n```\n';
}
run().catch(e=>{ result.failures.push({stage:'runner',message:String(e?.message??e)}); console.error(JSON.stringify({outcome:'INCOMPLETE',failures:result.failures})); process.exitCode=1; });
