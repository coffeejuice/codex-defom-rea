// Passive bounded material-shape producers and mode4 consumers; physical extents remain descriptor-owned.
// Reference buffers: 3883e9 calloc(count,4), 479910 free; no logical-count-derived allocation reads.
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
const columnWrappers=[];
const columnReaders=[];
let columnSerial=0;
const fieldReaders=[];
let fieldSerial=0;
const dynamicReaders=[];
const customReaders=[];
// Keep detailed-row lifetime outside shared InvocationContext properties.
const customRowInvocations=new Map();
const shapeReaders=[];
const shapeRows=[];
const shapeInvocations=new Map();
const shapeRowInvocations=new Map();
let shapeSerial=0;
let customSerial=0;
const customDynamicContainers=new Map();
const dynamicAllocations=new Map();
const dynamicMetadata=new Map();
let dynamicSerial=0;
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
        custom_fields:customFieldStore(p), dynamic_arrays:customDynamicStore(p),
        material_group_0x3980:p.add(0x3980).readS32(),
        saved_shape_width_0xadc:p.add(0xadc).readS32(),saved_texture_width_0xae0:p.add(0xae0).readS32(),
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
        material_count:p.add(0x1320).readS32(),material_list:materialList(p.add(0x1328))};
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
        user_node_width_0x3398:p.add(0x3398).readS32(),
        user_element_width_0x33a8:p.add(0x33a8).readS32(),
        user_field_metadata:dynamicUserMetadata(p),
        custom_last_id_0x33b8:p.add(0x33b8).readS32(),
        geometry_mode:p.add(0x850).readS32(),
        finalization_controls:finalizationControls(p),time_table:timeTable(p.add(0x938)),
        time_controls_hex:timeControlsHex(p),
        ...importFlags(p),nstart_option_0x1740:p.add(0x1740).readS32(),
        material_count:p.add(0x1320).readS32(),material_list:materialList(p.add(0x1328)),
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
                this.column=columnReaders.at(-1);this.field=fieldReaders.at(-1);this.dynamic=dynamicReaders.at(-1);this.custom=customReaders.at(-1);this.shape=shapeRows.at(-1);
                this.reference=activeHandler && activeHandler.category===4 && activeHandler.id===116 && this.format==='%Flf';
                if(this.format==='%Flf' && !this.segment && !this.reference && !this.column && !this.field && !this.dynamic && !this.custom && !this.shape) this.previousOutput=bytes(this.destination,8);});
    },
    onLeave(result) {
        if (!this.enabled || !this.before) return;
        safe(()=> {
            if (this.format!=='%39s' && this.format!=='%i' && this.format!=='%Flf') throw new Error('unexpected fscanf format '+this.format);
            if(activeHandler && activeHandler.category===4 && this.format==='%i') {
                const converted=result.toInt32()===1;
                emit('material-selector',{id:activeHandler.id,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    output_hex:converted?bytes(this.destination,4):null,
                    value:converted?this.destination.readS32():null});
                return;
            }
            if(this.shape) {
                const converted=result.toInt32()===1;
                if(this.format!=='%i' && this.format!=='%Flf')throw new Error('material shape scalar format');
                emit('material-shape-scan',{reader_serial:this.shape.serial,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            if(this.custom) {
                const converted=result.toInt32()===1;
                if(this.format!=='%i' && this.format!=='%Flf')throw new Error('custom scalar format');
                emit('custom-scan',{reader_serial:this.custom.serial,kind:this.custom.kind,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            if(this.dynamic) {
                const converted=result.toInt32()===1;
                if(this.format!=='%i' && this.format!=='%Flf')throw new Error('dynamic scalar format');
                emit('dynamic-scan',{reader_serial:this.dynamic.serial,kind:this.dynamic.kind,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            if(this.field) {
                const converted=result.toInt32()===1;
                if(this.format!=='%i' && this.format!=='%Flf')throw new Error('field scalar format');
                emit('field-scan',{reader_serial:this.field.serial,kind:this.field.kind,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            if(this.column) {
                const converted=result.toInt32()===1;
                if(this.format!=='%i' && this.format!=='%Flf')throw new Error('column scalar format');
                emit('column-scan',{reader_serial:this.column.serial,kind:this.column.kind,format:this.format,
                    conversions:result.toInt32(),before:this.before,after:stream(this.file),
                    destination:this.destination.toString(),
                    output_hex:converted?bytes(this.destination,this.format==='%i'?4:8):null,
                    value:converted?(this.format==='%i'?this.destination.readS32():this.destination.readDouble()):null});
                return;
            }
            if(this.reference) {
                const converted=result.toInt32()===1;
                emit('material-reference-scan',{format:this.format,conversions:result.toInt32(),
                    before:this.before,after:stream(this.file),destination:this.destination.toString(),
                    material:activeHandler.referenceMaterial?.toString()??null,
                    output_hex:converted?bytes(this.destination,8):null,
                    value:converted?this.destination.readDouble():null});
                return;
            }
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
for(const [rva,category] of [[0x38b8a0,2],[0x39aa90,1],[0x384a50,4]]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            this.root=args[0];this.id=args[2].toInt32();this.previousHandler=activeHandler;
            this.enabled=currentFile!==null;
            if(this.enabled){activeHandler={root:this.root,id:this.id,category,file:args[1]};
                safe(()=>emit('handler-before',{id:this.id,category,state:rootState(this.root),stream:stream(args[1])}));}
        },
        onLeave(result) {
            if(this.enabled && (this.id===210 || this.id===211))safe(()=>{
                if(shapeReaders.length || shapeRows.length || shapeInvocations.has(this.threadId) || shapeRowInvocations.has(this.threadId))
                    throw new Error('material shape frame remains at handler return');
            });
            if(this.enabled && (this.id===198 || this.id===266))safe(()=>{
                if(customReaders.length || (customRowInvocations.get(this.threadId)?.length??0))
                    throw new Error('custom reader frame remains at handler return');
            });
            if(this.enabled && this.id===162 && activeHandler.fieldResult) safe(()=>{
                const c=activeHandler.fieldResult;
                emit('field-postprocess',{id:162,reader_serial:c.serial,object:c.object.toString(),
                    dimension:this.root.add(0x1c).readS32(),before:c.before,after:fieldBuffer(c)});
            });
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
// Count/format stores precede allocation.275f30 stores the zero-initialized
// owned buffer after275fa0 returns; both27f360 import allocation and276140
// deep-copy allocation pass here. Observing only27f360 missed copied payloads.
Interceptor.attach(at(0x276030),{
    onEnter(args){segmentAllocations.delete(args[0].toString());}
});
Interceptor.attach(at(0x275f30),{
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

// Selected material fields only. Read the name through its first terminator;
// bytes after it and all unrelated material padding/property tables are omitted.
const materialScratch = new Map();
const materialRefreshFrames = [];
const referenceAllocations = new Map();
const referenceFinalizers = [];
let referenceAllocationSerial=0;
function materialName(p) {
    let length=0;
    while(length<65 && p.add(length).readU8()!==0) ++length;
    if(length===65)throw new Error('unterminated material name capacity65');
    return {name_hex:bytes(p,length),name_length:length,name_terminator_hex:bytes(p.add(length),1)};
}
function materialRecord(p) {
    if(p.isNull())throw new Error('null material record');
    const references=p.add(0x1ccc).readS32(),referencePointer=p.add(0x1cd0).readPointer();
    const known=referenceAllocations.get(referencePointer.toString());
    if(!referencePointer.isNull() && (!known || known.material!==p.toString()))
        throw new Error('unobserved material reference allocation');
    const capacity=referencePointer.isNull()?0:known.capacity;
    if(references>capacity)throw new Error('material reference logical count exceeds observed capacity');
    const allocationValues=capacity?readArray(referencePointer,capacity,true):[];
    return {address:p.toString(),id:p.readS32(),field_0x4:p.add(4).readS32(),field_0x8:p.add(8).readS32(),
        ...materialName(p.add(0xc)),field_0x17e8:p.add(0x17e8).readS32(),
        mode_0x1cc8:p.add(0x1cc8).readS32(),reference_count_0x1ccc:references,
        reference_pointer_0x1cd0:referencePointer.toString(),reference_ids:allocationValues.slice(0,Math.max(0,references)),
        reference_allocation:{pointer:referencePointer.toString(),capacity,values:allocationValues,
            values_hex:capacity?bytes(referencePointer,capacity*4):'',allocation_observed:!referencePointer.isNull()},
        field_0x1e40:p.add(0x1e40).readS32(),field_0x2150:p.add(0x2150).readS32(),
        field_0x2154:p.add(0x2154).readS32(),
        field_0x2158:p.add(0x2158).readS32(),field_0x215c:p.add(0x215c).readS32(),
        next:p.add(0x2200).readPointer().toString(),owner:p.add(0x2208).readPointer().toString()};
}
function materialList(list) {
    const head=list.readPointer(),tail=list.add(8).readPointer(),count=list.add(0x10).readS32();
    if(count<0 || count>4)throw new Error('material list bound');
    const records=[];let p=head;
    for(let i=0;i<count;++i) {
        if(p.isNull() || records.some(record=>record.address===p.toString()))
            throw new Error('short or cyclic material list');
        records.push(materialRecord(p));p=p.add(0x2200).readPointer();
    }
    if(!p.isNull() || (count===0 && (!head.isNull() || !tail.isNull()))
        || (count>0 && records[count-1].address!==tail.toString()))
        throw new Error('material count/tail mismatch');
    const ids=list.add(0x18).readPointer(),ranks=list.add(0x20).readPointer();
    const known=materialScratch.get(list.toString());
    let scratch={ids_pointer:ids.toString(),ranks_pointer:ranks.toString(),
        allocation_observed:false,count:null,ids:null,ranks:null};
    if(known) {
        if(known.ids!==ids.toString() || known.ranks!==ranks.toString())
            throw new Error('material scratch pointer changed without observed allocation');
        if(known.count>0 && (ids.isNull() || ranks.isNull()))
            throw new Error('null positive material scratch extent');
        scratch={...scratch,allocation_observed:true,count:known.count,
            ids:known.count?readArray(ids,known.count,true):[],
            ranks:known.count?readArray(ranks,known.count,true):[]};
    } else if(!ids.isNull() || !ranks.isNull()) {
        throw new Error('unobserved material scratch allocation');
    }
    return {address:list.toString(),root_count:list.sub(8).readS32(),head:head.toString(),
        tail:tail.toString(),count,owner:list.add(0x28).readPointer().toString(),records,scratch};
}
Interceptor.attach(at(0x413ab0),{
    onEnter(args){
        if(currentFile===null || !activeHandler || activeHandler.category!==4)return;
        this.entry=safe(()=>{
            const capacity=args[2].toInt32();
            if(activeHandler.id!==92 || capacity!==65)throw new Error('unexpected material name scope');
            return {stream:args[0],destination:args[1],capacity,before:stream(args[0]),
                previous_name:materialName(args[1]),lines:[],scans:[]};
        });
        if(this.entry)realStack().push(this.entry);
    },
    onLeave(result){
        if(!this.entry)return;
        const e=this.entry;
        safe(()=>{
            if(realStack().pop()!==e)throw new Error('material name reader stack mismatch');
            emit('material-name',{capacity:e.capacity,destination:e.destination.toString(),
                status:result.toInt32(),before:e.before,after:stream(e.stream),
                previous_name:e.previous_name,name:materialName(e.destination),lines:e.lines,scans:e.scans});
        });
    }
});
Interceptor.attach(at(0x29c2c0),{
    onEnter(args){if(currentFile!==null){this.list=args[0];this.id=args[1].toInt32();
        if(referenceFinalizers.length)safe(()=>emit('material-reference-lookup-before',
            {requested:this.id,state:materialList(this.list)}));}},
    onLeave(result){if(this.list)safe(()=>emit('material-lookup',{list:this.list.toString(),
        requested:this.id,result:result.toString(),record:result.isNull()?null:materialRecord(result)}));}
});
Interceptor.attach(at(0x29c060),{
    onEnter(args){if(currentFile!==null){this.list=args[0];safe(()=>emit('material-append-before',
        {state:materialList(this.list)}));}},
    onLeave(result){if(this.list)safe(()=>emit('material-append-after',
        {result:result.toString(),state:materialList(this.list)}));}
});
Interceptor.attach(at(0x29ca10),{
    onEnter(args){
        if(currentFile===null)return;
        this.list=args[0];this.mode=args[1].toInt32()&255;
        this.count=this.list.add(0x10).readS32();
        if(this.count<0 || this.count>4){emit('probe-error',{error:'material scratch count bound'});return;}
        safe(()=>emit('material-scratch-before',{mode:this.mode,state:materialList(this.list)}));
        materialScratch.delete(this.list.toString());
    },
    onLeave(){
        if(!this.list)return;
        safe(()=>{
            if(this.mode===0)materialScratch.set(this.list.toString(),{count:this.count,
                ids:this.list.add(0x18).readPointer().toString(),
                ranks:this.list.add(0x20).readPointer().toString()});
            emit('material-scratch-after',{mode:this.mode,state:materialList(this.list)});
        });
    }
});
Interceptor.attach(at(0x1d0fc0),{
    onEnter(args){
        if(currentFile===null)return;
        this.frame={root:args[0],option:args[1].toInt32()};
        materialRefreshFrames.push(this.frame);
        safe(()=>emit('material-refresh-before',{option:this.frame.option,state:rootState(this.frame.root)}));
    },
    onLeave(){
        if(!this.frame)return;
        safe(()=>{
            emit('material-refresh-after',{option:this.frame.option,state:rootState(this.frame.root)});
            if(materialRefreshFrames.pop()!==this.frame)throw new Error('material refresh stack mismatch');
        });
    }
});
Interceptor.attach(at(0x1d0320),{
    onEnter(args){
        if(currentFile===null || !materialRefreshFrames.length)return;
        this.root=args[0];
        safe(()=>emit('material-layout-before',{state:rootState(this.root)}));
    },
    onLeave(){if(this.root)safe(()=>emit('material-layout-after',{state:rootState(this.root)}));}
});
Interceptor.attach(at(0x29c310),{
    onEnter(args){if(currentFile!==null){this.list=args[0];safe(()=>emit('material-clear-before',
        {state:materialList(this.list)}));}},
    onLeave(){if(this.list)safe(()=>emit('material-clear-after',{state:materialList(this.list)}));}
});
Interceptor.attach(at(0x28cb70),{
    onEnter(args){if(currentFile!==null){this.record=args[0];safe(()=>emit('material-destroy-before',
        {record:materialRecord(this.record)}));}},
    onLeave(){if(this.record)emit('material-destroy-after',{address:this.record.toString()});}
});

// 1613a0 ABI from material-refresh dossier: dest,source,old79table,
// then six pointers to BYTE flags4..9. Copy snapshots are separate from the
// persistent root list. Temporary geometry payload reads are intentionally
// omitted until every alternate-copy allocation path is proven by this probe.
let materialCopySerial=0;
const materialTemporaryObjects=new Map();
function temporaryObject(p) {
    const vars=[];
    for(const entry of descriptors.values()) {
        if(entry.slot.compare(p.add(0x8b0))>=0 && entry.slot.compare(p.add(0xac0))<=0)
            vars.push(descriptor(entry.pointer,p));
    }
    vars.sort((a,b)=>a.slot_offset-b.slot_offset);
    return {address:p.toString(),owner:p.add(0x3a98).readPointer().toString(),
        material_group_0x3980:p.add(0x3980).readS32(),
        id:p.add(8).readS32(),type:p.add(12).readS32(),name_hex:boundedCStringHex(p.add(0x828),65),
        mesh_flag:p.add(0x87c).readS32(),nodes:p.add(0x880).readS32(),elements:p.add(0x884).readS32(),
        arity:p.add(0x894).readS32(),node_allocation_count:p.add(0x9cc).readS32(),
        element_allocation_count:p.add(0xac8).readS32(),allocation_arity:p.add(0xacc).readS32(),
        flag_0x3990:p.add(0x3990).readU8(),flag_0x3994:p.add(0x3994).readS32(),
        boundary_group_count:p.add(0xbd8).readS32(),boundary_total:p.add(0x88c).readS32(),
        spatial_geometry_pointer:p.add(0x3c00).readPointer().toString(),
        alternate_geometry_handle:p.add(0x3998).readPointer().toString(),
        geometry_payload_omitted:true,buffer_metadata:bufferSnapshot(p),descriptors:vars};
}
Interceptor.attach(at(0x1613a0),{
    onEnter(args){
        if(currentFile===null || !materialRefreshFrames.length)return;
        this.capture=safe(()=>{
            const frame={serial:++materialCopySerial,destination:args[0],source:args[1],
                root:materialRefreshFrames.at(-1).root,
                components:readArray(args[2],79,true),
                flags:Array.from({length:6},(_,i)=>args[i+3].readU8())};
            if(frame.source.isNull() || frame.destination.isNull())throw new Error('null material copy object');
            materialTemporaryObjects.set(frame.destination.toString(),frame.serial);
            emit('material-copy-before',{copy_serial:frame.serial,root:frame.root.toString(),
                flags:frame.flags,components:frame.components,
                source:object(frame.source),destination:temporaryObject(frame.destination)});
            return frame;
        });
    },
    onLeave(){
        if(!this.capture)return;
        const c=this.capture;
        safe(()=>emit('material-copy-after',{copy_serial:c.serial,root:c.root.toString(),
            flags:c.flags,components:c.components,source:object(c.source),destination:temporaryObject(c.destination)}));
    }
});
// Destination pointers are inspected only before the real destructor. After
// return, preserve its address/serial only and remove it from probe ownership.
Interceptor.attach(at(0x15f790),{
    onEnter(args){
        const serial=materialTemporaryObjects.get(args[0].toString());
        if(serial===undefined)return;
        this.capture={serial,address:args[0].toString()};
        if(currentFile!==null)safe(()=>emit('material-copy-destroy-before',
            {copy_serial:serial,destination:temporaryObject(args[0])}));
    },
    onLeave(){
        if(!this.capture)return;
        const c=this.capture;materialTemporaryObjects.delete(c.address);
        if(currentFile!==null)emit('material-copy-destroy-after',{copy_serial:c.serial,address:c.address});
    }
});



// The count helper zeroes and assigns the live material count before returning.
// Save old allocation extent independently; do not snapshot between that write
// and the positive replacement allocation, when count may exceed old capacity.
Interceptor.attach(at(0x413de0),{
    onEnter(args){
        if(currentFile===null || !activeHandler || activeHandler.category!==4 || activeHandler.id!==116)return;
        this.entry=safe(()=>{
            const material=args[1].sub(0x1ccc);
            activeHandler.referenceMaterial=material;
            const entry={kind:'int1',stream:args[0],destination:args[1],material,
                skip:args[2].toInt32(),before:stream(args[0]),previous:materialRecord(material),lines:[],scans:[]};
            if(entry.skip!==0)throw new Error('unexpected reference count skip flag');
            return entry;
        });
        if(this.entry)realStack().push(this.entry);
    },
    onLeave(result){
        if(!this.entry)return;
        safe(()=>{
            const e=this.entry;
            if(realStack().pop()!==e)throw new Error('reference count reader stack mismatch');
            emit('material-reference-count',{kind:e.kind,skip:e.skip,material:e.material.toString(),
                status:result.toInt32(),before:e.before,after:stream(e.stream),
                previous:e.previous,value:e.destination.readS32(),output_hex:bytes(e.destination,4),
                lines:e.lines,scans:e.scans});
        });
    }
});
Interceptor.attach(at(0x480e14),{
    onEnter(args){
        if(currentFile===null || !activeHandler || activeHandler.id!==116 || activeHandler.category!==4
            || !this.returnAddress.equals(at(0x3883ee)))return;
        this.capture=safe(()=>{
            const capacity=args[0].toInt32(),width=args[1].toInt32(),material=activeHandler.referenceMaterial;
            if(capacity<1 || capacity>8 || width!==4 || !material)throw new Error('reference calloc scope/bound');
            return {capacity,material,serial:++referenceAllocationSerial};
        });
    },
    onLeave(result){
        if(!this.capture)return;
        const c=this.capture;
        safe(()=>{
            if(result.isNull())throw new Error('reference allocation failed');
            referenceAllocations.set(result.toString(),{capacity:c.capacity,material:c.material.toString(),serial:c.serial});
            emit('material-reference-allocate',{serial:c.serial,material:c.material.toString(),pointer:result.toString(),
                capacity:c.capacity,width:4,values:readArray(result,c.capacity,true),values_hex:bytes(result,c.capacity*4),
                call_return_rva:0x3883ee});
        });
    }
});
Interceptor.attach(at(0x479910),{
    onEnter(args){
        const known=referenceAllocations.get(args[0].toString());
        if(!known)return;
        this.capture={...known,pointer:args[0].toString(),call_return_rva:this.returnAddress.sub(app.base).toInt32()};
        if(currentFile!==null)safe(()=>emit('material-reference-free-before',{...this.capture,
            values:readArray(args[0],known.capacity,true),values_hex:bytes(args[0],known.capacity*4)}));
        referenceAllocations.delete(args[0].toString());
    },
    onLeave(){if(this.capture && currentFile!==null)emit('material-reference-free-after',this.capture);}
});
Interceptor.attach(at(0x1d6250),{
    onEnter(args){
        if(currentFile===null)return;
        this.root=args[0];referenceFinalizers.push(this.root);
        safe(()=>emit('material-reference-finalize-before',{state:materialList(this.root.add(0x1328))}));
    },
    onLeave(){
        if(!this.root)return;
        safe(()=>{
            emit('material-reference-finalize-after',{state:materialList(this.root.add(0x1328))});
            if(referenceFinalizers.pop()!==this.root)throw new Error('reference finalizer nesting');
        });
    }
});
Interceptor.attach(at(0x1d64f0),{
    onEnter(args){
        const feature=args[1].toInt32();
        if(currentFile===null || feature<49 || feature>52)return;
        this.capture=safe(()=>({root:args[0],feature,control_0x85c:args[0].add(0x85c).readS32(),
            state:materialList(args[0].add(0x1328))}));
    },
    onLeave(result){if(this.capture)safe(()=>emit('material-reference-feature',{
        root:this.capture.root.toString(),feature:this.capture.feature,result:result.toInt32()&255,
        control_0x85c:this.capture.control_0x85c,state:this.capture.state}));}
});



function columnBuffer(p,rows,width,integers=false) {
    if(!Number.isInteger(rows) || !Number.isInteger(width) || rows<0 || rows>4 || width<0 || width>8)
        throw new Error('column buffer bound');
    const n=rows*width;
    return {values:n?readArray(p,n,integers):[],values_hex:bytes(p,n*(integers?4:8))};
}
function columnWrapperState(c) {
    return {serial:c.serial,id:c.id,root:c.root.toString(),object:c.object.toString(),
        destination:c.destination.toString(),element_count:c.count,destination_width:c.width,
        stream:stream(c.file),...columnBuffer(c.destination,c.count,c.width)};
}
Interceptor.attach(at(0x3981c0),{
    onEnter(args){
        if(currentFile===null || !activeHandler || ![205,216].includes(activeHandler.id))return;
        this.capture=safe(()=>{
            const c={serial:++columnSerial,id:activeHandler.id,root:args[0],file:args[1],object:args[2],
                destination:args[3],count:args[4].toInt32(),width:args[5].toInt32()};
            const slot=c.id===205?0xa58:0xa60;
            if(!c.object.add(slot).readPointer().equals(c.destination) || c.count<0 || c.count>c.object.add(0x884).readS32()
                || c.width<0 || c.width>c.root.add(0x1320).readS32())throw new Error('column destination binding/extent');
            columnWrappers.push(c);emit('column-wrapper-before',columnWrapperState(c));return c;
        });
    },
    onLeave(result){
        if(!this.capture)return;
        safe(()=>{
            emit('column-wrapper-after',{...columnWrapperState(this.capture),status:result.toInt32()});
            if(columnWrappers.pop()!==this.capture)throw new Error('column wrapper nesting');
        });
    }
});
Interceptor.attach(at(0x398350),{
    onEnter(args){
        if(!columnWrappers.length)return;
        this.capture=safe(()=>{
            const root=args[0],object=args[1],map=args[2],count=root.add(0x1320).readS32();
            if(map.isNull() || object.add(0x87c).readS32()===0 || count<1 || count>4)
                throw new Error('column map guard outside proven inputwidth domain');
            return {wrapper_serial:columnWrappers.at(-1).serial,root,object,map,count,width:args[3]};
        });
    },
    onLeave(){
        if(!this.capture)return;
        safe(()=>{
            const c=this.capture,n=c.object.add(0x884).readS32(),width=c.width.readS32();
            if(width<0 || width>c.count)throw new Error('column input width bound');
            emit('column-map-after',{wrapper_serial:c.wrapper_serial,root:c.root.toString(),object:c.object.toString(),
                material_count:c.count,map:readArray(c.map,c.count,true),map_hex:bytes(c.map,c.count*4),input_width:width,
                mesh_flag:c.object.add(0x87c).readS32(),
                direct61:bufferSnapshot(c.object,'elements').direct61,materials:materialList(c.root.add(0x1328))});
        });
    }
});
Interceptor.attach(at(0x416c90),{
    onEnter(args){
        if(!columnWrappers.length)return;
        this.entry=safe(()=>{
            const c={serial:++columnSerial,wrapper_serial:columnWrappers.at(-1).serial,kind:'columns',
                stream:args[0],buffer:args[1],count:args[2].toInt32(),width:args[3].readS32(),
                mode:args[4].toInt32(),lines:[],scans:[]};
            if(c.mode!==1)throw new Error('column mode outside selected producer');
            columnReaders.push(c);realStack().push(c);
            emit('column-reader-before',{serial:c.serial,wrapper_serial:c.wrapper_serial,kind:c.kind,
                element_count:c.count,input_width:c.width,mode:c.mode,stream:stream(c.stream),
                ...columnBuffer(c.buffer,c.count,c.width)});return c;
        });
    },
    onLeave(result){
        if(!this.entry)return;
        safe(()=>{
            const c=this.entry;
            emit('column-reader-after',{serial:c.serial,wrapper_serial:c.wrapper_serial,kind:c.kind,
                element_count:c.count,input_width:c.width,mode:c.mode,stream:stream(c.stream),
                ...columnBuffer(c.buffer,c.count,c.width),status:result.toInt32(),lines:c.lines,scans:c.scans});
            if(realStack().pop()!==c || columnReaders.pop()!==c)throw new Error('column reader nesting');
        });
    }
});
for(const [rva,kind] of [[0x419b90,'explicit'],[0x4199a0,'implicit']]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(!columnWrappers.length)return;
            this.capture=safe(()=>{
                const c={serial:++columnSerial,parent_serial:columnReaders.at(-1).serial,kind,file:args[0],buffer:args[1],
                    declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32(),
                    explicit:kind==='explicit'?args[5].toInt32():0};
                if(c.declared<0 || c.declared>8 || c.explicit<0 || c.explicit>8)throw new Error('column detailed row bound');
                columnReaders.push(c);
                emit('column-rows-before',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,
                    declared_rows:c.declared,element_count:c.count,input_width:c.width,explicit_width:c.explicit,
                    stream:stream(c.file),...columnBuffer(c.buffer,c.count,c.width)});return c;
            });
        },
        onLeave(result){
            if(!this.capture)return;
            safe(()=>{
                const c=this.capture;
                emit('column-rows-after',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,
                    declared_rows:c.declared,element_count:c.count,input_width:c.width,explicit_width:c.explicit,
                    stream:stream(c.file),...columnBuffer(c.buffer,c.count,c.width),status:result.toInt32()});
                if(columnReaders.pop()!==c)throw new Error('column rows nesting');
            });
        }
    });
}
Interceptor.attach(at(0x47f898),{
    onEnter(args){
        if(!columnReaders.length)return;
        this.capture=safe(()=>({reader:columnReaders.at(-1),buffer:args[0],limit:args[1].toInt32(),file:args[2],before:stream(args[2])}));
    },
    onLeave(result){
        if(!this.capture)return;
        safe(()=>{
            const c=this.capture;
            emit('column-line',{reader_serial:c.reader.serial,kind:c.reader.kind,limit:c.limit,eof:result.isNull(),
                hex:result.isNull()?null:boundedCStringHex(c.buffer,c.limit),before:c.before,after:stream(c.file)});
        });
    }
});




function groupObject(destination) {
    const root=activeHandler.root,count=root.add(0x1378).readS32();
    if(count<0 || count>8)throw new Error('group object list bound');
    let p=root.add(0x1380).readPointer();
    for(let i=0;i<count;++i,p=p.add(0x3940).readPointer()) {
        if(p.isNull())throw new Error('short group owner list');
        if(p.add(0x9f8).readPointer().equals(destination))return p;
    }
    throw new Error('group buffer has no persistent object owner');
}
Interceptor.attach(at(0x419d40),{
    onEnter(args){
        if(currentFile===null || !activeHandler || activeHandler.id!==174)return;
        this.entry=safe(()=>{
            const c={serial:++columnSerial,kind:'integercolumns',stream:args[0],buffer:args[1],
                count:args[2].toInt32(),width:args[3].toInt32(),object:groupObject(args[1]),lines:[],scans:[]};
            const saved=bufferSnapshot(c.object,'elements').direct61;
            if(c.count<0 || c.width!==1 || c.count*c.width>saved.allocated_items || saved.unknown_items!==0)
                throw new Error('group destination lacks proven initialized extent');
            columnReaders.push(c);realStack().push(c);
            emit('group-reader-before',{serial:c.serial,kind:c.kind,object:c.object.toString(),element_count:c.count,
                width:c.width,stream:stream(c.stream),...columnBuffer(c.buffer,c.count,c.width,true)});return c;
        });
    },
    onLeave(result){
        if(!this.entry)return;
        safe(()=>{
            const c=this.entry;
            emit('group-reader-after',{serial:c.serial,kind:c.kind,object:c.object.toString(),element_count:c.count,
                width:c.width,stream:stream(c.stream),...columnBuffer(c.buffer,c.count,c.width,true),
                status:result.toInt32(),lines:c.lines,scans:c.scans});
            if(realStack().pop()!==c || columnReaders.pop()!==c)throw new Error('group reader nesting');
        });
    }
});
Interceptor.attach(at(0x419ea0),{
    onEnter(args){
        if(!columnReaders.length || columnReaders.at(-1).kind!=='integercolumns')return;
        this.capture=safe(()=>{
            const c={serial:++columnSerial,parent_serial:columnReaders.at(-1).serial,kind:'integerrows',
                file:args[0],buffer:args[1],declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32()};
            if(c.declared<0 || c.declared>8 || c.width!==1)throw new Error('group row bound');
            columnReaders.push(c);
            emit('group-rows-before',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,
                declared_rows:c.declared,element_count:c.count,width:c.width,stream:stream(c.file),
                ...columnBuffer(c.buffer,c.count,c.width,true)});return c;
        });
    },
    onLeave(result){
        if(!this.capture)return;
        safe(()=>{
            const c=this.capture;
            emit('group-rows-after',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,
                declared_rows:c.declared,element_count:c.count,width:c.width,stream:stream(c.file),
                ...columnBuffer(c.buffer,c.count,c.width,true),status:result.toInt32()});
            if(columnReaders.pop()!==c)throw new Error('group rows nesting');
        });
    }
});
function groupPostprocess(p) {
    return {object:p.toString(),owner:p.add(0x3a98).readPointer().toString(),
        material_group_0x3980:p.add(0x3980).readS32(),mesh_flag:p.add(0x87c).readS32(),
        element_count:p.add(0x884).readS32(),direct61:bufferSnapshot(p,'elements').direct61};
}
Interceptor.attach(at(0x202960),{
    onEnter(args){
        if(currentFile===null || !activeHandler || activeHandler.id!==174)return;
        this.object=args[0];safe(()=>emit('group-postprocess-before',groupPostprocess(this.object)));
    },
    onLeave(){if(this.object)safe(()=>emit('group-postprocess-after',groupPostprocess(this.object)));}
});



const fieldDefinitions={"160":{"keyword":"DRZ","slot":2224,"component":1,"domain":"nodes","mode":6,"integers":false},"162":{"keyword":"BCCDEF","slot":2448,"component":18,"domain":"nodes","mode":null,"integers":true},"163":{"keyword":"BCCDFN","slot":2456,"component":19,"domain":"nodes","mode":null,"integers":true},"165":{"keyword":"URZ","slot":2232,"component":2,"domain":"nodes","mode":6,"integers":false},"166":{"keyword":"FRZ","slot":2240,"component":3,"domain":"nodes","mode":0,"integers":false},"167":{"keyword":"PRZ","slot":2256,"component":5,"domain":"nodes","mode":0,"integers":false},"168":{"keyword":"BCCTMP","slot":2464,"component":20,"domain":"nodes","mode":null,"integers":true},"169":{"keyword":"BCCTFN","slot":2472,"component":21,"domain":"nodes","mode":null,"integers":true},"171":{"keyword":"NDHEAT","slot":2280,"component":9,"domain":"nodes","mode":0,"integers":false},"172":{"keyword":"NDFLUX","slot":2296,"component":11,"domain":"nodes","mode":0,"integers":false},"176":{"keyword":"STNCMP","slot":2576,"component":40,"domain":"elements","mode":1,"integers":false},"177":{"keyword":"DENSTY","slot":2608,"component":44,"domain":"elements","mode":0,"integers":false},"206":{"keyword":"DVOLF","slot":2664,"component":51,"domain":"elements","mode":1,"integers":false},"207":{"keyword":"TICF","slot":2672,"component":52,"domain":"elements","mode":1,"integers":false},"213":{"keyword":"VOTAGE","slot":2328,"component":15,"domain":"nodes","mode":0,"integers":false},"214":{"keyword":"DATOM","slot":2312,"component":13,"domain":"nodes","mode":0,"integers":false},"217":{"keyword":"HDNOBJ","slot":2704,"component":56,"domain":"elements","mode":0,"integers":false},"219":{"keyword":"BCCCRB","slot":2480,"component":22,"domain":"nodes","mode":null,"integers":true},"220":{"keyword":"BCCRHT","slot":2488,"component":23,"domain":"nodes","mode":null,"integers":true},"221":{"keyword":"CRBFLX","slot":2320,"component":14,"domain":"nodes","mode":0,"integers":false},"222":{"keyword":"RHTFLX","slot":2336,"component":16,"domain":"nodes","mode":0,"integers":false},"233":{"keyword":"YLDS","slot":2712,"component":57,"domain":"elements","mode":0,"integers":false},"237":{"keyword":"ZEFI","slot":2720,"component":58,"domain":"elements","mode":0,"integers":false},"238":{"keyword":"ZMFI","slot":2728,"component":59,"domain":"elements","mode":0,"integers":false},"239":{"keyword":"CURRNT","slot":2736,"component":62,"domain":"elements","mode":0,"integers":false}};

function fieldBuffer(c) {
    if(c.count<0 || c.count>8 || c.width<0 || c.width>10)throw new Error('field count/width bound');
    const n=c.count*c.width;
    return {values:n?readArray(c.buffer,n,c.integers):[],values_hex:bytes(c.buffer,n*(c.integers?4:8))};
}
function fieldOwner(root,definition,destination) {
    const count=root.add(0x1378).readS32();
    if(count<0 || count>8)throw new Error('field owner count bound');
    let p=root.add(0x1380).readPointer();
    // Null zero-width slots cannot identify an object. The preceding selector
    // is recorded by19ccc0, so retain its returned pointer on the handler.
    if(activeHandler.selectedObject && !activeHandler.selectedObject.isNull())p=activeHandler.selectedObject;
    else throw new Error('field reader lacks observed selected object');
    if(!p.add(definition.slot).readPointer().equals(destination))throw new Error('field destination slot mismatch');
    return p;
}
function fieldReaderState(c) {
    return {serial:c.serial,id:c.id,keyword:c.keyword,object:c.object.toString(),slot:c.slot,component:c.component,
        domain:c.domain,integers:c.integers,mode:c.mode,element_count:c.count,width:c.width,
        stream:stream(c.stream),...fieldBuffer(c)};
}
Interceptor.attach(at(0x19ccc0),{
    onLeave(result){if(currentFile!==null && activeHandler && activeHandler.category===2)activeHandler.selectedObject=ptr(result.toString());}
});
for(const [rva,integers] of [[0x416c90,false],[0x419d40,true]]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(currentFile===null || !activeHandler || activeHandler.category!==2)return;
            const def=fieldDefinitions[activeHandler.id];if(!def || def.integers!==integers)return;
            this.entry=safe(()=>{
                const c={...def,serial:++fieldSerial,id:activeHandler.id,kind:integers?'integerfield':'realfield',
                    stream:args[0],buffer:args[1],count:args[2].toInt32(),
                    width:integers?args[3].toInt32():args[3].readS32(),
                    mode:integers?null:args[4].toInt32(),lines:[],scans:[]};
                if(c.mode!==def.mode)throw new Error('field reader mode differs from static mapping');
                c.object=fieldOwner(activeHandler.root,def,c.buffer);
                const header=c.object.add(c.domain==='nodes'?0x880:0x884).readS32();
                if(c.count!==header)throw new Error('field logical row count binding');
                if(integers) {
                    const b=bufferSnapshot(c.object)[`direct${c.component}`];
                    if(c.count*c.width>b.allocated_items || b.unknown_items!==0)throw new Error('field integer initialized extent unavailable');
                } else {
                    const entry=Array.from(descriptors.values()).find(e=>e.slot.equals(c.object.add(c.slot)));
                    if(!entry)throw new Error('field descriptor not registered');
                    const d=descriptor(entry.pointer,c.object);
                    if(c.width!==d.width || c.count*c.width>d.length || d.storage_pointer!==c.buffer.toString())
                        throw new Error('field descriptor actual allocation binding');
                }
                fieldReaders.push(c);realStack().push(c);
                emit('field-reader-before',fieldReaderState(c));return c;
            });
        },
        onLeave(result){
            if(!this.entry)return;
            safe(()=>{
                const c=this.entry;
                emit('field-reader-after',{...fieldReaderState(c),status:result.toInt32(),lines:c.lines,scans:c.scans});
                if(c.id===162)activeHandler.fieldResult={...c,before:fieldBuffer(c)};
                if(realStack().pop()!==c || fieldReaders.pop()!==c)throw new Error('field reader nesting');
            });
        }
    });
}
for(const [rva,kind,integers] of [[0x419b90,'explicit',false],[0x4199a0,'implicit',false],[0x419ea0,'integerrows',true]]) {
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(!fieldReaders.length)return;
            this.capture=safe(()=>{
                const c={serial:++fieldSerial,parent_serial:fieldReaders.at(-1).serial,kind,integers,
                    file:args[0],buffer:args[1],declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32(),
                    explicit:kind==='explicit'?args[5].toInt32():0};
                if(c.declared<0 || c.declared>8 || c.explicit<0 || c.explicit>10)throw new Error('field detailed row bound');
                fieldReaders.push(c);
                emit('field-rows-before',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,integers:c.integers,
                    declared_rows:c.declared,element_count:c.count,width:c.width,explicit_width:c.explicit,
                    stream:stream(c.file),...fieldBuffer(c)});return c;
            });
        },
        onLeave(result){
            if(!this.capture)return;
            safe(()=>{
                const c=this.capture;
                emit('field-rows-after',{serial:c.serial,parent_serial:c.parent_serial,kind:c.kind,integers:c.integers,
                    declared_rows:c.declared,element_count:c.count,width:c.width,explicit_width:c.explicit,
                    stream:stream(c.file),...fieldBuffer(c),status:result.toInt32()});
                if(fieldReaders.pop()!==c)throw new Error('field detailed rows nesting');
            });
        }
    });
}
Interceptor.attach(at(0x47f898),{
    onEnter(args){
        if(!fieldReaders.length)return;
        this.capture=safe(()=>({reader:fieldReaders.at(-1),buffer:args[0],limit:args[1].toInt32(),file:args[2],before:stream(args[2])}));
    },
    onLeave(result){
        if(!this.capture)return;
        safe(()=>{
            const c=this.capture;
            emit('field-line',{reader_serial:c.reader.serial,kind:c.reader.kind,limit:c.limit,eof:result.isNull(),
                hex:result.isNull()?null:boundedCStringHex(c.buffer,c.limit),before:c.before,after:stream(c.file)});
        });
    }
});

const dynamicDefinitions={
    170:{keyword:'NDTMP',component:8,slot:0x8e0,domain:'nodes'},
    196:{keyword:'USRNOD',component:17,slot:0x968,domain:'nodes'},
    197:{keyword:'USRELM',component:60,slot:0xac0,domain:'elements'},
    254:{keyword:'MATAXI',component:46,slot:0xa40,domain:'elements'},
    255:{keyword:'NWEAR',component:24,slot:0x928,domain:'nodes'},
    263:{keyword:'DRMESH',component:25,slot:0x930,domain:'nodes'}
};
function dynamicActive(){return currentFile!==null && activeHandler && activeHandler.category===2 && dynamicDefinitions[activeHandler.id];}
function dynamicObject(){
    const p=activeHandler.selectedObject;
    if(!p || p.isNull())throw new Error('dynamic reader lacks stable selected object');
    return p;
}
function dynamicDescriptor(p,component){
    const entry=Array.from(descriptors.values()).find(e=>e.pointer.add(8).readS32()===component &&
        e.slot.compare(p)>=0 && e.slot.compare(p.add(0x3b00))<0);
    if(!entry)throw new Error('dynamic descriptor registration missing '+component);
    return descriptor(entry.pointer,p);
}
function dynamicWidths(p){
    return {root:p.toString(),user_node_width_0x3398:p.add(0x3398).readS32(),
        user_element_width_0x33a8:p.add(0x33a8).readS32(),
        user_field_metadata:dynamicUserMetadata(p),
        components:Array.from({length:79},(_,i)=>p.add(0x15f8+i*4).readS32())};
}
function dynamicBuffer(c){
    let physical,source,detail=null;
    const owned=dynamicDescriptor(c.object,c.component);
    if(owned.storage_pointer===c.buffer.toString()){
        physical=owned.length;source='registered-descriptor-begin-end';detail=owned;
    }else{
        const saved=dynamicAllocations.get(c.buffer.toString());
        if(!saved || saved.freed)throw new Error('dynamic destination has no observed live allocation');
        if(saved.bytes%8)throw new Error('dynamic temporary allocation is not doubles');
        physical=saved.bytes/8;source='observed-calloc';detail=saved;
    }
    if(!Number.isInteger(physical) || physical<0 || physical>128)throw new Error('dynamic physical bound');
    const logical=c.count*c.width;
    const valid=Number.isInteger(logical) && logical>=0 && logical<=physical;
    return {buffer:c.buffer.toString(),physical_items:physical,extent_source:source,allocation:detail,
        logical_items:logical,logical_extent_valid:valid,
        values:physical?readArray(c.buffer,physical,false):[],values_hex:bytes(c.buffer,physical*8),
        logical_values:valid?(logical?readArray(c.buffer,logical,false):[]):null,
        logical_values_hex:valid?bytes(c.buffer,logical*8):null};
}
function dynamicReaderState(c){
    const width=c.widthPointer?c.widthPointer.readS32():c.width;
    return {serial:c.serial,parent_serial:c.parent_serial??null,id:c.id,keyword:c.keyword,kind:c.kind,
        object:c.object.toString(),component:c.component,domain:c.domain,mode:c.mode??null,
        count:c.count,width_before:c.width,width_current:width,declared_rows:c.declared??null,
        explicit_width:c.explicit??null,stream:stream(c.file),root_controls:dynamicWidths(c.root),
        ...dynamicBuffer({...c,width})};
}
Interceptor.attach(at(0x480e14),{
    onEnter(args){
        if(!dynamicActive() || activeHandler.id!==170 || activeHandler.root.add(0x1c).readS32()!==2)return;
        this.capture=safe(()=>({serial:++dynamicSerial,count:args[0].toInt32(),size:args[1].toInt32(),
            return_rva:this.returnAddress.sub(app.base).toInt32(),object:dynamicObject().toString()}));
    },
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture,total=c.count*c.size;
        if(c.count<0 || c.size!==8 || total>1024)throw new Error('dynamic calloc bound');
        const pointer=ptr(result.toString());
        if(!pointer.isNull())dynamicAllocations.set(pointer.toString(),{...c,bytes:total,pointer:pointer.toString(),freed:false});
        emit('dynamic-allocation',{...c,bytes:total,pointer:pointer.toString()});
    });}
});
Interceptor.attach(at(0x479910),{
    onEnter(args){
        const key=args[0].toString(),c=dynamicAllocations.get(key);
        if(c && !c.freed){c.freed=true;if(currentFile!==null)emit('dynamic-free',{pointer:key,allocation_serial:c.serial});}
    }
});
Interceptor.attach(at(0x415590),{
    onEnter(args){
        if(!dynamicActive())return;
        this.capture=safe(()=>({serial:++dynamicSerial,id:activeHandler.id,kind:'dynamic-header',
            file:args[0],stream:args[0],before:stream(args[0]),
            pointers:[args[1],args[2],args[3]],lines:[],scans:[]}));
        if(this.capture)realStack().push(this.capture);
    },
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture,conversions=c.scans.length?c.scans.at(-1).conversions:-1;
        // 415590 initializes all three destinations before fgets, including EOF.
        emit('dynamic-header',{serial:c.serial,id:c.id,kind:c.kind,status:result.toInt32(),
            conversions,before:c.before,after:stream(c.file),lines:c.lines,scans:c.scans,
            values:c.pointers.map((p,i)=>i===1?p.readDouble():p.readS32()),
            output_hex:c.pointers.map((p,i)=>bytes(p,i===1?8:4))});
        if(realStack().pop()!==c)throw new Error('dynamic header nesting');
    });}
});
Interceptor.attach(at(0x416c90),{
    onEnter(args){
        if(!dynamicActive())return;
        this.capture=safe(()=>{
            const c={...dynamicDefinitions[activeHandler.id],serial:++dynamicSerial,id:activeHandler.id,
                kind:'dynamic-field',root:activeHandler.root,object:dynamicObject(),file:args[0],stream:args[0],
                buffer:args[1],count:args[2].toInt32(),widthPointer:args[3],width:args[3].readS32(),
                mode:args[4].toInt32(),lines:[],scans:[]};
            dynamicReaders.push(c);realStack().push(c);emit('dynamic-reader-before',dynamicReaderState(c));return c;
        });
    },
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture;emit('dynamic-reader-after',{...dynamicReaderState(c),status:result.toInt32(),lines:c.lines,scans:c.scans});
        if(dynamicReaders.pop()!==c || realStack().pop()!==c)throw new Error('dynamic reader nesting');
    });}
});
for(const [rva,kind] of [[0x4199a0,'implicit'],[0x419b90,'explicit']])Interceptor.attach(at(rva),{
    onEnter(args){
        if(!dynamicActive())return;
        this.capture=safe(()=>{
            const c={...dynamicDefinitions[activeHandler.id],serial:++dynamicSerial,id:activeHandler.id,
                parent_serial:dynamicReaders.at(-1)?.serial??null,kind,root:activeHandler.root,object:dynamicObject(),
                file:args[0],buffer:args[1],declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32(),
                explicit:kind==='explicit'?args[5].toInt32():0};
            if(c.count<0 || c.count>8 || c.width<0 || c.width>10 || c.declared<0 || c.declared>8 || c.explicit<0 || c.explicit>10)
                throw new Error('dynamic detailed reader bound');
            dynamicReaders.push(c);emit('dynamic-rows-before',dynamicReaderState(c));return c;
        });
    },
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture;emit('dynamic-rows-after',{...dynamicReaderState(c),status:result.toInt32()});
        if(dynamicReaders.pop()!==c)throw new Error('dynamic row nesting');
    });}
});
Interceptor.attach(at(0x47f898),{
    onEnter(args){if(dynamicReaders.length)this.capture={reader:dynamicReaders.at(-1),buffer:args[0],limit:args[1].toInt32(),file:args[2],before:safe(()=>stream(args[2]))};},
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture;emit('dynamic-line',{reader_serial:c.reader.serial,kind:c.reader.kind,limit:c.limit,
            eof:result.isNull(),hex:result.isNull()?null:boundedCStringHex(c.buffer,c.limit),before:c.before,after:stream(c.file)});
    });}
});
for(const [rva,component,offset,hasOption] of [[0x1815b0,17,0x8a0,false],[0x184a30,60,0x9e0,false],[0x184150,46,0x9e0,true],[0x181190,24,0x8a0,true]]){
    Interceptor.attach(at(rva),{
        onEnter(args){
            if(!dynamicActive())return;
            this.capture=safe(()=>({serial:++dynamicSerial,rva,component,object:args[0].sub(offset),
                option:hasOption?args[1].readU8():null}));
            if(this.capture)safe(()=>emit('dynamic-getter-before',{serial:this.capture.serial,rva,component,
                object:this.capture.object.toString(),option:this.capture.option,
                descriptor:dynamicDescriptor(this.capture.object,component)}));
        },
        onLeave(result){if(this.capture)safe(()=>{
            const c=this.capture;emit('dynamic-getter-after',{serial:c.serial,rva,component,object:c.object.toString(),
                pointer:ptr(result.toString()).toString(),option:c.option,descriptor:dynamicDescriptor(c.object,component)});
        });}
    });
}
for(const rva of [0x1d21c0,0x1d2090])Interceptor.attach(at(rva),{
    onEnter(args){
        if(!dynamicActive())return;
        this.capture={serial:++dynamicSerial,rva,root:args[0],requested:args[1].toInt32()};
        safe(()=>emit('dynamic-width-before',{serial:this.capture.serial,rva,requested:this.capture.requested,...dynamicWidths(args[0])}));
    },
    onLeave(){if(this.capture)safe(()=>emit('dynamic-width-after',{serial:this.capture.serial,rva,
        requested:this.capture.requested,...dynamicWidths(this.capture.root)}));}
});
function dynamicMetadataList(root,kind,countOverride=null){
    const nodes=kind==='nodes',logical=root.add(nodes?0x3398:0x33a8).readS32();
    const pointer=root.add(nodes?0x33a0:0x33b0).readPointer();
    const saved=dynamicMetadata.get(root.toString()+':'+kind);
    const capacity=countOverride??(saved && saved.pointer===pointer.toString()?saved.capacity:null);
    if(pointer.isNull())return {logical_count:logical,pointer:pointer.toString(),capacity:0,allocation_observed:true,names:[]};
    if(capacity===null)return {logical_count:logical,pointer:pointer.toString(),capacity:null,allocation_observed:false,names:null};
    if(capacity<0 || capacity>8)throw new Error('dynamic metadata capacity bound');
    return {logical_count:logical,pointer:pointer.toString(),capacity,allocation_observed:true,
        names:Array.from({length:capacity},(_,i)=>{
            const p=pointer.add(i*8).readPointer();
            if(p.isNull())throw new Error('null user field name allocation');
            // Resize helper calloc(64,1) initializes the full name allocation.
            return {pointer:p.toString(),name_hex:boundedCStringHex(p,64),bytes_hex:bytes(p,64)};
        })};
}
function dynamicUserMetadata(root){return {nodes:dynamicMetadataList(root,'nodes'),elements:dynamicMetadataList(root,'elements')};}
for(const [rva,kind] of [[0x1cba80,'nodes'],[0x1cb7d0,'elements']])Interceptor.attach(at(rva),{
    onEnter(args){
        if(currentFile===null)return;
        this.capture=safe(()=>({serial:++dynamicSerial,rva,kind,root:args[0],oldCount:args[1].readS32(),newPointer:args[2]}));
        if(this.capture)safe(()=>emit('dynamic-metadata-before',{serial:this.capture.serial,rva,kind,
            old_count:this.capture.oldCount,requested_count:this.capture.newPointer.readS32(),
            root:this.capture.root.toString(),metadata:dynamicMetadataList(this.capture.root,kind)}));
    },
    onLeave(){if(this.capture)safe(()=>{
        const c=this.capture,capacity=c.newPointer.readS32(),pointer=c.root.add(kind==='nodes'?0x33a0:0x33b0).readPointer();
        if(capacity<0 || capacity>8)throw new Error('dynamic metadata new capacity bound');
        dynamicMetadata.set(c.root.toString()+':'+kind,{capacity,pointer:pointer.toString()});
        emit('dynamic-metadata-after',{serial:c.serial,rva,kind,old_count:c.oldCount,requested_count:capacity,
            root:c.root.toString(),metadata:dynamicMetadataList(c.root,kind,capacity)});
    });}
});
for(const [rva,kind] of [[0x1cb910,'nodes'],[0x1cb660,'elements']])Interceptor.attach(at(rva),{
    onEnter(args){
        if(currentFile===null)return;
        this.root=ptr(args[0].toString());this.serial=++dynamicSerial;
        safe(()=>emit('dynamic-metadata-clear-before',{serial:this.serial,rva,kind,root:this.root.toString(),metadata:dynamicMetadataList(this.root,kind)}));
    },
    onLeave(){if(this.root)safe(()=>{
        dynamicMetadata.set(this.root.toString()+':'+kind,{capacity:0,pointer:this.root.add(kind==='nodes'?0x33a0:0x33b0).readPointer().toString()});
        emit('dynamic-metadata-clear-after',{serial:this.serial,rva,kind,root:this.root.toString(),metadata:dynamicMetadataList(this.root,kind,0)});
    });}
});
Interceptor.attach(at(0x1d48d0),{
    onEnter(args){
        if(!dynamicActive())return;
        this.capture={serial:++dynamicSerial,root:ptr(args[0].toString()),component:args[1].readS32()};
        safe(()=>emit('dynamic-property-before',{serial:this.capture.serial,root:this.capture.root.toString(),component:this.capture.component}));
    },
    onLeave(){if(this.capture)emit('dynamic-property-after',{serial:this.capture.serial,root:this.capture.root.toString(),component:this.capture.component});}
});
Interceptor.attach(at(0x1d2b70),{
    onEnter(args){if(dynamicActive())this.root=ptr(args[0].toString());},
    onLeave(result){if(this.root)emit('dynamic-property-guard',{root:this.root.toString(),value:result.toInt32()&255});}
});
function customActive(){return currentFile!==null && activeHandler && activeHandler.category===2 && [198,266].includes(activeHandler.id);}
function customVector(handle,integers){
    const block=handle.readPointer();
    if(block.isNull())throw new Error('null custom vector block');
    const begin=block.add(8).readPointer(),end=block.add(0x10).readPointer(),capacity=block.add(0x18).readPointer();
    const size=integers?4:8,physical=end.sub(begin).toInt32()/size,allocated=capacity.sub(begin).toInt32()/size;
    if(!Number.isInteger(physical) || !Number.isInteger(allocated) || physical<0 || physical>64 || allocated<physical || allocated>128)
        throw new Error('custom vector physical bound');
    return {handle:handle.toString(),block:block.toString(),references:block.readU32(),pointer:begin.toString(),
        end:end.toString(),capacity_end:capacity.toString(),physical_items:physical,capacity_items:allocated,
        values:physical?readArray(begin,physical,integers):[],values_hex:bytes(begin,physical*size)};
}
function customEntry(p){
    return {address:p.toString(),vtable_rva:p.readPointer().sub(app.base).toInt32(),id:p.add(8).readS32(),
        custom_id:p.add(0x38).readS32(),kind:p.add(0xc).readS32(),
        shape:{count:p.add(0x10).readS32(),width:p.add(0x14).readS32(),multiplicity:p.add(0x18).readS32()},
        previous:{count:p.add(0x1c).readS32(),width:p.add(0x20).readS32(),multiplicity:p.add(0x24).readS32()},
        bound_slot:p.add(0x30).readPointer().toString(),storage:customVector(p.add(0x28),false)};
}
function customFieldStore(object){
    const container=object.add(0xb58),block=container.readPointer();
    if(block.isNull())throw new Error('null custom sentinel owner');
    const sentinel=block.add(8).readPointer(),count=Number(block.add(0x10).readU64().toString());
    if(count<0 || count>4)throw new Error('custom list count bound');
    const entries=[];let node=sentinel.readPointer(),previous=sentinel;
    for(let i=0;i<count;++i){
        if(node.isNull() || node.equals(sentinel))throw new Error('short custom list');
        if(!node.add(8).readPointer().equals(previous))throw new Error('custom previous-link mismatch');
        const entry=node.add(0x10).readPointer();if(entry.isNull())throw new Error('null custom entry');
        entries.push({node:node.toString(),previous_node:node.add(8).readPointer().toString(),next_node:node.readPointer().toString(),...customEntry(entry)});
        previous=node;node=node.readPointer();
    }
    if(!node.equals(sentinel) || !sentinel.add(8).readPointer().equals(previous))throw new Error('custom list terminal links');
    return {container:container.toString(),block:block.toString(),references:block.readU32(),sentinel:sentinel.toString(),count,
        root:container.add(8).readPointer().toString(),object:container.add(0x10).readPointer().toString(),entries};
}
function customDynamicEntry(p,index=null){
    return {address:p.toString(),index,vtable_rva:p.readPointer().sub(app.base).toInt32(),next:p.add(0x10).readPointer().toString(),
        source_0x0c:p.add(0xc).readS32(),type_0x18:p.add(0x18).readS32(),integer_0x1c:p.add(0x1c).readU8(),
        discriminator_0x20:p.add(0x20).readS32(),count_0x24:p.add(0x24).readS32(),width_0x28:p.add(0x28).readS32(),
        real_storage:customVector(p.add(0x30),false),integer_storage:customVector(p.add(0x38),true)};
}
function customDynamicStore(object){
    const container=object.add(0xba8),count=container.add(0x18).readS32(),head=container.add(8).readPointer(),tail=container.add(0x10).readPointer();
    customDynamicContainers.set(container.toString(),ptr(object.toString()));
    if(count<0 || count>4)throw new Error('dynamic custom list count bound');
    const entries=[];let p=head,last=ptr(0);
    for(let i=0;i<count;++i){if(p.isNull())throw new Error('short dynamic custom list');entries.push(customDynamicEntry(p,i+1));last=p;p=p.add(0x10).readPointer();}
    if(!p.isNull() || !last.equals(tail))throw new Error('dynamic custom terminal links');
    return {container:container.toString(),count,head:head.toString(),tail:tail.toString(),entries};
}
function customPayload(){
    if(!customActive() || !activeHandler.customPayload)throw new Error('missing custom selected payload');
    return activeHandler.customPayload;
}
function customReaderState(c){
    const storage=c.id===198?customVector(c.payload.add(0x28),false):customVector(c.payload.add(0x30),false);
    if(storage.pointer!==c.buffer.toString())throw new Error('custom row pointer differs from observed payload storage');
    const logical=c.count*c.width;if(logical<0 || logical>storage.physical_items)throw new Error('custom row logical extent exceeds physical storage');
    return {serial:c.serial,id:c.id,kind:c.kind,payload:c.payload.toString(),declared_rows:c.declared,count:c.count,width:c.width,
        stream:stream(c.file),storage,logical_values:logical?readArray(c.buffer,logical,false):[],logical_values_hex:bytes(c.buffer,logical*8)};
}
Interceptor.attach(at(0x416700),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==198)return;
        this.entry={serial:++customSerial,kind:'custom-header',file:args[0],stream:args[0],before:safe(()=>stream(args[0])),
            pointers:[args[1],args[2],args[3],args[4],args[5]],lines:[],scans:[]};realStack().push(this.entry);
    },
    onLeave(result){if(this.entry)safe(()=>{
        const c=this.entry;emit('custom-header',{serial:c.serial,kind:c.kind,status:result.toInt32(),before:c.before,after:stream(c.file),
            values:c.pointers.map((p,i)=>i===3?p.readDouble():p.readS32()),output_hex:c.pointers.map((p,i)=>bytes(p,i===3?8:4)),lines:c.lines,scans:c.scans});
        if(realStack().pop()!==c)throw new Error('custom header nesting');
    });}
});
Interceptor.attach(at(0x185d20),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==198)return;
        this.capture={serial:++customSerial,container:args[0],custom_id:args[1].toInt32(),kind:args[2].toInt32(),width:args[3].toInt32(),rows:args[4].toInt32()};
        safe(()=>emit('custom-configure-before',{...this.capture,container:this.capture.container.toString(),store:customFieldStore(dynamicObject())}));
    },
    onLeave(result){if(this.capture)safe(()=>{
        const p=ptr(result.toString());activeHandler.customPayload=p;
        emit('custom-configure-after',{serial:this.capture.serial,entry:customEntry(p),store:customFieldStore(dynamicObject())});
    });}
});
Interceptor.attach(at(0x185e00),{
    onEnter(args){if(customActive() && activeHandler.id===198)this.capture={container:args[0].toString(),custom_id:args[1].toInt32()};},
    onLeave(result){if(this.capture)emit('custom-lookup',{...this.capture,pointer:ptr(result.toString()).toString()});}
});
Interceptor.attach(at(0x186010),{
    onEnter(args){if(customActive() && activeHandler.id===198)this.capture={container:args[0].toString(),custom_id:args[1].toInt32()};},
    onLeave(result){if(this.capture)safe(()=>emit('custom-created',{...this.capture,entry:customEntry(ptr(result.toString()))}));}
});
Interceptor.attach(at(0x301620),{
    onEnter(args){if(customActive() && activeHandler.id===198){this.payload=ptr(args[0].toString());safe(()=>emit('custom-default-before',{entry:customEntry(this.payload)}));}},
    onLeave(){if(this.payload)safe(()=>emit('custom-default-after',{entry:customEntry(this.payload)}));}
});
Interceptor.attach(at(0x4199a0),{
    onEnter(args){
        if(!customActive())return;
        safe(()=>{
            const c={serial:++customSerial,id:activeHandler.id,kind:'custom-implicit',file:args[0],buffer:args[1],payload:customPayload(),
                declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32(),thread_id:this.threadId};
            if(c.declared<0 || c.declared>8 || c.count<0 || c.count>8 || c.width<0 || c.width>8)throw new Error('custom row bound');
            const snapshot=customReaderState(c),frames=customRowInvocations.get(this.threadId)??[];
            frames.push(c);customRowInvocations.set(this.threadId,frames);customReaders.push(c);
            emit('custom-rows-before',snapshot);
        });
    }
});
// Native caller continuations establish completion even when the helper's
// Interceptor onLeave callback is not delivered for a positive-row invocation.
// These are the two retained call instructions' exact fall-through addresses.
for(const [rva,id] of [[0x397e46,198],[0x397760,266]])Interceptor.attach(at(rva),{
    onEnter(){
        if(!customActive() || activeHandler.id!==id)return;
        const frames=customRowInvocations.get(this.threadId),c=frames?.at(-1);
        if(!c){safe(()=>{throw new Error('custom row continuation has no invocation frame');});return;}
        safe(()=>{
            try{
                if(c.id!==id)throw new Error('custom row continuation handler mismatch');
                emit('custom-rows-after',{...customReaderState(c),status:this.context.rax.toInt32(),completion_rva:rva});
            }
            finally{
                frames.pop();if(!frames.length)customRowInvocations.delete(this.threadId);
                if(customReaders.pop()!==c)throw new Error('custom row nesting');
            }
        });
    }
});
Interceptor.attach(at(0x47f898),{
    onEnter(args){if(customReaders.length)this.capture={reader:customReaders.at(-1),buffer:args[0],limit:args[1].toInt32(),file:args[2],before:safe(()=>stream(args[2]))};},
    onLeave(result){if(this.capture)safe(()=>{
        const c=this.capture;emit('custom-line',{reader_serial:c.reader.serial,kind:c.reader.kind,limit:c.limit,eof:result.isNull(),
            hex:result.isNull()?null:boundedCStringHex(c.buffer,c.limit),before:c.before,after:stream(c.file)});
    });}
});
Interceptor.attach(at(0x32f630),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==198)return;
        this.capture={target:ptr(args[0].toString()),value:args[1].toInt32(),root:activeHandler.root};
        safe(()=>emit('custom-root-update-before',{target:this.capture.target.toString(),root:this.capture.root.toString(),value:this.capture.value,previous:this.capture.target.readS32()}));
    },
    onLeave(){if(this.capture)safe(()=>emit('custom-root-update-after',{target:this.capture.target.toString(),root:this.capture.root.toString(),value:this.capture.target.readS32()}));}
});
Interceptor.attach(at(0x413900),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==266)return;
        this.entry={serial:++customSerial,kind:'dynamic-token-header',file:args[0],stream:args[0],countPointer:args[1],limit:args[2].toInt32(),
            before:safe(()=>stream(args[0])),lines:[],scans:[]};realStack().push(this.entry);
    },
    onLeave(result){if(this.entry)safe(()=>{
        const c=this.entry,status=result.toInt32(),count=status===0?c.countPointer.readS32():null;
        if(c.limit!==10 || (count!==null && (count<0 || count>10)))throw new Error('dynamic token header bound');
        const tokens=status===0?Array.from({length:count},(_,i)=>{
            const p=at(0x70cc60).add(i*8).readPointer();return p.isNull()?null:boundedCStringHex(p,1024);
        }):null;
        emit('custom-token-header',{serial:c.serial,status,count,limit:c.limit,tokens_hex:tokens,before:c.before,after:stream(c.file),lines:c.lines});
        if(realStack().pop()!==c)throw new Error('custom token header nesting');
    });}
});
Interceptor.attach(at(0x413a50),{
    onEnter(args){if(customActive() && activeHandler.id===266)this.index=args[0].toInt32();},
    onLeave(result){if(this.index!==undefined)safe(()=>emit('custom-token-access',{index:this.index,pointer:ptr(result.toString()).toString(),value_hex:boundedCStringHex(ptr(result.toString()),1024)}));}
});
Interceptor.attach(at(0x47b6c0),{
    onEnter(args){if(customActive() && activeHandler.id===266)this.input=safe(()=>boundedCStringHex(args[0],1024));},
    onLeave(result){if(this.input!==undefined)emit('custom-token-integer',{input_hex:this.input,value:result.toInt32()});}
});
Interceptor.attach(at(0x48754c),{
    onEnter(args){if(customActive() && activeHandler.id===266)safe(()=>emit('custom-token-real-input',{input_hex:boundedCStringHex(args[0],1024)}));}
});
Interceptor.attach(at(0x25a990),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==266)return;
        this.entry={serial:++customSerial,container:ptr(args[0].toString()),index:args[1].toInt32()};
        safe(()=>emit('custom-dynamic-lookup-before',{serial:this.entry.serial,index:this.entry.index,store:customDynamicStore(dynamicObject())}));
    },
    onLeave(result){if(this.entry)safe(()=>emit('custom-dynamic-lookup-after',{serial:this.entry.serial,index:this.entry.index,
        pointer:ptr(result.toString()).toString(),store:customDynamicStore(dynamicObject())}));}
});
Interceptor.attach(at(0x25b1d0),{
    onEnter(args){if(customActive() && activeHandler.id===266)this.input=ptr(args[0].toString());},
    onLeave(result){if(this.input){const p=ptr(result.toString());activeHandler.customPayload=p;emit('custom-dynamic-cast',{input:this.input.toString(),payload:p.toString()});}}
});
Interceptor.attach(at(0x25b220),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==266)return;
        this.entry={serial:++customSerial,payload:ptr(args[0].toString()),source:args[1].toInt32(),type:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32()};
        safe(()=>emit('custom-dynamic-configure-before',{serial:this.entry.serial,requested:{source:this.entry.source,type:this.entry.type,count:this.entry.count,width:this.entry.width},entry:customDynamicEntry(this.entry.payload)}));
    },
    onLeave(){if(this.entry)safe(()=>emit('custom-dynamic-configure-after',{serial:this.entry.serial,entry:customDynamicEntry(this.entry.payload)}));}
});
Interceptor.attach(at(0x25b580),{
    onEnter(args){if(customActive() && activeHandler.id===266){this.payload=ptr(args[0].toString());safe(()=>emit('custom-dynamic-default-before',{entry:customDynamicEntry(this.payload)}));}},
    onLeave(){if(this.payload)safe(()=>emit('custom-dynamic-default-after',{entry:customDynamicEntry(this.payload)}));}
});
for(const [rva,kind,width,integers] of [[0x415680,'real2',2,false],[0x415880,'real3',3,false],
    [0x414100,'int3',3,true],[0x414200,'int4',4,true],[0x414600,'int8',8,true]])Interceptor.attach(at(rva),{
    onEnter(args){
        if(!customActive() || activeHandler.id!==266)return;
        this.entry=safe(()=>{
            const p=customPayload(),storage=customVector(p.add(integers?0x38:0x30),integers),size=integers?4:8;
            const pointers=Array.from({length:width+1},(_,i)=>args[i+1]);
            const offset=pointers[1].sub(ptr(storage.pointer)).toInt32();
            if(offset<0 || offset%size || offset/size+width>storage.physical_items)throw new Error('dynamic typed row physical range');
            for(let i=1;i<=width;++i)if(!pointers[i].equals(pointers[1].add((i-1)*size)))throw new Error('dynamic typed row destination stride');
            const c={serial:++customSerial,kind,width,integers,payload:p,stream:args[0],before:stream(args[0]),
                pointers,offset:offset/size,lines:[],scans:[]};
            realStack().push(c);emit('custom-typed-row-before',{serial:c.serial,kind,width,integers,payload:p.toString(),offset:c.offset,before:c.before,storage});return c;
        });
    },
    onLeave(result){if(this.entry)safe(()=>{
        const c=this.entry;emit('custom-typed-row-after',{serial:c.serial,kind,width,integers,payload:c.payload.toString(),offset:c.offset,
            status:result.toInt32(),before:c.before,after:stream(c.stream),lines:c.lines,scans:c.scans,
            values:c.pointers.map((p,i)=>i===0 || integers?p.readS32():p.readDouble()),
            output_hex:c.pointers.map((p,i)=>bytes(p,i===0 || integers?4:8)),
            storage:customVector(c.payload.add(integers?0x38:0x30),integers)});
        if(realStack().pop()!==c)throw new Error('custom typed row nesting');
    });}
});
Interceptor.attach(at(0x185ca0),{
    onEnter(args){
        if(currentFile===null)return;
        this.object=ptr(args[0].toString()).sub(0xb58);
        safe(()=>emit('custom-store-destroy-before',{object:this.object.toString(),store:customFieldStore(this.object)}));
    },
    onLeave(){if(this.object)emit('custom-store-destroy-after',{object:this.object.toString(),destroyed:true});}
});
Interceptor.attach(at(0x25ab20),{
    onEnter(args){
        if(currentFile===null)return;
        const object=customDynamicContainers.get(args[0].toString());if(!object)return;
        this.object=ptr(object.toString());safe(()=>emit('custom-dynamic-clear-before',{object:this.object.toString(),store:customDynamicStore(this.object)}));
    },
    onLeave(){if(this.object)safe(()=>emit('custom-dynamic-clear-after',{object:this.object.toString(),store:customDynamicStore(this.object)}));}
});
// A destroyed object must not leave a stale container-address classification.
Interceptor.attach(at(0x15f790),{
    onEnter(args){this.object=ptr(args[0].toString());},
    onLeave(){customDynamicContainers.delete(this.object.add(0xba8).toString());}
});

