// Passive bounded alternate DIEGEO segment and matrix import observation.
// Derived from observe_geometry_import.js; original executable is never called by this probe.
// Geometry COW buffers use stored begin/end extents; classification extents come from observed allocation.
// No numeric-helper ABI assumptions or application-call injection.
// Passive import finalization, with allocation-aware full tracked-state snapshots.
// The caller substitutes __READY_GATE__ with a fresh private harness gate.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0;
let currentFile = null;
const descriptors = new Map();
const allocations = new Map();
let allocationSerial = 0;
let finalizerSerial = 0;
const finalizers = [];
let activeHandler=null;
const tableFrames=[];
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
        geometry:geometryState(p), geometry_segments:segmentState(p),
        stroke_0x22d0:readArray(p.add(0x22d0),3,false),stroke_hex:bytes(p.add(0x22d0),24),
        dimension: owner.add(0x1c).readS32(),flag_0x3990:p.add(0x3990).readU8(),
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

function finalizationControls(p) {
    return {
        control_unit_0x84c:p.add(0x84c).readS32(),selected_object_0x8b8:p.add(0x8b8).readS32(),pdie_alias_0x1100_hex:bytes(p.add(0x1100),8),
        step_number_0x8b0:p.add(0x8b0).readS32(),saved_step_0x744:p.add(0x744).readS32(),
        step_increment_0x930:p.add(0x930).readDouble(),
        time_step_0x920:p.add(0x920).readDouble(),
        time_step_secondary_0x928:p.add(0x928).readDouble(),
        control_0x93c:p.add(0x93c).readS32(),mode_0x990:p.add(0x990).readS32()
    };
}
function importFlags(p) {
    return {import_flag_0x2f88:p.add(0x2f88).readU8(),
        import_flag_0x2f89:p.add(0x2f89).readU8()};
}
function finalizationView(p) {
    return {root:p.toString(),finalization_controls:finalizationControls(p),time_table:timeTable(p.add(0x938)),
        time_controls_hex:timeControlsHex(p),
        ...importFlags(p),nstart_option_0x1740:p.add(0x1740).readS32(),
        material_count:p.add(0x1320).readS32()};
}
function finalizationInventory(p) {
    const count=p.add(0x1378).readS32(),objects=[];
    if(count<0 || count>8) throw new Error('finalization inventory bound');
    let q=p.add(0x1380).readPointer();
    for(let i=0;i<count;++i) {
        if(q.isNull()) throw new Error('short finalization object list');
        objects.push({address:q.toString(),id:q.add(8).readS32(),
            type:q.add(12).readS32(),flag_0x3990:q.add(0x3990).readU8()});
        q=q.add(0x3940).readPointer();
    }
    return objects;
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
    return {address:p.toString(), dimension:p.add(0x1c).readS32(),control_unit_0x84c:p.add(0x84c).readS32(),
        alternate_enabled:p.add(0x33f0).readU8(),
        geometry_mode:p.add(0x850).readS32(),
        finalization_controls:finalizationControls(p),time_table:timeTable(p.add(0x938)),
        time_controls_hex:timeControlsHex(p),
        ...importFlags(p),nstart_option_0x1740:p.add(0x1740).readS32(),
        material_count:p.add(0x1320).readS32(),
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
        safe(()=>{this.file=args[0];this.format=args[1].readCString();this.destination=args[2];this.before=stream(this.file);this.segment=segmentFrames.at(-1);
                if(this.format==='%Flf' && !this.segment) this.previousOutput=bytes(this.destination,8);});
    },
    onLeave(result) {
        if (!this.enabled || !this.before) return;
        safe(()=> {
            if (this.format!=='%39s' && this.format!=='%i' && this.format!=='%Flf') throw new Error('unexpected fscanf format '+this.format);
            if(this.segment) {
                const converted=result.toInt32()===1;
                emit('segment-scan',{reader_serial:this.segment.serial,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            emit(this.format==='%Flf'?'table-scan':'stream-scan',{format:this.format,conversions:result.toInt32(),before:this.before,after:stream(this.file),
                output_hex:this.format==='%Flf'?bytes(this.destination,8):null,
                previous_output_hex:this.format==='%Flf'?this.previousOutput:null,
                value:this.format==='%Flf'?this.destination.readDouble():(this.format==='%i'?this.destination.readS32():(result.toInt32()===1?this.destination.readCString():null))});
        });
    }
});
Interceptor.attach(at(0x2f76c0), {
    onEnter(args) { this.enabled=currentFile!==null; if(this.enabled){this.name=args[0].readCString();this.category=args[1];} },
    onLeave(result) { if(this.enabled) safe(()=>emit('keyword-resolved',{token:this.name,id:result.toInt32(),category:this.category.readS32()})); }
});
for(const [rva,category] of [[0x38b8a0,2],[0x39aa90,1]]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            this.root=args[0];this.id=args[2].toInt32();this.previousHandler=activeHandler;
            this.enabled=currentFile!==null;
            if(this.enabled){activeHandler={root:this.root,id:this.id,category,file:args[1]};
                safe(()=>emit('handler-before',{id:this.id,category,state:rootState(this.root),stream:stream(args[1])}));}
        },
        onLeave(result) {
            if(this.enabled) safe(()=>emit('handler-after',
                {id:this.id,category,status:result.toInt32(),state:rootState(this.root),
                    stream:stream(activeHandler.file)}));
            activeHandler=this.previousHandler;
        }
    });
}
Interceptor.attach(at(0x19ccc0), {
    onEnter(args) {this.id=args[1].toInt32();},
    onLeave(result) {if(currentFile!==null)emit('object-selected',{requested:this.id,found:!result.isNull()});}
});
Interceptor.attach(at(0x1d7f40), {
    onEnter(args) {
        if(currentFile===null) return;
        this.capture={root:args[0],serial:++finalizerSerial};
        finalizers.push(this.capture);
        safe(()=>emit('finalize-before',{finalizer_serial:this.capture.serial,
            state:rootState(this.capture.root)}));
    },
    onLeave() {
        if(!this.capture) return;
        safe(()=>{
            emit('finalize-after',{finalizer_serial:this.capture.serial,
                state:rootState(this.capture.root)});
            if(finalizers.pop()!==this.capture) throw new Error('finalizer nesting mismatch');
        });
    }
});
Interceptor.attach(at(0x1d7f10),{
    onEnter(args) {
        this.root=args[0];this.enabled=currentFile!==null;
        if(this.enabled) safe(()=>emit('flags-reset-before',finalizationView(this.root)));
    },
    onLeave() {
        if(this.enabled) safe(()=>emit('flags-reset-after',finalizationView(this.root)));
    }
});
for(const [rva,stage,offset] of [
    [0x1ee510,'control',0x750],[0x1d6250,'materials',0],[0x1d63c0,'objects',0]]) {
    Interceptor.attach(at(rva),{
        onEnter(args) {
            if(!finalizers.length) return;
            const parent=finalizers[finalizers.length-1],root=args[0].sub(offset);
            this.capture=safe(()=>{
                if(!root.equals(parent.root)) throw new Error('finalizer stage root mismatch');
                const c={root,parent:parent.serial,stage,
                    option:stage==='objects'?(args[1].toInt32()&255):null};
                emit('stage-before',{stage,finalizer_serial:c.parent,option:c.option,
                    ...finalizationView(root),
                    objects:stage==='objects'?finalizationInventory(root):null});
                return c;
            });
        },
        onLeave() {
            if(!this.capture) return;
            const c=this.capture;
            safe(()=>emit('stage-after',{stage,finalizer_serial:c.parent,option:c.option,
                ...finalizationView(c.root),
                objects:stage==='objects'?finalizationInventory(c.root):null}));
        }
    });
}
Interceptor.attach(at(0x2059a0),{
    onEnter(args) {
        if(!finalizers.length) return;
        const parent=finalizers[finalizers.length-1];
        this.capture=safe(()=>({
            object:args[0],parent:parent.serial,option:args[1].toInt32()&255,
            root:args[0].add(0x3a98).readPointer()
        }));
        if(this.capture) safe(()=>{
            const c=this.capture;
            if(!c.root.equals(parent.root)) throw new Error('object finalizer owner mismatch');
            emit('object-stage-before',{finalizer_serial:c.parent,option:c.option,
                root:c.root.toString(),object:c.object.toString(),id:c.object.add(8).readS32(),
                type:c.object.add(12).readS32(),flag_0x3990:c.object.add(0x3990).readU8(),geometry:geometryState(c.object)});
        });
    },
    onLeave() {
        if(!this.capture) return;
        const c=this.capture;
        safe(()=>emit('object-stage-after',{finalizer_serial:c.parent,option:c.option,
            root:c.root.toString(),object:c.object.toString(),id:c.object.add(8).readS32(),
            type:c.object.add(12).readS32(),flag_0x3990:c.object.add(0x3990).readU8(),geometry:geometryState(c.object)}));
    }
});

