# CP121 audit addendum: DOE special-operation preflight

No mismatch found between CP120 and the current `src/engine/save_project.cpp` implementation.

`mustBeLast` matches exactly `SfOprShapeOptim`, `SfOprDoeOutput`, and `SfOprReport`. `lastOperationCheck` scans each sub-project's `flowChain` up to, but excluding, its final item. On a match it records the pending modal/default-Yes facts and refuses because modal delivery/result is unrecovered. `saveCurrent` calls this for each nonnull sub-project before selecting the active flow item or invoking `saveCurrentOperation`.

This is already the bounded source-closed private contract described in CP120; adding a second helper would duplicate the existing classifier and scan. The existing guard remains necessary. The separate earlier Open/load prerequisites for active Report and type-3 ShapeOptim, and the modal answer/continuation dependency for non-last items, remain unchanged.

Read-only reconciliation of CP120 with the current source. No files outside this addendum were changed; no build or tests were run.
