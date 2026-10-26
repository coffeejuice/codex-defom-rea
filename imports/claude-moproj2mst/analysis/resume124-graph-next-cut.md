# CP124 model-access/property-table next cut

## Decision

The next independently source-closed model-access path is already integrated: `finalizeStepDataPropertyTable` takes a short `GuiModelAccess::withStep` borrow for `step_current_object(nativeStep, true)`, then ends that borrow before iterating captured views or querying property-table cells. The current no-view, wrong-owner, kind-7, empty-cell, missing-step, and parked-model cases cover this boundary. I found no additional source-closed property-table or `viewer.cpp` native-step borrow that is ready for another production migration.

The next property-table expansion is populated checkbox-cell behavior. That remains blocked on the actual GUI table item's population/value/event contract; the current adapter intentionally refuses populated cells. Do not infer checkbox state transitions from the absent-item branch or change the shared `GuiModelAccess` interface to pursue it.

## Current source path

`source/apps/moproj2mst/src/engine/viewer.cpp:1125–1153` establishes the order:

1. Check the model geometry barrier and require `PropertyTableState` to belong to the exact `GuiModel`.
2. Snapshot the current `View*` order from `model.viewer` before current-object refresh.
3. Call `step_current_object(nativeStep, true)` inside a synchronous void `GuiModelAccess::withStep` callback. The helper checks the barrier before borrowing `model.step.get()`.
4. End the callback, then walk the snapshot and skip null/unbound/kind-7 views or missing settings.
5. For each eligible row, query columns 1–6. The only source-closed branch is absent `table.item(row,column)`; the present-item path throws before applying unknown value/check-state behavior.

The native pointer is used only within the callback and is neither returned nor retained. The table owner guard precedes snapshot/current-object effects. The later item loop does not reacquire the native step, so there is no next short borrow to add there.

`analysis/resume86-property-table-source-review.md` independently checked the same ordering and limitations. It records that out-of-range `u1b4c=999` becomes zero before the first unrecovered cell query is recorded, which proves refresh precedes that boundary. The current focused test `source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp` retains this case as `property_table_unrecovered_population_refuses_at_first_cell_after_refresh`, alongside:

- `property_table_unbound_views_skip_unrecovered_cells_after_current_refresh`;
- `property_table_kind7_bound_view_skips_unrecovered_cells`;
- `property_table_rows_count_only_eligible_views_and_empty_cells_keep_flags`;
- `property_table_foreign_owner_rejects_adapter_misuse_before_effects`;
- `property_table_missing_step_skips_refresh_without_creating_model_or_viewer`.

The parked composite barrier check remains in `tests/mdt/test_viewer_box.cpp`, asserting refusal before owner/query work and `queryCount()==0`. This is existing coverage; a duplicate reset/order test would not add an independent regression signal.

## Remaining public-model-access boundary

The direct native-step reads left in `viewer.cpp` are either (a) identity/provenance checks, (b) historical frame observations taken before parking, (c) `modelDim`'s nullable read-only diagnostic/classification helper, or (d) the already migrated query/current-object callsites. CP96 explicitly rejected wrapping const `modelDim` as a mutable borrow and identified no useful scalar-only migration after CP95. `viewerRebuild` obtains object pointers through `GuiModel::objectAt`; that method checks the pending barrier, then returns a raw `LLOBJ_DATA*`. Wrapping just that lookup in `withStep` would let an object pointer escape the callback, which `GuiModelAccess`'s callback result constraint cannot prevent if captured into an outer variable. Wrapping lookup plus `bind` would widen into viewer/geometry effects and requires a separate source/order review. It is not a safe CP125 one-liner.

## CP125 ownership recommendation

No production source edit is recommended for CP125 on this evidence. If root wants a strictly isolated verification addition, the only exclusive file needed is `source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp`; no shared header, interface, `viewer.cpp`, or CMake edit is needed. However, the current tests already exercise the source-closed path and its useful null/parked/owner/order boundaries, so the best bounded action is not to add redundant coverage.

To progress beyond the absent-item property-table branch, keep the implementation and test stopped until a qualified GUI/native witness establishes: actual table item identity and role, its initial and current check-state/text, which cells are populated, and whether item access emits/re-enters callbacks. A populated-cell implementation would then own `viewer.cpp` and a new focused test in `tests/mdt/test_viewer_property_table.cpp`; it must preserve current-object refresh before cell access, six-column row order, and existing parked/owner guards. Until that witness exists, the first populated cell refusal remains correct.

## Scope and uncertainty

This report uses current converter source, the CP86 independent property-table review, the CP96 access frontier, and the existing focused tests. It did not run a build/test or control a GUI. The missing GUI item witness concerns only the populated-cell expansion; it does not undermine the already integrated absent-item/current-object path.
