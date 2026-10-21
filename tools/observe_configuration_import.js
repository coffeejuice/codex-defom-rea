// Passive configuration import, with allocation-aware known-prefix snapshots.
// The caller substitutes __READY_GATE__ with a fresh private harness gate.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0;
let currentFile = null;
const descriptors = new Map();
const allocations = new Map();
let allocationSerial = 0;
function emit(operation, fields) {
    send({sequence: ++sequence, operation, file: currentFile, ...fields});
}
function safe(fn) {
    try { return fn(); }
    catch (error) { emit('probe-error', {error: String(error)}); return null; }
}
function readArray(p, count, integers) {
    if (count < 0 || count > 512) throw new Error('array bound: ' + count);
    if (p.isNull()) return null;
    return Array.from({length: count}, (_, i) =>
        integers ? p.add(i * 4).readS32() : p.add(i * 8).readDouble());
}
function descriptor(p, object) {
    const storage = p.add(0x28).readPointer();
    const begin = storage.add(8).readPointer();
    const end = storage.add(0x10).readPointer();
    const length = end.sub(begin).toInt32() / 8;
    const slot = p.add(0x30).readPointer();
    return {
        address: p.toString(), id: p.add(8).readS32(),
        vtable_rva: p.readPointer().sub(app.base).toInt32(),
        slot_offset: slot.sub(object).toInt32(),
        count: p.add(0x10).readS32(), width: p.add(0x14).readS32(),
        multiplicity: p.add(0x18).readS32(),
        old_count: p.add(0x1c).readS32(), old_width: p.add(0x20).readS32(),
        old_multiplicity: p.add(0x24).readS32(),
        storage_references: storage.readU32(), length,
        storage_pointer: begin.toString(), bound_pointer: slot.readPointer().toString(),
        values: readArray(begin, length, false), values_hex:bytes(begin,length*8)
    };
}
function boundedItems(value) {
    if(!Number.isInteger(value) || value<0 || value>512)
        throw new Error('buffer item bound: '+value);
    return value;
}
function slots(p,kind=null) {
    const result=[];
    if(kind!=='elements') {
        result.push({name:'coordinates',slot:p.add(0x8a8),component:0,integers:false});
        for(let id=18;id<=23;++id)
            result.push({name:'direct'+id,slot:p.add(0x990+(id-18)*8),component:id,integers:true});
    }
    if(kind!=='nodes') {
        result.push({name:'connectivity',slot:p.add(0x9e8),component:null,integers:true});
        result.push({name:'direct61',slot:p.add(0x9f8),component:61,integers:true});
    }
    return result;
}
function bufferState(entry) {
    const pointer=entry.slot.readPointer(),saved=allocations.get(entry.slot.toString());
    const base={slot:entry.slot.toString(),pointer:pointer.toString(),integers:entry.integers};
    if(pointer.isNull()) return {...base,allocated_items:0,known_prefix:0,unknown_items:0,
        extent_source:'null-pointer',known_hex:'',values:[]};
    if(!saved || saved.pointer!==pointer.toString())
        return {...base,allocated_items:null,known_prefix:null,unknown_items:null,
            extent_source:'unknown-not-read',known_hex:null,values:null};
    const count=boundedItems(saved.items),known=boundedItems(saved.known);
    if(known>count) throw new Error('known prefix beyond allocation');
    const values=readArray(pointer,known,entry.integers).map(value=>
        Number.isFinite(value)?value:null);
    return {...base,allocated_items:count,known_prefix:known,unknown_items:count-known,
        allocation_serial:saved.serial,extent_source:saved.source,
        known_hex:bytes(pointer,known*(entry.integers?4:8)),
        values:values.concat(Array(count-known).fill(null))};
}
function bufferSnapshot(p,kind=null) {
    const result={};
    for(const entry of slots(p,kind)) result[entry.name]=bufferState(entry);
    return result;
}
// Initial allocators use calloc (257150 doubles;256dc0 integers). Their
// complete extents are initialized; preserving realloc adds no known tail.
for(const [rva,kind,offset,mode] of [
    [0x17ffe0,'nodes',0x8a0,'replacement'],[0x1836c0,'elements',0x9e0,'replacement'],
    [0x180150,'nodes',0x8a0,'refresh'],[0x183770,'elements',0x9e0,'refresh']]) {
    Interceptor.attach(at(rva),{
        onEnter(args) {
            this.capture=safe(()=>{
                const object=args[0].sub(offset),prior={};
                for(const entry of slots(object,kind)) {
                    const pointer=entry.slot.readPointer();
                    const saved=allocations.get(entry.slot.toString());
                    prior[entry.name]={pointer:pointer.toString(),
                        known:pointer.isNull()?0:(saved && saved.pointer===pointer.toString()
                            ?saved.known:null)};
                }
                return {serial:++allocationSerial,object,kind,mode,
                    count:args[1].toInt32(),components:readArray(args[2],79,true),
                    arity:kind==='elements'?args[3].toInt32():null,
                    selector:mode==='replacement'?args[kind==='nodes'?3:4].toInt32():null,
                    coordinates_were_null:kind==='nodes'?args[0].add(8).readPointer().isNull():null,
                    prior};
            });
        },
        onLeave() {
            if(!this.capture) return;
            safe(()=>{
                const c=this.capture,changed=[];
                if(c.count>0) {
                    for(const entry of slots(c.object,c.kind)) {
                        const selected=c.mode==='refresh' || (entry.name==='coordinates'
                            ?c.coordinates_were_null && [0,-1,-2].includes(c.selector)
                            :[-1,-2].includes(c.selector));
                        if(!selected) continue;
                        const width=entry.component===null?c.arity:c.components[entry.component];
                        const items=boundedItems(c.count*width),pointer=entry.slot.readPointer();
                        if(items>0 && pointer.isNull()) throw new Error('allocation returned null');
                        const oldKnown=c.prior[entry.name].known;
                        allocations.set(entry.slot.toString(),{
                            pointer:pointer.toString(),items,
                            known:c.mode==='replacement'?items:Math.min(oldKnown===null?0:oldKnown,items),
                            serial:c.serial,source:c.mode==='replacement'
                                ?'observed-calloc-allocator':'observed-preserving-realloc'});
                        changed.push(entry.name);
                    }
                }
                if(currentFile!==null) emit('allocation-update',{
                    serial:c.serial,object:c.object.toString(),kind:c.kind,mode:c.mode,
                    requested_count:c.count,requested_arity:c.arity,selector:c.selector,
                    changed_buffers:changed,metadata:bufferSnapshot(c.object,c.kind)});
            });
        }
    });
}
// Free paths invalidate slot knowledge even outside an import, preventing
// reused object addresses from inheriting a previous object's allocation.
for(const [rva,kind,offset] of [[0x17f130,'nodes',0x8a0],[0x182a50,'elements',0x9e0]]) {
    Interceptor.attach(at(rva),{
        onEnter(args) {
            this.object=args[0].sub(offset);
            for(const entry of slots(this.object,kind)) allocations.delete(entry.slot.toString());
        },
        onLeave() {
            if(currentFile!==null) safe(()=>emit('allocation-free',{
                object:this.object.toString(),kind,metadata:bufferSnapshot(this.object,kind)}));
        }
    });
}
function object(p) {
    const owner = p.add(0x3a98).readPointer();
    const n = p.add(0x880).readS32(), e = p.add(0x884).readS32();
    const dimension = owner.add(0x1c).readS32();
    const arity = p.add(0x894).readS32();
    const components = Array.from({length: 79}, (_, i) => owner.add(0x15f8 + i * 4).readS32());
    const buffer_metadata = bufferSnapshot(p);
    const direct = {}, initialized_prefixes = {};
    for (const [name, value] of Object.entries(buffer_metadata)) {
        const key = name.startsWith('direct') ? name.slice(6) : name;
        initialized_prefixes[key] = value.known_prefix;
        if (name.startsWith('direct')) direct[key] = value.values;
    }
    const groupCount = p.add(0xbd8).readS32();
    if (groupCount < 0 || groupCount > 100) throw new Error('boundary group count');
    const boundaryGroups = [];
    for (let i = 0; i < groupCount; ++i) {
        const g = p.add(0xbe0 + i * 0x30);
        if (dimension === 2) {
            const count = g.readS32();
            boundaryGroups.push({nodes: readArray(g.add(8).readPointer(), count, true),
                faces: [], owners: []});
        } else {
            const count = g.add(0x10).readS32();
            boundaryGroups.push({nodes: [], faces: readArray(g.add(0x18).readPointer(), count * 4, true),
                owners: readArray(g.add(0x20).readPointer(), count, true)});
        }
    }
    const vars = [];
    for (const entry of descriptors.values()) {
        if (entry.slot.compare(p.add(0x8b0)) >= 0 &&
            entry.slot.compare(p.add(0xac0)) <= 0) {
            vars.push(descriptor(entry.pointer, p));
        }
    }
    vars.sort((a, b) => a.slot_offset - b.slot_offset);
    return {
        address: p.toString(), id: p.add(8).readS32(), type: p.add(12).readS32(), secondary: p.add(0x2154).readS32(), element_kind: p.add(0x890).readS32(), name: p.add(0x828).readCString(),
        dimension: owner.add(0x1c).readS32(),
        mesh_flag: p.add(0x87c).readS32(), nodes: n, elements: e, arity,
        node_allocation_count:p.add(0x9cc).readS32(),
        element_allocation_count:p.add(0xac8).readS32(),allocation_arity:p.add(0xacc).readS32(),
        initialized_prefixes,buffer_metadata,
        components, direct, boundary_groups: boundaryGroups, boundary_total: p.add(0x88c).readS32(),
        coordinates:buffer_metadata.coordinates.values,
        connectivity:buffer_metadata.connectivity.values,
        descriptors: vars
    };
}
Interceptor.attach(at(0x300c90), {
    onEnter(args) { this.pointer = args[0]; this.slot = args[2]; },
    onLeave() {
        if (!this.slot.isNull()) descriptors.set(this.pointer.toString(),
            {pointer: this.pointer, slot: this.slot});
    }
});
// Remove entries before a descriptor's deleting destructor invalidates it.
for (const rva of [0x32ab10, 0x32ab50, 0x32cfb0, 0x32cff0,
    0x32d030, 0x32d070, 0x32d0b0, 0x32d0f0]) {
    Interceptor.attach(at(rva), {
        onEnter(args) { descriptors.delete(args[0].toString()); }
    });
}

