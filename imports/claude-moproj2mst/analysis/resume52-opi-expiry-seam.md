# OPI expiry seam after merged Operation cleanup

There is no current automatic cleanup point that can honestly call the native OPI QObject-boundary commit. The adapter now has the correct identity/guard leaf, but ordinary merged Operation destruction still has the wrong payload order and no separate OPI-versus-outer-item destruction phase. Keep automatic expiry classified as AdapterIdentityLifetime. The next safe event hook is an explicit completion point in a separate staged OPI cleanup attempt, before the outer row is deleted; reuse the existing commit method instead of adding another notification abstraction.

This is a read-only checkpoint51 source investigation. Only this report was written. No source/build/test/GUI/mutable REA action occurred. Source/native qualifications remain in resume50-close-destructor-next.md and resume52-current-binding-implementation.md. Checkpoint51 validation reported by root is not extended to new native cleanup behavior by this report.

## Current actual endpoints and why they do not establish native expiry

| Endpoint | What runs today | Why it cannot auto-commit native OPI expiry |
| --- | --- | --- |
| Operation destructor body | Defaulted; C++ then destroys all fields in reverse declaration order. | A destructor body executes BEFORE member destructors. Adding commit there would clear current before Props/model/list cleanup. |
| Last Operation member identity destruction | operationInfo_ is declared first, expires after all represented composite fields. | This is already the correct fallback safety endpoint, but it is after the entire folded row, including diagName. Native OPI expiry precedes the outer item's diagName/base destruction. |
| MoModelSlots operations storage release | operations_.reset(), then next slot advances. | The rows are already deleted when this returns, so an Operation reference cannot be used. The underlying OPI native prefix/Qt effects were not executed by this storage routine. |
| Project storage release | Session releases Project and clears backlink; slots survive separately. | Project is deleted before the native operation manager/OPI destruction stage. It is too early to clear all operation current guards here. |
| GuiModel destructor | Auxiliary drawing prefix/display forgetting, then members. | This is the separate OPI+38 target identity, not the OPI itself. Its source explicitly keeps earlier reset/cleanup qualified; engineModel.reset() may only release an adapter lease. |
| props.reset()/operations.clear() in model or tests | Releases represented C++ ownership at its caller. | Null/empty afterward is not proof that native virtual cleanup and callbacks completed, particularly when shared leases survive. |

No production caller currently invokes commitOperationInfoQObjectBoundary. Existing calls are declared test fixtures. The method's nonblank receipt string is a label, not evidence or a completed-prefix capability.

## Required native position

Source-bound native sequence is:

1. SfMoOprItem045cb0 ->045dc0 invokes OPI deleting virtual+18.
2. OPI050440 ->050690: delete Props0512c0; delete guarded engine model05f980; object cleanup061180; position cleanup061f70; delete owned recovery wrappers+20 and+28; delete guarded+160 and clear it.
3. OPI member destructors: guard+160, strings+158/+148/+130/+128/+120/+118, list+60/+50/+40 storage, guard+38.
4. QObject base at050531. clearGuards at6712f8bd invalidates bound current addresses before admitted destroyed delivery at6712f97b.
5. Return through OPI destructor/deleting wrapper and item+48=null, then item diagName+50 and QStandardItem base destruction.

Modern implicit Operation teardown destroys last-declared engineModel first and early Props late, while the identity fallback is last. No event hook alone can make that match. Null model/Props/list branches must be justified from actual state; successful Open generally has Props, so a declared empty test is not enough for production Close acceptance.

The unrepresented guarded OPI+160 is a specific blocker: model.hpp still says it is not modelled. Its absence cannot be inferred from empty current vectors or from an unrelated startup observation. A faithful staged path needs a source-qualified absence state or actual owned model/callback representation.

## Smallest safe staged adapter API

Add an explicit attempt outside ordinary C++ destruction, conceptually:

```cpp
OperationInfoCleanupResult continueOperationInfoCleanup(
    Operation& outerRow, OperationInfoCleanupProgress&, OperationInfoCleanupDriver&);
```

