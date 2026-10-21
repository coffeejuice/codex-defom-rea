// Passive TRANS32 and dependent feature observation; no target calls or writes.
const app=Process.getModuleByName('DEF_PRE_64.exe'),at=rva=>app.base.add(rva);
let file=null,root=null,active=null,sequence=0;
function emit(operation,fields){send({sequence:++sequence,operation,file,...fields});}
function safe(fn){try{fn();}catch(e){emit('probe-error',{error:String(e)});}}
function controls(p){return {trans:[0x854,0x858,0x85c,0x860,0x864].map(o=>p.add(o).readS32()),mode:p.add(0x844).readS32(),field_0x898:p.add(0x898).readS32()};}
function state(p){const linked=p.add(0x33e8).readPointer();return {...controls(p),linked:linked.isNull()?null:controls(linked),dimension:p.add(0x1c).readS32(),linked_dimension:linked.isNull()?null:linked.add(0x1c).readS32(),components:Array.from({length:79},(_,i)=>p.add(0x15f8+4*i).readS32()),object_count:p.add(0x1378).readS32(),material_count:p.add(0x1320).readS32()};}
function cursor(p){const c=p.readPointer(),b=p.add(0x10).readPointer();if(c.isNull()||b.isNull())return 0;const n=c.sub(b).toInt32();if(n<0||n>4096)throw new Error('bounded stream position');return n;}
Interceptor.attach(at(0x1b5f00),{onEnter(args){this.previous=file;file=args[1].readCString();root=ptr(args[0].toString());this.root=root;safe(()=>emit('import-before',{state:state(root)}));},onLeave(result){safe(()=>emit('import-after',{state:state(this.root),status:result.toInt32()}));file=this.previous;if(file===null)root=null;}});
Interceptor.attach(at(0x39aa90),{onEnter(args){this.enabled=file!==null&&args[2].toInt32()===32;if(!this.enabled)return;active={root:ptr(args[0].toString()),stream:ptr(args[1].toString()),calls:[],refreshes:[]};safe(()=>emit('handler-before',{state:state(active.root),cursor:cursor(active.stream)}));},onLeave(result){if(!this.enabled)return;safe(()=>emit('handler-after',{state:state(active.root),status:result.toInt32(),cursor:cursor(active.stream),calls:active.calls,refreshes:active.refreshes}));active=null;}});
Interceptor.attach(at(0x414200),{onEnter(args){this.enabled=active!==null;if(!this.enabled)return;safe(()=>{
 if(!args[0].equals(active.stream))throw new Error('TRANS reader FILE mismatch');
 this.outputs=Array.from({length:5},(_,i)=>ptr(args[i+1].toString()));
 this.skip=args[6].toInt32()&255;
 if(this.skip!==0)throw new Error('TRANS skip expected0');
 for(let i=0;i<5;++i)if(!this.outputs[i].equals(this.outputs[0].add(i*4)))throw new Error('TRANS temporary ABI mismatch');
});},onLeave(result){if(this.enabled)safe(()=>active.calls.push({status:result.toInt32(),skip:this.skip,values:this.outputs.map(p=>p.readS32())}));}});
Interceptor.attach(at(0x1d0fc0),{onEnter(args){if(active)active.refreshes.push(args[1].toInt32());}});
Interceptor.attach(at(0x1d64f0),{onEnter(args){this.enabled=root!==null&&args[0].equals(root)&&[29,49,53].includes(args[1].toInt32());if(!this.enabled)return;this.kind=args[1].toInt32();this.output=ptr(args[2].toString());},onLeave(result){if(this.enabled)safe(()=>emit('feature-query',{kind:this.kind,enabled:(result.toInt32()&255)!==0,width:this.output.isNull()?null:this.output.readS32(),state:state(root)}));}});
send({operation:'hooks-installed',pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