function rootState(p, includeLinked = true) {
    const count = p.add(0x1378).readS32();
    if (count < 0 || count > 8) throw new Error('object count bound');
    const objects = [];
    let q = p.add(0x1380).readPointer();
    for (let i = 0; i < count; ++i) {
        if (q.isNull()) throw new Error('short object list');
        objects.push(object(q)); q = q.add(0x3940).readPointer();
    }
    const linked = p.add(0x33e8).readPointer();
    return {address:p.toString(), dimension:p.add(0x1c).readS32(),
        alternate_enabled:p.add(0x33f0).readU8(),
        geometry_mode:p.add(0x850).readS32(),
        output_flags:Array.from({length:7},(_,i)=>p.add(0x86c+i*4).readS32()),
        process_units_raw:at(0x6fe778).readS32(),
        node8_default:at(0x6fe778).readS32() === 0 ? 20 : 68,
        linked_state:includeLinked && !linked.isNull() ? rootState(linked, false) : null,
        suppress_boundaries:p.add(0x1b5c).readU8(),
        components:Array.from({length:79},(_,i)=>p.add(0x15f8+i*4).readS32()),
        object_count:count, objects};
}
function bytes(p,n) {
    if (n<0 || n>4096) throw new Error('byte bound');
    if(n===0) return '';
    if(p.isNull()) throw new Error('null positive byte range');
    return Array.from(new Uint8Array(p.readByteArray(n)),b=>b.toString(16).padStart(2,'0')).join('');
}
function stream(p) {
    const pointer=p.readPointer(), count=p.add(8).readS32(), base=p.add(0x10).readPointer();
    if (pointer.isNull() || base.isNull()) return {cursor:0,count,buffer_hex:null};
    const cursor=pointer.sub(base).toInt32();
    const length=cursor+Math.max(count,0);
    if (cursor<0 || cursor>4096 || count < -1 || length>4096) throw new Error('stream buffer bound');
    return {cursor,count,buffer_hex:bytes(base,length)};
}
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) { this.root=args[0]; this.previous=currentFile; currentFile=args[1].readCString(); safe(()=>emit('import-before',{state:rootState(this.root)})); },
    onLeave(result) { safe(()=>emit('import-after',{status:result.toInt32(),state:rootState(this.root)})); currentFile=this.previous; }
});
Interceptor.attach(at(0x484924), {
    onEnter(args) {
        this.enabled=currentFile!==null;
        if (!this.enabled) return;
        safe(()=>{this.file=args[0];this.format=args[1].readCString();this.destination=args[2];this.before=stream(this.file);});
    },
    onLeave(result) {
        if (!this.enabled || !this.before) return;
        safe(()=> {
            if (this.format!=='%39s' && this.format!=='%i') throw new Error('unexpected fscanf format '+this.format);
            emit('stream-scan',{format:this.format,conversions:result.toInt32(),before:this.before,after:stream(this.file),
                value:this.format==='%i'?this.destination.readS32():(result.toInt32()===1?this.destination.readCString():null)});
        });
    }
});
Interceptor.attach(at(0x2f76c0), {
    onEnter(args) { this.enabled=currentFile!==null; if(this.enabled){this.name=args[0].readCString();this.category=args[1];} },
    onLeave(result) { if(this.enabled) safe(()=>emit('keyword-resolved',{token:this.name,id:result.toInt32(),category:this.category.readS32()})); }
});
for(const [rva,category] of [[0x38b8a0,2],[0x39aa90,1]]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {this.root=args[0];this.id=args[2].toInt32();},
        onLeave(result) {if(currentFile!==null)safe(()=>emit('handler-after',
            {id:this.id,category,status:result.toInt32(),state:rootState(this.root)}));}
    });
}
Interceptor.attach(at(0x19ccc0), {
    onEnter(args) {this.id=args[1].toInt32();},
    onLeave(result) {if(currentFile!==null)emit('object-selected',{requested:this.id,found:!result.isNull()});}
});
Interceptor.attach(at(0x1d7f40), {
    onEnter(args) {this.root=args[0];if(currentFile!==null)safe(()=>emit('finalize-before',{state:rootState(this.root)}));},
    onLeave() {if(currentFile!==null)safe(()=>emit('finalize-after',{state:rootState(this.root)}));}
});
send({operation:'hooks-installed',base:app.base.toString(),pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
