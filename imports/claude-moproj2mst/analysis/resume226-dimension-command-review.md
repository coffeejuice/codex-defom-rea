# CP226 dimension command review — PASS

The dimension setter relocation satisfies `analysis/resume226-dimension-owner-command-contract.md` in the inspected current source. No dimension-command defects found. This is a static source review; focused compilation, runtime checks, full source-stable validation and freeze remain owned by the implementation owner/root.

## Verified production contract

- `include/m2m/engine.hpp` declares `void setDimension(int)` without `noexcept`, documents the unchecked existing-step precondition, and explicitly preserves the staged StepIdentity limitation.
- `src/engine/gui_model.cpp`, `GuiModel::setDimension`, exactly executes `requireGeometrySyncAvailable(); const std::int32_t d = dim; step->set_dimension(&d);`. The guard precedes conversion and native-pointer use. There is no equality shortcut, integer restriction, null-step success, reset, viewer cleanup, identity update or GUI-default initialization. The unqualified call dispatches virtually through the actual owned step; the native header declares its override.
- `class_slots.cpp`, `installLoaderSlots`, keeps the immediate void loader hook and its registration position and delegates to `m.setDimension(dim)`. Removed access/layout includes have no remaining direct `GuiModelAccess`/`DB_STEP_DATA` uses in that file; compilation remains the final include check.
- `l6_slots.cpp`, `dieStressFromDbStep`, preserves absent-operation/model returns, scratch creation, actual-model reset, guarded scratch dimension read, guarded actual setter, then `objIds.clear()`. Only the actual setter callback is replaced. The scratch read still uses `GuiModelAccess::withStep`, preserving its geometry guard and unchecked dereference. Other aliases in this function remain.
- Native `DB_STEP_DATA::set_dimension` remains unchanged: compare dimension, write via `DB_WRITE_VAR::set_dimension`, free contents, initialize. Consequently partial mutation and failure order are preserved by the literal call. Equal dimensions still reach the virtual method. No intentional null-step execution or defined crash claim is introduced.
- `dimension() const noexcept` remains unguarded and unchanged. StepIdentity constructor/reset/destruction instrumentation is unchanged by this command. The new command deliberately does not claim live storage validity.

## Test preflight and meaning

`tests/mdt/test_gui_model_dimension.cpp` uses a Testkit entry point, supported `CHECK`, `CHECK_EQ` and `REQUIRE` macros, resolvable native headers and no C `assert`, filesystem probe or derived-owned-step fixture. Existing CMake `CONFIGURE_DEPENDS` discovery and normal m2m_core/testkit linkage cover its location; no registration result is claimed here.

For dimensions 2 and 3, the equal case verifies step address, object identity/count, seeded native control, wrapper state and mirrors. The changed case verifies the same step address, requested dimension, current object absence/list count, native control defaults and preserved wrapper sentinels. Its separate ordinary native transition qualifies the control-default expectation. It does not compare or dereference an object pointer after destruction. Identity assertions explicitly identify their observation-only limitation. These checks can detect accidentally routing through GuiModel::reset or introducing GUI/viewer cleanup.

`tests/mdt/test_viewer_box.cpp` retains hook success and parked refusal and adds direct `model.setDimension(2)` refusal within the existing byte/state/effect barrier checks. Parked scalar dimension reads remain exercised. No test deliberately invokes the unchecked null-step command. Existing unchanged DieStress and StepIdentity runtime tests remain required at integration.

## Boundaries

The inspected diff also contains concurrent `lastObjectNumber` declaration/implementation and viewer-test hunks. Those belong to the separate query review and are not certified by this dimension-command PASS. No dimension changes touch the public C header, core C ABI adapter, native dimension implementation or cache1 acceptance logic; the C ABI/cache1 gates are unchanged by this relocation. This does not establish ownership closure, native bypass coverage or a fresh original witness.

Only this review report was written. No source/test/build, executable, original application, GUI, REA or shared recovery-state changes were made.
