# CP86 property-table current-object access review

## Result

`finalizeStepDataPropertyTable` checks the model barrier and table ownership before snapshotting the current `View*` sequence. It then performs the `step_current_object(nativeStep, true)` operation inside a `GuiModelAccess::withStep` callback and discards the operation's result. The step pointer is not returned, captured, or used by the subsequent row/cell walk. The callback ends before any property-table item checks or query recording begin (`src/engine/viewer.cpp:1086-1119`).

This preserves the intended order: capture views first, reset/refresh current object while the step is available, then walk eligible views and inspect table cells. The out-of-range test sets `u1b4c = 999`, calls finalize, and verifies it becomes zero without a repick to object 1; when population is marked unrecovered, the first unsupported cell query happens afterward and increments `queryCount` to 1 (`tests/mdt/test_viewer_property_table.cpp:88-100`). This demonstrates the refresh precedes the query boundary.

## Edge cases and tests

- **No step / no viewer:** the function's snapshot is empty, `withStep` forwards null and the callback skips `step_current_object`; no viewer is created and no property queries occur (`test_viewer_property_table.cpp:112-124`).
- **No bound view / kind 7:** unbound views and kind-7 objects skip unsupported cells after the current-object call; the focused tests keep `queryCount == 0` (`36-61`).
- **Parked model:** the composite barrier test calls finalize with a marked table while the model is parked. It expects refusal before owner/query work and confirms query count/prefix remain zero (`test_viewer_box.cpp:1946-1950`). The access helper also checks before handing the step pointer to the callback.
- **Wrong owner:** the actual-model identity check refuses before the table is queried or either model gets a viewer (`test_viewer_property_table.cpp:102-109`).
- **No escaped borrow:** the `withStep` callback returns `void`; its only use of `nativeStep` is the synchronous current-object call. The helper's result constraint does not allow pointer/reference return.

## ABI/layout and limit

This is an internal callsite change only. It adds no `GuiModel` field and changes no exported API declaration or native `DB_STEP_DATA` layout. Existing source assertions cover `DB_STEP_DATA` size and C API function signatures in the viewer-box test; no binary export-table or `GuiModel` size check was run as part of this review. Public raw `step` and other model fields remain available, and previously acquired aliases remain outside the helper's revocation guarantee.

No blocker found. Review was read-only; no source/test edits, builds, or tests were run.