const shapeDefinitions={210:{component:54,slot:0xa80},211:{component:55,slot:0xa88}};
function shapeActive(){return currentFile!==null && activeHandler && activeHandler.category===2 && shapeDefinitions[activeHandler.id];}
function shapeState(c){
    const d=dynamicDescriptor(c.object,c.component);
    return {serial:c.serial,id:c.id,component:c.component,object:c.object.toString(),count:c.count,
        root_width:c.widthPointer.readS32(),saved_shape_width_0xadc:c.object.add(0xadc).readS32(),
        saved_texture_width_0xae0:c.object.add(0xae0).readS32(),descriptor:d,stream:stream(c.file)};
}
function shapeMaterialActive(){return currentFile!==null && activeHandler && activeHandler.category===4 && (activeHandler.id===127 || activeHandler.id===128);}
for(const [rva,id,width,offset] of [[0x413de0,127,1,0x2150],[0x413fb0,128,3,0x2154]])Interceptor.attach(at(rva),{
    onEnter(args){
        if(!shapeMaterialActive() || activeHandler.id!==id)return;
        this.shapeHeader=safe(()=>{
            const material=ptr(args[1].toString()).sub(offset);
            const c={serial:++shapeSerial,id,kind:width===1?'int1':'int3',file:args[0],stream:args[0],material,
                pointers:Array.from({length:width},(_,i)=>args[i+1]),before:stream(args[0]),before_material:materialRecord(material),lines:[],scans:[]};
            if(id===127 && args[2].toInt32()!==0)throw new Error('material shape integer skip flag');
            if(id===128 && !args[4].isNull())throw new Error('material shape triple optional count differs');
            realStack().push(c);return c;
        });
    },
    onLeave(result){if(this.shapeHeader)safe(()=>{
        const c=this.shapeHeader;
        try{emit('material-shape-producer-header',{serial:c.serial,id:c.id,kind:c.kind,material:c.material.toString(),
            status:result.toInt32(),before:c.before,after:stream(c.file),before_material:c.before_material,after_material:materialRecord(c.material),
            values:c.pointers.map(p=>p.readS32()),output_hex:c.pointers.map(p=>bytes(p,4)),lines:c.lines,scans:c.scans});}
        finally{if(realStack().pop()!==c)throw new Error('material shape header stack');}
    });}
});
Interceptor.attach(at(0x1d64f0),{
    onEnter(args){
        const feature=args[1].toInt32();if(currentFile===null || (feature!==54 && feature!==55))return;
        this.shapeQuery={root:ptr(args[0].toString()),feature,output:ptr(args[2].toString())};
        safe(()=>emit('material-shape-query-before',{root:this.shapeQuery.root.toString(),feature,
            output_pointer:this.shapeQuery.output.toString(),materials:materialList(this.shapeQuery.root.add(0x1328)),
            root_width:this.shapeQuery.root.add(0x15f8+feature*4).readS32()}));
    },
    onLeave(result){if(this.shapeQuery)safe(()=>{
        const c=this.shapeQuery,enabled=(result.toInt32()&255)!==0;
        emit('material-shape-query-after',{root:c.root.toString(),feature:c.feature,enabled,
            output_pointer:c.output.toString(),output:enabled && !c.output.isNull()?c.output.readS32():null});
    });}
});
Interceptor.attach(at(0x417c80),{
    onEnter(args){
        if(!shapeActive())return;
        safe(()=>{
            if(shapeInvocations.has(this.threadId))throw new Error('nested material shape reader');
            const def=shapeDefinitions[activeHandler.id],c={serial:++shapeSerial,id:activeHandler.id,kind:'material-shape-mode4',
                file:args[0],stream:args[0],object:ptr(args[1].toString()),component:args[2].toInt32(),count:args[3].toInt32(),
                widthPointer:ptr(args[4].toString()),mode:args[5].toInt32(),lines:[],scans:[]};
            if(c.component!==def.component || c.mode!==4 || !c.object.equals(dynamicObject()) ||
                !c.widthPointer.equals(activeHandler.root.add(0x15f8+c.component*4)) || c.count!==c.object.add(0x884).readS32() || c.count<0 || c.count>3)
                throw new Error('material shape reader ABI/count');
            const state=shapeState(c);shapeInvocations.set(this.threadId,c);shapeReaders.push(c);realStack().push(c);
            emit('material-shape-reader-before',state);
        });
    }
});
Interceptor.attach(at(0x47c354),{
    onEnter(args){
        const c=shapeReaders.at(-1);if(!c)return;
        this.shapeScan={parent:c,format:args[1].readCString(),input_hex:safe(()=>boundedCStringHex(args[0],1024)),
            pointers:[args[2],args[3],args[4],args[5]]};
    },
    onLeave(result){if(this.shapeScan)safe(()=>{
        const c=this.shapeScan;if(c.format!=='%i %i %i %Flf')throw new Error('material shape mixed header format');
        emit('material-shape-header-scan',{reader_serial:c.parent.serial,format:c.format,input_hex:c.input_hex,
            conversions:result.toInt32(),values:c.pointers.map((p,i)=>i===3?p.readDouble():p.readS32()),
            output_hex:c.pointers.map((p,i)=>bytes(p,i===3?8:4))});
    });}
});
Interceptor.attach(at(0x419b90),{
    onEnter(args){
        if(!shapeActive())return;
        safe(()=>{
            const parent=shapeReaders.at(-1);if(!parent || shapeRowInvocations.has(this.threadId))throw new Error('material shape detailed row parent');
            const c={...parent,serial:++shapeSerial,parent_serial:parent.serial,buffer:ptr(args[1].toString()),
                declared:args[2].toInt32(),count:args[3].toInt32(),width:args[4].toInt32(),explicit:args[5].toInt32()};
            const d=dynamicDescriptor(c.object,c.component);
            if(c.declared<0 || c.declared>8 || c.count<0 || c.count>3 || c.width<1 || c.width>10 || c.explicit!==c.width ||
                c.count*c.width>d.length || c.buffer.toString()!==d.storage_pointer)throw new Error('material shape detailed row physical bound');
            shapeRowInvocations.set(this.threadId,c);shapeRows.push(c);
            emit('material-shape-rows-before',{...shapeState(c),parent_serial:c.parent_serial,declared_rows:c.declared,width:c.width,explicit_width:c.explicit});
        });
    }
});
Interceptor.attach(at(0x41889c),{
    onEnter(){
        if(!shapeActive())return;
        safe(()=>{
            const c=shapeRowInvocations.get(this.threadId);if(!c)throw new Error('material shape row continuation has no frame');
            try{emit('material-shape-rows-after',{...shapeState(c),parent_serial:c.parent_serial,declared_rows:c.declared,
                width:c.width,explicit_width:c.explicit,status:this.context.rax.toInt32(),completion_rva:0x41889c});}
            finally{shapeRowInvocations.delete(this.threadId);if(shapeRows.pop()!==c)throw new Error('material shape row stack');}
        });
    }
});
for(const [rva,id] of [[0x39354f,210],[0x3935a0,211]])Interceptor.attach(at(rva),{
    onEnter(){
        if(!shapeActive() || activeHandler.id!==id)return;
        safe(()=>{
            const c=shapeInvocations.get(this.threadId);if(!c || c.id!==id)throw new Error('material shape reader continuation frame');
            try{emit('material-shape-reader-after',{...shapeState(c),status:this.context.rax.toInt32(),completion_rva:rva,lines:c.lines,scans:c.scans});}
            finally{
                shapeInvocations.delete(this.threadId);
                if(shapeReaders.pop()!==c || realStack().pop()!==c)throw new Error('material shape reader stack');
            }
        });
    }
});
for(const [rva,kind,component] of [[0x183840,'resize',null],[0x1845b0,'getter',54],[0x184640,'getter',55]])Interceptor.attach(at(rva),{
    onEnter(args){
        if(!shapeActive())return;
        this.shapeStorage=safe(()=>{
            const c={serial:++shapeSerial,kind,object:ptr(args[0].toString()).sub(0x9e0),
                component:component??args[1].toInt32(),requested_width:kind==='resize'?args[2].toInt32():null};
            if(c.component!==54 && c.component!==55)throw new Error('material shape storage component');
            emit('material-shape-storage-before',{...c,object:c.object.toString(),descriptor:dynamicDescriptor(c.object,c.component),saved_shape_width_0xadc:c.object.add(0xadc).readS32(),
                saved_texture_width_0xae0:c.object.add(0xae0).readS32()});return c;
        });
    },
    onLeave(result){if(this.shapeStorage)safe(()=>{
        const c=this.shapeStorage;emit('material-shape-storage-after',{...c,object:c.object.toString(),
            pointer:c.kind==='getter'?ptr(result.toString()).toString():null,descriptor:dynamicDescriptor(c.object,c.component),
            saved_shape_width_0xadc:c.object.add(0xadc).readS32(),saved_texture_width_0xae0:c.object.add(0xae0).readS32()});
    });}
});
send({operation:'hooks-installed',base:app.base.toString(),pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
