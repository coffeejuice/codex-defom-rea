# Checkpoint 46: DOE Report QObject static evidence

## Scope and conclusion

This note is a static evidence trace for the first native `Report` construction only. The source still correctly stops at the unsupported guard before `TaskInfo`; this evidence does not authorize removing it. The native public `QObject(parent=null)` constructor has a clear ordered prefix, but current reconstructed state cannot represent the shared-container increments or the selected live `QThreadData` reference. No new executable effect is closed by this review.

## Established original-derived prefix

The Report path's public base call is identified in `analysis/wave2-doe-task-public-qobject-ownership-contract.json`: caller VA `0x141403222` (RVA `0x01403222`) calls the `QObject(parent)` import at IAT `0x141e041c0`, resolving to `QtCore4_64.dll` preferred VA `0x671313d0` (RVA `0x001313d0`). This is the public constructor; `0x6712e630` is a different protected constructor used by other widget/grid paths and must not be substituted.

At `QtCore+0x001313d0`, public QObject construction writes vtable `0x6723cac8`, allocates `0x88` bytes of private state, and calls the private initializer at `QtCore+0x00131160` with version `0x40804`. It then commits `object+8 = private` and `private+8 = object`, preserving the same QObject identity. The Report descriptor already allocated by the DOE factory is `0xc0`; it must remain the stable `PendingTaskPrototype` allocation in the existing `TaskAllocationStore`, rather than being replaced by a parsed/session projection.

The private initializer's exact shared-reference actions are visible in `analysis/wave2-doe-task-qobject-private-initializer.asm`:

- `QtCore+0x00131185` stores shared QListData address `QtCore+0x002f0a50` into private `+0x18`; `+0x00131190` increments its DWORD refcount.
- `+0x001311a2` stores shared QString address `QtCore+0x002f0b30` into private `+0x30`; `+0x001311ad` increments it.
- `+0x001311c7` stores shared QVectorData address `QtCore+0x002f0bb8` into private `+0x68`; `+0x001311d2` increments it.
- `+0x001311da` stores the same QListData address into private `+0x70`; `+0x001311de` increments it a second time. The two QList increments are distinct effects and total `+2`; QString and QVector each receive `+1`.
- Fields `+0x40/+0x48/+0x50/+0x58`, `+0x78/+0x80`, `+0x08/+0x10/+0x38/+0x28`, and dwords `+0x24/+0x60/+0x64` are explicitly initialized as shown in the assembly. Dword `+0x20` is read, masked by `0xfffffcf0`, ORed with `0x60`, and written at `+0x00131224..+0x0013122f`; the known resulting mask/value are `0x36f/0x60`. Other bits are inherited, not proven zero.

The public parentless constructor then calls `currentThreadData` at `QtCore+0x0012aca0`. That routine calls TLS initialization `+0x00129600`, loads TLS index global `QtCore+0x002f08e8`, and invokes `TlsGetValue` at `+0x0012acbc`. On the qualified existing-index/non-null fast path, the returned actual pointer is selected. Public QObject stores it at private `+0x40`, calls ref helper `QtCore+0x00027630` at `+0x00131453` (helper performs `LOCK ADD DWORD [RCX],1`), then independently increments process QObject counter `QtCore+0x002f4344` at `+0x00131488`. These effects are ordered after private shared refs and backlinks. Parent-null skips parent-child work.

The capture contract explicitly excludes inference across TLS miss branches: missing TLS index or null TLS value can allocate/adopt data and are outside the fast-path qualification. The current `WidgetThreadState` only models notify depth; it does not bind TLS index/value, `QThreadData` identity, or its reference count. The current `WidgetRuntimeState` counter seam covers the process QObject counter only; it does not own the shared QListData/QString/QVectorData counts. A local copied count would violate the shared-owner model. `analysis/wave3-editor-shared-qobject-counter-contract.json` further establishes the process counter as process-global and separate from QWidget counters and thread notify depth; its observed editor counts do not establish a DOE Report constructor execution.

After the QObject base returns, TaskBase overwrites the vtable and starts `TaskInfo` initialization. Current font data must be sampled at its native TaskInfo call site; probe004's startup font observation is not proof of later invariance. DOE/report metadata, list defaults, translation/preferences, cache/factory completion, and all TaskInfo effects remain beyond the qualified prefix. Existing SessionTask list defaults are only a bounded DOE payload projection, not verified generic Report base defaults.

