# Startup PostPending dispatcher state and queue order

The retained startup capture resolves the exact queue/dispatcher identities and enqueue order. It does not contain the dispatcher HWND, serial or postedWake values: those reads were skipped by the recognizer. Immutable PE evidence resolves how to read and interpret them, but cannot manufacture historical process memory. The public constructor PostPending guard stays in place.

This work read immutable PE files and existing Evidence only. New bounded disassembly and an exact retained-payload selection are saved below. No original process, GUI control, source change, build or mutable REA session was used.

## Exact retained startup identities

All addresses in this table belong only to owned probe004 PID30208, source observation native TID4820, post context1. That process is already absent according to its independent cleanup receipt. These are historical identities, never addresses to read from a new process.

| Native identity | Value | Binding |
| --- | --- | --- |
| Actual splitter QObject | 0x3da3fc0 | Widget state5 before and10 after posting |
| Splitter private | 0x4f04a50 | receiver+8, both states |
| Receiver/application threadData | 0x3b40c70 | Receiver affinity, thread state3 and app thread agree |
| Queue owner | 0x3b40ca0 | threadData+30; queue states8/9 |
| Queue mutex address | 0x3b40cb8 | source offset threadData+48; computed address, not a separately captured lock object |
| Backing QList data | 0x50b0a30 | Same captured data before/after append; refcount1 |
| Dispatcher QObject | 0x3b43ca0 | threadData+20; state3 dispatcher and state11 agree |
| Dispatcher private | 0x4269b70 | Captured private identity, state11 |
| Dispatcher vtable / wake slot | QtGui+6adb68 / QtGui+5e0ca0 | Captured runtime-relative method identities |
| Incoming heap event | 0x52bad30 | raw74/0x4a, QtGui+69e350 vtable, same pointer throughout post |
| Appended native record | 0x52baa10 | Separate0x18 record allocation, descriptor alias20 |

Queue begin225 and capacity253 remain unchanged; end244->245 and count19->20. The first19 descriptor aliases remain in order and new alias20 is appended. Record/event identity are separate. The last queue snapshot was taken before the posted-bit store, so alias20's embedded flags4 are an earlier description of the same event, not a second event or proof its later posted bit stayed clear.

The queue owner's captured raw fields at +8,+c,+10 were `[0,0,0]`. Since owner=threadData+30, these correspond to threadData+38,+3c,+40. New source selection6711b040 below qualifies their selected roles: nested sendPostedEvents counter, queue read cursor, and insertion bound. Their observed startup values are therefore known zero at the selected snapshots. This does not prove their values at a later event loop or editor constructor.

## Retained chronological receipt

| Ordinal | Stage | Observed result |
| --- | --- | --- |
| 20 | post_enter | Same receiver/thread; incoming flags4, priority0, receiver posted count0. |
| 21 | post_before_insert_locked | Queue state8;19 records, same authoritative locked RDI owner, flags4. |
| 22 | post_after_insert_locked | Queue state9;20 records, same backing/prefix plus appended record, flags still4. |
| 23 | post_before_wake | Receiver count1, incoming flags5, same threadData; queue mutex has already been released by source order. |
| 24 | wake_enter | Dispatcher state11; actual Core wake hook reached in selected post context. |
| 25 | wake_return | Same state11 description; it contains only identity/route data. |
| 26 | post_return | Incoming flags5, receiver count1, same event/receiver/thread. |

The event remained posted through post_return. Wake is not synchronous delivery of this event. Equal stateId11 at wake entry/return only says the captured description is equal; skipped private fields are not compared. It cannot establish no serial change, an already-pending wake or a null HWND.

## Dispatcher private decoder, now source-bound

Immutable QtGui655e0ca0 is `jmp qword [65697618]`, the import thunk to Core QEventDispatcherWin32::wakeUp. Its slot98 receives the same this without adjustment. Core67141b70 loads private from this+8, atomically increments private+a8, then tests private+98 HWND. A nonnull HWND enables CAS private+b4 from0 to1; only successful CAS reaches PostMessageW(HWND,0x401,0,0). Native return is ignored. Null HWND or failed CAS returns after the serial increment.

