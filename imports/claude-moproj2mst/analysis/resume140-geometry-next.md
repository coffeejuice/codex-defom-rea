# CP140: zero-width populated mesh still contributes its coordinates

The smallest source-closed successor is to admit element width0 for the already-qualified populated packed-XY geometry profile. Keep positive node/element counts, actual ownership/getter qualifications, N metadata2, E metadata2/3, nonnull returned pointers, native triple-count bound and finite coordinates. Native width0 performs no connectivity loads but still constructs vertices and computes their rectangle. Negative width and unresolved storage profiles remain guarded.

This is a private Viewer rectangle contribution, not a public file-loader producer or complete display-vector reconstruction. This read-only lane wrote only this report; no source/build/test execution, GUI/original execution, or mutable REA occurred. Proposed exclusive source pair remains `src/engine/viewer.cpp` / `tests/mdt/test_viewer_box.cpp`, plus an implementation receipt, if assigned by root.

## Immutable evidence

Rehashed original `evidence/original-binaries/DEF_GUI_MO_64.exe`: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded offline DUMPBIN and PE byte reads supplement CP139's complete builder, initializer and actual virtual rectangle target. No shared REA session was accessed.

| Original half-open byte range | Purpose | SHA-256 |
| --- | --- | --- |
| `[14124bf52,14124bfce)` | Inner connectivity loop and exit | `fd1b2c4c0b1f6578d4c3fa31ebb30c68b0d32d8084e005385a80dc88860abf7b` |
| `[140452660,1404526ee)` | Integer vector resize | `8f1a8bbad9e049bda2ae0545e246e80e626298ee1ad7ca20a37d8399ea4a70ab` |
| `[1404526f0,140452762)` | Resize shrink/erase helper | `41375d6f76bfd60000a4ec6ef06b15f6fa7e75b676b52a8b5449f8bf9dcc0fc3` |
| `[140452770,1404527f8)` | Append helper with zero-count early return | `003d61c33877744fa5ba0a85fe28fc6fb6c984898221b8a2a09eb6d80a1b9787` |

CP139's `resume139-geometry-next.md` qualifies the full `[14124bec0,14124c14b)` builder, `[14124c1e0,14124c291)` initializer and actual `[14124c3d0,14124c462)` rectangle finalizer. Its builder hash is `2ffda9258e62250966b1df188f1e175addde74745b04c0f584eb716a3acc447e`; finalizer hash is `d6bf56997e9743aa344e83bad8090d70ac92d5294620c0d65c6a32a9f119e5f5`.

## Native branch and owner order

The first predicate `1408ee940` requires positive node/element counts and calls coordinate then connectivity getters. Null connectivity returns false before builder width is read: that remains the known edge-only skip. Width0 does not substitute for a nonnull connectivity pointer.

For a true predicate, `14126d0c0` reads node count (`d29b`), element count (`d2a9`), width (`d2b7`), performs the second coordinate/connectivity getters (`d2c5`, `d2d4`) and invokes builder `14124bec0` at `d305`. All actual owner/reader/cache/generation qualifications stay unchanged, including CP137's two real same-step high/cache0/null-companion reader rounds.

Initializer `14124c1e0` stores width/counts and resizes integer vector+198 to elementCount*width (`c23c..c257`). With width0, the low32 product is0 without overflow. Resize `140452660` compares requested0 to current size: a nonempty vector takes its shrink helper and sets its end to begin; an empty vector computes append count0. Append `140452770` tests that count at `2788`, jumps to `27ee`, and returns at `27f7` without insertion. These are normal container paths. Existing projection code does not expose the representation's private vector, so no new candidate container owner should be invented.

In the builder, each element initializes j=0 at `14124bf52`; `bf67..bf76` compares j with stored width and branches to the next element when j>=width. Width0 therefore skips the load at `bf97`, decrement and destination write. The outer element loop has no other contribution. After it ends, the builder executes the same constant color setup and packed XY coordinate conversion as any positive-width case.

Actual virtual +48 resolves to `14124c3d0`, qualified by the constructor/vtable edge in CP139. It resets rectangle+0b0, loops all nodeCount entries, extends from the float pair at `3*i+1`, and finalizes. It never reads width, connectivity or source element metadata. Thus positive nodeCount and width0 still contribute all finite XY nodes, rather than only the sampled geometry edges. Original coordinate pointer retention at representation+218 follows the finalizer; this private projection does not claim to reconstruct that lifetime or rendering state.

## Exact smallest source cut

In `geometryMeshRect`, change only width rejection from `width <= 0` to `width < 0`. Qualify the connectivity product overflow division with `width > 0` before evaluating `INT32_MAX / width`; do not divide by zero. The native no-wrap product is then0 for width0, so `connectivityCount=elementCount*width` remains defined and its validation loop naturally runs zero times. Retain the nonnull pointer guard and `nodeCount <= INT32_MAX/3` before coordinate traversal. Do not relax N/E metadata, negative widths, nonfinite values, connectivity indices for positive widths, or any owner/reader barrier.

Keep guard placement after both getter rounds and the existing count snapshots. On width0 success, union the current float-rounded XY rectangle with sampled edges. Later node/layout/count/finite failures retain completed getter/timer/edge prefix effects and unknown completion. No getter replay, synthetic cache1, query, new frame/resume, generic raw-pointer admission or public interface is required.

## Focused tests and existing expectation changes

- Use metadata2/3 across ownerless, actual cached and actual high/cache0/null-companion profiles. With allocated nonnull connectivity and width0, expect mesh height1 (not the edge-only height0), unchanged arrays/owners/version/cache and two getter dispatches. Null readers retain second dispatch after first Stop, two arrays checkpoints and final node count3.
- Exercise width0 -> width4 -> width0 on the same live item while changing an XY coordinate, proving fresh geometry contribution and stable owner/generation. A width0 mesh with invalid *contents* in its valid allocated connectivity storage should still contribute, since no entries are consumed. The same contents under positive width must remain unknown until restored. Do not introduce fake pointers or protected-memory fixtures.
- Preserve NULL-connectivity+width0 readiness skip, with one coordinate round and sampled edges only. Existing `ownerless_geometry_connectivity_null_skips_mesh_and_preserves_edge_bounds` already exercises this distinction.
- Preserve negative width, N dimension3, unsupported E metadata, nonfinite/float-overflow coordinates and triple-product count refusal after already-completed getters. Use a fresh width0 case to verify each relevant failure/restoration order.
- Two existing malformed-width cases deliberately set width0: `populated_geometry_mesh_rejects_unqualified_arrays_and_dimensions` and CP139's `geometry_element_metadata_three_keeps_later_layout_and_data_guards`. Change those inputs to -1 to preserve their genuine unsupported-width purpose; do not simply weaken their expected refusal. Keep the separate NULL-connectivity+width0 input unchanged.

Target remains `m2m_mdt_viewer_box`, focused CTest `m2m.mdt.viewer_box`. No implementation or validation result is claimed. This report is source-ready for the stated rectangle projection; it does not establish a new public loader path or authorize populated DB/assignment/3D-node continuation.
