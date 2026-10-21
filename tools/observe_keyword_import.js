// Passive observation of original descriptor and mesh-allocation state.
// The caller substitutes __READY_GATE__ with a fresh private harness gate.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0;
let currentFile = null;
const descriptors = new Map();
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
        values: readArray(begin, length, false)
    };
}
function object(p) {
    const owner = p.add(0x3a98).readPointer();
    const n = p.add(0x880).readS32(), e = p.add(0x884).readS32();
    const width = owner.add(0x15f8).readS32();
    const arity = p.add(0x894).readS32();
    const components = Array.from({length: 79}, (_, i) => owner.add(0x15f8 + i * 4).readS32());
    const direct = {};
    for (let id = 18; id <= 23; ++id)
        direct[id] = readArray(p.add(0x990 + (id - 18) * 8).readPointer(), n * components[id], true);
    direct[61] = readArray(p.add(0x9f8).readPointer(), e * components[61], true);
    const groupCount = p.add(0xbd8).readS32();
    if (groupCount < 0 || groupCount > 100) throw new Error('boundary group count');
    const boundaryGroups = [];
    for (let i = 0; i < groupCount; ++i) {
        const g = p.add(0xbe0 + i * 0x30);
        const count = g.add(0x10).readS32();
        boundaryGroups.push({faces: readArray(g.add(0x18).readPointer(), count * 4, true),
            owners: readArray(g.add(0x20).readPointer(), count, true)});
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
        components, direct, boundary_groups: boundaryGroups, boundary_total: p.add(0x88c).readS32(),
        coordinates: readArray(p.add(0x8a8).readPointer(), n * width, false),
        connectivity: readArray(p.add(0x9e8).readPointer(), e * arity, true),
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

function rootState(p) {
    const count = p.add(0x1378).readS32();
    if (count < 0 || count > 8) throw new Error('object count bound');
    const objects = [];
    let q = p.add(0x1380).readPointer();
    for (let i = 0; i < count; ++i) {
        if (q.isNull()) throw new Error('short object list');
        objects.push(object(q)); q = q.add(0x3940).readPointer();
    }
    return {dimension:p.add(0x1c).readS32(), suppress_boundaries:p.add(0x1b5c).readU8(),
        components:Array.from({length:79},(_,i)=>p.add(0x15f8+i*4).readS32()),
        object_count:count, objects};
}
function bytes(p,n) {
    if (n<0 || n>4096) throw new Error('byte bound');
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
Interceptor.attach(at(0x38b8a0), {
    onEnter(args) {this.root=args[0];this.id=args[2].toInt32();},
    onLeave(result) {if(currentFile!==null)safe(()=>emit('handler-after',{id:this.id,status:result.toInt32(),state:rootState(this.root)}));}
});
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
