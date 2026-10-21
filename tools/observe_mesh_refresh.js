// Passive preserving-resize evidence. No internal calls or process writes.
// Replace __READY_GATE__ with a fresh private harness gate.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0, currentFile = null, serial = 0;
const descriptors = new Map(), allocations = new Map(), active = [];
function emit(operation, fields = {}) {
    send({sequence: ++sequence, operation, file: currentFile, ...fields});
}
function safe(fn) {
    try { return fn(); }
    catch (error) { emit('probe-error', {error:String(error)}); return null; }
}
function bounded(count, maximum = 512) {
    if (!Number.isInteger(count) || count < 0 || count > maximum)
        throw new Error('extent bound: ' + count);
    return count;
}
function byteHex(p, length) {
    bounded(length, 4096);
    if (length === 0) return '';
    if (p.isNull()) throw new Error('null positive-size byte range');
    return Array.from(new Uint8Array(p.readByteArray(length)),
        value => value.toString(16).padStart(2,'0')).join('');
}
function readArray(p, count, integers) {
    bounded(count);
    if (p.isNull()) {
        if (count !== 0) throw new Error('null positive-size array');
        return [];
    }
    return Array.from({length:count}, (_,i) => {
        const value = integers ? p.add(i*4).readS32() : p.add(i*8).readDouble();
        // Raw bytes are retained separately; JSON cannot encode NaN/Infinity.
        return Number.isFinite(value) ? value : null;
    });
}
function table(p) { return readArray(p,79,true); }
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
        values: readArray(begin, length, false), values_hex: byteHex(begin, length * 8)
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

