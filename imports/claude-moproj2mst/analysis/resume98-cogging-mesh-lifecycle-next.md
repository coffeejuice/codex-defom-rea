# CP98 Cogging geometry-list mesh lifecycle boundary

## Current CP88 result

The flag-43/`0x2b` branch now matches the retained native sequencing:
`recompute3D` refreshes every item, checks for a pending sync before item-box
aggregation, validates the full item list, then unions the known item boxes
between geometry-rep and FE-mesh-rep processing. CP88's later-unknown-item
regression proves no earlier item rectangle is committed to the view box when
the list cannot be fully accepted. Checkpoint 88 records 252 registered tests,
245 passed, six logged-gap passes, one logged-difference pass, and zero failed
tests; the test qualification still does not constitute fresh-original
acceptance.

Current `refreshGeometryItem` and `qualifiedGeometryReadiness` preserve the
meaningful mesh guards: positive counts are not enough; the canonical node
registry and preallocated coordinate pointer must qualify; any nonnull node or
element owner must match the actual view model's step; and positive malformed
or unqualified mesh contributions leave `rectKnown=false`, which the 2b branch
refuses before aggregating any list item. For admitted arrays,
`geometryMeshRect` still checks the recovered 2D set layout, width, pointer and
product bounds, one-based connectivity range, and finite coordinates. No owner
is manufactured.

## Remaining public reachability boundary

Native Cogging Save `0x140766200` queries the per-view `0x1411f5040` box with
`recompute=false`; this branch can affect that cached view box only if a prior
refresh had the geometry-list display flag enabled. Retained source says
flag 43 defaults off on the normal path; DOE StepData imports the
`CrossSectionGeoOn` field at `source/apps/moproj2mst/src/doe/view_stepdata.cpp`.
There remains no retained original 3D Cogging session showing that the flag was
on, or that a matching view had a positive populated optional mesh at refresh.
The retained corpus/oracle survey found no such project fixture.

Ownership evidence separates the outer container from the nested node-set
owner. `GEO2D_AND_MESH2D` owns its optional `OBJ_MESH2D*` at `+0x28`. The mesh
constructor/reset leaves its embedded node-set owner `s008+0x130` null and
sets its local dimension flags. `OBJ_MESH2D::copy_from` copies coordinates and
connectivity using those local flags but does not call the LLOBJ node-owner
setter. Retained original evidence supports getters returning preexisting
arrays on this null-owner route. It does not establish that an actual supported
3D project supplies those arrays before `0x1408ee940`; an absent coordinate
pointer still reaches the owner-derived dimension allocation path, which is
not safe to infer from the null-owner case.

## Smallest source-bound increment

No production owner write or refusal relaxation is justified. The narrowest
candidate-only advance, if more port coverage is desired, is one test-only
extension of `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: create a
populated source `GEO2D_AND_MESH2D`, copy it through the actual
`GEO2D_AND_MESH2D::copy_from` path into a geometry-list item bound to a
`GuiModel` view, verify the copied mesh arrays and null `s008+0x130` owner, then
run the enabled flag-43 refresh and assert the per-view box. This exercises the
existing outer object/view binding plus the repository's copy path, without
inventing a DB owner. Label it a candidate unit test only: it would not prove
that an original 3D Open path invokes this copy path or that a real project
contains such a mesh.

The smallest action that advances original behavior is a retained immutable
3D project/object capture or allowed original runtime witness recording,
before the viewer refresh: object and cross-section identities, optional mesh
pointer, node/element counts, coordinate/connectivity pointer presence, node
owner and actual view setting flag 43. Then bind that same view to the
`0x140766200` cached-box query. Until one exists, retain current guards and
make no public Cogging parity claim. This read-only audit made no source, build,
GUI, original-process, or mutable REA database changes.
