# Minimal ConversionSession-owned Boolean trace bridge

Add two session-owned methods and one private admission scope. Bind against the existing private events_ owner, then validate the opaque receipt immediately before dispatchAt inside the same method. No events accessor, second scheduler, mutable shared timer list or public raw-observation handoff is needed.

This is an interface plan for root review only. Only this report is written. No source, build, original process, GUI, mutable REA or shared-state changes were made. Nested callback and constructor PostPending guards stay unchanged.

## Exact minimal interface

In conversion_session.hpp forward-declare the binder types in m2m::engine. Do not include boolean_trace_due.hpp there: that header already includes conversion_session.hpp for PumpPoint/DueObservation, so forward declarations avoid a cycle.

```cpp
class BooleanTraceOwnerScope;
class BooleanBoundTraceDue;
struct BooleanTraceBindingContext;
struct BooleanPumpTrace;
struct BooleanTraceDueResult;

// New ConversionSession public/internal methods:
BooleanTraceDueResult bindBooleanTrace(
    const BooleanTraceBindingContext& ownerNeutralRequest,
    const BooleanPumpTrace&) const;

DispatchResult dispatchBooleanTraceAt(
    const PumpPoint&, const BooleanBoundTraceDue&, FileEffects&);

// New private member, declared immediately after the existing events_:
Events events_;                              // existing sole timer owner
std::unique_ptr<BooleanTraceOwnerScope> booleanTraceScope_;
Events::ReceiverId receiver_;                // existing
```

The existing ConversionSession destructor is already declared and defined out of line. Its implementation can therefore destroy a unique_ptr to the forward-declared scope after including boolean_trace_due.hpp in conversion_session.cpp. No scope internals, new friend or scheduler ownership exposure is required.

Initialize booleanTraceScope_ with `make_unique<BooleanTraceOwnerScope>(events_)` in the constructor before receiver_ initialization, in member declaration order. The existing events_ is fully constructed first. This is an internal diagnostic admission allocation, not recovered native timer creation. Reset the scope at the start of the adapter Session destructor before project/events teardown. The scope's weak token then expires for escaped receipts while actual events_ still exists; do not recreate it during teardown.

Keep the scope across ordinary null diagram binding, subproject/model rebind and releaseProjectAfterCloseEffects. Original timers/receiver state survive those project transitions, and source183160 still fits before checking the then-current attachment. Scope lifetime is Session/adapter lifetime, not Project or one binding lifetime. Declaration after events_ also makes ordinary member destruction order release the scope before events_.

## Owner-neutral binding method

A caller supplies trace identity/expected pump/thread and native-to-logical registration mapping. It must not select another logical owner or admission scope. Reuse the current request type to keep the change small, but require both owner fields to be null at this Session API boundary. Return a diagnostic binding failure if either is supplied; do not silently overwrite a foreign owner proof.

Pseudocode using the existing isolated binder:

```cpp
BooleanTraceDueResult ConversionSession::bindBooleanTrace(
    const BooleanTraceBindingContext& request, const BooleanPumpTrace& trace) const
{
    if (!booleanTraceScope_)
        return {std::nullopt, "session trace admission scope is unavailable"};
    if (request.logicalOwner || request.ownerScope)
        return {std::nullopt, "session trace request must not choose an owner/scope"};
    auto context = request;
    context.logicalOwner = &events_;
    context.ownerScope = booleanTraceScope_.get();
    return bindBooleanTraceDue(events_, context, trace);
}
```

This method returns the existing opaque BooleanBoundTraceDue in BooleanTraceDueResult. The observation has no raw getter/conversion. It does not deliver a timer or change Project/model state. Logical receiver/timer IDs can be taken from the same Session's pending projection after actual commitNonNullDiagramBinding calls; a fabricated Events copy with matching IDs is never an admission substitute.

The capture/qualification layer still proves module/profile identity, actual native registration/delivery/readiness, thread, complete pump interval and lifetime mapping. This bridge does not authenticate caller-declared trace metadata, source booleans or original process memory.

## Validation and dispatch must be one Session operation

```cpp
auto validation = validateBooleanTraceDueForOwner(
    events_, *booleanTraceScope_, boundReceipt);
if (!validation.observation)
    unsupported(validation.issue.c_str());
return dispatchAt(point, validation.observation, effects);
```

Guard null scope before this sequence. There must be no callback, logging hook that can reenter, event pump, project/owner rebind or timer mutation between validation and the immediate dispatchAt call. Normal local result construction/copy is not a new native callback boundary; allocation failure must not consume a timer.

The isolated validator checks original owner and scope identity/token, live weak token and still-pending mapped registrations. dispatchAt then checks qualification/source site/occurrence before calling deliver. A foreign owner, expired/replaced scope, stale consumed registration or wrong PumpPoint must fail before timer consumption. No separate public `validate then return raw observation` Session method is needed: that would reopen the handoff gap.

Once admission succeeds, preserve current source semantics. DiagramLifecycleEvents::deliver consumes a due timer before fit. An unavailable fit/click effect still throws after that timer has been consumed; do not roll back or retry it. If the first of two flat deliveries hits an unsupported effect, the second remains pending because dispatchAt never reached it.

