'use strict';

/* Passive, source-bound diagnostic probe; does not launch or attach itself.
 * Load only into an orchestrator-owned DEFJobGroupSetup.exe diagnostic process.
 * No argument/register/return-value changes and no network/license payload reads.
 * Runtime use is still pending; see analysis/helper-diagnostic-handoff.json.
 */
const JobHelperProbe = (() => {
  const expected = Object.freeze({
    name: 'DEFJobGroupSetup.exe', arch: 'ia32', pointerSize: 4,
    size: 1036800, imageSize: 1110016,
    sha256: 'bef1dc1b7839b1a5f0b7d42933d1b59817397f0dfcb2315dfac350d33f6e7e18'
  });
  // Wildcards cover only absolute operands affected by PE base relocation.
  const anchors = Object.freeze({
    fetch: [0x13770, '6aff68????????64a1000000'],
    fileCheck: [0x10fc0, '518d914805000085d274318b'],
    warningFunction: [0x43510, '558bec6aff68????????64a1'],
    warningReference: [0x4357d, '68????????8d95e0f7ffff52'],
    directory: [0x11000, '578b7c240868????????c607'],
    path: [0x111a0, '81ec08040000a1????????33'],
    sendHeader: [0xe320, '80b97c03000000750683c8ff'],
    recvHeader: [0xe2f0, '80b97c03000000750683c8ff']
  });
  function bytesMatch(bytes, pattern) {
    const pairs = pattern.match(/../g);
    return bytes.length === pairs.length && pairs.every((p, i) => p === '??' || bytes[i] === parseInt(p, 16));
  }
  function endpoint(address, length) {
    if (address.isNull() || length < 2) return { decoded: false, reason: 'null_or_short_sockaddr' };
    const family = address.readU16();
    if (family === 2 && length >= 16) {
      return { decoded: true, family: 'ipv4', port: address.add(2).readU8() * 256 + address.add(3).readU8(),
        address: Array.from(new Uint8Array(address.add(4).readByteArray(4))).join('.') };
    }
    if (family === 23 && length >= 28) {
      const b = new Uint8Array(address.add(8).readByteArray(16));
      return { decoded: true, family: 'ipv6', port: address.add(2).readU8() * 256 + address.add(3).readU8(),
        address: Array.from({ length: 8 }, (_, i) => ((b[i * 2] << 8) | b[i * 2 + 1]).toString(16)).join(':'),
        scope_id: address.add(24).readU32() };
    }
    return { decoded: false, family, reason: 'unsupported_family_or_short_sockaddr' };
  }
  function install(h) {
    const listeners = [], missing = [], counts = {}, budget = 400;
    let emitted = 0, dropped = 0, terminal = 0, verified = false, sequence = 0, fetchSequence = 0;
    const fetchScopes = new Map();
    const currentThread = () => typeof h.Process.getCurrentThreadId === 'function' ? h.Process.getCurrentThreadId() : null;
    const activeFetch = () => {
      const stack = fetchScopes.get(currentThread()) || [];
      return stack.length ? stack[stack.length - 1] : null;
    };
    const emit = (kind, fields = {}, critical = false) => {
      counts[kind] = (counts[kind] || 0) + 1;
      if (emitted >= budget && !(critical && terminal++ < 8)) {
        dropped++;
        if (dropped === 1) h.send({ kind: 'probe_output_truncated', pid: h.Process.id, budget, utc: new Date().toISOString() });
        return;
      }
      emitted++;
      h.send(Object.assign({ kind, pid: h.Process.id, thread_id: currentThread(), sequence: ++sequence,
        active_fetch: activeFetch(), utc: new Date().toISOString() }, fields));
    };
    const readText = (p, wide = false) => {
      if (p.isNull()) return null;
      // Read one character at a time to avoid crossing an unreadable page after NUL.
      let text = '';
      for (let i = 0; i < 512; i++) {
        const n = wide ? p.add(i * 2).readU16() : p.add(i).readU8();
        if (n === 0) return text;
        text += String.fromCharCode(n);
      }
      return text + '[truncated]';
    };
    const status = () => ({ verified, hooks: listeners.length, missing: missing.slice(), emitted, dropped,
      counts: Object.assign({}, counts), native_error_state: 'observed_only', runtime_acceptance: false });
    const safe = (label, fn) => function (...args) {
      try { return fn.apply(this, args); }
      catch (e) { emit('probe_callback_error', { hook: label, error: String(e).slice(0, 200) }); }
    };
    const hook = (label, address, callbacks) => {
      if (!address) { missing.push(label); return; }
      const wrapped = {};
      for (const [k, fn] of Object.entries(callbacks)) wrapped[k] = safe(label + '.' + k, fn);
      try { listeners.push(h.Interceptor.attach(address, wrapped)); }
      catch (e) { missing.push(label); emit('probe_hook_unavailable', { hook: label, error: String(e).slice(0, 200) }); }
    };
    const instructionProbe = (label, address, fn) => {
      if (!address) { missing.push(label); return; }
      // Arbitrary instruction addresses use Frida's function callback form;
      // the onEnter/onLeave object form is reserved for function entry hooks.
      try { listeners.push(h.Interceptor.attach(address, safe(label + '.instruction', fn))); }
      catch (e) { missing.push(label); emit('probe_hook_unavailable', { hook: label, error: String(e).slice(0, 200) }); }
    };
    const find = (module, name) => {
      try { const m = h.Process.findModuleByName(module); return m ? m.findExportByName(name) : null; }
      catch (_) { return null; }
    };
    const exportHook = (module, name, callbacks) => hook(module + '!' + name, find(module, name), callbacks);
    try {
      const m = h.Process.mainModule;
      if (h.Process.arch !== expected.arch || h.Process.pointerSize !== expected.pointerSize ||
          m.name.toLowerCase() !== expected.name.toLowerCase() || m.size !== expected.imageSize) {
        throw new Error('Main module name, architecture, pointer size or image size differs');
      }
      let file;
      let bytes;
      try { file = new h.File(m.path, 'rb'); bytes = file.readBytes(expected.size + 1); }
      finally { if (file) file.close(); }
      if (bytes.byteLength !== expected.size || h.Checksum.compute('sha256', bytes) !== expected.sha256) {
        throw new Error('Main module on-disk SHA-256 or file size differs');
      }
      for (const [label, [rva, pattern]] of Object.entries(anchors)) {
        if (!bytesMatch(new Uint8Array(m.base.add(rva).readByteArray(pattern.length / 2)), pattern)) {
          throw new Error('Loaded instruction fingerprint differs: ' + label);
        }
      }
      verified = true;
      emit('probe_identity_verified', { image: m.path, image_base: m.base.toString(), sha256: expected.sha256,
        architecture: expected.arch, hooks_not_yet_installed: true });
    } catch (e) {
      emit('probe_identity_rejected', { error: String(e).slice(0, 240), hooks_installed: 0 }, true);
      return { status };
    }

    // Read exactly one environment variable, without enumerating the environment.
    try {
      const p = find('kernel32.dll', 'GetEnvironmentVariableW');
      if (!p) throw new Error('GetEnvironmentVariableW export unavailable');
      const get = new h.NativeFunction(p, 'uint32', ['pointer', 'pointer', 'uint32'], 'stdcall');
      const buffer = h.Memory.alloc(1024);
      const length = get(h.Memory.allocUtf16String('DEF_CFG_DIR'), buffer, 512);
      emit('selected_launch_context', { DEF_CFG_DIR: length > 0 && length < 512 ? readText(buffer, true) : null,
        env_result_length: length, env_overflow: length >= 512,
        cwd: h.Process.getCurrentDir() });
    } catch (e) { emit('selected_context_unavailable', { error: String(e).slice(0, 200) }); }

    const base = h.Process.mainModule.base;
    const src = (label, callbacks) => hook(label, base.add(anchors[label][0]), callbacks);
    src('warningFunction', { onEnter() { emit('configuration_warning_function_entered', { rva: '0x43510' }); } });
    instructionProbe('warningReference', base.add(anchors.warningReference[0]), function () {
      emit('license_warning_branch_observed', { rva: '0x4357d', qualification: 'warning string reference reached; dialog rendering is not observed' }, true);
    });
    src('fetch', {
      onEnter() {
        this.object = this.context.ecx;
        this.fetchThread = currentThread(); this.fetchId = ++fetchSequence;
        const stack = fetchScopes.get(this.fetchThread) || []; stack.push(this.fetchId); fetchScopes.set(this.fetchThread, stack);
        emit('configuration_fetch_entered', { rva: '0x13770', fetch_id: this.fetchId });
      },
      onLeave() {
        try { emit('configuration_fetch_result', { valid_flag: this.object.add(0x37c).readU8(), rva: '0x13770', fetch_id: this.fetchId,
          qualification: 'false flag has several possible causes; it is not proof that a license service stopped' }, true); }
        finally {
          const stack = fetchScopes.get(this.fetchThread) || [];
          if (stack[stack.length - 1] === this.fetchId) stack.pop();
          if (!stack.length) fetchScopes.delete(this.fetchThread);
        }
      }
    });
    src('fileCheck', {
      onEnter() { this.filename = readText(this.context.ecx.add(0x548)); },
      onLeave(ret) { emit('configuration_file_access_result', { path: this.filename, accessible: (ret.toUInt32() & 255) !== 0,
        qualification: 'low return byte summarizes fopen/fclose test; OS error not exposed by this function' }); }
    });
    for (const name of ['directory', 'path']) src(name, {
      onEnter(args) { this.output = args[0]; },
      onLeave() { emit(name === 'directory' ? 'resolved_config_directory' : 'resolved_config_path', { value: readText(this.output) }); }
    });
    for (const [name, direction] of [['sendHeader', 'send'], ['recvHeader', 'receive']]) src(name, {
      onLeave(ret) { emit('configuration_header_count', { direction, returned: ret.toInt32(), expected: 24, complete: ret.toInt32() === 24 }); }
    });

    // Count-only Winsock observation. Buffer pointers are deliberately never read.
    for (const name of ['send', 'recv']) exportHook('ws2_32.dll', name, {
      onEnter(args) { this.socket = args[0].toString(); this.requested = args[2].toInt32(); },
      onLeave(ret) { emit('socket_byte_count', { api: name, socket: this.socket, requested: this.requested,
        returned: ret.toInt32(), socket_error: ret.toInt32() === -1, last_error_snapshot: this.lastError }); }
    });
    for (const name of ['connect', 'WSAConnect']) exportHook('ws2_32.dll', name, {
      onEnter(args) { this.socket = args[0].toString(); this.destination = endpoint(args[1], args[2].toInt32()); },
      onLeave(ret) { emit('socket_connect_result', { api: name, socket: this.socket, destination: this.destination,
        return_value: ret.toInt32(), immediate_success: ret.toInt32() === 0, last_error_snapshot: this.lastError,
        qualification: 'a pending/nonblocking result is not final connection failure' }); }
    });
    exportHook('ws2_32.dll', 'gethostbyname', {
      onEnter(args) { this.host = readText(args[0]); },
      onLeave(ret) { emit('dns_result', { api: 'gethostbyname', host: this.host, returned_null: ret.isNull(), last_error_snapshot: this.lastError }); }
    });
    for (const [name, wide] of [['getaddrinfo', false], ['GetAddrInfoW', true]]) exportHook('ws2_32.dll', name, {
      onEnter(args) { this.host = readText(args[0], wide); this.service = readText(args[1], wide); },
      onLeave(ret) { emit('dns_result', { api: name, host: this.host, service: this.service, error_code: ret.toInt32() }); }
    });
    // File-name and status observation only. Content is never read here.
    for (const [name, wide] of [['CreateFileA', false], ['CreateFileW', true]]) exportHook('kernel32.dll', name, {
      onEnter(args) { this.filename = readText(args[0], wide); this.selected = /(?:^|[\\/])DEFORM_NLM_CONF$/i.test(this.filename || ''); },
      onLeave(ret) { if (this.selected) emit('config_file_open_result', { api: name, path: this.filename,
        opened: ret.toUInt32() !== 0xffffffff, last_error_snapshot: this.lastError }); }
    });
    for (const name of ['ExitProcess', 'TerminateProcess']) exportHook('kernel32.dll', name, {
      onEnter(args) { emit('process_exit_api_observed', { api: name,
        code: args[name === 'ExitProcess' ? 0 : 1].toUInt32(),
        target_handle: name === 'TerminateProcess' ? args[0].toString() : 'self',
        qualification: 'API entry observed; verify actual process exit separately; exit zero does not prove helper success' }, true); }
    });
    emit('probe_ready', { sha256: expected.sha256, hooks: listeners.length, missing_exports_or_hooks: missing,
      event_budget: budget, capture: 'metadata only; no license payloads; observation can affect timing' }, true);
    return { status };
  }
  return { install, endpoint, bytesMatch, expected, anchors };
})();

if (typeof module !== 'undefined' && module.exports) module.exports = JobHelperProbe;
if (typeof Process !== 'undefined' && typeof Interceptor !== 'undefined') {
  const installedJobHelperProbe = JobHelperProbe.install(globalThis);
  if (typeof rpc !== 'undefined') rpc.exports = { diagnosticstatus: () => installedJobHelperProbe.status() };
}
