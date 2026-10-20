// Passive probes for the bounded DB import/object-summary investigation.
// Original arguments, buffers, return values, and licensing state are unchanged.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const at = rva => app.base.add(rva);
function ints(p, n) {
    const out = [];
    for (let i = 0; i < n; ++i) out.push(p.add(i * 4).readS32());
    return out;
}
function bytes(p, n) { return Array.from(new Uint8Array(p.readByteArray(n))); }
function safe(fn) {
    try { fn(); } catch (error) { send({operation: 'probe-error', error: String(error)}); }
}
for (const [rva, label] of [
    [0xb170, 'db-menu'], [0x1cc2a0, 'select-step'], [0x1cc420, 'import-step'],
    [0x1cca80, 'read-header'], [0x1cd1f0, 'read-arrays'], [0x1d2230, 'read-objects']
]) {
    Interceptor.attach(at(rva), {
        onEnter(args) {
            this.contextPointer = args[0];
            safe(() => {
                const event = {operation: 'enter', function: label,
                    address: at(rva).toString(), caller: this.returnAddress.toString()};
                if (label === 'select-step' || label === 'import-step')
                    event.selection = args[2].toInt32();
                if (label === 'read-objects') {
                    const p = args[0];
                    event.object_count = p.add(0x1378).readS32();
                    event.integer_address_table = ints(p.add(0x1b90), 140);
                    event.real_address_table = ints(p.add(0x1dc0), 280);
                }
                send(event);
            });
        },
        onLeave(retval) {
            safe(() => {
                const event = {operation: 'leave', function: label};
                if (['read-header', 'read-arrays', 'import-step'].includes(label))
                    event.status = retval.toInt32();
                if (label === 'read-header') {
                    event.object_count = this.contextPointer.add(0x1378).readS32();
                    event.source_byteorder = this.contextPointer.add(0x748).readS32();
                }
                send(event);
            });
        }
    });
}
Interceptor.attach(at(0x16caf0), {
    onEnter(args) {
        this.object = args[0];
        safe(() => send({
            operation: 'object-input', caller: this.returnAddress.toString(),
            id: args[0].add(8).readS32(),
            integer_descriptor: ints(args[1], 50),
            raw_name_bytes: bytes(args[2].add(0x20), 64)
        }));
    },
    onLeave() {
        safe(() => send({
            operation: 'object-output', id: this.object.add(8).readS32(),
            internal_type: this.object.add(0xc).readS32(),
            mesh_flag: this.object.add(0x87c).readS32(),
            node_count: this.object.add(0x880).readS32(),
            element_count: this.object.add(0x884).readS32(),
            name_bytes: bytes(this.object.add(0x828), 65)
        }));
    }
});
send({operation: 'hooks-installed', base: app.base.toString()});
const gate = new File(
    'C:/Users/alext/Projects/codex-deform-rea/analysis/oracle/interactive/import-objects.ready', 'w');
gate.write('ready\n');
gate.close();
