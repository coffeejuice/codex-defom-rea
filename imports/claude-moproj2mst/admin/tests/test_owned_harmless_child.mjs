// Exercises only Python + the harmless sleeping fixture through the held-handle helper.
import { spawn } from 'node:child_process';
import fs from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const here=path.dirname(fileURLToPath(import.meta.url)), root=path.resolve(here,'../..');
const helper=path.join(root,'admin/owned_suspended_process.py');
const fixture=path.join(here,'harmless_suspended_child.py');
const python='C:\\Program Files\\Python312\\python.exe';
const hash=x=>crypto.createHash('sha256').update(fs.readFileSync(x)).digest('hex');
if(process.platform!=='win32') throw new Error('Windows-only helper fixture');
const child=spawn(python,[helper,'--exe',python,'--cwd',here,'--sha256',hash(python),'--max-seconds','20','--',fixture],
  {cwd:here,windowsHide:true,stdio:['pipe','pipe','pipe']});
let raw='',err='',pending=''; const events=[];
child.stdout.setEncoding('utf8').on('data',s=>{raw+=s;pending+=s;let n;while((n=pending.indexOf('\n'))>=0){const line=pending.slice(0,n);pending=pending.slice(n+1);events.push(JSON.parse(line.replace(/("creation_filetime"\s*:\s*)(\d+)/g,'$1"$2"')));}});
child.stderr.setEncoding('utf8').on('data',s=>err+=s);
const wait=async name=>{const end=Date.now()+8000;while(Date.now()<end){const e=events.find(x=>x.event===name);if(e)return e;if(events.some(x=>x.event==='error'||x.event==='cleanup_error'))throw new Error(raw);await new Promise(r=>setTimeout(r,20));}throw new Error(`timeout waiting ${name}: ${raw}`);};
const created=await wait('created');
if(created.state!=='suspended'||created.exit_verified!==false||created.identity.sha256!==hash(python)) throw new Error('bad held-child created receipt');
child.stdin.write('{"command":"resume"}\n');
const resumed=await wait('resumed'); if(resumed.previous_suspend_count!==1) throw new Error('resume did not report suspend count one');
child.stdin.write('{"command":"terminate"}\n'); child.stdin.end();
const close=await new Promise(resolve=>{const t=setTimeout(()=>resolve({timeout:true}),10000);child.once('close',(code,signal)=>{clearTimeout(t);resolve({code,signal});});});
if(close.code!==0) throw new Error(`helper failed ${JSON.stringify(close)} ${err} ${raw}`);
const cleanup=events.find(x=>x.event==='cleanup');
if(cleanup?.exit_verified!==true||cleanup?.handles_closed!==true||cleanup?.wait_result!==0) throw new Error(`cleanup unverified: ${raw}`);
process.stdout.write(JSON.stringify({test:'harmless-owned-child',created,resumed,endpoint:events.find(x=>x.event==='endpoint'),cleanup,helperExit:close,stderrBytes:Buffer.byteLength(err)})+'\n');
