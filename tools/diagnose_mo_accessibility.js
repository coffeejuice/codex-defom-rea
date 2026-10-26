'use strict';
// Diagnostic only. Changes accessibility in this process; never an unmodified-GUI pass.
const keep=[];
Process.attachModuleObserver({onAdded(m){
 if(m.name.toLowerCase()!=='qtgui4_64.dll')return;
 const exports=m.enumerateExports().filter(e=>e.name.includes('queryAccessibleInterface@QAccessible'));
 if(exports.length!==1){send({kind:'guard_failed',module:m.name,count:exports.length});return;}
 const callback=new NativeCallback(()=>ptr(0),'pointer',['pointer']);keep.push(callback);
 Interceptor.replace(exports[0].address,callback);
 send({kind:'accessibility_guard',utc:new Date().toISOString(),module:m.name,path:m.path,base:m.base.toString(),symbol:exports[0].name,address:exports[0].address.toString(),effect:'QAccessible queries return null; diagnostic only'});
}});
