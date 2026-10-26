# CP274 RGRAPH next-leaf readiness scan

## Decision

No source-ready public or internal owner leaf was found. Keep the populated-cell refusal in `source/apps/moproj2mst/src/engine/viewer.cpp` and do not extend `tests/mdt/test_viewer_property_table.cpp` with an inferred native result. The queue's exact blocker remains one normal-return original occurrence joining population and StepData finalization for the same selected owner and exact `SfObjectManager` allocation/generation.

## Reconciliation

The current `analysis/takeover-state.json` RGRAPH entry is `ready` but partial/incomplete; its `next_action` carries forward the CP200 same-manager-generation witness requirement. CP250 (`analysis/resume250-connector-graph-next.md`) and CP243 (`analysis/resume243-graph-next-action.md`) independently reaffirm that boundary. CP214 and CP208 reach the same conclusion. No later graph evidence in the queue supplies the missing joined occurrence.

Retained original-binary evidence closes useful local pieces, not the lifecycle join:

- CP159 decodes the mode-1 hierarchical writer and derived callback/role behavior. The callback report explicitly leaves owner/widget generation, row mapping and delivery timing unresolved.
- CP160 binds manager ownership, View/Settings selection, and the StepData finalizer's direct top-level `item(row,column)` queries. The writer's group children do not establish values at those queried coordinates.
- CP171 and CP200 identify the same missing transition. Static activation at `0x140cdfbd0:0x140cdfe77..0x140cdfe9a` is not joined to normal-return finalization at `0x140c90bd0 -> 0x140d7c260` for the same owner and manager allocation.
- CP203 rules out using CP201's unrelated inert QtCore TLS edge descriptions as graph evidence. CP243/250 likewise do not contribute an occurrence.

The current production seam is concrete: `finalizeStepDataPropertyTable` snapshots View pointers, refreshes the current object, admits eligible View/Settings rows, then accepts only absent cells in columns 1–6. Its focused test already exercises empty eligible rows, kind-7 exclusion, refresh ordering, refusal on the first populated cell, foreign ownership, and missing-step behavior. Those are adapter/constructed-state guarantees; they cannot decide what the original manager returned after a real population lifecycle. No smaller internal leaf has same-occurrence evidence plus an independent test seam either: the setter/callback effects require the unresolved queried cell, row identity, and callback order.

## Exact missing witness

In one original DEF_GUI_MO v11.0.2 occurrence (PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), bind the selected owner and exact manager allocation from the activation at `0x140cdfbd0:0x140cdfe77..0x140cdfe9a` through its mode and clear/rebuild to normal return from the StepData finalizer at `0x140c90bd0 -> 0x140d7c260`. Retain inserted item root/child coordinates, eligible View/Settings rows, and the direct top-level `item(row,1..6)` result for each relevant row. If any result is checkable, bind its selector and callback delivery order relative to the setting write; if all are null, retain all six null results for each relevant row. Follow the mode-2 delegate only if this same occurrence selects mode 2.

Only that witness can release the narrow `viewer.cpp` / `test_viewer_property_table.cpp` behavior change: assert either the captured selector write and callback sequence, or the complete all-null row outcome. Before assigning that later work, recheck exclusive `viewer.cpp` ownership against RGEOM. This is a specific missing-witness result, not evidence that the original has no populated path.

## Scope

Read-only scan of the converter queue, CP159/160/171/200/203/208/214/243/250 reports, current production seam and focused tests. No source, tests, fixtures, build, GUI/original process, mutable REA state, or shared plan/state was changed. This report is the only file written.
