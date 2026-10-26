'use strict';
// PREPARED ONLY. Parent verifies executable/DLL hashes, copies the manifest to
// C:\PROBLEM\m2m42, launches one owned fresh process with encoded native argv,
// and retains/terminates that exact PID. No GUI input, Qt getter, native call,
// state replacement, filesystem write, cwd change or exception swallowing here.
if (Process.platform !== 'windows' || Process.arch !== 'x64') throw new Error('Wrong target');
const EXPECTED_PROJECT = 'C:\\PROBLEM\\m2m42\\DB_deconstruction_02.moproj';
const MANIFEST = {
  "def_gui_mo_64.exe:10490": "4c8944241848895424104889",
  "def_gui_mo_64.exe:11a0": "488954241048894c24084881",
  "def_gui_mo_64.exe:1ca30": "488954241048894c24084881",
  "def_gui_mo_64.exe:8a9b0": "488954241048894c24084881",
  "def_gui_mo_64.exe:2a0e0": "488954241048894c24085657",
  "def_gui_mo_64.exe:eb26f0": "488954241048894c24085748",
  "def_gui_mo_64.exe:5f770": "48894c24084883ec38488b4c",
  "def_gui_mo_64.exe:ed9830": "488954241048894c240853b8",
  "def_gui_mo_64.exe:ed9969": "90488d8c2408020000ff1540",
  "qtgui4_64.dll:70d70": "48894c2408565741544883ec",
  "qtcore4_64.dll:12ae10": "48895c240848896c24104889",
  "qtcore4_64.dll:12e180": "40555657415441554883ec70",
  "qtcore4_64.dll:119570": "40565741544883ec4048c744"
};
const modules = {}, hooks = [], callbackHooks = new Map(), eventFrames = new Map();
const limit = { records: 140, list: 32, gestures: 32, ancestors: 12, text: 512, callbacks: 40 };
let mainWindow = null, project = null, modelChild = null, editor = null, operation = null;
let guiModel = null, ui = null, thread = null, sequence = 0, done = false;
let mainReady = false, normalOpen = false, postOpen = false, setupEntered = false;
let preselectedCallCount = 0;
function str(p) { return p ? p.toString() : null; }
function same(a, b) { return !!(a && b && a.equals(b)); }
function cp(p) { return ptr(p.toString()); }
function hex(data) { return Array.from(new Uint8Array(data)).map(x => x.toString(16).padStart(2, '0')).join(''); }
function moduleAt(p) {
  if (!p || p.isNull()) return null;
  const m = Process.findModuleByAddress(p);
  return m ? { name: m.name, path: m.path, rva: str(p.sub(m.base)) } : { unknown: str(p) };
}
function read(where, fn) { try { return fn(); } catch (e) { return { unavailable: true, where, error: String(e) }; } }
function emit(kind, data = {}) {
  if (++sequence > limit.records && kind !== 'capture_paused') return pause('record_limit');
  send({ kind, sequence, pid: Process.id, tid: Process.getCurrentThreadId(), ...data });
}
function pause(reason) {
  if (done) return;
  done = true;
  send({ kind: 'capture_paused', pid: Process.id, tid: Process.getCurrentThreadId(), reason,
    sequence, mainReady, normalOpen, postOpen, setupEntered,
    qualification: 'Owned selected thread blocked before continuation; root must retain evidence and terminate owned PID before unloading.' });
  recv('coordinator_release').wait();
}
function checked(m, rva) {
  const key = m.name.toLowerCase() + ':' + rva.toString(16), expected = MANIFEST[key];
  if (!expected) throw new Error('Missing original bytes: ' + key);
  const address = m.base.add(rva), actual = hex(address.readByteArray(expected.length / 2));
  if (actual !== expected) throw new Error('Original-byte mismatch: ' + key);
  send({ kind: 'hook_verified', key, address: str(address), bytes: actual });
  return address;
}
function attach(m, rva, callbacks) {
  const wrapped = {};
  for (const phase of ['onEnter', 'onLeave']) if (callbacks[phase]) wrapped[phase] = function(...args) {
    if (done) return;
    try { callbacks[phase].apply(this, args); }
    catch (e) { send({ kind: 'callback_error', hook: m.name + ':' + rva.toString(16), phase, error: String(e) }); pause('callback_error'); }
  };
  hooks.push(Interceptor.attach(checked(m, rva), wrapped));
}
function preselected(hook, details = {}) {
  ++preselectedCallCount;
  if (preselectedCallCount <= 8) send({ kind: 'preselected_call_ignored', pid: Process.id,
    tid: Process.getCurrentThreadId(), hook, ordinal: preselectedCallCount, mainReady, normalOpen, ...details });
  else if (preselectedCallCount === 9) send({ kind: 'preselected_calls_truncated', pid: Process.id, limit: 8,
    qualification: 'Further calls before the exact selected CLI Open are ignored; they are not selected Project/editor evidence.' });
}
function priv(object) { return object.add(8).readPointer(); }
function textAt(address, utf16 = true) {
  const data = address.readPointer(), count = data.add(8).readS32(), chars = data.add(0x10).readPointer();
  if (count < 0 || count > limit.text) return { data: str(data), count, unavailable: 'text bound' };
  return { data: str(data), count, chars: str(chars),
    nullString: utf16 ? data.equals(modules.core.base.add(0x2f0b30)) : undefined,
    text: count === 0 ? '' : utf16 ? chars.readUtf16String(count) : chars.readUtf8String(count) };
}
function list(data, mapper) {
  if (data.isNull()) return { data: str(data), unavailable: 'null list' };
  const begin = data.add(8).readS32(), end = data.add(12).readS32(), count = end - begin;
  if (begin < 0 || count < 0 || count > limit.list) return { data: str(data), begin, end, unavailable: 'list bound' };
  const entries = [];
  for (let i = begin; i < end; ++i) {
    const slot = data.add(0x18 + i * 8), value = slot.readPointer();
    entries.push(mapper ? read('list entry', () => mapper(value, slot)) : str(value));
  }
  return { data: str(data), begin, end, entries, complete: true };
}
function filters(object) {
  const d = priv(object);
  return { receiver: str(object), threadData: str(d.add(0x40).readPointer()),
    entries: list(d.add(0x70).readPointer(), guard => {
      const f = guard.isNull() ? guard : guard.readPointer();
      if (f.isNull()) return { guard: str(guard), object: str(f) };
      const vt = f.readPointer(), method = vt.add(0x28).readPointer();
      observeCallback(method, 'event_filter');
      return { guard: str(guard), object: str(f), vtable: moduleAt(vt), method: moduleAt(method),
        threadData: str(priv(f).add(0x40).readPointer()) };
    }) };
}
function internal() {
  const owner = modules.core.base.add(0x2f3c68).readPointer();
  if (owner.isNull()) return { owner: str(owner), absent: true };
  const data = owner.readPointer(), count = data.add(8).readS32();
  return { owner: str(owner), count, type3: count < 0 ? { malformed: true } : count <= 3 ? { absent: true } :
    list(data.add(0x28).readPointer(), method => { observeCallback(method, 'internal_type3'); return moduleAt(method); }) };
}
function frame() { const stack = eventFrames.get(Process.getCurrentThreadId()); return stack && stack[stack.length - 1]; }
function observeCallback(address, kind) {
  if (address.isNull()) return;
  const key = address.toString();
  if (callbackHooks.has(key)) { callbackHooks.get(key).add(kind); return; }
  if (callbackHooks.size >= limit.callbacks) throw new Error('Callback hook bound');
  const kinds = new Set([kind]); callbackHooks.set(key, kinds);
  const range = Process.findRangeByAddress(address);
  if (!range || !range.protection.includes('x')) throw new Error('Nonexecutable callback');
  hooks.push(Interceptor.attach(address, {
    onEnter(args) {
      this.selected = null;
      const f = frame(); if (!f || done) return;
      const direct = same(args[0], editor) && same(args[1], f.event);
      const filtered = same(args[1], editor) && same(args[2], f.event);
      const internalMatch = kinds.has('internal_type3') && read('internal callback array', () =>
        same(args[0].readPointer(), editor) && same(args[0].add(8).readPointer(), f.event)) === true;
      if (!direct && !filtered && !internalMatch) return;
      this.selected = f;
      emit('dispatch_callback_enter', { eventId: f.id, callback: moduleAt(address), kinds: Array.from(kinds),
        arguments: [str(args[0]), str(args[1]), str(args[2])],
        internalArguments: internalMatch ? read('internal args', () => [0, 8, 16].map(n => str(args[0].add(n).readPointer()))) : null });
    },
    onLeave(value) { if (this.selected && !done) emit('dispatch_callback_return', {
      eventId: this.selected.id, callback: moduleAt(address), value: value.toInt32() & 255 }); }
  }));
}
function gestures(object) {
  const records = [], seen = new Set(); let current = object;
  while (!current.isNull() && records.length < limit.ancestors) {
    if (seen.has(str(current))) return { records, unavailable: 'ancestor cycle' };
    seen.add(str(current)); const d = priv(current), w = current.add(0x20).readPointer();
    if (!(d.add(0x20).readU32() & 1)) return { records, unavailable: 'nonwidget ancestor' };
    const data = d.add(0x1c0).readPointer(), size = data.add(0x70).readS32(), entries = [];
    if (size < 0 || size > limit.gestures) return { records, size, unavailable: 'gesture bound' };
    let node = data.add(8).readPointer(); const nodes = new Set();
    while (!node.equals(data) && entries.length < limit.gestures) {
      if (node.isNull() || nodes.has(str(node))) return { records, unavailable: 'gesture cycle/null' };
      nodes.add(str(node)); entries.push({ node: str(node), key: node.sub(8).readS32(), flags: node.sub(4).readS32() });
      node = node.add(8).readPointer();
    }
    const windowFlags = w.add(12).readU32(), parent = d.add(0x10).readPointer();
    records.push({ object: str(current), parent: str(parent), windowFlags, data: str(data), size, entries,
      complete: node.equals(data) && entries.length === size });
    if (windowFlags & 1) return { records, terminal: 'window' }; current = parent;
  }
  return { records, terminal: current.isNull() ? 'null-parent' : 'ancestor bound' };
}
function properties(d) {
  const extra = d.add(0x38).readPointer(); if (extra.isNull()) return { extra: str(extra), absent: true };
  return { extra: str(extra), names: list(extra.add(8).readPointer(), (value, slot) => textAt(slot, false)),
    // QVariant is larger than a Qt4 list slot. Retain raw pointed records;
    // decoded variants beyond the known selected bool input remain qualified.
    values: list(extra.add(0x10).readPointer(), value => ({ record: str(value), raw: hex(value.readByteArray(16)) })) };
}
function widget(object) {
  const d = priv(object), data = object.add(0x20).readPointer(), extra = d.add(0x88).readPointer();
  return { object: str(object), private: str(d), vtable: moduleAt(object.readPointer()),
    parent: str(d.add(0x10).readPointer()), objectName: read('objectName', () => textAt(d.add(0x30))),
    hookData: str(d.add(0x78).readPointer()), dynamicMetaObject: str(d.add(0x28).readPointer()),
    threadData: str(d.add(0x40).readPointer()), windowFlags: data.add(12).readU32(),
    geometry: [0x14, 0x18, 0x1c, 0x20].map(n => data.add(n).readS32()),
    attributes: [0x1c8, 0x1cc, 0x1d0].map(n => d.add(n).readU32()),
    widgetExtra: str(extra), minMax: extra.isNull() ? null : [0x40, 0x44, 0x48, 0x4c].map(n => extra.add(n).readS32()),
    installedLayout: str(d.add(0xa8).readPointer()), properties: read('properties', () => properties(d)),
    filters: read('object filters', () => filters(object)) };
}
function shared(type) {
  const core = modules.core, app = core.base.add(0x2f4220).readPointer();
  if (app.isNull()) return { application: str(app), absent: true };
  observeCallback(app.readPointer().add(0x58).readPointer(), 'application_notify');
  return { application: str(app), appClosing: core.base.add(0x2f41fd).readU8(),
    appVtable: moduleAt(app.readPointer()), internal: read('internal', internal),
    applicationFilters: read('app filters', () => filters(app)),
    objectNameHook: moduleAt(core.base.add(0x2f4360).readPointer()),
    gestureManager: str(priv(app).add(0x148).readPointer()),
    gestures: type === 0xf ? read('Create gestures', () => gestures(editor)) : undefined };
}
function language() {
  const core = modules.core, app = core.base.add(0x2f4220).readPointer(), codecs = {};
  for (const [name, rva] of [['locale', 0x2f4438], ['translation', 0x2f4440], ['cStrings', 0x2f3e10]]) {
    const p = core.base.add(rva).readPointer(); codecs[name] = { pointer: str(p), vtable: p.isNull() ? null : moduleAt(p.readPointer()) };
  }
  return { codecs, translators: app.isNull() ? null : list(priv(app).add(0x98).readPointer()) };
}
function snapshot(stage) { emit(stage, { mainWindow: str(mainWindow), project: str(project), modelChild: str(modelChild),
  operation: str(operation), guiModel: str(guiModel), modelEqualsProjectChild: same(guiModel, modelChild),
  ownerGuards: setupEntered ? read('raw model/operation guards', () => ({ modelRaw: hex(editor.add(0x28).readByteArray(16)), operationRaw: hex(editor.add(0x38).readByteArray(16)) })) : null,
  editor: read('widget', () => widget(editor)),
  runtime: read('runtime', () => shared(0xaa)), language: read('language', language) }); }
