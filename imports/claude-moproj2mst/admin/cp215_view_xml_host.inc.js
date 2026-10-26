function qualifyObservation(messages,identity,filetime,ready){
  const raw=messages.filter(m=>m.type==='send'&&m.payload?.kind==='hook').map(m=>m.payload);
  const viewKinds=['viewbind-enter','viewcurrent-enter','viewcurrent-return','viewaccess-enter','viewaccess-return','modelbind-enter','modelbind-return','viewbind-return'];
  // Independent predecessor pass over a renumbered copy; raw full sequence/counters checked below.
  const kept=raw.filter(e=>!viewKinds.includes(e.event)),normalized=JSON.parse(JSON.stringify(kept));
  for(let i=0;i<normalized.length;i++){
    normalized[i].sequence=i+1;
    if(normalized[i].event==='postopen-return'&&normalized[i].hookSnapshot){normalized[i].hookSnapshot.sequence=i+1;for(const k of EVENT_KINDS)normalized[i].hookSnapshot.counts[k]=normalized.filter(e=>e.event===k).length;}
  }
  let n=0;const normalizedMessages=messages.filter(m=>!(m.type==='send'&&viewKinds.includes(m.payload?.event))).map(m=>m.type==='send'&&m.payload?.kind==='hook'?{...m,payload:normalized[n++]}:m);
  const out=qualifyPredecessorObservation(normalizedMessages,identity,filetime,ready);
  out.predecessorChainQualified=out.openObservationComplete===true&&out.postOpenObservationComplete===true&&out.intendedProjectActivationComplete===true;
  out.parserStateQualified=false;out.xmlReadStatus=null;out.viewObservationComplete=false;out.nonnullViewBindingQualified=false;
  try{
    requireThat(out.predecessorChainQualified,'CP214 predecessor chain required');
    requireThat(raw.length<=EVENT_CAP_TOTAL&&raw.every((e,i)=>e.sequence===i+1&&e.pid===identity.pid&&e.processCreationFiletime===filetime&&e.tid===identity.primary_tid),'Full raw sequence/identity');
    const outer=raw.find(e=>e.event==='outer-return'),accept=raw.find(e=>e.event==='acceptance-return'),p=raw.find(e=>e.event==='postopen-enter'),end=raw.find(e=>e.event==='postopen-return'),snap=end.hookSnapshot;
    requireThat(snap.sequence===raw.length&&snap.viewStateInvalid===false&&snap.viewSerial===1&&['overlap','identity','order','frame'].every(k=>snap.viewRejected[k]===0)&&EVENT_KINDS.every(k=>snap.counts[k]===raw.filter(e=>e.event===k).length&&snap.counts[k]<EVENT_CAP_PER_KIND&&snap.overflow[k]===0),'View counters/parity');
    const w=accept.parserWitness;
    requireThat(accept.lowByte===1&&w?.ok===true&&w.stable===true&&w.project===outer.rawReturn&&BigInt(w.address)===BigInt(w.project)+0x55n&&w.first===w.second&&w.rawByte===w.first&&[0,1].includes(w.rawByte),'Stable mapped parser byte required');
    out.parserStateQualified=true;out.xmlReadStatus=w.rawByte;out.parserStatusMeaning=w.rawByte===1?'ReadProjectFile true: QFile open, DOM parse and top-level WalkProject true; nested/main parsing not established':'ReadProjectFile false; failure stage not distinguished';
    const v=raw.filter(e=>viewKinds.includes(e.event)),entry=v[0],returned=v.at(-1),base=BigInt(ready.module.base);
    requireThat(entry?.event==='viewbind-enter'&&returned?.event==='viewbind-return'&&v.filter(e=>e.event==='viewbind-enter').length===1&&v.filter(e=>e.event==='viewbind-return').length===1,'Unique view frame required');
    requireThat(v.every(e=>e.invocationId===p.invocationId&&e.postId===p.postId&&e.viewId===entry.viewId)&&entry.main===p.main&&entry.project===p.project&&entry.rawDL===(Number(BigInt(entry.rawRDX)&255n))&&entry.rawDL===1&&entry.sequence>raw.find(e=>e.event==='activation-return').sequence&&returned.sequence<end.sequence,'View occurrence/true argument');
    requireThat(BigInt(entry.entryRsp)===BigInt(p.entryRsp)-0x4e0n&&BigInt(entry.returnAddress)===base+BigInt(VIEW_HOOKS[0].callerRva)&&returned.entryRsp===entry.entryRsp&&returned.returnAddress===entry.returnAddress&&returned.main===entry.main&&!returned.invalid&&!returned.pending&&returned.normalReturn===true&&returned.voidReturn===true,'View frame normal leave');
    const c=v[1],cr=v[2];requireThat(c?.event==='viewcurrent-enter'&&cr?.event==='viewcurrent-return'&&cr.normalReturn===true,'Fresh view current lookup required');
    for(const e of [c,cr])requireThat(BigInt(e.returnAddress)===base+0x383b1n&&BigInt(e.entryRsp)===BigInt(entry.entryRsp)-0xd0n,'View current caller/frame');
    requireThat(c.rawRCX===cr.rawRCX,'View current input frame');out.viewCurrentProjectRaw=cr.rawReturn;out.viewCurrentEqualsDelivered=cr.rawReturn===p.project;
    const a=v[3],ar=v[4],b=v[5],br=v[6];
    if(v.length===4){requireThat(returned.next===1,'Unadmitted view prefix');out.viewPath='returned_before_view_lookup';}
    else{
      requireThat(a?.event==='viewaccess-enter'&&ar?.event==='viewaccess-return'&&ar.normalReturn===true&&a.rawRCX===entry.main&&ar.rawRCX===entry.main,'View accessor normal pair');
      for(const e of [a,ar])requireThat(BigInt(e.returnAddress)===base+BigInt(VIEW_HOOKS[1].callerRva)&&BigInt(e.entryRsp)===BigInt(entry.entryRsp)-0xd0n,'View accessor caller/frame');
      out.viewRaw=ar.rawReturn;
      if(v.length===6){requireThat(returned.next===2,'No model binding prefix');out.viewPath='returned_before_final_binding';}
      else{
        requireThat(v.length===8&&b?.event==='modelbind-enter'&&br?.event==='modelbind-return'&&returned.next===3&&br.normalReturn===true&&br.voidReturn===true,'Final model binding normal pair');
        for(const e of [b,br])requireThat(BigInt(e.returnAddress)===base+BigInt(VIEW_HOOKS[2].callerRva)&&BigInt(e.entryRsp)===BigInt(entry.entryRsp)-0xd0n&&e.rawRCX===ar.rawReturn&&e.receiverSlot?.receiver===e.rawRCX&&e.receiverSlot.ok===true&&e.receiverSlot.stable===true&&BigInt(e.receiverSlot.slot)===BigInt(e.receiverSlot.vtable)+0x1b8n&&BigInt(e.receiverSlot.target)===base+BigInt(VIEW_HOOKS[2].rva),'Model receiver/caller/dynamic slot');
        requireThat(b.rawRDX===br.rawRDX,'Model argument changed between callbacks');
        out.modelRaw=b.rawRDX;out.viewPath='final_binding_returned';out.nonnullViewBindingQualified=out.viewCurrentEqualsDelivered&&BigInt(ar.rawReturn)!==0n&&BigInt(b.rawRDX)!==0n;
      }
    }
    out.viewObservationComplete=true;out.timerDeliveryObserved=false;out.parserCallDirectlyObserved=false;
  }catch(error){out.viewXmlQualificationError=String(error);}
  return out;
}
