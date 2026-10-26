# Geometry-list mesh node owner and native getter branches

## Target and evidence identity

The retained target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The two new captures are bounded DUMPBIN windows from that immutable PE:

- [resume56-geometry-node-dimensions-140853240.asm](resume56-geometry-node-dimensions-140853240.asm), SHA-256 `f0be8fbb54d296839e90e28cea67677da6701b61aade7a4adba4d0e457aad00a`.
- [resume56-geometry-connectivity-getter-140857ed0.asm](resume56-geometry-connectivity-getter-140857ed0.asm), SHA-256 `ba764dfe04a57293e2a162d34f0384ac98f5ba2e7b05b20d7a2fda6e12fe1240`.

Existing source-qualified captures provide the remaining windows: `analysis/wave3-editor-mds-coordinate-storage-140854af0.asm`, `analysis/wave3-editor-mds-gui-storage-twins-complete-140854200.asm`, `analysis/wave3-editor-mds-coordinate-allocator-dependencies-140b3e8e0.asm`, and `analysis/wave3-editor-mds-node-variable-key-zero-1409e49d0.asm` / `...-140b3f0e0.asm`; their receipts bind them to the same target digest.

## What sets the nested node owner

The known original owner setter is the LLOBJ owner-binding path: `LLOBJ_DATA` owner assignment writes its node set’s `+0x130` to the owning `DB_STEP_DATA` and changes the dimension flag from that model. The checked-in reconstruction is source-qualified at `source/port/defmodel/src/llobj_ctor/llobj_ctor.cpp` (`set_owner`, native `0x14015ef60`; nested nodal owner site `0x14017efc0`). This setter applies to the object’s node set at `LLOBJ_DATA+0x8a0`.

That owner setup is **not** the lifetime relationship of the refused mesh. `GEO2D_AND_MESH2D::mesh` at `+0x28` owns an independent `OBJ_MESH2D`; its constructor initializes the embedded node set owner `s008+0x130` to null and `OBJ_MESH2D::reset` establishes its local 2D flags. Its copy path copies/allocates node arrays using the mesh’s local `flags` and does not call the LLOBJ owner setter (`source/port/defmodel/src/obj_mesh2d/obj_mesh2d.cpp`). Thus static evidence does not show a mesh-specific writer assigning `s008+0x130`; do not infer an owner pointer from the `GEO2D_AND_MESH2D` cross-section’s ownership of the allocation.

## Accessor behavior when that owner is null

The original geometry item’s predicate is retained in `evidence/deform-rea-work/out/mdtV/item3.c` as `0x1408ee940`. After non-null mesh, positive node count (`0x1400c3570`) and positive element count (`0x1400c3590`), it calls coordinate getter `0x140514f00(mesh)`, rejects a null result, then calls connectivity getter `0x140894330(mesh)` and rejects a null result. Only then does the predicate return true, allowing `0x14126d0c0` to call builder `0x14124bec0`. A false predicate skips this mesh contribution; it is not an error status from the original selector.

The helper behavior explains how a geometry-owned mesh can pass these guards without an owner pointer, provided its arrays were already populated:

- `0x140514f00` forwards to `0x140854af0(mesh+0x008)`. The latter obtains the owner dimension table through `0x140853240`; the new raw capture shows that helper returns null when set `+0x130` is null. `0x140854200` only reads `dims[0]` when it has to allocate coordinates because `set+8` is null. When coordinates already exist, that direct allocation is bypassed.
- The coordinate allocator then dispatches key `0` over the set’s registered state variables (`0x140b3e8e0`). The retained state-variable entries have no id `0`; the bounded implementations at `0x1409e49d0` and `0x140b3f0e0` compare the request against each variable id and skip the resize when neither the id nor wildcard `-1` matches. `0x140854af0` also skips its model synchronization call when `+0x130` is null, then returns `set+8`.
- `0x140894330` forwards to `0x140857ed0(mesh+0x148)`. The new capture shows that a null element owner at `+0xf0` bypasses synchronization and returns the connectivity pointer at `set+8` directly.

The mesh’s local copy/reset path can provide the already allocated two-coordinate node array without a DB owner: local `OBJ_MESH2D::flags[0]` is 2, and its copy path allocates/copies the arrays with that table. This is consistent with the original rectangle builder reading two doubles per node. It qualifies a feasible static path; it does **not** prove that any selected real project contains a positive-count geometry mesh with non-null arrays.

## Exact remaining boundary and next action

The remaining original-behavior boundary is data reachability, not an unknown generic owner setter: static code supports the null-owner accessor path when coordinate/connectivity arrays already exist, but the retained corpus inventory does not demonstrate such a populated `GEO2D_AND_MESH2D::mesh` on a 3D viewer item. A positive count alone is insufficient. For absent coordinates, `0x140854200` takes the allocation branch and dereferences `dims[0]`; with a null owner dimension pointer, the clean coordinate-null return at `0x1408ee940` is not established and the path may fault before reaching that predicate branch.

Next discriminating action: statically inspect one retained, real 3D geometry-list mesh creator/copy record (or an immutable object/DB dump that preserves the optional mesh) and establish node count, element count, node `s008+8` pointer presence, and element `s148+8` pointer presence before the viewer calls the getters. If that record cannot be found, retain the current synthetic fixture solely as port unit coverage and keep original populated-mesh integration/parity unclaimed. Do not add a fake owner to the cross-section mesh or substitute the owning object’s main FE mesh.

No target code was executed. No source, build, GUI, REA session, or mutable database was modified.
