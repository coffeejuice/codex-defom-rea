# Checkpoint 53 retained-disassembly graph consumer trace

## Result from available artifacts

The retained files do not bind an owner-consuming caller after the histogram factory returns. They do establish this chain:

1. The `SfPstGraphFactory` vtable starts at `0x14284d0c8` (constructor contract `analysis/wave3-close-graph-manager-constructor-contract.json`, vtable value `0x14284d0c8`). Its slot `+0x58` at `0x14284d120` contains `0x140c36bd0`, the routine that constructs `SfGraphHistogramItem` and returns the pointer in RAX (`analysis/resume50-histogram-construction-function.asm`).
2. Slot `+0x60` at `0x14284d128` contains `0x140c36b80` (`analysis/resume51-factory-virtual-call.asm`). That wrapper resolves an object with `0x1412c0730`, invokes the object vtable's `+0x58` slot, and returns its RAX unchanged. This is the only retained next hop from the factory-wrapper entry.
3. The `c36bd0` constructor routine and the `c36b80` wrapper have no direct `QGraphicsScene::addItem` import call and no manager+`0x10` list insertion in their bounded bodies. No retained disassembly includes the indirect caller that invokes the `SfPstGraphFactory` `+0x60` method and then consumes its returned RAX.

The target identity for the static contracts is immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No mutable REA session or database was opened for this item.

## Ownership inference ceiling

The manager's `+0x10` graph list remains a guarded registry from the independently retained `c15db0/c170b0` contract; it removes a guard entry before deleting a live pointee. The scene independently discovers the same type through `QGraphicsScene::items()`. The factory chain produces a graph object but the available consumer edge is absent. Therefore no evidence here decides whether the scene's adapter should own the object with `unique_ptr`, mirror framework-owned items, or use another lifetime model. The fact that the manager calls a deleting destructor does not make its guard list owning, and an absent `addItem` in the factory routine does not establish that no later caller inserts it.

## Exact one-hop query for the root-owned REA pass

Query code/data references to the concrete vtable slot address `0x14284d128` (the `SfPstGraphFactory` `+0x60` entry pointing to `0x140c36b80`) in the retained binary's active immutable analysis, then inspect the containing caller function(s) only. This is the missing dispatch edge; the next bounded function view should begin at the indirect `+0x60` call and track returned RAX through the nearest owner write/call. Specifically determine whether it reaches IAT `0x141e08260` (`QGraphicsScene::addItem`), a guard/list insertion helper for manager+`0x10`, or another owner store. Stop at that one hop if the caller is still ambiguous; do not broaden to unrelated graph implementations.

This query does not mutate REA state by itself and fits the root-owned analysis boundary. It should provide enough to decide the next narrow probe without guessing ownership.
