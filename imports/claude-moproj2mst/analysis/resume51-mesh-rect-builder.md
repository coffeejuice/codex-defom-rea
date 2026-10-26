# Original populated geometry-mesh rectangle builder

## Finding

The minimal evidence-backed populated fixture values are a single four-node quadrilateral in the geometry-list item’s owned `OBJ_MESH2D`: node count `4`, element count `1`, width / nodes-per-element `4`, coordinates `[(0,0), (2,0), (2,1), (0,1)]`, and connectivity `[1,2,3,4]` in the original one-based convention. The order gives a non-degenerate counter-clockwise rectangle. This is a valid builder input shape, but safe instantiation through current allocation accessors depends on the mesh node set having a valid owner/dimension table; verify that precondition in the fixture setup before allocating. Its viewer path is still explicitly refused when the positive mesh contribution reaches `viewerSceneBox`, so the fixture is a trigger for that known gap, not evidence that rectangle output is already supported.

## Original-builder trace

The immutable target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. A bounded raw DUMPBIN capture of VA `0x14124bec0..0x14124c100` is retained as [resume51-mesh-rect-builder-14124bec0.asm](resume51-mesh-rect-builder-14124bec0.asm), SHA-256 `0a16b7adce008eba1d26601f135285952c0fe37867c777f9253392ef86ee7ffa`. This is a bounded address window, not a claim about the exact function end. Original code was not executed.

The retained decompilation `evidence/gui_mo/s1/viewer_rect/r0.c` identifies `0x14124bec0` and shows these operations:

1. It returns without building if either the element count or node count is non-positive.
2. It calls `0x14124c1e0(item, width, element_count, node_count)`. The retained body in `r3.c` stores those values at item offsets `+0x188`, `+0x18c`, `+0x190`; it sizes the connectivity buffer to `element_count * width` and the per-node display buffer to `node_count * 3`.
3. It copies `element_count * width` connectivity integers and subtracts one from each. Thus the original source mesh connectivity is one-based; zero-based `[0,1,2,3]` would be wrong for this call path.
4. For each node, it emits a color entry and copies exactly two doubles from the coordinate input into two display coordinates, narrowing each to `float`. The per-node buffer’s third slot is reserved by the `node_count * 3` allocation; this routine writes color plus x and y.
5. It calls the item vtable at `+0x48` to refresh the derived item rectangle and stores the source coordinate pointer at `+0x218`.

The geometry-list refresh call chain is also retained: `evidence/deform-rea-work/out/mdtV/item2.c` shows `0x14126d0c0` obtains the geometry mesh node count, element count, width, coordinates, and connectivity, then passes those values to `0x14124bec0`. `analysis/wave2-geometry-contract.md` qualifies this as distinct from the object’s main FE mesh.

The port layout and accessor evidence agree with the builder’s inputs:

- `source/port/defmodel/include/defmodel/layout/obj_mesh2d.hpp`: mesh owns the node set at `+0x008` and element set at `+0x148`; total object size is `0x1700`.
- `source/port/defmodel/include/defmodel/llobj/cls_14017ef20.hpp`: node count is at subobject `+0x12c`; coordinates start at `+0x008` and are sized by node count and dimensions.
- `source/port/defmodel/include/defmodel/llobj/cls_140182880.hpp`: element count and width are at subobject `+0xe8/+0xec`; connectivity starts at `+0x008`.
- `source/port/defmodel/src/obj_mesh2d/mesh_access.cpp`: `fun_1403de680` allocates nodes; `fun_1403de6e0` allocates elements with the supplied width; `fun_140220440` returns coordinates; `fun_140220460` returns connectivity.

For the proposed fixture, once the node-set owner/dimension precondition is verified, call the node allocator with `n=4`, the element allocator with `ne=1,nper=4`, write the eight coordinate doubles in node order, and write four connectivity integers `[1,2,3,4]`. Use `fun_140220440`/`fun_140220460` to obtain writable arrays after allocation instead of fabricating positive count fields as the current regression does. `fun_14017f020` derives node dimensions through the node set’s owner pointer at `+0x130` and owner field `+0x15f8`; the allocation contract establishes two coordinate components for this builder, while a fixture must not guess the exact initialized dimension value if allocation requires it. If the ordinary `GEO2D_AND_MESH2D` construction path leaves the set without a suitable owner, first recover the original dimension initialization/ownership contract rather than invoking these accessors on the unattached synthetic mesh.

## Exact next test target and qualification

The narrow source target is the positive-mesh branch in `source/apps/moproj2mst/src/engine/viewer.cpp` (`refreshGeometryItem`, currently guarded at the geometry-mesh node/element-count check); the existing regression is `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`, test case `empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused`. Replace or supplement its direct `nodes->n = 1; elements->n = 1` trigger with the allocated four-node/one-quad fixture. First acceptance assertion should be that refresh reaches the populated-mesh boundary using valid arrays without refusal from allocation or accessors. After the rectangle-builder port is implemented from the retained behavior, assert the derived geometry item rectangle spans x `[0,2]` and y `[0,1]` (float storage is exact for these coordinates) and verify the expected scene-box union with the existing curve/model rectangle. Keep the current unsupported-path assertion until the implementation is ready; the static builder evidence alone does not authorize changing that refusal.

`0x14124bec0` does **not** prove malformed connectivity behavior, handling of widths other than four, a 3D coordinate mesh, or any original runtime output. This is a source-qualified, static construction plan. The raw PE capture and existing decompilation provide complementary evidence; no app, build, or mutable REA session was used.
