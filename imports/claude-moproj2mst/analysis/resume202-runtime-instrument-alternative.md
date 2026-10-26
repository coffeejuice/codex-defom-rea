# CP202 runtime instrumentation capability diagnosis

The retained attach failure does **not** establish that REA lacks a Windows
Frida provider. The installed provider converts arbitrary backend failures
into the same `capability_unavailable` / `unsupported_provider` envelope.
The exact underlying attach failure remains unknown until the original
response's `details.reason` is recovered. A supported local API alternative
exists in the already installed Node Frida package, but successful attach is
unproven and it may encounter the same backend failure.

## Scope and observed evidence

This investigation only read tool metadata, `binary_session`, installed
implementation files, package declarations and existing converter reports.
No attach was retried; no target process, original application, GUI session,
mutable REA operation, installation, source edit, build or deployment occurred.
Only this report was written. Parent DMO and Claude work was untouched.

The earlier owned suspended Python child and its verified exact-handle cleanup
remain qualified by `resume202-frida-attach-selftest.md`. That report records
PID 33156 and the generic failure, but contains no underlying `details.reason`.
Do not reuse that PID. No accepted Close or original-process behavior follows
from that child experiment.

Read-only `binary_session({})` returned:

- REA package/server 6.7.0 at
  `C:\Users\alext\AppData\Roaming\npm\node_modules\rea-agents`;
  server started `2026-10-26T03:59:16.475Z`.
- Catalog combined SHA-256
  `96d8f6e6ea1b1ba13557216624a3461c738729d53c79b4b0100d1f3a3a32441c`.
- `open:false`, `cleanup_pending:false`, `analysis_provider_binding:null`.
- `tool_availability.groups`: `start_frida_session`, `close_frida_session`,
  `unload_frida_script`, `load_frida_script`, `resume_frida_session`,
  `instrument_with_frida`, `frida_session_status`, `list_frida_devices` and
  `list_frida_processes` all advertise `available:true`, `reason:available`.

This is advertised availability, not proof of successful injection. Crucially,
the open=false state and installed dispatch implementation exclude a missing
`open_binary` binding as a necessary repair for this target-free Frida lane.
Opening an unrelated binary would add no established remedy.

## Why the generic provider error appears

Paths below are relative to the verified installed REA package root, not this
checkout. These are shipped implementation observations, not executed tests.

1. `dist/server/registerFridaTools.js:60` registers the start handler and
   forwards an explicit attach PID to `FridaInstrumentationService.startSession`.
   It does not request an active binary or native analysis binding.
2. `dist/frida/FridaInstrumentationManager.js:39` dynamically imports `frida`
   and obtains its device manager. At lines 145–171 it selects the device,
   optionally resolves a process name, and calls `device.attach(pid)`.
   Failure of process-name lookup alone is explicitly tolerated.
3. The catch at line 222 routes failures to `providerError`. At lines
   1035–1059, every exception other than input/cancellation errors becomes
   `new AnalysisCapabilityUnavailableError("frida", operation, reason, ...)`;
   `reason` is the sanitized original exception text. This includes actual
   injection, target, permission and transport failures; the class alone
   cannot discriminate them.
4. `dist/mcp/analysisErrorProjection-B8teQwwD.js:425` maps that class to
   `unsupported_provider`; the standard user-facing text is generic.
   Its `providerErrorDetails` block at lines 789–794 preserves
   `{provider_id, operation, reason}`. Inspect the original tool error's
   `details.reason` before attributing a host or target limitation.

Therefore the established explanation is **error normalization**, with the
actual backend cause unresolved. Nothing reviewed proves a Windows prohibition,
missing Frida install, target-binding requirement, security-product block,
privilege mismatch or suspended-process limitation. Do not promote any of those
possibilities into a finding without the retained reason or a distinct test.

Implementation SHA-256 identities:

| Installed file | SHA-256 |
|---|---|
| `dist/frida/FridaInstrumentationManager.js` | `7755f69698ab26ba8fc1e326fd4b6ead546083695d6daed606e8d35b89d6c977` |
| `dist/server/registerFridaTools.js` | `f172d66205ca140323aa40491a49ee61165ff135a30259258e4e8c2706c48ac1` |
| `node_modules/frida/build/frida_binding.node` | `32c73230bdfd4b9a8f1f1871ef616001e486269919f18a111a195854352057dc` |

## Supported alternative and feasibility

Use the existing **Node Frida API directly** for a separately authorized,
owned-child qualification. `Get-Command node` resolves to
`C:\Program Files\nodejs\node.exe`. The installed package at
`C:\Users\alext\AppData\Roaming\npm\node_modules\rea-agents\node_modules\frida`
declares version **17.23.2** and includes a 135,663,104-byte native binding.
No install is needed to prepare this lane. It is the same backend family REA
uses, so it is a diagnostic/API alternative, not an established workaround.
An actual import under the selected Node ABI was not tested here.

The installed declarations establish the necessary API surface:

- `build/src/frida.d.ts:625`: `getLocalDevice()`.
- `build/src/frida_binding.d.ts:189`: `Device.attach(target, options?, cancellable?)`.
- Lines 518 and 522: `Session.detach()` and `Session.createScript(source, ...)`.
- Lines 549 and 551: `Script.load()` and `Script.unload()`.

A future launcher can retain the existing CreateProcessW suspended child
ownership/cwd/creation-time/watchdog lane, attach directly to its exact PID,
capture the raw exception name/message/code on failure, and detach explicitly
on success. Do not resume the child before hooks are confirmed, and retain
held-handle termination plus exit/absence verification independently of Frida.
Direct API detach does not terminate a process.

Only after an owned-child attach/load/detach/cleanup preflight succeeds could
this API carry the six-address script already specified in
`resume201-passive-oracle-probe-plan.md`. Preserve module hash/base checks,
ASLR translation, prehook bytes, finite capture endpoint and exact-process
cleanup. The addresses are original `0x1400154a4`, Qt `0x6539ffa0`, Qt
`0x653a0084`, original `0x14003a990`, `0x14003aa81`, `0x14003aaa2`.
The argument/return/thread/time observation policy remains unchanged.
Frida hooks inject instrumentation and may patch code internally; “passive”
here means no application API calls, synthesized input, value replacement or
intentional behavioral intervention. It is not a zero-perturbation trace.

Python 3.12 at `C:\Program Files\Python312\python.exe` reported no `frida`
module under both isolated ordinary import search and `-I -S`; no `frida`,
`frida-trace` or `rea` command resolved in this shell. These bounded checks do
not prove their absence from every environment. REA's exposed GDB contract is
Linux-only; its Rizin debugger contract requires a Linux TTY. Neither is an
established native-Windows replacement. Windows debugger API breakpoint tracing
would need a separate implementation, exception/continue/context handling and
cleanup qualification; no such ready six-address harness was established.

## Next smallest action

Recover the already-returned failed attach response's exact
`details.provider_id`, `details.operation` and `details.reason`, plus any
cleanup field, without running another attach. This is smaller and more
informative than switching tools. If the response was not retained, record
that loss explicitly. The next distinct execution experiment would then be
an approved direct-Node owned-child preflight capturing the raw backend
exception and verified cleanup. No original-process trace is qualified yet.

Access note: the first sandbox read of installed npm files was denied;
read-only escalated inspection succeeded. No automatic approval rejection,
configuration change or package acquisition occurred.