function timeControlsHex(p) {
    return {step_increment_0x930:bytes(p.add(0x930),8),
        time_step_0x920:bytes(p.add(0x920),8),
        time_step_secondary_0x928:bytes(p.add(0x928),8)};
}
const realStacks = new Map();
function realStack() {
    const id=Process.getCurrentThreadId();
    if(!realStacks.has(id)) realStacks.set(id,[]);
    return realStacks.get(id);
}
function boundedCStringHex(p,limit) {
    let n=0;
    while(n<limit && p.add(n).readU8()!==0) ++n;
    if(n===limit) throw new Error('unterminated bounded real-reader string');
    return bytes(p,n);
}
for(const [rva,kind,count] of [[0x414990,'real1',1],[0x414a40,'real2',2],[0x413ee0,'int2',2],[0x416aa0,'realint',2],[0x414c20,'real3',3]]) {
    Interceptor.attach(at(rva),{
        onEnter(args) {
            if(currentFile===null || (kind==='int2' && (!activeHandler || ![38,51].includes(activeHandler.id)))) return;
            this.entry=safe(()=>({kind,stream:args[0],before:stream(args[0]),
                pointers:Array.from({length:count},(_,i)=>args[i+1]),lines:[],scans:[]}));
            if(this.entry) realStack().push(this.entry);
        },
        onLeave(result) {
            if(!this.entry) return;
            safe(()=>{
                const e=this.entry;
                if(realStack().pop()!==e) throw new Error('real-reader nesting mismatch');
                emit(kind==='int2'?(activeHandler.id===38?'selection-row':'header-row'):(kind==='realint'?'mixed-row':'real-row'),{kind:e.kind,status:result.toInt32(),before:e.before,
                    after:stream(e.stream),lines:e.lines,scans:e.scans,
                    values:e.pointers.map((p,i)=>(kind==='int2' || (kind==='realint' && i===1))?p.readS32():p.readDouble()),output_hex:e.pointers.map((p,i)=>bytes(p,(kind==='int2' || (kind==='realint' && i===1))?4:8))});
            });
        }
    });
}
Interceptor.attach(at(0x47f898),{
    onEnter(args) {
        this.row=realStack().at(-1);this.table=tableFrames.at(-1);
        if(this.row || this.table){this.buffer=args[0];this.limit=args[1].toInt32();this.file=args[2];this.before=safe(()=>stream(this.file));}
    },
    onLeave(result) {
        if(this.row) safe(()=>this.row.lines.push({limit:this.limit,eof:result.isNull(),
            hex:result.isNull()?null:boundedCStringHex(this.buffer,this.limit)}));
        else if(this.table) safe(()=>emit('table-discard',{limit:this.limit,eof:result.isNull(),
            hex:result.isNull()?null:boundedCStringHex(this.buffer,this.limit),
            before:this.before,after:stream(this.file)}));
    }
});
Interceptor.attach(at(0x47c354),{
    onEnter(args) {
        this.row=realStack().at(-1);
        if(this.row) this.scan=safe(()=>({input_hex:boundedCStringHex(args[0],1024),
            format:args[1].readCString()}));
    },
    onLeave(result) {
        if(this.row && this.scan){this.scan.conversions=result.toInt32();this.row.scans.push(this.scan);}
    }
});


