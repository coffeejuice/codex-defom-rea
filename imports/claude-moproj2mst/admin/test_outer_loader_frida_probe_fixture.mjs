// Harmless in-memory fixture for the CP210 generated outer-loader callback script.
// It never imports Frida, opens the target, or creates a process.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const runner = fs.readFileSync(path.join(ROOT, 'admin', 'original_outer_loader_frida_probe.mjs'), 'utf8');
assert.match(runner, /const ORIGINAL_RUN_REVIEWED = false;/);
const value = pattern => {
  const match = runner.match(pattern);
  assert.ok(match, `Missing source fragment: ${pattern}`);
  return match[0];
};
const definitions = [
  value(/const EXE = '[^']+';/),
  value(/const HOOKS = \[[\s\S]*?\n\];/),
  value(/const EVENT_KINDS = \[[^\n]+;/),
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
const extracted = vm.runInNewContext(`${definitions}\n({HOOKS, EVENT_KINDS});`);
const prefixes = new Map(extracted.HOOKS.map(h => [(BASE + BigInt(h.rva)).toString(), h.bytes]));
const fields = new Map();
let patchedPrefixes = new Map();
class Pointer {
  constructor(value) { this.value = BigInt(value instanceof Pointer ? value.value : value); }
  add(value) { return new Pointer(this.value + BigInt(value)); }
  sub(value) { return new Pointer(this.value - BigInt(value instanceof Pointer ? value.value : value)); }
  isNull() { return this.value === 0n; }
  equals(value) { return this.value === new Pointer(value).value; }
  toUInt32() { return Number(this.value & 0xffffffffn); }
  toString() { return `0x${this.value.toString(16)}`; }
  readByteArray(length) {
    const hex = patchedPrefixes.get(this.value.toString()) ?? prefixes.get(this.value.toString());
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
function fixture(options = {}) {
  patchedPrefixes = new Map();
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
      const key = new Pointer(address).value.toString();
      const before = prefixes.get(key);
      const name = extracted.HOOKS.find(h => BASE + BigInt(h.rva) === new Pointer(address).value).name;
      let after = 'e900100000' + before.slice(10);
      if (options.badPatch === name) after = options.badKind === 'suffix' ? after.slice(0,10) + 'cc' + after.slice(12) : 'ff' + after.slice(2);
      patchedPrefixes.set(key, after);
    }, flush() {} },
    Thread: { backtrace: () => [0x4af4n, 0xa7377n, 0xa7510n, 0xeb1ecn, 0xeaa7an].map(rva => ptr(BASE + rva)) },
    Backtracer: { ACCURATE: 1 },
    DebugSymbol: { fromAddress: address => ({ toString: () => address.toString() }) },
    ptr,
    send: payload => messages.push(JSON.parse(JSON.stringify(payload))),
  };
  if (options.badPatch) {
    assert.throws(() => vm.runInNewContext(generated, context, { timeout: 1000 }), /five-byte gate/);
    assert.equal(messages.some(m => m.kind === 'handler-ready'), false);
    assert.equal(messages.at(-1).kind, 'install-readiness-failure');
    assert.equal(messages.at(-1).patches.length, 6);
    return {messages};
  }
  vm.runInNewContext(generated, context, { timeout: 1000 });
  assert.equal(hooks.size, 6);
  assert.equal(messages[0].kind, 'handler-ready');
  assert.equal(messages[0].hooks.length, 6);
  assert.equal(messages[0].registrationCallCompleted, true);
  assert.ok(handler);
  return {
    messages, setTid: value => { tid = value; },
    hook: name => hooks.get(ptr(BASE + BigInt(extracted.HOOKS.find(h => h.name === name).rva)).toString()),
    av: () => handler({ type: 'access-violation', address: ptr(QT + 0x32c0n),
      memory: { operation: 'read', address: ptr(0x58) },
      context: { rip: ptr(QT + 0x32c0n), rcx: ptr(0x58), rsp: ptr(0x800000),
        rax: ptr(1), rbx: ptr(2), rdx: ptr(3), rsi: ptr(4), rdi: ptr(5), r8: ptr(6), r9: ptr(7), r10: ptr(8), r11: ptr(9) } }),
  };
}

const invocation = (returnRva, rsp = 0x800000) => ({ returnAddress: ptr(BASE + BigInt(returnRva)),
  context: { rcx: ptr(O), rdx: ptr(O + 0x18d8n), rsp: ptr(rsp) } });
const enter = f => { const c = invocation(0x1c94); f.hook('outer').onEnter.call(c); return c; };
const leave = (f, c, value = 0) => f.hook('outer').onLeave.call(c, ptr(value));
const stage = (f, index, fail = false) => {
  const h = extracted.HOOKS[index + 1], c = invocation(h.callerRva, 0x7ffe20);
  f.hook(h.name).onEnter.call(c);
  const value = index === 0 ? (fail ? 1 : 0) : fail ? 0 : 0x100;
  // Acceptance uses AL: success 0x101 also proves nonzero upper bits do not matter.
  f.hook(h.name).onLeave.call(c, ptr(index === 4 ? fail ? 0x100 : 0x101 : value));
};
const hostStart = runner.indexOf('    const hookEvents =');
const hostEnd = runner.indexOf('    // CP209 main selects', hostStart);
const hostCode = runner.slice(hostStart, hostEnd);
const gateStart = runner.indexOf('function completionPrerequisites(');
const gateEnd = runner.indexOf('\nasync function run()', gateStart);
const gate = vm.runInNewContext(runner.slice(gateStart, gateEnd) + '\ncompletionPrerequisites;');
function qualify(f, mutate = x => x) {
  const payloads = mutate(JSON.parse(JSON.stringify(f.messages)));
  const result = { handlerReady: payloads.find(m => m.kind === 'handler-ready'),
    firstAvCore: payloads.find(m => m.kind === 'first-av-core'),
    firstAccessViolation: payloads.find(m => m.kind === 'first-access-violation'),
    cleanupReceipt: { exit_verified: true, handles_closed: true, wait_result: 0, termination_requested: false, exit_code: 0xc0000005 },
    helperExit: { code: 0 }, windowsAbsent: true, fridaAbsent: true };
  return vm.runInNewContext(`${definitions}\n${hostCode}\nresult;`, {
    result, path, scriptMessages: payloads.map(payload => ({ type: 'send', payload })),
    childIdentity: { pid: 9001, primary_tid: 101 }, creationFiletimeExact: '123456789',
  }, { timeout: 1000 });
}
let assertions = 0;
for (let failAt = 0; failAt <= 5; failAt++) {
  const f = fixture(), c = enter(f);
  for (let i = 0; i < (failAt === 5 ? 5 : failAt + 1); i++) stage(f, i, i === failAt);
  leave(f, c, failAt === 5 ? 0x100 : 0);
  assert.equal(f.av(), false);
  const r = qualify(f);
  assert.equal(r.outerEvidenceQualified, true);
  assert.equal(r.cp205FaultPath, true);
  assert.equal(r.outerPairs[0].firstFailedPrerequisite, failAt === 5 ? null : extracted.HOOKS[failAt + 1].name);
  assertions += 4;
  // Actual host sequence/count parity rejects lost and out-of-order transport.
  assert.equal(qualify(f, rows => rows.filter(m => !(m.kind === 'hook' && m.sequence === 2))).outerEvidenceQualified, false);
  assert.equal(qualify(f, rows => { const a = rows.findIndex(m => m.kind === 'hook'); [rows[a], rows[a+1]] = [rows[a+1],rows[a]]; return rows; }).outerEvidenceQualified, false);
  assertions += 2;
}
// Wrong-caller nested outer at EACH stage poisons the selected frame before
// helper callbacks can borrow it. Missing/unselected leave cannot repair it.
for (let at = 0; at < 5; at++) {
  const f = fixture(), c = enter(f);
  for (let i = 0; i < at; i++) stage(f, i);
  const wrong = invocation(0x77777);
  f.hook('outer').onEnter.call(wrong);
  stage(f, at, true);
  f.hook('outer').onLeave.call(wrong, ptr(0));
  leave(f, c); f.av();
  assert.equal(qualify(f).outerEvidenceQualified, false); assertions++;
}
{
 const f = fixture(), c = enter(f); stage(f, 0); f.av();
 assert.equal(qualify(f).outerEvidenceQualified, false); assertions++;
}
{
 const f = fixture(), c = enter(f), inner = enter(f);
 leave(f, inner); leave(f, c); f.av();
 assert.equal(qualify(f).outerEvidenceQualified, false); assertions++;
}
{
 const f = fixture(), c = enter(f); f.setTid(102); stage(f, 0, true); f.setTid(101); leave(f,c); f.av();
 assert.equal(qualify(f).outerEvidenceQualified, false); assertions++;
}
{
 const f = fixture(), c = enter(f), h = extracted.HOOKS[1], p = invocation(h.callerRva,0x7ffe20);
 f.hook(h.name).onEnter.call(p); stage(f, 0, true); f.hook(h.name).onLeave.call(p,ptr(0)); leave(f,c); f.av();
 assert.equal(qualify(f).outerEvidenceQualified, false); assertions++;
}
{
 const f = fixture(); for(let i=0;i<9;i++) enter(f); f.av();
 const core=f.messages.find(m=>m.kind==='first-av-core');
 assert.equal(core.hookSnapshot.boundedStateInvalid,true);
 assert.ok(core.activeInvocations.length<=8);
 assert.equal(qualify(f).outerEvidenceQualified,false); assertions+=3;
}
{
 const f=fixture(); for(let i=0;i<9;i++){const c=enter(f);stage(f,0,true);leave(f,c);} f.av();
 assert.equal(qualify(f).outerEvidenceQualified,false); assertions++;
}
{
 const f=fixture(),c=enter(f), h=extracted.HOOKS[1], p=invocation(h.callerRva,0x700000);
 f.hook(h.name).onEnter.call(p); f.hook(h.name).onLeave.call(p,ptr(1)); leave(f,c); f.av();
 assert.equal(qualify(f).outerEvidenceQualified,false); assertions++;
}
{
 const f=fixture(),c=enter(f);stage(f,0,true);leave(f,c);f.av();
 assert.equal(qualify(f, rows=>{rows.find(m=>m.event==='outer-enter').pathQString='0x1';return rows;}).outerEvidenceQualified,false);
 assert.equal(qualify(f, rows=>{for(const m of rows)if(m.kind==='hook')m.processCreationFiletime='wrong';return rows;}).outerEvidenceQualified,false);
 assert.equal(qualify(f, rows=>{rows.find(m=>m.kind==='first-av-core').hookSnapshot.counts['outer-enter']++;return rows;}).outerEvidenceQualified,false);
 assertions+=3;
}
{
 const f=fixture();f.setTid(102);const c=enter(f);stage(f,0,true);leave(f,c);f.av();
 assert.equal(qualify(f).outerEvidenceQualified,false); assertions++;
}
// Actual completion helper, including errors arriving after ready/resume or
// cleanup settlement. These test a necessary gate; literal release guards stay false.
const ok = { patchReadinessQualified:true, outerEvidenceQualified:true, cp205FaultPath:true, firstAvCoreQualified:true,
 exceptionRecordQualified:true, commandInvocationQualified:true, cleanupVerified:true,
 cleanupReceipt:{exit_code:0xc0000005}, processAbsent:true, helperStdoutClosed:true,
 scriptUnloadSettled:true, sessionDetached:true, fixtureComparison:{unchanged:true},
 rawHelperReceiptsRetained:true, failures:[] };
assert.equal(gate(ok,null,'',false),true); assertions++;
for (const reason of ['error-after-resume','error-after-cleanup']) {assert.equal(gate(ok,new Error(reason),'',false),false);assertions++;}
assert.equal(gate(ok,null,'partial',false),false);
assert.equal(gate(ok,null,'',true),false);
assert.equal(gate({...ok,cleanupReceipt:{exit_code:0}},null,'',false),false);
assert.equal(gate({...ok,commandInvocationQualified:false},null,'',false),false);
assertions+=4;
const patchHelperStart = runner.indexOf('function patchReadinessQualified(');
const patchHelperEnd = runner.indexOf('function completionPrerequisites(', patchHelperStart);
const patchGate = vm.runInNewContext(runner.slice(patchHelperStart,patchHelperEnd) + '\npatchReadinessQualified;');
for (const hook of extracted.HOOKS) for (const badKind of ['opcode','suffix']) {
  fixture({badPatch:hook.name,badKind}); assertions++;
}
{
  const f=fixture(), ready=f.messages.find(m=>m.kind==='handler-ready');
  assert.equal(patchGate(ready,extracted.HOOKS),true);
  const damaged=JSON.parse(JSON.stringify(ready));damaged.patches[0].redirectDestination='0x1';
  assert.equal(patchGate(damaged,extracted.HOOKS),false);
  const absent=JSON.parse(JSON.stringify(ready));absent.patches.pop();
  assert.equal(patchGate(absent,extracted.HOOKS),false);
  assert.equal(gate({...ok,patchReadinessQualified:false},null,'',false),false);
  assert.equal(gate({...ok,installReadinessFailure:{reason:'late'}},null,'',false),false);assertions+=5;
}
assert.match(runner,/const HOOK_BYTES_REVIEWED = false;/);
console.log(JSON.stringify({outcome:'PASS', assertions, actualGeneratedCallbacks:true, actualHostQualification:true,
 originalExecuted:false, guiControlAcquired:false, limitations:['Frida mocks do not prove patchability or native exception disposition','Runtime QString content is unverified; claim concerns the fixed CLI invocation']}));
