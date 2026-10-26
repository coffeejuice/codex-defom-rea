// Harmless runtime preflight for Frida Process.setExceptionHandler.
// Targets only a newly created Python child; never the converter or GUI.
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
import { spawn, execFileSync } from 'node:child_process';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const HELPER = path.join(ROOT, 'admin', 'owned_suspended_process.py');
const REPORT = path.join(ROOT, 'analysis', 'resume205-exception-handler-av-child-preflight.md');
const CWD = 'C:\\PROBLEM';
const PYTHON = 'C:\\Program Files\\Python312\\python.exe';
const FRIDA_URL = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const CHILD_CODE = 'import ctypes; ctypes.CFUNCTYPE(None)(0)()';
const MAX_SECONDS = 20;
const sha = x => crypto.createHash('sha256').update(x).digest('hex');
const fileSha = p => sha(fs.readFileSync(p));
const requireThat = (ok, message) => { if (!ok) throw new Error(message); };
const result = { schema: 'owned-python-exception-handler-preflight-v1', startedUtc: new Date().toISOString(),
  host: os.hostname(), target: { python: PYTHON, cwd: CWD, childCode: CHILD_CODE },
  guiInput: false, guiControlAcquired: false, interceptorHooks: 0, maxSeconds: MAX_SECONDS,
  stages: [], failures: [], helperRawReceipts: [], scriptMessages: [] };
let device, session, script, helper, helperExit, helperBuffer = '', stdoutClosed = false;
let childIdentity = null, readyResolve, readyReject, eventResolve, stderr = '';
const ready = new Promise((resolve, reject) => { readyResolve = resolve; readyReject = reject; });
const capture = new Promise(resolve => { eventResolve = resolve; });
const stage = name => result.stages.push({ name, utc: new Date().toISOString() });
function bounded(p, ms, label) {
  let timer;
  return Promise.race([p, new Promise((_, reject) => { timer = setTimeout(() => reject(new Error(`${label} timeout (${ms} ms)`)), ms); })])
    .finally(() => clearTimeout(timer));
}
function helperCommand(command) { if (helper && !helper.stdin.destroyed) helper.stdin.write(JSON.stringify({ command }) + '\n'); }
function presence(pid, creationFiletime) {
  const q = `$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' -ErrorAction Stop | ForEach-Object { [pscustomobject]@{ProcessId=$_.ProcessId;CreationFiletime=$_.CreationDate.ToUniversalTime().ToFileTimeUtc();ExecutablePath=$_.ExecutablePath;CommandLine=$_.CommandLine} }); [pscustomobject]@{pid=${pid};present=[bool]($rows.Count -gt 0);processes=@($rows)} | ConvertTo-Json -Depth 4 -Compress`;
  const s = execFileSync('powershell.exe', ['-NoProfile','-NonInteractive','-Command',q], { encoding:'utf8', windowsHide:true, timeout:8000 }).trim();
  const v = JSON.parse(s);
  requireThat(v.pid === pid && Array.isArray(v.processes) && v.present === (v.processes.length === 1) && v.processes.length <= 1, 'Invalid independent Windows process presence receipt');
  v.ownedCreationFiletime = String(creationFiletime);
  v.pidReuseObserved = v.processes.some(p => String(p.CreationFiletime) !== String(creationFiletime));
  v.ownedInstancePresent = v.processes.some(p => String(p.CreationFiletime) === String(creationFiletime));
  return v;
}
function injected() {
  return `
  let first = true;
  const str = v => { try { return v == null ? null : String(v); } catch (_) { return '<unprintable>'; } };
  function describe(a) {
    const m = a ? Process.findModuleByAddress(a) : null;
    let s = null; try { s = DebugSymbol.fromAddress(a).toString(); } catch (_) {}
    return { address: str(a), symbol: s, module: m ? { name:m.name, path:m.path, base:str(m.base), size:m.size, offset:str(a.sub(m.base)) } : null };
  }
  function registers(c) {
    const out = {}, names = new Set(['rip','rsp','rbp','rax','rbx','rcx','rdx','rsi','rdi','r8','r9','r10','r11','r12','r13','r14','r15','eflags','rflags', ...Object.keys(c)]);
    for (const n of names) try { const v=c[n]; if (v && typeof v.toString==='function') out[n]=v.toString(); } catch (_) {}
    return out;
  }
  Process.setExceptionHandler(d => {
    try {
      if (first && d && d.type === 'access-violation') {
        first=false;
        const c=d.context || {}; let frames=[];
        try { frames=Thread.backtrace(c, Backtracer.ACCURATE).map(describe); } catch (e) { frames=[{error:String(e)}]; }
        const rip=c.rip || c.pc || null;
        send({kind:'first-access-violation',pid:Process.id,exceptionType:d.type,exceptionAddress:describe(d.address),
          rip:describe(rip),faultingInstruction:describe(rip),registers:registers(c),accurateBacktrace:frames,
          memoryOperation:d.memory ? {operation:str(d.memory.operation),address:str(d.memory.address)} : null,
          disposition:'return_false_continue_natural_exception'});
      }
    } catch (e) { send({kind:'exception-handler-error',pid:Process.id,error:String(e).slice(0,1200)}); }
    return false;
  });
  send({kind:'handler-ready',pid:Process.id,arch:Process.arch,platform:Process.platform,registrationCallCompleted:true});
  `;
}

