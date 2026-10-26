# Frozen Boolean trace-to-DueObservation binder

Implemented an isolated evidence-mapping seam. It borrows the existing DiagramLifecycleEvents owner, validates a complete qualified native pump trace, and returns DueObservation with OriginalTrace origin and delivered IDs in actual observed order. It does not infer readiness, consume timers, invoke callbacks, create another scheduler, perform OS effects or change dispatchAt/public constructor guards.

Source is frozen for root review/build. Worker did not compile or execute tests. Only the three new files listed below and this report were written; no shared CMake, existing engine source, GUI or mutable REA state was edited.

## Contract

`bindBooleanTraceDue(owner, context, trace)` returns either a qualified optional observation or a diagnostic issue. Unavailable/incomplete evidence returns no observation. A complete explicitly observed empty interval returns an explicit empty due list qualified only to its exact profile/source occurrence.

The context pins the current borrowed event owner, expected EXE/QtCore/QtGui/script/fixture SHA-256 identities, action profile, native session, pump source occurrence and native thread. Hash syntax and identity equality are checked; this pure binder does not open/authenticate Evidence files or prove that caller-declared completeness flags are truthful. The capture/qualification layer remains responsible for producing those facts from retained original evidence.

Each registration maps one native receiver/timer address plus lifetime generations and registration ordinal/positive native timer ID to existing pending logical receiver/timer IDs with requested interval1000. Registration threads must match the pump. Duplicate logical timers, native timer lifetimes or registration ordinals are refused; a receiver lifetime cannot alias different logical receivers or vice versa. The actual borrowed owner pointer is checked even when numerical IDs collide across event owners. Caller must keep that same live C++ owner/mapping in scope; this is not a persistent ownership store or a detector of invalid/dangling caller references.

Each delivery requires actual timerEvent and modelUpdated callback evidence, matched callback receiver, balanced frame/thread identity, strict ordered entry ordinals inside the selected pump boundaries and exact native registration mapping. Duplicate delivery, stale lifetime/ID, unregistered callback, source/thread mismatch, crossing callback frames, errors or truncation refuse. Proper reentrant nesting is allowed and consumption order follows timerEvent entry order. Native timer IDs are the registration IDs retained by timerEvent entry, not late reads after the original kill/-1 store.

Only the selected modelUpdated-observed branch is admitted. Native disconnected/no-callback, missing-dispatcher registration0 and unmodelled callback/pump branches stay diagnostic guards. Existing logical receiver liveness and fit/click effects still belong to DiagramLifecycleEvents::deliver and the session effect driver; this binder validates pending registration identity rather than pretending to prove full callback effect equivalence.

The binder does not consume a trace or timer. Repeated binding of the same immutable trace is pure; dispatchAt/deliver owns actual logical consumption. Once mapped IDs have been consumed, a stale registration mapping no longer matches pending state. Context registration lists cover the selected currently pending mapping, not an unbounded history of all past native registrations. Diagnostic input limits are1024 registrations/4096 deliveries, explicitly adapter bounds rather than original Qt population limits.

## Validation and integration

Ten focused test cases were added using clearly labelled synthetic contract fixtures, not original timing observations. They cover observed ordering/purity; incomplete versus explicit-empty interval; source coverage errors/truncation; identity/source occurrence mismatch; foreign owner with colliding IDs; mapping ambiguity/no native registration; timer-address reuse generations; duplicate/crossing/unbalanced deliveries; mismatched thread/ID/generation; and bounded-input rejection. The positive test separately consumes returned IDs only through the existing events owner after confirming binding left both pending timers unchanged.

Root must add `src/engine/boolean_trace_due.cpp` to the appropriate converter source target and register `tests/mdt/test_boolean_trace_due.cpp` with the usual MDT test dependencies; no CMake edit was made by this worker. Suggested unique target/test name: m2m_mdt_boolean_trace_due / m2m.mdt.boolean_trace_due. Header depends on existing conversion_session.hpp for PumpPoint/DueObservation and lifecycle owner types. C++20 is required for value equality, matching the project standard.

Manual declaration/definition/include review and the static checks below passed. Compiler/linker/test outcomes are pending root. Allocation failure follows normal C++ container behavior; no original Qt allocation-failure semantics or runtime parity is claimed by this diagnostic binder.

Neither binder success nor isolated tests closes constructor PostPending, Windows wake, actual Boolean editor creation timing, native registration/delivery readiness or complete fit/click/Close acceptance. Those still require source-qualified original inputs and integration comparisons.

UTC: 2026-10-24T19:29:41.283841+00:00. Model token measurements unavailable; no efficiency claim.

| Frozen file | SHA-256 | Bytes |
| --- | --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/boolean_trace_due.hpp` | `4fa1cf07a80452114806494f00a8048da5527ba8acf530d4a48670975255a4b6` | 3169 |
| `source/apps/moproj2mst/src/engine/boolean_trace_due.cpp` | `abb52b8c042117c6ba252f1b99d815b84bed37a8af6f5f3a7266bd8c8456ab8d` | 7246 |
| `source/apps/moproj2mst/tests/mdt/test_boolean_trace_due.cpp` | `462cc262d52fb018650d3fa94bf37a5eea6f1f134dc5a37353a58fa3d1857257` | 9880 |

Static checks (not executed C++ tests):

- no_timer_mutation_or_delivery_calls: passed.
- no_clock_or_native_api_calls: passed.
- existing_owner_pointer_required: passed.
- origin_is_original_trace: passed.
- missing_is_not_empty: passed.
- one_test_main: passed.
- ten_focused_tests: passed.

New test cases:

- `boolean_trace_due_preserves_observed_order_and_never_consumes_owner_timers`
- `boolean_trace_due_unavailable_interval_is_distinct_from_complete_empty`
- `boolean_trace_due_rejects_incomplete_truncated_or_error_coverage`
- `boolean_trace_due_rejects_profile_module_session_and_pump_mismatch`
- `boolean_trace_due_rejects_other_owner_even_when_logical_ids_collide`
- `boolean_trace_due_rejects_ambiguous_or_unregistered_mapping`
- `boolean_trace_due_timer_address_reuse_requires_a_distinct_lifetime`
- `boolean_trace_due_rejects_duplicate_crossing_and_unbalanced_deliveries`
- `boolean_trace_due_rejects_delivery_from_other_thread_id_or_timer_generation`
- `boolean_trace_due_diagnostic_bounds_fail_closed_without_owner_mutation`

Design source: `analysis/resume53-boolean-timer-seam.md`, SHA-256 `b2bad88d21a08659d167e20bab6b3c7dad14f0cc47a733baa37503725a50e430`. Source qualifications remain attached to that plan and its referenced immutable PE contracts.
