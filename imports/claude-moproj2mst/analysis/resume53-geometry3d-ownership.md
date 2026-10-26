# Original 3D geometry-list mesh ownership and guard boundary

## Ownership is split across two levels

The original geometry-list mesh is the optional `OBJ_MESH2D*` stored at `GEO2D_AND_MESH2D + 0x28`. The cross-section owns that allocation: its constructor initializes `mesh=null`; reset/clear calls `delete_mesh`; and `delete_mesh` invokes the mesh scalar deleting destructor before nulling the pointer. Its copy path allocates `0x1700` bytes, constructs a new `OBJ_MESH2D`, then copies the mesh’s node and element sets. These facts are source-qualified in `source/port/defmodel/src/geo/geo2d_and_mesh2d.cpp`, `source/port/defmodel/src/obj_mesh2d/obj_mesh2d.cpp`, and `source/port/defmodel/include/defmodel/layout/geo2d_and_mesh2d.hpp`.

That establishes lifetime ownership by the geometry cross-section. It does **not** establish a database/model owner for the nested node state set at `OBJ_MESH2D + 0x008 + 0x130`. The retained `OBJ_MESH2D` constructor initializes that owner field to null; reset puts the node and element dimension flags at 2. The retained copy implementation copies arrays using `OBJ_MESH2D::flags`, but does not set the node set’s `u130` owner pointer. Do not conflate `GEO2D_AND_MESH2D::mesh` ownership with `Cls_14017ef20::u130` ownership.

## Original guard reached by a bound 3D geometry item

The static call chain is `0x14126d0c0` (geometry-item virtual refresh) → `0x1408ee940` (mesh-ready predicate) → `0x14124bec0` (item rectangle builder). `item2.c` shows the geometry-list resolver returning a `GEO2D_AND_MESH2D` instance and then passing its `+0x28` mesh into the getters. `analysis/wave2-geometry-14126d0c0-disassembly.txt` independently shows the predicate call and subsequent calls to node count, element count, width, coordinate getter, connectivity getter, and builder.

`evidence/deform-rea-work/out/mdtV/item3.c` retains the exact `0x1408ee940` order:

1. `geometry + 0x28` mesh pointer must be non-null.
2. `0x1400c3570(mesh)` node count must be at least 1.
3. `0x1400c3590(mesh)` element count must be at least 1.
4. **Next guard after the positive counts:** coordinate getter `0x140514f00(mesh)` must return non-null.
5. Then connectivity getter `0x140894330(mesh)` must return non-null.
6. Only when both getters return non-null does the predicate return true and `0x14126d0c0` proceed to `0x14124bec0`.

The raw wrapper bytes further qualify the accessor boundary without loading the target: `0x140514f00` passes `mesh + 0x008` to `0x140854af0`; `0x140894330` passes `mesh + 0x148` to `0x140857ed0`. The node-count wrapper reaches the `+0x12c` field; the element-count wrapper reaches `+0xe8`. Thus the existing count-only test does not establish that the native builder is reachable; its missing coordinate and connectivity arrays fail later readiness conditions, even though the counts are positive.

## Owner/dimension ambiguity still open in original behavior

The raw coordinate wrapper’s downstream dimension lookup is an important reason not to infer native reachability from a hand-built mesh. Bounded original PE disassembly at `0x140853240` shows it checks node-set `+0x130`, returns null when that field is null, and otherwise returns owner `+0x15f8`. `0x140854af0` obtains this result and passes it to the node-array helper `0x140854200`. The current port’s equivalent source path has the same nullable owner-derived dimension contract (`fun_14017f020` → `fun_1401808d0` → `node_set_alloc`). The cross-section’s ownership of the mesh does not by itself fill this node-set owner field.

The retained static evidence does not identify a geometry-mesh-specific write that makes `OBJ_MESH2D::s008.u130` non-null before `0x140514f00`, nor does it show that the complete helper chain safely accepts null dimensions for an already populated set. The copy code and default dimension flags are not proof of that behavior. Keep this as a residual unknown; do not claim that the four-node synthetic viewer fixture exercises original getter readiness, even though it now supplies non-null coordinate and connectivity arrays for the port’s raw-layout implementation.

## Smallest discriminating next action

Make a bounded, immutable static capture around the original creator/copy/read path that writes `OBJ_MESH2D + 0x008 + 0x130` (and its matching `u138` dimension setter), then connect that writer to the `GEO2D_AND_MESH2D::mesh` creation/copy path using exact xrefs. In parallel, finish the short bounded body trace of `0x140854200` and its state-variable helper only far enough to decide what the native getter does when its dimension argument is null and `u008` already contains data. The discriminating result is:

- if the mesh writer attaches a model owner, record the owner type and `u15f8[0]` value at the geometry-mesh boundary; determine whether the 2-coordinate builder inputs are separately packed before claiming they can use the model’s dimension-sized buffer;
- if no writer exists and the native getter tolerates null dimensions for preallocated arrays, qualify that exact path and retain non-null coordinate/connectivity return guards;
- if neither can be established statically, stop at the coordinate-getter-null guard and keep original-runtime/populated-mesh behavior unclaimed.

This next action is independent of GUI-control availability and does not require a corpus claim. The copied 3D KEY scan in `analysis/resume47-geometry-mesh-fixtures.md` remains qualified: primary `DIEGEO` surfaces are `OBJ_GEO3D`, not evidence of a populated optional cross-section `OBJ_MESH2D`. No original GUI behavior or mutable REA database was used here.