async function run() {
  try {
    requireThat(process.platform === 'win32', 'Windows only');
    requireThat(fs.statSync(PYTHON).isFile() && fs.statSync(HELPER).isFile() && fs.statSync(CWD).isDirectory(), 'Fixed runtime/helper/cwd missing');
    result.pythonSha256 = fileSha(PYTHON); result.helperSha256 = fileSha(HELPER);
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
    helperExit = new Promise((resolve,reject) => { helper.once('error',reject); helper.once('close',(code,signal)=>{ result.helperExit={code,signal}; resolve(); }); });
    helper.stdout.on('data', chunk => {
      helperBuffer += chunk;
      if (Buffer.byteLength(helperBuffer)>65536) { const e=new Error('Helper stdout exceeded bound'); readyReject(e); return; }
      let i; while ((i=helperBuffer.indexOf('\n'))>=0) {
        const line=helperBuffer.slice(0,i); helperBuffer=helperBuffer.slice(i+1);
        try {
          const row=JSON.parse(line); result.helperRawReceipts.push(row);
          if (row.event==='created') { childIdentity=row.identity; result.createdReceipt=row; readyResolve(row); }
          else if (row.event==='cleanup') result.cleanupReceipt=row;
          else if (row.event==='error'||row.event==='cleanup_error') { const e=new Error(`Helper ${row.event}: ${JSON.stringify(row)}`); readyReject(e); eventResolve({kind:'helper-error',error:e.message}); }
        } catch(e) { result.failures.push({stage:'helper-json',message:e.message}); readyReject(e); }
      }
    });
    helper.stdout.on('end',()=>{stdoutClosed=true;});
    stage('helper-started');
    const created=await bounded(ready,5000,'created receipt');
    requireThat(created.state==='suspended' && created.gui_control_acquired===false && childIdentity?.launcher_pid===helper.pid &&
      childIdentity.sha256===result.pythonSha256 && childIdentity.canonical_image.toLowerCase()===PYTHON.toLowerCase() &&
      childIdentity.cwd.toLowerCase()===CWD.toLowerCase() && childIdentity.argv.at(-1)===CHILD_CODE, 'Owned suspended child identity mismatch');
    result.ownedIdentity=childIdentity; stage('owned-python-child-created-suspended');
    session=await bounded(device.attach(childIdentity.pid),12000,'direct Frida attach'); stage('frida-attached-before-resume');
    script=await bounded(session.createScript(injected()),3000,'create exception handler script');
    let handlerReadyResolve;
    const handlerReady=new Promise(resolve=>handlerReadyResolve=resolve);
    script.message.connect(message=>{
      result.scriptMessages.push(message);
      if (message.type==='send' && message.payload?.kind==='handler-ready' && message.payload.pid===childIdentity.pid) { result.handlerReady=message.payload; handlerReadyResolve(message.payload); }
      if (message.type==='send' && ['first-access-violation','exception-handler-error'].includes(message.payload?.kind)) { if (!result.exceptionEvent) result.exceptionEvent=message.payload; eventResolve(message.payload); }
      if (message.type==='error') { result.scriptError={description:message.description??null,stack:message.stack??null}; readyReject(new Error('Frida injected script error')); eventResolve(result.scriptError); }
    });
    await bounded(script.load(),3000,'script load');
    result.handlerReady=await bounded(handlerReady,3000,'exception handler registration');
    requireThat(result.handlerReady.arch==='x64' && result.handlerReady.platform==='windows' && result.handlerReady.registrationCallCompleted===true,'Handler readiness mismatch');
    stage('handler-installed-before-resume');
    helperCommand('resume');
    const resumeLine=await new Promise((resolve,reject)=>{
      const until=Date.now()+4000;
      const poll=()=>{ const r=result.helperRawReceipts.find(x=>x.event==='resumed'); if(r) resolve(r); else if(Date.now()>until) reject(new Error('helper resume receipt timeout')); else setTimeout(poll,20); }; poll();
    });
    requireThat(resumeLine.previous_suspend_count===1 && ['running','exited'].includes(resumeLine.state),'Resume receipt mismatch');
    result.resumedReceipt=resumeLine; stage('resumed-once');
    result.observation=await bounded(capture,5000,'exception observation');
    stage('exception-observed-or-error');
  } catch(e) { result.failures.push({stage:result.stages.at(-1)?.name??'preflight',name:e?.name??'Error',message:String(e?.message??e).slice(0,2000)}); }
  finally {
    // The callback message precedes OS exception dispatch. Allow natural exit
    // and exact-handle cleanup before requesting any fallback termination.
    if (helperExit) {
      try { await bounded(helperExit,5000,'natural child/helper exit'); stage('helper-closed-after-natural-exit-window'); }
      catch(e) {
        result.naturalExitWindow={elapsedMs:5000,message:String(e?.message??e)};
        if (helper && !helper.stdin.destroyed) { helperCommand('terminate'); helper.stdin.end(); }
        try { await bounded(helperExit,22000,'fallback helper exit and exact-handle cleanup'); }
        catch(last) { result.failures.push({stage:'helper-exit',message:String(last?.message??last)}); }
      }
    }
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
      try { result.windowsAfter=presence(childIdentity.pid,childIdentity.creation_filetime); result.windowsAbsent=result.windowsAfter.present===false && result.windowsAfter.pidReuseObserved===false; }
      catch(e) { result.windowsAbsent=false; result.failures.push({stage:'windows-absence',message:String(e?.message??e)}); }
      try { const rows=await bounded(device.enumerateProcesses(),4000,'Frida process absence'); const matches=rows.filter(p=>p.pid===childIdentity.pid); result.fridaAfter={deviceId:device.id,matches}; result.fridaAbsent=matches.length===0; }
      catch(e) { result.fridaAbsent=false; result.failures.push({stage:'frida-absence',message:String(e?.message??e)}); }
    } else { result.windowsAbsent=false; result.fridaAbsent=false; }
    result.naturalExitVerified=result.cleanupReceipt?.exit_verified===true && result.cleanupReceipt?.handles_closed===true && result.cleanupReceipt?.wait_result===0 && result.cleanupReceipt?.exit_code===0xc0000005 && result.cleanupReceipt?.termination_requested===false && (result.cleanupReceipt?.termination_attempts?.length??-1)===0 && result.helperExit?.code===0;
    result.cleanupByTermination=result.cleanupReceipt?.termination_requested===true || (result.cleanupReceipt?.termination_attempts?.length??0)>0;
    result.cleanupVerified=result.naturalExitVerified===true;
    result.processAbsent=result.windowsAbsent===true && result.fridaAbsent===true;
    const av=result.exceptionEvent;
    result.exceptionRecordQualified=av?.kind==='first-access-violation' && av.pid===childIdentity?.pid && av.exceptionType==='access-violation' &&
      typeof av.exceptionAddress?.address==='string' && typeof av.rip?.address==='string' && av.faultingInstruction?.module?.name &&
      av.registers && Object.keys(av.registers).length>=10 && Array.isArray(av.accurateBacktrace) && av.accurateBacktrace.length>0 &&
      av.accurateBacktrace.every(f=>typeof f.address==='string') && av.disposition==='return_false_continue_natural_exception';
    result.captureComplete=result.exceptionRecordQualified===true && result.cleanupVerified && result.processAbsent && result.sessionDetached===true && result.failures.length===0;
    result.finishedUtc=new Date().toISOString();
    result.outcome=result.captureComplete?'PASS':result.cleanupByTermination?'INCOMPLETE_CLEANED_BY_TERMINATION':'INCOMPLETE';
    fs.writeFileSync(REPORT,'# Owned Python child Frida exception-handler preflight\n\n'+
      'Converter-only runtime preflight using a newly created, owned Python child. The child is created suspended, attached directly with Frida 17.23.2, and the exception handler is installed before one resume. Its fixed `ctypes.CFUNCTYPE(None)(0)()` call triggers an access violation. The callback records the first access violation and returns `false`, leaving natural exception handling in control. Passing cleanup requires natural child exit, exact helper handle closure, and absence from both Windows and Frida process views. No original converter, DEFORM application, GUI input, or existing PID is targeted.\n\n'+
      'The raw helper JSONL receipts and the bounded result are retained below. A passing result validates this isolated Frida callback mechanism only; it does not establish converter exception behavior.\n\n```json\n'+JSON.stringify(result,null,2)+'\n```\n',{flag:'wx'});
  }
  console.log(JSON.stringify({outcome:result.outcome,report:REPORT,captureComplete:result.captureComplete===true}));
  process.exitCode=result.captureComplete?0:1;
}
run().catch(e=>{ result.failures.push({stage:'runner',message:String(e?.message??e)}); console.error(JSON.stringify({outcome:'INCOMPLETE',failures:result.failures})); process.exitCode=1; });
