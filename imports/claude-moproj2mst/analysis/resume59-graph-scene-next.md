# Checkpoint 59 graph scene ownership next step

## Retained caller evidence

The retained static files expose one manager-to-factory leg but no incoming caller to that manager wrapper:

- `analysis/resume56-graph-manager-wrapper.asm` bounds `c15910` (`[0x140c15910,0x140c15963)`). It checks manager+`0x18`, dispatches factory virtual `+0x60`, and returns the virtual result unchanged in RAX.
- `analysis/resume51-factory-virtual-call.asm` bounds `c36b80`, which dispatches factory virtual `+0x58` and returns that result unchanged.
- `analysis/resume50-histogram-construction-function.asm` bounds `c36bd0`, where `SfGraphHistogramItem` is constructed and its pointer is returned.
- `analysis/resume56-graph-manager-caller.md` records a whole `.text` byte search with no direct E8/E9 call or absolute-VA reference to c15910. No other retained capture identifies an indirect caller and post-return owner write. A scan of generic `+0x60` virtual calls does not prove receiver type without the caller's receiver/vtable dataflow.

Thus the bounded known chain ends at: model's cached manager+`0x18` factory → factory `+0x60` → factory `+0x58` → graph item constructor → returned pointer. It demonstrates construction, not scene insertion or object lifetime ownership.

## Factory/list destruction evidence

`analysis/wave3-close-graph-manager-constructor-contract.json` establishes the manager-owned factory lifetime: native manager construction starts with `+0x18=null`, constructs the base factory, constructs the derived post factory, then deletes/replaces the base child in native order. The factory is QObject-parented to the manager and also cached in `+0x18`.

`analysis/wave3-close-graph-manager-cleanup-contract.json` establishes manager teardown order: delete nonnull factory `+0x18` first, then traverse graph entries at `+0x10`; entries are QPointer wrappers, not pointee owners. It records that live pointees may nevertheless be deleted after their registry entry is unlinked, and leaves populated destructor/reentrant effects qualified. These are distinct edges: owning the factory child does not make the guarded graph collection owning.

## Smallest concrete next step

Run one root-owned, read-only code/data xref query for `0x140c15910` in the active static analysis, including indirect/table references. If a caller resolves, capture only that caller through its first use of returned RAX and classify the observed edge (QGraphicsScene::addItem, manager+`0x10` QPointer insertion, another owner field, or pass-through). If no xref resolves, retain the open unknown and do not widen the source model.

This is the smallest step because every already retained bounded body ends at a return or an internal factory dispatch; no additional hand scan of unrelated `+0x60` call instructions can identify the receiver safely. No scene ownership conclusion follows from factory ownership, from manager teardown order, or from the existing manager's nonowning guard list.
