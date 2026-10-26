// PREPARED ONLY. No target starts without explicit execution and reviewed helper gate.
// Held-handle CREATE_SUSPENDED + direct Frida 17.23.2 attach, fixed B98 project.
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
import { spawn, execFileSync } from 'node:child_process';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const EXE = 'C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\DEF_GUI_MO_64.exe';
const CWD = 'C:\\PROBLEM\\p202a';
const SOURCE = path.join(ROOT, 'evidence', 'gui_mo', 'oracle_b', '98_B_DS3F_before');
const PROJECT = path.join(CWD, 'Gear_blank.moproj');
const FRIDA_URL = 'file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const PYTHON = 'C:\\Program Files\\Python312\\python.exe';
const HELPER = path.join(ROOT, 'admin', 'owned_suspended_process.py');
const REVIEWED_HELPER_SHA256 = '8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d';
// Parent will independently review this original-specific integration before run.
const ORIGINAL_RUN_REVIEWED = false;
const WINDOW_MS = 15000;
const MAX_RECORDS = 512;
const EXPECTED = {
  'DEF_GUI_MO_64.exe': 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
  'QtGui4_64.dll': 'c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887',
  'QtCore4_64.dll': 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160',
};
const HOOKS = [
  ['DEF_GUI_MO_64.exe', 0x1547e, 'prepared_visibility_request', '488b842450020000488b8020040000488b8c245002000048'],
  ['DEF_GUI_MO_64.exe', 0x3a990, 'slot_entry', '8854241048894c240856574881ecd8010000c64424600048'],
  ['DEF_GUI_MO_64.exe', 0x3aa86, 'busy_return', '884424620fb644246285c07407c644246301eb3e488b8c24'],
  ['DEF_GUI_MO_64.exe', 0x3aaa2, 'auto_save_call', 'e8d960fdff884424640fb644246485c0750dc78424c00100'],
  ['QtGui4_64.dll', 0x39ffa0, 'dock_event', '405355565741544881eca000000048c7442440feffffff4c'],
  ['QtGui4_64.dll', 0x3a0084, 'show_signal_call', 'e877d8ffffe946020000488bcee88ae2ffffe93902000032'],
];
const sha = bytes => crypto.createHash('sha256').update(bytes).digest('hex');
const fileSha = p => sha(fs.readFileSync(p));
function peImageSize(filename) {
  const bytes=fs.readFileSync(filename),pe=bytes.readUInt32LE(0x3c);
  requireThat(bytes.readUInt16LE(0)===0x5a4d&&bytes.readUInt32LE(pe)===0x4550&&bytes.readUInt16LE(pe+4)===0x8664&&bytes.readUInt16LE(pe+24)===0x20b,'Expected Windows x64 PE32+ image');
  return bytes.readUInt32LE(pe+24+56);
}
const canonical = p => fs.realpathSync.native(p).toLowerCase();
const delay = ms => new Promise(r => setTimeout(r, ms));
const errorRecord = e => ({ name: e?.name ?? 'Error', message: String(e?.message ?? e).slice(0, 1600) });
function requireThat(condition, message) { if (!condition) throw new Error(message); }
async function bounded(promise, ms, operation) {
  let timer;
  try { return await Promise.race([promise, new Promise((_, reject) => { timer=setTimeout(()=>reject(new Error(operation+' timed out; outcome uncertain')),ms); })]); }
  finally { clearTimeout(timer); }
}
// Frida 17.23.2 Session.isDetached is a METHOD, never a boolean property.
function sessionDetached(session) {
  if(session===null||session===undefined)return true;
  requireThat(typeof session.isDetached==='function','Session detached-state method unavailable');
  const detached=session.isDetached();
  requireThat(typeof detached==='boolean','Session detached-state method returned a nonboolean');
  return detached;
}
async function settleSessionCleanup(session, timeoutMs=2000) {
  if(session===null||session===undefined)return{settled:true,basis:'no_session'};
  if(sessionDetached(session))return{settled:true,basis:'already_detached'};
  await bounded(session.detach(),timeoutMs,'session detach');
  requireThat(sessionDetached(session),'Session detach completed without observed detached state');
  return{settled:true,basis:'detach_completed'};
}
function parseWindowsPresence(raw, pid) {
  requireThat(typeof raw==='string'&&raw.trim().length>0,'Windows presence inspection returned empty output');
  const value=JSON.parse(raw);
  requireThat(value&&typeof value==='object'&&!Array.isArray(value)&&value.schema==='owned-pid-presence-v1'&&value.querySucceeded===true&&value.pid===pid&&typeof value.present==='boolean'&&Array.isArray(value.processes),'Invalid Windows presence wrapper');
  requireThat(value.processes.length<=1&&value.processes.every(row=>row&&row.ProcessId===pid),'Windows presence PID/row mismatch');
  requireThat(value.present===(value.processes.length===1),'Windows presence boolean/row contradiction');
  return value;
}
function sameDevicePresence(pid, processes) {
  requireThat(Array.isArray(processes), 'Same-device process enumeration returned a non-array');
  const matches = processes.filter(p => p?.pid === pid);
  requireThat(matches.length <= 1, 'Same-device enumeration returned duplicate target PID rows');
  const matched = matches[0] ?? null;
  if (matched !== null) {
    requireThat(Number.isInteger(matched.pid) && matched.pid === pid, 'Malformed same-device target PID');
    requireThat(typeof matched.name === 'string' && matched.name.length > 0, 'Malformed same-device target name');
    requireThat(typeof matched.identifier === 'string' && matched.identifier.length > 0, 'Malformed same-device target identifier');
  }
  return { present: matched !== null, process: matched === null ? null : {
    pid: matched.pid, name: matched.name ?? null, identifier: matched.identifier ?? null,
  } };
}
function finalizeCaptureCompleteness(baseComplete, rawHelperReceiptsRetained, failureCount) {
  return baseComplete === true && rawHelperReceiptsRetained === true && failureCount === 0;
}