For this historical private identity, computed field addresses are HWND0x4269c08, serial0x4269c18 and postedWake0x4269c24. They identify the missing reads; none of their contents exists in retained state11. A PE section/RVA translation yields code/static data addresses, not allocator-private contents. Neither the dispatcher vtable nor zero queue control fields supply these values.

The original decoder in probe004 admitted only a direct Core+141b70 slot, rejected the captured QtGui thunk, and explicitly emitted `specificFields: unqualified dispatcher type; not read`. The exact raw selected payload demonstrates this absence. The smallest new passive capture is the corrected recognizer plus three bounded fields at the existing wake entry/return hooks, validating the loaded QtGui IAT target against Core+141b70. No new general memory dump, app input or dispatcher call is needed. A failed type/loaded-target/read check remains unknown and must not fall back to guessed layout values.

## Enqueue, wake and dequeue are separate operations

Core6711ae90 initially chooses receiver-private+40 threadData, locks its mutex, then rechecks affinity. A mismatch unlocks/reloads/relocks; its locked RDI at6711afc6 is the authoritative selected owner. It passes owner+30 and a stack record(receiver0,event8,priority10) to6711acd0. For the observed unique equal-priority branch,6711a200 appends a new record allocation copying those fields, without cloning the heap event. Then source6711afef..6711affc sets event postedbit0, increments actual receiver-private+24 and sets canWait false. It unlocks before reading dispatcher from the same threadData+20 and calling slot98.

New bounded immutable PE capture of `QCoreApplicationPrivate::sendPostedEvents(QObject*,int,QThreadData*)`,6711b040..6711b4fe, qualifies actual delivery order separately:

1. Nonnull receiver affinity must match the supplied threadData; mismatch diagnoses and returns. Entry increments threadData+38 (6711b090), establishing the selected nesting-counter role. Special requested0x46 paths have separate raw43/private behavior and are not generic delivery.
2. With queue locked, the routine initializes canWait from queue emptiness. The main nonempty path uses threadData+3c as the shared read cursor only for unfiltered `(receiver=null,type=0)` calls; filtered calls use a local cursor copy. It snapshots current active length into threadData+40, which bounds this pass.
3. It reads records in native order and advances the selected cursor before examining each event. Null event tombstones are skipped. Receiver/type mismatches set canWait false and continue. Raw0x34 has an additional notification-depth/recorded-loop-level admission/requeue branch; this report does not collapse it into raw4a behavior.
4. For an admitted normal event at6711b286..6711b2a0, clear postedbit0, save actual event/receiver pointers, decrement receiver-private+24, and null record.event **before unlocking**. The record receiver and event's separate allocation identities are retained.
5. After unlock it clears spontaneous bit1 on the live event (6711b313), calls actual application notify67119570 when the application exists (6711b32a), then invokes that event's virtual deleting destructor flag1 (6711b33e). The notify return value is not used by this selected sequence. It reacquires the queue lock before continuing.
6. Loop admission checks both current length and the stored insertion bound. Events posted by notify callbacks are not automatically consumed in this same pass merely because they were appended; nesting/reentrant delivery and priority insertion still require their actual cursor/bound state.
7. Normal loop exit decrements +38. If it reaches0 while canWait is false, a nonnull dispatcher is woken at6711b3f2. For an unfiltered call the code erases the consumed prefix through6711ab90, adjusts insertion-bound +40 and resets read-cursor +3c before final unlock. Filtered calls retain their own semantics. List detach/exception/callback dependencies remain qualified by source, not treated as completed public behavior.

No sendPostedEvents hook or its actual event delivery was recorded in startup004. The enqueue/wake timeline therefore cannot be extended into a historical dequeue ledger. The new PE trace proves possible selected sequencing, not that this startup event was delivered at a particular later point.