function timeTable(t) {
    const rank=t.add(4).readS32(),counts=Array.from({length:3},(_,i)=>t.add(0x1c+i*4).readS32());
    const width=t.add(0x40).readS32();
    if(rank<0 || rank>1 || width<0 || width>4 || counts.some(n=>n< -4 || n>16))
        throw new Error('time table outside bounded rank/count/width scope');
    const pointers=Array.from({length:3},(_,i)=>t.add(0x28+i*8).readPointer());
    const dependent=t.add(0x48).readPointer();
    const total=rank===0?(width<2?0:width):Math.max(0,counts[0]*width);
    const axes=pointers.map((p,i)=>p.isNull()?[]:readArray(p,Math.max(0,counts[i]),false));
    return {address:t.toString(),type_0x00:t.readS32(),rank_0x04:rank,
        constant_0x08:t.add(8).readDouble(),selectors_0x10:Array.from({length:3},(_,i)=>t.add(0x10+i*4).readS32()),
        counts_0x1c:counts,values_per_point_0x40:width,flag_0x50:t.add(0x50).readS32(),
        preserved_0x54:t.add(0x54).readS32(),mode_0x58:t.add(0x58).readS32(),
        preserved_0x60_hex:bytes(t.add(0x60),8),axes,
        dependent:dependent.isNull()?[]:readArray(dependent,total,false),
        raw_hex:{constant:bytes(t.add(8),8),axes:pointers.map((p,i)=>p.isNull()?'':bytes(p,Math.max(0,counts[i])*8)),
            dependent:dependent.isNull()?'':bytes(dependent,total*8)},
        pointers:{axes:pointers.map(p=>p.toString()),dependent:dependent.toString()}};
}
Interceptor.attach(at(0x41a800),{
    onEnter(args){
        if(currentFile===null) return;
        this.frame={file:args[0],table:args[1]};tableFrames.push(this.frame);
        safe(()=>emit('table-reader-before',{table:timeTable(args[1]),stream:stream(args[0])}));
    },
    onLeave(result){
        if(!this.frame)return;
        safe(()=>{
            emit('table-reader-after',{status:result.toInt32(),table:timeTable(this.frame.table),stream:stream(this.frame.file)});
            if(tableFrames.pop()!==this.frame)throw new Error('table reader nesting mismatch');
        });
    }
});
Interceptor.attach(at(0x12fc70),{
    onEnter(args){
        this.enabled=currentFile!==null && activeHandler && activeHandler.id===51;
        if(this.enabled){this.table=args[0];safe(()=>emit('table-reset-before',{type:args[1].toInt32(),table:timeTable(this.table)}));}
    },
    onLeave(){if(this.enabled)safe(()=>emit('table-reset-after',{table:timeTable(this.table)}));}
});