function bufferSlots(p, kind = null) {
    const slots = [];
    if (kind !== 'elements') {
        slots.push({name:'coordinates', slot:p.add(0x8a8), integers:false, component:0});
        for (let id=18;id<=23;++id)
            slots.push({name:'direct'+id, slot:p.add(0x990+(id-18)*8),
                integers:true, component:id});
    }
    if (kind !== 'nodes') {
        slots.push({name:'connectivity', slot:p.add(0x9e8), integers:true, component:null});
        slots.push({name:'direct61', slot:p.add(0x9f8), integers:true, component:61});
    }
    return slots;
}
function bufferState(entry) {
    const p = entry.slot.readPointer(), saved = allocations.get(entry.slot.toString());
    const base = {slot:entry.slot.toString(), pointer:p.toString(), integers:entry.integers};
    if (p.isNull()) return {...base, extent_source:'null-pointer', allocated_bytes:0,
        allocated_items:0, values:[], bytes_hex:''};
    if (!saved || saved.pointer !== p.toString())
        return {...base, extent_source:'unknown-not-read', allocated_bytes:null,
            allocated_items:null, values:null, bytes_hex:null};
    const size = entry.integers ? 4 : 8;
    bounded(saved.bytes / size);
    return {...base, extent_source:'completed-helper-request',
        allocation_serial:saved.serial, allocated_bytes:saved.bytes,
        allocated_items:saved.bytes/size, values:readArray(p,saved.bytes/size,entry.integers),
        bytes_hex:byteHex(p,saved.bytes)};
}
function buffers(p, kind = null) {
    const result = {};
    for (const entry of bufferSlots(p,kind)) result[entry.name] = bufferState(entry);
    return result;
}
function recordAllocations(c) {
    // 180150/183770 perform no resize for a nonpositive requested count.
    if (c.requested_count <= 0) return;
    for (const entry of bufferSlots(c.object,c.kind)) {
        const width = entry.component === null ? c.requested_arity
                                              : c.requested_components[entry.component];
        const items = bounded(c.requested_count * width);
        const p = entry.slot.readPointer();
        if (items > 0 && p.isNull()) throw new Error('allocation returned null');
        allocations.set(entry.slot.toString(), {
            pointer:p.toString(), bytes:items*(entry.integers?4:8), serial:c.serial});
    }
}
function preservation(before, after) {
    const result = {};
    for (const name of Object.keys(before)) {
        const a=before[name], b=after[name];
        if (a.allocated_bytes === null || b.allocated_bytes === null) {
            result[name] = {known_prefix_bytes:null, prefix_equal:null,
                qualification:'unknown allocation extent; no inferred read'};
            continue;
        }
        const count=Math.min(a.allocated_bytes,b.allocated_bytes);
        result[name] = {old_bytes:a.allocated_bytes,new_bytes:b.allocated_bytes,
            known_prefix_bytes:count,
            prefix_equal:a.bytes_hex.slice(0,count*2) === b.bytes_hex.slice(0,count*2),
            prefix_before_hex:a.bytes_hex.slice(0,count*2),
            prefix_after_hex:b.bytes_hex.slice(0,count*2),
            grown_tail_bytes:Math.max(0,b.allocated_bytes-a.allocated_bytes),
            grown_tail_initialization:'not-established',
            qualification:'Raw realloc prefix only; no per-row reshaping or tail zero assumption'};
    }
    return result;
}
function object(p) {
    const owner=p.add(0x3a98).readPointer(), dimension=owner.add(0x1c).readS32();
    const count=bounded(p.add(0xbd8).readS32(),100), groups=[];
    for (let i=0;i<count;++i) {
        const group=p.add(0xbe0+i*0x30);
        if (dimension===2) groups.push({
            nodes:readArray(group.add(8).readPointer(),group.readS32(),true),faces:[],owners:[]});
        else {
            const faces=bounded(group.add(0x10).readS32(),128);
            groups.push({nodes:[],faces:readArray(group.add(0x18).readPointer(),faces*4,true),
                owners:readArray(group.add(0x20).readPointer(),faces,true)});
        }
    }
    const vars=[];
    for (const entry of descriptors.values())
        if (entry.slot.compare(p.add(0x8b0))>=0 && entry.slot.compare(p.add(0xac0))<=0)
            vars.push(descriptor(entry.pointer,p));
    vars.sort((a,b)=>a.slot_offset-b.slot_offset);
    return {
        address:p.toString(),owner:owner.toString(),id:p.add(8).readS32(),
        type:p.add(12).readS32(),name:p.add(0x828).readCString(),dimension,
        nodes:p.add(0x880).readS32(),elements:p.add(0x884).readS32(),
        arity:p.add(0x894).readS32(),element_kind:p.add(0x890).readS32(),
        allocated_nodes:p.add(0x9cc).readS32(),
        allocated_elements:p.add(0xac8).readS32(),allocated_arity:p.add(0xacc).readS32(),
        mesh_flag:p.add(0x87c).readS32(),root_components:table(owner.add(0x15f8)),
        buffers:buffers(p),descriptors:vars,boundary_groups:groups,
        boundary_total:p.add(0x88c).readS32()
    };
}
function root(p) {
    const count=bounded(p.add(0x1378).readS32(),8), objects=[];
    let q=p.add(0x1380).readPointer();
    for(let i=0;i<count;++i) {
        if(q.isNull()) throw new Error('short object list');
        objects.push(object(q));q=q.add(0x3940).readPointer();
    }
    return {address:p.toString(),dimension:p.add(0x1c).readS32(),
        components:table(p.add(0x15f8)),object_count:count,objects};
}
function enterContext(c) {
    c.serial=++serial;c.parent_serial=active.length?active[active.length-1].serial:null;
    c.thread=Process.getCurrentThreadId();active.push(c);return c;
}
function leaveContext(c) {
    if(active.pop()!==c || c.thread!==Process.getCurrentThreadId())
        throw new Error('refresh capture nesting/thread mismatch');
}
for(const [rva,kind,offset] of [[0x180150,'nodes',0x8a0],[0x183770,'elements',0x9e0]]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            if(currentFile===null) return;
            this.capture=safe(()=>{
                const c=enterContext({kind,object:args[0].sub(offset),
                    requested_count:args[1].toInt32(),requested_components:table(args[2]),
                    requested_arity:kind==='elements'?args[3].toInt32():null});
                c.before_buffers=buffers(c.object,kind);
                emit('resize-enter',{serial:c.serial,parent_serial:c.parent_serial,kind,
                    container:args[0].toString(),requested_count:c.requested_count,
                    requested_arity:c.requested_arity,requested_components:c.requested_components,
                    object:object(c.object)});
                return c;
            });
        },
        onLeave() {
            if(!this.capture) return;
            safe(()=>{
                const c=this.capture;recordAllocations(c);
                const after=buffers(c.object,c.kind);
                emit('resize-leave',{serial:c.serial,parent_serial:c.parent_serial,kind,
                    object:object(c.object),buffers:after,
                    preservation:preservation(c.before_buffers,after)});
                leaveContext(c);
            });
        }
    });
}
Interceptor.attach(at(0x199650), {
    onEnter(args) {
        if(currentFile===null) return;
        this.capture=safe(()=>{
            const c=enterContext({object:args[0]});
            emit('mesh-refresh-enter',{serial:c.serial,parent_serial:c.parent_serial,
                requested_components:table(args[1]),prior_components_argument:table(args[2]),
                object:object(c.object)});
            return c;
        });
    },
    onLeave() {
        if(!this.capture) return;
        safe(()=>{
            const c=this.capture;
            emit('mesh-refresh-leave',{serial:c.serial,parent_serial:c.parent_serial,
                object:object(c.object)});
            leaveContext(c);
        });
    }
});
for(const [rva,kind] of [[0x1d0fc0,'all-objects'],[0x1d1660,'selected-object']]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            if(currentFile===null) return;
            this.capture=safe(()=>{
                const c=enterContext({root:args[0],kind,argument:args[1].toInt32()});
                emit('root-refresh-enter',{serial:c.serial,parent_serial:c.parent_serial,
                    kind,argument:c.argument,state:root(c.root)});
                return c;
            });
        },
        onLeave() {
            if(!this.capture) return;
            safe(()=>{
                const c=this.capture;
                emit('root-refresh-leave',{serial:c.serial,parent_serial:c.parent_serial,
                    kind,argument:c.argument,state:root(c.root)});
                leaveContext(c);
            });
        }
    });
}
Interceptor.attach(at(0x1d0320), {
    onEnter(args) {
        if(currentFile===null) return;
        this.capture=safe(()=>{
            const c=enterContext({root:args[0]});
            emit('layout-enter',{serial:c.serial,parent_serial:c.parent_serial,state:root(c.root)});
            return c;
        });
    },
    onLeave() {
        if(!this.capture) return;
        safe(()=>{
            const c=this.capture;
            emit('layout-leave',{serial:c.serial,parent_serial:c.parent_serial,state:root(c.root)});
            leaveContext(c);
        });
    }
});
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.root=args[0];this.previous=currentFile;currentFile=args[1].readCString();
        safe(()=>emit('import-before',{state:root(this.root)}));
    },
    onLeave(result) {
        safe(()=>emit('import-after',{status:result.toInt32(),state:root(this.root)}));
        currentFile=this.previous;
    }
});
Interceptor.attach(at(0x2f76c0), {
    onEnter(args) {
        this.enabled=currentFile!==null;
        if(this.enabled) {this.name=args[0].readCString();this.category=args[1];}
    },
    onLeave(result) {
        if(this.enabled) safe(()=>emit('keyword-resolved',
            {token:this.name,id:result.toInt32(),category:this.category.readS32()}));
    }
});
Interceptor.attach(at(0x1d7f40), {
    onEnter(args) {
        this.root=args[0];
        if(currentFile!==null) safe(()=>emit('finalize-before',{state:root(this.root)}));
    },
    onLeave() {
        if(currentFile!==null) safe(()=>emit('finalize-after',{state:root(this.root)}));
    }
});
send({operation:'hooks-installed',base:app.base.toString(),pid:Process.id,
    scope:'Passive preserving resize; grown raw tails have no established initialization'});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
