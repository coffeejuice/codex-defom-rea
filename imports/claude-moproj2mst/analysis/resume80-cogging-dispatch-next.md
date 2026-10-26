# CP80 Cogging and geometry dispatch gap

## Finding

The smallest remaining source-reachable gap is the positive populated mesh on a
bound 3D geometry-list item during Cogging Save. Cogging operation dispatch is
present; the unresolved behavior is the geometry contribution that the viewer
must provide before the writer can finish.

In native-save mode, `convert.cpp` visits each operation and tries
`saveCoggingMdt` before the ordinary property serializers. The Cogging handler
reaches `coggingAfterSave`, which computes geometry for objects 1, 2 and 4 and
then places billet, dies and manipulators (`cogging_save.cpp`). The recovered
native Cogging path consumes the cached viewer box with recomputation disabled
(`source/docs/re/moproj2mst/mdt/V/spec_V.md`). Thus the viewer's bound geometry
item rectangle can affect a Cogging output through the public save route.

For 3D scene-box selection, current `viewer.cpp` refuses a needed unknown item
rectangle with `M2M_E_UNSUPPORTED`. `refreshGeometryItem` can determine the
rectangle for a supplied, already populated geometry-owned `OBJ_MESH2D` under
the qualified getter and builder profile. This does not establish that a real
project supplies such a mesh. CP57 records that no retained real project fixture
proves the positive counts and prepopulated arrays. The old
`131R_B_COG_NEWOP_RESAVE` refusal is not a current reproducer: later validation
reported zero diffs.

## Dispatch reconciliation

- `SfOprCogging` is explicitly categorized in `save_current.cpp::propsToEngine`;
  `saveCurrentOperation` routes it through its post-save hook. It is not a
  missing operation dispatch.
- `convert.cpp`'s native per-operation loop invokes the Cogging writer before
  L6/Express property handlers. The aggregate `--write-mdt` path also invokes
  `saveCoggingMdts`. The multi-reheat refusal is separately and deliberately
  guarded; it does not explain the geometry gap.
- The geometry-list branch is reachable from the viewer's 3D scene-box
  selection when a bound item's rectangle is needed. The flag-0x2b per-view
  refresh route remains a separate uncertainty: its retained native body is
  known, but evidence tying the flag producer to ordinary Cogging Open/Save is
  absent. It is therefore not the selected next target.

## Original evidence boundary and next action

The native chain is `0x14126d0c0` (geometry item refresh) → `0x1408ee940`
(mesh readiness) → `0x14124bec0` (mesh rectangle), then the scene-box selection
and Cogging cached-box consumer. Retained ownership evidence in
`analysis/resume53-geometry3d-ownership.md` establishes that the optional mesh
belongs to `GEO2D_AND_MESH2D + 0x28`; it is independent of the object's main FE
mesh. CP62–65 receipts qualify getter order and selected synchronization fast
routes. CP64 keeps unknown owners/readiness refused. The current candidate
source therefore has a guarded synthetic populated-mesh route, but not a
source-qualified real-project population lifecycle or parity claim.

Smallest next action: inspect retained, immutable original project/object/DB
evidence for one bound 3D geometry-list item whose optional mesh has positive
node and element counts, preexisting coordinate and connectivity arrays, and
the admitted owner/registry profile *before* `0x1408ee940`. Bind that exact
item to the Cogging save route and compare its resulting scene box / Cogging
output with the retained oracle. If no such capture exists, keep the current
unsupported guard for real-project claims and record the synthetic route only
as candidate unit coverage. Do not infer a geometry mesh from the main FE mesh,
or infer its arrays from the fact that the native constructors allocate the
optional mesh.

No GUI, mutable REA database, source, build, or test action was performed.
