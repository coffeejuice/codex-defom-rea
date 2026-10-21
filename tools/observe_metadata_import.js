// Passive metadata observation of the original, through normal console imports.
// No application calls, argument changes, or writes to application memory.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let file = null, root = null, sequence = 0, active = null;
function emit(operation, fields) { send({sequence: ++sequence, operation, file, ...fields}); }
function safe(fn) { try { fn(); } catch (e) { emit('probe-error', {error: String(e)}); } }
function boundedText(p, capacity) {
    const b = new Uint8Array(p.readByteArray(capacity));
    const end = b.indexOf(0);
    if (end < 0) throw new Error('unterminated metadata');
    return Array.from(b.slice(0, end), c => String.fromCharCode(c)).join('');
}
function metadata(p) {
    return {title: boundedText(p.add(0x750), 81),
        operation_name: boundedText(p.add(0x7a1), 65),
        simulation_name: boundedText(p.add(0x7e2), 81),
        cursim: [0x834, 0x838, 0x83c].map(x => p.add(x).readS32())};
}
function state(p) {
    const linked = p.add(0x33e8).readPointer();
    return {metadata: {primary: metadata(p), linked: linked.isNull() ? null : metadata(linked)},
        dimensions: {primary: p.add(0x1c).readS32(),
            linked: linked.isNull() ? null : linked.add(0x1c).readS32()}};
}
function stream(p) {
    const pointer = p.readPointer(), base = p.add(0x10).readPointer();
    if (pointer.isNull() || base.isNull()) return {cursor: 0, buffered: false};
    const cursor = pointer.sub(base).toInt32();
    if (cursor < 0 || cursor > 4096) throw new Error('stream cursor outside bounded fixture');
    return {cursor, buffered: true};
}
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.previousFile = file; this.previousRoot = root;
        file = args[1].readCString(); root = ptr(args[0].toString()); this.root = root;
        safe(() => emit('import-before', state(root)));
    },
    onLeave(retval) {
        safe(() => emit('import-after', {status: retval.toInt32(), ...state(this.root)}));
        file = this.previousFile; root = this.previousRoot;
    }
});
Interceptor.attach(at(0x39aa90), {
    onEnter(args) {
        this.enabled = file !== null && args[2].toInt32() >= 22 && args[2].toInt32() <= 25;
        if (!this.enabled) return;
        this.previous = active;
        active = {id: args[2].toInt32(), filePointer: ptr(args[1].toString()),
            root: ptr(args[0].toString())};
        this.invocation = active;
        safe(() => emit('handler-before', {id: active.id, stream: stream(active.filePointer), ...state(active.root)}));
    },
    onLeave(retval) {
        if (!this.enabled) return;
        safe(() => emit('handler-after', {id: this.invocation.id, status: retval.toInt32(),
            stream: stream(this.invocation.filePointer), ...state(this.invocation.root)}));
        active = this.previous;
    }
});
for (const rva of [0x413ab0, 0x413fb0]) Interceptor.attach(at(rva), {
    onEnter(args) {
        this.enabled = active !== null;
        if (!this.enabled) return;
        this.filePointer = ptr(args[0].toString()); this.id = active.id;
        safe(() => emit('helper-before', {id: this.id, rva, stream: stream(this.filePointer),
            ...state(active.root)}));
    },
    onLeave(retval) {
        if (this.enabled) safe(() => emit('helper-after', {id: this.id, rva,
            status: retval.toInt32(), stream: stream(this.filePointer), ...state(root)}));
    }
});
send({operation: 'hooks-installed', pid: Process.id, base: app.base.toString()});
const gate = new File('__READY_GATE__', 'w'); gate.write('ready\n'); gate.close();