The existing raw dispatchAt remains unchanged for its declared adapter/testing seam. This new bridge guarantees the same-owner check for calls through dispatchBooleanTraceAt; it cannot stamp or constrain raw DueObservation values constructed elsewhere. Original-trace production callers must use the typed Session bridge. A global restriction on direct raw dispatchAt would be a separate shared API decision, not an implied outcome of this minimal change.

## Candidate-only colliding-ID regression

Add the regression to the existing conversion_session MDT test, which already has an owned subproject helper and candidate-only model setup. Include boolean_trace_due.hpp there. All trace data is explicitly labelled synthetic contract input; no original timing or HWND values are asserted.

1. Construct Sessions A and B, each with a diagnostic effect driver that increments a fit-attempt counter and immediately throws M2M_E_UNSUPPORTED. The driver deliberately does not emulate native fit; it proves whether admission reached that existing guard. Create one test subproject and commit two nonnull bindings in each Session.
2. Record both pending lists and assert the numerical receiver/timer IDs collide. Create an owner-neutral complete flat trace request mapping A's actual pending entries; bind it through A.bindBooleanTrace. Assert the opaque binding exists and neither pending list changed.
3. Submit A's bound receipt to B.dispatchBooleanTraceAt at the matching point. Require unsupported before fit: B fit-attempt count0; A fit-attempt count0; both pending lists identical. This is the exact regression for the old owner-erasure gap.
4. Submit the same receipt to A at a wrong PumpPoint occurrence. Require unsupported before consumption/fit, with both A timers still pending.
5. Submit it to A at the correct point. Require fit guard after one fit attempt; A's first due ID consumed, A's second still pending, B unchanged, no click/Boolean effect. The first due ID is trace order, which may differ from pending-list order.
6. Retry the stale receipt through A's typed bridge. Revalidation must reject before another fit attempt or consumption. It must not replay the consumed timer or advance the second one.

This regression requires no original process, filesystem fixture, GUI or dispatcher creation. It tests owner/lifetime admission plus the existing consume-before-effect boundary, not native timer readiness.

Useful adjacent lifecycle checks after the primary regression:

- Escape an opaque binding from a destroyed Session A, create a fresh Session B with colliding IDs, and require typed rejection/no fit/no consumption. The old weak scope lifetime is expired even if allocator addresses are reused.
- A request that already selects a standalone Events owner/scope fails at bindBooleanTrace rather than being overwritten into Session authority.
- Null-bind/release the project while retaining the Session; the same still-pending timer binding must not be invalidated merely by project storage release. On accepted dispatch, the original fit guard still runs before the null model branch.
- Nested/overlapping synthetic callback intervals continue to refuse in the isolated binder. Do not remove that guard or flatten them in the Session method.

These are proposed tests; this worker added none and executed none.

## Scope and review boundary

Root-owned source write set for this plan: conversion_session.hpp/.cpp plus test_conversion_session.cpp. Existing binder code/types remain unchanged. Existing source targets already compile these files, so no new CMake enrollment is expected; root should confirm the configured target before building. The sole scheduler remains DiagramLifecycleEvents; no public m2m C ABI change is proposed.

The bridge does not close constructor raw4a posting/wake, actual native Boolean callback schedule, source183160 fit/zoom notifications, complete click/UpdateMainView recipients, initial versus replacement editor branches, or normal Close. It preserves current public PostPending and effect-driver guards. Native runtime capture still follows the supported control-release/owned-session requirements in the existing plan.

Reviewable next step: root accepts or adjusts these two method signatures, scope member placement/lifetime and owner-neutral request rule, then assigns/implements the shared source and primary candidate regression. This report is not implementation authorization to another worker's files.


UTC: 2026-10-24T20:28:59.806762+00:00. Source-qualified design only; no tests/builds or runtime evidence added. Token measurements unavailable.

| Reviewed source/report | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `f6d4a46270b7d421e8a78f96db11f990de2e0d61482c01cff2d3662fbe695de4` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `39f1141d2ba59c3d624f7f2be55c50ba0e0e0c15af523b3e9b3430dae15ffbd7` |
| `source/apps/moproj2mst/include/m2m/engine/boolean_trace_due.hpp` | `c8dbb9eb4156fbc0e353422939085d79e4a1b92e57465892bb17239bf1eb2cff` |
| `source/apps/moproj2mst/src/engine/boolean_trace_due.cpp` | `1c90bff07b6ad55acea5e51612b09719166322c7734c8060424d493980593468` |
| `source/apps/moproj2mst/tests/mdt/test_boolean_trace_due.cpp` | `fd28d8d514d8664e88263012486f7598fb9c293c62dcbbbd49336f38a18e3392` |
| `analysis/resume54-boolean-owner-bridge.md` | `eea0a9561d4aae7534894fda4ca6c06665da2eef208418e32facbf006a260981` |
| `analysis/resume54-boolean-dispatch-boundary.md` | `5b311f35618241a98cbc51fc0336306796b3630c9d315ae19d2ae7a154b66823` |
