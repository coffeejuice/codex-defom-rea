// Executes actual generated agent and host predicates. No Frida/original process.
import assert from 'node:assert/strict';
import vm from 'node:vm';
import fs from 'node:fs';
import {patchReadinessQualified, injectedSource, HOOKS, EXE, PROJECT, CLI_ARG, assertEncodedArgument, qualifyObservation, createEndpoint, completionPrerequisites, cleanupComplete} from './original_view_xml_frida_probe.mjs';
const runner=fs.readFileSync(new URL('./original_view_xml_frida_probe.mjs',import.meta.url),'utf8');
for(const guard of ['ORIGINAL_RUN_REVIEWED','HOOK_BYTES_REVIEWED','QSTRING_LAYOUT_REVIEWED']) assert.match(runner,new RegExp(`const ${guard} = false;`));
assertEncodedArgument(CLI_ARG,PROJECT);
for(const bad of [PROJECT,CLI_ARG.slice(1),CLI_ARG.toUpperCase(),CLI_ARG+'0000','4300'+CLI_ARG.slice(4)]) assert.throws(()=>assertEncodedArgument(bad,PROJECT));
assert.throws(()=>assertEncodedArgument(CLI_ARG,PROJECT+'x'));
const BASE=0x140000000n, QT=0x58000000n, MAIN=0x700000n, OBJECT=MAIN+0x18d8n, HEADER=0x900000n, DATA=0xa00000n;
let memory, patches, options, headerReads, parserReads;
class P {
 and(n){return new P(this.n & BigInt(n));}
 constructor(n){this.n=BigInt(n instanceof P?n.n:n);} add(n){return new P(this.n+BigInt(n));} sub(n){return new P(this.n-new P(n).n);} equals(n){return this.n===new P(n).n;} compare(n){return this.n<new P(n).n?-1:this.n>new P(n).n?1:0;} isNull(){return this.n===0n;} toString(){return '0x'+this.n.toString(16);} toUInt32(){return Number(this.n&0xffffffffn);}
 readByteArray(n){
  if(this.n===0x155n && ++parserReads===2 && options.parserRace) memory.set(0x155n,0);
  if(this.n===HEADER && ++headerReads===2 && options.race) memory.set(HEADER+8n,35);
  if(options.partial && this.n===DATA) n--;
  const patch=patches.get(this.toString());if(patch) return Uint8Array.from(patch.match(/../g),b=>parseInt(b,16)).buffer;
  const out=[];for(let i=0;i<n;i++){if(!memory.has(this.n+BigInt(i)))throw Error('unreadable');out.push(memory.get(this.n+BigInt(i)));}return Uint8Array.from(out).buffer;
 }
 readPointer(){const b=new Uint8Array(this.readByteArray(8));return new P(new DataView(b.buffer).getBigUint64(0,true));}
 readS32(){const b=new Uint8Array(this.readByteArray(4));return new DataView(b.buffer).getInt32(0,true);}
 readU32(){return this.readS32()>>>0;} readU16(){const b=new Uint8Array(this.readByteArray(2));return new DataView(b.buffer).getUint16(0,true);}
}
const ptr=n=>new P(n), put=(address,bytes)=>bytes.forEach((b,i)=>memory.set(address+BigInt(i),b));
const number=(address,n,bytes=8)=>{const a=new Uint8Array(bytes),v=new DataView(a.buffer);if(bytes===8)v.setBigUint64(0,BigInt(n),true);else v.setInt32(0,n,true);put(address,a);};
function fixture(o={}) {
 options=o;headerReads=0;parserReads=0;memory=new Map();patches=new Map();
 put(0x155n,[o.parserByte??1]);number(0xc00000n,0xb00000n);number(0xb001b8n,o.slotTarget??BASE+0x170430n);
 number(OBJECT,HEADER);put(HEADER,new Uint8Array(26));number(HEADER,o.ref??1,4);number(HEADER+4n,o.capacity??34,4);number(HEADER+8n,o.length??34,4);number(HEADER+16n,o.nullData?0:DATA);
 put(QT,[0x4d,0x5a]);number(QT+0x3cn,0x80,4);number(QT+0x80n,0x4550,4);put(QT+0x84n,[0x64,0x86]);number(QT+0x88n,0x517180cf,4);put(QT+0x98n,[0x0b,0x02]);number(QT+0xd0n,0x31f000,4);
 const units=Array.from({length:34},(_,i)=>PROJECT.charCodeAt(i));if(o.zero)units[3]=0;
 put(DATA,units.flatMap(n=>[n&255,n>>8]));
 for(const h of HOOKS)put(BASE+BigInt(h.rva),Uint8Array.from(h.bytes.match(/../g),b=>parseInt(b,16)));
 for(const [r,b]of [[0xfec0,'488b018b4008c3'],[0x2e90,'488b018b4004c3'],[0x47940,'488b01488b4010c3']])put(QT+BigInt(r),Uint8Array.from(b.match(/../g),x=>parseInt(x,16)));
 const messages=[],hooks=new Map();let exception;
 const app={name:'def_gui_mo_64.exe',path:EXE,base:ptr(BASE),size:0x535f000};
 const qt={name:'QtCore4_64.dll',path:EXE.replace('DEF_GUI_MO_64.exe','QtCore4_64.dll'),base:ptr(QT),size:0x31f000};
 const ctx={Process:{id:9001,platform:'windows',arch:'x64',mainModule:app,getCurrentThreadId:()=>101,findModuleByName:()=>qt,enumerateModules:()=>[app,qt],findModuleByAddress:()=>app,findRangeByAddress:a=>o.unreadable&&a.n===DATA?null:{base:ptr(0x1000),size:0x200000000,protection:'rw-'},setExceptionHandler:f=>exception=f},
  Interceptor:{attach:(a,c)=>{hooks.set(a.toString(),c);const h=HOOKS.find(h=>BASE+BigInt(h.rva)===a.n);patches.set(a.toString(),(o.badPatch===h.name?'ff':'e9')+'00100000'+((h.name==='activation'||h.name==='viewbind')?(o.badPadding?'90909090':'0f1f4000')+(o.badSuffix?'cc'+h.bytes.slice(20):h.bytes.slice(18)):h.bytes.slice(10)));},flush(){}},ptr,send:p=>messages.push(JSON.parse(JSON.stringify(p))),DebugSymbol:{fromAddress:a=>a},Thread:{backtrace:()=>[]},Backtracer:{ACCURATE:1}};
 if(o.badPatch||o.badPadding||o.badSuffix){assert.throws(()=>vm.runInNewContext(injectedSource('123456789'),ctx),/five-byte gate/);assert.equal(messages.some(m=>m.kind==='handler-ready'),false);return {messages};}
 vm.runInNewContext(injectedSource('123456789'),ctx);
 const call=(hook,rsp,rcx=MAIN)=>({returnAddress:ptr(BASE+BigInt(hook.callerRva)),context:{rcx:ptr(rcx),rdx:ptr(OBJECT),rsp:ptr(rsp)}});
 const outer=call(HOOKS[0],0x800000n);hooks.get(ptr(BASE+BigInt(HOOKS[0].rva)).toString()).onEnter.call(outer);
 return {messages,hooks,call,outer,stage(i,fail=false){const h=HOOKS[i+1],c=call(h,0x7ffe20n,i===4?0x100n:MAIN),cb=hooks.get(ptr(BASE+BigInt(h.rva)).toString());cb.onEnter.call(c);cb.onLeave.call(c,ptr(i===0?fail?1:0:i===4?fail?0x100:0x101:fail?0:0x100));},leave(n){hooks.get(ptr(BASE+BigInt(HOOKS[0].rva)).toString()).onLeave.call(outer,ptr(n));},exception};
}
function complete(o={}) {
 const f=fixture(o);for(let i=0;i<5;i++)f.stage(i);f.leave(0x100);
 const enter=(name,rcx=MAIN,rdx=0x100n,rsp=0x800080n,caller=null)=>{const h=HOOKS.find(h=>h.name===name),c={returnAddress:ptr(BASE+BigInt(caller??h.callerRva)),context:{rcx:ptr(rcx),rdx:ptr(rdx),rsp:ptr(rsp)}};f.hooks.get(ptr(BASE+BigInt(h.rva)).toString()).onEnter.call(c);return c;};
 const leave=(name,c,value=0)=>f.hooks.get(ptr(BASE+BigInt(HOOKS.find(h=>h.name===name).rva)).toString()).onLeave.call(c,ptr(value));
 const p=enter('postopen'),current=enter('current',0x888n,0n,0x7ffba0n);leave('current',current,0x100);
 const activation=enter('activation',0x100n,0n,0x7ffba0n);leave('activation',activation);
 const v=enter('viewbind',o.main??MAIN,o.dl??1n,0x7ffba0n,o.caller);
 if(o.nested){const nested=enter('viewbind',MAIN,1n,0x7ffb00n,0x99999);leave('viewbind',nested);}
 const c=enter('current',0x888n,0n,0x7ffad0n,0x383b1);leave('current',c,o.current??0x100n);
 if(!o.skip){const a=enter('viewaccess',MAIN,0n,0x7ffad0n);leave('viewaccess',a,o.view??0xc00000n);
  if(!o.noBinding){const b=enter('modelbind',o.receiver??o.view??0xc00000n,o.model??0xd00000n,0x7ffad0n);if(!o.missingLeave)leave('modelbind',b);}
 }
 if(!o.missingView)leave('viewbind',v);if(!o.missingPost)leave('postopen',p);
 return {...f,enter,leave};
}
const qualify=f=>qualifyObservation(f.messages.map(payload=>({type:'send',payload})),{pid:9001,primary_tid:101},'123456789',f.messages[0]);
const valid=qualify(complete());assert.equal(valid.viewObservationComplete,true,JSON.stringify(valid));assert.equal(valid.nonnullViewBindingQualified,true);assert.equal(valid.parserStateQualified,true);assert.equal(valid.xmlReadStatus,1);
const xmlFalse=qualify(complete({parserByte:0}));assert.equal(xmlFalse.parserStateQualified,true);assert.equal(xmlFalse.xmlReadStatus,0);assert.equal(xmlFalse.nonnullViewBindingQualified,true);
for(const o of [{parserByte:2},{parserRace:true},{main:MAIN+8n},{dl:0n},{caller:0x99999},{nested:true},{receiver:0xc00008n},{slotTarget:BASE+0x170431n},{missingLeave:true},{missingView:true},{missingPost:true}])assert.equal(qualify(complete(o)).viewObservationComplete,false,JSON.stringify(o,(_,v)=>typeof v==='bigint'?String(v):v));
const negative=qualify(complete({model:0n}));assert.equal(negative.viewObservationComplete,true);assert.equal(negative.nonnullViewBindingQualified,false);
const different=qualify(complete({current:0x200n}));assert.equal(different.viewObservationComplete,true);assert.equal(different.nonnullViewBindingQualified,false);
const skipped=qualify(complete({skip:true}));assert.equal(skipped.viewObservationComplete,true);assert.equal(skipped.nonnullViewBindingQualified,false);
const absent=qualify(complete({view:0n,noBinding:true}));assert.equal(absent.viewObservationComplete,true);assert.equal(absent.nonnullViewBindingQualified,false);
const completeFixture=complete(),messages=completeFixture.messages.map(payload=>({type:'send',payload})),ready=completeFixture.messages[0];
for(const alter of [rows=>{rows.find(m=>m.payload.event==='modelbind-enter').payload.tid++;},rows=>{rows.find(m=>m.payload.event==='modelbind-enter').payload.processCreationFiletime='1';},rows=>{rows.find(m=>m.payload.event==='viewcurrent-return').payload.pid++;},rows=>{rows.find(m=>m.payload.event==='modelbind-return').payload.sequence++;}]){const rows=JSON.parse(JSON.stringify(messages));alter(rows);assert.equal(qualifyObservation(rows,{pid:9001,primary_tid:101},'123456789',ready).viewObservationComplete,false);}
for(const hook of HOOKS)fixture({badPatch:hook.name});fixture({badPadding:true});fixture({badSuffix:true});
assert.equal(patchReadinessQualified(ready,HOOKS),true);
for(const i of [8,9]){const bad=JSON.parse(JSON.stringify(ready));bad.patches[i].displacedLength=5;assert.equal(patchReadinessQualified(bad,HOOKS),false);}
const clean={cleanupVerified:true,processAbsent:true,helperStdoutClosed:true,scriptUnloadSettled:true,sessionDetached:true,rawHelperReceiptsRetained:true,failures:[],patchReadinessQualified:true,openObservationComplete:true,encodedPathQualified:true,postOpenObservationComplete:true,predecessorChainQualified:true,viewObservationComplete:true,parserStateQualified:true,sourcePreserved:true,fixtureAfter:{},resumedReceipt:{previous_suspend_count:1,state:'running'},observationEndpoint:{reason:'postopen-return',elapsedMs:1000},intentionalTerminationRequested:true};
assert.equal(completionPrerequisites(clean,null,'',false),true);for(const field of ['processAbsent','parserStateQualified','viewObservationComplete','predecessorChainQualified'])assert.equal(completionPrerequisites({...clean,[field]:false},null,'',false),false);
console.log(JSON.stringify({outcome:'PASS',scope:'cp215_actual_generated_agent_and_host',originalExecuted:false,guiControlAcquired:false}));
