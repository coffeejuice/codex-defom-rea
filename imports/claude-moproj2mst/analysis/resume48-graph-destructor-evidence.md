# Checkpoint 48 graph destructor/scene ownership evidence

## Evidence captured

A new bounded assembly view was generated from the retained immutable `evidence/original-binaries/DEF_GUI_MO_64.exe` using DUMPBIN only; no REA database, target runtime, build, GUI, or source file was touched. The binary SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The view is `analysis/resume48-c0c650-scene-delete-bounded.asm` (SHA-256 `599792e835b0eb2610c840c222abb7b2519ac91f2b3a9a9155ef0d3392c4d25b`), exact DUMPBIN range `[0x140c0c650, 0x140c0cb75)`.

The bounded code confirms `c0c650` obtains the scene's item collection and dispatches concrete RTTI matches. For directly deleted chart types it calls the object's virtual deleting slot `+0x18` with flag `1` (e.g. call sites `0x140c0c89a`, `0x140c0ca83`, `0x140c0cb25`). For graph types requiring manager coordination it instead calls `c15db0` (call sites `0x140c0c90e`, `0x140c0c95c`, `0x140c0c9aa`, `0x140c0ca01`) with the embedded key. The paired static contract `analysis/wave3-close-graph-manager-cleanup-contract.json` establishes that `c15db0` uses manager+`0x10` guarded entries, unlinks via `c170b0`, releases its temporary QPointer guard, then calls deleting virtual `+0x18`; it also records index increment after removal.

## Ownership conclusion

The available static evidence supports a two-role model, not one shared owning container: the scene supplies item identities and performs selective traversal, while manager+`0x10` is a QPointer-guarded lookup/cleanup registry that can explicitly request graph deletion after unlinking its own registry entry. The manager list is therefore nonowning in representation (guarded identity), even though manager cleanup may delete a live pointee. It is not safe to make manager entries owning smart pointers.

This evidence does **not** settle whether the current C++ scene `vector<unique_ptr<PostGraphSceneItemState>>` is compatible. `QGraphicsScene::items()` supplies a list result, and the native selected branches explicitly delete returned objects; the bounded view does not include the concrete object's full deleting destructor or prove how scene membership bookkeeping reacts to deletion while the returned list is being traversed. In particular it does not establish whether the scene's native collection is itself a lifetime owner in a way equivalent to `unique_ptr`, whether deleting a graph removes it from `items_` synchronously, or whether the original destructor reenters manager/scene cleanup. Do not conclude that the scene should use raw pointers or unique ownership solely from this helper call order.

## Exact next source evidence needed

For one representative `SfGraphHistogramItem` (or other manager-keyed graph), bind its RTTI type descriptor to the complete vtable, identify the scalar deleting destructor `+0x18` target, and capture the full destructor chain through `QGraphicsItem`/QObject-adjacent members. Search that bounded chain for calls that remove/unregister from its `QGraphicsScene`, alter manager+`0x10`, or delete nested graph payloads. Then trace the concrete constructor's scene insertion and manager list insertion separately. If destructor removes itself from scene, the scene model must support external deletion/removal during a traversal snapshot; if it does not, establish the framework ownership callback before selecting an owning C++ representation. The present evidence gives no basis to simulate those callbacks.

## Implementation implication

Keep `PostGraphManagerState::graphItems_` as a guarded nonowning collection conceptually, and preserve unlink-before-delete. Defer any change to `PostGraphSceneState::items_` ownership until the concrete destructor and insertion path are source-qualified. A future helper test can assert manager guard/list order but cannot certify scene lifetime until that destructor boundary is recovered.
