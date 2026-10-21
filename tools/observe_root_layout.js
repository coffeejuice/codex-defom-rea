// Passive captures of root dimension transitions and component-table producers.
// Substitute __READY_GATE__ with a private harness path before attaching.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0, currentFile = null, layoutSerial = 0;
const activeLayouts = [];
function emit(operation, data = {}) {
    send({sequence: ++sequence, operation, file: currentFile, ...data});
}
function safe(fn) {
    try { return fn(); }
    catch (error) { emit('probe-error', {error: String(error)}); return null; }
}
const scalarOffsets = [0x86c,0x870,0x874,0x878,0x87c,0x880,0x884,
    0x1320,0x13f4,0x3398,0x33a8,0x850,0x888,0x88c,0x890,
    0x33c8,0x33d8,0x11c4,0x123c,0x12b4];
function inputs(p) {
    const values = {};
    for (const offset of scalarOffsets) values['0x' + offset.toString(16)] = p.add(offset).readS32();
    return values;
}
function components(p) {
    return Array.from({length:79}, (_,i) => p.add(0x15f8 + i*4).readS32());
}
function root(p) {
    const count = p.add(0x1378).readS32();
    if (count < 0 || count > 8) throw new Error('object count bound');
    const objects = [];
    let q = p.add(0x1380).readPointer();
    for (let i=0;i<count;++i) {
        if (q.isNull()) throw new Error('short object list');
        objects.push({address:q.toString(), id:q.add(8).readS32(), type:q.add(12).readS32(),
            name:q.add(0x828).readCString(), nodes:q.add(0x880).readS32(),
            elements:q.add(0x884).readS32(), arity:q.add(0x894).readS32(),
            kind:q.add(0x890).readS32(), mesh:q.add(0x87c).readS32()});
        q = q.add(0x3940).readPointer();
    }
    return {address:p.toString(), dimension:p.add(0x1c).readS32(),
        components:components(p), scalars:inputs(p),
        material_count:p.add(0x1320).readS32(), object_count:count, objects,
        suppress_boundaries:p.add(0x1b5c).readU8(),
        alternate_root:p.add(0x33e8).readPointer().toString(),
        alternate_enabled:p.add(0x33f0).readU8()};
}
for (const [rva,kind] of [[0x1d09c0,'initialize'],[0x1d0320,'refresh']]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            this.capture = safe(() => ({
                serial:++layoutSerial, kind, address:args[0],
                dimension:kind==='initialize'?args[1].toInt32():args[0].add(0x1c).readS32(),
                scalars:inputs(args[0]), queries:[],
                thread:Process.getCurrentThreadId()
            }));
            if (this.capture) {
                activeLayouts.push(this.capture);
                emit('layout-enter',{serial:this.capture.serial,kind,root:args[0].toString(),
                    dimension:this.capture.dimension,scalars:this.capture.scalars});
            }
        },
        onLeave() {
            if (!this.capture) return;
            safe(() => {
                const c=this.capture;
                if (activeLayouts.pop()!==c) throw new Error('layout nesting mismatch');
                emit('layout-leave',{serial:c.serial,kind:c.kind,root:c.address.toString(),
                    dimension:c.dimension,scalars:c.scalars,queries:c.queries,
                    after_scalars:inputs(c.address),components:components(c.address)});
            });
        }
    });
}
Interceptor.attach(at(0x1d64f0), {
    onEnter(args) {
        this.capture=activeLayouts.at(-1);
        if (!this.capture || this.capture.thread!==Process.getCurrentThreadId() ||
            !this.capture.address.equals(args[0])) { this.capture=null; return; }
        this.id=args[1].toInt32(); this.output=args[2];
        this.before=this.output.isNull()?null:this.output.readS32();
    },
    onLeave(result) {
        if(this.capture) safe(() => this.capture.queries.push({
            id:this.id, enabled:result.toInt32()&255,
            width_before:this.before,width:this.output.isNull()?null:this.output.readS32()
        }));
    }
});
Interceptor.attach(at(0x1c8ba0), {
    onEnter(args) {
        this.root=args[0]; this.request=args[1].readS32();
        safe(() => emit('dimension-before',{request:this.request,state:root(this.root)}));
    },
    onLeave() {
        safe(() => emit('dimension-after',{request:this.request,state:root(this.root)}));
    }
});
Interceptor.attach(at(0x1cb440), {
    onEnter(args) {
        this.root=args[0];
        emit('root-clear-enter',{root:this.root.toString(),
            object_count:this.root.add(0x1378).readS32(),dimension:this.root.add(0x1c).readS32()});
    },
    onLeave() {
        emit('root-clear-leave',{root:this.root.toString(),
            object_count:this.root.add(0x1378).readS32(),dimension:this.root.add(0x1c).readS32()});
    }
});
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.root=args[0];this.previous=currentFile;currentFile=args[1].readCString();
        safe(() => emit('import-before',{state:root(this.root)}));
    },
    onLeave(result) {
        safe(() => emit('import-after',{status:result.toInt32(),state:root(this.root)}));
        currentFile=this.previous;
    }
});
send({operation:'hooks-installed',base:app.base.toString(),pid:Process.id});
const gate=new File('__READY_GATE__','w');gate.write('ready\n');gate.close();
