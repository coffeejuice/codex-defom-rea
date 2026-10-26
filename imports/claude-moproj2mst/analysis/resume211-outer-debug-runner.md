# CP211 outer hardware observer decision core

**HOLD for original execution. PASS for 71 harmless tests of the actual decision code and 13 fixed harmless native integration/cleanup cases.** This tranche implements the staged observer and a separately restricted fixture transport. Both `ORIGINAL_RUN_ENABLED` and `ORIGINAL_READY` are literal `False`. `FixedOriginalTransport.launch()` also unconditionally raises HOLD. There is no original launch implementation hidden behind those guards. See `analysis/resume211-outer-debug-native-transport.md` for the native evidence and its limits.

The implementation is `admin/original_outer_loader_debug_probe.py`. The synthetic fixture `admin/test_outer_loader_debug_probe_fixture.py` uses this exact imported `OuterObserver`, not an alternate mock state machine. It synthesizes stopped-event inputs and readback acknowledgements, launches no process and accesses no project. The added native fixture `admin/test_outer_loader_debug_native_fixture.py` owns only fixed harmless interpreter/native-code children. Across both suites no original, GUI, disposable/source project, source build, deployment, external fixture modification, shared plan/state edit or app-control acquisition occurred.

## Identity and evidence boundary

The pinned future target is the original `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Version 11.0.2 is the installation version. CP207 retains a null PE version-resource reading; this work did not acquire a new version-resource reading or verify the currently installed file. No reconstructed executable is involved.

The constants preserve the CP207 fixed argv `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, cwd `C:\PROBLEM\p202a`, source fixture `evidence/gui_mo/oracle_b/98_B_DS3F_before`, and manifest digest `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`. The script supplies no PID, attach, arbitrary executable, command, path or address interface. These constants identify the proposed future capture; they do not represent a preflight receipt from this synthetic run.

Read the subtree AGENTS, TAKEOVER, charter and bounded takeover/import state; CP210 outer-capture plan, loader static/C++ qualification and child-hardening source/report/review; CP209 independent outer-loader review; and the CP207 original summary, runtime identities and source-fixture receipt. The CP210 hardening source still hashes to `0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee`. Its review accepts a bounded single-slot child suite, while explicitly holding original adaptation and real Win32-failure/unwind readiness. This new code does not inherit those native passes.

The reverse-engineer-anything skill's distinction between static and observed runtime evidence applies here. CP207's `cp205FaultPath=true` and `same_fault_path_producer_returned_zero` describe its later producer observation, not the new outer prerequisites. The selected caller continuation is **RVA `1c94`**, consistent with CP209's byte-qualified `1c8f: call 1ca30`. The erroneous `1c99` continuation is an explicit negative fixture case. CP210 remains the source of the five proposed caller-side scalar stages; this tranche did not independently re-disassemble their original bytes.

## Implemented decisions

The core proposes DR0=`base+1ca30`, DR1=`base+1c94`, and DR2 disabled until selection. DR3 remains zero/disabled. Local execution enables are `DR7=0x5` for entry/return and `0x15` with the staged slot; execution/length fields remain zero. DR2 stages `1cf11 → 1d08a → 1d0bd → 1d0d8 → 1d0fb`. Every change disarms decision acceptance until an exact typed debug-register readback is acknowledged. The native fixture applies these through actual `SetThreadContext` calls and verifies readback; original hardware placement remains unimplemented. Hardware-hit comparison permits only the observed fixed reserved DR7 bit 10 difference; all other ownership fields remain exact.

Each thread is registered before its events can be consumed, with monotonically increasing TID generation and an exact creation FILETIME string bound to the owned process identity. Unexpected nonzero DR0–DR3/DR6/DR7 baselines are rejected, including disabled registers. Contexts, exact exception address/chance, DR6 breakpoint ownership and BD/BS/BT causes, unchanged desired register ownership, ordered event sequences and monotonic elapsed time are checked. A stage cannot migrate to a different thread or reused TID. The engine accepts only typed unsigned register values and rejects malformed contexts/readbacks.

Selection requires a successful return-marker read equal to `base+1c94`, exact decoded B98 path and nonnull entry RCX/RDX. There is no prefix or case-folded path match. The adapter must distinguish a read/decoder error from an unrelated successfully decoded path. Read errors invalidate the observation. The selected occurrence retains entry RSP E, main identity, QString address, path, TID/generation and occurrence number.

The stage frame must equal E minus a separately supplied qualified body delta; the selected continuation must equal E+8. Every stage and return also requires the guarded original return marker at E. No original body delta is supplied or invented. The fixture's `0x48` delta is explicitly synthetic and cannot be used as an original contract. Recursive entries and overlapping outer calls, including cross-thread/unrelated overlap, invalidate collection rather than leave a stale active frame.

The first stage treats nonzero lookup as duplicate identity. Subsequent zero results identify factory, cast and Project-accessor syntactic routes. Acceptance uses `RAX & 255`, retaining full RAX. An observed failure disables DR2 and awaits selected outer return; absence of later stages is then expected. A required missing/out-of-order stage is incomplete. Only a completed same-frame stage prefix and selected normal zero return can support the stored null-route description. A normal nonzero outer result is recorded as divergent. Five successful prerequisites followed by zero preserve the unknown later cause; no sixth cause or allocation explanation is invented.

