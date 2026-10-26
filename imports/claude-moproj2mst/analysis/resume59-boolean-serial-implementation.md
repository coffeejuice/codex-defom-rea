# Frozen isolated serial-gated posted-dispatch prefix

Implemented the new BooleanSerialGate helper from the source-qualified internal message branch67142908..6714296e. It compares an explicitly supplied source-stage serial read against the canonical private+ac projection. On mismatch it retains processedSerial=serial before exposing one pending sendPostedEvents(null,0,sameThreadData) requirement. It never performs that dispatch or infers Boolean timer readiness.

Only the three new files below and this receipt were written. Source is frozen for root review/build. No shared interfaces, CMake, public PostPending, GUI, original runtime or mutable REA state was changed. Worker did not compile or execute tests.

## Implemented scope

BooleanSerialGateState is a canonical diagnostic projection of the admitted dispatcher's private+ac store and dispatcher/private/threadData identities. Its factory requires explicit nonzero observation-local identities and qualification; processed serial is optional and unknown by default. It does not allocate a QObject, native dispatcher, timer or queue, store a HWND/postedWake value, or seed current fields from constructor zeros. Actual native identity/lifetime truth remains the capture/integration layer's responsibility.

Each noncopyable gate frame binds the same actual state pointer/control block, owner identities and positive source occurrence. It retains source-stage message/wParam/fallback timer/serial reads; known inputs cannot be replaced. Unknown observations can be refined without inventing values. Another frame cannot reuse the same occurrence on that canonical state. Admission expiry or lost owner rejects continuation without rolling back source effects; a weak frame does not keep its state alive.

After the actual application/native-filter/userdata prefix is explicitly admitted:

- Message0x401 enters serial comparison without reading a fallback timer discriminator.
- Message0x113 reads the explicit fallback private+b0 discriminator. Zero or full-width wParam mismatch stops before unimplemented registered-timer handling67141ff0; it is not a Boolean due event. Nonzero matching fallback enters the posted-event serial gate. The comparison zero-extends the32-bit field exactly as native EAX and compares to full64-bit wParam, rather than truncating or sign-extending it.
- Unknown discriminator/message/serial/processed fields remain pending. Other message routes remain unimplemented; no false native completion.
- Equal serial returns the bounded native no-dispatch branch with no request/store/callback.
- Unequal serial stores private+ac's projection first, then retains a request containing the same owner/occurrence, null receiver and requested type0. The stage remains PostedDispatchPending and completed=false because actual sendPostedEvents/notify/deletion effects are not implemented. Repeated resume retains the same request and never replays its store or sends a second request.

The helper owns no readiness clock, timer list, model snapshot or callback runner. It neither increments the wake serial nor implements Win32 hook/reset/PostMessage behavior; serialAtGate is supplied as an actually qualified source-stage read (or declared synthetic test input). A later new serial/occurrence requires a new frame. Existing pending frames retain their own source effects while other admitted frames may update the canonical processed field.

The returned request is a retained unclosed native call requirement, not an instruction to repeat delivery on every resume. A future root-owned continuation must preserve owner/frame identity and source call/return/exception order before it can acknowledge completion. No completion API or receiver-removal substitution was introduced here.

## Release-active tests

Ten focused cases use testkit CHECK/REQUIRE, not assert/NDEBUG. All native identities/serials/timer discriminators are explicitly labelled synthetic contract values; none is claimed as captured runtime state. Coverage includes:

- equal serial/no-dispatch and unequal update-before-unclosed-call retention;
- repeated resume, another same-serial occurrence, later different serial and no stale-frame store rollback;
- unknown prefix/message/serial/private+ac refinement without default zeros;
- full32-bit serial equality across supplied maximum/zero values without count/readiness inference;
- fallback timer versus registered timer, unknown discriminator and full64-bit wParam distinction;
- owner alias/state/control-block/source-occurrence substitution and replay rejection;
- changed known source observations, admission expiry and weak-owner replacement;
- unqualified factories/invocations and unsupported message routes.

No C++ test was run by this worker. Root must add src/engine/boolean_serial_gate.cpp to the converter source target and enroll tests/mdt/test_boolean_serial_gate.cpp using existing MDT dependencies. Suggested target/test: m2m_mdt_boolean_serial_gate / m2m.mdt.boolean_serial_gate. The new source uses the existing m2m Error contract and standard C++20 containers; no Qt/Win32 runtime dependency or CMake edit was added by this worker.

Manual declaration/definition/include/whitespace and source-order preflight passed. Compiler/linker/focused/full regression outcomes remain pending root. Diagnostic allocations and occurrence ledger limits are normal C++ behavior, not a reconstruction of native allocation-failure semantics or a generic concurrent Qt implementation.

## Remaining integration guard

This is an isolated source prefix, not public Windows message-loop or Boolean Open completion. Root still needs actual admitted owner/filter/userdata/field inputs, canonical integration with the shared dispatcher/thread/queue state, unclosed posted-event delivery, real timerEvent/modelUpdated timing and full effect-driver behavior. Keep constructor PostPending, nested-callback and public readiness guards. No original HWND/serial/postedWake/timer value was invented or supplied to a public profile.

UTC: 2026-10-24T20:56:55.622990+00:00. Static checks are not executed C++ tests. Token measurements unavailable.

| Frozen file | SHA-256 | Bytes |
| --- | --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/boolean_serial_gate.hpp` | `3926e87cc057b03108942c0d6a3ead4d96889bb84f0f668cbd91cc949e490622` | 3918 |
| `source/apps/moproj2mst/src/engine/boolean_serial_gate.cpp` | `ef8261c3c619920dee9e1ae1f55b8fbcabc52b85c939589124aaed6a9e9ca1ab` | 6309 |
| `source/apps/moproj2mst/tests/mdt/test_boolean_serial_gate.cpp` | `12564d8e48a662222acc44395ab8ba59b289e0344e615ad80da2c3897cb3c8e2` | 9075 |

Static preflight:

- no_native_or_timer_delivery_calls: passed.
- no_clock_or_readiness_inference: passed.
- processed_store_precedes_pending_request: passed.
- explicit_unknown_processed_and_serial_guards: passed.
- full_width_fallback_comparison: passed.
- separate_actual_state_control_blocks_guarded: passed.
- no_source_dispatch_completion_shortcut: passed.
- ten_release_active_tests: passed.
- one_test_main: passed.

Test cases:

- `boolean_serial_gate_equal_serial_returns_without_dispatch_or_other_state`
- `boolean_serial_gate_updates_processed_before_unclosed_dispatch_and_never_replays_store`
- `boolean_serial_gate_unknown_inputs_resume_without_fabricated_zero_fields`
- `boolean_serial_gate_uses_exact_unsigned_serial_equality_without_timer_count_inference`
- `boolean_serial_gate_matching_fallback_timer_is_not_registered_boolean_delivery`
- `boolean_serial_gate_unknown_timer_discriminator_and_full_width_wparam_stay_distinct`
- `boolean_serial_gate_rejects_owner_alias_control_block_and_occurrence_substitution`
- `boolean_serial_gate_rejects_changed_source_values_and_expired_admission_without_rollback`
- `boolean_serial_gate_weak_frame_cannot_adopt_a_replacement_owner_after_expiry`
- `boolean_serial_gate_unqualified_and_other_messages_do_not_run_posted_dispatch`

Governing immutable PE contract: `analysis/resume59-boolean-timing-next.md`, SHA-256 `bd7ddd47e660f81254e28f0238c0a1cab29a9e786973339cd47a28718bb80f19`. Its exact code ranges/import bindings/limitations remain controlling.
