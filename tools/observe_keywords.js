// Passive observation: no application calls, argument changes, or memory writes.
// Caller substitutes __READY_GATE__ with the fresh harness gate path.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0;
let currentFile = null;
let currentKeyword = null;
function safe(fn) {
    try { return fn(); }
    catch (error) { send({operation: 'probe-error', error: String(error)}); return null; }
}
function bytes(p, n) { return Array.from(new Uint8Array(p.readByteArray(n))); }
function object(p) {
    const n = p.add(0x880).readS32(), e = p.add(0x884).readS32();
    const xyz = p.add(0x8a8).readPointer(), con = p.add(0x9e8).readPointer();
    const result = {
        address: p.toString(), id: p.add(8).readS32(),
        type: p.add(12).readS32(), name_bytes: bytes(p.add(0x828), 81),
        mesh_flag: p.add(0x87c).readS32(), nodes: n, elements: e,
        element_kind: p.add(0x890).readS32(), arity: p.add(0x894).readS32(),
        coordinates_pointer: xyz.toString(), connectivity_pointer: con.toString()
    };
    // This experiment supplies only 3D fixtures, at most sixteen nodes/elements.
    if (n > 0 && n <= 16 && !xyz.isNull())
        result.coordinates = Array.from({length: n * 3}, (_, i) => xyz.add(i * 8).readDouble());
    if (e > 0 && e <= 16 && result.arity > 0 && result.arity <= 8 && !con.isNull())
        result.connectivity = Array.from({length: e * result.arity},
            (_, i) => con.add(i * 4).readS32());
    for (const [offset, count] of [[0x8e0, n], [0xa30, e], [0xa38, e]]) {
        const values = p.add(offset).readPointer();
        if (count > 0 && count <= 16 && !values.isNull())
            result['array_' + offset.toString(16) + '_prefix'] =
                Array.from({length: count}, (_, i) => values.add(i * 8).readDouble());
    }
    return result;
}
function state(p) {
    const count = p.add(0x1378).readS32();
    const result = {address: p.toString(), object_count: count,
        list_count: p.add(0x1390).readS32(), objects: []};
    let item = p.add(0x1380).readPointer();
    const visited = new Set();
    for (let i = 0; i < Math.min(Math.max(count, 0), 16) && !item.isNull(); ++i) {
        if (visited.has(item.toString())) { result.cycle = true; break; }
        visited.add(item.toString());
        result.objects.push(object(item));
        item = item.add(0x3940).readPointer();
    }
    result.truncated = count > 16;
    return result;
}
function emit(operation, fields) {
    send({sequence: ++sequence, operation, file: currentFile,
        keyword: currentKeyword, ...fields});
}
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.model = args[0];
        this.previousFile = currentFile;
        currentFile = args[1].readCString();
        currentKeyword = null;
        safe(() => emit('import-before', {state: state(this.model)}));
    },
    onLeave(retval) {
        safe(() => emit('import-after', {status: retval.toInt32(), state: state(this.model)}));
        currentFile = this.previousFile;
    }
});
Interceptor.attach(at(0x2f76c0), {
    onEnter(args) { this.name = args[0].readCString(); this.category = args[1]; },
    onLeave(retval) {
        currentKeyword = this.name;
        safe(() => emit('keyword-resolved', {id: retval.toInt32(), category: this.category.readS32()}));
    }
});
for (const rva of [0x39aa90, 0x38b8a0, 0x388fb0, 0x384a50, 0x383120, 0x378e00]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            this.model = args[0]; this.id = args[2].toInt32();
            safe(() => emit('handler-before', {rva, id: this.id, state: state(this.model)}));
        },
        onLeave(retval) {
            safe(() => emit('handler-after', {rva, id: this.id, status: retval.toInt32(),
                state: state(this.model)}));
        }
    });
}
Interceptor.attach(at(0x19ccc0), {
    onEnter(args) { this.requested = args[1].toInt32(); },
    onLeave(retval) {
        safe(() => emit('object-resolved', {requested: this.requested,
            object: retval.isNull() ? null : object(retval)}));
    }
});
Interceptor.attach(at(0x1d7f40), {
    onEnter(args) {
        this.model = args[0];
        safe(() => emit('finalize-before', {state: state(this.model)}));
    },
    onLeave() { safe(() => emit('finalize-after', {state: state(this.model)})); }
});
send({operation: 'hooks-installed', base: app.base.toString(), pid: Process.id});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
