# CP88 flag-0x2b geometry-list box integration

Implemented the frozen CP88 viewer-list branch. `recompute3D` now takes the
current `GuiModel` so the branch can call the existing guarded
`refreshGeometryItem` for every item. With flag `0x2b` set and a nonempty list,
it refreshes and unions each known item rectangle as a Box3 between geometry
rep and FE mesh rep contributions. An item with `rectKnown == false` keeps the
existing `M2M_E_UNSUPPORTED` refusal. A pending geometry synchronization stops
the recomputation before using an incomplete item result.

Added three focused Testkit cases in the existing viewer-box file:

- two edge-only geometry-list items contribute their combined extents to the
  per-view box;
- a qualified populated synthetic 2D geometry mesh contributes with its
  surrounding edge rectangle;
- an unqualified positive-count mesh is refused at the flag-0x2b path before
  any getter dispatch, with its arrays and owner restored/unchanged.

The tests set the actual candidate setting through
`engine::readViewerDisplayOptions` and `CrossSectionGeoOn`, then observe the
per-view box through `engine::viewerBox`, matching Cogging's cached view-box
reader. These are candidate internal viewer tests only; no original project
mesh lifecycle or public Cogging Save parity is claimed.

Only assigned files changed:

- `source/apps/moproj2mst/src/engine/viewer.cpp`
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`

Source-only preflight: the viewer-box file contains 44 unique `TEST_CASE`s, a
Testkit `main`, zero C `assert` calls, and zero trailing-whitespace lines. The
new test code uses existing resolvable project headers and supported
`REQUIRE`/`CHECK` macros. Current file SHA-256 values:

- `viewer.cpp`: `b3415bc2f19e5210cd80b0767e7c096d042cd755cb2cc1b93f481feff2ff0e67`
- `test_viewer_box.cpp` after fixture correction: `03791540762d02624c30040920e15674871d44951287ec40322845f5cc2a0a61`

## CP88 fixture correction

Root's focused test run found all three new cases stopped at the helper's
visibility assertion. The source diagnosis is in
`source/apps/moproj2mst/src/doe/view_stepdata.cpp::readDisplayOptionFields`:
when a `DispFlagList` is present, non-mesh/non-geometry fields such as
`ObjectOn` are reset to zero unless explicitly supplied. The test input had
only `CrossSectionGeoOn='1'`, so it correctly disabled visibility before
`viewerRefresh`. Updated the helper XML to include both
`ObjectOn='1'` and `CrossSectionGeoOn='1'`. No production source change was
needed. Root owns rerunning the focused target; no test was run in this repair
turn.

No build or test was run; root owns validation. No CMake, public interface,
application, GUI, or mutable REA database was changed or used.
