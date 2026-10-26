# Resume 50 populated 3D geometry fixture

## Finding

An evidence-backed synthetic trigger already exists in `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`, test case **`empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused`**. It constructs a 3D `GuiModel`, adds a curve-backed geometry item, attaches an `OBJ_MESH2D`, and changes the native-layout node and element counts from zero to one. That is sufficient to trigger the exact current positive-count refusal without launching the original app or mutating a retained fixture.

There is no located retained corpus fixture with a populated `OBJ_MESH2D` on the `OBJ_GEO2D` geometry-list item path. Existing `vdie42_cogging_viewer_boxes` evidence explicitly describes its objects as geometry-only and without a mesh. The synthetic test is therefore the best existing fixture for the guard boundary, but it is **not** a valid populated-node/connectivity fixture and does not exercise the unknown mesh rectangle builder.

## Exact branch and setup

The test setup calls `geometryLine(m, 2.0)`, which creates the object's first `OBJ_GEO2D`, adds a two-point curve, and synchronizes the 3D viewer. It then sets:

```cpp
geometry->mesh = new def::model::OBJ_MESH2D();
nodes->n = 1;
elements->n = 1;
```

The backing pointers are `geometry->mesh->s008` cast to `Cls_14017ef20` and `geometry->mesh->s148` cast to `Cls_140182880`. After `engine::viewerRefresh(m)`, the source-qualified port path in `source/apps/moproj2mst/src/engine/viewer.cpp::refreshGeometryItem` reads both counts. When both are positive it marks the mesh contribution unknown (`rectKnown = false`). `viewerSceneBox` throws `M2M_E_UNSUPPORTED` only if the shown-view settings select the model rectangle; the test's synchronized view makes that selection and catches this exact guard:

`3D viewer: a bound geometry-list mesh item rectangle (0x14126d0c0 -> 0x14124bec0) is not traced`

The original source qualification is in `analysis/wave2-geometry-contract.json`: `0x14126d0c0` refreshes a bound item; `0x1408ee940` checks the `GEO2D_AND_MESH2D::mesh` pointer and its node/element counts; positive counts continue into coordinate/connectivity getters and builder `0x14124bec0`, which remain unrecovered. This trigger reaches the exact boundary before guessing those calls.

## Test path and latest receipt

The exact CTest name is **`m2m.mdt.viewer_box`**, executable `build/candidate/bin/m2m_mdt_viewer_box.exe`. A retained latest test-run log at `build/test-runs/1792866828185209600/m2m.mdt.viewer_box.log` reports all five cases passing, including the populated-count refusal case; its per-test receipt reports `passed`, zero failure, no gap, and no diagnostic difference. This establishes that the current candidate's safety guard fires for the synthetic positive-count trigger.

For a fresh coordinator-owned focused rerun after the candidate source is fixed and frozen, use:

```powershell
ctest --test-dir build/candidate -R '^m2m\.mdt\.viewer_box$' --output-on-failure
```

No run was performed for this audit.

## What remains to resolve the omission

This synthetic trigger confirms the current guard, not parity or a real populated geometry box. The smallest next evidence action is to find an existing immutable DB/KEY fixture whose **geometry-list** `OBJ_GEO2D::mesh` has positive node and element counts, or construct a byte-backed synthetic only after the `OBJ_MESH2D` layout, node getter, element/connectivity ownership, and lifetime rules are source-qualified. The fixture must populate data only in a disposable test copy and retain distinct raw counts/arrays; writing `n=1` alone is not enough to call it a valid geometry fixture.

Then recover the first coordinate and connectivity getter reached from `0x14124bec0`, along with original order/side effects, and add a separate focused case that checks the produced item rect and scene selection. Keep the existing positive-count guard test until that implementation has source evidence and regression coverage. Do not substitute the object's separate 2D mesh or a geometry-only `vdie42` case; neither exercises this list-item mesh path.

## Qualifications

- The test bypasses file loading and creates the mesh object directly, so it proves a unit-level state trigger, not that real Open/Save fixtures can reach this state.
- Counts are positive but coordinate/connectivity arrays are not populated; the original builder is deliberately not called.
- Passing `m2m.mdt.viewer_box` validates the candidate guard and preceding empty/curve-only geometry behavior. It does not establish the original builder's output, original runtime parity, or completion of the `RGEOM`/objective acceptance gate.
- Historical `131R` and scene-box refusals corrected by the geometry work are unrelated; do not revive them as evidence for this remaining branch.

## Evidence inspected

- `analysis/wave2-geometry-contract.json` and `analysis/wave2-geometry-validation.json`.
- `source/apps/moproj2mst/src/engine/viewer.cpp`.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`, especially `geometryLine` and `empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused`.
- `build/test-runs/1792866828185209600/m2m.mdt.viewer_box.log` and its `receipt.json`.
- CTest name/executable from retained `build/candidate/Testing/Temporary/LastTest.log` and `CTestCostData.txt`.

No source, build, fixture, application, or mutable REA database was changed or executed in this audit.
