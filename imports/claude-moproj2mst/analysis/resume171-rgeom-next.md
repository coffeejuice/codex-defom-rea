# CP171: geometry-list mesh readiness remains owner-blocked

## Finding

The next owner/reachability witness is still missing. The current native evidence proves a bounded downstream cut from a geometry-list item refresh to the rectangle builder, but it does not prove a real producer initializes the optional mesh node set's owner pointer (`OBJ_MESH2D + 0x008 + 0x130`) or that a reachable original instance consequently passes the coordinate getter. Do not admit the positive geometry-list mesh profile into the converter from synthetic counts, arrays, or port behavior alone.

## Qualified native cut

For binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, the retained exact function disassembly `analysis/wave2-geometry-14126d0c0-disassembly.txt` (function `0x14126d0c0`, exact pdata range `[0x14126d0c0,0x14126d3d1)`) shows the bound geometry-list item path. At `0x14126d266` it invokes readiness `0x1408ee940`; on true, it resolves the mesh, obtains node and element counts (`0x1400c3570`, `0x1400c3590`), width (`0x140ab4b90`), coordinates (`0x140514f00`), and connectivity (`0x140894330`), then calls rectangle builder `0x14124bec0` at `0x14126d305`. `analysis/resume53-geometry3d-ownership.md` qualifies readiness order: non-null mesh, positive node and element counts, non-null coordinate result, then non-null connectivity result. This establishes the consumer guard and call path, not a populated original producer.

The rectangle contribution composes with scene selection at `0x140c8e0f0`; `analysis/wave2-geometry-contract.md` records its conditions and the `1e-6` radius cutoff. This is not a reason to expand the present cut into the whole viewer or claim Cogging Save refresh ordering.

## Ownership and reachability gap

`GEO2D_AND_MESH2D::mesh` is owned by its geometry cross-section. That does not establish the nested node-set owner at `OBJ_MESH2D + 0x008 + 0x130`. The source-qualified port files `source/port/defmodel/src/geo/geo2d_and_mesh2d.cpp`, `source/port/defmodel/src/obj_mesh2d/obj_mesh2d.cpp`, and `source/port/defmodel/include/defmodel/layout/geo2d_and_mesh2d.hpp` establish allocation, deletion and copy structure only. The constructor initializes the nested owner null; the reviewed copy path copies arrays but does not set it.

Retained wrapper evidence in `resume53-geometry3d-ownership.md` says `0x140514f00` reaches `0x140854af0`, which passes node state to `0x140854200`; bounded PE bytes at `0x140853240` show null `+0x130` returns null before using owner `+0x15f8`. No retained evidence identifies a geometry-mesh-specific writer setting that owner, nor a qualified original getter behavior for the null-owner populated-array case. Thus a synthetic mesh with positive counts and candidate arrays is insufficient to claim the native rectangle builder is reachable.

The earlier exact next action remains: statically capture and qualify a creator/copy/read writer that stores `OBJ_MESH2D + 0x008 + 0x130` (and matching dimension setter `u138`), then connect its xrefs to the geometry cross-section's mesh create/copy path. In parallel, bound `0x140854200` and its state-variable helper only enough to determine null-dimension behavior for prepopulated `u008`. Required witness: writer identity and owner type/value, exact flow to the optional cross-section mesh, and a complete guarded path showing non-null coordinate and connectivity outputs reaching `0x14124bec0` for the same profile. If no writer or safe getter contract is found, keep the explicit refusal for positive geometry-list mesh contribution.

## Source qualifications and acceptance test

- `analysis/wave2-geometry-static-evidence.json` binds offline exact/leaf disassembly records to the original binary. Exact pdata functions and bounded wrapper windows are distinguished there; a wrapper/leaf window is not promoted to a complete function body.
- `analysis/wave2-geometry-contract.md` combines retained decompilation with fresh offline PE checks. Threshold expectations are source-derived, not original runtime outputs. No full scene-selector execution was performed.
- `analysis/resume53-geometry3d-ownership.md` separates cross-section ownership from nested node-set owner state and explicitly limits the synthetic fixture's evidentiary value.
- `source/apps/moproj2mst/src/engine/viewer.cpp` currently admits only qualified readiness profiles; preserve its refusal until the witness above is closed.

Acceptance for the next bounded cut: produce hash-bound static evidence for the owner writer, exact xref chain to a reachable geometry-list mesh producer, and getter/rectangle call order; add no inferred owner initialization. If runtime observation is required, use only an already authorized, isolated immutable capture workflow; this report itself uses none. The existing focused rectangle test must then demonstrate the exact admitted native-compatible owner profile and continue refusing null/wrong owner, missing coordinate, missing connectivity, and unqualified positive-count profiles. Do not call this native parity without an original observation or a complete source-qualified behavioral proof.

No source or build was changed; no GUI, original execution, live REA, or shared mutable state was used. The only new artifact is this bounded audit note.
