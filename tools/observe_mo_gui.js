'use strict';
const now=()=>new Date().toISOString();
send({kind:'probe_ready',utc:now(),pid:Process.id,arch:Process.arch});
Process.setExceptionHandler(d=>{
 if(d.type==='system')return false; // Do not flood the receipt with handled C++ exceptions.
 send({kind:'native_exception',utc:now(),pid:Process.id,type:d.type,address:d.address.toString(),pc:d.context.pc.toString(),sp:d.context.sp.toString(),backtrace:Thread.backtrace(d.context,Backtracer.ACCURATE).map(a=>({address:a.toString(),symbol:DebugSymbol.fromAddress(a).toString()}))});
 return false;
});
for(const suffix of ['W','A']){
 try {
  const address=Process.getModuleByName('KernelBase.dll').getExportByName('CreateProcess'+suffix);
  Interceptor.attach(address,{
   onEnter(args){
    const read=p=>p.isNull()?null:(suffix==='W'?p.readUtf16String():p.readCString());
    const buffer=Memory.alloc(65536);const getCwd=new NativeFunction(Process.getModuleByName('KernelBase.dll').getExportByName('GetCurrentDirectoryW'),'uint32',['uint32','pointer']);
    const count=getCwd(32768,buffer);
    this.info=args[9]; this.record={kind:'child_launch',utc:now(),parent_pid:Process.id,application:read(args[0]),command_line:read(args[1]),working_directory:read(args[7]),inherited_working_directory:count>0&&count<32768?buffer.readUtf16String():null};
   },
   onLeave(ret){
    this.record.success=!ret.isNull();
    if(this.record.success&&!this.info.isNull())this.record.pid=this.info.add(Process.pointerSize*2).readU32();
    send(this.record);
   }
  });
 }catch(e){send({kind:'probe_limit',api:'CreateProcess'+suffix,error:String(e)});}
}
