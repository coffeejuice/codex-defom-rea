// Passive capture of the original 3D boundary builder and its object consumer.
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
        this.previousFile = currentFile; currentFile = args[1].readCString();
        emit('import-before', {});
    },
    onLeave(retval) {
        emit('import-after', {status: retval.toInt32()});
        currentFile = this.previousFile;
    }
});
Interceptor.attach(at(0x354ed0), {
    onEnter(args) {
        this.outputCount = args[4]; this.faces = args[5]; this.owners = args[6];
        safe(() => {
            this.elements = args[0].readS32(); this.arity = args[2].readS32();
            emit('boundary-before', {elements: this.elements, arity: this.arity,
                connectivity: integers(args[1], this.elements * this.arity)});
        });
    },
    onLeave() {
        safe(() => {
            const count = this.outputCount.readS32();
            emit('boundary-after', {count, faces: integers(this.faces, count * 4),
                owner_elements: integers(this.owners, count)});
        });
    }
});
Interceptor.attach(at(0x1fc290), {
    onEnter(args) { this.objectPointer = args[0]; },
    onLeave() {
        safe(() => {
            const p = this.objectPointer;
            const dimension = p.add(0x3a98).readPointer().add(0x1c).readS32();
            const groups = p.add(0xbd8).readS32();
            if (dimension !== 3 || groups < 0 || groups > 4)
                throw new Error('unsupported boundary layout');
            const boundaries = [];
            for (let i = 0; i < groups; ++i) {
                const group = p.add(0xbe0 + i * 0x30);
                const count = group.add(0x10).readS32();
                boundaries.push({count, faces: integers(group.add(0x18).readPointer(), count * 4),
                    owner_elements: integers(group.add(0x20).readPointer(), count)});
            }
            emit('object-boundary', {id: p.add(8).readS32(), dimension,
                total: p.add(0x88c).readS32(), boundaries});
        });
    }
});
Interceptor.attach(at(0x43c760), {
    onEnter(args) {
        this.records = args[0]; this.count = args[1].readS32();
        safe(() => emit('sort-before', {count: this.count,
            records: integers(this.records, this.count * 2)}));
    },
    onLeave() {
        safe(() => emit('sort-after', {count: this.count,
            records: integers(this.records, this.count * 2)}));
    }
});
send({operation: 'hooks-installed', base: app.base.toString(), pid: Process.id});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