Interceptor.attach(at(0x1d6ea0),{
    onEnter(args){
        this.enabled=currentFile!==null && activeHandler && activeHandler.id===49;
        if(this.enabled){this.root=args[0];this.requested=args[1].toInt32();}
    },
    onLeave(result){
        if(!this.enabled)return;
        safe(()=>emit('step-object-lookup',{root:this.root.toString(),requested:this.requested,
            found:!result.isNull(),object:result.toString(),id:result.isNull()?null:result.add(8).readS32(),
            type:result.isNull()?null:result.add(12).readS32(),
            stroke_hex:result.isNull()?null:bytes(result.add(0x22d0),24)}));
    }
});


const geometryClassification = new Map();
const geometryFinalizerFrames = [];
function geometryVector(slot,integers) {
    const owner=slot.readPointer();
    if(owner.isNull())throw new Error('null geometry COW owner');
    const begin=owner.add(8).readPointer(),end=owner.add(0x10).readPointer(),capacityEnd=owner.add(0x18).readPointer();
    const size=integers?4:8,lengthBytes=end.sub(begin).toInt32(),capacityBytes=capacityEnd.sub(begin).toInt32();
    if(lengthBytes<0 || lengthBytes%size!==0 || lengthBytes>2048 || capacityBytes<lengthBytes || capacityBytes>8192)
        throw new Error('geometry vector bound');
    return {owner:owner.toString(),references:owner.readU32(),begin:begin.toString(),end:end.toString(),
        capacity_end:capacityEnd.toString(),length:lengthBytes/size,capacity:capacityBytes/size,
        values:readArray(begin,lengthBytes/size,integers)||[],raw_hex:bytes(begin,lengthBytes)};
}
function planeRecord(p) {
    return {address:p.toString(),id:p.readS32(),point:readArray(p.add(8),3,false),
        normal:readArray(p.add(0x20),3,false),point_hex:bytes(p.add(8),24),normal_hex:bytes(p.add(0x20),24),
        next:p.add(0x38).readPointer().toString()};
}
function geometryData(g) {
    const coordinates=geometryVector(g.add(0x10),false),faces=geometryVector(g.add(0x18),true);
    if(coordinates.length%3!==0 || faces.length%4!==0)throw new Error('geometry vector divisibility');
    const faceCount=faces.length/4,classification=g.add(0x20).readPointer();
    const observed=geometryClassification.get(g.toString());
    if(!classification.isNull() && faceCount>0 && (!observed || observed.pointer!==classification.toString() || observed.count!==faceCount))
        throw new Error('unproven geometry classification extent');
    const list=g.add(0x28),head=list.readPointer(),tail=list.add(8).readPointer(),count=list.add(0x10).readS32();
    if(count<0 || count>32)throw new Error('plane count bound');
    let node=head,last=ptr(0);const planes=[];
    for(let i=0;i<count;++i){
        if(node.isNull())throw new Error('short plane list');
        planes.push(planeRecord(node));last=node;node=node.add(0x38).readPointer();
    }
    if(!node.isNull() || !last.equals(tail))throw new Error('plane list count/tail mismatch');
    return {address:g.toString(),vtable:g.readPointer().toString(),vertices:coordinates.length/3,face_count:faceCount,
        coordinates,faces,classification:{pointer:classification.toString(),length:faceCount,
            allocation_observed:!!observed,values:classification.isNull()?[]:readArray(classification,faceCount,true),
            raw_hex:classification.isNull()?'':bytes(classification,faceCount*4)},
        face_vector_0x68:geometryVector(g.add(0x68),true),
        plane_list:{head:head.toString(),tail:tail.toString(),count,planes}};
}
function geometryState(object) {
    const slot0=object.add(0x3c00).readPointer(),owner=object.add(0x3a98).readPointer(),dimension=owner.add(0x1c).readS32();
    return {snapshot_completeness:'constructor-backed-spatial-vectors-classification-planes',
        enabled_0x3990:object.add(0x3990).readU8(),flag_0x3994:object.add(0x3994).readS32(),
        slot0:slot0.toString(),data:slot0.isNull()?null:geometryData(slot0)};
}
Interceptor.attach(at(0x2f9970),{
    onEnter(args){this.geometry=args[0];},
    onLeave(){safe(()=>{
        const g=this.geometry,faces=geometryVector(g.add(0x18),true),count=faces.length/4,pointer=g.add(0x20).readPointer();
        if(count>0){
            if(pointer.isNull())throw new Error('geometry classification allocation failed');
            geometryClassification.set(g.toString(),{pointer:pointer.toString(),count});
        }
        if(currentFile!==null)emit('geometry-classification-allocated',{geometry:g.toString(),count,pointer:pointer.toString()});
    });}
});
Interceptor.attach(at(0x2059a0),{
    onEnter(args){
        if(currentFile===null || args[0].add(0x3990).readU8()===0)return;
        this.frame={object:args[0]};geometryFinalizerFrames.push(this.frame);
    },
    onLeave(){if(this.frame && geometryFinalizerFrames.pop()!==this.frame)emit('probe-error',{error:'geometry finalizer stack mismatch'});}
});
Interceptor.attach(at(0x23e050),{
    onEnter(args){
        if(!geometryFinalizerFrames.length)return;
        this.output=args[3];this.points=safe(()=>[0,1,2].map(i=>bytes(args[i],24)));
    },
    onLeave(){if(this.output)safe(()=>emit('geometry-normal',{points_hex:this.points,normal_hex:bytes(this.output,24)}));}
});
for(const [rva,field] of [[0x1985e0,'enabled_0x3990'],[0x198600,'flag_0x3994']]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            this.enabled=currentFile!==null;
            if(!this.enabled)return;
            this.object=args[0];this.value=field==='enabled_0x3990'?(args[1].toInt32()&255):args[1].toInt32();
            safe(()=>emit('geometry-flag-before',{field,value:this.value,object:this.object.toString(),
                geometry:geometryState(this.object)}));
        },
        onLeave(){if(this.enabled)safe(()=>emit('geometry-flag-after',{field,value:this.value,
            object:this.object.toString(),geometry:geometryState(this.object)}));}
    });
}
Interceptor.attach(at(0x198620),{
    onEnter(args){
        this.enabled=currentFile!==null;
        if(!this.enabled)return;
        this.object=args[0];this.index=safe(()=>args[1].readS32());
    },
    onLeave(result){if(this.enabled)safe(()=>emit('geometry-access',{
        object:this.object.toString(),index:this.index,geometry:result.toString()}));}
});
Interceptor.attach(at(0x2fcd90),{
    onEnter(args){
        this.enabled=currentFile!==null;
        if(!this.enabled)return;
        this.geometry=args[0];
        // 2f9600/2f96a0 take pointers to signed counts; nonpositive counts preserve vectors.
        safe(()=>emit('geometry-replace-before',{geometry:this.geometry.toString(),
            requested_vertices:args[1].readS32(),requested_faces:args[2].readS32(),state:geometryData(this.geometry)}));
    },
    onLeave(){if(this.enabled)safe(()=>emit('geometry-replace-after',{geometry:this.geometry.toString(),state:geometryData(this.geometry)}));}
});


