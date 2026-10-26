# Resume 47 geometry-owned mesh fixture inventory

## Result

No retained copied 3D KEY fixture proves a populated `GEO2D_AND_MESH2D::mesh` that would satisfy the current guarded scene-box refusal. A raw positive 3D `DIEGEO <object> 1 <nodes> <elements>` record is not that evidence: in 3D, the recovered DIEGEO parser places this surface form in `obj+0x3c00[0]` (`OBJ_GEO3D`), while curve-form DIEGEO creates geometry-list entry 0 at `obj+0x3998`. Packet V's refused mesh is instead the optional `OBJ_MESH2D*` owned by a geometry-list cross-section at `GEO2D_AND_MESH2D+0x28`.

The retained 3D corpus has objects containing both curve-form DIEGEO and 3D surface-form DIEGEO, but that demonstrates a 2D curve section plus a separate 3D surface; it does not establish the geometry-list cross-section's optional mesh pointer or the nested `OBJ_MESH2D` node/element counts. For example, `53_Cogging_Multiple_Ops_Lab1/Objects/Object00001.KEY` has curve DIEGEO records at lines 34 and 41 and a surface record at line 67. The packet grammar explicitly maps the latter to the separate 3D surface. Do not treat these records as trigger fixtures.

## Corpus sweep performed

Read-only inspection covered all copied object KEY files under `evidence/gui_mo/s1/corpus` (1,912 KEY files total). The source row and copied folder identify the 3D cases; packet DIEGEO grammar was used to distinguish 3D curve forms (two header integers after object id) from 3D surface forms (three). There are 35 positive surface-form records across 49 copied 3D project folders in this scan, but they are `OBJ_GEO3D` / primary surface data, not geometry-owned `OBJ_MESH2D` evidence. No KEY keyword grammar or retained manifest field serializes the optional cross-section mesh pointer/counts.

References:

- `source/docs/re/keywords/DIEGEO.md`, §§ KEY grammar, Model storage, Surface form, and corpus check: 3D curve DIEGEO creates `obj+0x3998` geometry-list curves; 3D surface DIEGEO resolves `obj+0x3c00[0]` and sizes surface arrays.
- `source/port/defmodel/include/defmodel/layout/geo2d_and_mesh2d.hpp`: geometry-list cross-section has owned optional mesh at `+0x28`.
- `source/port/defmodel/include/defmodel/layout/obj_mesh2d.hpp`: that mesh is a separate 0x1700-byte type with node/element subobjects at `+0x008` / `+0x148`.
- `analysis/wave2-geometry-contract.md` and `.json`: viewer checks those nested counts and positive-count mesh coordinate/connectivity/item bounds remain untraced.
- `source/docs/corpus/manifest.tsv` lists KEY path, dimensions, sizes, hashes and origin; it does not describe this pointer or the nested mesh counts.

## Qualification and next path

This proves only that existing copied KEY text does not establish a geometry-owned mesh trigger; it does not prove the complete corpus has no runtime instance. A project DB can load state beyond the copied KEY records, and the current manifests do not provide a decoded `GEO2D_AND_MESH2D` object graph. The next conclusive source must be an existing copied DB-step dump or a faithful DB-step decoder that reports, for each 3D operation object's geometry-list entry, mesh pointer presence and both positive counts. Bind that result to an existing project/case and show its `viewerSceneBox` display flags select the 3D rectangle fallback. Until then, there is no evidence-backed existing fixture to nominate for the guard.

No source or retained evidence was modified. No converter, build, GUI application, or REA database was run.
