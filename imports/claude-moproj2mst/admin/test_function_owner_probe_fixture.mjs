// Harmless in-memory fixture for the CP207 injected callback script.
// It never imports Frida, opens the target, or creates a process.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const runner = fs.readFileSync(path.join(ROOT, 'admin', 'original_function_owner_probe.mjs'), 'utf8');
assert.match(runner, /const ORIGINAL_RUN_REVIEWED = false;/);
const value = pattern => {
  const match = runner.match(pattern);
  assert.ok(match, `Missing source fragment: ${pattern}`);
  return match[0];
};
const definitions = [
  value(/const EXE = '[^']+';/),
  value(/const HOOKS = \[[\s\S]*?\n\];/),
  value(/const EVENT_CAP_PER_KIND = \d+;/),
  value(/const EVENT_CAP_TOTAL = \d+;/),
].join('\n');
const begin = runner.indexOf('function injectedSource(creationFiletimeExact) {');
const end = runner.indexOf('\nasync function run()', begin);
assert.ok(begin >= 0 && end > begin);
const generated = vm.runInNewContext(`${definitions}\n${runner.slice(begin, end)}\ninjectedSource('123456789');`, {}, { timeout: 1000 });
new vm.Script(generated);

const BASE = 0x140000000n;
const QT = 0x58000000n;
const O = 0x700000n;
const H = 0x710000n;
const prefixes = new Map([
  [(BASE + 0xeaf60n).toString(), '48894c2408534881ec60010000'],
  [(BASE + 0x1d510n).toString(), '48894c24084883ec28488b4c2430'],
]);
const fields = new Map([[(O + 0x30n).toString(), H], [(H + 0x38n).toString(), 0n]]);
class Pointer {
  constructor(value) { this.value = BigInt(value instanceof Pointer ? value.value : value); }
  add(value) { return new Pointer(this.value + BigInt(value)); }
  sub(value) { return new Pointer(this.value - BigInt(value instanceof Pointer ? value.value : value)); }
  equals(value) { return this.value === new Pointer(value).value; }
  toUInt32() { return Number(this.value & 0xffffffffn); }
  toString() { return `0x${this.value.toString(16)}`; }
  readByteArray(length) {
    const hex = prefixes.get(this.value.toString());
    assert.ok(hex, `Unexpected code read ${this}`);
    assert.equal(length, hex.length / 2);
    return Uint8Array.from(hex.match(/../g), byte => parseInt(byte, 16)).buffer;
  }
  readPointer() {
    if (!fields.has(this.value.toString())) throw new Error('mock-unreadable');
    return new Pointer(fields.get(this.value.toString()));
  }
}
const ptr = value => new Pointer(value);
const app = { name: 'def_gui_mo_64.exe', path: 'c:\\program files\\sftc\\deform\\v11.0.2\\def_gui_mo_64.exe', base: ptr(BASE), size: 0x535f000 };
const qt = { name: 'QtCore4_64.dll', path: 'c:\\program files\\sftc\\deform\\v11.0.2\\QtCore4_64.dll', base: ptr(QT), size: 0x320000 };
function fixture() {
  const messages = [], hooks = new Map();
  let handler = null, tid = 101;
  const context = {
    Process: {
      id: 9001, platform: 'windows', arch: 'x64', mainModule: app,
      getCurrentThreadId: () => tid,
      findModuleByAddress: address => {
        const n = new Pointer(address).value;
        return n >= BASE && n < BASE + BigInt(app.size) ? app : n >= QT && n < QT + BigInt(qt.size) ? qt : null;
      },
      setExceptionHandler: callback => { assert.equal(handler, null); handler = callback; },
    },
    Interceptor: { attach: (address, callback) => {
      assert.ok(prefixes.has(new Pointer(address).value.toString()));
      assert.ok(!hooks.has(address.toString()));
      hooks.set(address.toString(), callback);
    } },
    Thread: { backtrace: () => [0x4af4n, 0xa7377n, 0xa7510n, 0xeb1ecn, 0xeaa7an].map(rva => ptr(BASE + rva)) },
    Backtracer: { ACCURATE: 1 },
    DebugSymbol: { fromAddress: address => ({ toString: () => address.toString() }) },
    ptr,
    send: payload => messages.push(JSON.parse(JSON.stringify(payload))),
  };
  vm.runInNewContext(generated, context, { timeout: 1000 });
  assert.equal(hooks.size, 2);
  assert.equal(messages[0].kind, 'handler-ready');
  assert.equal(messages[0].hooks.length, 2);
  assert.equal(messages[0].registrationCallCompleted, true);
  assert.ok(handler);
  return {
    messages, setTid: value => { tid = value; },
    owner: hooks.get(ptr(BASE + 0xeaf60n).toString()),
    producer: hooks.get(ptr(BASE + 0x1d510n).toString()),
    av: () => handler({ type: 'access-violation', address: ptr(QT + 0x32c0n),
      memory: { operation: 'read', address: ptr(0x58) },
      context: { rip: ptr(QT + 0x32c0n), rcx: ptr(0x58), rsp: ptr(0x800000),
        rax: ptr(1), rbx: ptr(2), rdx: ptr(3), rsi: ptr(4), rdi: ptr(5), r8: ptr(6), r9: ptr(7), r10: ptr(8), r11: ptr(9) } }),
  };
}
const invocation = (returnRva, rcx = O) => ({ returnAddress: ptr(BASE + BigInt(returnRva)),
  context: { rcx: ptr(rcx), rsp: ptr(0x800000) } });

