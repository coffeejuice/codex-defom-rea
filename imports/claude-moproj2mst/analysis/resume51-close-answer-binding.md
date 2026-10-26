# Binding the isolated Close answer helper

## Current boundary

The Close-answer helper is isolated in:

- `source/apps/moproj2mst/include/m2m/engine/close_dialog_answer.hpp`
- `source/apps/moproj2mst/src/engine/close_dialog_answer.cpp`
- `source/apps/moproj2mst/tests/s/test_close_dialog_answer.cpp`

No production Close entrypoint or parent lifecycle currently calls it. This is appropriate: `ConversionSession` exposes lifecycle/timer primitives and `releaseProjectAfterCloseEffects()`, whose contract explicitly says it does not decide Close, call Save, emit notifications, or claim original Close completion. There is no current native-style Close guard to extend safely.

The helper represents only the source-selected process/model guard, qualified normal-question answer, explicit modal schedule dispatch, nested Save answer substitution, and exact Yes-to-Cancel remapping. It intentionally has no project teardown or accepted-Close callbacks.

## Exact future internal seam

When a production Close integration is assigned, add an internal session coordinator between the current caller and the existing Close effects. Keep it private to the conversion/session implementation and do not add a public C ABI operation. Its order should be:

1. Resolve the retained process type and current project/model presence. Preserve the original immediate-allow cases before constructing any dialog request.
2. Build `CloseInput` for the current-operation question only when the guard requires it. Set the `ShippedWindowsStyleInitialFocus` profile only when the selected static style/focus qualification applies; otherwise require a recorded explicit answer.
3. Supply a validated `CloseQuestion` schedule for the exact guard/modal occurrence. The dispatcher must deliver the listed events in order through the session-owned timer IDs and current receiver/model attachment before `resolve` consumes the selected/default answer. A missing schedule, unknown event kind, invalid timer ID, or unsupported callback must fail closed.
4. In the nested-Save callback, run Save while the project and diagram remain attached. If its per-subproject validation encounters the last-operation warning, supply a `LastOperationWarning` schedule and dispatch it before returning the warning's Yes result to the Close helper. Preserve Save's early return before current-operation/file writes.
5. Let `resolve` map exactly nested Save Yes to Cancel. On Cancel, leave the project attached and do not call accepted-Close display or removal callbacks. Only a proven non-Cancel answer may proceed to separately recovered callbacks and teardown, in original order.

This seam composes two modal intervals, not one schedule reused twice. Each profile must identify its boundary and source occurrence. A zero-timer profile is valid only when supported by evidence; an empty `dueTimerIds` vector with an arbitrary nonempty string is not evidence by itself. The helper currently checks boundary/source string and nonempty qualification but does not cryptographically or structurally validate a receipt; the production integration must validate the receipt/profile before constructing `ModalSchedule`.

## Testable internal branch now

The existing focused test can remain isolated and test the answer-mapping layer with injected callbacks, without enabling public Close:

- A nonqualifying process type or absent model returns `CloseAllowed` without requiring modal schedules.
- A qualifying process/model with the shipped-style/initial-focus profile dispatches the Close question schedule before entering nested Save.
- The nested-Save callback receives a dispatcher; for a last-operation warning it dispatches the matching warning schedule before returning `DialogAnswer::Yes`.
- The result is `CloseCancelled`; the two modal intervals are dispatched in order. An absent or mismatched warning schedule throws before a Cancel outcome can be reported.
- No project-detach or accepted-Close callback is present, so this branch cannot accidentally release the model or pretend the later display mutation is known.

This branch is a unit of integer-answer/profile composition only. It does not prove original timer readiness, UI default behavior at runtime, all event callbacks, or full Close lifecycle parity. The original runtime validation gate remains separate.

## Current caveat

The helper's adapter contract is deliberately strict but its callbacks are injected abstractions. In particular, `NestedSaveResult` says a warning occurred and supplies a schedule, while the nested callback is responsible for dispatching that schedule before returning. The helper checks that a matching warning schedule was dispatched, but it cannot verify callback side effects or immutable receipt authenticity. Keep the helper unbound until an owner adds a typed verified-profile validation layer and tests the actual session dispatch/Save sequencing.

No source/build/GUI/mutable-REA change was made for this binding design.
