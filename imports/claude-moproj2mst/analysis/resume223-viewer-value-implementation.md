# CP223 viewer geometry value operation

## Implementation

Added `GuiModel::simGeometryTypeIf2D() const` as a named scalar read. It first
calls the same `requireGeometrySyncAvailable()` barrier used by `simCtl()`, then
returns zero for a missing step or a dimension other than 2. For dimension 2 it
returns the existing simulation-control integer at offset `+0x100`.

The viewer's existing `simGeometryType()` helper now delegates to the model
operation. The viewer call site and branch remain unchanged. Other `modelDim()`
callers and all legacy raw aliases remain untouched. No C ABI, cache1 refusal,
layout, or error mapping was changed.

Focused Testkit assertions cover the existing 3D model returning zero, a 2D
step returning a configured value of 37, missing-step zero, and refusal on the
same parked model used for the `simCtl()` barrier case. The parked refusal
confirms the operation checks the barrier before reading dimension/storage.

## Validation and limits

`admin/check_new_cpp_bindings.py` passed on all four changed C++ files. This
checks Testkit entry point, include resolution, and known assertion issues; it
does not compile or execute the code. `git diff --check` passed.

The root requested that no build or CTest run while another source owner is
active. A focused `admin/build_candidate.py` invocation had been started before
that instruction arrived. I interrupted it immediately on receipt. CMake
configure had succeeded; the build log stopped at Ninja step 39/79. The
processes were verified absent afterward. There is no completed build receipt,
and no focused CTest was run. Root owns the grouped focused build, full suite,
and CP223 freeze.

This is only a compiling migration increment. It does not close other native
aliases, change geometry-barrier policy, or qualify original runtime behavior.
