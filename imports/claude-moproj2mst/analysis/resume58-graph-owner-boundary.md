# Checkpoint 58 graph-owner boundary

## Existing source-qualified ownership model

The current source already contains the smallest supported manager/factory lifetime model; no production addition is justified by the remaining evidence.

- `DbHandle::postGraphManager` is a lazy shared cache corresponding to the model child at native `+0x1970`. `ensurePostGraphManager` verifies the cached manager's model owner, creates it only for the qualified Mo model/scene path, and stores it after construction/registration succeeds (`source/apps/moproj2mst/include/m2m/engine.hpp`; `source/apps/moproj2mst/src/engine/close_effects.cpp`).
- `PostGraphManagerState::factory_` is a unique owned child at native `+0x18`; `PostGraphFactoryState` carries a nonowning manager backlink and weak model backlink. Construction models the base factory then derived post factory replacement. Destruction resets the factory before graph-list traversal. This matches the bounded static manager contract's factory creation/replacement/teardown order.
- `PostGraphManagerState::graphItems_` is weak guarded identity at native `+0x10`, not an owning smart-pointer list. It has no insertion path yet. That correctly avoids attributing graph-object ownership to the manager registry while concrete graph construction/scene registration is unresolved.
- The factory destructor contract says the derived/base factory wrappers delete their factory objects and run QObject base destruction, with no application-level graph-list or scene mutation in those factory destructors. Native constructor evidence passes the manager as QObject parent; the source's explicit owned factory child is the supported lifetime projection for the manager cache, while full QObject parent teardown remains framework-qualified.

Static qualifications: `analysis/wave3-close-graph-manager-constructor-contract.json` records the exact root+`0x1970` cache, base/derived factory creation order, QObject parent arguments, and manager replacement. `analysis/wave3-close-graph-manager-cleanup-contract.json` records manager destruction deleting `+0x18` before traversing `+0x10` and treating entries as guarded pointees. Both refer to immutable original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Remaining edge after c15910

The bounded PE wrapper at `0x140c15910` returns the factory `+0x60` result unchanged. A static byte search over `.text` found no direct E8/E9 caller and no absolute function-pointer reference to `0x140c15910`; therefore the caller's RAX use is still unbound. The downstream factory chain (`manager+0x18` → factory `+0x60` → factory `+0x58` → graph constructor) proves graph creation only. It does not establish a scene add, manager+`0x10` insertion, or separate owner store.

The smallest next action is one code/data xref query to `0x140c15910` in the root-owned static REA analysis, including indirect references, followed only by the caller's bounded RAX flow. If the xref resolves a caller, identify whether it reaches `QGraphicsScene::addItem` (`0x141e08260`), an actual QPointer/list insertion helper for manager+`0x10`, or another owner field. If it does not resolve, preserve the open question and do not change scene ownership.

## Recommendation

Retain the existing `shared_ptr` manager cache, owned `factory_`, nonowning factory backlinks, and weak `graphItems_` registry. Do not add factory-created graph objects to `PostGraphSceneState::items_`, and do not add a manager-list insertion API until the missing caller and scene registration edge are established. The current `unique_ptr` scene projection remains neither validated nor refuted by the factory constructor/destructor contracts.
