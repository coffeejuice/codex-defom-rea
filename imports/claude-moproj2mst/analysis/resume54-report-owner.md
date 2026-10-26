# Checkpoint 54: DOE Report QObject owner dependency

## Result

The Checkpoint 53 ledger is a useful inert event-order validator, but it is not yet an owner for native QObject effects. Its lifetime is the ledger instance, its `allocationIdentity_` is caller-supplied, and its recorded final process-counter event is not connected to `WidgetRuntimeState::commitQObjectConstruction`. That separation is correct while Report construction remains guarded.

The smallest safe integration step is structural and still dormant: retain one ledger/frame with the already-retained `PendingTaskPrototype` and put the three QtCore shared-global base/delta records under the existing root `WidgetRuntimeState` shared owner. Add no constructor effect, destructor effect, or `qobjectBaseCompleted` transition. Keep `TaskFactoryRuntime::initializeNativePrefix` throwing before the Report QObject call. This step can ensure state has one owner and survives factory adapter operations; it cannot certify a native constructor.

## Existing ownership seams

- `TaskAllocationStore::allocatePendingReportPrototype()` creates a `unique_ptr<PendingTaskPrototype>`, retains it in `prototypes_`, and returns a stable descriptor reference. That descriptor has only `Kind::Report`, `nativeAllocationBytes=0xc0`, and `qobjectBaseCompleted=false`; it is not native storage and contains no QObject identity, private pointer, TLS pointer, or refcount observation.
- `TaskFactoryRuntime` holds a weak allocation-store link and optional shared `WidgetRuntimeState` link. `initializeNativePrefix()` retains/records the pending descriptor index, then throws at the first Report QObject call. The factory is an adapter/control owner and is not a suitable home for process-wide shared QtCore counters.
- `WidgetRuntimeState` is already shared by `shared_ptr` and owns optional QObject count base plus accumulated delta. Its `Values` subobject is also shared-owned and is the natural private backing store for additional qualified shared-global state. `WidgetThreadState` is separately shared but currently models notify depth; that value is not a QThreadData address or reference owner.
- `QObjectConstructionRecord` is bound to an actual object identity and is one-time. `commitQObjectConstruction` increments only the process QObject counter delta, modulo 32 bits. It deliberately does not establish QObject allocation, parent/private backlinks, thread binding, or destruction.

## Ledger audit against evidence

The ledger records the static ordered sequence: four shared references (`QList`, `QString`, `QVector`, `QList`), reciprocal object/private backlink values, existing-index/non-null TLS fast path, QThreadData helper increment, then process QObject increment. The retained QtCore image identity and RVAs in the ledger match the corrected Checkpoint 53 contract. Its stage cursor rejects repeats and out-of-order calls; accepted increments are modular uint32 `+1` observations.

The current validation is intentionally weaker than a runtime-owner contract in several places:

1. `constructorIdentity` is just a repeated nonzero integer equality token. It does not prove that the token names the same captured Report descriptor or native object. In particular, backlink observations are not cryptographically/module qualified and the ledger does not bind their `objectIdentity` to an independently observed native allocation.
2. The ledger records shared-ref before/after pairs and self-attributed deltas, but it does not retain an absolute base, establish that the three addresses are mapped in the active module instance, or reconcile them with an already-qualified root owner. The non-adjacent values are valid as per-event records, not evidence of a whole-process count.
3. The TLS record accepts only the initialized-index/non-null profile. No captured active DOE call binds the index, TlsGetValue result, `currentThreadData` return, QObject private `+0x40`, helper RCX, and refcount address to one actual frame.
4. The process-counter observation is accepted by the ledger but is not committed through `QObjectConstructionRecord`. Conversely, calling the existing commit helper without completing the preceding constructor stages would violate the observed order.
5. The available destructor excerpt proves the process-counter `XADD`/zero-cleanup sequence only. It does not prove matching decrements for the three shared empty-container references or the QThreadData ref, nor safe zero-count teardown. No native lifetime can be completed from this ledger.

These limits agree with `analysis/resume53-doe-report-sharedref.md` and `analysis/resume48-doe-report-refcleanup.md`: no active DOE Report capture exists; the pending descriptor is adapter metadata; and container/QThreadData release behavior remains unqualified. The counter destructor evidence must not be generalized to the other refs.

## Minimal future integration boundary

For a purely structural follow-up, keep a per-prototype constructor frame adjacent to the same `PendingTaskPrototype` allocation (or in a store keyed by that stable descriptor) and make `WidgetRuntimeState` the single shared owner for qualified QtCore global identities and any observed absolute bases/deltas. Do not create a second DOE-local/global owner. Treat the descriptor address, if used at all, only as an adapter token; never equate it with native QObject bytes or a native object address.

The API boundary should accept immutable explicit observations and reject unless all owner links agree: allocation-store descriptor identity, constructor-frame token, actual QObject identity, private identity/backlink pair, and root runtime owner. The existing ledger can remain the ordered cursor, but its current observation types need a separately captured native-object association before they can support that check. No observation should invoke Qt or write target/native memory. The process counter helper can be called only after all earlier stages have been captured and accepted on that exact frame; `qobjectBaseCompleted` must remain false until the full source/capture-supported boundary is specified.

## Runtime inputs still missing

An active, same-call Report capture is still needed to provide:

- the loaded QtCore module identity/base and the actual addresses/values of QListData `+0x2f0a50`, QString `+0x2f0b30`, QVectorData `+0x2f0bb8`, and QObject counter `+0x2f4344`, including their before/after DWORDs;
- a stable mapping from retained pending Report descriptor to the actual native QObject address and allocated private block, plus the observed reciprocal links `object+8` ↔ `private+8`;
- the TLS index before/after initializer, TlsGetValue index/result, and evidence that the existing-index/non-null path was actually taken;
- the one selected QThreadData identity carried without substitution through currentThreadData return, private `+0x40`, helper RCX, and the exact incremented refcount DWORD with its before/after value;
- proof that these observations belong to the same constructor invocation and same `WidgetRuntimeState` root as the process QObject counter observation.

Even that constructor capture would not qualify object cleanup. Before owning teardown or claiming completed object lifetime, separately recover the actual Report/common QObject destructor and member cleanup path for all three shared refs and QThreadData, including zero-count consequences and allocation-failure/unwind behavior. Existing static evidence establishes none of those release paths.

## Decision

Keep the public Report guard unchanged. The ledger does not close the owner dependency or justify a native-effect integration. The next safe action is an active same-frame passive capture that fills the missing identities; until then, source code may only retain dormant, explicitly unobserved owner state. Do not claim public Report support or set `qobjectBaseCompleted`.
