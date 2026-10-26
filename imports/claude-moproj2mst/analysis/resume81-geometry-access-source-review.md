# CP81 `GuiModelAccess::withStep` source review

## Result

The first migration seam meets the CP80 design's key constraints for a synchronous access helper. `withStep` checks `model.requireGeometrySyncAvailable()` before fetching `model.step.get()` or invoking the callback (`src/engine/gui_model_access.hpp:20-27`). Its callback constraint admits only `void` or `bool` results, so the helper cannot directly return a step pointer or reference. The focused test has compile-time rejects for pointer, reference, and `bool&` results (`tests/mdt/test_viewer_box.cpp:61-77`) and verifies the callback is not invoked for a pending model (`1930-1935`).

`GuiModelAccess` is forward-declared and friended; it adds no instance fields. The `GuiModel` member declarations and order are unchanged in this patch, including `step`'s `unique_ptr` type. The ABI checks in the test retain `DB_STEP_DATA` size `0x3408`, assert the current `step` member type, and check the existing exported C function signatures and API version (`test_viewer_box.cpp:79-91`). The inspected shared ABI remains the `extern "C"` `M2M_API` surface in `src/dll/abi.cpp` / `include/moproj2mst.h`; this internal header adds no export. These are source-level ABI/layout checks, not a binary export-table comparison or a `sizeof(GuiModel)` comparison.

## Caller and nonpending behavior

The sole production caller found is the `g_loaderSlots.setDimension` lambda (`class_slots.cpp:181-187`). It uses the helper to make the same native `set_dimension` call with the same dimension value. The callback is invoked once on the nonpending control model, and the test confirms both callback execution and the dimension change. As before, it assumes `m.step` is non-null; the helper deliberately forwards null unchanged rather than inventing a model or owner. Thus the migration preserves the caller's nullable semantics, although a null step still fails at the existing dereference just as the prior direct-field implementation did.

## Limit and follow-up

The helper prevents pointer/reference escape through its *return type*, but C++ cannot prevent a callback from storing the borrowed pointer in an external variable, global, or captured object. The header comment explicitly leaves that capture/alias escape to review. The current caller does not capture or return the pointer, so this use is bounded. Keep the callback constrained to known synchronous native operations and avoid exposing `withStep` as a general-purpose public API. The `step` field remains public, so this one seam does not close direct raw-field paths or aliases from other accessors; it is a staged migration only.

No blocker found for this first caller. A later hardening step could add a compile-time rule/source allowlist for `withStep` callsites and a test that the callback cannot be invoked after parking (already covered). A binary export comparison can be run with the project verification gate when the parent authorizes builds; it was not performed here. No source changes, builds, or tests were made during this review.
