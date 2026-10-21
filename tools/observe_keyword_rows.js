// Passive row-reader observation; no application calls or argument writes.
// Caller replaces __READY_GATE__ with a fresh private harness gate.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let file = null, sequence = 0;
const stacks = new Map();
function stack() {
    const id = Process.getCurrentThreadId();
    if (!stacks.has(id)) stacks.set(id, []);
    return stacks.get(id);
}
function emit(operation, fields) {
    send({sequence: ++sequence, operation, file, ...fields});
}
function safe(fn) {
    try { return fn(); }
    catch (error) { emit('probe-error', {error: String(error)}); }
}
function hex(p, n) {
    if (n < 0 || n > 4096) throw new Error('byte bound');
    return Array.from(new Uint8Array(p.readByteArray(n)),
        b => b.toString(16).padStart(2, '0')).join('');
}
function cbytes(p, limit) {
    let n = 0;
    while (n < limit && p.add(n).readU8() !== 0) ++n;
    if (n === limit) throw new Error('unterminated observed string');
    return hex(p, n);
}
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) { this.previous = file; file = args[1].readCString(); emit('import-before', {}); },
    onLeave(result) { emit('import-after', {status: result.toInt32()}); file = this.previous; }
});
Interceptor.attach(at(0x2f76c0), {
    onEnter(args) { this.enabled = file !== null; if (this.enabled) this.token = cbytes(args[0], 40); },
    onLeave(result) { if (this.enabled) emit('keyword', {token_hex: this.token, id: result.toInt32()}); }
});
const helpers = [
    {rva: 0x413de0, kind: 'int1', count: 1, skip: 2},
    {rva: 0x413ee0, kind: 'int2', count: 2},
    {rva: 0x414200, kind: 'int5', count: 5, skip: 6},
    {rva: 0x414600, kind: 'int9', count: 9, skip: 10},
    {rva: 0x415680, kind: 'coord2', count: 3, coordinates: true},
    {rva: 0x415880, kind: 'coord3', count: 4, coordinates: true},
    {rva: 0x413ab0, kind: 'name', count: 1, name: true}
];
for (const helper of helpers) {
    Interceptor.attach(at(helper.rva), {
        onEnter(args) {
            this.enabled = file !== null;
            if (!this.enabled) return;
            safe(() => {
                const entry = {kind: helper.kind, skip: helper.skip === undefined
                    ? 0 : args[helper.skip].toInt32() & 255,
                    pointers: Array.from({length: helper.count}, (_, i) => args[i + 1]),
                    lines: [], scans: []};
                if (helper.name) {
                    entry.name_limit = args[2].toInt32();
                    if (entry.name_limit !== 81) throw new Error('unexpected name limit');
                    entry.previous_name_hex = hex(args[1], 81);
                }
                this.entry = entry;
                stack().push(entry);
            });
        },
        onLeave(result) {
            if (!this.enabled || !this.entry) return;
            safe(() => {
                if (stack().pop() !== this.entry) throw new Error('row nesting mismatch');
                const e = this.entry;
                const observation = {kind: e.kind, skip: e.skip, status: result.toInt32(),
                    lines: e.lines, scans: e.scans};
                if (helper.name) {
                    observation.previous_name_hex = e.previous_name_hex;
                    observation.name_hex = hex(e.pointers[0], 81);
                } else {
                    observation.values = e.pointers.map((p, i) =>
                        helper.coordinates && i > 0 ? p.readDouble() : p.readS32());
                    observation.output_hex = e.pointers.map((p, i) =>
                        hex(p, helper.coordinates && i > 0 ? 8 : 4));
                }
                emit('row', observation);
            });
        }
    });
}
Interceptor.attach(at(0x47f898), {
    onEnter(args) {
        this.row = stack().at(-1);
        if (this.row) { this.buffer = args[0]; this.limit = args[1].toInt32(); }
    },
    onLeave(result) {
        if (!this.row) return;
        safe(() => this.row.lines.push({limit: this.limit, eof: result.isNull(),
            hex: result.isNull() ? null : cbytes(this.buffer, this.limit)}));
    }
});
Interceptor.attach(at(0x47c354), {
    onEnter(args) {
        this.row = stack().at(-1);
        if (this.row) this.scan = {input_hex: cbytes(args[0], 1024),
            format: args[1].readCString()};
    },
    onLeave(result) {
        if (this.row) { this.scan.conversions = result.toInt32(); this.row.scans.push(this.scan); }
    }
});
send({operation: 'hooks-installed', pid: Process.id, base: app.base.toString()});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
