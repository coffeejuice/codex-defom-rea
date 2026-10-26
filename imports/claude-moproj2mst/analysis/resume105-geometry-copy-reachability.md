# CP105 Cogging acceptance versus geometry-list mesh reachability

## Finding

CP103's strict five-file acceptance is valuable public `WRITE_SAVE` coverage for
ordinary nonempty Cogging output. It does not establish the optional
geometry-list mesh branch exercised by CP88. The retained case
`RS_42_vdie` compares `Cogging.MDT` and `Object00010..13.KEY`; the strict
acceptance requires the selected Save to produce all five outputs byte-for-byte
equal to the retained after-state, including the known pre/post changes. Those
files establish that a real Save transition occurred, but none is a decoded
snapshot of the viewer's `GEO2D_AND_MESH2D::mesh` pointer or its nested array
state.

The 42_vdie public result therefore cannot be used as evidence that a positive
geometry-list mesh was present, that display flag 43 (`0x2b`) was enabled, or
that `recompute3D` incorporated such a mesh into the cached box. Native Cogging
Save reads the cached view box with recomputation disabled; CP88's two-pass
refresh/aggregate test exercises the code that can establish that cache when
the flag and qualified item are present. The CP103 and CP88 claims are
complementary, but there is no observed path joining them for this fixture.

## What static source narrows

The native/source-qualified copy implementation explains one possible way to
produce a populated optional mesh, but also narrows the missing lifecycle edge:

- `source/port/defmodel/src/geo/geo2d_and_mesh2d.cpp`,
  `GEO2D_AND_MESH2D::copy_from`, replaces the destination optional mesh when
  the source mesh is non-null, allocates its `OBJ_MESH2D`, and dispatches the
  native copy helper `0x1403de460`.
- `source/port/defmodel/src/obj_mesh2d/obj_mesh2d.cpp`,
  `OBJ_MESH2D::copy_from`, resets the destination then copies node and element
  sets with the mesh's local flags. It does not bind the embedded node-set
  owner to the enclosing LLOBJ or `DB_STEP_DATA`.
- `source/port/defmodel/src/llobj_ctor/llobj_copy.cpp::copy_geometry` invokes
  `copy_u3998` only when the owner is dimension 2. Its dimension-3 route copies
  Geo3D slots instead. This prevents treating the existing 2D geometry-list
  owner-copy test as proof that a normal 3D Open or object-copy path populates
  this mesh.
- The retained KEY grammar in `source/docs/re/keywords/DIEGEO.md` further
  rules out a tempting but incorrect fixture inference. In 3D, the curve forms
  populate geometry-list entry zero, whereas the surface form populates the
  separate `OBJ_GEO3D` surface slot (`obj+0x3c00[0]`). The KEY format and
  corpus manifest do not encode the optional `GEO2D_AND_MESH2D::mesh` pointer
  or the embedded node/element array pointers.

This evidence shows the optional mesh can be copied if a source mesh already
exists. It does not identify a 3D producer that creates that source mesh, show
that a 3D project invokes the outer copy path, or establish array readiness
before the viewer getters. Retained node-owner evidence allows an already
allocated coordinate/connectivity array to be read with a null nested owner;
it does not qualify a null-array allocation path. The existing refusal remains
appropriate for unqualified inputs.

## CP103 acceptance boundary

The strict test specified in `analysis/resume103-session-cogging-save-acceptance.md`
uses public conversion with `WRITE_SAVE` and compares exactly these five
geometry files: `Cogging.MDT` plus `Object00010.KEY` through
`Object00013.KEY`. It specifically verifies a nonempty state transition and
raw bytes without path-marker normalization. It does not inspect internal
viewer display flags, cross-section item ownership, optional mesh state, or
which cached-box contribution produced the serialized placement. Therefore,
passing all five comparisons proves the ordinary Save contract for that
fixture only; it neither validates nor falsifies CP88's geometry-list mesh
contribution.

## Smallest missing witness and next action

The most discriminating retained artifact would be an immutable decoded object
or DB-step capture for one real 3D project, taken before the viewer refresh,
that binds the project, object, geometry-list item, and view together and
records:

1. that item's optional mesh pointer is non-null;
2. positive node and element counts;
3. non-null preexisting coordinate and connectivity array pointers;
4. the nested node owner value (including whether it is null); and
5. flag 43 enabled for that same view before its box is queried by Cogging Save.

The retained copied KEY corpus cannot supply those pointer/lifecycle fields,
and the available static copy branch has no proven 3D caller/producer that
fills them. Until such a capture is located or separately authorized, the
smallest safe next action is to keep CP103 as strict ordinary nonempty Save
acceptance, retain CP88's guards and test as candidate branch coverage, and
make no claim that a real public 3D Open-to-Save fixture reaches the optional
mesh branch. A synthetic populated-mesh copy test would be unit coverage only;
it would not close this reachability gap.

This was a read-only reconciliation. No source, test, build, GUI, original
execution, or mutable REA database was touched.
