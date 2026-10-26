# CP125: DOE notifier reset and failure-order audit

## Finding

No contradiction was found in the newly frozen `requireRefreshDelivery` prefix for the three **ordinary XML return** profiles. It resets the actual Session-owned `flag90` only when `suppressed == false`, and it does so after the XML writer's normal return and the recorded refresh boundary. XML open failure and post-close error are both normal Boolean returns in this candidate path, so both reach that prefix. Suppression skips the reset but does not make the public Save complete: it still reaches the retained receiver-delivery guard. An interrupted XML writer does not reach the reset.

This is a candidate source/test contract, not proof that a native Qt receiver ran or that a timer stopped. `RefreshBoundaryReached` only records that the wrapper's signal call-site boundary is next.

## Ordered evidence

1. `src/doe/type2_xml_prefix.cpp:84-107` opens the InputTable device. If open fails, it records `writerSucceeded=false` through `observeInputTableXmlWriterReturn` and returns normally. The comment at line 107 reflects original wrapper `0x1400d3200`: its `signalDoeTableRefresh()` call follows the writer on this ordinary-false path.
2. On successful open, it builds the empty-variable InputTable document, streams it, closes the device, then reads `error()` (`type2_xml_prefix.cpp:109-128`). A nonzero post-close error sets `writerSucceeded=false`, but the function still records a normal writer return and returns. The `streamFailed` fact is retained separately.
3. `src/moproj/write.cpp:143-160` records the exact latest normal-return event, marks `RefreshBoundaryReached`, gets the actual Session's `DoeTableOwnerState`, then invokes `requireRefreshDelivery`. The candidate's normal public path still stops there.
4. `src/doe/table_owner_state.cpp:233-245` first validates that the admission scope and notifier belong to this Session. It then follows the native suppression order: if unsuppressed, write `flag90=false`; if suppressed, leave `flag90` unchanged. It subsequently refuses on unavailable main-window `+0x458`, or on the still-unimplemented timer/UI delivery boundary. No key insertion, receiver invocation, callback normal return or PAR continuation is represented.

The frozen focused test `tests/doe/test_refresh_notifier_prefix.cpp` covers these distinctions:

- Test `refresh_notifier_actual_save_retains_unsuppressed_store_after_all_normal_xml_returns` seeds `flag90=true` diagnostically and checks it becomes false for successful XML, failed open, and post-close error. Failed open preserves the prior recorded file. Post-close error records closed bytes and a false writer result. All three stop after XML at the main-window guard with no receiver effects.
- Test `refresh_notifier_suppressed_callee_prefix_keeps_flag_and_public_continuation_guard` covers both initial `flag90` values and confirms suppression preserves them while the normal writer boundary is reached and public continuation remains guarded.
- Test `refresh_notifier_interrupted_xml_never_reaches_scalar_prefix` raises after open while constructing the XML payload; it records open/close and empty requested stream bytes, but no normal writer return or refresh boundary, and `flag90` remains true.
- The foreign/released-scope test confirms owner validation precedes either notifier mutation. The constructor-field receipt test distinguishes a field observation from actually executing the dispatcher prefix.

These are explicit fault-injection/model tests; they do not establish original runtime outcomes for equivalent filesystem faults.

## Caller boundaries and non-equivalences

- **DOE session-selection/Open boundary:** `src/doe/session_records.cpp:191-208` emits `ChangeStepRange`, then falls back to part zero, sets current session to `-1`, then emits `OpenSession`, `ChapterChanged`, and `NumChaptersChanged`. `observeEmptySessionNotification` (`src/doe/session_notifications.cpp:50-69`) gates each on the same suppression byte and, when unsuppressed, resets `flag90` before the empty-key insertion. The default-task path at `session_records.cpp:234` separately observes `GetCurrentStepViewInfo`; the return-valued `CreateTaskBase` path explicitly shares suppression but does **not** reset `flag90` (`session_records.cpp:224-236`, `session_notifications.cpp:10-34`). Thus an earlier Open/session signal may already have cleared the value; tests deliberately reseed it to isolate the Save occurrence.
- **Close boundary:** the retained `analysis/wave3-normal-close-diagram-lifetime-contract.json` records native normal-Close order `0x140029a54..0x140029a76`: set busy, call `0x14002aae0`, obtain the model manager, clear it. `0x14002aae0` requests tree, property, and scene refreshes, subject to notifier `+0x141`. Candidate `src/engine/close_effects.cpp:202-214` likewise returns immediately when suppressed and otherwise resets `flag90` before it requires the RemoveGraphItems registration population. The separate `close_tree_notification_prefix.cpp:55-76` models the same generic notifier gate for TreeEnableSelection and stops at key construction. Neither is the Type2 Save `DoeTableRefresh` dispatch, so their resets cannot be used as evidence that Save's receiver delivery completed.
- **Schedule boundary:** the retained material inspected here identifies the DOE session-selection notification sequence above, but does not establish an additional Schedule-specific caller of `DoeTableRefresh`, nor a timer callback that returned normally for this Save occurrence. Do not extend this audit's Save result to a scheduling/run action.

## Conclusion / next boundary

Keep the current public guard. The frozen reset is source-ordered correctly for ordinary XML returns, including both known false-return classes, and its suppression branch preserves the scalar. The next parity dependency remains the actual, same-occurrence `DoeTableRefresh` receiver/timer/slot completion and its normal return; no Schedule or Close evidence closes that dependency.

No source, test, build, GUI, original application, or mutable REA state was changed by this audit.
