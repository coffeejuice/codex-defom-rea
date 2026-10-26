# OBJ_MESH2D fixture owner and coordinate dimension

## Resolved precondition

For the proposed geometry-list mesh, do **not** call `fun_140220440` on a freshly constructed, standalone `OBJ_MESH2D`. Its node-set owner pointer is null by construction, so the normal coordinate accessor cannot obtain the dimension table that its allocator requires.

The source chain establishes this directly:

- `Cls_14017ef20` constructor initializes `u130` (the owner at node-set `+0x130`) to null; see `source/port/defmodel/src/llobj/cls_14017ef20.cpp`.
- `OBJ_MESH2D::reset` sets the node set’s `u138` to `2`, but does not set `u130`; see `source/port/defmodel/src/obj_mesh2d/obj_mesh2d.cpp`.
- `fun_14017f020` / `svkw::set8a0_dims` returns null when `u130` is null; see `source/port/defmodel/src/statevar_kw/statevar_kw.cpp`.
- `fun_140220440` calls `fun_1401808d0`, which passes that dimension pointer to `node_set_alloc`. For a positive count, `node_set_alloc` evaluates `dims[0]` to allocate coordinates; see `source/port/defmodel/src/node_access/node_access.cpp` and `source/port/defmodel/src/bc/node_arrays.cpp`. Thus an unattached positive-count mesh plus this accessor is not a safe setup.

There is a second trap: binding that node set to the normal 3D `DB_STEP_DATA` owner does not establish a 2D coordinate stride. `state_remap.cpp` sets `DB_STEP_DATA::u15f8[0]` from the model dimension (`2` or `3`), and `set8a0_dims` exposes that table. A 3D owner therefore makes the coordinate allocator size three doubles per node. The original rectangle builder `0x14124bec0` demonstrably reads **two** doubles for each node, using source offset `2*i + component` (see `evidence/gui_mo/s1/viewer_rect/r0.c`). Mixing a three-double allocation with that two-double read stride would misinterpret rows after node zero. The constructor’s `u138=2` alone does not override this accessor path.

## Safe synthetic fixture construction

For the focused viewer test, initialize the mesh’s already-defined raw array fields to the dimensions this builder actually consumes, rather than calling the owner-dependent coordinate getter:

- node set at mesh `+0x008`: `n=4`, `u008` points to eight doubles in node order: `(0,0),(2,0),(2,1),(0,1)`;
- element set at mesh `+0x148`: `n=1`, `m=4`, `u008` points to four connectivity integers `[1,2,3,4]`;
- ensure any allocated storage uses `calloc`/`malloc` compatible with the existing `free_arrays` destructors, which release these fields with `free`; leave unrelated arrays null;
- let normal `OBJ_MESH2D` destruction release the owned arrays. Do not bind its node-set owner to the 3D model merely to make the getter non-null.

The exact fields and destructors are source-backed by `cls_14017ef20.hpp/.cpp`, `cls_140182880.hpp/.cpp`, `obj_mesh2d.hpp`, and `bc/node_arrays.cpp`. The geometry-mesh rectangle path in the original GUI is called with width 4 and reads two coordinate values per node. This synthetic layout therefore satisfies the recovered builder contract without inventing owner semantics or causing allocation to dereference a null dimension pointer.

This is a fixture-construction design, not a test run. The existing test `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp::empty_geometry_mesh_is_known_but_populated_mesh_rect_stays_refused` currently sets only the two count fields, which is adequate for reaching the explicit refusal but is not populated mesh evidence. A separate focused case can replace/supplement it with these correctly shaped arrays while preserving the expected `M2M_E_UNSUPPORTED` until the rectangle builder is ported. A future production implementation must decide how the two-dimensional coordinate source is represented and accessed; this fixture does not justify changing that contract.

## Limits

The native call chain identifies separate coordinate and connectivity getters before `0x14124bec0`, and the builder’s two-coordinate stride is visible in retained decompilation. Their full original machine-level accessor bodies and the original runtime behavior of a populated geometry mesh were not executed or newly claimed here. The conclusion about the current port’s null-owner accessor follows from the checked-in constructor/allocation source, not from original application execution.
