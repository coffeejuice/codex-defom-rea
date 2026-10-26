# CP207 REA runtime-alternative feasibility audit

## Decision

No currently exposed REA tool or discoverable installed local debugger provides a documented Windows process workflow that can stop the owned `DEF_GUI_MO_64.exe` at the required original instruction addresses and return exact register plus memory snapshots without injecting instruction hooks. The feasibility result is **unavailable through the inspected supported interfaces**.

## REA capabilities inspected

- `mcp__rea__instrument_with_frida` is available. Its contract runs a caller-authored Frida script against an explicitly selected process (`attach` or `spawn`) and detaches at the observation window end. This is the only exposed general native runtime instrumentation interface applicable to this Windows target. It can be asked to capture context and guarded memory reads at selected addresses, but the tool does not provide a non-invasive Windows exception/debug-event snapshot mode. Implementing address callbacks with Frida would return to in-process instrumentation; exact instruction hooks are the method that diverged in CP206. A single function-boundary Frida callback is a smaller probe candidate, not evidence that it is non-perturbing.
- `mcp__rea__observe_native_calls` is explicitly for launching Mach-O under LLDB; it records function-entry arguments and auto-continues. It is not a Windows PE debugger and does not promise arbitrary instruction-site snapshots.
- `mcp__rea__start_gdb_session` documents persistent GDB sessions only on Linux. Its Windows Job Object behavior can terminate debugger inferiors, so it does not satisfy the owned disposable Windows process requirement here.
- `mcp__rea__start_rizin_debug_session` requires Linux `/proc` descriptor verification and a Linux TTY. Its backend support is build-dependent and it is not a documented Windows runtime path.
- Static REA operations (instruction inspection, disassembly, recorded-crash inspection) cannot produce live register/memory state at the original fault sites.

No REA runtime/debugger operation was invoked. This is a metadata-only assessment.

## Local debugger availability

Read-only PATH and common Windows Kits/WinDbg install-location checks found no `windbg`, `cdb`, `ntsd`, `x64dbg`, `x32dbg`, `gdb`, or `ollydbg` command/install in the checked locations. This does not rule out a debugger outside PATH or those locations, but none was discoverable by this bounded inspection. No debugger was launched or installed.

## CP206 comparison and limits

The targeted receipt is `analysis/resume206-original-null-owner-2026-10-26T11-33-28-746Z/summary.json`; the interpretation is recorded in `analysis/resume206-null-owner-probe-outcome.md`. CP206 used the pinned original v11.0.2 image and an owned suspended child with validated identity, then resumed once without GUI input. Four byte-verified mid-instruction Frida hooks were installed before resume. The first exception was an unmapped execute AV at RIP/access address `0x7ff632e30008`, outside a mapped module, with `RCX=0x32c63f0`; hook sequence and all four hit counts were zero. The process naturally exited `0xC0000005`, cleanup was recorded, and the fixture digest remained unchanged. CP205 on the same pinned image/project/cwd, without Interceptor hooks, instead captured the QtCore `QListData::size` read AV at `0x58` (`RCX=0x58`). Therefore CP206 shows probe-path divergence before any selected hook fired. It does not identify a particular hook or prove the source of divergence.

## Recommended next supported method

Preserve CP205 as the comparison gate. Since this interface set has no suitable Windows debugger, first use a supported Windows debugger/debug-event facility only if one is already available through the user's approved environment; do not substitute Linux-only REA GDB/Rizin. If none is available, the narrow REA-backed fallback is a separately scoped owned-process Frida observation at **one function boundary**, with no mid-instruction patch set, and with the exact CP205 first-AV tuple (module/RVA, access type/address, relevant register, and stack identity) as a hard parity gate. Treat any divergence as instrumentation-inconclusive and stop attribution. This fallback remains in-process instrumentation and is not equivalent to the requested non-injected snapshot method. The ownership, disposable fixture, no-GUI-input, natural-exit, and cleanup checks from CP205 must remain in force.

No application, debugger, GUI, mutable database, installation, or configuration was touched in this audit.
