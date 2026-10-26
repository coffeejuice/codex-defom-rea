# CP200 RGRAPH source-readiness audit

## Finding

No additional RGRAPH operation-graph or property-dispatch behavior is source-ready from the retained evidence. The smallest exact missing original witness is one normal-return, same-owner/same-`SfObjectManager` lifecycle occurrence joining the selected population activation (or the concrete mode-2 delegate) and its clear/rebuild to the later StepData finalizer. It must retain the manager allocation and mode, each inserted item's root/child coordinates, and the finalizer's direct top-level `item(row, 1..6)` return values for the eligible View/Settings rows. If any queried item is checkable, retain its row/selector mapping and delivered callback order relative to the setting write; if all are null, retain all six null results for each relevant row. This is the existing CP171 blocker stated at the narrowest evidence boundary, not a claim that the original has no such path.

## Exact evidence and current implementation

The original evidence subject is `evidence/original-binaries/DEF_GUI_MO_64.exe`, DEF_GUI_MO v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- CP159 `analysis/resume159-graph-decode.md` proves the mode-1 updater clears the manager, inserts a group beneath the invisible root, then appends object rows beneath that group, with checkable items and metadata roles. CP159 callback evidence (`analysis/resume159-graph-callback.md`) identifies the callback receiver and role path but does not establish when a callback is delivered in the lifecycle.
- CP160 `analysis/resume160-graph-widget-next.md` binds the same manager to the StepData dispatcher/finalizer, View/Settings lookup and direct `QStandardItemModel::item(row,column)` calls. It explicitly distinguishes hierarchical group children from those top-level queries. Its captured activation site `0x140cdfbd0:0x140cdfe77..0x140cdfe9a` is not yet joined to finalization for the same selected owner and manager generation. CP164 and CP171 retain this exact unresolved join; neither licenses another broad writer/callback decode.
- Current `source/apps/moproj2mst/src/engine/viewer.cpp:1921` checks actual-model ownership, performs the current-object refresh, and only accepts absent top-level cells; populated cells still refuse. `tests/mdt/test_viewer_property_table.cpp` already covers absent-cell rows, kind-7 exclusion, refresh ordering, foreign ownership and missing-step behavior. Those projections do not establish the missing original occurrence.

This behavior is publicly relevant because the native finalizer runs in the StepData processing path used by operation Save; it determines whether per-view settings are updated from manager cells. The retained static fragments establish the endpoints and local behavior, but not one accepted lifecycle connecting them, so changing the public projection would risk inventing a cell result or callback timing.

## Prospective source/test seam and focused test

If the witness proves one concrete outcome, the narrow prospective pair is `source/apps/moproj2mst/src/engine/viewer.cpp` and `source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp`. They are currently not released for a behavior change. `viewer.cpp` is also shared with the independent RGEOM queue, so file exclusivity must be rechecked before assigning concurrent work.

The focused test should build the exact witnessed manager generation, invoke the production finalizer, and assert queried coordinates, resulting per-View selector state, and callback/finalizer order against the captured outcome. For an all-null witness it should assert the complete six-column null row and unchanged flags; for a positive cell witness it should assert only the witnessed selector writes and callback sequence. Do not add either fixture until the original witness supplies those literal expectations.

Only this analysis note was written. No source, test, fixture, build, GUI, mutable REA state, or original process was changed or run.
