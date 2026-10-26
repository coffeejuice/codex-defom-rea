# Frozen Session-owned Boolean trace admission bridge

Implemented the reviewed minimal Session bridge. Each ConversionSession retains one private BooleanTraceOwnerScope bound to its existing sole events_ owner. Owner-neutral requests map through that scope, and typed dispatch revalidates owner/lifetime/pending state immediately before calling existing dispatchAt. Public PostPending and the isolated binder's nested-callback guard remain intact.

Only conversion_session.hpp/.cpp, test_conversion_session.cpp and this report were written. No binder/CMake/GUI/mutable REA changes or build were performed. Source is frozen for root validation.

## Source changes

The header forward-declares binder types instead of including boolean_trace_due.hpp, avoiding the existing include dependency cycle. It adds bindBooleanTrace and dispatchBooleanTraceAt and declares a unique_ptr admission scope immediately after events_. The implementation includes the complete binder type, initializes the scope against the already-constructed events_ before receiver_ initialization, and invalidates it at the start of the Session adapter destructor before project/event teardown. There is no new scheduler, events accessor or public raw observation-returning Session method.

bindBooleanTrace rejects a missing Session scope and any request that selects an owner/scope. It copies the owner-neutral request, fills the actual private events_/scope, and calls the frozen binder. It does not mutate the caller's request, consume timers or infer native readiness.

dispatchBooleanTraceAt validates the opaque receipt against that exact live events_/scope and pending registration state, then calls dispatchAt directly, with no callback/pump/rebind between those operations. Foreign owner, expired scope and stale registration fail before consumption; existing dispatchAt retains qualification/source occurrence checks and consume-before-fit behavior.

The scope remains Session-owned across null diagram binding and Project release, matching surviving receiver/timer state. Its lifetime is not one Project or one model attachment. Session adapter destruction expires escaped bindings; a new Session with colliding numerical timer IDs cannot adopt them.

Raw dispatchAt's normalized text is identical to the verified checkpoint56 source archive. It remains available for its existing declared adapter/testing seam. The typed bridge is the same-owner admission path for bound OriginalTrace receipts; no claim is made that arbitrary shared raw DueObservation construction is globally prohibited.

## Release-active candidate regression coverage

Four new testkit cases (36 total in test_conversion_session.cpp) use clearly labelled synthetic trace contracts and guarded diagnostic effect drivers. CHECK/REQUIRE in the current testkit evaluate in Release and do not depend on assert/NDEBUG. The driver counts reaching fit and then throws unsupported; it does not emulate native Qt fit, invoke Boolean effects or assert original readiness.

1. **Colliding foreign IDs, wrong pump, consume-before-effect and stale replay.** Two Sessions have equal receiver/timer numbers. A-bound receipt submitted to B refuses with neither pending list/callback changed. Wrong source occurrence on A also consumes nothing. Correct typed dispatch on A reaches the unavailable fit guard after consuming only the first ID in observed trace order (deliberately reverse pending order), leaving the other pending. Replaying the stale receipt refuses before another fit attempt/consumption; B stays unchanged.
2. **Owner-selected, incomplete and nested requests.** Caller-selected foreign owner or scope refuses instead of being overwritten. Incomplete traces and overlapping/nested callback frames refuse without fit/click or timer mutation.
3. **Scope survives null binding/Project release.** An already-bound receipt remains admissible after real Session null-binding/storage-release steps. Fit is still reached before the source null-model skip; the unavailable fit guard consumes only the selected timer and performs no click.
4. **Escaped receipt after Session destruction.** A receipt from a destroyed Session cannot be rebound to a fresh Session with colliding IDs; typed admission refuses before fit or pending mutation.

These are candidate integration tests, not new original captures or acceptance comparisons. No files, original processes, native dispatcher/HWND values or timer-ready inference are needed by the new fixtures.

## Root validation and remaining dependencies

Worker static preflight passed; compiler/linker and all C++ test results are pending root. Existing conversion_session/binder targets already contain the touched sources, so no CMake edit was made. Root should run the coordinated focused conversion_session/boolean_trace_due/lifecycle tests and required full regression.

This closes the scoped Session-owned typed admission path. It does not enable public constructor PostPending, native raw4a posting/wake, original timer readiness, nested callback replay or complete fit/click/UpdateMainView/Close effects. Those guards and original evidence dependencies remain as before. No public m2m C ABI change or native-value assignment is introduced.

UTC: 2026-10-24T20:31:35.081534+00:00. No worker build/test execution. Token measurements unavailable.

| Frozen file | SHA-256 | Bytes |
| --- | --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `2f932e7386e98c8a2343f53c5c7343f2423fb6cd3b8911832c56ebe6d4c2eead` | 10227 |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `3b26e49c7a431d035f546c0ce7d76d22f4ea56ad75649059962d54cfaccb8278` | 21617 |
| `source/apps/moproj2mst/tests/mdt/test_conversion_session.cpp` | `fc62ac9656329f8190a1f6fb601f550b0dd2b31fe688db19e0e7547b11677e18` | 54849 |

Static checks (not executed C++ tests):

- raw_dispatchAt_unchanged_from_verified56: passed.
- scope_declared_after_events: passed.
- scope_expires_before_teardown: passed.
- owner_neutral_request_guard: passed.
- same_owner_validation_immediately_before_dispatch: passed.
- four_new_release_active_tests: passed.
- no_new_assert_macro_tests: passed.
- single_test_main: passed.
- public_postpending_guard_retained: passed.

Reviewed plan: `analysis/resume57-boolean-session-bridge.md`, SHA-256 `144405a3b73d7df971e2d0a3dbd7f6de3f5b9a866fba724da61ca45ad70909dd`. Original source qualifications remain attached to its referenced contracts.