// These are deterministic directory manifests, not native filesystem snapshots.
// Root/ancestor/entry links and Windows reparse attributes are rejected separately.
function manifest(directory, enforceShort = false) {
  const base = fs.realpathSync.native(directory);
  const entries = [];
  let unstable = false;
  function walk(dir) {
    const st = fs.lstatSync(dir, { bigint: true });
    requireThat(!st.isSymbolicLink(), 'Link directory rejected: ' + dir);
    const names = fs.readdirSync(dir).sort();
    for (const name of names) {
      const p = path.join(dir, name);
      const rel = path.relative(base, p).replaceAll('\\', '/');
      const a = fs.lstatSync(p, { bigint: true });
      requireThat(!a.isSymbolicLink(), 'Link entry rejected: ' + rel);
      requireThat(canonical(p).startsWith(base.toLowerCase() + path.sep), 'Manifest escape: ' + rel);
      if (enforceShort) requireThat(p.length < 100, 'Path exceeds DEFORM bound: ' + p);
      if (a.isDirectory()) { entries.push({ kind: 'directory', path: rel, mtimeNs: String(a.mtimeNs) }); walk(p); }
      else {
        requireThat(a.isFile(), 'Nonregular file rejected: ' + rel);
        const digest = fileSha(p);
        const b = fs.lstatSync(p, { bigint: true });
        if (a.size !== b.size || a.mtimeNs !== b.mtimeNs) unstable = true;
        entries.push({ kind: 'file', path: rel, bytes: String(a.size), mtimeNs: String(a.mtimeNs), sha256: digest });
      }
    }
  }
  walk(base);
  entries.sort((a, b) => a.path < b.path ? -1 : a.path > b.path ? 1 : 0);
  const content = entries.map(({ mtimeNs, ...e }) => e);
  return { root: base, entries, unstable, contentDigest: sha(JSON.stringify(content)), metadataDigest: sha(JSON.stringify(entries)) };
}
function compare(before, after) {
  const a = new Map(before.entries.map(e => [e.path, e]));
  const b = new Map(after.entries.map(e => [e.path, e]));
  const out = { added: [], deleted: [], contentChanged: [], metadataOnly: [] };
  for (const [p, e] of a) {
    const f = b.get(p);
    if (!f) out.deleted.push(p);
    else if (e.kind !== f.kind || e.sha256 !== f.sha256 || e.bytes !== f.bytes) out.contentChanged.push(p);
    else if (e.mtimeNs !== f.mtimeNs) out.metadataOnly.push(p);
  }
  for (const p of b.keys()) if (!a.has(p)) out.added.push(p);
  return out;
}
function windowsInspection(pid = null) {
  // Only fixed/numeric values are interpolated; no caller-provided shell text.
  const query = pid === null
    ? `$paths=@('${SOURCE.replaceAll("'", "''")}','${CWD}'); $bad=@(); foreach($p in $paths){$q=Get-Item -LiteralPath $p -Force; while($q){if(($q.Attributes -band [IO.FileAttributes]::ReparsePoint)-ne 0){$bad+=$q.FullName};$q=$q.Parent}; Get-ChildItem -LiteralPath $p -Force -Recurse | Where-Object {($_.Attributes -band [IO.FileAttributes]::ReparsePoint)-ne 0} | ForEach-Object {$bad+=$_.FullName}}; [pscustomobject]@{reparsePaths=@($bad);version=(Get-Item -LiteralPath '${EXE}').VersionInfo.FileVersion;host=$env:COMPUTERNAME} | ConvertTo-Json -Compress`
    : `$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${Number(pid)}' -ErrorAction Stop | Select-Object ProcessId,CreationDate,ExecutablePath,CommandLine); [pscustomobject]@{schema='owned-pid-presence-v1';querySucceeded=$true;pid=${Number(pid)};present=[bool]($rows.Count -gt 0);processes=@($rows)} | ConvertTo-Json -Depth 4 -Compress`;
  const raw = execFileSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', query], { encoding: 'utf8', windowsHide: true, timeout: 5000 }).trim();
  return { raw, value: pid===null ? (raw ? JSON.parse(raw) : null) : parseWindowsPresence(raw,pid) };
}

