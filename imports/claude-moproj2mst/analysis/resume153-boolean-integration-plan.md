# CP153: bind the existing notifier owner before choosing a Boolean callsite

The canonical notifier is reachable from the candidate Boolean lifecycle without adding another owner. The missing part is admission of the native constructor stage, not access to the notifier. A reset placed in ordinary editor fill would execute at an unsupported boundary. No Boolean production mutation is ready to enable from CP152 alone, and a new isolated prefix would not close that gap.

Only this report was written. This lane inspected current candidate source and the frozen CP152 qualification. It made no source edits, builds, tests, GUI/app-control calls, original execution or mutable REA access.

## Existing owner and narrow lifecycle site

`ConversionSession` creates its one `SessionNotificationState` in `src/engine/conversion_session.cpp:156`. Its `FlowReceiverWitnessRegistry` holds a weak reference to that same owner; the Session binds the registry into its actual Project at line 208. The existing bridge is:

`Project::flowReceiverWitnessRegistry()` -> `FlowReceiverWitnessRegistry::owner()` -> canonical Session notifier.

The Project getter is implemented in `src/model/subproject.cpp:282`; a bound-but-expired registry refuses. The registry getter returns its weak notifier and supplies **identity access only**. It does not prove native registration, a live MainWindow receiver, constructor progress or event delivery. It must not be used to prolong native liveness, seed a second notification state, or treat a released registry as a live Session. Candidate Session ownership/lifetime must still be checked at any effect boundary.

The narrow existing Boolean creation hook is `BooleanClass::objectTreeFill` in `src/ops/boolean.cpp:63`, reached by `postLoadViews` at `src/engine/postload_views.cpp:194`. It calls the installed `booleanEditorFill` slot directly. `src/engine/boolean_editor.cpp:22` implements the later selection-fill body corresponding to native `1411e25b0`; its first action resets the candidate editor record. The class hook is also called by `reselectBooleanObjectEditor` at `src/engine/boolean_lifecycle.cpp:137`. Therefore a shared flag change at fill would affect both initial Open and replacement/reselection.

Native CP152's order is different in detail: actual editor allocation/base construction -> operation guard binding at `130857` -> notifier callee entry from `130ad1` -> remaining base construction -> Boolean table publication at `11e24b7` -> selection fill. Current `BooleanObjectEditor` (`include/m2m/mst/loader_l5.hpp:37`) stores only shown/text values. It has no source-stage constructor frame or actual QObject/OPI guard lease with which to admit `130ad1`. `shown`, existing properties, class-current equality and a model pointer cannot substitute for that admission.

The existing staged editor base does not supply it: `src/engine/editor_base.cpp:471-475` separates returned QWidget from unrecovered application wrappers, and belongs to `simcontrol300::StagedEditor300`, not this Boolean E. Its retained QWidget tail/PostPending guard cannot be borrowed as a Boolean constructor receipt.

## Prefer the notifier cut; defer W+48

The notifier cut needs no E/H delivery or MainWindow callback: on a genuinely reached `2cf3c0` callee entry, read the existing shared `suppressed` once. True preserves `flag90`; false writes that same owner's `flag90=false` before stopping at `2cf41f` key construction. A consumed constructor frame must not reread suppression or replay the store on retry. This is the cut to integrate when an actual Boolean constructor-stage owner reaches `130ad1`.

That instruction pair is already implemented for other admitted callers. In particular, `src/engine/close_tree_notification_prefix.cpp:53-83` checks Session/manager/notifier ownership and consumes the gate/reset once. Its progress constructor requires a reached Close busy prefix and exact Close occurrence. Reusing that progress for Boolean would fabricate a Close prerequisite; simply duplicating it in a new Boolean diagnostic helper would add no lifecycle reachability. Leave it unchanged for this task.

The W alternative is larger and currently less ready. `MainViewState` represents the main view, not MainWindow W. `WidgetAreaPrimaryWindow` is identity-only and expressly supplies no native Qt construction, liveness or field values. Neither owns a current `W+48` QPointer cell. A nullable payload is also insufficient to identify the guard target's lifetime or cbc920 completion. Do not add W+48 to MainView, seed it from a selected operation/model, or create a second synthetic main-window cache. CP152's null-preserves-old rule remains a contract for the future real W owner.

