# CP225 viewer tail-number value operation

## Implementation

Added `GuiModel::lastObjectNumber() const`. It first applies the same geometry
barrier check as `GuiModelAccess::withStep`, then returns the actual
`def::model::last_object_number(*step)` value, or zero when the step is absent.
The method returns only an integer and does not allocate or mutate model state.

`viewerSync` now reads the value after its existing geometry guard, null-object
shortcut, and viewer lookup. `viewerRebuild` retains its guarded missing-step
return and optional `viewerClear` order; its later tail-number read now uses the
model operation. Its initial boolean `withStep` check remains because it
preserves the missing-step early return before viewer lookup or clearing.
`appendObject` and all unrelated aliases remain unchanged.

Testkit additions check that sparse objects 1 and 4 produce last object number
4 even though `step_count_objects` is 2, that missing-step queries return zero,
and that a geometry-parked model refuses the new query. Existing viewerSync
null-object and viewerRebuild missing-step coverage remains in place.

## Validation and limits

`admin/check_new_cpp_bindings.py` passed for the four changed C++ files, and
`git diff --check` passed. No build or CTest was run, as directed; root owns the
grouped source-stable validation.

This is a staged scalar migration. It leaves C ABI, cache1 refusal, Stopping
logic, other raw aliases, and geometry continuation behavior unchanged. It
adds no independent original-runtime evidence and does not establish closure.
