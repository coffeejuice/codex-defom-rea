# CP233: next editor native-alias leaf after frequency-text review

## Recommendation

The smallest independent candidate outside the title and frequency commands is the identity-only `simCtl()` borrow in `StagedEditor300::pageState()` (`source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`, around line 243). The callback does not read or mutate control fields; it only compares `pages_->sourceControl` with the current control address. Move that comparison behind a value-only model query so `pageState()` no longer receives a raw `SimControl*` for identity testing.

Proposed signature:

```cpp
bool GuiModel::matchesCurrentSimControl(
    const def::model::Cls_1401e7b10* expected) const;
```

Implementation contract: call `requireGeometrySyncAvailable()` once, resolve the current nullable control internally from the current step, compare its address to `expected`, and return the equality result. Do not dereference `expected`, require a non-null expected token, or return a control pointer. Preserving raw pointer equality matters here: `pageState()`’s current predicate is `pages_->sourceControl != model->simCtl()`, whose equality permits null/null. Completed editor construction normally establishes a non-null `sourceControl`, but the new query should retain the exact comparison semantics rather than adding an unrelated policy.

## Caller order and guards

Keep `pageState()`’s existing short-circuit order unchanged: OPI identity check, state/page completeness, model guard, source-model identity, then current-control identity. Replace only the final pointer inequality with `!model->matchesCurrentSimControl(pages_->sourceControl)`. This retains the `simCtl()` geometry guard at the same point and does not move a model query ahead of the OPI/page checks. It must not call `pageState()` recursively or refresh/repair the page/session.

`pageState()` is currently `noexcept` while `simCtl()` may throw `Error(M2M_E_UNSUPPORTED)` on a parked geometry model. The proposed method must preserve the same guard and exception behavior; changing the exception policy or `noexcept` declaration is outside this narrow leaf. That existing termination risk should be tracked separately rather than hidden by catching the error here.

This check is read-only and has no Stopping continuation, coordinate read, cache1 access/publication, widget write, or table mutation. It does not replace the later raw control use in `toggleNstart`, `reactionRateClicked`, or `frequencyTextEdited`; CP232’s frequency-text command remains an independent implementation dependency.

## Exact files and tests

- `source/apps/moproj2mst/include/m2m/engine.hpp`: declare the value query beside `simCtl()` and the other control owner commands.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: implement one guard, nullable current-control address resolution, and equality comparison; return only `bool`.
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: replace only the current-control pointer retrieval/comparison in `StagedEditor300::pageState()`.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_owner.cpp`: extend `editor300_completed_pages_remain_owned_but_do_not_complete_native_constructor` or add a focused case using its complete-page fixture. Retain the old step allocation, replace `model.step` with a fresh `DB_STEP_DATA(3)`, and verify `owner.pageState()` refuses while `partialPageState()` remains available. This proves replacement identity is compared without dereferencing the cached control. Existing OPI expiry and partial-page cases continue to cover the earlier short-circuit guards.
- Root’s focused regression set should include `m2m.mdt.simcontrol300_owner`, `m2m.mdt.viewer_box` for parked geometry refusal, and `m2m.mdt.simcontrol300_stopping_cached`. The `noexcept` parked call itself should not be exercised as an exception test; its current behavior is termination because the barrier exception crosses a `noexcept` method.

## Evidence and limits

The concrete source is `StagedEditor300::pageState()` and `GuiModel::simCtl()` in the current reconstructed source. CP222’s `analysis/resume222-editor-alias-ledger.md` explicitly lists the page-state cached-control comparison as an ownership ingress and recommends a checked identity query. Existing focused lifecycle tests are in `tests/mdt/test_simcontrol300_owner.cpp`.

This is an adapter-side identity check, not a recovered original native editor handler. No native control-field semantics or original behavior should be inferred from it. The CP137 frequency evidence and the CP232 frequency-text review do not supply provenance for `pageState()` itself. The recommendation only preserves the current reconstructed C++ predicate and its geometry guard. No source, build, shared recovery state, original process, GUI, or REA state was changed for this read-only note.
