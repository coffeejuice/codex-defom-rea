# CP81 Cogging refresh versus Save ordering

## Static order recovered

The bounded original PE path resolves the ordering at the Cogging operation
hook. `0x14075c260` enters `0x140762ec0(M, 1)`; `0x140762ec0` makes the three
geometry calls in this order:

1. `0x140766200(M,1)`, `(M,2)`, `(M,4)` for Cogging billet, die, manipulator
   geometry;
2. `0x140763e50(M)` for positioning;
3. `0x140762f30(M)` for manipulator object state.

This exact order is visible in the retained source-qualified caller map at
`source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` and independently in bounded
DUMPBIN range `0x140762ec0..0x140763090`. `0x140766200` reads the scene via
`0x140c82d50`, then calls `0x1411f5040(scene, outBox, false)`; the third
argument is zero. It does not recompute the view there.

The native refresh/recompute path is separate. `0x140c8e640` calls
`0x1411f5540` for eligible viewer objects, and `0x1411f5540` refreshes geometry
list items (`0x1411f5a90`) before aggregating their boxes when the per-view
geometry-list display flag is set. The retained V contract (sections 3.1, 7)
lists operation loaders' bind/refresh calls through `0x140c80280` and
`0x140c7ff80`; it also lists Save-time calls to `0x140763e50` and
`0x140762f30` as viewer-sync paths. Those two functions run after the
`0x140766200` Cogging geometry extraction in the hook, so they cannot refresh
the scene box before that extraction on this call path. They may refresh state
for later consumers.

Thus static evidence qualifies the ordering rule: Cogging reads the current
cached box before its post-geometry positioning/viewer-sync steps. Its cache
was last bound/refreshed earlier (normally during loading or another caller),
but this bounded call does not prove the cache's freshness or which refresh
was last for a particular session.

## Geometry-list mesh consequence and limit

The optional mesh item contribution can enter `view+0xd8` only on the earlier
`0x1411f5540` refresh path when the setting enables geometry-list items and
the item refresh/readiness/builder chain succeeds. The retained
`source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` states display flag 43 (settings
byte `0x2b`) defaults to zero for the normal path. The retained V disassembly
states the refresh's item-list aggregation branch is guarded by that flag and
nonempty list. Therefore the new bounded evidence does not establish that an
ordinary Cogging operation has this contribution enabled. It also does not
resolve real optional-mesh node owner/array population; see
`analysis/resume53-geometry3d-ownership.md`.

This narrows the earlier CP81 gap: there is no direct missing Cogging Save
refresh call to implement. The remaining question is whether a real supported
session reaches the mesh-enabled cached state before the Cogging operation
hook, and whether that mesh reaches native readiness. Do not use a synthetic
candidate mesh test as proof of original Save behavior.

## Smallest runtime witness if static evidence is insufficient

If an original comparison becomes available, observe a single disposable
Cogging operation from its last view bind/refresh through the Save hook. Capture
only the identities and values needed to tie the cache to the same model:

- model, viewer and view addresses/identities at refresh and at
  `0x140766200`'s `0x140c82d50` lookup;
- settings flag 1 and flag `0x2b` for that view;
- geometry-list item count and bound item identity;
- optional mesh pointer at `GEO2D_AND_MESH2D + 0x28`, positive counts,
  coordinate/connectivity getter return-nullness, and node owner/registry
  profile at readiness;
- cached `view+0xd8` box immediately after refresh and box returned by
  `0x1411f5040(..., false)` inside Cogging Save.

The witness must not assert guessed pointer contents or dereference an
unqualified owner. It should establish whether refresh precedes Save for the
same viewer and whether the mesh contribution actually appears in the box.
If flag `0x2b` stays zero or the item has no optional mesh, this is not a
positive-mesh reproduction.

Target and tool identities are those recorded in
`analysis/resume81-cogging-mesh-pe.md`. This pass used read-only bounded PE
inspection and retained source receipts only; no GUI, source, build, test or
mutable analysis DB action was performed.
