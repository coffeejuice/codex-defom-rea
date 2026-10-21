// Passive TNOW31 observer. No native calls, argument changes or target-data writes.
const app=Process.getModuleByName('DEF_PRE_64.exe'),at=rva=>app.base.add(rva);
let file=null,active=null,sequence=0;
function emit(operation,fields){send({sequence:++sequence,operation,file,...fields});}
function safe(fn){try{fn();}catch(e){emit('probe-error',{error:String(e)});}}
function hex(p){return Array.from(new Uint8Array(p.readByteArray(8)),b=>b.toString(16).padStart(2,'0')).join('');}
function tnow(p){return {field_0xd90_hex:hex(p.add(0xd90)),field_0xd98_hex:hex(p.add(0xd98)),field_0xda0:p.add(0xda0).readS32(),field_0xda8_hex:hex(p.add(0xda8))};}
function state(p){const linked=p.add(0x33e8).readPointer();return {tnow:tnow(p),linked_tnow:linked.isNull()?null:tnow(linked),dimension:p.add(0x1c).readS32(),linked_dimension:linked.isNull()?null:linked.add(0x1c).readS32(),components:Array.from({length:79},(_,i)=>p.add(0x15f8+4*i).readS32()),object_count:p.add(0x1378).readS32(),material_count:p.add(0x1320).readS32(),mode:[0x844,0x854,0x858].map(o=>p.add(o).readS32()),unit:p.add(0x84c).readS32(),global_unit_flags:[at(0x6fe778).readS32(),at(0x70ba70).readS32()]};}
function cursor(p){const c=p.readPointer(),b=p.add(0x10).readPointer();if(c.isNull()||b.isNull())return 0;const n=c.sub(b).toInt32();if(n<0||n>4096)throw new Error('bounded stream position');return n;}
Interceptor.attach(at(0x1b5f00),{onEnter(args){this.previous=file;file=args[1].readCString();this.root=ptr(args[0].toString());safe(()=>emit('import-before',{state:state(this.root)}));},onLeave(result){safe(()=>emit('import-after',{state:state(this.root),status:result.toInt32()}));file=this.previous;}});
Interceptor.attach(at(0x39aa90),{onEnter(args){this.enabled=file!==null&&args[2].toInt32()===31;if(!this.enabled)return;active={root:ptr(args[0].toString()),stream:ptr(args[1].toString()),calls:[],refreshes:[]};safe(()=>emit('handler-before',{state:state(active.root),cursor:cursor(active.stream)}));},onLeave(result){if(!this.enabled)return;safe(()=>emit('handler-after',{state:state(active.root),status:result.toInt32(),cursor:cursor(active.stream),calls:active.calls,refreshes:active.refreshes}));active=null;}});
Interceptor.attach(at(0x414e40),{onEnter(args){this.enabled=active!==null;if(!this.enabled)return;safe(()=>{
    if(!args[0].equals(active.stream))throw new Error('TNOW reader FILE mismatch');
    this.destinations=Array.from({length:4},(_,i)=>ptr(args[i+1].toString()));
    for(let i=0;i<4;++i)if(!this.destinations[i].equals(this.destinations[0].add(8*i)))throw new Error('four temporary output ABI mismatch');
    if(this.destinations.some(p=>[0xd90,0xd98,0xda0,0xda8].some(o=>p.equals(active.root.add(o)))))throw new Error('TNOW expected temporary outputs');
});},onLeave(result){if(this.enabled)safe(()=>active.calls.push({status:result.toInt32(),values_hex:this.destinations.map(p=>hex(p))}));}});
Interceptor.attach(at(0x1d0fc0),{onEnter(args){if(active)active.refreshes.push(args[1].toInt32());}});
send({operation:'hooks-installed',pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
