# CP242 loader next reachable ownership leaf

## Candidate

Move the final HeatTransferExpress post-load control read/write in `source/apps/moproj2mst/src/engine/loader_l3.cpp::htePostLoad` into a finite `GuiModel` command such as `setHeatExpressPrimaryObject()`. This remains live after CP237; unlike the retired CP238 helper, it has active production dispatch and existing caller-level fixtures.

The current sequence after three earlier effects is: `expressCopyPrevWizard(p, op)`; `expressBasePostLoad(p, op)`; `setTrans1(p, op, true)`; look up model, return if missing; call `m->simCtl()`, return if missing; read `*(int32_t*)(sc+0x240)` with unused result; then set `sc->pdie1 = (m->objectCount() == 1 ? 1 : 2)`. Replace only the final two native-control operations with the owner command, preserving position after the earlier functions and `setTrans1`.

## Command semantics and guards

The command should call `simCtl()` first, so the existing geometry barrier check still occurs before reading/writing control state. If there is no sim control, return without mutation; the call site may keep its current `if (!sc) return` guard or the migration can replace that exact guard with an explicit command result, but must not make earlier operations conditional differently. Preserve the otherwise-unused `+0x240` read before the PDIE assignment inside the owner command; do not discard it as an optimization without reviewing whether it is part of the recovered call sequence. Compute the branch from the current `objectCount()` at that point: exactly one gives PDIE 1, every other count gives PDIE 2. Do not use slot count or object type count. Do not expose or return `Cls_1401e7b10*`.

The current `simCtl()` access calls `requireGeometrySyncAvailable()` and then returns nullable control. Preserve guard-before-null and no write on null. There is no additional null-step behavior to infer beyond `simCtl()`; it returns null without a step. Do not add a separate mutation before the owner call.

## Test seam and qualification

Existing caller-level tests in `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` reach HeatTransferExpress post-load and assert PDIE values: `air_convect_2d_active` has one object and expects 1; `heat_express_3d` has one object and expects 1; `heat_resting_2d` has two objects and expects 2. Together AirConvect/HeatExpress (one object) and HeatResting (two objects) provide the exact current `objectCount() == 1 ? 1 : 2` branch coverage. The HeatExpress fixture establishes the integrated type-0 route with PDIE 1; it is not the two-object case. Retain and run these in the focused `m2m.mdt.engine_l3` target. Add a direct owner test only if needed to exercise an absent control/geometry parked refusal; prefer the existing geometry-barrier fixture for refusal if it can reach this command without altering earlier call order. No original execution is implied by these converter tests.

Original source qualification: the callback annotation identifies `0x1413e63a0` (3D) and `0x1413e5ae0` (2D); `source/docs/re/moproj2mst/mdt/L3/spec.md` §3 and `ledger.tsv` record the post-load PDIE 1/2 behavior as recovered/ported. This is decompilation/source evidence, not fresh original dynamic capture. Do not claim parity beyond the retained fixtures and qualified oracle records.

Production ownership: `source/apps/moproj2mst/include/m2m/engine.hpp`, `source/apps/moproj2mst/src/engine/gui_model.cpp`, and `source/apps/moproj2mst/src/engine/loader_l3.cpp`; tests stay in existing `test_engine_l3.cpp`. No C ABI changes. No source, test, build, shared state, original, GUI, or REA artifacts were changed; no tests were run.

