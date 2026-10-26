# CP80 geometry barrier audit

## Finding

The CP79 correction closes the `GuiModel` method-entry gap: `requireGeometrySyncAvailable()` is first in the mutating and mutable-capability methods `simCtl`, `beginPickingConstructionPrefix`, `reset`, `object`, `objectAt`, `objectWrap`, `appendObject`, `readKey`, `material`, `readMaterial`, `loadObjectKey`, `loadObjectKeyGeometry`, and `setPdie` (`gui_model.cpp:173-400`). Value-only `pdie` and `objectCount` do not mutate. The composite viewer test now explicitly checks the guarded methods at `test_viewer_box.cpp:1825-1852`, in addition to viewer sync/rebuild/refresh/clear/reset and scene/box rejection.

This is not a model-wide mutation barrier yet. Three concrete public/support routes still reach effects before any geometry barrier guard:

1. `readDbEntry(GuiModel&, idx, resetViewer)` (`db_load.cpp:389`) has no entry gate. When `resetViewer == false`, it skips `viewerClear`, writes `m.step->u1b58` at line 398, then runs the DB step reader at line 401 before its later `viewerRebuild` call reaches the guard. When `resetViewer == true`, `viewerClear` does stop the route before those writes. `readDbStep` at line 377 also has no gate and delegates to this path.
2. `deleteObject(GuiModel&, n)` (`cogging_save.cpp:253`) directly calls `step_delete_object` at line 261. It can finish without any guarded method if the deleted object is not the current PDIE. If it is the current PDIE, it calls `setPdie` only after deletion, so that guard throws after the object is already gone.
3. `displayShow` (`viewer_camera.cpp:401`) changes the display's main model and `m->display` without checking the barrier. `displayRefreshScene` calls it first, then `fit`; `fit` calls `viewerRefresh` at line 269 and throws on the pending model after the display association has changed. `displayShow` is also installed directly as the loader `showModel` callback (`class_slots.cpp:176`).

The loader callback `g_loaderSlots.setDimension` at `class_slots.cpp:181` writes through public `m.step` directly, with no guard. This is another concrete retained callback bypass.

## Aliases and public fields

The guarded accessors return mutable objects: `object()`/`objectAt()` return `LLOBJ_DATA*`, and `simCtl()` returns a mutable sim-control pointer. A caller may acquire any of those before the barrier is installed and keep using it afterward; installing the barrier cannot revoke an ordinary C++ pointer. `GuiModel::step`, `db`, `viewer`, table fields, and viewer mirror state remain public mutable members in `engine.hpp` (`GuiModel` declaration around lines 128-203). Direct writes and pointer replacement therefore remain possible without invoking a guarded member. The existing test itself keeps `firstObject` from before parking and confirms that `viewerSync` refuses when passed it, but it does not mutate through the retained pointer.

Consequently the current guarantee is bounded: known guarded entrypoints refuse at entry, and the barrier survives Viewer replacement. It does not make arbitrary retained aliases or other engine callbacks inert. Fully preventing that class of bypass requires encapsulating raw mutable fields/capabilities or defining a narrower supported-entry contract; a test cannot make the public raw fields inaccessible.

## Smallest next regression

Extend `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass` with the directly reproducible `readDbEntry` bypass. While the actual model is parked, invoke `readDbEntry(model, &index, false)` for the fixture's existing in-range step and require `M2M_E_UNSUPPORTED` before either `u1b58` or any `DB_STEP_DATA` bytes change. The current implementation will not satisfy that assertion; the required guard belongs at `readDbEntry` entry, before `modelDbReader`/step access. Then add a separate assertion for `deleteObject(model, n)` if a future patch claims all engine mutation callbacks are parked, since it currently changes object count before any guarded `setPdie` call.

A separate display test should be required only if `displayShow`/`displayRefreshScene` are included in the parked-model contract: snapshot `Display.main`, `GuiModel.display`, and view state, call `displayRefreshScene` while parked, and verify refusal happens before those links change.

No source files were changed and no builds, tests, GUI, or mutable REA actions were run during this audit.