## What remains unknown for active DOE

No existing original-derived runtime trace reaches this first Report call. Probe004 (`analysis/wave3-original-editor-probe-004-receipt.json`) reached `selected_editor_first_grid_published` in a fresh owned process, with matching executable and QtCore hashes, and was cleaned up; its bounded capture ended before proving DOE task-factory reachability. It provides module identity and an example of passive capture/cleanup ordering, not constructor values or execution evidence.

For an active DOE constructor, the following are not yet known from runtime observation: whether the actual factory path reaches this call in the selected fresh case; actual Report object/private addresses and register/backlink values; loaded module bases for that process; actual shared-global refcount bases and before/after values attributable to this call; whether TLS index is initialized and its value; `TlsGetValue` result and selected `QThreadData` identity; selected thread-ref DWORD base/value and delta; exact same-thread/call-ordinal identity chain to the process-counter increment; and whether nested constructors make attribution ambiguous. The currently observed editor counter values cannot fill these gaps. Also unknown are any native effects after this public constructor, including current-font/TaskInfo writes and later Report metadata; they are not part of this capture's proposed stop boundary.

## One serialized passive capture design

Use one fresh original process and one bounded, read-only Frida observation run. Before launch, the operator verifies executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; records actual module paths, bases, hashes, PID, TID, cwd, argv, environment, locale/code page, startup settings, and a clean disposable DOE case. Resolve all addresses as module base plus RVA; do not reuse probe004's base `0x53b00000` except as an example.

In the single capture, instrument these ordered points and retain only bounded registers/memory reads, timestamps, thread IDs, call ordinal, caller/return addresses, and module-relative instruction identity:

1. `DEF_GUI_MO_64.exe+0x01403222` before the public-base call: record RCX candidate object, RDX parent (require null), R8/R9, RSP/return address, bounded caller bytes, and confirm the call belongs to the actual DOE Report factory path.
2. `QtCore4_64.dll+0x001313d0` entry and return: bind the same RCX object identity, parent, object `+0/+8`, returned private pointer, and reciprocal `private+8` backlink.
3. `QtCore4_64.dll+0x00131160` entry/return and instruction offsets `+0x1190`, `+0x11ad`, `+0x11d2`, `+0x11de`, `+0x1232`: record private pointer/version, each shared global DWORD immediately before/after each increment, and the raw private `+0x20` dword before/after mask update. Distinguish the two QList increments; record actual shared-global identities and bases, with no assumption that an unobserved initial value is zero.
4. `QtCore4_64.dll+0x0012aca0`, TLS initializer `+0x00129600`, and the `TlsGetValue` call at `+0x0012acbc`: record TLS index global `+0x2f08e8`, index argument, actual return pointer, and whether the run took initialized-index/non-null fast path. If it takes a missing-index or null-result path, preserve that trace but classify it unqualified and stop before claiming the normal thread-ref/counter prefix.
5. At `QtCore+0x00131453`, helper `+0x00027630`, and continuation `+0x00131488`: bind private `+0x40` to the same observed TLS-returned pointer; record helper RCX, `DWORD[RCX]` before/after, then process counter global `+0x2f4344` before/after its distinct increment. Require same thread, constructor ordinal, object/private identity chain, and unambiguous attribution.

No target-memory writes, function calls, Qt API calls, GUI input, event pumping, Save, simulation, or extra dialog interaction are in scope. Stop recording immediately after the selected constructor returns or the first guarded/unqualified prefix is reached. Retain script hash, raw ordered events, capture status, truncation/read errors, and the exact stop reason. Then quiesce observation, independently verify the exact owned PID is absent before closing the REA/Frida session, and retain cleanup evidence. One capture serializes shared-count and thread-ref effects under the same observed process/thread and constructor identity; separate runs would not prove a single ordered chain.

## Decision

Keep the existing `M2M_E_UNSUPPORTED` guard before the first Report QObject call and before TaskInfo. Implementation requires a root-owned shared empty-container refcount API, actual TLS/current-QThreadData observation bound to the existing `WidgetThreadState`, and a resumable constructor frame on the same stable pending allocation. Only after those are qualified can the ordered private refs, thread reference, and process QObject counter be represented without guessing.
