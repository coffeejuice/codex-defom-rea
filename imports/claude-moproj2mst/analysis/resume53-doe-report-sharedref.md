# DOE Report QObject shared-reference prerequisite and smallest isolated seam

## Audit result

The retained PE disassembly supports a precise public parentless `QObject` constructor prefix, but no active DOE Report execution is captured. It does not authorize public Report support. The existing `M2M_E_UNSUPPORTED` guard in `TaskFactoryRuntime::initializeNativePrefix` is still the correct integration boundary: it allocates/retains the pending `0xc0` Report descriptor, then refuses before constructing QObject or entering TaskInfo.

This audit uses retained original QtCore assembly and existing source/contracts only. It changes no source, build, GUI state, or mutable REA state.

## Corrected addresses and native order

All module locations below are QtCore RVAs relative to the actually loaded `QtCore4_64.dll` base. The retained preferred VAs have image base `0x67000000`.

- DOE Report path: caller `DEF_GUI_MO_64.exe+0x01403222` sets `EDX=0`; the public QObject import call is at caller RVA `+0x01403229`, target IAT `0x141e041c0`, resolving to QtCore `+0x001313d0`. This is the public `(QObject*)` constructor. The protected constructor at QtCore `+0x0012e630` is a different path and must not be substituted.
- Public constructor sets vtable, allocates `0x88` private bytes, then calls private initializer QtCore `+0x00131160` with version `0x40804`.
- Private-initializer ordered shared reference actions:
  1. QtCore `+0x00131190`: `LOCK ADD DWORD[+0x002f0a50],1` (QListData); private `+0x18` points at this shared address.
  2. `+0x001311ad`: `LOCK ADD DWORD[+0x002f0b30],1` (QString); private `+0x30` points there.
  3. `+0x001311d2`: `LOCK ADD DWORD[+0x002f0bb8],1` (QVectorData); private `+0x68` points there.
  4. `+0x001311de`: second `LOCK ADD DWORD[+0x002f0a50],1` (QListData); private `+0x70` points to the same shared address. Net constructor deltas: QList `+2`, QString `+1`, QVector `+1`, all uint32 wrapping additions. Preserve the two QList events separately in an ordered frame.
- The initializer also writes a qualified subset of private fields. Dword private `+0x20` is transformed as `(old & 0xfffffcf0) | 0x60`; known result bits are mask `0x36f`/value `0x60`; untouched bits remain unknown. The initializer returns before the public constructor writes backlinks.
- Backlinks are written by the public constructor after initializer return: `object+8 = private` at QtCore `+0x00131425`, then `private+8 = object` at `+0x00131431`. This is a correction to earlier shorthand saying the private initializer itself commits backlinks.
- Parent-null test branches to the current-thread call at QtCore `+0x00131449`. The call is **at `+0x00131449`**, not `+0x00131440` (the latter address appeared as an incorrect call-site description in the earlier resume47 note). Its returned RAX is stored at `private+0x40` by the confirmed instruction `+0x0013144e` (`49 89 44 24 40`).
- Thread-reference helper call is at `+0x00131453`; helper is QtCore `+0x00027630`, which performs `LOCK ADD DWORD[RCX],1`. RCX must equal the returned/stored QThreadData pointer. Parent-null then skips parent validation/child callbacks and reaches the separate process QObject increment `LOCK ADD DWORD[QtCore+0x002f4344],1` at `+0x00131488`. The process-global counter commit is therefore after the QThreadData reference.

TLS fast-path details: `currentThreadData` entry is QtCore `+0x0012aca0`; it calls TLS initializer `+0x00129600` at current-thread `+0x0012acb1`, loads the TLS index DWORD at `+0x002f08e8` at `+0x0012acb6`, and calls the resolved `TlsGetValue` target through IAT slot `+0x001a80a8` at `+0x0012acbc`. If the index is not `0xffffffff` the initializer returns without the allocation/one-time path. A nonnull `TlsGetValue` result branches directly to current-thread return at `+0x0012ae42`. The `0xffffffff` index path may call `TlsAlloc` through IAT `+0x001a8378` and stores its result to `+0x002f08e8` at `+0x0012968c`; a null TLS value enters a separate allocation/adoption path. Neither branch is qualified for DOE Report. In particular, initialized index alone does not prove nonnull TLS value.

## Existing source seam and guard

`PendingTaskPrototype` in `doe_task_runtime.hpp` is a stable `unique_ptr`-owned descriptor with `Kind::Report`, native size `0xc0`, and `qobjectBaseCompleted=false`. `TaskFactoryRuntime::initializeNativePrefix` retains this allocation in the `TaskAllocationStore`, then currently throws `M2M_E_UNSUPPORTED` at the first Report QObject call. Do not replace this node with parsed `SessionTask` or add defaults to the descriptor.

