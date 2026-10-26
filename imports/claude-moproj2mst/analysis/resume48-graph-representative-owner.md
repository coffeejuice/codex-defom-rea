# Checkpoint 48 representative graph owner trace

## Result

Representative selected: `SfGraphHistogramItem`. The requested RTTI-to-deleting-destructor-to-scene-insertion chain is **not closed** by the retained evidence inspected in this bounded item. Available evidence establishes the class identity and two independent deletion call paths, but it does not bind this type's complete constructor/destructor to scene membership. I did not infer scene ownership semantics from the class name or from `QGraphicsScene::items()` alone.

## Verified static edges

The immutable target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- `analysis/wave3-close-remove-graphs-contract.json` identifies `.?AVSfGraphHistogramItem@@` at TypeDescriptor `0x143706c20` / RTTI test address `0x143706c10`, and states that `c0c650` passes the item's `+0xc0` key to `c15db0` for this type.
- `analysis/resume48-c0c650-scene-delete-bounded.asm` is the bounded static view `[0x140c0c650,0x140c0cb75)`. It shows the graph-type branch calling `c15db0`, while other matched chart branches call virtual deleting slot `+0x18` directly.
- `analysis/wave3-close-graph-manager-cleanup-contract.json` traces the manager-key path: `c15db0` uses manager+`0x10`; `c170b0` unlinks an entry and returns a temporary QPointer guard; `c15db0` releases that temporary guard before invoking the pointee's deleting virtual `+0x18`. It notes index advancement after a match/removal.
- `analysis/wave3-close-secondary-splitview-contract.json` records that `cd5490` queries `QGraphicsScene::items()`, skips non-null `parentItem()`, and returns a top-level `SfGraphHistogramItem` when found. This is scene lookup evidence, not a constructor or destructor trace.

## Ownership conclusion and limit

The manager collection is a guarded lookup/cleanup registry, not an owning smart-pointer list. It can request deletion of a live graph object after unlinking its registry entry. Scene membership is a separate domain: the original can find the same concrete object through QGraphicsScene and can ask the manager registry to delete it. This supports retaining weak/guarded manager references, but it does not choose the C++ owner for scene objects.

I did not find a retained contract binding `SfGraphHistogramItem`'s type descriptor to its vftable, the virtual `+0x18` target to a complete scalar deleting-destructor body, or the constructor path that inserts it into a `QGraphicsScene`. The bounded `c0c650` and `c15db0` bodies stop at virtual dispatch; they do not establish whether the destructor removes the object from scene storage, whether Qt parenting owns/deletes it, or whether deletion reenters manager code. Consequently neither `vector<unique_ptr<...>>` nor a raw/guard-only scene representation can yet be claimed equivalent.

## Required next evidence

Continue with the immutable PE only: bind the class RTTI to its vftable and slot `+0x18`, capture that deleting wrapper and destructor chain with explicit bounds, and locate a constructor/factory caller that inserts the resulting item into the scene. Record any scene-removal/unregistration call and its ordering relative to member destruction. If static bytes do not resolve these edges, keep the ownership question open for the distortion/REA specialist; this note does not authorize mutable REA access or runtime probing.
