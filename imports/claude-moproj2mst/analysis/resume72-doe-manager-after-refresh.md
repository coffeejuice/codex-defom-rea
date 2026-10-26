# Checkpoint 72: manager selection after refresh

There is no source-closed current Session path from the shared Type2 Save arm to the isolated PAR sequence without resolving its refresh continuation. An actual Session/owned Study can execute the isolated helper directly as a bounded internal stage, but that does not establish traversal through native Save. Native dispatch does have returns without Qt invocation; none is currently proven for the fresh Type2 Session profile.

The earliest missing integration decision is the actual shared notifier's suppression/population branch for `2signalDoeTableRefresh()`. On its unsuppressed live-table route, the next unavailable receiver effect remains the actual table child's QTimer stop. The fact that sample ownership is already representable does not advance either boundary.

## Two different manager selections

Retained original CP63/64 evidence establishes the saved manager:

- `0x14009969a -> 0x1400b5a90` obtains the active Type2 SUB's manager (`SUB+0x20`). `0x14009969f` caches it in `[rsp+0x90]`; qualified null skips the study arm.
- That cached pointer is RCX for the InputTable wrapper at `0x140099770`, selected-mode helper at `0x1400997b2`, and fixed modes0/1/2 at `0x140099a77/99d7c/9a081`. The caller does not resolve a replacement manager after notification.
- Only after the InputTable wrapper and its refresh return normally does `0x1400d75a0` read the selected mode (`+0x40`) and invalid byte (`+0x70+mode`), then select retained table storage (`+0x58+8*mode`). The four sequence calls continue after ordinary false. CP71 documents the precise reads, rereads and predicates.

Refresh independently resolves the table's UI context. The actual slot at `0x1400f4e10` first stops QTimer `table+0xb0`, then calls refresh virtual `+0x1b8 -> 0x1400f47e0`. That refresh stores `table+0x98=1`, performs the first Delete widget setEnabled, and only then resolves global main-window -> current Project -> active SUB -> that SUB's Study manager. The resulting table aliases `+0x40/+0x48/+0x50` may reflect a changed global selection. They are not the caller's cached manager and do not replace it for the later four writes.

Candidate ModelManager already represents native main-window `+0x50` and manager `+0x10` owner link. Its currentModel checks liveness/list membership, and Mo DbHandle.moProject is the Project backlink. This can serve the later UI lookup at its proper source phase, but cannot be invoked early to bypass timer/flag/widget effects. A selected Mo with no reconstructed Project is a lazy-reconstruction guard, not permission to substitute `session.project()` or the XML Study.

## Exact no-Qt possibilities and why they are not admitted here

The retained complete `0x1402cf3c0` dispatcher and CP64 asm prefix establish this order:

1. `0x1402cf3f1` reads the actual notifier byte `+0x141`; `0x1402cf3fa` branches to return when nonzero. This suppression return precedes flag reset, key lookup, snapshot, and all receivers.
2. Otherwise `0x1402cf408` clears flag `+0x90`, constructs the exact key, calls map lookup/default insertion at `0x1402cf454`, and copies the ordered registration list at `0x1402cf461`.
3. It visits the snapshot, removes expired guards from that local snapshot, and uses QMetaObject::invokeMethod with DirectConnection1 for a live guard. The invoke Boolean is ignored. An empty snapshot, all-dead snapshot, or a non-invoked method can consequently return without actual slot effects, but each requires its actual population/guard/method facts. No backend-null short circuit is established at this global dispatcher; that is a separate receiver-specific concept elsewhere.

These are source branches, not permission to choose a convenient profile:

| Proposed shortcut | Missing fact in current fresh Session path |
| --- | --- |
| Set notifier suppressed=true | No production source transition has been bound that leaves actual global suppression set at this Save phase. Fresh SessionNotificationState defaults false; recovery does not introduce a new suppression scope to avoid delivery. |
| Treat absent typed key as empty | Exact `DoeTableRefresh` list population is unavailable, rather than qualified empty. Native table constructor registers this key at `0x1400ebc5c`. |
| Treat table cache KnownNull as no receiver | Constructor registration precedes constructor return/cache publication. Unpublished, interrupted, or deleted cache state is not itself the notifier's complete live-guard history. |
| Treat an opaque receipt as a dead/live QObject | Admission identities are diagnostic leases, not native QObject weak guards, QTimer ownership or Qt metadata. |
| Catch requireRefreshDelivery and proceed | The guard is an unrecovered boundary, not a native ordinary false return. Catching it loses required behavior and falsely reaches PAR. |
| Resolve current manager and use it | Native saves through the earlier cached manager even when refresh observes another selection. |

The candidate's `observeEmptySessionNotification` explicitly permits only its six recovered empty keys (`session_notifications.cpp:50–55`); DoeTableRefresh is excluded. SessionNotificationState has task/graph/view projections but no source-qualified typed refresh registration list. Manually mutating `emptyKeys` or default-constructing state cannot close that absence.

## Current production guard and Session representation

`src/moproj/write.cpp:138–159` performs the owned Type2 node prefix and, for empty variables, actual immediate XML. It records the normal-return refresh boundary, obtains the actual Session DoeTableOwnerState and calls requireRefreshDelivery at line157. There is no call to saveType2SampleSequenceImmediate in this arm. The following unconditional prefix guard also remains. An XML normal false still reaches the same boundary.