for(const [rva,kind,count] of [[0x413fb0,'geometry-header3',3],[0x415880,'geometry-coord3',4],[0x414200,'geometry-face5',5],[0x413ee0,'geometry-header2',2],[0x415990,'geometry-matrix4',5]]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(currentFile===null || !activeHandler || activeHandler.id!==158)return;
            this.entry=safe(()=>({kind,stream:args[0],before:stream(args[0]),
                pointers:Array.from({length:count},(_,i)=>args[i+1]),lines:[],scans:[],
                conversion_pointer:kind==='geometry-header3'?args[4]:null}));
            if(this.entry)realStack().push(this.entry);
        },
        onLeave(result){
            if(!this.entry)return;
            safe(()=>{
                const e=this.entry;
                if(realStack().pop()!==e)throw new Error('geometry row stack mismatch');
                emit('geometry-row',{kind,destinations:e.pointers.map(p=>p.toString()),status:result.toInt32(),before:e.before,after:stream(e.stream),lines:e.lines,scans:e.scans,
                    values:e.pointers.map((p,i)=>(kind==='geometry-coord3' || kind==='geometry-matrix4') && i>0?p.readDouble():p.readS32()),
                    output_hex:e.pointers.map((p,i)=>bytes(p,(kind==='geometry-coord3' || kind==='geometry-matrix4') && i>0?8:4)),
                    optional_conversion_count:e.conversion_pointer && !e.conversion_pointer.isNull()?e.conversion_pointer.readS32():null});
            });
        }
    });
}
const segmentFrames=[];
const transformedFrames=[];
const segmentAllocations=new Map();
let segmentReaderSerial=0;
function nativeList(handle,limit) {
    if(handle.isNull())throw new Error('null native list handle');
    const storage=handle.readPointer();
    if(storage.isNull())throw new Error('null native list storage');
    const count=storage.add(0x10).readU64().toNumber();
    if(!Number.isSafeInteger(count) || count<0 || count>limit)throw new Error('native list count bound');
    const sentinel=storage.add(8).readPointer();
    if(sentinel.isNull())throw new Error('null native list sentinel');
    const nodes=[];let node=sentinel.readPointer();
    for(let i=0;i<count;++i) {
        if(node.isNull() || node.equals(sentinel))throw new Error('short native list');
        if(nodes.some(seen=>seen.equals(node)))throw new Error('native list cycle before sentinel');
        nodes.push(node);node=node.readPointer();
    }
    if(!node.equals(sentinel))
        throw new Error('native list sentinel closure mismatch');
    return {storage:storage.toString(),references:storage.readU32(),sentinel:sentinel.toString(),count,nodes};
}
function segmentPayload(p) {
    if(p.isNull())throw new Error('null segment payload');
    const format=p.add(0x10).readS32(),count=p.add(0x14).readS32(),data=p.add(0x18).readPointer();
    if(![0,1,2].includes(format) || count<0 || count>16)throw new Error('segment format/count outside bounds');
    const width=format===1?3:format===2?6:0,items=count*width;
    if(p.readPointer().sub(app.base).toInt32()!==0x537a38)throw new Error('unexpected segment vtable');
    const observed=segmentAllocations.get(p.toString());
    if(items>0 && (data.isNull() || !observed || observed.pointer!==data.toString() || observed.items!==items))
        throw new Error('unproven segment allocation extent');
    if(items===0 && !data.isNull())throw new Error('non-null zero-extent segment allocation');
    return {address:p.toString(),vtable_rva:0x537a38,format,count,width,
        data_pointer:data.toString(),allocation_observed:!!observed,
        auxiliary_pointer:p.add(0x20).readPointer().toString(),flag_0x28:p.add(0x28).readU8(),
        values:items?readArray(data,items,false):[],values_hex:items?bytes(data,items*8):''};
}
function segmentContainer(container) {
    const list=nativeList(container.add(0x18),32),segments=list.nodes.map(node=>{
        const wrapper=node.add(0x10).readPointer();
        if(wrapper.isNull())throw new Error('null DEF_EDGE wrapper');
        const payload=wrapper.add(8).readPointer();
        return {node:node.toString(),wrapper:wrapper.toString(),
            wrapper_vtable_rva:wrapper.readPointer().sub(app.base).toInt32(),
            id:wrapper.add(0x30).readS32(),...segmentPayload(payload)};
    });
    const {nodes,...listState}=list;
    return {address:container.toString(),list:listState,segments,
        matrix:readArray(container.add(0x38),16,false),matrix_hex:bytes(container.add(0x38),128),
        flag_0xb8:container.add(0xb8).readU8()};
}
function segmentState(object) {
    const handle=object.add(0x3998).readPointer();
    if(handle.isNull())return {handle:handle.toString(),list:null,containers:[]};
    const list=nativeList(handle,4),{nodes,...listState}=list;
    return {handle:handle.toString(),list:listState,
        containers:nodes.map(node=>({node:node.toString(),...segmentContainer(node.add(0x10))}))};
}
// Count/format stores precede allocation; only inspect payload after the virtual
// allocator returns, when its entire owned range has been zero-initialized.
Interceptor.attach(at(0x276030),{
    onEnter(args){segmentAllocations.delete(args[0].toString());}
});
Interceptor.attach(at(0x27f360),{
    onEnter(args){this.payload=args[0];},
    onLeave(){safe(()=>{
        const p=this.payload,format=p.add(0x10).readS32(),count=p.add(0x14).readS32();
        if(![0,1,2].includes(format) || count<0 || count>16)throw new Error('segment allocation outside bounds');
        const width=format===1?3:format===2?6:0;
        segmentAllocations.set(p.toString(),{pointer:p.add(0x18).readPointer().toString(),items:count*width});
        if(currentFile!==null)emit('segment-allocated',{segment:segmentPayload(p)});
    });}
});
for(const [rva,kind] of [[0x39a200,'segments'],[0x399e60,'transformed']]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(currentFile===null)return;
            this.frame={serial:++segmentReaderSerial,root:args[0],file:args[1],object:args[2],
                first:args[3].toInt32(),second:args[4].toInt32(),kind};
            (kind==='segments'?segmentFrames:transformedFrames).push(this.frame);
            safe(()=>emit('segment-reader-before',{reader_serial:this.frame.serial,kind,
                root:this.frame.root.toString(),object:this.frame.object.toString(),
                first:this.frame.first,second:this.frame.second,stream:stream(this.frame.file),
                state:segmentState(this.frame.object)}));
        },
        onLeave(result){
            if(!this.frame)return;
            const c=this.frame;
            safe(()=>emit('segment-reader-after',{reader_serial:c.serial,kind,status:result.toInt32(),
                stream:stream(c.file),state:segmentState(c.object)}));
            if((kind==='segments'?segmentFrames:transformedFrames).pop()!==c)
                emit('probe-error',{error:'segment reader stack mismatch'});
        }
    });
}
Interceptor.attach(at(0x27fa30),{
    onEnter(args){
        if(currentFile===null)return;this.container=args[0];
        safe(()=>emit('segment-clear-before',{state:segmentContainer(this.container)}));
    },
    onLeave(){if(this.container)safe(()=>emit('segment-clear-after',{state:segmentContainer(this.container)}));}
});
Interceptor.attach(at(0x39aa60),{
    onEnter(args){
        if(currentFile===null)return;this.container=args[0];
        safe(()=>emit('segment-matrix-before',{state:segmentContainer(this.container),
            replacement:readArray(args[1],16,false),replacement_hex:bytes(args[1],128)}));
    },
    onLeave(){if(this.container)safe(()=>emit('segment-matrix-after',{state:segmentContainer(this.container)}));}
});
send({operation:'hooks-installed',base:app.base.toString(),pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