## Exact prospective write set and stop condition

No new helper, CMake change, public event enable or source edit is proposed for CP153. For a later root-assigned **integrated constructor** increment, the narrow reviewed write set would be existing files under `source/apps/moproj2mst/`:

| Existing file | Required purpose |
| --- | --- |
| `src/ops/boolean.cpp` | Call the constructor-stage owner before the existing fill slot, only at its actually reached operation-wrapper notification phase. Preserve a distinct replacement invocation. |
| `include/m2m/mst/loader_l5.hpp` | If the actual constructor frame must be retained with the existing Boolean editor record, declare that owned frame here; do not default it to reached/complete or erase it with the selection-fill reset. |
| `include/m2m/engine/boolean_lifecycle.hpp` | Declare the integrated owner/stage boundary, without a free-standing capture-label admission API. |
| `src/engine/boolean_lifecycle.cpp` | Resolve and revalidate the already bound Project/Session notifier owner; consume only the qualified constructor gate/reset; stop before key/receiver work. |
| `tests/mdt/test_boolean_postload_creation.cpp` | Exercise the actual Open hook, keeping the zero-OPI-slot postLoad creation distinct from Apply and constructor notification. |
| `tests/mdt/test_conversion_session.cpp` | Verify the actual Session notifier owner, cross-Session rejection and lifecycle lifetime/retry boundaries. |

This is a dependency/write-set plan, **not approval to invent the frame or claim those declarations are already sufficient**. Before assignment, close the native operation-wrapper stage that actually reaches `130ad1` and identify its candidate retained owner. If that stage cannot be bound, do not insert a reset at fill and do not allocate the proposed frame merely to admit itself. A string qualification, raw Boolean flag or test-only token cannot certify reachability. An existing Project registry can resolve the notifier but cannot provide the missing stage receipt.

No `SessionNotificationState` field change is needed for the two existing canonical cells. No `view_manager`, `widget_events`, primary-window/area owner, dispatcher or raw Apply API belongs in this notifier-only write set. If the constructor owner requires a shared Session interface beyond the existing bridge, root must review that additional ownership before release.

## Focused tests for an admitted integrated increment

Use existing Release-active Testkit targets; these names describe proposed cases, not tests run in this lane:

- `boolean_constructor_unreached_notification_stage_preserves_shared_notifier`: actual Open hook cannot reset/consume merely from shown/current/model values.
- `boolean_constructor_suppressed_entry_preserves_flag90_before_fill`: known reached callee, same canonical owner, preserved flag and no registry/key/receiver effects.
- `boolean_constructor_unsuppressed_entry_resets_before_key_guard`: actual owner's flag changes once before refusal; selection fill and Apply have not run past the guard.
- `boolean_constructor_notification_retry_does_not_reread_or_replay`: change suppression and flag after the first stop; retry preserves those later values.
- `boolean_constructor_notification_rejects_foreign_or_expired_session_before_store`: compare live ownership/control blocks; retained diagnostic leases cannot resurrect a dead Session.
- `boolean_replacement_has_distinct_constructor_notification_occurrence`: initial Open and reselection cannot share a consumed frame.

Keep the existing postLoad zero/nonzero-slot tests unchanged in meaning. Also retain `close_tree_prefix_suppressed_callee_preserves_actual_shared_notification_state`, `close_tree_prefix_unsuppressed_store_precedes_unknown_key_and_population` and `close_tree_prefix_retries_never_reread_suppression_or_replay_flag_store` as regression coverage if a later shared refactor touches that implementation. No test expectation may infer same-E/H dequeue, MainWindow slot delivery, cache mutation, Apply or addObject from notifier reset.

## Next evidence action

Trace only the wrapper segment after the actual operation-guard bind `130857` to the `130ad1` caller argument construction and identify the retained constructor stage that can own it. Then reconcile that stage with the candidate creation hook and its earlier guarded QWidget/PostPending prerequisites. This is a source/candidate ownership problem separable from the live same-E/H witness. If no owner-bound stage exists, retain the public timing guard and report that precise gap; do not repeat the post/compression proof or create another isolated helper.