At AV input, the compact core is emitted before proposed enrichment, causal collection stops, and the required disposition is `DBG_EXCEPTION_NOT_HANDLED` for both chances. The core checks chance ordering, same-thread generation and equal captured pair. Its CP205 input contract requires QtCore4_64.dll RVA `32c0`, read `0x58`, RCX `0x58`, parameter count two, app returns `a7510 → eb1ec → eaa7a`, callback returns `2a725 → 1cae`, one eligible closed zero-return occurrence on that thread and explicit qualified module/unwind/association inputs. A first AV on a different path cannot match. Interrupted frames, multiple closed occurrences, wrong-thread AVs, missing first chance or changed pair remain incomplete/indeterminate.

The qualification booleans in synthetic AV inputs test the acceptance predicate only. They do not establish a native unwind or module qualification. Summary field `cp205_input_contract_matched` deliberately names this distinction. Every summary remains `status=HOLD`, `original_ready=false`, `native_cleanup_verified=false` even for a matching synthetic AV pair.

The fixed release budgets remain 30 seconds, 4,096 events, 16 selected entries and 128 scalar records. Reaching a cap invalidates collection; no silent drop advances state. The synthetic engine checks adapter-provided monotonic timestamps, so it does not implement a native wall-clock deadline while blocked inside a Windows wait. Unknown exceptions emit the pass-through disposition and invalidate the observation. The transport must continue/drain outstanding events independently after any decision exception.

## Validation and retained receipts

Executed from the subtree:

```text
python -I -S admin/test_outer_loader_debug_probe_fixture.py
```

Final synthetic result: **71/71 passed**, exit 0, Python 3.12.10 x64. Complete per-case input/decision records are retained in `analysis/resume211-outer-debug-fixture.jsonl`; summaries, exact interpreter version and hashes are retained in `analysis/resume211-outer-debug-fixture.json`. The final timer reported 0.016 seconds; no matched baseline or speed gain is claimed. Token usage is unavailable. The initial 68-case tranche was extended with reserved-DR7 acceptance/rejection and an unconditional fixed-original skeleton test.

Tests cover all five syntactic failure routes; acceptance AL versus high RAX bits; all-success/nonzero outer and unknown later zero; wrong caller/prefix/case paths; early unrelated zero; partial/lost stages; frame reuse/read failures; recursion and cross-thread overlap; thread generation reuse; all six nonzero debug baselines; malformed contexts/debug values; unknown/multiple hardware causes; unacknowledged register changes; process identity/sequence/time violations; cap reaches; matching/divergent/unqualified CP205 contracts; interrupted outer frames; changed/lost AV chances; ambiguous closed occurrences; durable serialization/write failure propagation; and the closed CLI interfaces.

`JsonlSink` flushes each serialized record and propagates serialization/write failure. This does not claim fsync durability, native continuation parity or handle cleanup. Complete input events and emitted decisions are persisted for these synthetic cases; no original event stream was recorded.

Final hashes:

| Artifact | SHA-256 |
| --- | --- |
| Observer | `1079b0661b23e5a11a3242c860b640481a04af4ad011daf9a0ad06990c54298b` |
| Synthetic fixture | `da02b208eb6748dabc23a386e4e597372a8cdf1597c70be5e14b801998012e66` |
| Raw synthetic records | `63230b7b270698e7c5ac384b2eaee41dd75364fd6fd379ce61106540f38d5503` |

## Remaining original-release work

1. Complete the unconditionally held fixed original transport with exact target/fixture preflight, pinned loaded module/byte spans, qualified loader breakpoint and independently derived original frame relation. The harmless fixture's owned-child transport is implementation evidence only; preserve the exact source fixture and fail on any original identity mismatch.
2. Implement bounded original QString and guarded memory reads, native x64 unwind/module association, and strict closed-occurrence/main-postOpen linkage. Raw stack-word scanning cannot satisfy this gate.
3. Independently review original-context restart/arm policy. The harmless native fixture proves actual staged hits/normal return, creation-event arm, integer/status preservation with identified reserved-bit normalization, baseline restoration and two natural AV chances for its own fixed code. It does not verify the original's thread/frame/exception behavior or original observation neutrality.
4. Independently review exact-handle ownership and continuation/drain mechanics before original adaptation. Those mechanics now have harmless native receipts, including acquisition generations, retained creation handle, checked continuations, automatic event-handle closures and manual image/DLL closures. They remain unproved against every real API failure and the original's event stream.
5. Test partial mutation, deadline while no event is pending, failing continuation/close/readback/termination APIs and debugger death. The native suite tests one actual invalid-handle `SetThreadContext` error, plus control-flow failures and exact-child cleanup/independent absence. It does not reproduce partial mutation on a valid handle or the listed untested failures. Forced cleanup never counts as natural capture completion. The decision core alone still performs no OS cleanup.
6. Bind final transport/observer/fixture hashes and actual persisted native receipts in independent release review before root considers enabling the original-run guard. Do not combine Frida with this debugger to fill its unwind gap without a separate instrumentation review.

The smallest safe tranche is therefore complete and reviewable, while original execution remains explicitly HOLD. No app-control session was acquired or claimed released. The GUI lifecycle restriction remains in force and is not bypassed by this offline decision core.
