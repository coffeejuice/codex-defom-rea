# CP238 loader next-leaf design

## Candidate

Remove the last direct step-pointer read in the `loader_l3.cpp` dimension predicate by rewriting `modelIs2D(GuiModel&)` to use the existing value API: `return m.dimension() == 2;`. This is a narrow loader/model ownership cleanup distinct from CP237's dimension-flag mutation command and CP235 editor ownership. It requires no new public method and returns only a scalar.

Today `stepIs2D(const DB_STEP_DATA*)` reads the dimension field at `+0x1c`; `modelIs2D` checks `m.step` then passes `m.step.get()` to it. `GuiModel::dimension() const noexcept` already returns the owned step's dimension or 0 when the step is absent. Thus the replacement preserves the bool result: dimension 2 is true; any other present dimension is false; absent step is false because dimension() returns 0.

## Guard and ordering

This helper has no geometry-barrier check today. `dimension()` is likewise a scalar, `noexcept`, unguarded read (see `gui_model.cpp`), so the replacement adds no throw or refusal. In `feDimFlags`, the caller still has its own `if (!m || !m->step) return` before calling `modelIs2D`; retain that existing null gate and all following field-write order unchanged. Do not remove `stepIs2D` if another use appears during implementation; current scoped `rg` shows only this call.

## Focused verification and evidence

Existing `source/apps/moproj2mst/tests/mdt/test_gui_model_dimension.cpp` verifies ordinary dimensions 2/3 and the dimension setter, but currently does not assert the absent-step result. Add a focused case there: construct a `GuiModel`, reset `model.step`, then assert `model.dimension() == 0`. The source implementation establishes that value; the added assertion will verify it for the owner API. Use its configured test `m2m.mdt.gui_model_dimension` alongside `m2m.mdt.engine_l3`. The consumer change is a pure delegation, so no test-only hook or FormingExpress corpus is needed. `test_engine_l3.cpp::forming_active` remains ordinary Forming and is not evidence of FormingExpress dispatch.

Original qualification is limited: `source/docs/re/moproj2mst/ledger.tsv` records original address `0x140c7c0e0` as “engine model is 2D” and marks it ported from the P15 engine-facts evidence; the converter's current helper comment cites the same address. The retained record supports the 2D predicate behavior, but no fresh original execution of this FormingExpress path is established. This migration claims only equivalence between two current source-level scalar access paths, not original runtime parity.

Production file: `source/apps/moproj2mst/src/engine/loader_l3.cpp` only. No C ABI impact. No source, test, build, shared recovery state, original, GUI, or REA changes were made; no tests were run.

