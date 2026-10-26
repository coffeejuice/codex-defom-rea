# Eligible diagram timer during the last-operation warning

## Bounded evidence

This trace composes two immutable, static contracts for original binary `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore hash `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`:

- `analysis/wave3-boolean-timer-contract.json` traces nonnull diagram attachment to a distinct `QTimer::singleShot(1000, receiver, modelUpdated())`, then through timer delivery, the currently bound model/item, and a possible selection/editor refresh.
- `analysis/wave3-directory-failure-dialog-contract.json` establishes that a modal `QDialog::exec` enters a nested event loop repeatedly dispatching the current-thread event dispatcher. `analysis/wave3-normal-close-guard-contract.json` applies that modal-loop fact to the Close question and explicitly leaves pending eligible timers as possible pre-answer deliveries.
- `analysis/wave3-close-save-return-contract.json` binds the last-operation warning's single Yes answer, Save short-circuit, and Close Yes-to-Cancel mapping.

These are offline PE/DUMPBIN/qualified static analyses; no original runtime, GUI or timer schedule was observed. In particular, they establish a callback that *could* run when eligible, not that this timer is due or delivered during a particular warning.

## One eligible callback route

1. A nonnull model attachment through original view function `0x140170430` registers a separate 1000 ms single-shot timer to that view's `modelUpdated()` slot. The retained normal active-subproject path requests two such timers; no global timer count is asserted.
2. If a timer is eligible while `QMessageBox::warning` is inside its nested modal event loop, Qt dispatches the timer. The single-shot is killed/consumed before the timeout activation and deletion.
3. The receiver is the diagram view, but the model/item are not captured in the timer. `modelUpdated()` first calls the view fit operation, then resolves the currently attached model and its current item. A null model/item skips the selection portion after fit.
4. With a live current item, the callback emits diagram-item-click and UpdateMainView. The original route uses the then-current item ID and active-project/`!loadingSub` gates. For a non-process item with an existing property editor, it applies the old editor, leaves it, constructs/refills the selected item's editor, and shows it. Thus an eligible delivery can change view/selection/editor state while the warning is open.
5. A null rebind does not cancel pending timers. A live receiver still receives the callback and fits; it then skips selection. Receiver destruction disconnects the callback, while the dispatcher-owned timer remains until its own delivery or dispatcher teardown. If the receiver/model attachment changes before delivery, the callback uses the current receiver connection and current attachment, not the attachment that existed at registration.

## Effect on the Save/Close decision

The warning result is derived from the only available button: Yes (`0x4000`). `0x1400b8fb0` returns that result after the modal call. Nested Save `0x1400980d0` sees a non-Yes (`0x10000`) result and exits before current-operation Save or file writers. The normal Close caller maps Yes to Cancel (`0x400000`) and retains the project.

The timer callback can mutate model-view selection/editor state before the dialog returns. The retained contracts do not show it changing the QMessageBox button result or the caller's fixed integer mapping. Therefore the static Save/Close answer path remains Yes → Save short-circuit → Close Cancel; the pre-answer callback effect is a separate state transition. Reentrant close/save behavior or a timer callback changing warning outcome is not established and must not be invented. The later accepted-Close callback `d0e540(0,0,true)` is not reached on this Cancel branch.

## Adapter implication

The existing session event API records distinct receiver/timer identities and requires an explicit due-timer list at a qualified pump point. Its callback consumes a timer before running `modelUpdated`, and resolves the then-current binding. It does not infer readiness from the requested interval or wall-clock time. The last-operation warning currently remains unsupported because there is no source-qualified due-event profile for its modal interval.

To model this branch, the adapter must receive a verified modal-window event schedule (including a qualified empty due set where appropriate), dispatch eligible timer IDs in order through the session-owned live receiver against the then-current attachment, preserve fit-before-selection and null-rebind semantics, then consume the Yes result and apply the existing Save/Close mapping. Without that schedule, neither omitting the callback nor forcing one is evidence-based. Static evidence also does not enumerate all other event callback families reachable through the nested event loop.

No source edits, builds, app-control sessions, runtime captures, or mutable REA operations were performed.
