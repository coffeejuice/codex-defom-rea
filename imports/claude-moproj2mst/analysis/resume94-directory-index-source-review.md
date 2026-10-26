# CP94 independent review: signed directory-index borrow

**Result: no source blocker found.** The `u740` migration preserves the signed directory-index checks and keeps the native step pointer within a synchronous callback.

## Review evidence

In `source/apps/moproj2mst/src/engine/viewer.cpp`, the qualified path first requires a live `model->step` and verifies that the geometry node/element identity matches that step (lines 553–566). Before the new borrow it pins `currentProject()->dataDb`, checks that the handle matches `model->db`, obtains the reader, and checks reader/step identity through an earlier `withStep` call (596–606). It then obtains the current allocation receipt (608), copies only `nativeStep->u740` into local `std::int32_t index` (609–612), and applies the original guard in this order: receipt present, signed `index < 0`, cast-to-size_t extent check, and receipt-current check (613–616). Record access and query/frame logic follow those checks (617 onward).

`GuiModelAccess::withStep` in `gui_model_access.hpp` calls `requireGeometrySyncAvailable()` before passing `model.step.get()` to the callback. It returns the callback's declared result type and does not introduce a lease or fallback. Here the callback is void, captures only `&index`, reads one scalar, and ends before the value is consumed. The earlier live-step qualification makes that pointer non-null on this path. No native pointer or reference escapes, and there is no write to native/model state.

The existing `geometry_nonnull_directory_query_bounds_use_actual_extent_not_logical_count` case covers `-1`, physical extent 32, `INT32_MAX`, and valid index 31 despite logical count 1. Owner/receipt and reload/multipart cases cover missing or foreign ownership and allocation-generation invalidation. Those cases exercise the signed and physical-extent contract without needing a new test for the scalar-access wrapper.

## Conclusion

I found no change to owner/reader qualification, receipt-to-index ordering, signedness, physical allocation bounds, query/frame ordering, or public/native layout. The implementation report records final `viewer.cpp` SHA-256 `aee911933a4d05fb4dbf7ddb98f7c5dde6147fdd500e1434a9ad6e5baa286b49`; the current file hash matches. Root owns the reported focused and grouped validation; I did not run a build or tests.