const observer = Process.attachModuleObserver({ onAdded(m) {
  const name = m.name.toLowerCase();
  if (!['def_gui_mo_64.exe', 'qtcore4_64.dll', 'qtgui4_64.dll'].includes(name)) return;
  send({ kind: 'module', name: m.name, path: m.path, base: str(m.base), size: m.size });
  try {
    if (name === 'def_gui_mo_64.exe') {
      modules.exe = m;
      attach(m, 0x10490, { onEnter(args) { if (!mainWindow) mainWindow = cp(args[0]); this.selected = same(args[0], mainWindow); },
        onLeave() { if (this.selected) { mainReady = true; emit('main_window_constructor_return', { mainWindow: str(mainWindow) }); } } });
      attach(m, 0x11a0, { onEnter(args) {
        this.selected = this.returnAddress.equals(m.base.add(0x1c52));
        if (!this.selected) { preselected('11a0', { caller: moduleAt(this.returnAddress) }); return; }
        if (!mainReady) throw new Error('Selected decoder before actual mainwindow return');
        this.output = cp(args[0]); this.input = cp(args[1]);
      }, onLeave() { if (this.selected) emit('native_path_decode', { input: read('encoded path', () => textAt(this.input)), output: read('decoded path', () => textAt(this.output)) }); } });
      attach(m, 0x1ca30, { onEnter(args) {
        this.selected = false;
        if (!normalOpen && (!mainReady || !same(args[0], mainWindow) || !this.returnAddress.equals(m.base.add(0x1c94)))) {
          preselected('1ca30', { owner: str(args[0]), caller: moduleAt(this.returnAddress), path: read('preselected path', () => textAt(args[1])) }); return;
        }
        if (!mainReady || !same(args[0], mainWindow) || !this.returnAddress.equals(m.base.add(0x1c94))) throw new Error('Unexpected native Open owner/caller');
        const path = textAt(args[1]); if (path.text.toLowerCase() !== EXPECTED_PROJECT.toLowerCase()) throw new Error('Unexpected Open path');
        this.selected = true; normalOpen = true; emit('native_open_enter', { path, mainWindow: str(mainWindow), caller: moduleAt(this.returnAddress) });
      }, onLeave(result) { if (this.selected) emit('native_open_return', { result: str(result), sameProject: same(result, project) }); } });
      attach(m, 0x8a9b0, { onEnter(args) {
        this.selected = normalOpen;
        if (!this.selected) { preselected('8a9b0', { project: str(args[0]), caller: moduleAt(this.returnAddress), path: read('preselected project path', () => textAt(args[1])) }); return; }
        if (project && !same(project, args[0])) throw new Error('Unexpected second Project');
        project = cp(args[0]); modelChild = project.add(8).readPointer();
        emit('project_read_enter', { project: str(project), modelChild: str(modelChild), path: read('project path', () => textAt(args[1])) });
      }, onLeave(result) { if (this.selected) emit('project_read_return', { result: result.toInt32() & 255 }); } });
      attach(m, 0x2a0e0, { onEnter(args) {
        if (!normalOpen) { preselected('2a0e0', { owner: str(args[0]), project: str(args[1]), caller: moduleAt(this.returnAddress) }); return; }
        if (!same(args[0], mainWindow) || !same(args[1], project)) throw new Error('Unexpected postOpen ownership');
        postOpen = true; emit('post_open_enter', { mainWindow: str(mainWindow), project: str(project) });
      } });
      attach(m, 0xeb26f0, { onEnter(args) {
        if (!normalOpen) { preselected('eb26f0', { editor: str(args[0]), operation: str(args[1]), caller: moduleAt(this.returnAddress) }); return; }
        if (!normalOpen || !project || !modelChild || modelChild.isNull() || editor) throw new Error('Unexpected editor ownership/lifetime');
        editor = cp(args[0]); operation = cp(args[1]); thread = Process.getCurrentThreadId();
        if (operation.isNull()) throw new Error('Null native operation');
        emit('editor_constructor_enter', { editor: str(editor), operation: str(operation), project: str(project), postOpen });
      } });
      attach(m, 0x5f770, { onEnter(args) { this.selected = same(args[0], operation); }, onLeave(value) {
        if (this.selected) { guiModel = cp(value); emit('operation_model_resolved', { operation: str(operation), guiModel: str(guiModel) }); }
      } });
      attach(m, 0xed9830, { onEnter(args) {
        if (!normalOpen) { preselected('ed9830', { ui: str(args[0]), editor: str(args[1]), caller: moduleAt(this.returnAddress) }); return; }
        if (!same(args[1], editor) || !same(args[0], editor.add(0x140).readPointer()) || !guiModel || guiModel.isNull()) throw new Error('Setup owner/model mismatch');
        ui = cp(args[0]); setupEntered = true; snapshot('setup_entry');
      } });
      attach(m, 0xed9969, { onEnter() {
        if (!normalOpen) { preselected('ed9969'); return; }
        if (!setupEntered || Process.getCurrentThreadId() !== thread) throw new Error('Unexpected setup endpoint');
        snapshot('size_grip_setter_return'); emit('setter_return', { lowByte: this.context.rax.toInt32() & 255, ui: str(ui) });
        pause('selected_editor_before_first_grid');
      } });
    } else if (name === 'qtgui4_64.dll') {
      modules.gui = m;
      attach(m, 0x70d70, { onEnter(args) { this.selected = same(args[0], editor); if (this.selected) emit('editor_qwidget_constructor_enter', { parent: str(args[1]), flagsPointer: str(args[2]) }); },
        onLeave() { if (this.selected) snapshot('editor_qwidget_constructor_return'); } });
    } else {
      modules.core = m;
      attach(m, 0x12ae10, { onEnter(args) { this.selected = setupEntered && same(args[0], editor); if (this.selected) emit('name_setter_enter', { value: textAt(args[1]), before: read('widget', () => widget(editor)) }); },
        onLeave() { if (this.selected) snapshot('name_setter_return'); } });
      attach(m, 0x12e180, { onEnter(args) {
        this.selected = setupEntered && same(args[0], editor) && args[1].readUtf8String(15) === 'sizeGripEnabled';
        if (this.selected) emit('size_grip_setter_enter', { variantRaw: hex(args[2].readByteArray(16)), before: read('widget', () => widget(editor)) });
      }, onLeave(value) { if (this.selected) emit('size_grip_native_return', { value: value.toInt32() & 255, after: read('widget', () => widget(editor)) }); } });
      attach(m, 0x119570, { onEnter(args) {
        this.selected = same(args[1], editor); if (!this.selected) return;
        const type = args[2].add(0x10).readU16(); if (type !== 0xf && type !== 0xaa) { this.selected = false; return; }
        const event = cp(args[2]), tid = Process.getCurrentThreadId(), f = { id: sequence + 1, event, type };
        let stack = eventFrames.get(tid); if (!stack) { stack = []; eventFrames.set(tid, stack); } stack.push(f); this.frame = f;
        observeCallback(editor.readPointer().add(0x20).readPointer(), 'receiver_event');
        emit('notify_enter', { eventId: f.id, type, event: str(event), header: hex(event.readByteArray(24)),
          propertyName: type === 0xaa ? read('property event name', () => textAt(event.add(0x18), false)) : null,
          receiver: read('widget', () => widget(editor)), runtime: read('runtime', () => shared(type)) });
      }, onLeave(value) {
        if (!this.selected) return; const stack = eventFrames.get(Process.getCurrentThreadId());
        if (!stack || stack.pop() !== this.frame) throw new Error('Unbalanced event frame');
        emit('notify_return', { eventId: this.frame.id, type: this.frame.type, value: value.toInt32() & 255,
          header: hex(this.frame.event.readByteArray(24)), receiver: read('widget', () => widget(editor)) });
      } });
    }
  } catch (e) { send({ kind: 'installation_error', module: m.name, error: String(e) }); pause('installation_error'); }
} });
let exceptionCount = 0;
Process.setExceptionHandler(details => {
  if (++exceptionCount > 8) {
    if (exceptionCount === 9) send({ kind: 'native_exception_truncated', pid: Process.id, limit: 8,
      mainReady, normalOpen, postOpen, setupEntered, qualification: 'Further native exceptions are omitted; original handling continues unchanged.' });
    return false;
  }
  const registers = {};
  for (const name of ['rip', 'rsp', 'rbp', 'rax', 'rbx', 'rcx', 'rdx', 'rsi', 'rdi', 'r8', 'r9', 'r10', 'r11', 'r12', 'r13', 'r14', 'r15']) registers[name] = str(details.context[name]);
  send({ kind: 'native_exception', pid: Process.id, tid: Process.getCurrentThreadId(), type: details.type,
    address: str(details.address), module: moduleAt(details.address), memory: details.memory, registers,
    stackRaw: read('exception stack', () => hex(details.context.rsp.readByteArray(256))),
    backtrace: read('exception backtrace', () => Thread.backtrace(details.context, Backtracer.ACCURATE).slice(0, 24).map(moduleAt)),
    mainReady, normalOpen, postOpen, setupEntered, editor: str(editor), operation: str(operation), project: str(project) });
  return false; // original exception handling/crash remains untouched
});
send({ kind: 'probe_ready', pid: Process.id, expectedProject: EXPECTED_PROJECT,
  qualification: 'Prepared passive observation only; no runtime launched by worker. Root owns hash checks, launch and cleanup.' });
