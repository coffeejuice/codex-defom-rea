// Passive REA/Frida probe. No argument, memory, or return-value modifications.
// Address and argument layout: analysis/0x1403b6679.json.
const executable = Process.getModuleByName('DEF_PRE_64.exe');
const entry = executable.base.add(0x3b6679);
function digest(address, count, width) {
    if (count < 0 || count > 100000000)
        throw new Error('Unexpected array count: ' + count);
    return Checksum.compute('sha256', address.readByteArray(count * width));
}
Interceptor.attach(entry, {
    onEnter(args) {
        this.mode = args[1].readS32();
        this.record = args[2];
        this.step = args[3];
        this.headerCount = args[4];
        this.header = args[5];
        this.integerCount = args[6];
        this.integers = args[7];
        this.realCount = args[8];
        this.reals = args[9];
        this.error = args[10];
    },
    onLeave() {
        try {
            const status = this.error.readS32();
            const value = {
                operation: 'loader-return', mode: this.mode, status,
                record: this.record.readS32(), step: this.step.readS32()
            };
            if (status === 0 && this.mode === 2) {
                value.header_count = this.headerCount.readS32();
                value.header_sha256 = digest(this.header, value.header_count, 8);
            }
            if (status === 0 && this.mode === 3) {
                value.integer_count = this.integerCount.readS32();
                value.real_count = this.realCount.readS32();
                value.integer_sha256 = digest(this.integers, value.integer_count, 4);
                value.real_sha256 = digest(this.reals, value.real_count, 8);
            }
            send(value);
        } catch (error) {
            send({operation: 'probe-error', message: String(error)});
        }
    }
});
send({operation: 'hook-installed', entry: entry.toString()});
const gate = new File(
    'C:/Users/alext/Projects/codex-deform-rea/analysis/oracle/interactive/rea-step25.ready', 'w');
gate.write('ready\n');
gate.close();