The engine already has an `QObjectConstructionRecord` bound to an actual native identity and `WidgetRuntimeState::commitQObjectConstruction`. That helper is one-time and increments the process-global QObject delta using modulo-32-bit state; test coverage exercises that counter seam. It does not encode a public QObject private allocation, shared-container identities, actual TLS/QThreadData, thread-ref ownership, or their constructor order. `WidgetThreadState` has notify-depth state only. `WidgetRuntimeState` has an optional QObject count/base and counter delta. These existing types therefore cannot be reused as if the earlier stages had already happened.

## Smallest isolated code seam that can be unit-tested

The smallest useful implementation slice is an **inert, root-owned staged ledger**, not a factory-local count or executable Report constructor. Place ownership with `WidgetRuntimeState`; attach the per-construction cursor/frame to the same stable `PendingTaskPrototype` in place. It should accept only explicit observations and never fabricate addresses, count bases, or TLS state.

Minimal model:

- Shared-reference owner records three fixed semantic identities `(QtCore module SHA-256, symbol RVA)`: `(+0x2f0a50, QListData)`, `(+0x2f0b30, QString)`, `(+0x2f0bb8, QVectorData)`. Each stores optional qualified absolute base (absent unless observed), retained uint32 delta, and same-owner provenance. Do not use module-relative identity without the qualified QtCore hash; do not create a second owner in DOE factory.
- One pending-Report constructor frame stores the stable allocation identity token, exact next phase/event, and already-applied event bitset/cursor. Constructor sequence is private refs in the four-event order, private/backlink completion, qualified existing-index/non-null TLS identity, thread-ref event, then process QObject counter `QObjectConstructionRecord` commit. Reject skip, duplicate, wrong-identity, or out-of-order effects. Re-entering a frame must not replay committed events.
- `WidgetThreadState` gains a qualified optional TLS index and actual selected `QThreadData` identity/ref owner only when explicit source/capture evidence binds them; notify depth is not a substitute. Before an absolute thread-ref base is observed, retain an exact uint32 delta with unknown base rather than report zero.
- The existing process counter helper remains last in the ordering. Do not call it until private/shared and thread-reference stages have been accepted on this same frame. `PendingTaskPrototype::qobjectBaseCompleted` must advance only after the source/capture-supported completion boundary, not just because the ledger has constructor deltas.

This seam can be tested in isolation without a native process for mechanics only: correct ordered deltas (`2,1,1` with two distinct QList events), uint32 wrap, absent-base preservation, module-hash/RVA identity checks, no duplicate application across resume, rejection of missing-index/null-TLS profiles, exact pointer equality through TLS return → private `+0x40` → helper RCX, and counter commit strictly last. Such unit tests establish the ledger/state machine's behavior, not that these observations occurred in the original application or that the DOE path reaches it. Until a capture supplies those inputs and the root shared-owner/thread seam exists, the ledger remains dormant behind the current guard.

## Unresolved owner/evidence facts

1. No active DOE Report capture reaches caller `DEF_GUI_MO_64.exe+0x01403222`/the QtCore public constructor. Probe004 stops at first editor grid publication; its TLS/counter samples cannot be assigned to Report.
2. Shared QtCore global addresses are statically established, but no same-call runtime record supplies their absolute starting values, observed deltas, module base, or state owner binding. Static add instructions justify symbolic deltas only, not execution of a reconstructed process-global counter.
3. No runtime profile qualifies existing TLS-index/non-null `TlsGetValue`, the selected actual QThreadData pointer, or its refcount DWORD/base for the DOE constructor. TLS index miss and null-value allocation/adoption are explicitly unqualified.
4. No proven link maps the native `0xc0` pending Report descriptor identity to a writable native QObject layout/private allocation. `PendingTaskPrototype` is an adapter owner record, not native bytes.
5. Matching release/decrement paths for the three shared-container references and QThreadData ref have not been qualified. The retained QObject destructor does show the distinct process-counter `XADD`/zero-cleanup sequence, but that is insufficient to implement Report cleanup or global shared-ref teardown.
6. QObject allocation failure, other constructor version/parent branches, unwinding, destructor and zero-count cleanup are outside this bounded prefix. TaskInfo font/preferences/cache and remaining prototype construction begin after the public base and remain guarded.

## Decision

Keep `M2M_E_UNSUPPORTED` before the Report QObject call. A dormant shared-owner ledger is the smallest mechanically testable seam, but executing it or setting `qobjectBaseCompleted` requires the missing runtime identity/delta observations and owner integration. Do not claim public Report support from static disassembly or isolated ledger tests.

