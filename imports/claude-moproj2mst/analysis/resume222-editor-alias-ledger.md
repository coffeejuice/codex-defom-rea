# CP222 editor alias migration ledger

Scope: source audit for moving SimControl300/editor access behind concrete `GuiModel` ownership callbacks. This is a design ledger, not an implementation. No source or tests were changed. The existing explicit cache1 refusal remains the boundary: do not manufacture stopping coordinates when the cached native flag selects that path.

## Existing contract

`src/engine/gui_model_access.hpp` has a narrow `GuiModelAccess::withStep(GuiModel&, F)` seam. It calls `requireGeometrySyncAvailable()` before borrowing `model.step.get()`, preserves a nullable step, and accepts only callbacks returning `void` or `bool`. The comment correctly identifies callback capture/alias escape as a code-review boundary; the template cannot itself prove non-escape. `GuiModel` owns `step`, exposes `simCtl()`, `objectAt(position)`, `object(number)`, and `objectCount()`; these accessors are the natural owner entry points. Do not retain any returned step/control/object pointer past the callback.

## Per-site migration ledger

| Site | Current ingress / alias | Required owner operation | Guard and migration note |
|---|---|---|---|
| `src/engine/simcontrol300.cpp:287,298,532,628,685,692,1255,1523` | Direct `model.objectAt(i)` in object scans and recovery-table reconstruction | `GuiModel::withObjectAt(position, callback)` or one scoped `withObjects(callback)` traversal | Preserve position-vs-object-number distinction and null holes. Callback must not return/store `LLOBJ_DATA*`; return copied scalar/typed records only. |
| `simcontrol300.cpp:375,393,470-472` | Step identity comparisons, node coordinate shape and raw `model.step->u15f8` reads | `GuiModel::withStep` callback receiving named `StepView`/borrow token | Keep owner identity validation (`set.u130 == step`) and dimension/count overflow checks within the owner callback. Raw pointer must not escape. |
| `simcontrol300.cpp:663-692` | Step dimension, `simCtl()` identity, TIMEID/ERECID widths, object scan | Single `withStepAndControl` operation, then scoped object visit | Check step and control together so callback cannot mix generations; preserve null-step and `control != &sc` refusal. Copy widths/entries before leaving. |
| `simcontrol300.cpp:1078,1130,1203,1251,1318,1340` | Repeated `!model.step` and dimension checks before page apply | `withStep` with typed `StepIdentity`/dimension result | Avoid creating a fake/default step. Keep each page's existing preflight order and partial-write boundary. |
| `simcontrol300.cpp:1495,1593,1628` | Step/control identity checks and `sc.owner == model.step.get()` | `withStepAndControl` | Validate all identities within one borrow; retain `UnsupportedSnapshot` outcome and exact caller-visible mutation ordering. |
| `src/engine/simcontrol300_owner.cpp:243` | `pages_->sourceControl != model->simCtl()` cached control identity | `GuiModel::withSimControl` / `withStepAndControl` identity query | Treat cached pointer only as an identity token; do not dereference stale source control. Continue rejecting mismatched page/model binding. |
| `simcontrol300_owner.cpp:266,297,328,389,412` | `binding.model.simCtl()` at callback entry for NSTART, title, reaction, curve, frequency/index, view-factor and solver events | `Editor300Binding` owner operation yielding a scoped `SimControlView` plus checked control identity | Enforce binding identity before touching cached widgets. Reentrant notification must not reuse a previously borrowed pointer; reacquire via `GuiModel` on each delivery. |
| `simcontrol300.cpp:663,677-678,1495,1593,1628` | `simCtl()` and `SimControl::owner` relationships | Typed `SimControlIdentity`/`SimControlSnapshot` returned from owner callback | Distinguish null control, wrong control, absent step, and wrong owner. Do not convert to default success. |

`simcontrol300_owner.cpp` already centralizes editor callback delivery and verifies cached widgets against delivered values (NSTART checkbox, title text, frequency text/index). Keep these checks before model mutation. Where a callback emits another UI notification synchronously, end the current borrow before dispatch and reacquire for the nested event; add a reentrancy test proving no stale alias is reused.

## Boolean lifecycle/editor ingress

Audit the Boolean-specific lifecycle entry points as the same ownership problem, not as an excuse to merge their semantics into SimControl300. The relevant current surface includes `include/m2m/engine/boolean_lifecycle.hpp`, `src/ops/boolean.cpp`, `src/engine/boolean_lifecycle.cpp`, `boolean_external_callback.cpp`, `boolean_close_flush.cpp`, and Boolean property/save handlers. Any Boolean callback that reaches the active operation/model must receive a fresh `GuiModel` owner binding, use the same scoped step/control/object operations, and return copied result types. External Boolean callbacks and close/flush callbacks are explicit reentrancy/alias-escape boundaries: never capture the borrowed step/control/object in a closure, deferred event, timer or stored field. Preserve Boolean lifecycle's distinct timer, save-buffer and close-flush order.

## Named operations and value types required

Recommended API additions (names are contracts for the migration plan; final declaration placement may follow existing `GuiModel` headers):

- `GuiModel::withStep(F)` → callback-scoped `StepBorrow` or existing nullable `DB_STEP_DATA*` adapter.
- `GuiModel::withSimControl(F)` → callback-scoped `SimControlBorrow`, preserving absent control.
- `GuiModel::withStepAndControl(F)` → coherent `StepControlBorrow` containing nullable step/control and owner identity.
- `GuiModel::withObjectAt(int position, F)` and `GuiModel::withObjectNumber(int number, F)` → callback-scoped `ObjectBorrow`.
- `StepIdentity`, `SimControlIdentity`, `StepControlIdentity`, `ObjectIdentity`; copied values only, no raw pointers.
- `StoppingCoordinateSnapshot`, `RecoveryEntrySnapshot`, `SimControlPageSnapshot`, `EditorEventValue` for values crossing callback/UI boundaries.
- `BorrowResult<T>` or operation-specific report enums must distinguish missing owner, identity mismatch, unsupported snapshot and completed result; avoid a generic default object.

Callback constraints should ban pointer/reference return types and constrain accepted callback results to `void`, `bool`, or explicit value/report types. This is a compile-time guard only; capture escape still needs targeted review and tests.

## Focused tests

1. `GuiModelAccess` rejects reference/pointer return types at compile time; nullable step behavior and pre-borrow `requireGeometrySyncAvailable()` ordering remain intact.
2. Step/control pair identity is coherent across reset/replacement; callbacks see null or mismatch explicitly and cannot carry old aliases into the next call.
3. Object traversal preserves sparse positions and distinguishes position lookup from numbered lookup; callback results contain values, not pointers.
4. Stopping cached flag cache1 continues to refuse coordinate reconstruction with the established issue/stage and no coordinate getter call; cache0 reaches the existing getter path. Do not relax the cached guard.
5. Nested/reentrant editor delivery (for example NSTART or frequency change triggering a notification) reacquires the current owner and cannot use an alias from the outer callback.
6. Boolean external callback and close/flush tests assert event order, timer/buffer effects, and that a model reset during callback invalidates subsequent borrows rather than yielding stale pointers.
7. Existing page tests retain partial-mutation order and `UnsupportedSnapshot` results for identity mismatch; run through Testkit, not C `assert`.

## Independent ownership

This ledger owns only `analysis/resume222-editor-alias-ledger.md` within `imports/claude-moproj2mst`. No source files, tests, shared status, or build artifacts were edited. Parent integration should map each row to its separately assigned source owner before implementation; do not infer authorization to edit these sites from this document.
