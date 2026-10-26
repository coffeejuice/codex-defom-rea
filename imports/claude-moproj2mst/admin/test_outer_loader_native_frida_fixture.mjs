// Fixed disposable native fixture only. No target/PID arguments; never DEFORM.
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import crypto from 'node:crypto';
import assert from 'node:assert/strict';
import {spawn,execFileSync} from 'node:child_process';
import {fileURLToPath} from 'node:url';
const ROOT=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
assert.equal(process.argv.length,2,'No target or PID arguments accepted');
const CWD=path.join(ROOT,'scratch','cp210n03');
const meta=JSON.parse(fs.readFileSync(path.join(CWD,'metadata.json'),'utf8'));
const EXE=path.join(CWD,'fixture.exe');
const HELPER=path.join(ROOT,'admin','owned_suspended_process.py');
const PYTHON='C:\\Program Files\\Python312\\python.exe'.replaceAll('\\\\','\\');
const FRIDA_URL='file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
const sha=b=>crypto.createHash('sha256').update(b).digest('hex');
assert.equal(meta.exe.toLowerCase(),EXE.toLowerCase());
assert.equal(sha(fs.readFileSync(EXE)),meta.sha256);
assert.equal(sha(fs.readFileSync(HELPER)),'8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d');
const OUT=path.join(ROOT,'analysis','resume210-native-frida-'+new Date().toISOString().replaceAll(/[:.]/g,'-'));
fs.mkdirSync(OUT);
const source=fs.readFileSync(path.join(ROOT,'admin','original_outer_loader_frida_probe.mjs'),'utf8');
assert.match(source,/const ORIGINAL_RUN_REVIEWED = false;/);
assert.match(source,/const HOOK_BYTES_REVIEWED = false;/);
const begin=source.indexOf('function injectedSource(creationFiletimeExact) {');
const end=source.indexOf('\nfunction completionPrerequisites(',begin);
assert.ok(begin>=0&&end>begin);
// Reuse actual generated callbacks; ONLY identity, hook RVAs/prefixes and
// synthetic module-size checks change. No original process is opened.
const functionSource=source.slice(begin,end).replaceAll('def_gui_mo_64.exe','fixture.exe').replaceAll('0x535f000',String(meta.size));
const definitions=`const EXE=${JSON.stringify(EXE)};const HOOKS=${JSON.stringify(meta.hooks)};`+
 source.match(/const EVENT_KINDS = \[[^\n]+;/)[0]+ '\n'+
 source.match(/const EVENT_CAP_PER_KIND = \d+;/)[0]+'\n'+source.match(/const EVENT_CAP_TOTAL = \d+;/)[0];
const generate=vm.runInNewContext(definitions+'\n'+functionSource+'\ninjectedSource;');
const packagePath=fileURLToPath(new URL('../../package.json',FRIDA_URL));
assert.equal(JSON.parse(fs.readFileSync(packagePath,'utf8')).version,'17.23.2');
const {getLocalDevice}=await import(FRIDA_URL);
const device=await getLocalDevice();
const summary={scope:'synthetic_native_six_entry_fixture_only', originalExecuted:false,guiControlAcquired:false,
  runnerSha256:sha(fs.readFileSync(fileURLToPath(import.meta.url))), observerSha256:sha(source),
  helperSha256:sha(fs.readFileSync(HELPER)), executable:meta, fridaVersion:'17.23.2',trials:[]};
const bounded=(p,ms,label)=>{let t;return Promise.race([p,new Promise((_,reject)=>{t=setTimeout(()=>reject(Error(label+' timeout; outcome uncertain')),ms);})]).finally(()=>clearTimeout(t));};
function windowsPresence(pid){
 const command=`$ErrorActionPreference='Stop'; $rows=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' -ErrorAction Stop | Select-Object ProcessId,CreationDate,ExecutablePath,CommandLine); [pscustomobject]@{pid=${pid};present=[bool]($rows.Count -gt 0);processes=@($rows)} | ConvertTo-Json -Depth 4 -Compress`;
 const raw=execFileSync('powershell.exe',['-NoProfile','-NonInteractive','-Command',command],{encoding:'utf8',windowsHide:true,timeout:5000}).trim();
 const parsed=JSON.parse(raw);assert.equal(parsed.pid,pid);assert.equal(parsed.present,parsed.processes.length>0);return {raw,parsed};
}
async function trial(mode,injectFailure=false){
 const row={mode,injectFailure,rawLines:[],messages:[],failures:[]};summary.trials.push(row);
 let helper,session,script,buffer='',readyResolve,readyReject,drainResolve;
 const ready=new Promise((a,b)=>{readyResolve=a;readyReject=b;});ready.catch(()=>{});
 const drained=new Promise(a=>{drainResolve=a;});
 let exitPromise;
 const command=c=>{if(helper&&!helper.stdin.destroyed)helper.stdin.write(JSON.stringify({command:c})+'\n');};
 try{
  helper=spawn(PYTHON,['-I','-S',HELPER,'--exe',EXE,'--cwd',CWD,'--sha256',meta.sha256,'--max-seconds','20','--',String(mode)],{cwd:ROOT,windowsHide:true,stdio:['pipe','pipe','pipe']});
  row.helperPid=helper.pid;
  helper.stdin.on('error',e=>{row.stdinError=String(e);});
  helper.stdout.setEncoding('utf8');helper.stdout.on('data',chunk=>{
   if(buffer.length+chunk.length>65536){row.transportOverflow=true;buffer='';command('terminate');return;}
   buffer+=chunk;let i;
   while((i=buffer.indexOf('\n'))>=0){const line=buffer.slice(0,i);buffer=buffer.slice(i+1);row.rawLines.push(line);
    try{const record=JSON.parse(line);if(record.event==='created'){row.created=record;row.filetime=line.match(/"creation_filetime"\s*:\s*(\d+)/)?.[1];readyResolve(record);}
     if(record.event==='cleanup')row.cleanup=record;if(record.event==='resumed')row.resumed=record;
     if(['error','cleanup_error'].includes(record.event)){row.helperError=record;readyReject(Error('Helper error'));}
    }catch(e){row.parseError=String(e);readyReject(e);}
   }
  });
  helper.stdout.on('end',()=>{row.stdoutEnded=true;drainResolve();});
  helper.stderr.setEncoding('utf8');helper.stderr.on('data',s=>{row.stderr=(row.stderr??'')+s;});
  exitPromise=new Promise((a,b)=>{helper.on('error',b);helper.on('close',(code,signal)=>{row.helperExit={code,signal};a();});});
  await bounded(ready,4000,'owned suspended create');
  assert.equal(row.created.identity.canonical_image.toLowerCase(),EXE.toLowerCase());
  assert.equal(row.created.identity.sha256,meta.sha256);assert.match(row.filetime,/^\d+$/);
  assert.deepEqual(row.created.identity.argv.slice(1),[String(mode)]);
  const agent=generate(row.filetime);fs.writeFileSync(path.join(OUT,`agent-${mode}-${injectFailure}.js`),agent);
  row.agentSha256=sha(agent);
  session=await bounded(device.attach(row.created.identity.pid),5000,'fixture attach');
  script=await bounded(session.createScript(agent),3000,'script creation');
  let handlerResolve,handlerReject;const handlerReady=new Promise((a,b)=>{handlerResolve=a;handlerReject=b;});
  script.message.connect(message=>{row.messages.push(message);if(message.type==='send'&&message.payload?.kind==='handler-ready')handlerResolve(message.payload);if(message.type==='error')handlerReject(Error(message.description));});
  await bounded(script.load(),3000,'script load');row.handlerReady=await bounded(handlerReady,3000,'handler readiness');
  assert.equal(row.handlerReady.hooks.length,6);assert.equal(row.handlerReady.pid,row.created.identity.pid);
  assert.equal(row.handlerReady.module.path.toLowerCase(),EXE.toLowerCase());
  if(injectFailure){row.intentionalFailure='after-hook-ready-before-resume';throw Error(row.intentionalFailure);}
  command('resume');row.resumeRequested=true;
  await bounded(exitPromise,5000,'natural child/helper AV exit');
 }catch(e){row.failures.push(String(e));command('terminate');}
 finally{
  if(exitPromise){try{await bounded(exitPromise,22000,'owned helper cleanup');await bounded(drained,2000,'stdout drain');}catch(e){row.failures.push(String(e));}}
  row.partialLine=buffer;
  try{if(session){assert.equal(typeof session.isDetached,'function');if(!session.isDetached()){if(script)await bounded(script.unload(),2000,'unload');await bounded(session.detach(),2000,'detach');}row.detached=session.isDetached();}}catch(e){row.failures.push(String(e));}
  if(row.created?.identity?.pid){
   const pid=row.created.identity.pid;
   try{row.windowsAfter=windowsPresence(pid);const rows=await bounded(device.enumerateProcesses(),3000,'Frida enumeration');row.fridaAfter=rows.filter(p=>p.pid===pid).map(p=>({pid:p.pid,name:p.name}));row.absent=row.windowsAfter.parsed.present===false&&row.fridaAfter.length===0;}catch(e){row.failures.push(String(e));}
  }
  fs.writeFileSync(path.join(OUT,`helper-${mode}-${injectFailure}.jsonl`),row.rawLines.join('\n')+'\n');
  fs.writeFileSync(path.join(OUT,`messages-${mode}-${injectFailure}.json`),JSON.stringify(row.messages,null,2));
 }
 const payloads=row.messages.filter(m=>m.type==='send').map(m=>m.payload);
 const events=payloads.filter(m=>m.kind==='hook');const core=payloads.find(m=>m.kind==='first-av-core');
 row.snapshotParity=!!core&&events.length===core.hookSnapshot.sequence&&events.every((e,i)=>e.sequence===i+1)&&Object.entries(core.hookSnapshot.counts).every(([kind,count])=>events.filter(e=>e.event===kind).length===count)&&Object.values(core.hookSnapshot.overflow).every(n=>n===0);
 row.core=core;row.eventKinds=events.map(e=>e.event);
 const expected=['outer-enter',...meta.hooks.slice(1,mode===5?6:mode+2).map(h=>h.name+'-return'),'outer-return'];
 const nativeNormal=mode===5?events.at(-1)?.rawReturn==='0x100':events.at(-1)?.rawReturn==='0x0';
 const clean=row.cleanup?.exit_verified===true&&row.cleanup.handles_closed===true&&row.cleanup.wait_result===0&&row.helperExit?.code===0&&row.absent===true&&row.detached===true&&row.stdoutEnded===true&&!buffer&&!row.transportOverflow&&!row.helperError&&!row.parseError&&!row.stdinError;
 row.passed=injectFailure?clean&&row.cleanup.termination_requested===true&&row.failures.length===1&&row.failures[0]==='Error: after-hook-ready-before-resume'&&row.resumed==null:
 clean&&row.failures.length===0&&row.cleanup.termination_requested===false&&row.cleanup.exit_code===0xc0000005&&row.snapshotParity&&JSON.stringify(row.eventKinds)===JSON.stringify(expected)&&nativeNormal&&core?.exceptionType==='access-violation'&&core?.memoryOperation?.address==='0x58'&&core?.disposition==='return_false_continue_natural_exception';
 fs.writeFileSync(path.join(OUT,`trial-${mode}-${injectFailure}.json`),JSON.stringify(row,null,2));
 console.log(JSON.stringify({mode,injectFailure,passed:row.passed,events:row.eventKinds,exit:row.cleanup?.exit_code,absent:row.absent}));
 if(!row.passed)throw Error('Fixture trial failed; stop further launches');
}
try{for(let mode=0;mode<=5;mode++)await trial(mode);await trial(5,true);summary.outcome='PASS';}
catch(e){summary.outcome='FAIL_INCOMPLETE';summary.error=String(e);process.exitCode=1;}
finally{summary.finishedUtc=new Date().toISOString();fs.writeFileSync(path.join(OUT,'summary.json'),JSON.stringify(summary,null,2));console.log(JSON.stringify({outcome:summary.outcome,out:OUT,trials:summary.trials.length}));}
