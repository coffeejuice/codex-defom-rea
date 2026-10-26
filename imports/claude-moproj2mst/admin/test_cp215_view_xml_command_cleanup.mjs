// Actual generated runner endpoint/finally snippets and real Node Writable streams.
// No process spawn, Frida import, original application or GUI control.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
import {Writable} from 'node:stream';
import {createCommandChannel} from './original_view_xml_frida_probe.mjs';
const source=fs.readFileSync(new URL('./original_view_xml_frida_probe.mjs',import.meta.url),'utf8');
for(const guard of ['ORIGINAL_RUN_REVIEWED','HOOK_BYTES_REVIEWED','QSTRING_LAYOUT_REVIEWED']) assert.match(source,new RegExp(`const ${guard} = false;`));
const endpoint=source.match(/    result.intentionalTerminationRequested = true;\s+commandChannel.terminateAndEnd\(\);/)[0];
const finalCleanup=source.match(/      if \(helper\) \{ result.intentionalTerminationRequested = true; commandChannel.terminateAndEnd\(\); \}/)[0];
assert.equal(source.includes('helper.stdin.end()'),false);
assert.match(source,/const sendCommand = command => commandChannel.send\(command\);/);
function stream(){const writes=[],errors=[];let endCalls=0;const w=new Writable({autoDestroy:false,write(chunk,encoding,done){writes.push(chunk.toString());done();}});w.on('error',e=>errors.push(e));const end=w.end.bind(w);w.end=(...a)=>{endCalls++;return end(...a);};return {w,writes,errors,endCalls:()=>endCalls};}
// Reproduce original timing: writableEnded=true while destroyed remains false.
const a=stream(),channel=createCommandChannel(()=>a.w),result={};
vm.runInNewContext(`try {${endpoint}} finally {${finalCleanup}}`,{commandChannel:channel,result,helper:{stdin:a.w}});
assert.equal(a.w.writableEnded,true);assert.equal(a.w.destroyed,false);
assert.deepEqual(a.writes,['{"command":"terminate"}\n']);assert.equal(a.endCalls(),1);
channel.terminateAndEnd();assert.equal(channel.send('terminate'),false);assert.equal(channel.send('status'),false);
await new Promise(resolve=>setImmediate(resolve));assert.equal(a.errors.length,0);assert.equal(result.intentionalTerminationRequested,true);
// An earlier installation/catch termination is not repeated by final cleanup.
const b=stream(),second=createCommandChannel(()=>b.w);assert.equal(second.send('terminate'),true);assert.equal(second.send('terminate'),false);second.terminateAndEnd();second.terminateAndEnd();assert.equal(b.writes.length,1);assert.equal(b.endCalls(),1);
// Already-ended or destroyed streams, and an absent helper, never receive input.
const c=stream();c.w.end();const third=createCommandChannel(()=>c.w);third.terminateAndEnd();assert.equal(c.writes.length,0);assert.equal(c.endCalls(),1);
const d=stream();d.w.destroy();createCommandChannel(()=>d.w).terminateAndEnd();assert.equal(d.writes.length,0);assert.equal(d.endCalls(),0);
const missing=createCommandChannel(()=>undefined);assert.equal(missing.send('terminate'),false);assert.deepEqual(missing.terminateAndEnd(),{terminationSent:false,ended:false});
// A false write return remains an enqueued termination; no retry is permitted.
let count=0,ends=0;const backpressure={destroyed:false,writableEnded:false,writableFinished:false,writable:true,write(){count++;return false;},end(){ends++;}};
const fourth=createCommandChannel(()=>backpressure);fourth.terminateAndEnd();fourth.terminateAndEnd();assert.equal(count,1);assert.equal(ends,1);
await new Promise(resolve=>setImmediate(resolve));for(const s of [a,b,c,d])assert.equal(s.errors.length,0);
console.log(JSON.stringify({outcome:'PASS',scope:'actual_generated_endpoint_finally_real_writable_regression',cases:7,originalExecuted:false,guiControlAcquired:false}));
