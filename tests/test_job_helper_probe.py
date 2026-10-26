"""Offline behavior checks for the helper observer; no Frida or target execution.

Uses the existing Node runtime to execute the actual JS against mocked Frida APIs.
The mocks model only the APIs used here and cannot establish runtime compatibility.
"""
from pathlib import Path
import shutil
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tools" / "observe_job_helper.js"
NODE = shutil.which("node")
HARNESS = r"""
const assert = require('node:assert/strict');
const probe = require(process.argv[1]);
function harness(options = {}) {
  const events = [], hooks = new Map(), memory = [], exportNames = new Map();
  let closed = false, allocAt = 0x800000;
  class Pointer {
    constructor(n) { this.n = Number(n) >>> 0; }
    add(n) { return new Pointer(this.n + n); }
    toString() { return '0x' + this.n.toString(16); }
    toInt32() { return this.n | 0; }
    toUInt32() { return this.n; }
    isNull() { return this.n === 0; }
    block(size) {
      const m = memory.find(m => this.n >= m.at && this.n + size <= m.at + m.bytes.length);
      if (!m) throw Error('unmapped memory read');
      return m.bytes.subarray(this.n - m.at, this.n - m.at + size);
    }
    readU8() { return this.block(1).readUInt8(0); }
    readU16() { return this.block(2).readUInt16LE(0); }
    readU32() { return this.block(4).readUInt32LE(0); }
    readByteArray(n) { return Uint8Array.from(this.block(n)).buffer; }
  }
  const ptr = n => new Pointer(n);
  const map = (at, bytes) => { memory.push({ at, bytes: Buffer.from(bytes) }); return ptr(at); };
  const alloc = bytes => { const p = map(allocAt, bytes); allocAt += bytes.length + 16; return p; };
  const text = (s, wide = false) => alloc(Buffer.from(s + '\0', wide ? 'utf16le' : 'latin1'));
  const main = { name: options.name || probe.expected.name, path: 'C:\\trial\\DEFJobGroupSetup.exe',
    base: ptr(0x400000), size: probe.expected.imageSize };
  for (const [name, [rva, pattern]] of Object.entries(probe.anchors)) {
    const b = Buffer.from(pattern.replaceAll('??', '00'), 'hex');
    if (options.badAnchor === name) b[0] ^= 1;
    map(main.base.n + rva, b);
  }
  const module = {
    findExportByName(name) {
      if ((options.missing || []).includes(name)) return null;
      const p = ptr(0x900000 + exportNames.size * 16); exportNames.set(p.n, name); return p;
    }
  };
  const env = {
    Process: { id: 2468, arch: options.arch || 'ia32', pointerSize: 4, mainModule: main,
      getCurrentDir: () => 'C:\\PROBLEM\\R3\\CS', findModuleByName: () => module },
    File: class {
      constructor(path, mode) { assert.equal(path, main.path); assert.equal(mode, 'rb'); }
      readBytes(max) { assert.equal(max, probe.expected.size + 1); return new ArrayBuffer(probe.expected.size); }
      close() { closed = true; }
    },
    Checksum: { compute(type) { assert.equal(type, 'sha256'); return options.badHash ? '0'.repeat(64) : probe.expected.sha256; } },
    Memory: { alloc: n => alloc(Buffer.alloc(n)), allocUtf16String: s => text(s, true) },
    NativeFunction: function (address, result, args, abi) {
      assert.equal(exportNames.get(address.n), 'GetEnvironmentVariableW'); assert.equal(abi, 'stdcall');
      return (name, buffer, capacity) => {
        assert.equal(name.block(24).toString('utf16le'), 'DEF_CFG_DIR\0'); assert.equal(capacity, 512);
        const value = 'C:\\SFTC\\Configuration'; const b = Buffer.from(value + '\0', 'utf16le');
        b.copy(buffer.block(1024)); return value.length;
      };
    },
    Interceptor: { attach(address, callbacks) { hooks.set(address.n, callbacks); return { detach() { throw Error('not authorized in probe'); } }; } },
    send: event => events.push(event)
  };
  const installed = probe.install(env);
  const source = name => hooks.get(main.base.n + probe.anchors[name][0]);
  const exported = name => hooks.get([...exportNames].find(([at, n]) => n === name && hooks.has(at))?.[0]);
  const invoke = (callbacks, args = [], returned = ptr(0), extras = {}) => {
    const context = Object.assign({ context: { ecx: ptr(0) }, lastError: 10061 }, extras);
    if (typeof callbacks === 'function') callbacks.call(context, args);
    else {
      if (callbacks.onEnter) callbacks.onEnter.call(context, args);
      if (callbacks.onLeave) callbacks.onLeave.call(context, returned);
    }
    return context;
  };
  return { events, hooks, source, exported, invoke, installed, ptr, map, alloc, text, closed: () => closed };
}
"""


