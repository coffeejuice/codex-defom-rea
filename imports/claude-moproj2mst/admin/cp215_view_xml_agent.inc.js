  // Reads only at live normal-return/entry boundaries, never after child cleanup.
  function projectByte(project) {
    const out={project:text(project),address:null,ok:false,stable:false,rawByte:null,first:null,second:null};
    try {
      const address=project.add(0x55);out.address=text(address);
      if(project.isNull()||address.compare(project)<0)throw Error('invalid Project field address');
      out.first=new Uint8Array(readable(address,1))[0];out.second=new Uint8Array(readable(address,1))[0];
      out.stable=out.first===out.second;
      if(!out.stable)throw Error('Project status changed during read');
      out.rawByte=out.first;out.ok=true;
    }catch(error){out.error=String(error);}
    return out;
  }
  function receiverSlot(receiver) {
    const out={receiver:text(receiver),ok:false,stable:false,vtable:null,slot:null,target:null};
    try {
      if(receiver.isNull()||receiver.and(7).toUInt32()!==0)throw Error('invalid view receiver');
      const object=readable(receiver,8),vtable=ptr('0x'+new DataView(object).getBigUint64(0,true).toString(16));out.vtable=text(vtable);
      const slot=vtable.add(0x1b8);out.slot=text(slot);
      if(vtable.isNull()||vtable.and(7).toUInt32()!==0||slot.compare(vtable)<0)throw Error('invalid vtable');
      const target=readable(slot,8);out.target='0x'+new DataView(target).getBigUint64(0,true).toString(16);
      out.stable=hexBytes(object)===hexBytes(readable(receiver,8))&&hexBytes(target)===hexBytes(readable(slot,8));
      if(!out.stable)throw Error('view receiver slot changed during read');
      out.matchesConcreteTarget=ptr(out.target).equals(app.base.add(0x170430));out.ok=true;
    }catch(error){out.error=String(error);}
    return out;
  }
  let viewFrame=null,viewSerial=0,viewStateInvalid=false;
  const viewRejected={caller:0,helperCaller:0,overlap:0,identity:0,order:0,frame:0};
  const viewHooks=hooks.slice(9);
  function viewEvent(kind,call,extra={}){const f=viewFrame;emit(kind,{invocationId:f.invocationId,postId:f.postId,viewId:f.viewId,...call,frame:undefined,...extra});}
  function viewCurrentEnter(){
    this.viewCurrentCall=null;
    if(viewFrame?.pending==='viewcurrent'){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
    const f=viewFrame;
    if(!f||f.invalid||f.pending||f.next!==0||Process.getCurrentThreadId()!==f.tid||!this.context.rsp.equals(ptr(f.entryRsp).sub(0xd0))){if(f)f.invalid=true;viewStateInvalid=true;viewRejected.order++;return;}
    f.pending='viewcurrent';this.viewCurrentCall={frame:f,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),rawRCX:text(this.context.rcx)};
    viewEvent('viewcurrent-enter',this.viewCurrentCall);
  }
  function viewCurrentLeave(retval){
    const c=this.viewCurrentCall,f=c.frame;
    if(viewFrame!==f||f.invalid||f.pending!=='viewcurrent'){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;return;}
    f.pending=null;f.next=1;f.current=text(retval);viewEvent('viewcurrent-return',c,{normalReturn:true,rawReturn:text(retval)});
  }
  Interceptor.attach(app.base.add(viewHooks[0].rva),{
    onEnter(){this.view=null;try{
      if(viewFrame){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
      if(!this.returnAddress.equals(app.base.add(viewHooks[0].callerRva))){viewRejected.caller++;return;}
      const p=postFrame;
      if(++viewSerial!==1||viewFrame||!p||p.invalid||p.pending||p.next!==2||Process.getCurrentThreadId()!==p.tid||!this.context.rcx.equals(ptr(p.main))||!this.context.rsp.equals(ptr(p.entryRsp).sub(0x4e0))){viewStateInvalid=true;viewRejected.identity++;send({kind:'hook-error',site:'viewbind-identity',pid:Process.id,error:'duplicate/mismatched viewbind'});return;}
      viewFrame={invocationId:p.invocationId,postId:p.postId,viewId:String(viewSerial),tid:p.tid,main:text(this.context.rcx),project:p.project,rawRDX:text(this.context.rdx),rawDL:this.context.rdx.toUInt32()&255,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),next:0,pending:null,invalid:false};
      this.view=viewFrame;viewEvent('viewbind-enter',{...viewFrame});
    }catch(error){send({kind:'hook-error',site:'viewbind-enter',pid:Process.id,error:String(error)});}},
    onLeave(){try{const f=this.view;if(!f)return;if(viewFrame!==f||f.pending){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;}viewEvent('viewbind-return',{...f},{normalReturn:true,voidReturn:true});if(viewFrame===f)viewFrame=null;}catch(error){send({kind:'hook-error',site:'viewbind-return',pid:Process.id,error:String(error)});}}
  });
  for(const hook of viewHooks.slice(1))Interceptor.attach(app.base.add(hook.rva),{
    onEnter(){this.viewCall=null;try{
      if(viewFrame?.pending===hook.name){viewFrame.invalid=true;viewStateInvalid=true;viewRejected.overlap++;}
      if(!this.returnAddress.equals(app.base.add(hook.callerRva))){viewRejected.helperCaller++;return;}
      const f=viewFrame;
      if(!f||f.invalid||f.pending||Process.getCurrentThreadId()!==f.tid||(hook.name==='viewaccess'?f.next!==1:f.next!==2)||!this.context.rsp.equals(ptr(f.entryRsp).sub(0xd0))){if(f)f.invalid=true;viewStateInvalid=true;viewRejected.order++;return;}
      f.pending=hook.name;this.viewCall={frame:f,entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),rawRCX:text(this.context.rcx),rawRDX:text(this.context.rdx)};
      viewEvent(hook.name+'-enter',this.viewCall,hook.name==='modelbind'?{receiverSlot:receiverSlot(this.context.rcx)}:{});
    }catch(error){send({kind:'hook-error',site:hook.name+'-enter',pid:Process.id,error:String(error)});}},
    onLeave(retval){try{const c=this.viewCall;if(!c)return;const f=c.frame;
      if(viewFrame!==f||f.invalid||f.pending!==hook.name){f.invalid=true;viewStateInvalid=true;viewRejected.frame++;return;}
      f.next++;f.pending=null;if(hook.name==='viewaccess')f.view=text(retval);
      viewEvent(hook.name+'-return',c,{normalReturn:true,...(hook.name==='viewaccess'?{rawReturn:text(retval)}:{voidReturn:true,receiverSlot:receiverSlot(ptr(c.rawRCX))})});
    }catch(error){send({kind:'hook-error',site:hook.name+'-return',pid:Process.id,error:String(error)});}}
  });