// Pure observer: no NativeFunction, target API call, data/register assignment,
// replacement, GUI input, event pumping, or application-side file writing.
function observerSource() {
  const sizes=Object.fromEntries(Object.keys(EXPECTED).map(name=>[name,peImageSize(path.join(path.dirname(EXE),name))]));
  return `
  'use strict';
  const hooks=${JSON.stringify(HOOKS)}, expectedSizes=${JSON.stringify(sizes)}, expectedDirectory=${JSON.stringify(path.dirname(EXE))};
  const max=${MAX_RECORDS}, windowMs=${WINDOW_MS};
  let seq=0, observations=0, ended=false, timer=null, observer=null, slotSerial=0, armed=false, continuity=true;
  const start=Date.now(), listeners=[], installed=new Set(), modules=new Map(), stacks=new Map(), readySeq=new Map();
  const names=['DEF_GUI_MO_64.exe','QtGui4_64.dll','QtCore4_64.dll'];
  const canonicalName=name=>names.find(n=>n.toLowerCase()===name.toLowerCase());
  function coverage(){return{installed:[...installed],modules:[...modules.keys()],readySeq:Object.fromEntries(readySeq),continuity};}
  function preReady(){return observer!==null&&modules.has('DEF_GUI_MO_64.exe')&&hooks.filter(h=>h[0]==='DEF_GUI_MO_64.exe').every(h=>installed.has(h[2]));}
  function emit(kind,data={}) {
    const record={kind,seq:++seq,ms:Date.now()-start,tid:Process.getCurrentThreadId(),...data};
    if(JSON.stringify(record).length>1024) {finish('record_size_exceeded'); return;}
    send(record);
  }
  function finish(reason,detail=null) {
    if(ended)return; ended=true; if(timer!==null)clearTimeout(timer);
    const detachErrors=[];let observerStopped=false;
    for(const listener of listeners)try{listener.detach();}catch(e){detachErrors.push(String(e).slice(0,60));}
    if(observer)try{observer.detach();observerStopped=true;}catch(e){detachErrors.push(String(e).slice(0,60));}
    send({kind:'terminal',seq:++seq,ms:Date.now()-start,reason,detail,observations,detachErrors,observerStopped,...coverage()});
  }
  function record(kind,data) {
    if(ended||!armed)return;
    if(observations>=max){finish('budget_exhausted');return;}
    observations++;emit(kind,{...data,hookReadySeq:readySeq.get(kind)??null});
  }
  function top(tid){const s=stacks.get(tid);return s&&s.length?s[s.length-1]:null;}
  function safe(fn){try{return fn();}catch(e){finish('read_or_callback_error',String(e).slice(0,180));return null;}}
  function install(module) {
    const name=canonicalName(module.name);
    if(ended||!name)return;
    try {
      if(modules.has(name)){continuity=false;emit('duplicate_module',{name,path:module.path,base:String(module.base),size:module.size});throw new Error('duplicate_or_substituted_module:'+name);}
      const expected=(expectedDirectory+'\\\\'+name).toLowerCase();
      if(module.path.toLowerCase()!==expected)throw new Error('module_path_mismatch:'+name);
      if(module.size!==expectedSizes[name])throw new Error('module_size_mismatch:'+name);
      const own=hooks.filter(h=>h[0]===name);
      const prefixes=own.map(h=>({rva:'0x'+h[1].toString(16),bytes:Array.from(new Uint8Array(module.base.add(h[1]).readByteArray(24))).map(b=>b.toString(16).padStart(2,'0')).join('')}));
      // Compare EVERY prefix in the module before Interceptor changes any of them.
      for(let i=0;i<own.length;i++)if(prefixes[i].bytes!==own[i][3])throw new Error('prefix_mismatch:'+own[i][2]);
      if(name==='QtCore4_64.dll') {
        const q=Array.from(new Uint8Array(module.base.add(0x11730).readByteArray(5))).map(b=>b.toString(16).padStart(2,'0')).join('');
        if(q!=='0fb74110c3')throw new Error('QEvent_type_prefix_mismatch');
      }
      modules.set(name,{name,path:module.path,base:String(module.base),size:module.size,prefixes});
      emit('module_identity',modules.get(name));
      for(const h of own){
        const kind=h[2];
        const callbacks={onEnter(){safe(()=>{
          const c=this.context,tid=Process.getCurrentThreadId();
          const active=top(tid),common={slotId:active?active.id:null};
          if(kind==='prepared_visibility_request'){
            const rsp=c.rsp,requestedRaw=rsp.add(0x74).readU8(),main=rsp.add(0x250).readPointer(),dock=main.add(0x420).readPointer();
            const boolQualified=requestedRaw===0||requestedRaw===1;
            record(kind,{candidateRva:'0x1547e',meaning:'prepared_request_before_dock_call',rsp:String(rsp),main:String(main),dock:String(dock),requestedRaw,boolQualified,...common});
            if(!boolQualified){finish('unexpected_prepared_bool',String(requestedRaw));return;}
          }
          if(kind==='dock_event'){if(!modules.has('QtCore4_64.dll')){record('unqualified_dock_event',{dock:String(c.rcx),event:String(c.rdx),reason:'QtCore_type_bytes_not_yet_qualified'});finish('event_type_unqualified');return;}const type=c.rdx.add(0x10).readU16();record(kind,{dock:String(c.rcx),event:String(c.rdx),type,typeLabel:type===17?'Show':type===18?'Hide':'other',...common});}
          if(kind==='show_signal_call')record(kind,{dock:String(c.rcx),emitted:c.rdx.toUInt32()&255,...common});
          if(kind==='slot_entry'){
            const dock=c.rcx.add(0x420).readPointer(),s=stacks.get(tid)||[];
            this.slotId=++slotSerial;s.push({id:this.slotId,main:String(c.rcx),dock:String(dock)});stacks.set(tid,s);
            record(kind,{main:String(c.rcx),dock:String(dock),value:c.rdx.toUInt32()&255,slotId:this.slotId,parentSlotId:common.slotId});
          }
          if(kind==='busy_return')record(kind,{busy:c.rax.toUInt32()&255,...common});
          if(kind==='auto_save_call')record(kind,{main:String(c.rcx),...common});
        });}};
        if(kind==='slot_entry')callbacks.onLeave=function(){safe(()=>{const tid=Process.getCurrentThreadId(),s=stacks.get(tid)||[];if(!s.length||s.at(-1).id!==this.slotId)throw new Error('slot_stack_mismatch');s.pop();});};
        listeners.push(Interceptor.attach(module.base.add(h[1]),callbacks));installed.add(kind);
      }
      const next=seq+1;for(const h of own)readySeq.set(h[2],next);
      emit('module_ready',{name,path:module.path,base:String(module.base),size:module.size,hooks:own.map(h=>h[2]),armed});
    }catch(e){finish('hook_error',String(e).slice(0,180));}
  }
  function removed(module){const name=canonicalName(module.name);if(ended||!name)return;continuity=false;emit('module_removed',{name,path:module.path,base:String(module.base),size:module.size});for(const h of hooks.filter(h=>h[0]===name))installed.delete(h[2]);finish('required_module_removed');}
  rpc.exports={
    status(){return{pid:Process.id,platform:Process.platform,arch:Process.arch,ended,installed:[...installed],modules:[...modules.values()],observerInstalled:observer!==null,preResumeReady:preReady(),continuity};},
    arm(){if(ended||!preReady()||!continuity)throw new Error('required_EXE_and_observer_pre_resume_readiness_missing');armed=true;timer=setTimeout(()=>finish('startup_window_elapsed'),windowMs);emit('armed',{pid:Process.id});return true;},
    stop(){finish('external_stop');}
  };
  if(Process.platform!=='windows'||Process.arch!=='x64')finish('unsupported_platform');
  else if(typeof Process.attachModuleObserver!=='function')finish('module_observer_unavailable');
  else {observer=Process.attachModuleObserver({onAdded:install,onRemoved:removed});if(ended)observer.detach();else emit('observer_ready',{pid:Process.id});}
  `;
}

