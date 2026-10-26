# CP224 viewer dimension value operation

## Implementation

Added `GuiModel::dimension() const noexcept`, returning `step->dimension` when
the step exists and `0` otherwise. It performs no geometry-barrier check, so
the existing scalar query remains available while geometry work is parked.
The field is the same native dimension at `+0x1c` read by the prior viewer
helper; `DB_WRITE_VAR::dimension` is documented at that offset in
`source/port/defmodel/include/defmodel/layout/db_write_var.hpp`.

The viewer's `modelDim()` now delegates to `GuiModel::dimension()`. Its four
call sites and surrounding control flow remain unchanged. Testkit coverage in
`test_viewer_box.cpp` checks dimensions 3 and 2, a missing step returning zero,
and a parked model returning its current scalar through both the owner operation
and `guiModelDim()`.

No C API, layout, error mapping, geometry barrier policy, or cache1/CP163 path
was changed. Public `step` and other legacy aliases remain for staged migration.

## Validation and limits

CP223 is frozen at `build/wave3-checkpoint223-verified/`; its validation records
367 registered tests, 360 clean passes, six passes with logged gaps, one pass
with logged differences, and zero failures. That is prior-checkpoint evidence,
not CP224 validation.

`admin/check_new_cpp_bindings.py` passed for the four changed C++ files, and
`git diff --check` passed. Root requested no build or CTest until both CP224
source owners release their files; accordingly, no CP224 build or test result
is claimed. Root owns grouped focused/full validation and freeze.

This increment moves one viewer scalar read behind an owner method. It does not
close raw aliases or establish general parked-model access closure, and it adds
no new original-runtime evidence.