The progress belongs to that actual Operation identity and can only advance through driver-owned effects. It is not a caller-populated `cleanupComplete=true`. It preserves a reached stage if the first unknown payload refuses. Default nonnull/unqualified effects refuse BEFORE mutation. This path does not replace the existing nonthrowing adapter destructor or enable public Close.

Stages follow the native list above. The immediate closed increment is the Props entry boundary0512c0: inspect the real Props owner, take the source null skip if null, otherwise enter an explicitly supplied qualified virtual-destructor effect. Keep the default nonnull path guarded until its actual subtype destructor is recovered. A shared_ptr use_count or a later empty field is not a native destructor receipt; the driver must account for actual pointee deletion and retained adapter aliases.

The actual expiry hook is a leaf inside this continuation after all native OPI payload/member stages have completed, while the outer row allocation remains alive:

```cpp
// Entered only from the progress transition after qualified OPI members finish.
const auto before = actualClass.current.snapshot(); // optional diagnostic view
outerRow.commitOperationInfoQObjectBoundary(prefixReceipt);
// Actual admitted QObject destroyed recipients, when separately recovered.
// They must observe current.get()==nullptr and must not read expired OPI data.
// Later finish OPI delete/item+48 null, outer diagName/base/item storage.
```

Do not add a synthetic destroyed callback merely to demonstrate ordering: actual recipient population and connection delivery are separate source-bound effects. A test observer is explicitly a fixture. Existing guard and CurrentOperationBinding APIs already provide the leaf behavior, so another event-sink helper would add no native recovery by itself.

Once the cleanup prefix is recovered, this continuation can be used from a distinct native/staged operation-manager release entry. Keep MoModelSlots::releaseCompletedSlotStorage as storage-only, and keep its adapter fallback nonthrowing. Ordinary storage cleanup must not start throwing at unknown native payloads or relabel itself accepted Close.

## What may be integrated now and what must wait

Source-ready now: stage the actual Props cleanup entry/conditions and its unresolved dispatch; maintain a distinct OPI cleanup progress object tied to OperationInfoIdentity; preserve original stage order and early refusal. The null Props branch may advance only that stage. Root can test this without publishing any QObjectBoundary expiry.

Not source-ready now: automatically invoke native expiry from Operation::~Operation, Project cleanup, engine-model cleanup, or operations_.reset; infer all callbacks absent; mark vector/shared_ptr release as native destructor execution; erase entire registry selections; or publish a completed native OPI boundary after only the modern merged members are gone.

A post-storage diagnostic hook could report AdapterIdentityLifetime using a captured guard snapshot after row deletion, but it cannot advance native fidelity and is unnecessary unless root needs a specific diagnostic. Do not invest in it as a substitute for the destructor prefix.

## Focused tests for the next source increment

- Actual Operation/OpClass bound through current. At Props stage, a qualified fixture destructor probe observes current still live; the stage does not publish QObjectBoundary.
- Null Props advances only the Props stage. A default unqualified nonnull Props payload refuses before Props reset, model deletion or guard expiry.
- Native prefix attempts keep operation identity/outer row distinct. Unknown engine model, +160, list/member or QObject recipient state refuses at its exact stage; a null modern pointer alone must not invent +160 absence.
- Once all explicitly declared fixture prefix effects complete, a fixture may reach the existing commit leaf: all aliases null before an observer, unrelated/rebound identity live, outer row still allocated. Label this continuation-fixture validation, not original acceptance.
- Ordinary default Operation destruction continues to produce AdapterIdentityLifetime and must not throw. Capsule cleanup remains compatible with that fallback.
- Retry resumes only after a completed sub-effect; it must not replay a partially executed native destructor. Place guards before unresolved effects rather than catching an exception and restarting a whole destructor.

No source edits should occur until root freezes checkpoint51 and assigns the new progress/driver files. The active public Close guard and candidate validation remain unchanged.
