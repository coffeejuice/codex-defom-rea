# CP202 attempt 2: QtCore crash offset and hook coverage

## Finding

The recorded fault is at `QtCore4_64.dll` RVA `0x32c0`. The retained QtCore export table maps that exact RVA to the exported function `QListData::size() const` (`?size@QListData@@QEBAHXZ`). The installed QtCore DLL has SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, matching the recorded original DLL identity. This is a real code entry and not the previously examined `0x12a0e4` location; the latter belongs to a different crash record and must not be used to explain this event.

Attempt 2's six interceptors were placed at four `DEF_GUI_MO_64.exe` RVAs (`0x1547e`, `0x3a990`, `0x3aa86`, `0x3aaa2`) and two `QtGui4_64.dll` RVAs (`0x39ffa0`, `0x3a0084`). None is in `QtCore4_64.dll`, and none is at RVA `0x32c0`. Thus the reported instruction address is outside every instrumented site. The crash may still be indirectly related to interception or timing; the event record has no faulting registers or call stack to establish that.

**Assessment: unrelated to the hooked instruction sites, but instrumentation causality remains unknown.** The attempt 2 trace is incomplete: it reports process termination, `captureComplete: false`, `coverageComplete: false`, all six hooks missing in final coverage, and no continuous observation. It contains two callback records (`prepared_visibility_request` and `slot_entry`) before termination. Those records do not prove the other hooks were installed/reached or that the process completed ordinary startup. No Save action is identified.

## Static context and limits

The export table also places `QListData::isEmpty() const` at RVA `0x32d0`, immediately after `size()`; this is consistent with a small Qt container helper region. Export names establish symbol attribution for the entry, but do not establish the failing memory operand, the caller, or whether a caller supplied invalid list state. The earlier retained disassembly around `0x12a0e4` is unrelated. No matching disassembly, call stack, or usable dump was available for `0x32c0`; the archived WER report directory was inaccessible to this read-only session. Consequently I cannot identify the caller or distinguish a baseline invalid object from an instrumentation-induced state/timing issue.

Evidence used:

- `analysis/resume202-original-probe-attempt2-crash.md` (Event 1000/1001 identity, exception and timestamp)
- `analysis/resume202-original-probe-attempt2-trace.md` (retained trace and coverage qualifications)
- `analysis/wave2-save-diffs-qtcore-exports.txt` (RVA `0x32c0` export mapping)
- `admin/frida_original_save_probe.mjs` (the configured hook RVAs and modules)
- Installed `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll` hash, matching the retained identity

## Smallest useful non-GUI discriminator

Use the same converter command, fixture, environment, and helper lifecycle for a paired run: first the no-Frida baseline, then the identical run with the existing passive hooks. Do not send GUI input or invoke target APIs. Keep process supervision and event collection outside the target, and collect a full user-mode dump on any crash. A baseline crash at the same `QListData::size()` fault supports a pre-existing/runtime-path defect; a clean baseline plus a reproducible hooked crash supports an instrumentation association but still requires stack/dump comparison for causality. If only one follow-up is feasible, the no-Frida baseline with dump-on-crash is the highest-value discriminator. A static dump-only inspection would improve caller attribution but, by itself, cannot separate baseline from instrumentation because it observes only the already-hooked attempt.