@unittest.skipUnless(NODE, "existing Node executable unavailable; no dependencies installed")
class HelperProbeTests(unittest.TestCase):
    def run_js(self, body):
        result = subprocess.run([NODE, "-e", HARNESS + "\n" + body, str(SCRIPT)],
                                capture_output=True, text=True, timeout=15)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_syntax(self):
        result = subprocess.run([NODE, "--check", str(SCRIPT)], capture_output=True, text=True, timeout=15)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_identity_rejection_has_no_hooks(self):
        self.run_js(r"""
for (const option of [{badHash:true}, {arch:'x64'}, {name:'wrong.exe'}, {badAnchor:'warningReference'}]) {
  const h = harness(option); assert.equal(h.hooks.size, 0); assert.equal(h.installed.status().verified, false);
  assert.equal(h.events.at(-1).kind, 'probe_identity_rejected');
}
assert.equal(harness({badHash:true}).closed(), true);
""")

    def test_verified_identity_and_selected_context(self):
        self.run_js(r"""
const h = harness(); assert.equal(h.installed.status().verified, true); assert.equal(h.closed(), true);
const context = h.events.find(x => x.kind === 'selected_launch_context');
assert.equal(context.DEF_CFG_DIR, 'C:\\SFTC\\Configuration'); assert.equal(context.cwd, 'C:\\PROBLEM\\R3\\CS');
assert.equal(h.events.at(-1).kind, 'probe_ready'); assert.equal(h.events.at(-1).missing_exports_or_hooks.length, 0);
""")

    def test_x86_configuration_flag_and_file_result(self):
        self.run_js(r"""
const h = harness(); const object = Buffer.alloc(0x800); object[0x37c] = 0;
Buffer.from('C:\\cfg\\DEFORM_NLM_CONF\0').copy(object, 0x548); const p = h.alloc(object);
h.invoke(h.source('fetch'), [], h.ptr(0), {context:{ecx:p}});
h.invoke(h.source('fileCheck'), [], h.ptr(0x12340001), {context:{ecx:p}});
assert.equal(h.events.find(x => x.kind === 'configuration_fetch_result').valid_flag, 0);
const result = h.events.find(x => x.kind === 'configuration_file_access_result');
assert.equal(result.accessible, true); assert.equal(result.path, 'C:\\cfg\\DEFORM_NLM_CONF');
h.invoke(h.source('fileCheck'), [], h.ptr(0x12340000), {context:{ecx:p}});
assert.equal(h.events.at(-1).accessible, false);
h.invoke(h.source('warningReference')); assert.equal(h.events.at(-1).kind, 'license_warning_branch_observed');
""")

    def test_header_counts_and_network_payload_not_read(self):
        self.run_js(r"""
const h = harness();
h.invoke(h.source('sendHeader'), [], h.ptr(24)); h.invoke(h.source('recvHeader'), [], h.ptr(12));
const results = h.events.filter(x => x.kind === 'configuration_header_count');
assert.equal(results[0].complete, true); assert.equal(results[1].complete, false);
// 0xdeadbeef is deliberately unmapped; a payload read would throw and fail this assertion.
h.invoke(h.exported('send'), [h.ptr(17), h.ptr(0xdeadbeef), h.ptr(24)], h.ptr(-1));
const result = h.events.at(-1); assert.equal(result.kind, 'socket_byte_count');
assert.equal(result.socket_error, true); assert.equal(result.returned, -1); assert.equal(result.requested, 24);
assert.equal(result.last_error_snapshot, 10061); assert.equal(h.events.some(x => x.kind === 'probe_callback_error'), false);
""")

    def test_header_attribution_is_limited_to_active_fetch(self):
        self.run_js(r"""
const h = harness(); const object = Buffer.alloc(0x800); object[0x37c] = 1;
const invocation = {context:{ecx:h.alloc(object)}};
h.source('fetch').onEnter.call(invocation);
h.invoke(h.source('sendHeader'), [], h.ptr(24));
assert.equal(h.events.at(-1).active_fetch, 1);
h.source('fetch').onLeave.call(invocation, h.ptr(0));
assert.equal(h.events.at(-1).fetch_id, 1); assert.equal(h.events.at(-1).valid_flag, 1);
h.invoke(h.source('sendHeader'), [], h.ptr(24));
assert.equal(h.events.at(-1).active_fetch, null);
""")

    def test_warning_reference_uses_instruction_callback(self):
        self.run_js(r"""
const h = harness();
const warningInstruction = h.source('warningReference');
assert.equal(typeof warningInstruction, 'function');
assert.equal(typeof h.source('warningFunction'), 'object');
h.invoke(warningInstruction);
assert.equal(h.events.at(-1).kind, 'license_warning_branch_observed');
assert.equal(h.events.at(-1).rva, '0x4357d');
assert.equal(h.events.some(x => x.kind === 'probe_callback_error'), false);
""")

    def test_sockaddr_endianness_and_short_buffer(self):
        self.run_js(r"""
const h = harness(); const b = Buffer.alloc(16); b.writeUInt16LE(2); b.writeUInt16BE(34445, 2);
b.set([127,0,0,1], 4); const addr = h.alloc(b);
h.invoke(h.exported('connect'), [h.ptr(17), addr, h.ptr(16)], h.ptr(-1));
const result = h.events.at(-1); assert.equal(result.destination.port, 34445);
assert.equal(result.destination.address, '127.0.0.1'); assert.equal(result.immediate_success, false);
const v6 = Buffer.alloc(28); v6.writeUInt16LE(23); v6.writeUInt16BE(34445, 2); v6[23] = 1; v6.writeUInt32LE(12, 24);
const decoded = probe.endpoint(h.alloc(v6), 28); assert.equal(decoded.address, '0:0:0:0:0:0:0:1'); assert.equal(decoded.scope_id, 12);
assert.equal(probe.endpoint(addr, 1).decoded, false);
""")

    def test_dns_and_exact_config_path_filter(self):
        self.run_js(r"""
const h = harness();
h.invoke(h.exported('getaddrinfo'), [h.text('Sim2'), h.text('34445')], h.ptr(11001));
assert.equal(h.events.at(-1).error_code, 11001); assert.equal(h.events.at(-1).host, 'Sim2');
const n = h.events.length;
h.invoke(h.exported('CreateFileW'), [h.text('C:\\private.key', true)], h.ptr(1)); assert.equal(h.events.length, n);
h.invoke(h.exported('CreateFileW'), [h.text('C:\\cfg\\DEFORM_NLM_CONF', true)], h.ptr(-1));
assert.equal(h.events.at(-1).opened, false); assert.equal(h.events.at(-1).last_error_snapshot, 10061);
""")

    def test_missing_export_and_callback_fault_are_explicit(self):
        self.run_js(r"""
const h = harness({missing:['recv','GetEnvironmentVariableW']});
assert.equal(h.events.some(x => x.kind === 'selected_context_unavailable'), true);
assert.equal(h.installed.status().missing.includes('ws2_32.dll!recv'), true);
h.invoke(h.source('fileCheck'), [], h.ptr(0), {context:{ecx:h.ptr(0xbad)}});
assert.equal(h.events.some(x => x.kind === 'probe_callback_error'), true);
""")

    def test_bounded_output_retains_exit_evidence(self):
        self.run_js(r"""
const h = harness(); for (let i = 0; i < 1000; i++) h.invoke(h.source('sendHeader'), [], h.ptr(24));
h.invoke(h.exported('ExitProcess'), [h.ptr(0)]);
assert.ok(h.events.length <= 409); assert.ok(h.installed.status().dropped > 0);
assert.equal(h.events.filter(x => x.kind === 'probe_output_truncated').length, 1);
assert.equal(h.events.at(-1).kind, 'process_exit_api_observed'); assert.equal(h.events.at(-1).code, 0);
assert.equal(h.installed.status().runtime_acceptance, false);
""")


if __name__ == "__main__":
    unittest.main()
