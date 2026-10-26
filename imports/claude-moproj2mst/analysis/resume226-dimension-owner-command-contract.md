# CP226: dimension-setting owner command

Move the two remaining raw application dimension setters into `GuiModel::setDimension(int)`. This removes an actual callback-delivered native pointer at both callers while preserving the native virtual call, geometry refusal and partial mutation order. It is a compiling migration increment, not live ObjectKey coverage or ownership closure.

This is a read-only contract based on the current source inspected during CP226. CP225's mutation-route audit remains the broader dependency map. No source/test edits, builds, original execution, GUI session, mutable REA operation or shared recovery-state update was performed. Only this report is written. Source owners are active elsewhere, so line numbers are locators; recheck the named functions at integration.

## Exact production contract

Add `void setDimension(int dim);` to `source/apps/moproj2mst/include/m2m/engine.hpp` next to the dimension value query/reset operations. It must return no native pointer/reference and accept only the copied integer. It is not `noexcept`: the existing geometry guard throws, and native cleanup may fail.

In `source/apps/moproj2mst/src/engine/gui_model.cpp`, implement the literal moved operation:

```cpp
void GuiModel::setDimension(int dim)
{
    requireGeometrySyncAvailable();
    const std::int32_t d = dim;
    step->set_dimension(&d);
}
```

Keep virtual dispatch through the actual owned `DB_STEP_DATA`. Do not call `DB_STEP_DATA::set_dimension` with explicit class qualification, replace the operation with `reset()`, precheck equal dimension in the wrapper, initialize GUI defaults afterwards, rebuild viewer state, or validate the integer to 2/3. Those would change the present call/effect surface. The native setter decides whether a dimension equals the current value.

The current selected callers do **not** have null-step no-op semantics. `GuiModelAccess::withStep` first calls `requireGeometrySyncAvailable()`, then invokes its callback with nullable `model.step.get()`. Both selected callbacks immediately dereference that argument for a virtual call. Thus an unparked model without a step has an unchecked null dereference, not a specified false result, exception, or no-op. Preserve this existing supported-domain precondition; do not add a fake step or silently make null succeed. It is accurate to document “requires an existing step; legacy null-step path is unchecked,” but not to claim a defined original crash result from C++ undefined behavior. Geometry rejection must still happen first, including before attempting to acquire/use the step. No deliberate null-dereference runtime test is required for this relocation.

## Caller migration and ownership

Use one source owner for the shared header/implementation and both caller edits, or root must land the common API before releasing dependent caller owners. All paths below are beneath `source/apps/moproj2mst/`.

| File / function | Current operation | Minimum migration |
|---|---|---|
| `src/engine/class_slots.cpp`, `installLoaderSlots`, currently around lines 182–187 | `g_loaderSlots.setDimension` uses `GuiModelAccess::withStep`, converts `int dim` to local `int32_t`, and calls `step->set_dimension(&d)`. | Keep the hook table signature and registration timing, replace the callback body with `m.setDimension(dim)`. This remains an immediate void operation. |
| `src/engine/l6_slots.cpp`, `dieStressFromDbStep`, around lines 433–453 | Returns if no operation/model; creates scratch; resets actual model; reads scratch dimension through a separate guarded callback; then a second guarded callback calls `actual->set_dimension(&d)`; only then clears `opi->objIds`. | Replace only the second callback with `m->setDimension(d)`. Preserve scratch creation, `m->reset()`, scratch read, actual-model guard/setter, and `objIds.clear()` order exactly. |
| `include/m2m/engine.hpp` / `src/engine/gui_model.cpp` | Common owner surface. | Add declaration and body together; keep CP224's unguarded scalar `dimension() const noexcept` behavior unchanged. |

The `int` to `int32_t` conversion moves from the class-slot callback to the owner body, after the same model geometry guard. DieStress already obtains an `int32_t`; Windows `int` is 32 bits on this target. Do not change the hook's public C++ signature in `include/m2m/mst/writer.hpp`.

