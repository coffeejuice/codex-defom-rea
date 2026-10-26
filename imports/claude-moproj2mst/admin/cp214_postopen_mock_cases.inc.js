function prepared(options={}) {
 const f=fixture(options);for(let i=0;i<5;i++)f.stage(i);f.leave(0x100);
 f.post=(name,rcx=MAIN,rdx=0x100n,rsp=0x800080n,caller=null)=>{const h=HOOKS.find(h=>h.name===name),c={returnAddress:ptr(BASE+BigInt(caller??h.callerRva)),context:{rcx:ptr(rcx),rdx:ptr(rdx),rsp:ptr(rsp)}};f.hooks.get(ptr(BASE+BigInt(h.rva)).toString()).onEnter.call(c);return c;};
 f.done=(name,c,value=0)=>f.hooks.get(ptr(BASE+BigInt(HOOKS.find(h=>h.name===name).rva)).toString()).onLeave.call(c,ptr(value));
 return f;
}
function complete(o={}) {
 const f=prepared(),p=f.post('postopen',o.main??MAIN,o.project??0x100n);
 if(o.nested){const q=f.post('postopen',MAIN,0x100n,0x800000n,0x99999);f.done('postopen',q);}
 const c=f.post('current',0x888n,0n,0x7ffba0n,o.caller);f.done('current',c,o.current??0x100n);
 if((o.current??0x100n)!==0n){const a=f.post('activation',o.activationProject??o.current??0x100n,o.index??0n,0x7ffba0n);if(!o.missingLeave)f.done('activation',a);}
 if(!o.missingPost)f.done('postopen',p);return f;
}
const qualify=f=>qualifyObservation(f.messages.map(payload=>({type:'send',payload})),{pid:9001,primary_tid:101},'123456789',f.messages[0]);
const valid=qualify(complete());assert.equal(valid.postOpenObservationComplete,true,JSON.stringify(valid));assert.equal(valid.intendedProjectActivationComplete,true);
const zero=qualify(complete({current:0n}));assert.equal(zero.postOpenObservationComplete,true,JSON.stringify(zero));assert.equal(zero.postOpenReturned,true);assert.equal(zero.activationReturned,false);
const other=qualify(complete({current:0x200n}));assert.equal(other.postOpenObservationComplete,true);assert.equal(other.currentProjectEqualsDelivered,false);assert.equal(other.intendedProjectActivationComplete,false);
const index=qualify(complete({index:1n}));assert.equal(index.postOpenObservationComplete,true);assert.equal(index.activationSubprojectZero,false);assert.equal(index.intendedProjectActivationComplete,false);
for(const bad of [{main:MAIN+8n},{project:0x200n},{nested:true},{caller:0x99999},{activationProject:0x300n},{missingLeave:true},{missingPost:true}])assert.equal(qualify(complete(bad)).postOpenObservationComplete,false,JSON.stringify(bad,(_,v)=>typeof v==='bigint'?String(v):v));
for(const hook of HOOKS)fixture({badPatch:hook.name});
fixture({badPadding:true});fixture({badSuffix:true});
const readiness=complete().messages.find(m=>m.kind==='handler-ready');
assert.equal(patchReadinessQualified(readiness,HOOKS),true);
for(const mutate of [r=>{r.patches[8].padding='90909090';},r=>{r.patches[8].displacedLength=5;},r=>{r.patches[8].redirectDestination='0x1';},r=>{r.patches[0].displacedLength=9;}]){const r=JSON.parse(JSON.stringify(readiness));mutate(r);assert.equal(patchReadinessQualified(r,HOOKS),false);}
const full=complete();const duplicate=full.post('postopen');full.done('postopen',duplicate);assert.equal(qualify(full).postOpenObservationComplete,false);
const attempted=qualify(complete({missingLeave:true,missingPost:true}));assert.equal(attempted.activationAttempted,true);assert.equal(attempted.activationReturned,false);assert.equal(attempted.postOpenReturned,false);
const ep=createEndpoint(10);ep.start();ep.accept({type:'send',payload:{kind:'hook',event:'outer-return'}});ep.accept({type:'send',payload:{kind:'hook',event:'postopen-return'}});assert.equal((await ep.wait).reason,'postopen-return');
let clock=0;const late=createEndpoint(10,()=>clock);late.start();clock=11;late.accept({type:'send',payload:{kind:'hook',event:'postopen-return'}});assert.equal((await late.wait).reason,'deadline');
const noReturn=createEndpoint(1);noReturn.start();assert.equal((await noReturn.wait).reason,'deadline');
const av=createEndpoint(100);av.start();av.accept({type:'send',payload:{kind:'first-av-core'}});assert.equal((await av.wait).reason,'first-av');
const clean={cleanupVerified:true,processAbsent:true,helperStdoutClosed:true,scriptUnloadSettled:true,sessionDetached:true,rawHelperReceiptsRetained:true,failures:[],patchReadinessQualified:true,openObservationComplete:true,encodedPathQualified:true,postOpenObservationComplete:true,sourcePreserved:true,fixtureAfter:{},resumedReceipt:{previous_suspend_count:1,state:'running'},observationEndpoint:{reason:'postopen-return',elapsedMs:1000},intentionalTerminationRequested:true};
assert.equal(completionPrerequisites(clean,null,'',false),true);
for(const field of ['cleanupVerified','processAbsent','helperStdoutClosed','sessionDetached','postOpenObservationComplete'])assert.equal(completionPrerequisites({...clean,[field]:false},null,'',false),false);
assert.equal(completionPrerequisites({...clean,cleanupReceipt:{termination_attempts:[{source:'watchdog_deadline'}]}},null,'',false),false);
assert.equal(completionPrerequisites(clean,null,'partial',false),false);
assert.equal(completionPrerequisites(clean,null,'',true),false);
console.log(JSON.stringify({outcome:'PASS',scope:'cp214_actual_generated_agent_and_host',originalExecuted:false,guiControlAcquired:false}));
