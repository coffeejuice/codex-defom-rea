// Passive UNIT30 discovery; target data and native arguments are never changed.
const app=Process.getModuleByName('DEF_PRE_64.exe'),at=rva=>app.base.add(rva);
let file=null,root=null,active=null,sequence=0;
function emit(operation,fields){send({sequence:++sequence,operation,file,...fields});}
function safe(fn){try{fn();}catch(e){emit('probe-error',{error:String(e)});}}
function hex(p,n){return Array.from(new Uint8Array(p.readByteArray(n)),b=>b.toString(16).padStart(2,'0')).join('');}
function text100(p){const b=Array.from(new Uint8Array(p.readByteArray(100))),end=b.indexOf(0);if(end<0)throw new Error('unit label lacks bounded terminator');if(b.slice(0,end).some(c=>c>127))throw new Error('non-ASCII unit label');return String.fromCharCode(...b.slice(0,end));}
const tableOffsets=[0xb18,0xc10,0xc88,0xd00],scalarOffsets=[0xb00,0xc08,0xb20,0xc18,0xc90,0xd08,0xd78,0xd80];
function table(p){return {type:p.readS32(),rank:p.add(4).readS32(),constant_hex:hex(p.add(8),8),
    selectors:Array.from({length:3},(_,i)=>p.add(0x10+4*i).readS32()),counts:Array.from({length:3},(_,i)=>p.add(0x1c+4*i).readS32()),
    width:p.add(0x40).readS32(),flag50:p.add(0x50).readS32(),metadata54:p.add(0x54).readS32(),mode58:p.add(0x58).readS32(),metadata60_hex:hex(p.add(0x60),8),
    storage_null:[0x28,0x30,0x38,0x48,0x68].map(o=>p.add(o).readPointer().isNull())};}
function units(p){return {selector:p.add(0x84c).readS32(),scalar_hex:scalarOffsets.map(o=>hex(p.add(o),8)),tables:tableOffsets.map(o=>table(p.add(o)))};}
function globals(){const a=at(0x70ba80).readPointer(),b=at(0x70ba78).readPointer(),count=at(0x70ba88).readS32();
    if(count<0||count>28)throw new Error('unit display count outside28');
    return {mode:at(0x6fe778).readS32(),display_mode:at(0x70ba70).readS32(),count,limit:at(0x70ba8c).readS32(),field90:at(0x70ba90).readS32(),field94:at(0x70ba94).readS32(),
        table_a:a.isNull()?null:Array.from({length:count},(_,i)=>{const r=a.add(i*0x88);return {id:r.readS32(),text:text100(r.add(4)),raw64:[0x68,0x70,0x78,0x80].map(o=>hex(r.add(o),8))};}),
        table_b:b.isNull()?null:Array.from({length:count},(_,i)=>{const r=b.add(i*0x1f8);return {id:r.readS32(),text:[4,0x68,0xcc,0x130,0x194].map(o=>text100(r.add(o)))};})};}
function state(p){const link=p.add(0x33e8).readPointer();return {unit:units(p),linked_unit:link.isNull()?null:units(link),global_units:globals(),dimension:p.add(0x1c).readS32(),linked_dimension:link.isNull()?null:link.add(0x1c).readS32(),
    components:Array.from({length:79},(_,i)=>p.add(0x15f8+4*i).readS32()),object_count:p.add(0x1378).readS32(),material_count:p.add(0x1320).readS32(),
    mesh_number:p.add(0x840).readS32(),mode:p.add(0x844).readS32(),type:p.add(0x848).readS32()};}
function cursor(p){const c=p.readPointer(),b=p.add(0x10).readPointer();if(c.isNull()||b.isNull())return 0;const n=c.sub(b).toInt32();if(n<0||n>4096)throw new Error('stream bound');return n;}
Interceptor.attach(at(0x1b5f00),{onEnter(args){this.previous=file;file=args[1].readCString();root=ptr(args[0].toString());this.root=root;safe(()=>emit('import-before',{state:state(root)}));},onLeave(result){safe(()=>emit('import-after',{state:state(this.root),status:result.toInt32()}));file=this.previous;}});
Interceptor.attach(at(0x39aa90),{onEnter(args){this.enabled=file!==null&&args[2].toInt32()===30;if(!this.enabled)return;active={root:ptr(args[0].toString()),stream:ptr(args[1].toString()),calls:[],refreshes:[]};safe(()=>emit('handler-before',{state:state(active.root),cursor:cursor(active.stream)}));},onLeave(result){if(!this.enabled)return;safe(()=>emit('handler-after',{state:state(active.root),status:result.toInt32(),cursor:cursor(active.stream),calls:active.calls,refreshes:active.refreshes}));active=null;}});
Interceptor.attach(at(0x413de0),{onEnter(args){this.enabled=active!==null;if(!this.enabled)return;this.dest=ptr(args[1].toString());safe(()=>{if(this.dest.equals(active.root.add(0x84c)))throw new Error('UNIT expected temporary');active.calls.push({call:'reader',option:args[2].toInt32()});});},onLeave(result){if(this.enabled)safe(()=>{active.calls.at(-1).value=this.dest.readS32();active.calls.at(-1).status=result.toInt32();});}});
Interceptor.attach(at(0x1f4100),{onEnter(args){this.enabled=active!==null;if(!this.enabled)return;safe(()=>{if(!args[0].equals(active.root.add(0x750)))throw new Error('UNIT controls owner');active.calls.push({call:'reset',before:units(active.root)});});},onLeave(){if(this.enabled)safe(()=>active.calls.push({call:'reset-after',after:units(active.root)}));}});
Interceptor.attach(at(0x12d2a0),{onEnter(args){if(active)safe(()=>active.calls.push({call:'table-reset',offset:args[0].sub(active.root).toInt32(),type:args[1].toInt32()}));}});
for(const [rva,name]of [[0x235c30,'global-mode'],[0x3f8b70,'display-mode'],[0x3f9f20,'display-refresh']])Interceptor.attach(at(rva),{onEnter(args){if(active)safe(()=>active.calls.push({call:name,...(name==='display-refresh'?{}:{value:args[0].toInt32()})}));else if(file)safe(()=>emit('outside-unit-call',{call:name}));}});
Interceptor.attach(at(0x1d0fc0),{onEnter(args){if(active)active.refreshes.push(args[1].toInt32());}});
send({operation:'hooks-installed',pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
