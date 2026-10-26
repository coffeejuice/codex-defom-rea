# CP270 Advanced Apply next-leaf review — HOLD

## Verdict

Do not replace only `model.step->dimension != 3` with `model.dimension() != 3` inside `advancedApplyProfile`. The substitution preserves the dimension result for ordinary, unparked models, but it does **not** preserve the exact null/parked/identity short-circuit order. No source or tests were changed.

## Current order and effects

`advancedApplyProfile` currently evaluates this left-to-right:

1. `!model.step`
2. `model.step->dimension != 3`
3. `model.simCtl() != &sc`
4. `advancedTimeProfile(model)`
5. the `timeRadio` constraint
6. element/node option checks, then user-string checks

The first two terms refuse a missing step and every non-3D dimension before any geometry-synchronized query. `model.simCtl()` calls `requireGeometrySyncAvailable()` before returning the control pointer. Consequently, with the geometry query parked, a missing-step or non-3D model returns `false` at terms 1 or 2; a 3D model reaches term 3 and throws at the barrier. Identity mismatch is checked only after that query and before time-profile reads or Apply stores.

`GuiModel::dimension()` returns `step ? step->dimension : 0` and is `noexcept`, so it makes the first two terms equivalent for unparked null/non-3D/3D states. But in the proposed single-term replacement, null and non-3D now pass the new first term (`0 != 3` is true) and proceed to `model.simCtl()`. If the geometry query is parked, those cases throw instead of returning `false`. Thus it changes observable refusal/exception order. Moving or adding a guard would be a larger edit and is outside the requested one-term design.

For an unparked model, null/non-3D still fail before identity; valid 3D still checks identity before time/profile checks and stores. For a parked 3D model, the same barrier remains at `simCtl()`. The CP258 remap is downstream: `applyAdvanced3D` calls `model.remapEditorAdvancedState()` only after all stores, so this candidate does not reorder the remap itself; the mismatch is admission behavior before it.

## Caller and test evidence

The production actual caller is `applyAdvanced3D(sc, model, form)`, which directly invokes `advancedApplyProfile` before constructing the UTF-8 string array, validation, stores, and final CP258 remap. The owner-bound recovery-tail and empty-recovery-tail callers also invoke `advancedApplyProfile` in their existing later positions; they are distinct call sites whose earlier recovery-list effects must not be attributed to the initial `applyAdvanced3D` admission.

`test_simcontrol300_snapshot.cpp` exercises the actual `applyAdvanced3D` entry, not a helper-only substitute. Existing cases cover successful state writes/remap and unsupported snapshots with no initial time store, including live object/time-profile changes between fill and apply. CP258’s review records the focused lifecycle/snapshot CTest pass and confirms the final remap follows all page stores. These are useful caller and effect-order evidence, but no located test asserts this exact missing/non-3D plus parked-barrier ordering for `advancedApplyProfile`. Do not claim that the existing suite validates this proposed alias substitution.

## Original evidence qualification

`wave2-save-diffs-300-advanced-contract.json` qualifies its support as static recovery of a bounded initial-editor branch: the original application was not executed, there is no public lifecycle acceptance claim, and native instruction-shape matches are not whole-call-graph equivalence proof. It supports the recovered bounded Advanced Apply field and remap order, not a new original runtime observation for parked models or this alias change.

## Ready condition

HOLD for the exact one-term proposal. Reconsider only with a design that retains the old short-circuit behavior for missing-step/non-3D models while respecting the parked-query barrier, plus actual-caller regression coverage for the affected parked and unparked cases. This note is a static design review only; no source, tests, shared state, build, GUI, original executable, or mutable REA state was touched.
