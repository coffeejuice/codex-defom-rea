# Windows Qt wake and a bounded PostPending prefix

The immutable wake body proves an unconditional32-bit serial increment followed by a HWND/CAS/message branch. It does not establish the startup HWND, serial base, postedWake value, CAS outcome or queued-message delivery. The candidate receiver-removal helper closes a different native routine. Constructor PostPending remains guarded.

This is a static PE/source audit only. No source edit, original process, GUI session, runtime memory read, build or mutable REA session was used. “No-GUI prefix” below means a proposed reconstruction diagnostic continuation requiring no original GUI control; it is not a newly observed native execution.

## Source-closed Windows branch

The qualified route is dispatcher virtual slot+98 -> exact QtGui655e0ca0 FF25 thunk -> Core67141b70, with no this adjustment. A different dynamic slot/loaded import target must remain unresolved. A null dispatcher at postEvent6711b019 bypasses the whole wake call; that differs from a nonnull dispatcher whose HWND is null.

| Site | Native operation | Required distinction |
| --- | --- | --- |
| 67141b74 | Load dispatcher private from this+8. | Actual private owner must be the same as the qualified dispatcher; no independent local copy. |
| 67141b78 | Atomic add1 to private+a8 dword. | Happens before all HWND/CAS tests. Native arithmetic wraps modulo2^32; no signed saturation or invented zero base. |
| 67141b80..67141b88 | Read private+98 qword and return if zero. | With null HWND, postedWake is not read or changed. Serial still advanced. |
| 67141b8a..67141b91 | EDX1/EAX0; atomic compareExchange private+b4 dword0->1. | This is a32-bit CAS, not a boolean read followed by assignment. Nonzero old values are preserved on failure. |
| 67141b99 | Return if CAS did not see0. | A pre-CAS snapshot is not the atomic outcome unless concurrency is qualified. |
| 67141b9b | Reload private+98 HWND. | This is a second load after successful CAS. Preserve the distinction from the earlier nonnull test under unqualified concurrent mutation. |
| 67141ba2..67141bad | PostMessageW(reloadedHWND,0x401,0,0). | This attempted external effect is source-required after success. Return value is ignored; a failed call does not reset postedWake in this body. |
| 67141bb3 | Return. | Returning does not deliver posted events, consume the OS message, reset wake state or prove queue emptiness. |

The wake body has no queue mutex acquisition and no serial rollback. It does not itself clear receiver posted-event counts or event posted flags. Any later consumer/reset of postedWake belongs to separately selected message-processing code. Do not make postedWake permanently true or clear it from queue cleanup without that source/runtime boundary.

Branch summary with symbolic serial S:

- Dispatcher absent: no call, no S increment.
- Admitted dispatcher, testHWND0: S'=S+1 modulo2^32; no CAS or message call.
- Admitted dispatcher, testHWND nonzero, CAS old value P!=0: S'=S+1; CAS leaves P unchanged; no message call.
- Admitted dispatcher, testHWND nonzero, CAS old value0: S'=S+1; postedWake becomes1; reload HWND and attempt message call; ignore its return. The branch is incomplete if the reconstruction omits the attempted message effect.
- Unknown testHWND or CAS outcome: preserve only already-completed effects and retain a pending diagnostic frame at the corresponding stage. Unknown is not the null/no-message branch.

These are conditional source contracts, not assignments of any process values. Immutable PE bytes cannot recover allocator-private startup fields. The earlier startup capture's opaque equal state descriptor does not establish equality of fields it never read.

## Candidate helper audit

The current `WidgetPostEventQueue` is a shared member of WidgetThreadState. Its explicit complete observation binds separately identified heap events, record allocations, same actual receiver components/thread and receiver-owned posted counts. Stack WidgetEventState objects cannot enter it. A default queue stays unknown; no dispatcher presence/window/serial/postedWake state is stored by these APIs.

For its admitted unique-backing receiver/type removal, the helper decrements the receiver count, clears each event postedbit, collects event owners and tombstones records under a represented queue lock. Nonzero cleanupControl38 preserves tombstones/bounds. Zero control compacts by record-pointer swaps and releases the trailing record allocations. It preserves the odd-whole-population native begin adjustment. It then unlocks before deleting collected plain QtGui QEvent raw4a/raw4c objects in match order. Other dynamic destructors/raw43 payloads/shared backing remain guards. That is the bounded6711b500 contract.

None of these operations is a postEvent append, a sendPostedEvents notify/delivery, dispatcher wake or OS message consumption. `postedEventCount`, an event's postedbit and dispatcher postedWake are three distinct native stores. The helper's lockHeld diagnostic models selected queue sequencing; it is not a dispatcher CAS, a host operating-system mutex or evidence of cross-thread exclusion. Its existing tests do not establish the wake branch or current process HWND values.

Its zero-receiver-count fast path deliberately returns after represented lock/unlock without needing compaction state, matching native6711b500's early return. That does not license bypassing the mandatory serial increment in a different routine. The constructor continues to retain its exact stack Create event at PostPending and has no posting/wake continuation. No helper success should move that public guard.

## Smallest defensible reconstruction prefix

This is an implementation boundary proposal, not a change delivered here. It becomes source-ready only after root binds the actual admitted dispatcher/private owner and selected posting state to the same pending constructor/runtime. Do not create a synthetic dispatcher object merely to exercise it.

