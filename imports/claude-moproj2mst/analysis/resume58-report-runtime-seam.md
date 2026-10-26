# Checkpoint 58: dormant DOE Report frame in the runtime owner

## Finding

The smallest owner hook is a retention-only method on `WidgetRuntimeState`, backed by its existing shared `Values` state, with one `ReportQObjectSharedRefLedger` keyed to the stable pending Report descriptor identity. Call it after `TaskFactoryRuntime::initializeNativePrefix()` has retained the descriptor in `TaskAllocationStore` and immediately before the existing unsupported throw. It would retain the phase cursor for the pending frame; it would not invoke or simulate any Qt constructor operation.

The descriptor is already a stable `unique_ptr`-owned `PendingTaskPrototype`. Its address can be converted to an opaque `uintptr_t` ledger token solely for lookup/identity equality. That adapter token must not be treated as a native QObject pointer, native private pointer, or captured object address. No TLS index, TLS value, QThreadData pointer, or refcount value is needed or inferred to create the dormant frame. The ledger's initial `QListFirst` stage is only its next-expected-event cursor; it is not evidence that the first event ran.

## Proposed owner API and behavior

In concept, the hook can be narrowly expressed as `WidgetRuntimeState::retainPendingReportQObjectFrame(std::uintptr_t stableDescriptorToken)`. `Values` owns a map from that opaque token to one ledger. The hook rejects a missing owner state or zero token, retains a single frame, and treats a duplicate request for the same token as retrieval/idempotence rather than replacing the cursor. A diagnostic query may report presence and current stage for mechanics tests.

The factory continues to throw `M2M_E_UNSUPPORTED` at the same Report boundary. The hook must not advance any stage, commit `QObjectConstructionRecord`, touch the process QObject delta, establish a shared-global base, mark `qobjectBaseCompleted`, or publish a Report object. A fresh frame retains no observations: shared refs, backlinks, TLS, thread ref, and process counter remain absent. In particular, `WidgetThreadState` notify depth stays unrelated and untouched.

This is smaller than implementing a new QObject owner or modifying the constructor ledger stages. It creates one shared-runtime lifetime for the dormant frame while leaving the actual native owner unresolved. It does not yet add owner-wide absolute/delta accounting for the three QtCore shared globals; that needs runtime base/delta evidence and should remain a separate later change.

## Exact source/test scope for that structural step

If implemented, the exclusive files should be:

- `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` — declare the retention/query surface on `WidgetRuntimeState`.
- `source/apps/moproj2mst/src/engine/view_manager.cpp` — store and implement the frame map in `WidgetRuntimeState::Values`.
- `source/apps/moproj2mst/src/doe/task_factory.cpp` — pass the retained descriptor's opaque token to the owner just before the unchanged unsupported throw; preserve behavior when the optional WidgetRuntime owner is absent by keeping the guard and not fabricating owner state.
- `source/apps/moproj2mst/tests/mdt/test_report_qobject_ledger.cpp` — test that the owner retains one pristine frame, repeat lookup does not replace/advance it, separate descriptor tokens remain separate, zero token is rejected, and no TLS observation/value is required. Add a narrow factory assertion only if it can verify the retained frame while confirming the existing unsupported failure and `qobjectBaseCompleted == false`.

No changes are needed in `PendingTaskPrototype`, `doe_task_runtime.hpp/.cpp`, the ledger header/source, thread state, or public Report guard for this storage-only hook. In particular, do not add TLS fields with default values to either runtime or prototype structures.

## Is static evidence enough?

Yes for this dormant retention surface only. Source evidence establishes that the prototype allocation is stable and retained before the guard, that `WidgetRuntimeState` is a shared root owner with shared backing `Values`, and that the isolated ledger has an inert initial cursor. Those facts are sufficient to specify and mechanically test ownership, uniqueness, and idempotent retention.

No for accepting observations or executing/reporting any QObject effect. Static evidence does not establish that DOE reaches the original Report constructor in the current runtime, map the adapter descriptor to a native QObject/private allocation, qualify the shared-global bases, or identify the active TLS/QThreadData branch. The retained original disassembly supplies event order and RVAs only. Active same-frame observations are required before moving the ledger cursor, committing the process counter, or setting `qobjectBaseCompleted`. Destructor/member release paths remain independently unqualified, so even successful constructor capture would not authorize modeled teardown.

## Decision

This can be a reviewable, low-risk owner-storage step while Report stays guarded. It should be kept strictly dormant and should not be described as a source-qualified QObject implementation. Public Report support remains unsupported.
