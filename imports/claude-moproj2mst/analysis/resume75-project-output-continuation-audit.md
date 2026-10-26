# CP75: project post-close failure coverage and smallest next edge

The existing private output factory is sufficient to test the shared conversion continuation after a real FileDevice reports a persistent post-close error. A new production callback is unnecessary. However, neither the CP73 scripted test nor a deliberately prepared physical device test establishes a physically reachable failure through the unchanged public entry or the original GUI's final status. Those are distinct claims.

## Frozen evidence inspected

The CP73 immutable archive SHA-256 rehashes to `2df50c2697594d54d76911f487db0284fc0f371667872ac09e2b0c18e50e7180`, matching its receipt; recorded source tree is `25f0af4c32432a0dc082ca1469f64ee8194fafa80a6e69b4331ec4652640ee5f`. Its focused log for `m2m.moproj.project_output_device` reports 0 of 1 cases failed, SHA-256 `1e2d3c9ca5a182c9f440e943dd07fabb136fc9141b0e8df207de4fb4adc039e8`; `m2m.s.save_io` reports 0 of 71 cases failed, SHA-256 `48c2331e5b892f3e05be67b89300b12c15bf5a94bb34154469d7e7dc9439ce05`. Both retained log hashes were rechecked against validation records. Full classification was 212 passed, 6 with logged gaps, 1 with logged differences, zero failed/skipped/incomplete. Objective completion is explicitly false.

Archive source bindings:

| Component-relative source | SHA-256 |
|---|---|
| src/moproj/project_output_device.hpp | 586132f1b480112b5b3d4152b35aca508114fe65a93e786d0b11d2ba8cbec669 |
| src/moproj/write.cpp | 70ed65c4c2581ccbafaf123ab70d1fefd0d4747701fd72b17cf6ce8ab1f96717 |
| src/core/conversion_call_services.hpp | 8fc9fe687a5a3a187d9f913fc7ba1b5ae62bfdeb97975a9ebcedf1ffdf492a6a |
| src/core/convert.cpp | 1dccfc3e5e99d592e0383a096970c029d061774c367c16e39ea907203d180f84 |
| tests/moproj/test_project_output_device.cpp | 8eb2ae13ab0cf60d3509a601129f13debf738b67841f46a2e198468934e47c2c |
| tests/s/test_save_io.cpp | c6a2f5c36c928c151cce9575c144f0588e337b64d00291b361b03289d5a75997 |

