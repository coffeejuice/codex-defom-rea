# Populated geometry-mesh scene-box implementation plan

## Scope

Implement the populated `GEO2D_AND_MESH2D::mesh` rectangle contribution in `source/apps/moproj2mst/src/engine/viewer.cpp::refreshGeometryItem`, and replace the count-only fixture in `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` with valid arrays. Keep the work on this path separate from `viewer2d.cpp::meshItemPoints`, which reads the object’s main mesh and is not the geometry-list mesh.

The implementation should preserve the current overall flow: reset the item rectangle, union curve rectangles, then union the optional geometry mesh rectangle. A valid populated mesh makes `it.rectKnown=true`; a supported empty mesh contributes no rectangle. `viewer::refresh` then propagates item knowledge to `modelRectKnown`, and `viewerSceneBox` can use the model rectangle when its existing 3D-view flag and radius conditions select that fallback. Keep the current `M2M_E_UNSUPPORTED` for an unknown rectangle until this helper is implemented and covered.

## Data contract and access method

The original `0x14124bec0` receives width, element count, node count, connectivity, and coordinates. It allocates `element_count * width` connectivity entries, changes each source index from one-based to zero-based, reads exactly two doubles per node, narrows x/y to floats, and rebuilds the item rectangle by extending over every node position. The rectangle therefore includes all mesh nodes, independent of whether a node is referenced by an element. The item’s connectivity affects its mesh representation, but the original rectangle loop ranges over node count.

Do not use `fun_140220440` as the coordinate getter for this geometry-owned mesh. A fresh `OBJ_MESH2D` has node-set owner `u130=null`; that accessor derives its allocation dimensions from the owner. Binding this set to a 3D `DB_STEP_DATA` would instead expose a 3-coordinate allocation table, inconsistent with the rectangle builder’s two-coordinate stride. Read the recovered node and element subobject fields directly, after the checks below:

- node subobject is `mesh->s008`; node count is `+0x12c`; the coordinate pointer is `+0x008`;
- element subobject is `mesh->s148`; element count and width are `+0xe8/+0xec`; connectivity pointer is `+0x008`;
- `OBJ_MESH2D::reset` initializes its node and element dimension flags (`u138` and `u0f8`) to 2. Require those expected flags for this 2D mesh path; do not use the enclosing viewer model’s dimension as the mesh coordinate stride.

Implement a small internal helper that takes the const mesh plus a `Rect2` output and returns a known/unsupported result. For a valid positive mesh, iterate all nodes, take `static_cast<float>(coords[2*i])` and `static_cast<float>(coords[2*i+1])`, then extend a local `Rect2` using the float overload. That preserves the original double-to-float conversion before rectangle accumulation. Union the local rectangle with the curve-derived rectangle only after the full mesh validates, so malformed input cannot leave a partial cache.

## Bounded refusal conditions

Preserve the known-empty rule: if either node count or element count is zero, there is no mesh contribution, matching the original positive-count gate. For positive counts, return the existing unsupported status with a specific reason before reading arrays when any of these is true:

- the node or element dimension flags do not equal 2;
- width is non-positive, or checked multiplication of `element_count * width` / `node_count * 2` would overflow the representable allocation/index size;
- the coordinate or connectivity pointer is null;
- a coordinate is non-finite (the populated rectangle’s NaN/Infinity behavior is not recovered);
- any one-based connectivity value is outside `[1,node_count]`.

These checks intentionally bound malformed input instead of attributing new behavior to the original. Do not require width exactly 4: the retained builder uses the supplied width as a stride and count. The focused fixture is one quad, width 4, but a positive safe width with complete valid indices can follow the same loop contract. Do not attempt to recover or render mesh colors, material groups, element topology, or a 3D coordinate mesh in this branch.

## Fixture and acceptance assertions

In `empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused` or a neighboring test, keep the existing empty case, then create a fresh geometry mesh and populate its raw arrays with destructor-compatible allocations:

```text
nodes.n = 4
nodes.u008 = calloc(8, sizeof(double))
coords = (0,0), (2,0), (2,1), (0,1)
elements.n = 1
elements.m = 4
elements.u008 = calloc(4, sizeof(int32_t))
connectivity = (1,2,3,4)
```

Initialize no unrelated arrays. `Cls_14017ef20::free_arrays` and `Cls_140182880::free_arrays` release `u008` with `std::free`; `OBJ_MESH2D` owns and destroys these subobjects. `calloc` therefore matches cleanup and provides deterministic initial memory. Avoid the normal positive-count coordinate getter for this standalone fixture because its owner/dimension precondition is absent.

Use the current curve fixture with extent `[-2,2]` as a simple union check. After a valid mesh refresh, the item/model rectangle must be `x=[-2,2], y=[0,1]`; using a curve outside that range can separately verify actual union (e.g. curve extent `[-4,4]` gives `x=[-4,4]`). On the existing shown 3D-view setup where the scene-rectangle fallback is selected and its radius exceeds `1e-6`, `viewerSceneBox` should return that rectangle as a z=0 box. Do not assert the scene box uses the rectangle under flags that select the 3D model box; the existing display-flag decision remains authoritative.

Add focused guard cases for null arrays, width 0, out-of-range connectivity, non-finite coordinates, and a non-2 dimension flag. Assert `M2M_E_UNSUPPORTED` before partial rectangle publication. Preserve controls for zero nodes or zero elements: their mesh portion is skipped and the curve-only result remains known. Keep the existing explicit failure test until the populated valid case passes; remove that refusal only with the source implementation and these regression receipts together.

## Evidence and limits

This plan is grounded in `analysis/resume51-mesh-rect-builder.md`, `analysis/resume52-mesh-fixture-owner.md`, original decompilation at `evidence/gui_mo/s1/viewer_rect/r0.c` and `r3.c`, caller `evidence/deform-rea-work/out/mdtV/item2.c`, and the current source/layout files cited above. Original behavior for malformed arrays, non-finite geometry coordinates, arbitrary widths, and a populated mesh runtime was not observed. No production source, build, test, GUI, or mutable REA state was changed or exercised for this plan.
