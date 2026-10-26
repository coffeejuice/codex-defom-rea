# CP131 Cogging geometry-list next branch

## Finding

CP88's `recompute3D` change implements the native flag-`0x2b` list-box stage
in the qualified order: refresh every geometry-list item, stop on pending
geometry sync, validate the complete refreshed list, union every known
rectangle, then continue to the FE mesh-representation stage. The later-item
failure test specifically protects against committing an earlier item box
before discovering an unknown later rectangle. The checkpoint88 grouped
validation receipt records 245 passed, six passed with logged gaps, one
passed with logged differences, and zero failures across 252 tests. This is
candidate internal viewer integration, not a populated native mesh lifecycle
or Cogging Save witness.

I found no remaining source-closed production scalar step to add. The current
unknown-owner/array guards should stay. A small candidate-only test can
exercise the existing `GEO2D_AND_MESH2D::copy_from` path before the flag-43
refresh, but the missing acceptance fact is still one original 3D
geometry-list item whose optional mesh is populated and reaches the cached
box read used by Cogging.

## Reconciliation

The native and candidate sequence is recorded in
`analysis/resume88-cogging-list-order-correction.md` and
`analysis/resume88-cogging-list-box-implementation.md`; the result is included
in `analysis/wave3-checkpoint88-validation.json`. The implementation keeps
the native full-list refresh-before-aggregate ordering and preserves explicit
refusal if any refreshed `rectKnown` value is false. It does not bypass
`refreshGeometryItem`, manufacture a node owner, or treat a positive count
alone as sufficient for a mesh rectangle.

The consumer and order evidence remains:

- Native Cogging writer `0x140766200` asks `0x1411f5040(scene, outBox,
  recompute=false)` for a cached box. Its body does not refresh or recompute
  that view at the call site (`analysis/resume81-cogging-mesh-pe.md`).
- In the Save operation hook, the three Cogging geometry calls run before
  positioning and manipulator-state synchronization. Those later calls do
  not refresh the box before Cogging extracts its geometry
  (`analysis/resume81-cogging-refresh-save-order.md`). The cache was therefore
  established by an earlier bind/refresh or other caller; its same-view
  freshness in a supported session is not captured.
- Display flag `0x2b` is off on the ordinary/default path. The retained
  corpus scan has no confirmed fixture with the optional
  `GEO2D_AND_MESH2D::mesh` populated on a 3D viewer item.

The mesh owner and readiness chain are narrowed by
`analysis/resume53-geometry3d-ownership.md` and
`analysis/resume56-geometry3d-node-owner.md`. The geometry cross-section
owns the optional mesh pointer and its copy path copies local node/element
arrays. Its `OBJ_MESH2D` constructor/reset leaves the nested node-set owner
null; no geometry-mesh-specific owner setter was located. Native getters
can return preallocated coordinate/connectivity arrays on that null-owner
route, but missing coordinates take an owner-derived dimension path that is
not safe to infer. Thus only the already-populated-array branch has a
source-qualified static path; a real supported project supplying those
arrays has not been shown.

## Smallest candidate-only extension

If more internal coverage is useful, add one focused case to the existing
viewer-box test that starts with a valid populated source cross-section,
copies it through `GEO2D_AND_MESH2D::copy_from` into the bound geometry-list
item, verifies the copied mesh arrays and retained null nested owner, then
enables `CrossSectionGeoOn` and runs the normal refresh/box query. This uses
the already-source-defined copy, refresh, and union functions without adding
production behavior. Label it as candidate copy-path coverage only; it does
not show that the original 3D loader creates/copies that state or that a
Cogging Save consumes it.

## Exact missing original occurrence

The smallest decisive artifact is one retained immutable 3D project/object
capture, or a same-session original runtime witness, recorded before
`0x1411f5540` refresh. It must bind the same view/model and show:

1. display flag `0x2b` is enabled and the item list is nonempty;
2. the item's optional mesh pointer at cross-section `+0x28` is nonnull;
3. node and element counts are positive, with preallocated coordinate and
   connectivity pointers present before getters run (or a separately
   qualified owner/dimension allocation path);
4. the original item refresh accepts that mesh and the resulting item
   rectangle enters the complete list union; and
5. the same view identity and cached box are later passed to Cogging's
   `0x1411f5040(..., false)` call in the Save hook.

An empty item list, edge-only item, positive-count but unpopulated mesh,
synthetic candidate quad, or unrelated main FE mesh does not satisfy this
occurrence. Until such evidence exists, CP88 is a recovered cached-box helper
branch with conservative guards; it is not evidence of a public Cogging
output mismatch or a reason to relax the mesh readiness checks.

Read-only source/evidence review only. No GUI, mutable REA, source, build, or
test action occurred.
