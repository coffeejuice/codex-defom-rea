// Fixed synthetic child only. No arbitrary path/PID arguments; no DEFORM launch.
import fs from 'node:fs';import path from 'node:path';import crypto from 'node:crypto';import assert from 'node:assert/strict';import {spawn,execFileSync} from 'node:child_process';import {fileURLToPath} from 'node:url';
import {injectedSource,HOOKS,createEndpoint,patchReadinessQualified,qualifyObservation} from './original_postopen_frida_probe.mjs';
assert.equal(process.argv.length,2);
const ROOT=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'), CWD=path.join(ROOT,'scratch','cp214p02'), EXE=path.join(CWD,'fixture.exe');
const meta=JSON.parse(fs.readFileSync(path.join(CWD,'metadata.json'))),HELPER=path.join(ROOT,'admin','cp214_owned_suspended_process.py');
const sha=b=>crypto.createHash('sha256').update(b).digest('hex');assert.equal(sha(fs.readFileSync(EXE)),meta.sha256);assert.equal(meta.exe,EXE);
assert.equal(sha(fs.readFileSync(HELPER)),'0e5389f7dd3f734557d87725e8dad422a3daafc507b230f060fb900da5c14edf');
const FRIDA='file:///C:/Users/alext/AppData/Roaming/npm/node_modules/rea-agents/node_modules/frida/build/src/frida.js';
assert.equal(JSON.parse(fs.readFileSync(fileURLToPath(new URL('../../package.json',FRIDA)))).version,'17.23.2');
const {getLocalDevice}=await import(FRIDA),device=await getLocalDevice();
const OUT=path.join(ROOT,'analysis','resume214-native-postopen-'+new Date().toISOString().replaceAll(/[:.]/g,'-'));fs.mkdirSync(OUT);
const summary={originalExecuted:false,guiControlAcquired:false,scope:'synthetic nine prologue shapes plus real held-handle lifecycle; synthetic Qt header in fixture memory, no Qt DLL',meta,trials:[]};
const bounded=(p,ms,label)=>{let t;return Promise.race([p,new Promise((_,r)=>t=setTimeout(()=>r(Error(label+' timeout')),ms))]).finally(()=>clearTimeout(t));};
function absent(pid){const cmd=`$ErrorActionPreference='Stop';$p=@(Get-CimInstance Win32_Process -Filter 'ProcessId = ${pid}' -ErrorAction Stop);[pscustomobject]@{pid=${pid};present=[bool]($p.Count -gt 0)}|ConvertTo-Json -Compress`;return JSON.parse(execFileSync('powershell.exe',['-NoProfile','-NonInteractive','-Command',cmd],{encoding:'utf8',windowsHide:true,timeout:5000}));}
async function trial(mode,preResumeReject=false){
 const row={mode,preResumeReject,messages:[],raw:[],failures:[]};summary.trials.push(row);let child,session,script,buffer='',done,created,drained,endpoint=createEndpoint(1000);let makeReady,rejectReady;
 const ready=new Promise((a,b)=>{makeReady=a;rejectReady=b;});ready.catch(()=>{});
 const command=c=>{if(child&&!child.stdin.destroyed)child.stdin.write(JSON.stringify({command:c})+'\n');};
 try{
  child=spawn('C:\\Program Files\\Python312\\python.exe',['-I','-S',HELPER,'--exe',EXE,'--cwd',CWD,'--sha256',meta.sha256,'--max-seconds','30','--',String(mode)],{cwd:ROOT,windowsHide:true,stdio:['pipe','pipe','pipe']});
  child.stdin.on('error',e=>row.stdinError=String(e));
  child.stdout.setEncoding('utf8');child.stdout.on('data',c=>{buffer+=c;if(buffer.length>65536){row.overflow=true;command('terminate');return;}let i;while((i=buffer.indexOf('\n'))>=0){const line=buffer.slice(0,i);buffer=buffer.slice(i+1);row.raw.push(line);try{const r=JSON.parse(line);if(r.event==='created'){row.created=r;row.filetime=line.match(/"creation_filetime"\s*:\s*(\d+)/)[1];makeReady(r);}if(r.event==='cleanup')row.cleanup=r;if(r.event==='resumed')row.resumed=r;if(['error','cleanup_error'].includes(r.event)){row.helperError=r;rejectReady(Error('helper error'));}}catch(e){row.parseError=String(e);rejectReady(e);}}});
  child.stderr.setEncoding('utf8');child.stderr.on('data',c=>row.stderr=(row.stderr??'')+c);
  drained=new Promise(a=>child.stdout.on('end',()=>{row.stdoutEnded=true;a();}));
  done=new Promise((a,b)=>{child.on('error',b);child.on('close',(code,signal)=>{row.helperExit={code,signal};a();});});
  created=await bounded(ready,4000,'created');assert.equal(created.state,'suspended');assert.equal(created.identity.sha256,meta.sha256);assert.equal(created.identity.launcher_pid,child.pid);assert.equal(created.identity.canonical_image.toLowerCase(),EXE.toLowerCase());assert.equal(created.identity.cwd.toLowerCase(),CWD.toLowerCase());assert.deepEqual(created.identity.argv.slice(1),[String(mode)]);
  // Identity substitutions are explicit synthetic-only adaptations. Agent callbacks/readers otherwise identical.
  let agent=injectedSource(row.filetime).replace(JSON.stringify(HOOKS),JSON.stringify(meta.hooks)).replaceAll('def_gui_mo_64.exe','fixture.exe').replaceAll('0x535f000',String(meta.size));
  agent=agent.replace(/app\.path\.toLowerCase\(\) !== "[^"]+"/,`app.path.toLowerCase() !== ${JSON.stringify(EXE.toLowerCase())}`);
  agent=agent.replace(/const qt = Process.findModuleByName\('QtCore4_64.dll'\);[\s\S]*?throw new Error\('Loaded QtCore identity mismatch'\);/, 'const qt = Process.mainModule;');
  agent=agent.replace(/const qtGetters = \[[^\n]+;/,'const qtGetters = [];');
  agent=agent.replace(/const qtMatches = Process.enumerateModules\(\)[\s\S]*?throw new Error\('Loaded QtCore PE metadata mismatch'\);/,'// Synthetic fixture has no loaded QtCore; metadata qualification remains covered by actual-agent mocks.');
  fs.writeFileSync(path.join(OUT,`agent-${mode}-${preResumeReject}.js`),agent);
  session=await bounded(device.attach(created.identity.pid),8000,'attach');script=await bounded(session.createScript(agent),2000,'createScript');
  let installed,installReject;const installation=new Promise((a,b)=>{installed=a;installReject=b;});installation.catch(()=>{});
  script.message.connect(m=>{row.messages.push(m);endpoint.accept(m);if(m.type==='send'&&m.payload?.kind==='handler-ready')installed(m.payload);if(m.type==='error')installReject(Error(m.description));});
  await bounded(script.load(),2000,'load');row.handlerReady=await bounded(installation,2000,'readiness');assert.equal(patchReadinessQualified(row.handlerReady,meta.hooks),true);
  if(preResumeReject){row.endpoint={reason:'intentional_pre_resume_rejection'};}
  else{endpoint.start();command('resume');row.endpoint=await Promise.race([endpoint.wait,done.then(()=>({reason:'natural-exit'}))]);}
  command('terminate');if(!child.stdin.destroyed)child.stdin.end();row.terminationCommandSent=true;
 }catch(e){row.failures.push(String(e));command('terminate');if(child&&!child.stdin.destroyed)child.stdin.end();}
 finally{
  endpoint.cancel();if(done)try{await bounded(done,22000,'cleanup');await bounded(drained,2000,'drain');}catch(e){row.failures.push(String(e));}
  try{if(session){if(!session.isDetached()){if(script)await bounded(script.unload(),2000,'unload');await bounded(session.detach(),2000,'detach');}row.detached=session.isDetached();}}catch(e){row.failures.push(String(e));}
  if(created){try{row.windowsAfter=absent(created.identity.pid);row.fridaAfter=(await bounded(device.enumerateProcesses(),3000,'absence')).filter(p=>p.pid===created.identity.pid).map(p=>({pid:p.pid,name:p.name}));row.absent=!row.windowsAfter.present&&row.fridaAfter.length===0;}catch(e){row.failures.push(String(e));}}
  row.partialLine=buffer;fs.writeFileSync(path.join(OUT,`raw-${mode}-${preResumeReject}.jsonl`),row.raw.join('\n')+'\n');
 }
 const events=row.messages.filter(m=>m.type==='send'&&m.payload?.kind==='hook').map(m=>m.payload);
 row.eventKinds=events.map(e=>e.event);row.witnesses=events.filter(e=>e.pathWitness).map(e=>({event:e.event,witness:e.pathWitness}));
 const post=events.find(e=>e.event==='postopen-enter'), current=events.find(e=>e.event==='current-return'), a=events.find(e=>e.event==='activation-enter'), ar=events.find(e=>e.event==='activation-return');
 row.nativeWitnesses={postDelivered:post?.project==='0x100'&&post.main===events[0]?.main,current:current?.rawReturn,activationRCX:a?.rawRCX,activationEDX:a?.rawEDX,activationNormal:ar?.normalReturn===true,stackDelta:a&&post?String(BigInt(post.entryRsp)-BigInt(a.entryRsp)):null};
 if(mode===5&&!preResumeReject)assert.ok(row.nativeWitnesses.postDelivered&&current?.rawReturn==='0x100'&&a?.rawRCX==='0x100'&&a?.rawEDX===0&&ar?.normalReturn===true&&row.nativeWitnesses.stackDelta==='1248');
 if(mode===8)assert.ok(current?.rawReturn==='0x0'&&!a);
 const clean=row.cleanup?.exit_verified===true&&row.cleanup.handles_closed===true&&row.cleanup.wait_result===0&&row.helperExit?.code===0&&row.absent===true&&row.detached===true&&row.stdoutEnded===true&&!buffer&&!row.overflow&&!row.helperError&&!row.parseError&&!row.stdinError&&row.failures.length===0;
 row.passed=clean&&(preResumeReject?row.resumed==null&&row.cleanup.exit_code===0xe201:mode===6?row.cleanup.exit_code===42:mode===9?row.endpoint?.reason==='deadline'&&row.cleanup.exit_code===0xe201&&events.some(e=>e.event==='activation-enter')&&!events.some(e=>e.event==='activation-return'):row.endpoint?.reason==='postopen-return'&&row.cleanup.exit_code===0xe201&&events.at(-1)?.event==='postopen-return'&&row.witnesses.length===4&&row.witnesses.every(w=>w.witness.ok===true&&w.witness.length===34));
 fs.writeFileSync(path.join(OUT,`trial-${mode}-${preResumeReject}.json`),JSON.stringify(row,null,2));console.log(JSON.stringify({mode,preResumeReject,passed:row.passed,exit:row.cleanup?.exit_code,endpoint:row.endpoint?.reason,absent:row.absent}));assert.equal(row.passed,true);
}
try{await trial(5);await trial(8);await trial(9);await trial(6);await trial(5,true);summary.outcome='PASS';}catch(e){summary.outcome='FAIL';summary.error=String(e);process.exitCode=1;}
finally{fs.writeFileSync(path.join(OUT,'summary.json'),JSON.stringify(summary,null,2));console.log(JSON.stringify({outcome:summary.outcome,out:OUT}));}

