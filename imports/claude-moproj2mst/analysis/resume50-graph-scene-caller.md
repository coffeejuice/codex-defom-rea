# Checkpoint 50 post-construction scene ownership trace

## Caller boundary

The immutable PE runtime-function table places the constructor-caller routine at `0x140c36bd0` through `0x140c3712d`; the bounded direct disassembly is `analysis/resume50-histogram-construction-function.asm` (`[0x140c36bd0,0x140c3712e)`). This function contains two calls to the bound `SfGraphHistogramItem` constructor `0x140c2b6b0`, at `0x140c36d9d` and `0x140c36e55`. Each returned pointer is stored in a local result slot. Both paths join at `0x140c3711f`, return that result in RAX, and exit at `0x140c3712d`.

The function body has no call to the imported `QGraphicsScene::addItem` slot (`0x141e08260`), no direct manager `+0x10` list insertion, and no explicit owner wrapper transfer in the captured interval. It appears to be a construction/factory routine returning a graph item pointer to an outer caller; its external caller is not a direct relative CALL in `.text`, so the next ownership transfer may be indirect (for example through a virtual factory interface). I did not label the containing class or invent an owner based on the nearby function-pointer data.

## What this resolves

The concrete object can be constructed and returned by `0x140c36bd0`; the constructor itself does not attach it to a `QGraphicsScene` in the retained body. Its manager list is populated somewhere outside this captured factory path. Previously bound cleanup shows that the same runtime identity is discoverable from `QGraphicsScene::items()` and is separately addressable through manager+`0x10` guarded entries. This is consistent with a scene-managed item plus a nonowning manager registry, but it does not prove the lifetime owner.

The deleting wrapper `0x140c301e0` still dispatches concrete destructor `0x140c2c600`. That destructor emits HideSecondary, destroys its fields, and calls base `0x140c38f40`; the bounded body has no explicit `QGraphicsScene::removeItem` call. Automatic removal during QGraphicsItem base destruction remains framework behavior and is not established by this application-code capture. Therefore `PostGraphSceneState::items_` `unique_ptr` compatibility remains undecided.

## Exact next link

Resolve the indirect caller of `0x140c36bd0` from its vftable/function-pointer table and capture the caller after this result returns. Follow the returned pointer to any `QGraphicsScene::addItem` call, manager guard insertion, or other owner store. Then qualify the framework base destructor callback that removes the item from the scene. Only those two links together decide whether the scene adapter owns lifetime directly or mirrors a framework-owned raw-item collection.

## Evidence identity

All captures use immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No REA session/database, executable process, build, application control, or source files were used.
