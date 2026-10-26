# CP211 fixed native fixture transport

**PASS for 13 fixed harmless native cases and 71 synthetic actual-core cases. Original capture remains HOLD.** The runner now contains a fixed original transport contract and an unconditionally disabled original launch method, plus a separately restricted harmless transport that exercises real Windows debug events through the CP211 `OuterObserver`. No original executable, GUI, project fixture, build or deployment was run or changed.

The original remains installation v11.0.2 `DEF_GUI_MO_64.exe`, pinned SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; CP207's version-resource reading is null. This tranche does not freshly verify that installed image. No reconstructed executable is involved.

## Boundary and source

`admin/original_outer_loader_debug_probe.py` owns the observer, `FixedOriginalTransport` contract and `run_harmless_native_fixture`. `admin/test_outer_loader_debug_native_fixture.py` supplies only fixed native fixture roles. `admin/test_outer_loader_debug_probe_fixture.py` still exercises the same observer synthetically. No shared plan/state, converter source, CP210 debugger source or Frida implementation was edited.

Both literal original flags remain false. `FixedOriginalTransport.launch()` raises HOLD without importing native APIs, creating a process or falling back to a fixture. The original CLI opt-in also returns HOLD. There is no executable/PID/attach/arbitrary command interface. The only `CreateProcessW` in the module has its executable fixed to the current Python interpreter and its script fixed to `test_outer_loader_debug_native_fixture.py`, with isolated Python flags and one fresh fixture token. It uses `DEBUG_ONLY_THIS_PROCESS | CREATE_NO_WINDOW`, no inherited handles and the fixed fixture script directory as cwd.

The native ABI declarations are loaded lazily from the unchanged CP210 `child_debug_event_fixture.py`, only after requiring its exact source hash `0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee`. Import does not invoke its suite. The code verifies x64 EVENT size 176, CONTEXT size 1232 and RIP offset 248; CP210's helper supplies the 16-byte-aligned CONTEXT allocation.

Final observer SHA-256 is `1079b0661b23e5a11a3242c860b640481a04af4ad011daf9a0ad06990c54298b`; native fixture SHA-256 is `1fe65eb982a3b646c9f8dcfc48ed80e9c2c451909cc855b12f989f5ed6605752`. The JSON receipt binds those sources, interpreter/tool identities, each raw file hash and source snapshot paths. Exact source copies are retained under `analysis/resume211-outer-debug-native-sources/`.

## Actual native integration

Each child is a fresh owned Python 3.12.10 x64 process. The debugger retains a separate duplicate creation handle and verifies queried process image path/hash, primary TID and exact creation FILETIME text before continuing its event stream. The actual interpreter module base is recorded separately from the fixture's private code allocation at `0x1a0000000`. That private allocation is not an original ASLR base or a loaded PE module.

The child writes fixed harmless machine-code spans into its own RW allocation and changes protection to execute/read. It publishes a fresh token marker only after allocation. The debugger verifies every expected instruction/data span and its hash before accepting hardware hits. The synthetic labels deliberately use the original proposed RVAs to exercise the real observer mapping; their bytes and frame relation establish no original instruction provenance. The fixture uses a direct UTF-16 path buffer, not an original QString, and independently verifies that fixed pointer and bounded contents.

DR0 observes fixture `1ca30`, DR1 observes fixture continuation `1c94`, and DR2 advances through the five fixture stage labels. DR3 stays disabled. Every created thread receives its own zero-baseline check and before/desired/fresh-readback record while stopped at its creation event. Each hardware hit is fed to the same `OuterObserver.hit` used by the synthetic suite. Context writes preserve the captured integer registers and architected status/control flags, setting RF only for restart. Every staged write gets fresh exact debug-register readback before the core accepts another event.

The normal fixture yields seven owned hardware hits: entry, five stages and continuation. It returns the expected `0x4444` through the actual native call to ctypes. The duplicate fixture yields entry, first stage and continuation, returning zero; later stages remain correctly disabled. Its full entry/body/return stack relation is derived from its own emitted `sub rsp,48h` and native call instruction. This does not provide the original body delta.

