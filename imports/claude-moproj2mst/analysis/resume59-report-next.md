# Checkpoint 59: Report frame evidence and next bounded step

## Audit result

No retained capture establishes a pristine native DOE Report constructor frame, binds that native object to the adapter's pending `PendingTaskPrototype`, or records an ordered Save effect on the same object. The available evidence is split between immutable PE/source qualification and a live startup observation that ends earlier. Therefore no new Report runtime effect is ready for integration. Keep the public Report guard and the dormant ledger unchanged.

## What the retained artifacts establish

- `analysis/wave2-doe-task-public-qobject-ownership-contract.json`, the QtCore assembly views, and `analysis/wave3-doe-report-qobject-prerequisite-contract.json` statically identify the Report factory call and native order: caller RVA `0x01403222`/QObject call `0x01403229`, public QtCore QObject `+0x1313d0`, shared-ref events and backlinks, current-thread/TLS selection, QThreadData ref, then process QObject counter at `+0x2f4344`.
- `analysis/wave3-doe-report-qobject-capture-plan.json` is explicitly a **static plan only; no process launched or attached**. It requires the capture to observe prospective native QObject identity in RCX and parent in RDX at the DOE factory call, track the private allocation and refcount deltas, then preserve the TLS-to-QThreadData identity chain. It explicitly says the process must reach the selected real DOE factory path and that probe004 does not establish reachability.
- `analysis/wave3-original-editor-probe-004-receipt.json` is the available live receipt. It identifies the executable and loaded QtCore hashes/base, but its endpoint is `selected_editor_first_grid_published`; it reports 73 records and no callback/read errors. The receipt's qualifications say it stopped before later setup and before full editor or Save/Close acceptance. It contains no Report constructor frame fields, pending descriptor association, or Save event sequence. Its directory snapshot shows no change/add/remove, which is not a Save trace.
- The current source-side pending prototype is an adapter descriptor retained by `TaskAllocationStore::prototypes_`; its size is `0xc0`, but it is not the native object's bytes. The in-memory pending descriptor identity cannot be equated with the constructor's runtime RCX from static source alone.
- Report `.DS` document loading is another Report-related path. `resume51-report-ds-load.md` and `resume52-report-ds-owner.md` explicitly distinguish its parented `0x70` SP+`0x30` document object from the parentless `0xc0` TaskFactory Report prototype. Its path/read ordering does not qualify the task factory QObject or a Save effect for the task prototype.

The same capture plan explicitly says **do not attempt Save** during its bounded QObject observation. That is the right bound for the first capture. There is consequently no evidence that connects a completed Report constructor, its native identity, and a later ordered Save on one instance. Static Save dispatch notes and files on disk cannot supply that same-instance runtime connection.

## Bounded recommendation

1. Keep the implementation dormant. The Checkpoint 58 retention-only owner hook remains the maximum source step justified by static stability/lifetime facts; its frame token must remain an adapter identity only. Do not accept staged observations, advance constructor stages, call the QObject counter helper, set `qobjectBaseCompleted`, or relax the first Report guard.
2. When root authorizes and serializes the original capture, first execute the existing bounded passive constructor plan in a fresh, identified process. Record the real DOE caller path, native RCX/pending-allocation mapping if one exists, object/private backlinks, shared ref pairs, actual TLS branch and exact QThreadData chain through constructor return. Preserve ambiguity and stop on a missing-index/null-value branch.
3. Treat ordered Save evidence as a separate follow-on capture, not an extension of the constructor probe. Only after a native Report instance is validly reached should a separately scoped original lifecycle observe the exact Save entry/return points, writer order, per-write outcome and resulting file effects, all keyed to that same native object/session. That run needs its own explicit capture plan and fresh-process/environment receipt; the constructor plan forbids Save and cannot be repurposed as Save authorization.
4. Resume implementation only against observations that bind the same root runtime owner and same native object across constructor and Save. Constructor evidence alone still will not qualify destructor/member cleanup or make ordinary Close safe.

## Exact unresolved evidence

- No actual runtime mapping from `PendingTaskPrototype` identity to native QObject RCX/object address or private allocation.
- No runtime before/after values for the three shared globals in one Report frame, nor actual TLS index/result/branch classification or QThreadData pointer/refcount pair.
- No successful constructor-return receipt identifying the object as the first Report reached through the real native DOE/task-factory path.
- No ordered Save trace on that same object, and no file-write attribution tied to its lifecycle.
- No matching shared-container/QThreadData release path evidence for supported teardown.

## Decision

Static evidence is enough to preserve the expected event order and justify dormant frame storage. It is not enough to claim a pristine runtime frame, map that frame to native memory, or infer any ordered Save effect. This is a source/evidence boundary, not a coding gap that can be filled by an additional synthetic test. No source, build, GUI, or mutable REA action was taken for this audit.
