# Checkpoint 48 histogram graph lifetime trace

## Status

The requested concrete chain remains unresolved in the retained static evidence. The representative class is `SfGraphHistogramItem`; its class identity, scene lookup, scene cleanup dispatch, and manager-list deletion route are established, but its concrete vftable slot `+0x18`, full deleting-destructor chain, and insertion constructor/caller were not bound here. I will not infer these from nearby RTTI records or class-name strings.

## Bounded receipts and verified links

Original binary: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- `analysis/resume48-c0c650-scene-delete-bounded.asm` covers `[0x140c0c650,0x140c0cb75)`. `c0c650` gets the scene item sequence and RTTI-dispatches it. The `SfGraphHistogramItem` path calls `c15db0` with item+`0xc0`; direct chart-type paths call virtual deleting slot `+0x18` themselves.
- `analysis/resume48-c15db0-graph-delete-bounded.asm` covers `[0x140c15db0,0x140c15ef7)`. On a name match, `c15db0` calls `c170b0` on manager+`0x10`, transfers the returned guarded object pointer to a local, destroys/releases the temporary QPointer wrapper, then dispatches virtual slot `+0x18` with flag `1` if the guarded pointer remains nonnull. It increments the loop index on the common loop edge after that path.
- `analysis/wave3-close-remove-graphs-contract.json` ties the RTTI identity `.?AVSfGraphHistogramItem@@` to TypeDescriptor `0x143706c20` and the manager-key route.
- `analysis/wave3-close-secondary-splitview-contract.json` shows `cd5490` can find a top-level object of this type from `QGraphicsScene::items()`. This supports scene discoverability, but is not a constructor/insertion trace.

Both asm files are direct bounded DUMPBIN output from the immutable retained PE. No REA state, application process, or source/build files were used.

## Ownership implication

The manager's list is a guarded registry, not ownership by smart pointer. It intentionally deletes a live pointee after unlinking the guarded entry. The scene also enumerates that concrete object, so the scene and manager refer to the same runtime identity. These facts still do not prove which object owns lifetime: a destructor may unregister from scene membership, Qt may remove the item when destroyed, or a different owner may manage it. The bounded bodies terminate at virtual dispatch and cannot answer that question.

Therefore the existing C++ `PostGraphSceneState::items_` `unique_ptr` representation cannot be either accepted or rejected on this evidence alone. Adding manager-driven deletion on top of it without a destructor/removal callback could double-delete or leave stale scene entries; replacing it with raw pointers would invent lifetime behavior.

## Exact missing links

The next immutable-PE pass must bind the `SfGraphHistogramItem` TypeDescriptor to its COL/vftable and locate vftable slot `+0x18`; inspect that target and direct base/member destructor calls; then find a constructor/factory path that invokes `QGraphicsScene::addItem` or equivalent and registers a QPointer in manager+`0x10`. The PE has multiple occurrences of the type string due to RTTI, Qt metadata, and string literals, so a matching string or RTTI dynamic-cast alone is insufficient to select a constructor/destructor. Until these links are established, keep scene ownership explicitly unresolved and do not widen graph acceptance.