The class-slot callback is the only use of `GuiModelAccess` and `DB_STEP_DATA` in `class_slots.cpp` in the inspected source. After migration, its direct `gui_model_access.hpp` and `defmodel/layout/db_step_data.hpp` includes can be removed if a compile confirms the rest of the file does not need their accidental transitive declarations. This include cleanup is optional and is not a reason to broaden edits. `l6_slots.cpp` has many other raw step/control/material callbacks; retain its access/layout includes and do not label that file alias-free.

Two production callers reach the hook indirectly: `src/engine/shapeoptim_load.cpp:82–83` after its explicit reset, and `src/ops/noop.cpp:165–166` in DOEOutput after `resetModel`. They require no edit. Preserve the current hook table and its initialization; do not redirect these callers to an extra reset or alter the order of subsequent show/load actions. No other raw `set_dimension` call was found in the scoped application `src` search; native-port KEY/import/DB setters remain independent routes.

## Native callee and effect order

`source/port/defmodel/include/defmodel/layout/db_step_data.hpp` declares `DB_STEP_DATA::set_dimension(const int32_t*) override` and documents its single virtual slot. `source/port/defmodel/src/step/db_step_data.cpp`, around lines 71–80, implements:

1. Read the current dimension.
2. If it differs, call `DB_WRITE_VAR::set_dimension(dim)`.
3. Free native step contents.
4. Initialize the native step for the requested dimension.

`source/port/defmodel/src/step/db_write_var.cpp:35–36` shows that the base setter writes the dimension field. Consequently the dimension changes **before** `step_free_contents`, and it is not rolled back if cleanup later fails. `step_free_contents` destroys objects/materials/control data and clears counts; `step_init` initializes native storage/defaults. The wrapper must preserve this base-write/free/init order and must not add `guiSimCtrlInit`, which `GuiModel::reset()` calls separately.

Equal dimension still invokes the virtual method; the current native implementation then does no mutation. Moving an equality shortcut into the owner would suppress the call itself. Keeping the exact virtual call expression avoids that source-interface change without needing a test-only subclass. `DB_STEP_DATA` has a nonvirtual destructor, so replacing the model's owning `unique_ptr` with a derived tracing fixture would also require careful lifetime handling; do not introduce such a fixture merely to test this relocation.

## StepIdentity qualification

Keep CP222 StepIdentity metadata unchanged in this batch. The existing contract explicitly covers constructor/reset/destruction only; it is an observation, not live storage authority. The direct dimension setters currently do not advance generation. Moving their bodies behind a named command does not by itself authorize expanding that contract.

The new declaration should explicitly say that this staged command does not extend StepIdentity invalidation coverage. After a changed dimension destroys objects, an older CP222 observation may still report current; this remains an intentional migration limitation. DieStress's earlier `m->reset()` already advances generation before its dimension setter, but that does not cover an independently called class-slot setter. Do not use an identity observation to skip the geometry guard or native call.

If a future batch extends observations to this command, specify and test that separately: changed/equal behavior, exhaustion before the first effect, preserved virtual dispatch and failure prefixes, and explicit remaining native KEY/import/DB bypasses. CP225 already explains why instrumenting this one command cannot establish complete live-currentness semantics.

## Minimum focused verification

The relocation is small; reuse existing integration fixtures and add only meaningful coverage for the owner API's mutation boundary.

