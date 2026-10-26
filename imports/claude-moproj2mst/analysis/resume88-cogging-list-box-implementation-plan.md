# CP88 plan: flag-0x2b geometry-list box union

## Scope and ownership

Wait until the CP87 `viewerRebuild` freeze is identified by root and use that
frozen revision as the implementation base. This plan assigns exactly one
small production hunk in `source/apps/moproj2mst/src/engine/viewer.cpp`, inside
`recompute3D`, plus focused coverage in
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`. Do not modify CMake,
public interfaces, `refreshGeometryItem`, `geometryMeshRect`, or Cogging Save
dispatch. This is a viewer cached-box implementation, not a Cogging writer
change.

## Native order to preserve

Retained native `0x1411f5540` order is: reset view box; union enabled geometry
rep; when flag `0x2b` and a nonempty item list, refresh item boxes and union
`0x1411f58e0`'s list result; union enabled FE mesh rep. In current
`recompute3D`, the middle path is an unconditional unsupported throw. The
small change should replace only that throw:

1. Iterate `view.items` in existing order.
2. Call existing `refreshGeometryItem(m, view, item)` for each item, matching
   the native `0x1411f5a90` refresh-before-aggregate order.
3. If the sync barrier became pending, preserve current barrier semantics and
   stop; do not consume an incomplete rectangle.
4. If `item.rectKnown` is false, throw the current explicit
   `M2M_E_UNSUPPORTED` message before treating that item as a box.
5. Convert the known item rectangle with existing `rectBox` and union into
   `view.box`; do not use `modelRect` or the main FE mesh as a substitute.
6. Continue to the flag-2 FE mesh branch only after the item list was fully
   processed, preserving the original geometry → item-list → mesh order.

The list union must include every item, not only item 0. Existing
`refreshGeometryItem` preserves empty/unbound and curve-only outcomes, computes
supplied populated 2D mesh rectangles under its existing qualification, and
marks unqualified populated mesh data unknown. Do not relax those conditions or
allow a partial edge prefix to stand for the complete item rectangle.

## Focused test plan

Keep tests in the existing `m2m.mdt.viewer_box` Testkit target. Set the actual
candidate settings bit using the existing
`engine::readViewerDisplayOptions(model, viewIndex, node)` API and a parsed
`<SfDispOpt><DispFlagList CrossSectionGeoOn='1'/></SfDispOpt>` node; the field
mapping is retained in `source/apps/moproj2mst/src/doe/view_stepdata.cpp`.
`engine::viewerBox(model, objectNumber, mn, mx)` is the right observable for
the per-view box consumed by Cogging's `0x1411f5040(..., false)` path.

Add one focused case, or a compact set of named cases, for these assertions:

- **Two known edge items:** create two bound geometry-list entries with
  disjoint edge rectangles, turn on flag `0x2b`, call `viewerRefresh`, and
  assert `viewerBox` contains the union of both entries. This proves the loop
  doesn't accidentally aggregate only item 0.
- **Known populated synthetic mesh:** use the existing four-node quad helper
  and qualified ownerless/preallocated arrays; with flag `0x2b` enabled,
  assert the per-view box includes the quad and edge bounds. Preserve existing
  getter/COW checks where practical. This tests the already-qualified
  candidate path, not original loaded-mesh lifecycle.
- **Unqualified positive mesh:** give an existing bound item positive counts
  with an already covered disallowed owner/registry profile, enable flag
  `0x2b`, and assert `viewerRefresh` still throws the exact unsupported
  diagnostic. Verify no out-of-profile getter dispatch or fabricated owner,
  registry, array, or flag changes.
- **Order guard:** where existing fixtures can express both geometry rep and
  FE mesh without changing production defaults, assert list bounds are unioned
  between them. If no minimally qualified fixture exists, rely on the local
  source order and explicitly do not add a synthetic dual-display mode.

The expected unsupported error remains for an item whose `rectKnown` is false;
all tests must use `REQUIRE`/`CHECK` and the current Testkit style. Do not assert
original Cogging file bytes or public Save parity.

## Acceptance gate

Root should first confirm the CP87 freeze identity and reserve the two-file
ownership. Then the implementation is acceptable only if:

- only the `recompute3D` flag-`0x2b` hunk and the assigned viewer-box test file
  change;
- iteration/refresh/union order matches the retained `0x1411f5a90` then
  `0x1411f58e0` contract and remains before FE mesh union;
- all known rectangles, including each list item, contribute to the per-view
  box, while unknown positive mesh state still refuses;
- focused `m2m.mdt.viewer_box` build and CTest pass, with the source diff and
  result receipt reviewed against the frozen base.

Even after those checks, label the result **candidate internal viewer-box
integration**. Public Cogging parity still requires evidence that an original
session enables flag `0x2b` for a Cogging view and has a native-ready optional
geometry mesh before the refresh; existing evidence says the flag defaults
off and the mesh owner/population lifecycle is unqualified.

This is a plan only. No source, build, test, GUI or mutable analysis database
was changed or accessed.
