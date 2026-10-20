// Passive import observation: stack frames and loader outputs only.
// Does not call application functions or change arguments, memory, or results.
const app = Process.getModuleByName('DEF_PRE_64.exe');
const entry = app.base.add(0x3b6679);
Interceptor.attach(entry, {
    onEnter(args) {
        this.mode = args[1].readS32();
        this.headerCount = args[4];
        this.header = args[5];
        this.integerCount = args[6];
        this.integers = args[7];
        this.realCount = args[8];
        this.reals = args[9];
        this.status = args[10];
        send({
            operation: 'loader-enter', mode: this.mode,
            caller: this.returnAddress.toString(),
            backtrace: Thread.backtrace(this.context, Backtracer.ACCURATE).map(p => p.toString())
        });
    },
    onLeave() {
        const status = this.status.readS32();
        if (status !== 0) {
            send({operation: 'loader-error', status});
            return;
        }
        if (this.mode === 2) {
            const n = this.headerCount.readS32();
            if (n < 0 || n > 10000) throw new Error('Unexpected header size');
            const values = [];
            for (let i = 0; i < n; ++i) values.push(this.header.add(i * 8).readDouble());
            send({operation: 'header', address: this.header.toString(), values});
        }
        if (this.mode === 3) {
            send({
                operation: 'arrays', integers: this.integers.toString(),
                integer_count: this.integerCount.readS32(),
                reals: this.reals.toString(), real_count: this.realCount.readS32()
            });
        }
    }
});
send({operation: 'hook-installed', entry: entry.toString()});
const gate = new File(
    'C:/Users/alext/Projects/codex-deform-rea/analysis/oracle/interactive/import-slice1.ready', 'w');
gate.write('ready\n');
gate.close();
