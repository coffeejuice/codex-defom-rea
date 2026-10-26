# Checkpoint 56 geometry-mesh increment and remaining boundary

## Decision

The smallest source-qualified populated mesh rectangle branch is already implemented in the assigned files; no further source edit is justified by this read-only follow-up:

- `source/apps/moproj2mst/src/engine/viewer.cpp::geometryMeshRect` reads the geometry-list mesh’s recovered node/element subobjects directly, requires 2D dimension flags, positive width, non-null data pointers, checked signed products, valid one-based connectivity, and finite double/float coordinates. It narrows each two-component node point to floats and builds a temporary rectangle over all nodes before unioning it into the geometry item. It does not ask for or fabricate a node-set owner.
- `refreshGeometryItem` leaves an empty mesh contribution known, and marks a positive but malformed/unqualified contribution unknown so the existing `viewerSceneBox` refusal still bounds that state.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` has a four-node, one-element, width-4 synthetic mesh with exactly eight coordinate doubles and four one-based connectivity integers. Its `calloc` allocations are released by the existing node/element set destructors. The model/view is 3D, while the independent geometry mesh retains its 2D set flags. Separate negative cases retain the unqualified refusal.

The geometry of this increment is source-supported: original `0x14124bec0` allocates connectivity by element count and supplied width, subtracts one from source indices, reads two doubles per node, converts them to floats, and derives the rectangle over node positions. The latest static receipt in `analysis/resume56-geometry3d-node-owner.md` explains the no-owner accessor path when arrays are already populated. The test provides those arrays explicitly. It does not infer that real corpus projects necessarily contain such a mesh.

## What this increment does not authorize

Keep these cases on the existing unknown/refusal boundary: missing positive-count arrays, non-2D set flags, non-positive width, overflowing counts, out-of-range connectivity, non-finite coordinates, and any case whose storage length cannot be justified by its mesh counts. Do not allocate coordinates through the owner-derived getter for a standalone geometry mesh; with absent coordinates and null node owner, original `0x140854200` would dereference a null dimension table before the predicate’s later null-result check is established. Do not add behavior for a real loaded 3D project, malformed native arrays, arbitrary dimensional layouts, or original parity without a qualifying fixture/runtime receipt.

## Next executable gate

The next action is root validation of the focused `m2m.mdt.viewer_box` target and its new malformed/valid cases. Review the resulting test receipt and source diff, then keep original populated-mesh parity marked unproven until a retained real 3D geometry-list mesh is shown to have positive counts and non-null node/connectivity arrays before its original getters. No test/build was run by this worker.

No source changes were made during this follow-up; the two implementation/test files named above are the existing assigned increment. This report is the only new file.
