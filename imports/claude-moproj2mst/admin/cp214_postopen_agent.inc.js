  let postFrame = null, postSerial = 0, postStateInvalid = false;
  const postRejected = {caller:0, helperCaller:0, overlap:0, order:0, identity:0, frame:0};
  const postHooks = hooks.slice(6);
  function postSnapshot(nextKind) {return {postStateInvalid,postSerial,postRejected:{...postRejected},boundedStateInvalid,sequence:sequence+1,
    counts:{...counts,[nextKind]:counts[nextKind]+1},overflow:{...overflow},rejected:{...rejected}};}
  Interceptor.attach(app.base.add(postHooks[0].rva), {
    onEnter() {
      this.post = null;
      try {
        // Poison the selected frame before caller filtering on any nested postOpen.
        if(postFrame) {postFrame.invalid=true;postStateInvalid=true;postRejected.overlap++;}
        if(!this.returnAddress.equals(app.base.add(postHooks[0].callerRva))) {postRejected.caller++;return;}
        if(++postSerial!==1 || !lastOuter || invocationSerial!==1 || activeByTid.size!==0 || postFrame || lastOuter.invalid || ptr(lastOuter.rawReturn).isNull() || Process.getCurrentThreadId()!==lastOuter.tid || !this.context.rcx.equals(ptr(lastOuter.main)) || !this.context.rdx.equals(ptr(lastOuter.rawReturn))) {postStateInvalid=true;postRejected.identity++;send({kind:'hook-error',site:'postopen-identity',pid:Process.id,error:'duplicate or mismatched postOpen'});return;}
        postFrame={invocationId:lastOuter.invocationId,postId:String(postSerial),tid:Process.getCurrentThreadId(),main:text(this.context.rcx),project:text(this.context.rdx),entryRsp:text(this.context.rsp),returnAddress:text(this.returnAddress),next:0,pending:null,invalid:false};
        this.post=postFrame;emit('postopen-enter',{...postFrame,outerSequence:lastOuter.outerSequence});
      }catch(error){send({kind:'hook-error',site:'postopen-enter',pid:Process.id,error:String(error)});}
    },
    onLeave() {
      try {const frame=this.post;if(!frame)return;
        if(postFrame!==frame || frame.pending) {frame.invalid=true;postRejected.frame++;}
        emit('postopen-return',{...frame,normalReturn:true,voidReturn:true,hookSnapshot:postSnapshot('postopen-return')});
        if(postFrame===frame)postFrame=null;
      }catch(error){send({kind:'hook-error',site:'postopen-return',pid:Process.id,error:String(error)});}
    }
  });
  for(const hook of postHooks.slice(1)) Interceptor.attach(app.base.add(hook.rva), {
    onEnter() {
      this.postCall=null;
      try {
        if(postFrame?.pending===hook.name) {postFrame.invalid=true;postStateInvalid=true;postRejected.overlap++;}
        if(!this.returnAddress.equals(app.base.add(hook.callerRva))) {postRejected.helperCaller++;return;}
        const f=postFrame;
        if(!f || f.invalid || f.pending || Process.getCurrentThreadId()!==f.tid || (hook.name==='current'?f.next!==0:f.next!==1) || !this.context.rsp.equals(ptr(f.entryRsp).sub(0x4e0))) {if(f)f.invalid=true;postStateInvalid=true;postRejected.order++;return;}
        f.pending=hook.name;
        this.postCall={frame:f,returnAddress:text(this.returnAddress),entryRsp:text(this.context.rsp),rawRCX:text(this.context.rcx),rawRDX:text(this.context.rdx),rawEDX:this.context.rdx.toUInt32()};
        emit(hook.name+'-enter',{invocationId:f.invocationId,postId:f.postId,...this.postCall,frame:undefined});
      }catch(error){send({kind:'hook-error',site:hook.name+'-enter',pid:Process.id,error:String(error)});}
    },
    onLeave(retval) {
      try {const call=this.postCall;if(!call)return;const f=call.frame;
        if(postFrame!==f || f.invalid || f.pending!==hook.name) {f.invalid=true;postStateInvalid=true;postRejected.frame++;return;}
        f.next++;f.pending=null;
        if(hook.name==='current')f.current=text(retval);
        emit(hook.name+'-return',{invocationId:f.invocationId,postId:f.postId,...call,frame:undefined,normalReturn:true,...(hook.name==='current'?{rawReturn:text(retval)}:{voidReturn:true})});
      }catch(error){send({kind:'hook-error',site:hook.name+'-return',pid:Process.id,error:String(error)});}
    }
  });