The AV fixture completes the seven-hit outer sequence, then creates one native thread starting at its fixed read-from-`0x58` instruction sequence. This avoids ctypes' synchronous SEH wrapper. Both AV chances are recorded and continued with `DBG_EXCEPTION_NOT_HANDLED`; the process exits naturally with `0xc0000005`. At first AV, all tracked owned debug-register states are restored/read back while the process is stopped. Its core explicitly labels its memory as a private harmless executable fixture, leaves original module/native unwind qualifications false and cannot match CP205. The native fault is evidence of exception delivery mechanics, not original-path parity.

Each debug event and continuation has an ordered receipt. Caller, retained duplicate, event process/thread, image-file and DLL-file handles are tracked by acquisition generation, including numeric-handle aliases and reuse. Sibling handles are registered before fallible receipt writes. Image/DLL file handles close explicitly; event-owned process/thread closure is marked only after successful exit-event continuation. Immediately checking invalidity preserves the observed closure evidence. Successful continuation clears the pending flag before receipt output, preventing a later write error from retrying an already continued event.

On injected failure, the debugger restores all mutated owned contexts while a debug event is pending, terminates only the exact created process handle when required, continues any pending event, registers resources during the failure drain and drains through checked exit continuation. It waits for the exact retained process to signal before closing caller/retained handles. A separate non-debugger parent checks PID absence after each owning debugger subprocess exits. No debugger detach or GUI-control API is used or claimed as app-control release.

## Final-source results

Executed:

```text
python -I -S admin/test_outer_loader_debug_probe_fixture.py
python -I -S admin/test_outer_loader_debug_native_fixture.py
```

Synthetic final-source result: **71/71 passed**, exit 0. Native final-source result: **13/13 passed**, exit 0, 1.953 seconds reported suite wall time. This is a measurement for this run, not a matched speed comparison or token-saving claim. Token usage is unavailable.

| Native case | PID | Events / continued | Recorded outcome |
| --- | --- | --- | --- |
| Normal staged return | 42372 | 49 / 49 | Seven hardware hits, natural exit 0 |
| Duplicate early zero | 12016 | 45 / 45 | Three hardware hits, natural exit 0 |
| Native AV | 36004 | 51 / 51 | Seven hardware hits, AV first/last pair, natural exit `c0000005` |
| Before first create event | 26796 | 3 / 3 | No context was mutated; forced exact-child exit `e211` |
| After create event | 21600 | 3 / 3 | Baseline restored, forced exit `e211` |
| Before context set | 9428 | 3 / 3 | Baseline preserved, forced exit `e211` |
| After context set/before readback | 31708 | 3 / 3 | Attempted mutation tracked before API call; restored, exit `e211` |
| After verified arm | 40264 | 3 / 3 | Baseline restored, exit `e211` |
| After thread event | 33960 | 7 / 7 | New thread resources accounted for, restored, exit `e211` |
| Before hardware continuation | 19236 | 41 / 41 | Outstanding event resolved, baseline restored, exit `e211` |
| After first stage hit | 30768 | 42 / 42 | Staged DR2 restored, exit `e211` |
| Before exit continuation | 12676 | 49 / 49 | Exit event continued, recorded exit 0 |
| Actual invalid-handle SetThreadContext | 2604 | 3 / 3 | Error 6 retained; baseline preserved, forced exit `e211` |

All 13 rows have exact-process-signaled=true, handles-closed=true, pending-event=false, no cleanup errors, matching event/continuation counts and independent parent OpenProcess error 87. In the before-exit-continuation row, `TerminateProcess` returned false/error 5; it is preserved as a failed termination attempt. The already delivered exit was continued successfully and the exact process handle signaled. This row does not count as a successful termination API call.

The three natural cases alone have `native_capture_complete=true`. All ten failure cases have forced-cleanup=true and native-capture-complete=false. Their fixture pass means their expected cleanup outcome occurred. The first pre-create failure has no context restoration field because no context mutation occurred, rather than claiming a nonexistent restoration.

