# CP88 flag-0x2b list-box source review

## Decision: one ordering blocker

The known-box union and unknown-item refusal are scoped to the intended `recompute3D` list branch, but the implementation does not preserve the documented native refresh/aggregate ordering. The retained native pseudocode in `source/docs/re/moproj2mst/mdt/V/spec_V.md:340-342` calls `1411f5a90(view)` to refresh **each item in the entire list**, then calls `1411f58e0(view, tmp, 0)` once to union the list's boxes. The implementation instead interleaves refresh and aggregate:

```cpp
for (auto& item : view.items) {
    refreshGeometryItem(...);
    ...
    view.box = boxUnion(view.box, rectBox(item.rect));
}
```

This contradicts both the source comment immediately above the loop (“refreshes the entire item list before ... unions”) and the original-function order in the spec. It also means a later item's refresh may run after earlier item boxes have already been committed to `view.box`; on a later unknown item/refusal or a pending transition, that differs from the native two-phase flow. The three new tests validate final ordinary unions and the single-item unknown path, so they do not expose this ordering difference.

**Smallest correction:** use a first pass to call `refreshGeometryItem` for each item in order, returning on a newly pending barrier; then a second pass to reject any `!rectKnown` item and union every `rectBox(item.rect)` into `view.box`. Keep the complete list stage before the existing FE mesh branch. This matches `1411f5a90` followed by `1411f58e0`, preserves the barrier return before consuming an incomplete current item, and leaves the explicit unsupported diagnostic unchanged. A minimal order regression can put an unknown positive-count mesh in an early list item and a later known edge item whose refreshed rect can be inspected after refusal; alternatively the static check should require distinct refresh and aggregate loops.

## Other checks

- The `recompute3D` signature/call change is local to `viewer.cpp`; the known-list branch remains between geometry-rep and FE-mesh contributions.
- Each current item is refreshed before its own `rectKnown` check; unknown positive-count mesh still throws the established `M2M_E_UNSUPPORTED` diagnostic.
- The focused cases set flag 0x2b through `CrossSectionGeoOn`, verify two edge items' combined extents, verify a qualified synthetic mesh contribution, and verify unqualified positive-count mesh refusal without getter dispatch or owner/array changes.
- Scope receipts match the frozen implementation report: `viewer.cpp` SHA-256 `b3415bc2f19e5210cd80b0767e7c096d042cd755cb2cc1b93f481feff2ff0e67`; `test_viewer_box.cpp` SHA-256 `259fade35e08870b575d804f3393fb82d8390da8e39df23ab02fb87a5fac4ccb`.

No source edits, builds, or tests were made. Root owns the disposition and any follow-up change.

## CP88 two-pass correction re-review

After the correction was released, the production path now matches the retained
native sequencing: it refreshes every item first, checks the pending state
before aggregation, validates the complete list, then performs the item-box
unions before entering the FE mesh branch (`viewer.cpp:939-959`). This removes
the original ordering blocker. The new regression uses an earlier known edge
item followed by a later unqualified populated mesh and verifies refusal leaves
the per-view box empty, with no getter dispatch and unchanged mesh arrays/owner
(`test_viewer_box.cpp:699-732`). The existing two-edge and qualified-mesh tests
continue to cover successful aggregate results.

The pending check is after the refresh loop rather than inside it. If an item
refresh makes the barrier pending, each remaining `refreshGeometryItem` call
returns immediately at its own leading pending check; no later item is
refreshed, unioned, or otherwise changed. I found no observable effect from
finishing those no-op iterations under the stated single-threaded/no-reentrant
contract. An immediate in-loop pending return would state the stop rule more
directly, but it is not a blocker for the frozen correction.

Current hashes match the correction receipt: `viewer.cpp`
`b7baa6c3046c5fc288a7f8b57ea80ced61314529d0b9d46ac2af885ba988da74` and
`test_viewer_box.cpp`
`89f8dfcd24337f27413f7fef7ee4740706281e12fe971c10af0b56dedc9ae05f`.
The source diff remains limited to the assigned viewer function and focused
tests. Final CP88 review decision: no blocker after the two-pass correction.
This was read-only; no builds or tests were run.
