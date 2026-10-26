# Checkpoint 75 histogram ownership evidence boundary

## Concrete object and scene/registry binders

The retained original PE is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Existing evidence binds `SfGraphHistogramItem` RTTI to its primary vftable, scalar deleting slot `+0x18` at `0x140c301e0`, and concrete destructor `0x140c2c600` (`resume49-graph-ownership.md`). Its constructor is `0x140c2b6b0`; the two retained callsites are inside factory routine `0x140c36bd0` (`resume49-histogram-constructor-callers.asm`, `resume50-histogram-construction-function.asm`).

Neither the constructor body nor the factory routine has a retained `QGraphicsScene::addItem` call, and the factory body returns the graph pointer through RAX. The known scene cleanup path enumerates `QGraphicsScene::items()` and dispatches by RTTI (`resume48-c0c650-scene-delete-bounded.asm`); this proves an item can be found in the scene during cleanup, not how it entered or who owns its lifetime. Separately, manager `+0x10` holds QPointer guard wrappers, and the cleanup path unlinks a matching guard before explicitly deleting a live graph item (`wave3-close-graph-manager-cleanup-contract.json`). This establishes a guarded registry/deletion sequence, not an owning manager container or the registration callsite for this histogram object.

The available `QGraphicsScene::addItem` sites around `0x140c2c560` and `0x140c30fd7` are not connected by retained pointer flow or RTTI proof to the factory result. They cannot be used as the missing binder.

## Destructor chain and its limit

The deleting virtual calls concrete destructor `0x140c2c600`, which processes application members, emits `signalHideSecondaryWidgetInSplitView`, then calls base destructor `0x140c38f40` (`resume49-histogram-destructor-chain.asm`). The bounded base body resets its vtables, destroys member fields, calls `0x140c1c690` on `this+0x10`, and finally calls imported `QObject::~QObject` (`resume49-histogram-base-chain.asm`). The retained body does not show an explicit `QGraphicsScene::removeItem` call or mutation of manager `+0x10`.

The effect of `0x140c1c690` on scene membership is not established by the retained captures. It may be a base/member teardown boundary, but assigning it a Qt class or claiming that it unregisters from the scene would exceed the current evidence. Consequently, the framework side of deletion is still unbound too.

## Narrow conclusion

The object has a concrete constructor and deleting-destructor chain, but there is no source-qualified scene insertion, manager-guard insertion, or other owner-store edge for the returned pointer. The destructor evidence does not settle implicit scene removal through its `+0x10` base/member call. The exact missing binder is: factory return → caller that adds the item to a scene and/or registers its QPointer → scene/base destructor membership effect.

Until those links are recovered, native scene ownership cannot be distinguished from external ownership with scene membership, and `PostGraphSceneState::items_` as `unique_ptr` remains unvalidated. Manager factory ownership and manager guard-list deletion behavior do not resolve this separate scene lifetime question.
