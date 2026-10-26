# CP131 RGRAPH queue reconciliation

## Queue correction

`analysis/takeover-state.json` still says RGRAPH next action is “CP85 viewerSync count borrow migrated; CP86 property-table current-object refresh under implementation.” That entry is stale. CP86 property-table current-object access is complete and independently reviewed; subsequent actual step accesses through the geometry viewer path were migrated through CP97. `python admin/check_continuation.py` reports the four active remaining streams R300/RDOE/RBOOL/RIO, not an outstanding CP86 implementation. The RGRAPH record should be updated to describe completed borrows and its remaining ownership/legacy exposure boundary; this report does not edit the shared queue file.

## Integrated source and receipts

The current `source/apps/moproj2mst/src/engine/viewer.cpp` hash is `e2287cac017f17035e7c6f172d6f2765300a7c56ff0207b489e7b07142cdbede`, matching the CP97 frozen receipt. `gui_model_access.hpp` remains `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`; it gates before lending the actual `model.step.get()` and constrains callback result to void or bool.

The following source-qualified calls are no longer candidates for implementation:

- CP85 `viewerSync` object-count read: short `withStep` scalar copy; viewer resizing follows the callback.
- CP86 `finalizeStepDataPropertyTable`: snapshot view order, borrow for `step_current_object(nativeStep, true)`, end borrow, then inspect/query table cells. The independent review `analysis/resume86-property-table-source-review.md` found the owner guard, null-step behavior and refresh-before-query order correct.
- CP89–94 readiness path: qualified readers, signed directory index and other scalar reads are short synchronous borrows with gates before the borrow. The CP94 review confirms signed bounds and receipt/index order.
- CP95 high-record path: the already-closed `step_dir_sizes` query uses a void callback with the same seven native arguments and the strong DB owner in scope.
- CP97 low-record initial path: one prepared callback is acquired before installation and contains install → same native query → postVersion/stage. The CP97 review bounds it to a synchronous callback-free interval, keeps the frame pointer historical, and preserves later barrier refusals.

Thus CP86 did not remain under implementation: it is a completed callsite, and CP97 is the latest completed viewer access integration. Current RGRAPH status should reflect integrated borrows with remaining public raw fields/aliases, not a pending CP86 patch.

## Remaining graph boundary and exact witness

No additional source-closed viewer borrow is ready. `GuiModelAccess::withStep` cannot return an object pointer/reference; a borrow around `GuiModel::objectAt` that copies its `LLOBJ_DATA*` into an outer local would defeat the non-escape contract. Widening the callback to include `bind`/viewer geometry work would cross a broader effect boundary and needs separate original-order/callback qualification. CP96 already rejects low-value scalar-only churn and any attempt to treat historical frame pointers as allocation leases.

For property-table behavior, the existing adapter supports only absent cells and deliberately refuses a present checkbox item before value/check-state effects. The exact next witness is a same-widget/property-table occurrence that binds:

1. Actual `QTableWidget` identity and its owner/generation within the live model/window.
2. The eligible row-to-View order and the present/absent status for columns 1–6.
3. For a present item, item type/role, initial check-state/text, and any item access or state write signal/event callback behavior.
4. Normal return of the corresponding refresh/finalize path, with current-object refresh remaining before item inspection.

Until those fields are recovered, there is no source-safe populated-cell branch; preserve the current first-populated-cell refusal. This property-table witness is distinct from general public `GuiModel::step`/viewer aliases, which require a full caller inventory and a staged encapsulation design rather than a local callback wrap.

## Recommendation

Mark the RGRAPH CP86 work integrated and update its next action in the shared state when root next edits the queue. No source or test edit is justified by this reconciliation. If a future original witness closes populated property-cell semantics, the exclusive implementation should be `viewer.cpp` plus a focused `tests/mdt/test_viewer_property_table.cpp` case; no `GuiModelAccess` interface change is implied by this report. If the next work instead targets raw object pointer access, first recover a scope that consumes the object pointer entirely within the borrow without changing native bind/geometry callback order.

## Scope

Read-only comparison of the RGRAPH queue entry, CP85–97 receipts, current viewer/access source and property-table tests. No source, build, GUI, original execution or mutable REA state changed.