const good = fixture();
const owner = invocation(0xeaa7a);
const producer = invocation(0xeb000, H);
good.owner.onEnter.call(owner);
good.producer.onEnter.call(producer);
good.producer.onLeave.call(producer, ptr(0));
assert.equal(good.av(), false);
const kinds = good.messages.map(message => message.kind);
assert.deepEqual(kinds, ['handler-ready', 'hook', 'hook', 'hook', 'first-av-core', 'first-access-violation']);
const events = good.messages.filter(message => message.kind === 'hook');
assert.deepEqual(events.map(event => event.event), ['owner-enter', 'producer-enter', 'producer-return']);
assert.deepEqual(events.map(event => event.sequence), [1, 2, 3]);
assert.equal(events[0].invocationId, events[1].invocationId);
assert.equal(events[1].invocationId, events[2].invocationId);
assert.equal(events[2].producerP, '0x0');
assert.deepEqual(JSON.parse(JSON.stringify(events[2].preStore)), {
  holderH: { ok: true, value: `0x${H.toString(16)}` },
  field: { ok: true, value: '0x0' }, phase: 'pre-store-context',
});
const core = good.messages.find(message => message.kind === 'first-av-core');
assert.equal(core.tid, 101);
assert.deepEqual(core.activeInvocations, [events[0].invocationId]);
assert.equal(core.hookSnapshot.sequence, 3);
assert.equal(core.hookSnapshot.counts['producer-return'], 1);
assert.equal(core.ripModule.offset, '0x32c0');
assert.equal(core.memoryOperation.address, '0x58');
assert.equal(good.messages.at(-1).accurateBacktrace.length, 5);
good.owner.onLeave.call(owner);

const rejected = fixture();
rejected.owner.onEnter.call(invocation(0xeaa7b));
rejected.producer.onEnter.call(invocation(0xeb001, H));
rejected.producer.onEnter.call(invocation(0xeb000, H)); // Correct caller, no owner frame.
const nestedOuter = invocation(0xeaa7a), nestedInner = invocation(0xeaa7a);
rejected.owner.onEnter.call(nestedOuter);
rejected.owner.onEnter.call(nestedInner);
rejected.producer.onEnter.call(invocation(0xeb000, H)); // Two active owners are ambiguous.
assert.equal(rejected.av(), false);
const rejectedCore = rejected.messages.find(message => message.kind === 'first-av-core');
assert.equal(rejectedCore.hookSnapshot.counts['producer-enter'], 0);
assert.equal(rejectedCore.hookSnapshot.rejected.ownerCaller, 1);
assert.equal(rejectedCore.hookSnapshot.rejected.producerCaller, 1);
assert.equal(rejectedCore.hookSnapshot.rejected.noUniqueOwner, 2);
assert.equal(rejected.messages.filter(message => message.kind === 'hook').length, 2);

const crossThread = fixture();
const crossOwner = invocation(0xeaa7a);
crossThread.owner.onEnter.call(crossOwner);
crossThread.setTid(202);
crossThread.producer.onEnter.call(invocation(0xeb000, H));
assert.equal(crossThread.messages.filter(message => message.kind === 'hook').length, 1);
crossThread.setTid(101);
crossThread.owner.onLeave.call(crossOwner);

const noReturn = fixture();
const pendingOwner = invocation(0xeaa7a), pendingProducer = invocation(0xeb000, H);
noReturn.owner.onEnter.call(pendingOwner);
noReturn.producer.onEnter.call(pendingProducer);
assert.equal(noReturn.av(), false);
assert.equal(noReturn.messages.filter(message => message.event === 'producer-return').length, 0);
assert.equal(noReturn.messages.find(message => message.kind === 'first-av-core').hookSnapshot.counts['producer-return'], 0);

const unreadable = fixture();
const unreadableOwner = invocation(0xeaa7a), unreadableProducer = invocation(0xeb000, H);
fields.delete((H + 0x38n).toString());
try {
  unreadable.owner.onEnter.call(unreadableOwner);
  unreadable.producer.onEnter.call(unreadableProducer);
  unreadable.producer.onLeave.call(unreadableProducer, ptr(0));
  const returned = unreadable.messages.find(message => message.event === 'producer-return');
  assert.equal(returned.preStore.holderH.ok, true);
  assert.equal(returned.preStore.field.ok, false);
  assert.match(returned.preStore.field.error, /mock-unreadable/);
} finally { fields.set((H + 0x38n).toString(), 0n); }

const capped = fixture();
for (let index = 0; index < 17; index++) {
  const frame = invocation(0xeaa7a);
  capped.owner.onEnter.call(frame);
  capped.owner.onLeave.call(frame);
}
assert.equal(capped.av(), false);
const cappedCore = capped.messages.find(message => message.kind === 'first-av-core');
assert.equal(cappedCore.hookSnapshot.counts['owner-enter'], 16);
assert.equal(cappedCore.hookSnapshot.overflow['owner-enter'], 1);
assert.equal(cappedCore.hookSnapshot.sequence, 16);

console.log(JSON.stringify({ outcome: 'PASS', cases: ['qualified-return-and-first-av-order', 'caller-and-recursion-rejection',
  'cross-thread-rejection', 'no-return-no-record', 'failed-read-status', 'event-cap-overflow'],
  originalLaunched: false, guiControlAcquired: false }));
