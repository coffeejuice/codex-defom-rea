'use strict';
// Synthetic, converter-owned memory only; loaded in one newly owned suspended child.
function hex(p, n) { return Array.from(new Uint8Array(p.readByteArray(n)), x => x.toString(16).padStart(2, '0')).join(''); }
function requireThat(ok, text) { if (!ok) throw new Error(text); }
const candidateHex = '488b842450020000488b8020040000488b8c2450020000488b8920040000488b000fb6542474';
const bytes = h => Array.from(h.match(/../g), x => parseInt(x, 16));
const code = Memory.alloc(Process.pageSize);
const prologue = bytes('4881ec7802000048898c245002000088542474');
const body = [...prologue, ...Array(64-prologue.length).fill(0x90), ...bytes(candidateHex), ...bytes('498900498948084189501089d04881c478020000c3')];
code.writeByteArray(body);
requireThat(Memory.protect(code, Process.pageSize, 'r-x'), 'Memory.protect failed');
const site = code.add(64);
const main = Memory.alloc(0x428), dock = Memory.alloc(16), vtable = Memory.alloc(0x60), out = Memory.alloc(24);
main.add(0x420).writePointer(dock); dock.writePointer(vtable);
const fn = new NativeFunction(code, 'uint32', ['pointer', 'uint8', 'pointer']);
const observations = [];
let active = null;
function invoke(phase, requested) {
  out.writeByteArray(Array(24).fill(0xcc));
  const returned = fn(main, requested, out);
  const row = {phase, requested, returned, rax:out.readPointer().toString(), rcx:out.add(8).readPointer().toString(), edx:out.add(16).readU32()};
  requireThat(returned === requested && out.readPointer().equals(vtable) && out.add(8).readPointer().equals(dock) && out.add(16).readU32() === requested, 'execution mismatch ' + JSON.stringify(row));
  return row;
}
try {
  requireThat(Process.arch === 'x64' && Process.pointerSize === 8, 'x64 required');
  requireThat(hex(site, 38) === candidateHex, 'candidate prefix mismatch');
  const before = hex(site, 64);
  const runs = [invoke('baseline',0), invoke('baseline',1)];
  active = Interceptor.attach(site, {onEnter() {
    const rsp = this.context.rsp;
    const seenMain = rsp.add(0x250).readPointer();
    const seenDock = seenMain.add(0x420).readPointer();
    const raw = rsp.add(0x74).readU8();
    observations.push({sequence:observations.length+1,tid:Process.getCurrentThreadId(),rsp:rsp.toString(),main:seenMain.toString(),dock:seenDock.toString(),requestedRaw:raw,match:seenMain.equals(main)&&seenDock.equals(dock)&&(raw===0||raw===1)});
  }});
  Interceptor.flush();
  const patched = hex(site,64);
  runs.push(invoke('hooked',0), invoke('hooked',1));
  active.detach(); active = null; Interceptor.flush();
  const restored = hex(site,64);
  runs.push(invoke('restored',0), invoke('restored',1));
  requireThat(observations.length === 2 && observations.every(x=>x.match) && observations.map(x=>x.requestedRaw).join(',')==='0,1', 'observation mismatch');
  requireThat(before === restored, 'bytes not restored');
  requireThat(before !== patched, 'no visible patch');
  const differences = [...Array(64).keys()].filter(i=>before.slice(i*2,i*2+2)!==patched.slice(i*2,i*2+2));
  send({kind:'fixture-result',passed:true,pid:Process.id,arch:Process.arch,fridaRuntimeVersion:Frida.version,scriptRuntime:Script.runtime,code:code.toString(),site:site.toString(),siteOffset:64,main:main.toString(),dock:dock.toString(),vtable:vtable.toString(),candidateHex,before64:before,patched64:patched,restored64:restored,changedByteOffsets:differences,changedSpanBytes:differences.at(-1)+1,observations,runs,listenerDetached:true});
} catch(e) {
  if (active) { active.detach(); Interceptor.flush(); }
  send({kind:'fixture-result',passed:false,pid:Process.id,error:String(e),stack:e.stack,observations});
}
