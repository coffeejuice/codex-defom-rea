# Resume 46 embedded geometry-mesh trigger inventory

## Finding

No existing corpus fixture is proven by the retained manifests/indexes to satisfy the remaining positive embedded-geometry-mesh scene-box refusal. This is a limit of the evidence, not proof that the corpus contains no such project. The tracked source corpus manifest records file path, file kind, byte size, hash, and original path; `runner/cases.tsv` and `cases_b.tsv` record project files, linked-file counts, operation types, and provenance. None records the runtime `GEO2D_AND_MESH2D::mesh` pointer or its nested node and element counts. The Oracle-B resave fixture `131R_B_COG_NEWOP_RESAVE` was considered and ruled out as a demonstrated trigger: current retained comparator receipts mark it `SAME` with zero differences.

## Exact guard predicates

`source/apps/moproj2mst/src/engine/viewer.cpp` shows `refreshGeometryItem` marks `rectKnown = false` only when all of the following hold: a bound geometry item resolves to a source geometry; `geometry->mesh` is non-null; the mesh node count (`mesh->s008`) is greater than zero; and its element count (`mesh->s148`) is greater than zero. `viewerSceneBox` throws only if that unknown rectangle reaches the 3D model-rectangle fallback: some view is shown, all shown views have flags 2, 3, and `0x2d` clear, and `modelRectKnown` is false. A 2D model uses `modelRect` directly as its scene box and does not take this 3D fallback branch.

The source predicate therefore needs a 3D model containing a cross-section `GEO2D_AND_MESH2D` item with a populated owned `OBJ_MESH2D`, plus viewer state that selects the 3D rect fallback. The 2D files listed in the corpus cannot be used as evidence for the final scene-box guard by themselves, even if they contain populated cross-section meshes.

## Evidence review

- `source/docs/corpus/manifest.tsv` has no extracted geometry or mesh-presence fields. Searching its contents for geometry-mesh labels, `GEO2D_AND_MESH2D`, and object mesh markers yields no match.
- `source/docs/re/moproj2mst/runner/cases.tsv` and `cases_b.tsv` do not encode cross-section mesh counts. They can identify 2D/3D project candidates and KEY dependencies only.
- `analysis/wave2-geometry-contract.md` and `.json` qualify the exact gap: `0x1408ee940` reads nested node and element counts; empty contributions are known, positive mesh coordinate/connectivity and item-box paths remain unrecovered. `test_viewer_box.cpp` synthetic coverage distinguishes empty from populated counts but does not bind those states to a corpus project.
- Ordinary object FE mesh evidence such as KEY `RZ`/`ELMCON` is not sufficient: the gap is the separate mesh pointer owned by a geometry-list cross-section, not the LLOBJ object's primary mesh arrays.
- The historical 131/131R refusal was on 3D scene-box fallback before geometry-item rect recovery. `analysis/wave2-geometry-validation.json` and `analysis/wave3-candidate45-doe-fix-d-tests.json` instead record both cases as `SAME`, zero diff counts, and no comparator adjustments. They do not assert raw byte identity or identify a current positive embedded-mesh trigger.

## Smallest next evidence step

To prove absence or identify a trigger, enumerate actual copied 3D project object KEYs, parse each object's geometry-list records, and inspect the `GEO2D_AND_MESH2D` owned mesh counts (`OBJ_MESH2D +0x008/+0x148` nested count fields). The reader must bind each KEY back to its project row and hash so the selected item is reproducible. Then retain only projects with positive node and element counts and inspect whether the project’s 3D Save path can enter the `viewerSceneBox` fallback. If no eligible row remains after this source-level parse, report none in the current copied 3D corpus; do not use a 2D mesh sample or top-level FE mesh as a substitute.

This inventory used static retained source and records only. No KEY parsing executable, converter, build, application, or REA database was run or changed.
