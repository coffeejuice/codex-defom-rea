# CP163 independent review: QObject constructor prefix proposal

## Decision

Accept the proposed prototype-one prefix as a safe next source increment, with the normal non-null allocation path as its sole admission. Keep the stop immediately before QtCore RVA `0x131185` (preferred VA `0x67131185`). This is the earliest useful reachable constructor state after the currently retained 0xc0 Report allocation: it records the actual QObject address point and separately owned 0x88 private allocation/base address point while refusing before the first shared QList load/refcount effect. Do not advance to TaskInfo, map registration, TLS/thread state or the process counter in this increment.

The evidence supports the sequencing claim at the cited ranges: GUI Report allocation/constructor/registration (`0x140137610`), TaskBase (`0x141403210`), Qt public QObject (`0x671313d0`) and private initialization (`0x67131160`). The relevant capture receipts identify the original GUI and QtCore hashes and explicitly qualify DUMPBIN ranges as unwind regions rather than complete logical functions. Treat the proposal's byte-level cursor as a bounded observed prefix, not proof of all constructor behavior or failure cleanup.

## Source ownership and guard

The present source confirms the reachable location: `TaskFactoryRuntime::createTaskBase` enters `initializeNativePrefix` before key lookup; that method commits Factory/Data, asks the shared `TaskAllocationStore` to retain one `PendingTaskPrototype`, then refuses before QObject. `PendingTaskPrototype` currently has only kind, 0xc0 size, and `qobjectBaseCompleted`; it has no actual QObject/private construction frame. `report_qobject_ledger` and widget counter helpers are observation/state seams and cannot substitute for the missing owner or native prefix.

Implement the partial state as an optional frame owned by the same retained prototype node, with stable object identity across refusal/reentry. Preserve the native 0xc0 and 0x88 size descriptors, but do not model native memory with host pointers or zero-fill unknown fields. Store module-qualified address-point identities and a phase/cursor. At the cut, expose no public/private backlink and keep `qobjectBaseCompleted`, prototype/native completion, and both maps false/empty. A repeated `createTaskBase` must encounter the same pending Report and frame and refuse at the same cursor without reallocating or rewriting earlier effects. Keep the requested-key lookup behind native initialization as it is now.

Admission must be explicit: only a normal successful non-null private allocation may enter this prefix. Null allocation, allocation exceptions, partial unwind/destructor behavior, and any retry semantics not observed in the original are unsupported and must continue to refuse without claiming transactional parity. If a host allocation fails while creating the adapter-owned frame, do not publish a half-advanced cursor or fake the native successful path. Adapter-scope release may free the retained frame only under the documented fresh-process adapter boundary; do not call that recovered QObject destruction.

## Focused test contract

Add focused tests when implementing (not part of this review) around the real `TaskFactoryRuntime` and shared `TaskAllocationStore`:

- Successful-prefix state is exactly ordered and represented: one pending Report identity, QObject base address point, one private-state identity/0x88 descriptor, initial private base address point, cursor before RVA `0x131185`.
- Repeated entry/refusal preserves both identities and phase; retained prototype count remains one and no second task/private allocation or address-point reset occurs.
- Public `task+8` and private `private+8` links remain unavailable; no SessionTask/TaskInfo/PIMPL object is created; completion flags remain false; metadata/task maps remain empty; key lookup does not fabricate a requested result.
- No shared-container, thread-reference, QObject-counter, font, TLS or TaskInfo effects occur before the cut.
- Separate negative controls verify unsupported null/failure paths do not report prefix success or native cleanup parity, and adapter-scope release invalidates the frame without claiming native destruction.

These tests should assert observable owner state and ordering, not merely a diagnostic ledger. Existing unrelated import comparisons need not be rerun solely for this constructor-prefix unit, though normal checkpoint validation policy still applies when source work is actually integrated.

## Risks and limits

The main implementation risk is representing native construction with a host-language object whose default initialization accidentally invents native field values. Keep the frame an explicit sparse phase record, with only observed writes represented. Another risk is advancing the phase before allocation/state is durably attached to the retained owner; build the next phase transactionally at the adapter-model level, while making no claim this reproduces native allocation-failure unwinding. Reentry must be idempotent at the adapter frame, not a second execution of earlier native stores.

The cut does not yield a usable Task or an application-visible feature. It is a truthful partial constructor checkpoint. Further advancement requires root-owned shared Qt empty-container identity and evidence for the next ordered increments; reaching TaskInfo additionally requires the same-thread TLS/QThreadData and counter ordering, followed by actual TaskInfo/PIMPL and call-time font evidence. Separate font observations or preseeded metadata cannot bridge this gap.

No source, test, build, GUI, live REA, or external state was changed by this review. Relevant evidence: `analysis/resume163-connector-prototype-next.md`, `analysis/resume163-connector-prototype-static.json`, `analysis/resume163-connector-qobject-static.json`, and the current `doe_task_runtime.hpp`, `task_runtime.cpp`, and `task_factory.cpp` source snapshots.
