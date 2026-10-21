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
    const vars = [];
    for (const entry of descriptors.values()) {
        if (entry.slot.compare(p.add(0x8b0)) >= 0 &&
            entry.slot.compare(p.add(0xac0)) <= 0) {
            vars.push(descriptor(entry.pointer, p));
        }
    }
    vars.sort((a, b) => a.slot_offset - b.slot_offset);
    return {
        address: p.toString(), id: p.add(8).readS32(),
        dimension: owner.add(0x1c).readS32(),
        mesh_flag: p.add(0x87c).readS32(), nodes: n, elements: e, arity,
        components: Array.from({length: 79}, (_, i) => owner.add(0x15f8 + i * 4).readS32()),
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
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.root = args[0]; this.previousFile = currentFile;
        currentFile = args[1].readCString();
        emit('import-before', {});
    },
    onLeave(retval) {
        safe(() => {
            const count = this.root.add(0x1378).readS32();
            if (count < 0 || count > 8) throw new Error('object bound: ' + count);
            const objects = [];
            let p = this.root.add(0x1380).readPointer();
            for (let i = 0; i < count && !p.isNull(); ++i) {
                objects.push(object(p)); p = p.add(0x3940).readPointer();
            }
            emit('import-after', {status: retval.toInt32(), object_count: count, objects});
        });
        currentFile = this.previousFile;
    }
});
// Allocation wrappers: do not read old buffers before replacement, since the
// caller may have already stored the new count while the old allocation exists.
for (const [rva, operation] of [[0x1652b0, 'nodes-replaced'], [0x1653b0, 'elements-replaced']]) {
    Interceptor.attach(at(rva), {
        onEnter(args) { this.objectPointer = args[0]; this.count = args[1].toInt32(); },
        onLeave() { safe(() => emit(operation, {requested_count: this.count,
            object: object(this.objectPointer)})); }
    });
}
send({operation: 'hooks-installed', base: app.base.toString(), pid: Process.id});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
