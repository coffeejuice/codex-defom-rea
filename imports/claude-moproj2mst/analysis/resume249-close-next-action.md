# CP249 RCLOSE / Operation cleanup next-action audit

## Decision

**No new Close/destructor source leaf is admissible from the current evidence.** Keep public normal Close and automatic native OPI expiry guarded. The smallest missing evidence is one same-process original Open → accepted default Close occurrence that carries a single Project/Mo/SubProject/operation owner through nested Save, post-answer callbacks, and the actual destructor prefix. This is the blocker identified by CP196, CP208, and R228, still present in current source.

CP248's editor meshing writeback is a separate finite alias relocation. It does not establish any Project/Operation destruction behavior and is excluded from this lifecycle decision. CP239–247's frozen commands and behavior are not reopened here.

## Reconciliation of the current implementation

`ConversionSession::~ConversionSession()` labels itself adapter exception/final cleanup, invokes `releaseProjectStorage()`, then tears down adapters. `releaseProjectAfterCloseEffects()` requires diagram null binding before calling that same storage release. `releaseProjectStorage()` unbinds the adapter diagram, retains Display, removes `ProjectScope`, resets `Project`, and clears a still-matching Mo backlink. These are explicit adapter lifetime boundaries; they do not replay accepted native Close, native feature callbacks, or the OPI manager destructor.

`Project::~Project()` only invalidates feature-context metadata before ordinary member destruction. `Operation::~Operation()` remains defaulted. `OperationInfoIdentity::~OperationInfoIdentity()` supplies the late adapter fallback `AdapterIdentityLifetime`; it is not the earlier native OPI QObject boundary. No production caller invokes `commitOperationInfoQObjectBoundary()`.

The staged `OperationInfoCleanupProgress` is an isolated source-prefix model, not a Close driver. It can record the actual null Props, null engine cache, empty object/position lists, and null recovery wrappers/flags in source order. It blocks on nonnull payloads before native virtual/destructor effects and stops at `ItemModelCacheCleanupUnavailable`; the current `OperationInfoModelCache` remains `Unknown` absent a bound native defaults event. The code correctly does not infer `+0x160 == null` from empty current vectors or a null modern model pointer. Advancing by resetting a shared pointer, clearing `current`, or treating the adapter destructor as native cleanup would bypass those source boundaries.

## Prior-audit reconciliation

- CP196 and CP208 established that the Report Close fixture only consumes a diagnostic initial slot observation and stops before the second live `S+0x30` read/deleting virtual. Static Close source shows the second current value selects the deleting virtual and the guard is then cleared; it does not bind that value to the Open/Save document or establish its live payload/parent state.
- R228 reconciled the normal-Close gate: can-close and the nested default Save result precede post-answer callbacks and detach. An earlier explicit Save does not substitute for the nested result. A detached cleanup helper or synthetic prefix test cannot establish those effects or ownership.
- CP52/53 Operation cleanup work captured the source-ordered null/empty prefixes while retaining guards around nonnull Props/model/list/recovery effects and the unmodeled `+0x160` owner. It explicitly forbids automatic expiry from `Operation::~Operation`, Project cleanup, engine-model cleanup, or operations storage release.
- The current source retains these limits; none of the reviewed CP239–247 work supplies the missing same-owner accepted-Close occurrence.

## Smallest missing same-process evidence

On one fresh original application state, capture a natural successful Open followed by accepted default Close, retaining stable identities and generations for the main window/manager, Mo child, Project/SubProject, operation row/OPI, and any Report document. Record the nested Save answer and integer return, post-answer callback effects, and any owner/slot rebind or QObject-parent destruction before Close rereads its owners.

For the reached Operation destructor, bind the same OPI identity to the live Props/cache/list/position/recovery-wrapper/recovery-flags and guarded `+0x160` values; record each reached skip or deleting virtual, entry/return, actual owner/member clear, OPI guard invalidation and destroyed-recipient delivery, then outer item destruction. For the Report path if `S+0x30` is nonnull, record both live slot reads, the second target's table/payload, deleting-virtual entry/return, guard clear, and parent/child teardown. Record process-global stores only if reached. This single trace is the minimum evidence that can distinguish actual same-owner cleanup from storage release or a slot rebind.

## Tests and next step

No new synthetic test is justified now: `test_operation_info_cleanup.cpp` exercises the staged adapter decisions, and `test_report_document_slot.cpp` exercises the diagnostic slot boundary. Neither can establish accepted Close, real destructor delivery, QObject parent acceptance, or identity continuity across nested Save. After the same-process trace, derive the smallest source increment from the actually reached branch and add a regression bound to that owner/order; until then keep the existing guards and tests as-is.

This was a read-only source/evidence audit. No source, test, build, shared recovery state, original application, GUI, or mutable REA state was changed or run. The original Close/destructor qualifications cited here remain retained static/decompilation evidence; this audit provides no fresh original runtime observation.
