function qualifyObservation(messages,identity,filetime,ready) {
  const events=messages.filter(m=>m.type==='send'&&m.payload?.kind==='hook').map(m=>m.payload);
  const outerReturn=events.find(e=>e.event==='outer-return');
  const prefix=messages.filter(m=>!(m.type==='send'&&m.payload?.kind==='hook') || m.payload.sequence<=outerReturn?.sequence);
  const result=qualifyOuterObservation(prefix,identity,filetime,ready);
  result.postOpenObservationComplete=false;result.postOpenReturned=false;result.currentProjectEqualsDelivered=false;result.activationAttempted=false;result.activationReturned=false;
  try {
    requireThat(result.openObservationComplete && result.encodedPathQualified && BigInt(result.rawOuterReturn)!==0n,'Qualified nonzero loader required');
    requireThat(events.every((e,i)=>e.sequence===i+1&&e.pid===identity.pid&&e.processCreationFiletime===filetime&&e.tid===identity.primary_tid),'Post event sequence/identity');
    const later=events.filter(e=>e.sequence>outerReturn.sequence),entry=later[0],returned=later.at(-1),base=BigInt(ready.module.base);
    // Entry-only evidence survives timeout, but remains separately qualified.
    const observedCurrent=later.find(e=>e.event==='current-return'),observedActivation=later.find(e=>e.event==='activation-enter');
    result.activationAttempted=entry?.event==='postopen-enter'&&entry.main===outerReturn.main&&entry.project===outerReturn.rawReturn&&observedCurrent?.normalReturn===true&&observedActivation?.postId===entry.postId&&observedActivation?.invocationId===entry.invocationId&&observedActivation?.rawRCX===observedCurrent?.rawReturn&&BigInt(observedActivation.returnAddress)===base+BigInt(POST_HOOKS[2].callerRva)&&BigInt(observedActivation.entryRsp)===BigInt(entry.entryRsp)-0x4e0n;
    requireThat(entry?.event==='postopen-enter'&&returned?.event==='postopen-return'&&later.filter(e=>e.event==='postopen-enter').length===1&&later.filter(e=>e.event==='postopen-return').length===1,'Unique postOpen pair required');
    requireThat(later.every(e=>e.invocationId===outerReturn.invocationId&&e.postId===entry.postId),'Post occurrence mismatch');
    requireThat(entry.main===outerReturn.main&&entry.project===outerReturn.rawReturn&&entry.outerSequence===outerReturn.sequence&&BigInt(entry.returnAddress)===base+BigInt(POST_HOOKS[0].callerRva),'Delivered main/P mismatch');
    requireThat(returned.main===entry.main&&returned.project===entry.project&&returned.entryRsp===entry.entryRsp&&returned.returnAddress===entry.returnAddress&&returned.normalReturn===true&&returned.voidReturn===true&&!returned.invalid&&!returned.pending,'PostOpen normal frame return required');
    const snapshot=returned.hookSnapshot;
    requireThat(snapshot?.postStateInvalid===false&&snapshot.postSerial===1&&snapshot.boundedStateInvalid===false&&snapshot.sequence===events.length&&Object.values(snapshot.postRejected).every(n=>Number.isInteger(n)&&n>=0)&&['overlap','order','identity','frame'].every(k=>snapshot.postRejected[k]===0),'Post attribution counters');
    requireThat(EVENT_KINDS.every(k=>snapshot.counts[k]===events.filter(e=>e.event===k).length&&snapshot.counts[k]<EVENT_CAP_PER_KIND&&snapshot.overflow[k]===0)&&events.length<=EVENT_CAP_TOTAL,'Post counter parity');
    requireThat(!messages.some(m=>m.type==='error'||['hook-error','exception-handler-error','install-readiness-failure'].includes(m.payload?.kind)),'Observer errors');
    const currentEnter=later[1],currentReturn=later[2];
    requireThat(currentEnter?.event==='current-enter'&&currentReturn?.event==='current-return','Current lookup entry/leave required');
    for(const e of [currentEnter,currentReturn]) requireThat(BigInt(e.returnAddress)===base+BigInt(POST_HOOKS[1].callerRva)&&BigInt(e.entryRsp)===BigInt(entry.entryRsp)-0x4e0n,'Current stack/caller mismatch');
    requireThat(currentReturn.normalReturn===true&&currentEnter.rawRCX===currentReturn.rawRCX&&currentEnter.entryRsp===currentReturn.entryRsp,'Current call frame mismatch');
    const zero=BigInt(currentReturn.rawReturn)===0n;
    result.currentProjectRaw=currentReturn.rawReturn;result.currentProjectEqualsDelivered=currentReturn.rawReturn===entry.project;
    if(zero) requireThat(later.length===4&&returned.next===1,'Null-current skip must omit activation');
    else {
      const a=later[3],r=later[4];result.activationAttempted=a?.event==='activation-enter';
      requireThat(later.length===6&&a?.event==='activation-enter'&&r?.event==='activation-return'&&returned.next===2,'Activation normal pair required');
      for(const e of [a,r]) requireThat(BigInt(e.returnAddress)===base+BigInt(POST_HOOKS[2].callerRva)&&BigInt(e.entryRsp)===BigInt(entry.entryRsp)-0x4e0n&&e.rawRCX===currentReturn.rawReturn&&e.rawEDX===Number(BigInt(e.rawRDX)&0xffffffffn),'Activation frame/raw index mismatch');
      requireThat(a.rawRDX===r.rawRDX&&r.normalReturn===true&&r.voidReturn===true,'Activation normal call return required');
      result.activationReturned=true;result.activationSubprojectZero=a.rawEDX===0;
    }
    result.postOpenReturned=true;result.postOpenObservationComplete=true;
    result.intendedProjectActivationComplete=result.currentProjectEqualsDelivered&&result.activationReturned&&result.activationSubprojectZero;
    result.postOpenRoute={observed:true,normalReturn:true,voidReturn:true,deliveredProject:entry.project};
  } catch(error) {result.postOpenQualificationError=String(error);}
  return result;
}
