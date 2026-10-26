# CP231 loader next leaf

## Recommended leaf

After CP228's unit command and CP230's planned furnace post-load command, migrate the distinct `furnaceLoadPrologue` step flag in `source/apps/moproj2mst/src/engine/loader_l2.cpp` (around lines 306–321). Its current `withOpStep` callback checks `s` and sets exactly `s->sim_ctrl.tnow3 = 1`. This is a real callback-delivered raw `DB_STEP_DATA*` with a narrow value mutation, suitable for a finite `GuiModel::setFurnaceLoadTnow3()` command. It is not the same effect as furnace post-load: prologue sets TNOW3; post-load enables/defaults furnace curves.

## Required call and failure ordering

Keep the surrounding sequence unchanged: `mst::baseLoadPrologue(c,p,op)`; `setTrans2(p,op,false)`; `setTrans1(p,op,true)`; then the step mutation.

`source/apps/moproj2mst/src/engine/engine_internal.hpp:60–76` documents and implements `withOpStep` as a synchronous `opModel` lookup followed by callback invocation with `model ? model->step.get() : nullptr`; it explicitly adds no geometry guard. The furnace callback null-checks before writing. Therefore the owner command must preserve the unguarded nullable semantics: if the operation model lookup yields no model/step, do nothing; otherwise set `tnow3 = 1`. Do not add `requireGeometrySyncAvailable()`, throw for null, create/replace the step, alter the two preceding trans calls, or move this operation into furnace post-load. No C ABI changes.

This is qualified from current wrapper source, not a runtime claim: `withOpStep` has no geometry guard and provides a nullable borrowed pointer. The command must keep the null no-op and avoid introducing a barrier refusal; no dependency on CP226 geometry-guard semantics remains.

## Files and tests

Production edit set: `source/apps/moproj2mst/include/m2m/engine.hpp`, `source/apps/moproj2mst/src/engine/gui_model.cpp`, and `source/apps/moproj2mst/src/engine/loader_l2.cpp`. The command should be a void value command; no model/control/step pointer escapes.

Use `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp`, specifically `heat1_as_furnace` (around line 278), as the existing regression: it asserts `sc->furnace_on == 1`, `sc->tnow3 == 1`, and furnace curves after the full load path. Add geometry refusal/state-prefix coverage only if another independent contract calls for it; it is not needed for this leaf, whose current wrapper path is unguarded. Retain the existing loader regression and add a focused owner test only if existing fixture support can exercise null-step no-op without introducing an artificial production seam. Confirm configured target/test name from metadata and include resolution before implementation.

## Evidence and limits

`analysis/resume222-loader-alias-ledger.md` inventories `loader_l2.cpp` callback-delivered step/control aliases and emphasizes preserving mutation order. Current `loader_l2.cpp` source establishes the assignment and its position after the base/trans setup; `engine_internal.hpp:60–76` establishes nullable, unguarded wrapper behavior. No original runtime capture or fresh original execution is claimed. No source, build, shared state, original, GUI, or REA state is changed here; no tests were run.
