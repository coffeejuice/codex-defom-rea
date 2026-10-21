// Install while the original main thread is suspended. Passive observation only.
// Substitute __READY_GATE__ with a fresh private harness gate path.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
let sequence = 0;
let context = null;
function emit(operation, data = {}) { send({sequence: ++sequence, operation, ...data}); }
function safe(fn) {
    try { fn(); } catch (error) { emit('probe-error', {error: String(error)}); }
}
function ints(p, count) {
    return Array.from({length: count}, (_, i) => p.add(i * 4).readS32());
}
function boundedCString(p, capacity) {
    const data = new Uint8Array(p.readByteArray(capacity));
    const end = data.indexOf(0);
    if (end < 0) throw new Error('unterminated string in bounded startup field');
    return String.fromCharCode(...data.slice(0, end));
}
function objectList(p, count) {
    const result = [];
    let item = p.readPointer();
    for (let i = 0; i < Math.min(Math.max(count, 0), 8) && !item.isNull(); ++i) {
        result.push({address: item.toString(), id: item.add(8).readS32(),
            type: item.add(12).readS32(), name: boundedCString(item.add(0x828), 81),
            mesh_flag: item.add(0x87c).readS32(), nodes: item.add(0x880).readS32(),
            elements: item.add(0x884).readS32()});
        item = item.add(0x3940).readPointer();
    }
    return result;
}
function root(p, completeContext = false) {
    const controls = p.add(0x750);
    const controlInts = {}, controlDoubles = {};
    for (const offset of [0xe4, 0xe8, 0xec, 0xf0, 0xf4, 0xf8, 0xfc, 0x100,
        0x104, 0x108, 0x10c, 0x110, 0x114, 0x118, 0x11c, 0x120, 0x124,
        0x128, 0x12c, 0x130, 0x134, 0x160, 0x164, 0x168, 0x16c, 0x1c8,
        0x2e0, 0x2e4, 0x310, 0x314, 0x318, 0x31c, 0x320])
        controlInts['0x' + offset.toString(16)] = controls.add(offset).readS32();
    for (const offset of [0x170, 0x178, 0x1d0, 0x1d8, 0x260, 0x268,
        0x270, 0x278, 0x280, 0x2c0, 0x2c8, 0x2d0, 0x2d8, 0x3a8,
        0x3b0, 0x3b8, 0x3d0, 0x660, 0x668])
        controlDoubles['0x' + offset.toString(16)] = controls.add(offset).readDouble();
    const count = p.add(0x1378).readS32();
    const result = {
        address: p.toString(), dimension: p.add(0x1c).readS32(),
        problem_id: boundedCString(p.add(0x340), 1024),
        simulation_title: boundedCString(controls, 81),
        operation_name: boundedCString(controls.add(0x51), 65),
        simulation_name: boundedCString(controls.add(0x92), 81),
        material_count: p.add(0x1320).readS32(), object_count: count,
        object_list_count: p.add(0x1390).readS32(),
        object_head: p.add(0x1380).readPointer().toString(),
        object_tail: p.add(0x1388).readPointer().toString(),
        object_owner: p.add(0x1398).readPointer().toString(),
        alternate_root: p.add(0x33e8).readPointer().toString(),
        alternate_enabled: p.add(0x33f0).readU8(),
        run_type: p.add(0x1738).readS32(),
        component_counts: ints(p.add(0x15f8), 79),
        control_ints: controlInts, control_doubles: controlDoubles,
        objects: objectList(p.add(0x1380), count), truncated: count > 8
    };
    if (completeContext) result.database_helper = p.add(0x3390).readPointer().toString();
    return result;
}
function state() {
    return context === null ? null : {
        context: context.toString(),
        primary: root(context.add(0x190), true),
        secondary: root(context.add(0x4eb0), true),
        primary_database: context.add(0x3598).toString(),
        secondary_database: context.add(0x82b8).toString()
    };
}
Interceptor.attach(at(0x1c9f0), {
    onEnter(args) { emit('main-enter', {argc: args[0].toInt32()}); },
    onLeave(retval) { emit('main-leave', {status: retval.toInt32()}); }
});
Interceptor.attach(at(0x1b7b0), {
    onEnter() { emit('arguments-enter'); },
    onLeave(retval) { emit('arguments-leave', {mode_mask: retval.toInt32()}); }
});
Interceptor.attach(at(0x1c770), {
    onEnter(args) { this.contextPointer = args[0]; emit('context-construction-enter'); },
    onLeave() {
        context = this.contextPointer;
        safe(() => emit('context-construction-leave', {state: state()}));
    }
});
Interceptor.attach(at(0x1c8950), {
    onEnter(args) {
        this.root = args[0];
        emit('root-construction-enter', {address: args[0].toString(), dimension: args[1].toInt32()});
    },
    onLeave() { safe(() => emit('root-construction-leave', {root: root(this.root)})); }
});
Interceptor.attach(at(0x1d8000), {
    onEnter(args) { emit('link-roots', {primary: args[0].toString(), secondary: args[1].toString()}); }
});
Interceptor.attach(at(0x1c5700), {
    onEnter(args) {
        safe(() => emit('set-dimension', {root: args[0].toString(), value: args[1].readS32()}));
    }
});
Interceptor.attach(at(0x1b8560), {
    onEnter(args) { this.path = args[0].readCString(); },
    onLeave(retval) {
        if (['DEF_MULTI.INI', 'DEF_PRE3.INI', 'DEF_PRE.INI'].includes(this.path))
            emit('startup-file-check', {path: this.path, result: retval.toInt32()});
    }
});
Interceptor.attach(at(0x9f20), {
    onEnter() { safe(() => emit('environment-enter', {state: state()})); },
    onLeave() { safe(() => emit('environment-leave', {state: state()})); }
});
Interceptor.attach(at(0x1b5f00), {
    onEnter(args) {
        this.path = args[1].readCString();
        safe(() => emit('startup-keyword-enter', {path: this.path, root: args[0].toString()}));
    },
    onLeave(retval) {
        safe(() => emit('startup-keyword-leave', {path: this.path,
            status: retval.toInt32(), state: state()}));
    }
});
Interceptor.attach(at(0xa200), {
    onEnter() { safe(() => emit('main-menu-enter', {state: state()})); }
});
send({operation: 'hooks-installed', pid: Process.id, base: app.base.toString()});
const gate = new File('__READY_GATE__', 'w');
gate.write('ready\n');
gate.close();