Retained aggregate receipt: `analysis/resume211-outer-debug-native-fixture.json`. Its per-case raw files have unique suffixes and contain **1,751 parsed records** in this final run. They retain scalar decisions, event/exception contexts, proposed/observed debug state, module/site qualifications, handles and every continuation. The aggregate binds each raw SHA-256, sources and independent parent checks. Per-case intermediate stdout is decoded into that receipt; the command prints only bounded case summaries. Older successful receipts are retained in `analysis/resume211-outer-debug-native-history/`.

## Initial rejection and precise normalization

The first native iteration failed closed on its first hardware hit: the observed DR7 acquired fixed reserved bit 10 (`0x5 → 0x405`). Its cleanup readback also exposed an EFLAGS reserved-bit-1 representation difference (`0x202 → 0x200`). The original code required equality for both and rejected collection. That failed receipt/raw stream remains in `analysis/resume211-outer-debug-native-attempt1/`; it records forced exit, exact child signal, handle closure and independent absence, plus the rejected restoration-equality check. It is not counted as a natural or successful integrated capture.

The final hardware ownership check excludes only DR7 bit 10 from its expected-versus-observed comparison. Readback after writes is still exact. The integer/status preservation comparison excludes only EFLAGS bit 1; RF must match the explicitly requested restart/restoration value and every other captured status/control bit and integer register remains exact. The raw values before and after are preserved. This host representation observation is bounded by these fixtures. Intel documents DR7 bit 10 as fixed one and EFLAGS bit 1 as reserved; that classification supports the narrow masks, while the Windows receipts supply the observed behavior. See the [Intel Software Developer's Manual](https://cdrdv2-public.intel.com/868137/325462-089-sdm-vol-1-2abcd-3abcd-4.pdf) and [Intel EFLAGS reserved-bit description](https://www.intel.com/content/dam/support/us/en/documents/processors/quark/sb/intelquarkcore_devman_001.pdf).

Two synthetic tests independently cover accepting only the DR7 fixed-bit difference and rejecting another DR7 control-bit change. The failed source was retained by exact reversal of the two normalization edits and independently SHA-256 matched to its original receipt (`3a1e03c692d0d1be40bae1db07f0a8b2d6afebf64a2a01ab07a6717b61839b99`); that is a reconstructed hash-verified source snapshot, not a contemporaneous copy. Later and final native sources are contemporaneously snapshotted before launching their suites.

## Original-release blockers retained

The fixed original contract preserves the exact CP207 target, argv, cwd and fixture digest, owned handle/event requirements and technical HOLD reasons. It does not perform original preflight or create the original process. The remaining CP210 gates are explicit:

1. Independently verify original PE/loaded instruction spans, actual call boundaries and body stack relation. Do not substitute the native fixture's spans/delta or silently reuse its private allocation as ASLR evidence.
2. Implement guarded original QString decoding and source/disposable fixture manifest equality, including null versus read/decode failure. The fixed native fixture's direct UTF-16 buffer is insufficient.
3. Implement and qualify native original stack unwinding, loaded-module identities and closed-occurrence/main-postOpen association. The same-run CP205 Qt RVA/operands/return-chain/natural-exit predicate remains required; no native fixture can satisfy it.
4. Independently review restart/flags/debug-register ownership and every thread/generation transition for the original. The tested policy rejects unexplained nonzero baseline state; it does not prove arbitrary existing debugger-state preservation.
5. Expand failure testing to partial mutation on a valid handle, genuine continuation/readback/close/termination failure, deadline without a pending event, resource-receipt serialization failure, abrupt debugger death and unopenable/ambiguous process checks. The actual invalid-handle SetThreadContext error is one concrete API failure; other named injections are control-flow exceptions, not invented kernel failures. The outer subprocess timeout path is not a verified cleanup receipt.
6. Have an independent release review bind final source/fixture/raw hashes and original-capable transport behavior before changing either literal flag. The original factory's unconditional HOLD still requires deliberate implementation, not only a flag change. Do not add Frida to fill the unwind gap without separate review.

No app-control session was acquired or claimed released. Original execution and GUI acquisition remain disabled; successful harmless native fixtures do not establish original observation neutrality or authorize a behavior change in the converter.