1. Retain one source occurrence/frame with exact runtime/thread/receiver/heap event/queue/dispatcher/private identities. Guard the unresolved virtual/import destination before claiming Core wake effects. Caller postEvent owns queue mutation and unlock order; entering wake must not replay append or receiver count changes.
2. At Core-wake entry, perform the serial mutation once. If the numeric base is unknown, a symbolic native32 delta may preserve the proven+1 while the numeric value remains unknown. It must belong to the dispatcher-private serial, not notify depth, a callback count or receiver posted count. A downstream numeric consumer remains guarded until the base is stage-qualified.
3. Stop before the HWND decision if its current source-stage value is unknown. The pending frame must retain the completed serial mutation. Resuming the same frame must not increment twice, replace owners, rollback queue changes, destroy stack Create or return success.
4. If a qualified null testHWND is supplied, complete this wake call without reading/assigning postedWake. If the first load is known nonnull, retain a CAS stage. A source-stage-qualified atomic outcome is required; do not use an earlier entry field value as a concurrency shortcut.
5. CAS failure can complete the wake call after the mandatory serial increment, preserving the actual old nonzero32-bit value. CAS success sets postedWake1 and reaches a second HWND-load/message-call boundary. If reloaded HWND or attempted PostMessage effect is unresolved, preserve CAS1 and the pending call; do not clear it to permit a later retry or claim return because the native return value is ignored.
6. Only after the admitted wake returns may postEvent return, then constructor private+c8 store and the retained stack Create destructor run. Later QFrame/QSplitter/style and actual Boolean editor timer delivery remain independent dependencies.

Fresh observations must identify whether each value is entry/pre-increment, after-increment, pre-CAS, atomic-outcome or post-CAS/reloaded-HWND state, including source occurrence and actual owner identity. An after-prefix value cannot silently replace an unknown pre-prefix base. Retain observed concurrent changes explicitly; a local staged model is not a generic concurrent Qt dispatcher implementation.

This prefix could permit useful source-order diagnostics before an unknown HWND without requiring original GUI control. It cannot currently be wired through the candidate constructor from a queue snapshot alone, because posting and dispatcher owner state are not implemented or bound. Keep the public guard.

## Focused validation boundaries for a future increment

These proposed tests are not executed or counted as passing in this audit:

| Input/continuation | Observable contract |
| --- | --- |
| Unknown serial base, unknown HWND | One retained+1 symbolic mutation, numeric serial unknown, same pending frame/owners; repeated resume leaves+1. |
| Known serial0xffffffff, qualified null testHWND | Serial wraps to0; postedWake stays unread/unchanged; no message effect; wake returns. |
| Nonnull testHWND, CAS observes0xffffffff | Serial advances once; full32-bit postedWake0xffffffff preserved; no message effect. |
| CAS succeeds from0, message effect unresolved | postedWake1 and serial mutation retained; second HWND stage/message attempt remains pending; no retry that repeats CAS. |
| Successful CAS, qualified attempted PostMessage returnsfalse | The call was attempted with source arguments; native result ignored; wake returns; postedWake stays1 in this body. |
| Same pending frame with substituted dispatcher/private owner | Guard, preserving earlier mutations and stack Create lifetime. |
| Receiver cleanup after an independently completed wake | Clear event/count/record state only; do not change serial or postedWake without separate message-consumer evidence. |

An original numeric claim still requires root's bounded passive same-process reads under the no-GUI ownership/control plan. This audit invokes none of them and does not relax the supported app-control release/status gate.


UTC: 2026-10-24T18:55:02.331349+00:00. Three static checks passed: mandatory increment/CAS/call order, two separate HWND loads, public guard retained. No build or runtime validation; token measurements unavailable. Original PE code claims use the immutable bounded source captures below.

| Input | SHA-256 |
| --- | --- |
| `analysis/wave3-closure-next-67141b70.asm` | `6949b93ac56f20790204f57351050845a6a76c7f9e4ed10866481ff61deaeddb` |
| `analysis/wave3-closure-next-655e0ca0.asm` | `73cd298937578e135a9cbb50466cf41806dac7d0f2c3ba248962ddb263c89955` |
| `analysis/wave3-closure-next-6711ae90.asm` | `070fb953e47536325d0987838f0502c47c43fef39851bfffaa4d990843974f90` |
| `analysis/resume47-boolean-cleanup-contract.md` | `b7cb3bde92b875779e51bf97eb96bdfb2ca3b8f31567545641b2e9a9136e6dec` |
| `analysis/resume48-boolean-queue-helper.json` | `880219eaee1aa6d31fcc00217b2f90f3d71a358fe1a7bda76f116d74143dcad5` |
| `analysis/resume51-boolean-dispatcher-state.md` | `d32385f367861569551d7146f52dc773d8ed7a89813c100449eeabf6cd476a2e` |
| `source/apps/moproj2mst/include/m2m/engine/widget_events.hpp` | `e75a9f7631dab2202a894310e94adfbda0906232074a6b57c8ab1531cd55b4ec` |
| `source/apps/moproj2mst/src/engine/widget_events.cpp` | `1ddfbe54765865f795262f93279a9ae2db36d6b640eedcce54b7c0c6f2ca14aa` |
| `source/apps/moproj2mst/tests/mdt/test_widget_events.cpp` | `9a815b7ff87bd0fd1862cebdc7047f5e899326738f4a1c917a09d2138b0ecc1b` |
