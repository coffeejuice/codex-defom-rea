# CP225: next viewer owner-operation leaf

## Recommendation

Add `int GuiModel::lastObjectNumber() const` as a named scalar query and replace the two `GuiModelAccess::withStep` callbacks that read `def::model::last_object_number` in `viewer.cpp`. This is a finite native operation with an integer result, so the viewer no longer receives a step alias or supplies a generic callback. It preserves the current geometry refusal and null-step values if the method begins with `requireGeometrySyncAvailable()` and then returns `step ? def::model::last_object_number(*step) : 0`.

This is a staged source/API increment only. Do not change cache1 guards, Stopping behavior, geometry continuation predicates, or any `GeometrySyncBarrier` stage. Do not claim closure: viewer frames and several geometry paths still retain or borrow native aliases.

## Current call semantics

The current `GuiModelAccess::withStep` in `src/engine/gui_model_access.hpp:22-28` calls `model.requireGeometrySyncAvailable()` before invoking its callback with `model.step.get()`. Thus both existing reads refuse before native access on a geometry-parked model.

- `viewerSync` (`src/engine/viewer.cpp:1849-1868`) first calls `GeometrySyncAccess::requireAvailable`, then returns immediately for a null object, then obtains the viewer. At `1854-1857`, its nullable callback assigns `n = last_object_number(step)` when a step exists and leaves `n = 0` otherwise. The method must preserve that zero result when there is no step and retain the call after the null-object shortcut and viewer lookup.
- `viewerRebuild` (`src/engine/viewer.cpp:1871-1883`) first calls `GeometrySyncAccess::requireAvailable`; at `1874-1876` it returns before viewer lookup or clearing when the step is absent. After optional `viewerClear`, it reads the tail object number at `1880-1882`. Keep the initial null-step gate and optional-clear ordering as-is, then use the method for the integer read.
- `GuiModel::appendObject` (`src/engine/gui_model.cpp:314-323`) separately uses `last_object_number(*step) + 1` to choose the next object number. Leave it unchanged in this leaf; it is a writer with subsequent native object creation and caller/viewer effects, not the same read-only viewer operation.

Use the actual `last_object_number` helper, not `objectCount()`: the former reports the tail native object number and may differ from the number of list entries. Do not replace it with a maximum/ordinal approximation.

## Exact file ownership and API shape

Paths are relative to `source/apps/moproj2mst/`.

- `include/m2m/engine.hpp`: declare `int lastObjectNumber() const;` adjacent to the existing model scalar operations. Do not expose a native reference or step pointer.
- `src/engine/gui_model.cpp`: implement adjacent to the existing model accessors. The method must perform `requireGeometrySyncAvailable()` before checking null, then return `step ? def::model::last_object_number(*step) : 0`. Do not mutate the step or allocate anything.
- `src/engine/viewer.cpp:1854-1857` and `1880-1882`: replace only the two `withStep` reads with `n = m.lastObjectNumber()` (or the corresponding model name). Preserve the surrounding entry guards, nullable-object shortcut, initial missing-step rebuild return, optional clear, resize, and binding order.
- `tests/mdt/test_viewer_box.cpp`: extend the existing `viewer_sync_uses_tail_number_and_keeps_null_object_shortcut_before_allocation` case at lines 438-467 and/or `viewer_rebuild_preserves_normal_flags_and_missing_step_shortcut_before_clear` at 469-524.

## Focused regression requirements

In the existing sparse/tail viewer-sync test, verify that the new operation returns the same `last_object_number` value used to size the views after objects 1 and 4 are created (while `step_count_objects` remains 2). Verify a model with no step returns 0 from `lastObjectNumber()` and preserve the existing `viewerSync` no-step/no-viewer behavior. In the rebuild case, preserve coverage that an absent step returns before clear and does not create a viewer.

The geometry barrier regression already verifies `viewerSync`/`viewerRebuild` refusal. Keep those tests and add/retain a direct method refusal check against the geometry-parked model, demonstrating that `requireGeometrySyncAvailable()` still precedes the native read. Use Testkit `CHECK` and `REQUIRE`; no C `assert`.

The CMake glob for `tests/mdt/test_*.cpp` covers the file. The likely focused target is `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`; root must confirm the configured target. This report does not claim a configure, build, or test run.

## C ABI, cache1, and scope limits

This adds an internal C++ method only. Leave `include/moproj2mst.h`, exported C signatures/layouts, API version, flags, and error mapping untouched; do not claim C++ binary ABI compatibility. No cache1 code or store changes are part of this increment. The operation has no continuation or publication authority and cannot be used to justify the selected-length/cache1 path.

The operation is not a generic borrow wrapper: it returns a copied integer and the call sites need no callback. Still, the step remains public, `GuiModelAccess::withStep` remains, and other viewer code uses direct step fields, retained frame identities, and scoped multi-field borrows. The exact producer/consumer and geometry continuation families remain outside this leaf.

## Evidence and limits

CP222's `analysis/resume222-editor-closure-architecture.md` and `analysis/resume222-viewer-alias-ledger.md` describe why local scalar migrations are staging only. The latter catalogs persistent model, step, reader, frame, and slice aliases; this recommendation does not remove those. The current source directly establishes the callback's null and barrier behavior. The tail-number function is also used by `appendObject`; its semantics are described in `source/port/defmodel/include/defmodel/gui_twins.hpp` and `source/port/defmodel/tests/test_gui_twins.cpp` (tail value rather than object count).

Read-only scope: no production or test edits, build, executable, original process, GUI/app-control, mutable REA session, or shared recovery state were changed. No independent original runtime observation is added by this report.
