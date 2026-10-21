// Passive original GRNDAT108 observation. No calls into the application and no
// target-memory writes. Registered table extents follow observed12fad0 calls.
const app = Process.getModuleByName('DEF_PRE_64.exe'), at = rva => app.base.add(rva);
const registered = new Set(), capacities = new Map();
let file = null, root = null, active = null, sequence = 0;
function emit(operation, fields) { send({sequence: ++sequence, operation, file, ...fields}); }
function safe(fn) { try { fn(); } catch (e) { emit('probe-error', {error: String(e)}); } }
function bounded(n, max = 256) { if (!Number.isInteger(n) || n < 0 || n > max) throw new Error('extent ' + n); return n; }
function bytes(p, n) {
    bounded(n, 2048); if (n === 0) return '';
    if (p.isNull()) throw new Error('null positive range');
    return Array.from(new Uint8Array(p.readByteArray(n)), b => b.toString(16).padStart(2, '0')).join('');
}
function total(t) {
    const rank = t.add(4).readS32(), width = t.add(0x40).readS32();
    bounded(rank, 3); bounded(width, 16);
    if (rank === 0) return width < 2 ? 0 : width;
    let n = width;
    for (let i = 0; i < rank; ++i) n *= Math.max(0, t.add(0x1c + i * 4).readS32());
    return bounded(n);
}
function table(t) {
    if (!t.add(0x68).readPointer().isNull()) throw new Error('nonnull table cache outside frozen owned profile');
    const known = capacities.get(t.toString());
    function buffer(p, extent) {
        if (p.isNull()) return '';
        if (!extent || extent.pointer !== p.toString()) throw new Error('unobserved table buffer');
        return bytes(p, bounded(extent.count) * 8);
    }
    return {type_0x00: t.readS32(), rank_0x04: t.add(4).readS32(),
        selectors_0x10: [0, 1, 2].map(i => t.add(0x10 + 4*i).readS32()),
        counts_0x1c: [0, 1, 2].map(i => t.add(0x1c + 4*i).readS32()),
        values_per_point_0x40: t.add(0x40).readS32(), flag_0x50: t.add(0x50).readS32(), mode_0x58: t.add(0x58).readS32(),
        raw_hex: {constant: bytes(t.add(8), 8),
            axes: [0, 1, 2].map(i => buffer(t.add(0x28 + i*8).readPointer(), known?.axes[i])),
            dependent: buffer(t.add(0x48).readPointer(), known?.dependent)}};
}
function record(p) {
    return {count_0x00: p.readS32(), inline_0x08: bytes(p.add(8), 96),
        scalar_0x68: bytes(p.add(0x68), 8), integer_0x70: p.add(0x70).readS32(),
        skip_first_two_0x1e0: p.add(0x1e0).readU8(),
        tables: [0x78, 0xf0, 0x168].map(o => table(p.add(o)))};
}
function list(p) {
    const count = bounded(p.add(0x10).readS32(), 8), result = [], visited = new Set();
    let node = p.readPointer(), tail = p.add(8).readPointer(), last = ptr(0);
    for (let i = 0; i < count; ++i) {
        if (node.isNull() || visited.has(node.toString())) throw new Error('short/cyclic record list');
        visited.add(node.toString()); result.push(record(node)); last = node;
        node = node.add(0x1e8).readPointer();
    }
    if (!node.isNull() || !last.equals(tail)) throw new Error('record tail/count mismatch');
    return result;
}
function material(p) {
    return {id: p.readS32(), selector: p.add(0x17e8).readS32(),
        grndat: {secondary_0x17ec: p.add(0x17ec).readS32(), table_0x1740: table(p.add(0x1740))},
        grndat_extended: {flags_0x1078: [0, 1, 2, 3].map(i => p.add(0x1078 + i*4).readS32()),
            tables_0x1088: Array.from({length: 9}, (_, i) => table(p.add(0x1088 + i*0x78))),
            scalars_0x14c0: bytes(p.add(0x14c0), 16), secondary_0x14d0: p.add(0x14d0).readS32(),
            embedded: record(p.add(0x14d8)), lists: [0x16c8, 0x16e0, 0x16f8, 0x1710, 0x1728].map(o => list(p.add(o))),
            coefficients_0x17b8: bytes(p.add(0x17b8), 48), secondary_0x17ec: p.add(0x17ec).readS32()}};
}
function state(p) {
    const count = bounded(p.add(0x1320).readS32(), 4), materials = [];
    let node = p.add(0x1328).readPointer();
    for (let i = 0; i < count; ++i) {
        if (node.isNull()) throw new Error('short material list');
        materials.push(material(node)); node = node.add(0x2200).readPointer();
    }
    if (!node.isNull()) throw new Error('material count mismatch');
    if (p.add(0x864).readS32() !== 0) throw new Error('enabled grain gate outside frozen profile');
    return {dimension: p.add(0x1c).readS32(), materials};
}
function stream(p) {
    const base = p.add(0x10).readPointer(), current = p.readPointer();
    return base.isNull() || current.isNull() ? 0 : bounded(current.sub(base).toInt32(), 4096);
}
Interceptor.attach(at(0x28b850), {onEnter(args) {
    const group = args[0];
    for (let i = 0; i < 9; ++i) registered.add(group.add(0x10 + i*0x78).toString());
    registered.add(group.add(0x6c8).toString());
}});
Interceptor.attach(at(0x28a4c0), {onEnter(args) {
    for (const o of [0x78, 0xf0, 0x168]) registered.add(args[0].add(o).toString());
}});
Interceptor.attach(at(0x12f480), {onEnter(args) {this.table = ptr(args[0].toString());},
    onLeave() {capacities.delete(this.table.toString());}});
Interceptor.attach(at(0x12fad0), {onEnter(args) {this.table = ptr(args[0].toString());}, onLeave() {
    const t = this.table; if (!registered.has(t.toString())) return;
    safe(() => capacities.set(t.toString(), {
        axes: [0, 1, 2].map(i => ({pointer: t.add(0x28 + i*8).readPointer().toString(), count: bounded(Math.max(0, t.add(0x1c + i*4).readS32()))})),
        dependent: {pointer: t.add(0x48).readPointer().toString(), count: total(t)}}));
}});
Interceptor.attach(at(0x1b5f00), {onEnter(args) {
    this.previous = file; file = args[1].readCString(); root = ptr(args[0].toString()); this.root = root;
    safe(() => emit('import-before', {state: state(root)}));
}, onLeave(result) {
    safe(() => emit('import-after', {status: result.toInt32(), state: state(this.root)})); file = this.previous;
}});
Interceptor.attach(at(0x384a50), {onEnter(args) {
    this.enabled = file !== null && args[2].toInt32() === 108;
    if (!this.enabled) return;
    active = {root: ptr(args[0].toString()), stream: ptr(args[1].toString()), refreshes: []};
}, onLeave(result) {
    if (!this.enabled) return;
    safe(() => emit('handler-after', {status: result.toInt32(), cursor: stream(active.stream),
        refreshes: active.refreshes, state: state(active.root)})); active = null;
}});
Interceptor.attach(at(0x1d0fc0), {onEnter(args) {if (active) active.refreshes.push(args[1].toInt32());}});
send({operation: 'hooks-installed', pid: Process.id});
const gate = new File('__READY_GATE__', 'w'); gate.write('ready\n'); gate.close();