async function run() {
  requireThat(process.platform === 'win32', 'Windows execution host required');
  requireThat(process.argv.length === 3 && process.argv[2] === '--execute-reviewed-probe',
    'Prepared only. Execution requires --execute-reviewed-probe AND a reviewed held-handle helper hash.');
  requireThat(ORIGINAL_RUN_REVIEWED, 'BLOCKED before launch: original-specific integration awaits independent review');
  requireThat(fileSha(HELPER) === REVIEWED_HELPER_SHA256, 'Reviewed helper hash mismatch');
  // Helper protocol remains a review gate. Do not change the hash without reviewing
  // the CLI/JSONL contract below and proving independent watchdog cleanup on a child.
  const stamp = new Date().toISOString().replaceAll(/[:.]/g, '-');
  const out = path.join(ROOT, 'analysis', 'resume202-passive-oracle-run-' + stamp);
  fs.mkdirSync(out); // no overwrite, retained even on preflight failure
  const result = { startedUtc: new Date().toISOString(), host: os.hostname(), exe: EXE, cwd: CWD, argv: [EXE, '-m', 'open', '-f', PROJECT], guiAcquired: false, acceptedClose: false, helperProtocolValid:true,moduleIntegrityValid:true, failures: [], stages: [], loadedModuleChecks:[] };
  let device, session, script, helper, pid=null, ownerReady=null, helperExit=null, terminal=null, hostTimer, resumedResolve, attachSettlement;
  const loadedModules=new Map(),moduleReady=new Map();let messageSeq=0;
  const resumedReceipt=new Promise(resolve=>{resumedResolve=resolve;});
  let rawCount=0, stopped=false, cleanupPromise;
  const rawStream=fs.createWriteStream(path.join(out, 'frida-messages.jsonl'), {flags:'wx'});
  const helperStream=fs.createWriteStream(path.join(out, 'held-handle-helper.jsonl'), {flags:'wx'});
  const helperRawLines=[];
  const save=(name,value)=>fs.writeFileSync(path.join(out,name),JSON.stringify(value,null,2)+'\n',{flag:'wx'});
  const saveBestEffort=(name,value,stageName)=>{try{save(name,value);return true;}catch(e){result.failures.push({stage:stageName,...errorRecord(e)});return false;}};
  const stage=(name,data={})=>result.stages.push({name,utc:new Date().toISOString(),...data});
  let endResolve;
  const end=new Promise(r=>{endResolve=r;});
  const stop=(reason)=>{if(stopped)return;stopped=true;result.endpoint=reason;endResolve();};
  const helperInvalid=(reason,message)=>{result.helperProtocolValid=false;result.failures.push({stage:reason,message:String(message).slice(0,1600)});stop(reason);};
  const onSignal=()=>stop('external_stop');
  process.once('SIGINT',onSignal);process.once('SIGTERM',onSignal);
  async function cleanup(){
    if(cleanupPromise)return cleanupPromise;
    cleanupPromise=(async()=>{
      clearTimeout(hostTimer);
      // A retained handle in the independent helper owns termination. No numeric-PID fallback.
      if(helper&&!helper.stdin.destroyed&&helper.exitCode===null)try{helper.stdin.end(JSON.stringify({command:'terminate'})+'\n');}catch(e){result.failures.push({stage:'request_held_handle_termination',...errorRecord(e)});}
      if(helper)try{await bounded(helperExit,6000,'helper exit');}catch(e){result.failures.push({stage:'helper_exit',...errorRecord(e)});}
      result.attachCleanupSettled=!attachSettlement;
      if(attachSettlement)try{const settled=await bounded(attachSettlement,4000,'attach and late-detach settlement');if(settled.ok){session=settled.session;result.attachCleanupSettled=!result.attachTimedOut||settled.lateDetached===true;}else{result.attachCleanupSettled=true;result.attachOperationError=settled.error;}}catch(e){result.attachCleanupSettled=false;result.failures.push({stage:'late_attach_unsettled',...errorRecord(e)});}
      result.heldHandleExitVerified=result.heldHandleCleanup?.exit_verified===true&&result.heldHandleCleanup?.handles_closed===true&&result.heldHandleCleanup?.wait_result===0&&result.helperExit?.code===0;
      if(pid!==null&&!result.heldHandleExitVerified)result.failures.push({stage:'held_handle_cleanup',message:'Required held-handle exit/close receipt missing or failed'});
      // Settle active/late sessions before taking fresh absence observations.
      try{if(script&&session&&!sessionDetached(session)){await bounded(script.unload(),2000,'script unload');stage('script_unload_observed');}else if(script)result.scriptUnload='not_applicable_session_already_detached';}catch(e){result.failures.push({stage:'unload',...errorRecord(e)});}
      result.sessionCleanupSettled=false;
      try{const receipt=await settleSessionCleanup(session);result.sessionCleanupSettled=receipt.settled===true;result.sessionCleanupBasis=receipt.basis;stage('session_cleanup_observed',receipt);}catch(e){result.sessionCleanupSettled=false;result.failures.push({stage:'detach',...errorRecord(e)});}
      await observeOwnedAfterState();
      result.ownedPidObservedAbsent=result.fridaAbsent===true&&result.windowsAbsent===true;
      result.processAbsenceVerified=result.ownedPidObservedAbsent&&result.attachCleanupSettled===true&&result.sessionCleanupSettled===true&&result.heldHandleExitVerified===true;
    })().catch(async e=>{
      result.failures.push({stage:'cleanup_unexpected',...errorRecord(e)});
      result.processAbsenceVerified=false;
      await observeOwnedAfterState();
      result.ownedPidObservedAbsent=result.fridaAbsent===true&&result.windowsAbsent===true;
      result.processAbsenceVerified=result.ownedPidObservedAbsent&&result.attachCleanupSettled===true&&result.sessionCleanupSettled===true&&result.heldHandleExitVerified===true;
    });return cleanupPromise;
  }
  async function observeOwnedAfterState(){
    if(pid===null)return;
    try{
      const listing=await bounded(device.enumerateProcesses(),3000,'same-device enumeration');
      const presence=sameDevicePresence(pid,listing);
      result.fridaAbsent=!presence.present;
      saveBestEffort('same-device-absence.json',{schema:'frida-device-presence-v1',querySucceeded:true,utc:new Date().toISOString(),deviceId:device.id,pid,...presence},'same_device_absence_receipt_write');
    }catch(e){result.fridaAbsent=false;saveBestEffort('same-device-absence-error.json',{schema:'frida-device-presence-v1',querySucceeded:false,utc:new Date().toISOString(),deviceId:device?.id??null,pid,...errorRecord(e)},'same_device_absence_error_write');result.failures.push({stage:'same_device_absence',...errorRecord(e)});}
    try{const win=windowsInspection(pid);result.windowsAbsent=win.value.present===false;saveBestEffort('windows-absence.json',{utc:new Date().toISOString(),...win},'windows_absence_receipt_write');}
    catch(e){result.windowsAbsent=false;saveBestEffort('windows-absence-error.json',{utc:new Date().toISOString(),...errorRecord(e)},'windows_absence_error_write');result.failures.push({stage:'windows_absence',...errorRecord(e)});}
  }
  try {
    const inspection=windowsInspection();save('windows-preflight.json',inspection);
    requireThat(inspection.value.reparsePaths.length===0,'Reparse path rejected');
    result.originalVersionResource=inspection.value.version;
    result.expectedHistoricalVersion='11.0.2';
    result.identities={};
    for(const [name,digest]of Object.entries(EXPECTED)){
      const p=path.join(path.dirname(EXE),name),actual=fileSha(p);
      requireThat(canonical(p)===p.toLowerCase(),'Image canonical path mismatch');
      requireThat(actual===digest,'Image SHA mismatch: '+name);result.identities[name]={path:p,sha256:actual,imageSize:peImageSize(p)};
    }
    const sourceBefore=manifest(SOURCE),copyBefore=manifest(CWD,true);
    requireThat(!sourceBefore.unstable&&!copyBefore.unstable,'Unstable before manifests');
    requireThat(sourceBefore.contentDigest===copyBefore.contentDigest,'Disposable differs from source');
    requireThat(sourceBefore.entries.filter(e=>e.kind==='file').length===27,'Unexpected source fixture count');
    requireThat(fileSha(PROJECT)==='e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd','Project SHA mismatch');
    save('source-before.json',sourceBefore);save('disposable-before.json',copyBefore);
    const packagePath=fileURLToPath(new URL('../../package.json',FRIDA_URL));
    requireThat(JSON.parse(fs.readFileSync(packagePath,'utf8')).version==='17.23.2','Frida version mismatch');
    const fridaRoot=path.dirname(packagePath);
    const runtimeFiles=fs.readdirSync(fridaRoot,{recursive:true}).filter(p=>/\.(js|json|node|dll)$/i.test(p)).sort();
    requireThat(runtimeFiles.length>0&&runtimeFiles.length<4096,'Unexpected Frida runtime tree');
    save('runtime-identities.json',{node:{path:process.execPath,version:process.version,sha256:fileSha(process.execPath)},python:{path:PYTHON,sha256:fileSha(PYTHON)},frida:{version:'17.23.2',entry:FRIDA_URL,packagePath,files:runtimeFiles.map(p=>({path:p,sha256:fileSha(path.join(fridaRoot,p))}))}});
    const probe=observerSource();fs.writeFileSync(path.join(out,'observer.js'),probe,{flag:'wx'});
    result.scriptSha256=sha(probe);result.runnerSha256=fileSha(fileURLToPath(import.meta.url));result.helperSha256=fileSha(HELPER);
    const frida=await import(FRIDA_URL);device=await frida.getLocalDevice();result.deviceId=device.id;
    // Helper creates original suspended and retains original process/thread handles.
    // No Frida spawn, numeric-PID kill, guardian adoption, or Frida resume is used.
    const helperStarted=Date.now();
    result.helperArgv=['-I','-S',HELPER,'--exe',EXE,'--cwd',CWD,'--sha256',EXPECTED['DEF_GUI_MO_64.exe'],'--max-seconds','20','--',...result.argv.slice(1)];
    helper=spawn(PYTHON,result.helperArgv,{cwd:ROOT,windowsHide:true,stdio:['pipe','pipe','pipe']});
    stage('held_handle_launcher_started',{launcherPid:helper.pid});
    hostTimer=setTimeout(()=>stop('host_cleanup_deadline'),20000);
    helper.stdin.on('error',e=>helperInvalid('helper_stdin_error',e.message));
    let readyResolve,readyReject;const ready=new Promise((resolve,reject)=>{readyResolve=resolve;readyReject=reject;});
    let partial='',helperLines=0,discardHelper=false;const helperEvents=new Map();
    helper.stdout.on('data',bytes=>{
      if(discardHelper)return;
      if(Buffer.byteLength(partial)+bytes.length>65536){partial='';discardHelper=true;helperInvalid('helper_output_overflow','Discarding excess helper stdout after 64 KiB buffer bound');readyReject(new Error('Helper output overflow'));return;}
      partial+=bytes.toString('utf8');let i;
      while((i=partial.indexOf('\n'))>=0){
        const line=partial.slice(0,i);partial=partial.slice(i+1);
        if(++helperLines>32||Buffer.byteLength(line)>8192){partial='';discardHelper=true;helperInvalid('helper_output_budget','Discarding excess helper stdout after line/count bound');readyReject(new Error('Helper output budget'));return;}
        helperRawLines.push(line);
        helperStream.write(line+'\n');
        try{
          const e=JSON.parse(line);if(!e||Array.isArray(e)||!['created','resumed','status','endpoint','cleanup','error','cleanup_error'].includes(e.event))throw new Error('Unexpected helper event');
          const count=(helperEvents.get(e.event)??0)+1;helperEvents.set(e.event,count);
          if(['created','resumed','cleanup'].includes(e.event)&&count!==1)throw new Error('Duplicate helper '+e.event);
          if(e.event==='created'){ownerReady=e;readyResolve(e);}
          if(e.event==='resumed')resumedResolve(e);
          if(e.event==='cleanup')result.heldHandleCleanup=e;
          if(e.event==='error'||e.event==='cleanup_error'){helperInvalid('helper_error',line);readyReject(new Error('Helper error'));}
        }catch(e){helperInvalid('helper_protocol_error',e.message);readyReject(e);}
      }
    });
    let stderrBytes=0;
    helper.stderr.on('data',bytes=>{if(bytes.length){if(stderrBytes<4096)fs.appendFileSync(path.join(out,'helper-stderr.txt'),bytes.subarray(0,4096-stderrBytes));if(stderrBytes===0)helperInvalid('helper_stderr','Helper emitted stderr; excess bytes discarded after 4 KiB');stderrBytes+=bytes.length;result.helperStderrBytes=stderrBytes;}});
    helperExit=new Promise(resolve=>{helper.once('error',e=>{readyReject(e);helperInvalid('helper_process_error',e.message);});helper.once('exit',(code,signal)=>{result.helperExit={code,signal};if(!ownerReady)readyReject(new Error('Helper exited before readiness'));});helper.once('close',()=>{result.helperPipesDrained=true;result.helperLines=helperLines;if(partial.trim()){helperRawLines.push(partial);helperInvalid('helper_framing','Unterminated JSONL tail');}resolve();});});
    await bounded(ready,3000,'held-handle creation readiness');
    requireThat(Number.isInteger(ownerReady.identity?.pid)&&ownerReady.identity.pid>0,'Invalid helper-created PID');
    pid=ownerReady.identity.pid;result.pid=pid;result.ownedCreation=ownerReady;
    requireThat(Number.isInteger(ownerReady.identity.primary_tid)&&ownerReady.identity.primary_tid>0&&typeof ownerReady.identity.creation_filetime==='number'&&Number.isFinite(ownerReady.identity.creation_filetime)&&ownerReady.identity.creation_filetime>0,'Invalid primary TID or creation FILETIME; exact integer retained in raw JSONL');
    requireThat(ownerReady.state==='suspended'&&ownerReady.identity?.launcher_pid===helper.pid,'Held-handle launcher/suspension mismatch');
    requireThat(ownerReady.identity?.canonical_image?.toLowerCase()===EXE.toLowerCase()&&ownerReady.identity?.sha256===EXPECTED['DEF_GUI_MO_64.exe']&&ownerReady.identity?.creation_filetime&&ownerReady.identity?.cwd?.toLowerCase()===CWD.toLowerCase(),'Held-handle identity/cwd mismatch');
    requireThat(ownerReady.identity?.host?.toLowerCase()===os.hostname().toLowerCase(),'Launcher host mismatch');
    requireThat(JSON.stringify(ownerReady.identity.argv.map((v,i)=>i===0?v.toLowerCase():v))===JSON.stringify(result.argv.map((v,i)=>i===0?v.toLowerCase():v)),'Launcher argv mismatch');
    stage('held_handle_ready');
    attachSettlement=device.attach(pid).then(async attached=>{
      if(result.attachTimedOut){result.lateAttachResolved=true;const receipt=await settleSessionCleanup(attached);result.lateAttachDetached=receipt.settled===true;return{ok:true,session:attached,lateDetached:receipt.settled===true};}
      return{ok:true,session:attached};
    },error=>({ok:false,error:errorRecord(error)}));
    try{const settled=await bounded(attachSettlement,3000,'owned attach');requireThat(settled.ok,'Owned attach rejected: '+JSON.stringify(settled.error));session=settled.session;}catch(e){result.attachTimedOut=true;result.failures.push({stage:'attach_timeout_or_error',message:'Attach cancellation is unavailable; retain outcome uncertainty and do not retry.'});throw e;}
    session.detached.connect(reason=>{result.sessionDetachedReason=reason;stop('target_exited_or_detached');});
    script=await bounded(session.createScript(probe),2000,'script create');
    script.message.connect((message,data)=>{
      if(++rawCount>550||data!==null&&data!==undefined||JSON.stringify(message).length>4096){result.deliveryOverflow=true;stop('provider_or_message_budget');return;}
      rawStream.write(JSON.stringify(message)+'\n');
      if(message.type==='error'){result.failures.push({stage:'script_error',message:message.description});stop('hook_error');}
      if(message.type==='send')try{
        const p=message.payload;requireThat(p&&Number.isInteger(p.seq)&&p.seq===messageSeq+1,'Observer message sequence gap/reorder');messageSeq=p.seq;
        if(p.kind==='module_identity'){
          requireThat(Object.hasOwn(EXPECTED,p.name)&&!loadedModules.has(p.name),'Unexpected/duplicate module identity');
          const expectedPath=path.join(path.dirname(EXE),p.name);
          requireThat(p.path.toLowerCase()===expectedPath.toLowerCase()&&canonical(p.path)===expectedPath.toLowerCase()&&p.size===result.identities[p.name].imageSize,'Loaded module path/size mismatch');
          const prefixes=HOOKS.filter(h=>h[0]===p.name).map(h=>({rva:'0x'+h[1].toString(16),bytes:h[3]}));
          requireThat(JSON.stringify(p.prefixes)===JSON.stringify(prefixes),'Loaded module prefix receipt mismatch');
          const actual=fileSha(p.path);requireThat(actual===EXPECTED[p.name],'Observed loaded-module file SHA mismatch');
          loadedModules.set(p.name,p);result.loadedModuleChecks.push({utc:new Date().toISOString(),messageSeq:p.seq,name:p.name,path:p.path,base:p.base,size:p.size,sha256:actual});
        }
        if(p.kind==='module_ready'){
          const m=loadedModules.get(p.name);requireThat(m&&!moduleReady.has(p.name)&&m.path===p.path&&m.base===p.base&&m.size===p.size,'Module-ready identity mismatch');
          requireThat(JSON.stringify(p.hooks)===JSON.stringify(HOOKS.filter(h=>h[0]===p.name).map(h=>h[2])),'Module-ready hook set mismatch');
          moduleReady.set(p.name,p);
        }
        if(HOOKS.some(h=>h[2]===p.kind)){
          const h=HOOKS.find(h=>h[2]===p.kind),ready=moduleReady.get(h[0]);
          requireThat(ready&&p.hookReadySeq===ready.seq&&ready.seq<p.seq,'Observation precedes qualified hook-ready receipt');
        }
        if(p.kind==='module_removed'||p.kind==='duplicate_module'){result.moduleIntegrityValid=false;stop('module_continuity_lost');}
        if(p.kind==='terminal'){terminal=p;stop(p.reason);}
      }catch(e){result.moduleIntegrityValid=false;result.failures.push({stage:'observer_receipt',...errorRecord(e)});stop('observer_identity_or_order_error');}
    });
    await bounded(script.load(),2000,'script load');const status=await bounded(script.exports.status(),1000,'readiness');save('pre-resume-readiness.json',status);
    requireThat(!status.ended&&status.pid===pid&&status.platform==='windows'&&status.arch==='x64'&&status.observerInstalled&&status.preResumeReady&&status.continuity&&HOOKS.filter(h=>h[0]==='DEF_GUI_MO_64.exe').every(h=>status.installed.includes(h[2]))&&moduleReady.has('DEF_GUI_MO_64.exe'),'Four EXE hooks and active observer must be ready BEFORE resume; Qt may load synchronously later');
    for(const m of status.modules)requireThat(fileSha(m.path)===EXPECTED[m.name],'Loaded module file hash mismatch');
    requireThat(!stopped,'Stop requested before resume');
    requireThat(Date.now()-helperStarted<4000,'Setup consumed the full-window budget before resume');
    await bounded(script.exports.arm(),1000,'arm');helper.stdin.write(JSON.stringify({command:'resume'})+'\n');
    const resumed=await bounded(resumedReceipt,1000,'held-thread resume receipt');
    requireThat(resumed.previous_suspend_count===1&&resumed.state==='running','Resume receipt mismatch');
    stage('resumed_once',{receipt:resumed});
    // Host deadline applies even if the target timer stalls. No late observer polling.
    await Promise.race([end,delay(WINDOW_MS).then(()=>stop('host_startup_window_elapsed'))]);
    // Request termination before hashing live files, avoiding unobserved execution
    // during snapshot work. Quiescent manifests follow independent absence checks.
    await cleanup();
    if(result.processAbsenceVerified){
      const copyAfter=manifest(CWD,true),sourceAfter=manifest(SOURCE);
      save('disposable-after.json',copyAfter);save('source-after.json',sourceAfter);
      result.directoryEffects=compare(copyBefore,copyAfter);result.sourceUnchanged=sourceBefore.metadataDigest===sourceAfter.metadataDigest;
      result.quiescentAfterStable=!copyAfter.unstable&&!sourceAfter.unstable;
    }
  }catch(e){result.failures.push({stage:result.stages.at(-1)?.name??'preflight',...errorRecord(e)});stop('error');}
  finally{
    await cleanup();process.removeListener('SIGINT',onSignal);process.removeListener('SIGTERM',onSignal);
    result.terminal=terminal;result.rawMessages=rawCount;result.finishedUtc=new Date().toISOString();
    const expectedHooks=HOOKS.map(h=>h[2]).sort(),expectedModules=Object.keys(EXPECTED).sort();
    result.finalCoverage={moduleReady:[...moduleReady.values()],missingModules:expectedModules.filter(n=>!moduleReady.has(n)),missingHooks:expectedHooks.filter(n=>!terminal?.installed?.includes(n)),observerContinuity:terminal?.continuity??false};
    result.coverageComplete=terminal?.continuity===true&&terminal?.observerStopped===true&&Array.isArray(terminal?.detachErrors)&&terminal.detachErrors.length===0&&JSON.stringify([...(terminal?.installed??[])].sort())===JSON.stringify(expectedHooks)&&JSON.stringify([...(terminal?.modules??[])].sort())===JSON.stringify(expectedModules)&&expectedModules.every(n=>moduleReady.has(n)&&loadedModules.has(n))&&HOOKS.every(h=>Number.isInteger(terminal?.readySeq?.[h[2]])&&terminal.readySeq[h[2]]===moduleReady.get(h[0])?.seq&&terminal.readySeq[h[2]]<terminal.seq);
    result.captureComplete=terminal?.reason==='startup_window_elapsed'&&result.coverageComplete&&result.moduleIntegrityValid===true&&result.helperProtocolValid===true&&result.failures.length===0&&result.deliveryOverflow!==true&&result.processAbsenceVerified===true&&result.heldHandleExitVerified===true&&result.quiescentAfterStable===true&&result.sourceUnchanged===true&&result.helperPipesDrained===true&&result.attachTimedOut!==true;
    result.qualifications=['Interceptors alter code and timing.','No GUI input or target API calls.','No dialog-free startup claim: GUI is unobserved.','Six-point trace cannot prove writer success.','Unchanged files do not prove no Save.','Directory manifests exclude external/registry/settings writes.','Owned termination is not accepted Close.'];
    await Promise.all([new Promise(r=>rawStream.end(r)),new Promise(r=>helperStream.end(r))]);
    result.helperRawReceiptsRetained=saveBestEffort('held-handle-helper-raw-receipts.json',helperRawLines,'helper_raw_receipts_write');
    result.captureComplete=finalizeCaptureCompleteness(result.captureComplete,result.helperRawReceiptsRetained,result.failures.length);
    save('loaded-module-file-identities.json',result.loadedModuleChecks);
    save('summary.json',result);
    const finalManifest=fs.readdirSync(out).sort().map(name=>({name,sha256:fileSha(path.join(out,name))}));
    save('capture-manifest.json',finalManifest);
    fs.writeFileSync(path.join(out,'capture-manifest.sha256'),fileSha(path.join(out,'capture-manifest.json'))+'\n',{flag:'wx'});
    console.log(JSON.stringify({directory:out,endpoint:result.endpoint,processAbsenceVerified:result.processAbsenceVerified??false,failures:result.failures.length}));
    process.exitCode=result.captureComplete?0:1;
  }
}

run().catch(error=>{console.error(error.message);process.exitCode=2;});
