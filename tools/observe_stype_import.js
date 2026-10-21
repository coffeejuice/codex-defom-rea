// Passive STYPE26 observation; no application calls or target-data writes.
const app = Process.getModuleByName('DEF_PRE_64.exe'), at = rva => app.base.add(rva);
let file = null, root = null, active = null, sequence = 0;
function emit(operation, fields) {send({sequence: ++sequence, operation, file, ...fields});}
function safe(fn) {try {fn();} catch(e) {emit('probe-error', {error:String(e)});}}
function mode(p) {return {mode_0x844:p.add(0x844).readS32(),flag_0x854:p.add(0x854).readS32(),flag_0x858:p.add(0x858).readS32()};}
function state(p) {
    const linked=p.add(0x33e8).readPointer();
    return {simulation_type_0x848:p.add(0x848).readS32(),simulation_mode:mode(p),dimension:p.add(0x1c).readS32(),
        linked_type:linked.isNull()?null:linked.add(0x848).readS32(),linked_mode:linked.isNull()?null:mode(linked),linked_dimension:linked.isNull()?null:linked.add(0x1c).readS32(),
        components:Array.from({length:79},(_,i)=>p.add(0x15f8+4*i).readS32()),
        object_count:p.add(0x1378).readS32(),material_count:p.add(0x1320).readS32()};
}
function cursor(p) {const current=p.readPointer(),base=p.add(0x10).readPointer();
    if(current.isNull()||base.isNull())return 0;
    const n=current.sub(base).toInt32();if(n<0||n>4096)throw new Error('stream outside bounded fixture');return n;}
Interceptor.attach(at(0x1b5f00),{onEnter(args){this.previous=file;file=args[1].readCString();root=ptr(args[0].toString());this.root=root;
    safe(()=>emit('import-before',{state:state(root)}));},onLeave(result){safe(()=>emit('import-after',{status:result.toInt32(),state:state(this.root)}));file=this.previous;}});
Interceptor.attach(at(0x39aa90),{onEnter(args){this.enabled=file!==null&&args[2].toInt32()===26;
    if(!this.enabled)return;active={root:ptr(args[0].toString()),stream:ptr(args[1].toString()),refreshes:[],calls:[]};
    safe(()=>emit('handler-before',{state:state(active.root),cursor:cursor(active.stream)}));},onLeave(result){
    if(!this.enabled)return;safe(()=>emit('handler-after',{status:result.toInt32(),state:state(active.root),cursor:cursor(active.stream),refreshes:active.refreshes,calls:active.calls}));active=null;}});
Interceptor.attach(at(0x413de0),{onEnter(args){this.enabled=active!==null;if(!this.enabled)return;
    this.destination=ptr(args[1].toString());safe(()=>{
        if(!this.destination.equals(active.root.add(0x848)))throw new Error('STYPE live destination mismatch');
        if(args[2].toInt32()!==0)throw new Error('STYPE reader option mismatch');
        active.calls.push({before:this.destination.readS32()});
    });},onLeave(result){if(this.enabled)safe(()=>{
        active.calls.at(-1).after=this.destination.readS32();active.calls.at(-1).status=result.toInt32();
    });}});
Interceptor.attach(at(0x1d0fc0),{onEnter(args){if(active)active.refreshes.push(args[1].toInt32());}});
send({operation:'hooks-installed',pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