1. **One focused owner test file**, proposed `tests/mdt/test_gui_model_dimension.cpp`, owns direct-command equal/changed behavior without corpus dependency. Create an ordinary `GuiModel`, establish a native object using already available fixture/native construction facilities, and seed a control value plus wrapper-only sentinel state (`state100` / `viewerMirrorPlanes`). Equal dimension must preserve the node, control value, wrapper state and native step address. Changed dimension must retain the step address, change the dimension, clear old objects and initialize native defaults, while preserving wrapper-only sentinel state. This catches accidentally routing the command through `GuiModel::reset()` or adding viewer cleanup. Do not compare/dereference the destroyed object's pointer after the change; query current object absence/count instead. A small counterpart native `DB_STEP_DATA` transition can qualify the expected native control defaults without assuming GUI reset defaults. Add a short qualification assertion that the old StepIdentity remains the same instrumented observation across this command, with an explicit comment that it grants no storage access.
2. **Existing geometry-barrier fixture**, `tests/mdt/test_viewer_box.cpp`, test `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`: it already checks successful `g_loaderSlots.setDimension(control, 2)` (around line 2718), and refusal while parked (around line 2864), then compares state bytes/effects. Add direct `model.setDimension(2)` refusal beside the hook refusal so the new public command cannot accidentally omit the guard. Retain the existing hook success/refusal tests; they verify the production adapter delegates correctly. The unguarded `dimension()` reads while parked must continue to work.
3. **Existing DieStress route**, `tests/mdt/test_l6_slots.cpp`, `diestress_without_db` around line 364. It already checks the class-substituted no-DB route's dimension 3, empty object/slot state, step number, NSTART, SOLMTD and DTMAX after the migrated call. Run it unchanged. Its comments correctly limit this to the literal no-DB fixture. No duplicated broad DieStress test is needed for replacing one callback.

Root should run the changed targets plus StepIdentity as one focused group: expected new target/test `m2m_mdt_gui_model_dimension` / `m2m.mdt.gui_model_dimension`, existing `m2m.mdt.viewer_box`, `m2m.mdt.l6_slots`, and `m2m.mdt.step_identity`. Confirm names in configured metadata. The existing `tests/mdt/test_l5_loader.cpp` is broader regression context, not a minimum new-test dependency: the two indirect hook users here are ShapeOptim and DOEOutput. The full source-stable launcher will cover the broader registered suite before freeze.

Preflight each new test with `source/port/testkit/include/testkit/check.hpp`: `CHECK`, `CHECK_EQ`, `REQUIRE`, no `REQUIRE_EQ`, no C `assert`, and a Testkit `main`. Resolve native includes by actual files before writing the test; `defmodel/layout/db_step_data.hpp`, `defmodel/layout/llobj_data.hpp`, and `defmodel/llobj_ctor/llobj_ctor.hpp` exist and are the relevant construction/layout headers. Avoid file probes entirely in the new direct-command fixture; any later missing-file probes belong beneath `M2M_TEST_SCRATCH`. Root owns CMake/registration; its `CONFIGURE_DEPENDS` test glob should discover the proposed file without a manual edit. No build/check results are claimed by this design.

## Unexpected alias and scope limits

The adjacent scratch-dimension callback in DieStress still receives a raw `DB_STEP_DATA*`. Replacing it with `s->dimension()` is **not** part of the minimum batch: the scalar CP224 query is unguarded and returns 0 for null step, whereas the current callback first rejects a parked scratch model and then unchecked-dereferences its step. A casual getter substitution changes both guard and null behavior. Keep the read as-is, or migrate it later through a separately specified admitted value operation that preserves the reached semantics. Likewise, subsequent material-copy callbacks and retained `sc` in the same function remain separate alias work.

The setter's destructive native call can invalidate viewer/object/interior aliases without updating their bindings. This migration introduces no new continuation and does not resolve that existing gap. Public `step`, mutable getters, generic callback seams and native KEY/import dimension changes remain. Source closure and R300 cache1 original-witness gates remain unchanged.

The original annotations map the class callback to GUI `0x140c7acd0(W)->vt[0]` and DieStress to GUI `0x140c7ff30` / native setter `0x1401c8ba0`. `source/docs/re/moproj2mst/mdt/L6/spec.md` describes retained Ghidra/decompile provenance and the no-DB scratch path; the current source provides the exact call/order evidence used here. Those records do not establish a fresh original execution, original key identity, or all original null/failure behavior. No reconstructed executable or deployment was produced by this report.
