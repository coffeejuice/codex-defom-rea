# CP223: smallest owner-operation API increment

## Recommendation

Add one named, value-returning `GuiModel` operation for the viewer’s simulation-control geometry type, then route the existing viewer helper through it. Keep the method narrowly scoped to this one read and leave all legacy aliases and the cache1 refusal in place. This is a compiling migration increment, not ownership closure or new admission authority.

Suggested API: `int simGeometryTypeIf2D() const;` (name subject to root’s common-header convention). Implement the current behavior inside `GuiModel`: first perform the same `requireGeometrySyncAvailable()` check that `simCtl()` currently performs; return `0` if no step exists or the step dimension at `+0x1c` is not `2`; otherwise return the simulation-control geometry type at `+0x100`. Do not return or accept a `SimControl*`, a `DB_STEP_DATA*`, a reference, or a callback. The native reads stay within the model-owned method. Do not use `StepIdentity` as permission to read native storage: CP222 expressly limits it to an observation token.

## Exact consumer and source changes

All paths below are relative to `source/apps/moproj2mst/`.

- `src/engine/viewer.cpp:550-563`: `modelDim()` directly reads `m.step` at `+0x1c`; `simGeometryType()` obtains `m.simCtl()` and reads its `+0x100` field after checking dimension 2. Replace the `simGeometryType()` body with the new owner operation and remove `modelDim()` only if a fresh call-site search confirms no remaining uses. Current other `modelDim()` uses at lines 1181, 1751, 2100, and 2112 must remain untouched in this batch.
- `include/m2m/engine.hpp:198-206`: place the new value-only declaration alongside existing `simCtl()` / model operations. The public `step` at line 179 and all existing APIs remain for staged migration.
- `src/engine/gui_model.cpp:203-213`: `requireGeometrySyncAvailable()` and `simCtl()` establish the existing geometry-barrier refusal and null behavior. Implement the new operation adjacent to `simCtl()`, preserving the barrier check before its native reads and returning zero for missing/non-2D models.
- `src/engine/viewer.cpp:1766`: the actual call site uses the helper to select geometry behavior; its call and downstream branch remain unchanged.

The ordering matters: the current helper calls `simCtl()` first, so a geometry-parked model refuses before dimension is read. Preserve that ordering rather than returning zero for a non-2D model before checking the barrier. For an available model, preserve `0` for a missing control or non-2D dimension, and preserve the exact `+0x100` value for dimension 2.

## Focused test target

Extend `tests/mdt/test_viewer_box.cpp`, which already exercises the `GuiModel` barrier and alias behavior at lines 2698-2710 and 2818-2829. Add checks for: missing step returns `0` if constructible in the existing fixture; non-2D returns `0`; dimension 2 returns the configured `sim_ctrl` geometry value; and the new operation throws/refuses on the same parked geometry model as `simCtl()` (near the existing `pendingModel.simCtl()` refusal at line 2825). Use existing Testkit `CHECK`/`REQUIRE` conventions, not C `assert`.

The CMake test glob at `source/apps/moproj2mst/CMakeLists.txt` registers `tests/mdt/test_*.cpp`; expected focused target/name should be confirmed by root’s configure output, likely `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`. This report did not run a configure, build, or test.

## Behavioral and compatibility boundary

This moves the viewer’s geometry-type consumer off both raw aliases while keeping the same values and geometry-barrier refusal. It does not migrate `modelDim()`’s remaining callers, change callers of public `simCtl()`, modify model storage, invalidate aliases, add an active-borrow/parking system, or permit the R300 selected-length/cache1 continuation. CP163’s A-only publication and immediate `CoordinateSync` return stay unchanged. Keep all existing cache1 checks and the separate original-witness gate.

No C ABI change is needed: this is an internal C++ method on `GuiModel`; do not edit `include/moproj2mst.h`, C exports, layouts, version, or error mapping. This batch also makes no C++ binary ABI promise and cannot by itself prove external C++ source compatibility. Existing `simCtl()` stays available during staged migration.

## Basis and limits

CP222’s `analysis/resume222-editor-closure-architecture.md` requires named owner commands with value/key interfaces and warns that partial migration cannot establish closure. `analysis/resume222-editor-step-seam.md` likewise rejects a local wrapper as closure but identifies small compiling increments as staging only. `analysis/resume222-step-identity-implementation.md` and `analysis/resume222-step-identity-review.md` establish that the current token is observation-only; this proposal does not elevate it.

This is a source proposal only. I did not modify production or tests, build, run tests, execute the original, use GUI/app control, access mutable REA state, or change shared recovery state. The proposed numeric field reads follow the existing viewer helper; this report does not independently qualify their original runtime behavior. Root should confirm the layout/native twin and the focused target during integration.
