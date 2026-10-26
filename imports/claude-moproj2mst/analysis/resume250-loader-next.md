# CP250 loader next-leaf audit (revised CP251 contract)

## Candidate and owner command

Migrate the final two sim-control writes in `source/apps/moproj2mst/src/engine/multiblow_load.cpp::multiBlowLoadPrologue` into one void `GuiModel::initializeMultiBlowLoadControl()` command:

```cpp
void GuiModel::initializeMultiBlowLoadControl()
{
    auto* control = simCtl();
    if (!control) return;
    control->dttype.u58 = 0;
    control->nstep = 10000;
}
```

Do not mark it `noexcept`: `simCtl()` applies the geometry barrier and can throw. It performs exactly one guarded nullable `simCtl()` call, then the ordered DTTYPE mode reset (`SC+0x240`) and NSTEP write (`SC+0x16c`). Missing control/step is a no-op after the guard; no model or step is created.

## Caller and effect order

Keep `multiBlowLoadPrologue` as: base load prologue; `setTrans2(p,op,true)`; `setTrans1(p,op,true)`; resolve `opModel` and return if null; then invoke `m->initializeMultiBlowLoadControl()` as the final operation. Remove the existing caller-level `simCtl()` guard and raw writes so the final segment has only the command's single admission. Do not merge or hoist the independent provider calls or their guards inside the earlier base/TRANS operations. If model lookup is null, earlier effects remain as today. If the step is absent, the final command does nothing. If geometry is parked, the command throws at its `simCtl()` access after the same earlier prologue effects; it does not roll them back. No C ABI change.

CP249's common-file reservation was the sequencing blocker when CP250 was written. This report is a design update, not authorization to edit `engine.hpp` or `gui_model.cpp`; root must reconcile CP249's current source-stable freeze/ownership state before assigning the command implementation. Do not create a loader-local shim.

## Focused tests

1. Keep actual class-prologue dispatch in `source/apps/moproj2mst/tests/mdt/test_multiblow_load.cpp::multiblow_prologue_and_class_data`. Seed both `dttype.u58` and `nstep` with distinct nondefault values immediately before `loadPrologue`; retain assertions for final mode 0, NSTEP 10000, base TNOW and TRANS effects, and add checks that unrelated control fields remain unchanged where fixtures support it. This is final-state regression only; it does not dynamically prove DTTYPE-before-NSTEP order.
2. Add a direct owner-command refusal call inside the established parked geometry block of `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp::geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`. Use its `refused` helper and retain whole-step/effect/state comparisons. Calling the whole prologue while parked is not a substitute because earlier base/TRANS operations can refuse before this command.
3. Add a direct ordinary `GuiModel` absent-step case: reset `model.step`, call `initializeMultiBlowLoadControl()`, and assert no exception and no storage recreation. Do not test by deliberately dereferencing null.

Resolve configured test names before execution; expected targets are `m2m_mdt_multiblow_load` / CTest `m2m.mdt.multiblow_load` and the existing viewer-box target / `m2m.mdt.viewer_box`. Include both in the focused group.

## Source and original qualification

`evidence/gui_mo/s1/mdt/L4/d0.c` function `0x140601f70` and `slots.asm` show the final model-control accessor, DTTYPE setter at `0x140601fec`, then NSTEP write at `0x140601ff6`. The L4 spec §2 and ledger agree. This retained decompile/assembly qualifies the source order; final-state unit assertions cannot prove that dynamic order.

`source/docs/re/moproj2mst/mdt/L4/spec.md` §6 records historical fresh-GUI Oracle B cases `93_B_MB3ACT` and `94_B_MB2ACT`; those are saved outcome witnesses, not a new original execution or direct observation of transient prologue ordering. No current corpus project has a MultiBlow operation: the converter test reclassifies corpus 42's operation 4. Call that reconstructed fixture coverage, not an original MultiBlow corpus occurrence. Later KEY loading may alter DTTYPE; the test should inspect immediately after prologue for this command-specific final state.

CP247's Forming successful-add order repair is separate; its native add/copy failure contract remains open evidence work. CP248 meshing editor work and CP249 operation/property/Close findings are not duplicated here. No global loader alias closure is claimed.

This report changes no source, tests, build, shared state, original process, GUI, or REA state; no tests were run.
