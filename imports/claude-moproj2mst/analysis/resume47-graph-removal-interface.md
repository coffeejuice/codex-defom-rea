# Checkpoint 47 graph-removal interface seam

## Native ownership and order

The static contracts identify two distinct ownership domains in `Close` cleanup:

1. `c0c650` receives a `QGraphicsScene`, traverses the scene's ordered `items()` result (not `clear()`), then dispatches by seven concrete RTTI types. Two types are directly deleted through virtual `+0x18`; four are passed to `c15db0` with their `+0xc0` name key; the remaining type handling is separately specified in `analysis/wave3-close-remove-graphs-contract.json`. The graph item is scene-owned for scene membership purposes, but name-selected graph lifetime is coordinated through the manager collection.
2. `c15db0` mutates manager+`0x10`, whose nodes contain separately allocated QPointer guards. `c170b0` detaches/unlinks the selected list node and returns a temporary guarded pointer; the temporary guard is released before deleting virtual `+0x18`. The original numeric index then advances. Manager teardown `c155d0` deletes its owned factory child before traversing remaining guarded graph entries; these are different ownership edges and must remain ordered.

Evidence is bounded offline PE/DUMPBIN against `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The exact list and destructor behavior is in `analysis/wave3-close-graph-manager-cleanup-contract.json`; concrete scene dispatch is in `analysis/wave3-close-remove-graphs-contract.json`. This establishes the captured static sequencing. Populated graph destructor callbacks, reentrancy, and full runtime scene traversal remain qualified; no runtime result should be inferred from these records.

## Existing state mapping

- `PostGraphSceneState::items_` is `vector<unique_ptr<PostGraphSceneItemState>>`: this owns scene item objects in this projection and is the likely host for real graph item objects after their constructors and destructors are recovered. Its current `removeGraphItems(PostGraphManagerState&)` only handles the known text-item nonmatch; it refuses any other concrete type.
- `PostGraphManagerState::graphItems_` is `vector<weak_ptr<PostGraphItemState>>`, exposed read-only via `graphItems()`. It models the manager's native `+0x10` guarded collection but has no insertion/removal API. Weak pointers approximate QPointer nulling only under current single-threaded/non-reentrant assumptions; they do not model per-node guard wrappers, shared QList detachment, or exact unlink/temporary-guard lifetime.
- `PostGraphManagerState::factory_` is a separate unique child. Destructor currently resets factory then clears the weak collection, matching only the known child-before-list traversal prefix; clearing weak entries does not implement graph deletion.
- `removeGraphItems(notifications, window)` resolves live manager receivers and actual primary/secondary scene owners in receiver order, then asks each scene to clean its items. The current binding types correctly keep constructor-cached scene identity distinct from the current global scene results.

## Recommended future interface seam (proposal, not implemented)

Keep the existing Close entrypoint and scene traversal boundary. Introduce the smallest private manager-list adapter in `close_effects.cpp` (or a narrowly scoped internal header only if tests need the type) rather than adding a public C ABI or changing the dispatcher:

- `PostGraphItemState` becomes the polymorphic graph-object owner contract only after each concrete constructor/destructor prefix is qualified. It needs `nameKey()` returning the native `+0xc0` key and `destroyNative()` for the recovered deleting-virtual effect; do not give the scene a second independent owning `unique_ptr` if the original destructor removes/deletes the scene item itself.
- Represent a manager list entry as a guarded identity object with an explicit weak pointee plus the operations `lockForTakeAt(index)`, `eraseAt(index)`, and `releaseTemporaryGuard()`. The `takeAt` operation must first retain a temporary live handle, erase the list entry, and return that handle. The caller then drops that temporary guard and only then invokes the object's recovered deleting destructor, matching `c170b0` then `c15db0`.
- Implement name removal as an index-based loop over the actual entry sequence, with `++index` on the match path after removal. Preserve the native ordering and possible shifted-entry skip; do not replace it with remove-if, erase-all, or a range loop whose invalidation semantics differ. Null guard entries take their source-qualified native cleanup branch.
- Let scene dispatch classify the recovered concrete types and either directly delete the two direct-delete classes or call the manager helper with the exact embedded key for the four manager-key classes. Scene list traversal and manager list mutation must remain distinct operations.

The seam is a design boundary only. Before coding, resolve whether graph destruction calls back into scene membership or manager state and whether QObject/Qt parent ownership adds a competing destruction edge. If the recovered concrete destructor directly deletes the scene item, scene traversal must use an iteration snapshot/guard arrangement matching native `QGraphicsScene::items()` rather than holding owning `unique_ptr` entries through deletion. Current `vector<unique_ptr<...>>` makes that consequence especially important: simply adding manager `takeAt` would risk double ownership or invalidating the live scene loop.

## Test boundary

A future `tests/mdt/test_graph_list.cpp` can test the manager helper without shared build-interface edits because `source/apps/moproj2mst/CMakeLists.txt` uses test globbing. It should use the exact helper actually called by production and assert guard acquisition, unlink-before-destroy, destructor observations, and native index advancement. Do not add a test that reimplements the algorithm or assert a final survivor set until the destructor/reentrancy trace establishes it. Original-first runtime characterization is still needed before widening acceptance from the current text-only/no-match branch.
