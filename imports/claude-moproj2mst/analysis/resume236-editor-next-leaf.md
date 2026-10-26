# CP236: next Editor300 alias migration after CP235

## Recommendation

Reuse CP235’s `GuiModel::matchesCurrentSimControl(expected)` query at the two remaining `model.simCtl()` identity-only checks in `applyInitialEditorPageStateBound` (`source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`, around lines 278 and 325). This is a small, independent lifecycle consumer migration: both checks obtain a raw control pointer solely to compare its address with `session.sourceControl`; neither dereferences the returned pointer at those sites.

Do not add a second API. The existing CP235 query performs one geometry availability check, resolves the nullable current control internally, compares the current address with the expected token, and returns only `bool`. Keep expected-address comparison semantics; the cached expected pointer is not dereferenced and is not a durable identity.

## Exact call order

At the initial apply preflight, preserve the predicate short-circuit order:

1. `session.pageStateComplete`;
2. `&model == session.sourceModel`;
3. non-null `session.sourceControl`;
4. current-control identity check via `model.matchesCurrentSimControl(session.sourceControl)`;
5. remesh identity.

Do not move the identity query before the earlier guards or after the remesh check. The current `model.simCtl()` call enforces the geometry barrier at this same predicate position; the CP235 query preserves it without returning an alias. A mismatch still returns `UnsupportedSnapshot` before any page/model/file effects.

After page application and Extra3D I/O, preserve `pages.meshing.objects.clear()` before the current-control recheck. When `actualOperation` is present, replace only `model.simCtl() != session.sourceControl` with the inverse of `model.matchesCurrentSimControl(session.sourceControl)`. This check remains after callback-capable I/O and before `applyOwnedRecoveryTail3D`; mismatch still records `recoveryTail = UnsupportedSnapshot` and does not run the tail. Do not add this second query to the `actualOperation == nullptr` path, which currently has no post-Extra3D current-control check.

This migration removes two transient identity-comparison aliases only. It does **not** remove the following `auto& sc = *session.sourceControl`, the page Apply accesses through that cached control reference, or the independent SimControl aliases in `toggleNstart`, `reactionRateClicked`, and the still-separate editor/property paths. Do not describe it as lifecycle closure.

## Exact files and tests

- `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: replace only the two current-control comparison expressions with the existing CP235 value query. No declaration or `gui_model.cpp` edit is required if CP235 lands as reviewed.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp`: extend the existing `initial_editor_actual_operation_rechecks_control_after_extra3d_callback` case for the post-I/O query; it already replaces the current step during Extra3D callback and expects an unsupported recovery tail. Add a preflight replacement case: open a complete session, keep the original step allocation alive, replace `model.step`, call `applyInitialEditorPageState`, and verify unsupported outcome, no page results/recovery tail, no I/O, and no mutation of the retained old or replacement control. The retained old step ensures the expected pointer token remains a stable comparison value during the test.
- Preserve the existing `initial_editor_actual_operation_mismatch_refuses_before_pages_or_io` OPI-binding regression; it covers a distinct earlier guard.
- Root integration should include `m2m.mdt.simcontrol300_lifecycle`, the CP235 `m2m.mdt.simcontrol300_owner` case, `m2m.mdt.viewer_box` for parked geometry refusal, and `m2m.mdt.simcontrol300_stopping_cached`. No Stopping or cache1 code/test needs modification.

This change does not test barrier exceptions from `applyInitialEditorPageStateBound` while parked; its `simCtl()` calls already establish the guard point and the replacement query preserves that point. The viewer barrier suite remains the runtime guard regression.

## Reconciliation and evidence limits

CP222’s editor alias ledger identifies the model/cached-control identity seam and requires treating cached control addresses only as comparison tokens. CP232 moves frequency text parsing/table storage into `GuiModel`; it is a separate operation and is not repeated here. CP235 adds `matchesCurrentSimControl` for `StagedEditor300::pageState`; CP236 reuses it at two distinct lifecycle boundaries rather than proposing another query.

The source evidence is the current `applyInitialEditorPageStateBound` implementation and its tests in `tests/mdt/test_simcontrol300_lifecycle.cpp`. Its post-Extra3D check protects against the explicit test callback replacing `model.step`; this is reconstructed adapter evidence. The leaf only relocates a pointer-equality predicate and does not establish a corresponding original native routine, fresh original execution, process-level lifetime safety, or ownership closure. The larger page Apply still carries `session.sourceControl` across calls and callback-capable I/O, so no general alias safety claim follows.

This is a read-only plan. No source, tests, build, shared recovery state, original process, GUI, or REA state was changed.