`ConversionSession` creates its actual notification owner at `conversion_session.cpp:148` and a canonical DoeTableOwnerState at line168. The holder captures weak actual Session/ModelManager/WidgetRuntime/notifier identities. This closes ownership of an admission skeleton, not the native main-window QWidget/table/timer lifecycle.

`src/doe/table_owner_state.cpp:232–236` is explicitly noreturn. It validates the Session admission scope, refuses when main-window `+0x458` is unavailable, then refuses timer/UI delivery regardless of diagnostic field receipts. Its report leaves timer state, receiver liveness and widget state unavailable. Releasing admission scope during Session cleanup is not native receiver death.

This is an intentionally conservative public guard. It currently stops even a hypothetical suppressed profile; an explicitly qualified future dispatcher prefix could return before consulting table ownership when actual suppression is true. That prospective branch does not make the present fresh false-suppression profile executable.

## What can execute internally now

The existing immediate helper can receive the Study owned by an actual Session Project/SUB and that Project's actual StudyIo. Its source-closed responsibility is the admitted sample predicates, file selection/order and the existing bounded immediate text-I/O contract. It neither needs nor delivers Qt to perform that isolated computation. Tests of this stage may explicitly invoke it after constructing those actual owners and use diagnostic I/O faults; they must call it an isolated stage, not a completed native refresh continuation or a public Save path.

An optional ownership-checking internal wrapper could validate the real Session/Project/SUB/cached Study/StudyIo before delegating. That would improve misuse detection without making refresh return. It should not accept a `refreshSucceeded` Boolean, generic callback, or an enum manufactured by a test as evidence that the missing source stage ran. There is no need to implement this wrapper merely to claim a new reachable public branch.

For eventual real continuation, retain the original cached Study weak identity, not a strong diagnostic lease across notification. After a qualified normal dispatch return, first confirm Session's live Project and original SUB membership without dereferencing a stale SUB. Then validate owned Study pointer and control block, raw aliases/backlink, actual unchanged Project StudyIo and Session ModelManager owner. Loss/replacement refuses at the reached boundary while retaining XML/refresh prefix effects. Such refusal is a reconstruction guard; native safe behavior after arbitrary destruction is not claimed. Do not reselect the current active SUB for the saved manager. Read its current sample mode at the reached PAR stage.

## Smallest next dependency and ownership proposal

The smallest missing production interface is a same-owner refresh-dispatch prefix that consumes the actual Session notifier's suppression value and distinguishes unavailable ordered population from a qualified empty/list snapshot. It must preserve native flag90 reset only on the unsuppressed path, exact key/default insertion and local dead-guard handling. A typed registration/liveness projection must be bound to actual constructor/death events, not populated from a post-hoc list or owner labels. Root must own `doe_session_notifications.hpp/.cpp`, shared Session bindings and `write.cpp` to implement such a seam; this lane edits none of them.

That prefix can only admit a no-Qt return when its source facts support one. Fresh false suppression proceeds to actual ordered population. The known table registration route then requires real SfMoDoeTable/QTimer/UI construction/liveness before the first QTimer.stop can advance. Source qualification of the field lifecycle is retained, but the opaque holder has not implemented constructor initial UI work, publication, Qt child ownership, metadata dispatch, timer stop or widget semantics.

No new original capture is needed merely to prove cached-manager selection or the suppression branch; existing evidence already closes those facts. The next evidence/implementation work should target binding actual refresh population/liveness or a real source suppression transition at this phase. Do not spend another checkpoint adding duplicate PAR serialization or a second manager snapshot in lieu of that dependency.

The immediate helper's existing pre-serialization/whole-buffer ofstream limitations, separate textWrites lifetime and absence of a global XML/PAR event order remain as qualified in CP71. They are additional fidelity/output-surface work after ownership reachability, not the earliest reason public PAR is currently unreached. Type3/OPT and public B121/B127 guards remain separate.

## Evidence and inspection receipt

Original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained source evidence: CP63/64 public call windows/current PAR body, CP65 slot/update/registration captures, CP67 main-window lifecycle/current-Project lookup, complete retained `wave2-doe-session-notifier-retained-1402cf3c0-d1.c`, and CP64 dispatch asm. No new native capture was performed.

| Inspected file | SHA-256 |
| --- | --- |
| analysis/resume64-boolean-event-route-1402cf3c0.asm | 9AB15E4A88C891082CBFC05D07D01893D082AA204330970C893E783773DC3B64 |
| source/apps/moproj2mst/src/moproj/write.cpp | 6CEE2B06CE406E18C3A74643FE387B14C0434E22025DEBC5D734F55505D259D1 |
| source/apps/moproj2mst/src/doe/table_owner_state.cpp | D57B8A6BA6F6DAC0D7D9EFB515B73A6DAFBC10BA4213E5681C37744FAF5FCFCF |
| source/apps/moproj2mst/include/m2m/doe_session_notifications.hpp | BE68879BF7756A67B229662D5D2397F16D89668C3ACCF277C7A97B442C5AD625 |
| source/apps/moproj2mst/src/engine/conversion_session.cpp | 99731A89662BAC4B9162C22F0BBB649DA3E65C64A5431C96048060774B4AB8A3 |

Only this report was written. No source, shared state, build, tests, GUI, application control, or mutable REA operation occurred. This report proposes dependency closure, not a native/public success claim.
