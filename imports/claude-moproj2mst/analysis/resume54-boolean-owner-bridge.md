# Boolean trace owner bridge: isolated source freeze

Closed the isolated binder owner-loss seam by replacing its exposed raw DueObservation result with an opaque owner-bound receipt. A separate validation step requires the same live owner/admission scope and unchanged pending registration mapping before returning a raw observation. Nested/overlapping native callback frames now fail closed because a flat due list cannot represent their effect timing.

Source is frozen for root build/review. No shared ConversionSession, DueObservation, dispatchAt, DiagramLifecycleEvents, public PostPending, CMake, GUI or mutable REA state was edited. Worker did not compile or run C++ tests.

## Implemented API

- `BooleanTraceOwnerScope` borrows one existing DiagramLifecycleEvents object and holds a distinct lifetime token. It is noncopyable, nonmovable and cannot be rebound. It creates/owns no timer or scheduler. The actual events object must outlive its admission scope.
- `BooleanTraceBindingContext` now requires both the same logical owner pointer and an ownerScope for that owner. Mapping against a different owner/scope refuses before returning a binding.
- `bindBooleanTraceDue` returns `optional<BooleanBoundTraceDue>`, not `optional<DueObservation>`. The bound receipt privately retains its observation, original owner/scope, a weak scope-lifetime token and selected pending registration snapshot. There is no raw observation accessor, implicit conversion or public constructor bypass.
- `validateBooleanTraceDueForOwner(owner, scope, bound)` returns a raw observation only when owner identity, original scope identity/token and live token agree, and all mapped registrations still match pending ID/receiver/interval state. It neither consumes timers nor invokes callbacks. Wrong owner with colliding IDs, a replacement scope, expired scope or consumed registration returns no observation plus a diagnostic issue.
- A bound receipt does not keep its admission scope alive. Reusing the same native/C++ address in a newly constructed scope cannot match its old token. A dangling reference to an actual events object is still invalid caller behavior; this non-owning seam does not claim to detect arbitrary violations of the required events-outlives-scope C++ lifetime.

The validation output is deliberately the existing raw DueObservation type. It must be used immediately at the same-owner dispatch boundary without intervening owner/registration replacement. Shared dispatchAt is unchanged and does not itself acquire a new owner stamp. Thus the future root-owned Session bridge must perform this validation against its actual private events_ immediately before dispatch; arbitrary callers can still construct/copy the shared raw type outside this seam. This increment does not claim to have changed that shared/public API.

## Native callback representation guard

The prior isolated binder accepted properly nested native callback frames and flattened them into entry order. That acceptance is superseded for this bounded profile. A later timerEvent entry at or before the preceding selected callback's return now refuses with `Nested/overlapping native callbacks have no flat dispatch representation`. Crossing and nested frames are both rejected; disjoint ordered callbacks remain admitted. No reentrant schedule is silently linearized. The source183160 fit-before-current-model decision and actual nested-pump effects remain outside this flat profile.

The existing nested-acceptance assertion was changed to expect refusal. This is an explicit unsupported representation boundary, not an assertion that the original cannot reenter or that no callbacks were observed.

## Tests and root integration

Existing10 binder contract tests now go through mapping then same-owner validation before inspecting a raw observation. Four new focused tests cover:

1. A bound receipt mapped to one event owner rejects another owner with colliding pending IDs, including mismatched owner/scope pairings, while preserving both pending lists.
2. A fresh/rebound admission scope cannot validate an old receipt even on the same events object. A fresh mapping validates only under its new scope; null scope mapping rejects.
3. An escaped receipt cannot retain a destroyed temporary admission scope and rejects under a replacement live scope.
4. Admission rechecks pending registrations: a timer consumed after mapping invalidates the stale receipt without consuming the remaining timer.

Compile-time assertions require the bound receipt not to convert to DueObservation and the scope not to copy/move. Total test cases14. These are explicitly synthetic candidate contract fixtures, not original timer readiness or callback-effect acceptance.

No CMake change is needed from this worker: root already enrolled the existing boolean_trace_due source/MDT target at checkpoint53. Root should compile/run that target and the coordinated regression suite. Declaration/definition/include and whitespace preflight passed; actual compiler/test results remain pending. No build run or efficiency/token-savings claim.

## Limits and next dependency

This closes the isolated typed mapping-to-admission boundary and guards unsupported nesting. It preserves the sole actual timer owner and all source-qualified missing-versus-explicit-empty, registration/lifetime/profile/thread checks. It does not supply original dispatcher private values/readiness, connect the binder to private ConversionSession storage, execute fit/click notifications or remove public PostPending guards. Root retains the shared bridge and original comparison dependencies recorded in resume54-boolean-dispatch-boundary.md.

UTC: 2026-10-24T19:44:20.600217+00:00. Static checks below are not executed C++ tests. Token measurements unavailable.

| Frozen file | SHA-256 | Bytes |
| --- | --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/boolean_trace_due.hpp` | `c8dbb9eb4156fbc0e353422939085d79e4a1b92e57465892bb17239bf1eb2cff` | 6478 |
| `source/apps/moproj2mst/src/engine/boolean_trace_due.cpp` | `1c90bff07b6ad55acea5e51612b09719166322c7734c8060424d493980593468` | 8427 |
| `source/apps/moproj2mst/tests/mdt/test_boolean_trace_due.cpp` | `fd28d8d514d8664e88263012486f7598fb9c293c62dcbbbd49336f38a18e3392` | 13468 |

Static preflight:

- opaque_map_result: passed.
- scope_has_weak_receipt_lifetime: passed.
- scope_noncopyable_nonmovable: passed.
- same_owner_scope_validation: passed.
- rechecks_pending_registration: passed.
- nested_callbacks_guarded: passed.
- no_timer_mutation: passed.
- fourteen_tests: passed.
- single_main: passed.

Governing boundary report: `analysis/resume54-boolean-dispatch-boundary.md`, SHA-256 `5b311f35618241a98cbc51fc0336306796b3630c9d315ae19d2ae7a154b66823`. Its native source qualifications remain attached.