The component root is `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst`. Receipts, original static captures and this report are under its converter takeover root `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## What CP73 actually establishes

The writer-level test delegates real file open/write/flush/close, then returns declared status 7 in one branch while separately requiring the real FileDevice error to be zero. The paired integration case uses `detail::convertDetailedWithServices`, which invokes the same private `run()` as ordinary convert/convertDetailed. The factory is consulted only at native project output. It proves later Boolean BWZ/INI, MST and deterministic remesh output after a normal project-writer false, identical later bytes to the control, final adapter M2M_E_WRITE and matching returnedCode. No copied coordinator is involved.

This is empirical candidate coverage through a private entry sharing public core code. The old public calls supply no services, so it is not an executed fault through public convertDetailed itself and does not test the DLL C ABI's transport of that failure. Static binding to the same run/result mapping supports the narrower shared-code conclusion. Merely adding another private callback would still not make the call public-entry execution.

For the original, retained `analysis/wave3-project-close-status-contract.json` binds original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Original project writer closes at 0x140098dc8 and samples QFile.error at 0x140098dd3. `analysis/save-semantics.md` and retained Save captures show project false does not gate the later property/MST stages; the top Save returns its earlier check/status local, not the combined project/MST Boolean. M2M_E_WRITE is the adapter's mapping, not an original GUI Save status. GUI dirty state, action state, timer delivery and subsequent Close behavior after that error are not established by CP73's artifact/status tests.

## Why a simple physical lock does not supply the missing persistent error

The current writer inserts one complete serialized UTF-16 document into Stream and then closes. Stream's text threshold and FileDevice's byte block are both 16,384. Stream flush encodes the whole pending string, calls device.write once and, after positive progress, device.flush.

For an ordinary initially empty real FileDevice under a byte lock:

- A small document remains in Stream until aboutToClose. FileDevice has already saved an initially successful empty-buffer flush before emitting that callback. If the callback write/flush fails, successful handle close clears the later file error. Writer may therefore return true despite sticky Stream failure.
- A large UTF-8 document is normally larger than the FileDevice block and takes the direct-write route before Close. A locked direct failure does not populate the QFile byte buffer. The initial close flush is again empty/successful, so successful handle close clears its error. Positive direct partial progress also does not itself create pending QFile bytes.

The existing locked direct-writer case exercises this clearing behavior. These are bounded source paths, not a proof excluding every unusual native engine/close failure or all encoding inputs. An after-Open/before-output-lock callback would solve input-reader timing but would not by itself change this buffer-state problem. An early manual aboutToClose, a forced Stream flush or chunking/withholding adapter flushes changes serialization timing and must not be called an unchanged native writer test.

## Smallest deterministic next candidate edge: real persistent status, artificial initial buffer

Add one bounded integration case using the existing private factory and no production changes. Suggested case: `save_project_real_persistent_device_error_continues_properties_mst_and_remesh` in exclusively assigned `tests/s/test_save_io.cpp` after root releases that shared file. Reuse its ordinary Boolean Rig, explicit remesh fallback, details checks and existing DelegatingProjectOutput; do not alter conversion ordering or public settings.

The test factory alone prepares this declared fault profile:

1. Construct real FileDevice on the output path after input Open has completed; require opened.
2. Write one byte into that real device before handing it to the writer, without flush. Require write returns 1 and physical size remains zero. This deliberately seeds QFile's byte buffer; it is an artificial initial state, not serialization performed by the original project writer.
3. Open a second disposable handle with read/write access and read/write sharing. Lock byte zero via LockFileEx with LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY; retain the handle and lock through the writer's actual Close and error query.
4. Return the existing delegating adapter with reportedError=-1. No status override, preflush, altered chunking, early callback or FileDevice setter is permitted.
5. Let the unchanged serializer and close run. The seeded byte remains pending when the first native write/flush is blocked. The initial Close flush must therefore fail; its saved false preserves actual FileDevice error 2 after handle close. Assert actual opened, serializer write attempt, normal close return, callback while open and exactly one real post-close error query returning 2. Do not demand positive accepted serializer bytes on this failing profile.
6. Return normally to the same run. Assert project Attempted/Returned true and Succeeded false, successful MST flags, deterministic BWZ/INI/MST/remesh bytes equal to ordinary control, and final Result/returnedCode M2M_E_WRITE with the project diagnostic.
7. Release the lock safely on all exits before reading the main output; its zero-length candidate artifact is an expected consequence of this explicit seeded-buffer/byte-lock profile only. Retain RAII cleanup and isolate every write beneath existing configured scratch. Do not infer original partial bytes or fresh-project parity.

This is an implementation-ready proposal, not an executed result. The smallest useful increment is one test plus a test-local factory holding the lock; existing production interfaces suffice. It would replace scripted status with a measured physical FileDevice status while preserving the shared serializer/close/caller path. It still starts from an injected nonempty device buffer, so it cannot close the unchanged-public-path physical-fault gap.

## Remaining boundary

Do not introduce a production callback solely to chase a stronger label. To claim an unchanged public physical failure, first identify a real failure condition reachable by that ordinary output device and single-insertion serializer, then calibrate it without the seeded state or status override. To claim original status/lifecycle parity, obtain separately permitted fresh original evidence of the same failure and post-Save/Close state. Neither follows from another private hook. Tokens are unavailable; no speed or token saving is claimed.

No source, test, build, GUI, runtime fault injection or mutable REA action was performed for this audit. Only this report was written.
