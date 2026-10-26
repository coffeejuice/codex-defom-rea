# CP228 loader unit owner command contract

## Decision

Use `GuiModel::setModelUnit(std::int32_t unit, bool reset)` as a named owner command; do not add a `createModel(dim, unit)` factory. The factory would combine allocation and mutation into a new lifecycle operation, while the existing code clearly sequences shared allocation, unit mutation, and return. A method call on the already allocated owner closes the raw `step` alias and leaves that sequence intact.

## Exact behavior and ordering

`source/apps/moproj2mst/src/engine/loader.cpp::newModel(Project&, int)` currently:

1. Evaluates `projectDb(p)` and constructs a shared `GuiModel` with `dim`.
2. Calls `def::key::set_model_unit(*m->step, p.unit, true)`.
3. Returns the same shared pointer.

`GuiModel::GuiModel(DbHandle*, int)` in `src/engine/gui_model.cpp` allocates its step and initializes GUI simulation controls before `newModel` sets the project unit. `def::key::set_model_unit` (`source/port/defkey/src/item_writers/model_unit.cpp`) changes `sim_ctrl.unit` only when different; with `reset=true`, a change invokes `reset_unit_defaults`. Equal unit makes no change. Therefore implement the named method as a thin delegation to the existing `def::key::set_model_unit(*step, unit, reset)`, and change only the call in `newModel` to `m->setModelUnit(p.unit, true)`.

Do not move unit into the constructor or factory, reorder it before GUI initialization, change `reset=true`, precheck equality, or catch exceptions. Allocation/constructor failure still occurs before unit mutation. Any unit-setter exception still propagates before return and destroys the local shared owner during unwinding, matching current semantics. The same initialized shared object is returned on success. Existing null-step behavior is not defined by this current path because the constructor always creates one; do not introduce a null fallback.

## Files and ownership

Production edit set: `source/apps/moproj2mst/include/m2m/engine.hpp` (declaration), `source/apps/moproj2mst/src/engine/gui_model.cpp` (implementation and existing defkey include/dependency), and `source/apps/moproj2mst/src/engine/loader.cpp` (single caller). These are separate from CP226's reported owned paths, but coordinate the common model header/implementation with the current owner before editing. No C ABI changes: this is an internal C++ method, and `m2m/moproj2mst.h` / exported C surface remains untouched.

## Focused verification

Add or extend one small model-owner test, preferably `source/apps/moproj2mst/tests/mdt/test_gui_model_unit.cpp`, registering through the existing `tests/mdt/CMakeLists.txt` glob if applicable. Test both changed and equal unit against a native `DB_STEP_DATA` baseline using `def::key::set_model_unit` as the reference: changed unit with reset=true updates unit and the reset-controlled fields; equal unit leaves a seeded reset-sensitive value unchanged. Verify the owner method returns no native pointer/reference at compile-time/API review. Then exercise `newModel` through the narrowest existing loader test seam with `Project.unit` values that differ and match the constructor default; assert successful owner state and project-unit result. Also keep a failing-construction/allocation case only if an injectable test seam already exists; do not add a factory solely to make one. The observable failure contract is sequencing/propagation, which code inspection establishes.

Preflight new test against Testkit macros and actual test target registration. Existing `tests/mdt/test_gui_startup_unit.cpp` concerns loaded unit versus GUI constructor global and is not a sufficient test of `newModel`; avoid changing it unless its setup directly covers this route. Keep the focused test independent of C ABI smoke tests; integration should run the new owner test and the existing loader target together.

## Evidence boundary

Source evidence supports the current call sequence and exact unit behavior. No original executable or runtime test was used for this design. No files outside the requested report are changed; no source, build, shared recovery state, original, GUI, or REA state is modified.
