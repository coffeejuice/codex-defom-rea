# CP87 smallest Cogging/geometry integration without a mesh witness

## Decision

Do not change `0x140766200` or claim its public Cogging Save parity from the
current synthetic geometry-mesh tests. The native Cogging writer reads
`0x1411f5040(view, ..., false)`, so its scene contribution must already be in
the cached per-view box. Default display flag 43 is off. The positive optional
geometry-mesh helper is candidate-tested but does not by itself show that a
real Cogging project enables that display path or has native-ready arrays.

The smallest evidence-backed integration candidate is instead the bounded
geometry-list aggregation branch in `source/apps/moproj2mst/src/engine/viewer.cpp::recompute3D`,
native `0x1411f5540` flag `0x2b` route. Current source throws unconditionally
when flag `0x2b` is on and `view.items` is nonempty. The retained native body
refreshes the items (`0x1411f5a90`) and unions their item boxes
(`0x1411f58e0`) into the cached view box. The item rectangle helper and
unknown-readiness guard already exist.

## Narrow implementation/test shape for a separately authorized source task

In `recompute3D`, preserve the current order: reset the box; union enabled
geometry representation; when flag `0x2b` and items exist, refresh and union
items in list order; then union enabled FE mesh representation. Use each
existing `GeometryItem::rect` converted by the established rectangle-to-box
helper, and refuse before union when `rectKnown == false`. Do not bypass
`refreshGeometryItem`, fill unknown mesh data, or promote the independent
synthetic `OBJ_MESH2D` fixture into a native-loaded-mesh claim.

Focused tests belong in the existing
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` target
(`m2m.mdt.viewer_box`). Use the existing DOE display-option reader to set the
real candidate settings bit via `CrossSectionGeoOn`, if the test API exposes
the path, then cover:

1. An edge-only or empty item list with flag `0x2b=1`: item refresh and
   list-box union yield the expected view box.
2. A known populated synthetic quad: its already-qualified helper result is
   unioned in list order.
3. A positive-count unqualified mesh: refresh marks the item unknown and the
   flag branch preserves an explicit refusal, without changing the arrays or
   fabricating owner/registry state.

This is a deterministic candidate integration of a recovered list refresh and
union helper; it remains a preparatory internal branch, not evidence that a
real supported Cogging Save produces that state. Root should assign source
ownership and validate the focused target before considering the change.

## Remaining blocker for public Cogging parity

To claim a public behavior difference, still require one original same-session
witness tying the item-list-enabled refresh to the cached view queried by the
Cogging writer. At minimum record the viewer/view identities, flag `0x2b`,
item identity and optional mesh pointer/counts/getter readiness at refresh,
then compare the cached `view+0xd8` with the box returned by
`0x1411f5040(..., false)` at `0x140766200`. The retained native owner and
prepopulated-array path remains unresolved; an empty-list or edge-only test
does not settle it.

This is a source/evidence review only. No source, build, test, GUI or mutable
analysis database was touched.
