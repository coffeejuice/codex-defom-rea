// Passive observation of 2D directed-edge chains and object boundary groups.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0, currentFile = null;
function emit(operation, fields) {
    send({sequence: ++sequence, operation, file: currentFile, ...fields});
}
function safe(fn) {
    try { return fn(); }
    catch (error) { emit('probe-error', {error: String(error)}); return null; }
}
function integers(p, count) {
    if (count < 0 || count > 512) throw new Error('array bound: ' + count);
    if (p.isNull()) {
        if (count === 0) return [];
        throw new Error('null positive-size array');
    }
    return Array.from({length: count}, (_, i) => p.add(i * 4).readS32());
}
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.previousFile = currentFile;
        this.root = args[0];
        currentFile = args[1].readCString();
        emit('import-before', {dimension: this.root.add(0x1c).readS32()});
    },
    onLeave(retval) {
        emit('import-after', {status: retval.toInt32(),
            dimension: this.root.add(0x1c).readS32()});
        currentFile = this.previousFile;
    }
});
Interceptor.attach(at(0x32d65e), {
    onEnter(args) {
        this.groupCount = args[2]; this.ranges = args[3]; this.nodes = args[4];
        safe(() => {
            const elements = args[0].readS32();
            emit('planar-before', {elements, connectivity: integers(args[1], elements * 4)});
        });
    },
    onLeave() {
        safe(() => {
            const count = this.groupCount.readS32();
            if (count < 0 || count > 128) throw new Error('group bound: ' + count);
            const flat = integers(this.ranges, count * 2);
            const ranges = Array.from({length: count}, (_, i) => flat.slice(i * 2, i * 2 + 2));
            const length = count ? flat[flat.length - 1] + 1 : 1;
            emit('planar-after', {count, ranges, nodes: integers(this.nodes, length)});
        });
    }
});
Interceptor.attach(at(0x1fc290), {
    onEnter(args) { this.objectPointer = args[0]; },
    onLeave() {
        safe(() => {
            const p = this.objectPointer;
            const dimension = p.add(0x3a98).readPointer().add(0x1c).readS32();
            const count = p.add(0xbd8).readS32();
            if (dimension !== 2 || count < 0 || count > 100)
                throw new Error('unsupported object boundary layout');
            const groups = [];
            for (let i = 0; i < count; ++i) {
                const group = p.add(0xbe0 + i * 0x30);
                const size = group.readS32();
                groups.push(integers(group.add(8).readPointer(), size));
            }
            emit('object-planar', {id: p.add(8).readS32(), dimension,
                arity: p.add(0x894).readS32(), total: p.add(0x88c).readS32(), groups});
        });
    }
});
send({operation: 'hooks-installed', base: app.base.toString(), pid: Process.id});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
