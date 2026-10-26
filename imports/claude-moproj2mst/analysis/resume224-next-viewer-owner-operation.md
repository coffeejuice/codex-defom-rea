# CP224: next viewer owner-operation increment

## Recommendation

Add a value-only `GuiModel::dimension() const noexcept` operation and route the existing viewer `modelDim()` helper through it. Keep the operation a direct scalar observation with the current null behavior: return `0` when `step` is absent; otherwise return the native `dimension` field unchanged. Do not add `requireGeometrySyncAvailable()` or any other refusal. Existing `modelDim()` is deliberately an unguarded scalar read, and some callers can run while geometry work is parked; adding a barrier check would change supported behavior.

This is one owner-operation API increment. It moves every viewer caller of `modelDim()` off direct public `GuiModel::step` access while preserving the current helper and call graph. It does not privatize storage or establish full alias closure. Keep cache1 refusal and CP163's A-only publication plus immediate `CoordinateSync` return unchanged.

## Exact source ownership and call sites

Paths are relative to `source/apps/moproj2mst/`.

- `include/m2m/engine.hpp:198-206`: add `int dimension() const noexcept;` among the model's value queries. Keep the public `step` member, `simCtl()`, and `simGeometryTypeIf2D()` unchanged for staged migration.
- `src/engine/gui_model.cpp:209-215`: implement the method beside the existing model access operations. It must only read `step ? step->dimension : 0`; do not call `requireGeometrySyncAvailable()`, issue a token-based admission check, mutate state, or expose a pointer/reference.
- `src/engine/viewer.cpp:550-554`: change `modelDim(const GuiModel&)` to return `m.dimension()`. Keep the helper and its callers unchanged. The direct current read is `m.step ? *at<int32_t>(m.step.get(), 0x1c) : 0`.

The helper has four remaining call sites: geometry readiness at `viewer.cpp:1179`, viewer refresh at `1749`, `guiModelDim()` at `2098`, and viewer scene-box selection at `2110`. Re-run `rg -n "modelDim\\("` at implementation time to confirm the exact locations. Do not alter the separate raw step identity checks or retained `frame->step` aliases in this increment.

The `viewerSceneBox` path first obtains the viewer and then reads the dimension at `viewer.cpp:2106-2112`; it currently does not reject a parked geometry model. The owner method must preserve that. The readiness path at `1179-1180` also reads the scalar and then returns `Unqualified` unless it equals 3; keep this comparison and all surrounding COW/query ordering unchanged.

## Focused tests

Extend `tests/mdt/test_viewer_box.cpp`, which already has the relevant fixture and a focused geometry-barrier test.

- In `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass` (starts at line 2667), the fixture tests `guiModelDim(control) == 2` after `setDimension` at lines 2697-2714. Add direct `control.dimension()` equality coverage there for dimension 2 and, with the same fixture or another existing fixture, dimension 3 and a missing-step model returning 0.
- In the same test, while `model` is geometry-parked (`pendingModel` is established at line 2825), assert `pendingModel.dimension()` returns the current value and does not throw. Also assert the existing `engine::guiModelDim(pendingModel)` remains equal. This specifically prevents a future implementation from accidentally importing `simCtl()`'s geometry-barrier refusal into this scalar query. Keep all existing refusal checks for `simCtl`, object/material access, `simGeometryTypeIf2D`, and mutating entries.

Use Testkit `CHECK`/`REQUIRE`. The existing CMake `tests/mdt/test_*.cpp` glob supplies registration; root should confirm the concrete target/name from configured build metadata. Expected focused file/target is `tests/mdt/test_viewer_box.cpp` / likely `m2m_mdt_viewer_box` and `m2m.mdt.viewer_box`. No build or test was run for this report.

## Compatibility and refusal boundary

No C ABI file or export changes: leave `include/moproj2mst.h`, C declarations, layouts, API version, flags, and error mapping untouched. This is an internal C++ source API addition and does not establish C++ binary ABI compatibility. Keep the new operation read-only and value-only. In particular, do not implement it by returning a `DB_STEP_DATA*`, passing a pointer through a callback, or using `StepIdentity` as a native read grant.

No cache1 branch or Stopping logic changes. Existing scalar dimension reads continue to work while geometry is pending, exactly as before; calls which already go through `simCtl()` retain their existing geometry refusal. Do not change `simGeometryTypeIf2D()`'s geometry check or weaken any cache1 guard.

## Limits

CP222's seam/architecture reports make clear that a local owner operation is staging, not closure: public storage, mutable aliases, loaders, editors, viewer-retained pointers, and serializers still exist. This batch only removes one direct-step scalar access path in `viewer.cpp`; it does not claim that a parked model is generally inaccessible or that any original R300 continuation is qualified.

This recommendation is read-only. I did not edit source/tests, build, execute the original, use GUI/app control, access mutable REA state, or change shared recovery state. Native field equivalence is grounded in the existing viewer helper and should be confirmed against its source qualification during implementation; this report adds no independent original-runtime evidence.

