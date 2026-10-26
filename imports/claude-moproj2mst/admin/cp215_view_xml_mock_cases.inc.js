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