| Routine | Queue selection | Matched event lifetime/order |
| --- | --- | --- |
| postEvent6711ae90 | Receiver affinity with lock/recheck | Append record -> set posted/count/canWait -> unlock -> wake. |
| sendPostedEvents6711b040 | Explicit threadData; check receiver affinity | Clear posted/count/tombstone under lock -> unlock -> notify -> event deleting destructor -> relock; later prefix erase. |
| removePostedEvent67119970 | Current threadData | First exact event match; clear count/posted; deleting destructor in its locked path; tombstone; return. |
| removePostedEvents6711b500 | Receiver affinity, or current thread for null receiver | Collect matching events and tombstone/compact under lock -> unlock -> delete collected events in match order; no notify/delivery. |

Checkpoint48's helper implements only the last row's bounded admitted classes/backing. It must not substitute for delivery or imply the public constructor can now complete. Opaque earlier queue receiver identities in the capture also cannot be fabricated into live reconstructed WidgetState components just to satisfy its observation API.

## Decision and next dependency

Resolved from retained evidence: exact startup queue owner/backing, same receiver/event/dispatcher identities, append order, native control-field offsets/observed zero values, and source-qualified enqueue/delivery/cleanup distinctions. Unresolved: three startup dispatcher-private values and selected HWND/CAS branch, actual later dequeue/callback timing, and dynamic callback behavior/Boolean timer delivery.

Root can obtain the missing fields with the bounded no-GUI plan in `analysis/resume50-boolean-passive-hook-plan.md` on a fresh owned process. Historical addresses must be recomputed from that process's actual owners. The app-control release gate remains unchanged; this static worker acquired no control or debugger session. Keep constructor PostPending and public Boolean timing guarded until actual required inputs and continuation tests are integrated.


UTC: 2026-10-24T18:43:38.588325+00:00. Seven retained-payload identity/order/absence assertions passed. No C++ test/build/original runtime was performed. Token measurements unavailable.

| Input/capture | SHA-256 |
| --- | --- |
| `build/original-splitter-probe-004/capture-evidence.json` | `184e45ee34732e4436057df2b99741ce1bed713440f9cc0bfd7e7f9a7ce4df9d` |
| `analysis/wave3-original-splitter-probe-004-receipt.json` | `edadc1a656191b75f9cbc4666ee5439c973f6b3bdbf6b6a46cf5002102e1faed` |
| `build/original-splitter-probe-004/probe.js` | `582b6291dc9abc631048732631bac058f8697d69049bb28a2a4c496e73f394b1` |
| `analysis/wave3-close-probe004-dispatcher-gesture-corrections.json` | `5e5314051d507ec9b68b029f4072ccd8348731b5da6fd58f87eb2d921cc48f2e` |
| `analysis/wave3-closure-next-6711ae90.asm` | `070fb953e47536325d0987838f0502c47c43fef39851bfffaa4d990843974f90` |
| `analysis/wave3-closure-next-67141b70.asm` | `6949b93ac56f20790204f57351050845a6a76c7f9e4ed10866481ff61deaeddb` |
| `analysis/wave3-closure-next-655e0ca0.asm` | `73cd298937578e135a9cbb50466cf41806dac7d0f2c3ba248962ddb263c89955` |
| `analysis/resume51-boolean-dispatcher-static.json` | `4b8c13a91e225f837361655e9515f06f57125db6b2ca53f1a4ec9523819b5332` |
| `analysis/resume51-boolean-dispatcher-6711b040.asm` | `b2d9392bbeef5556b4ec47496a04fb4ac1c80ecfa8a70e31d4ae6a93ea10e265` |
| `analysis/resume51-boolean-dispatcher-selected.json` | `ab1ec0e558d6d0a67096b916ff51df67be1246d70169cde7005a79219da3087a` |
