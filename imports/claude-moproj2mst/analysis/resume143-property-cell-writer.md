# CP143: property-table cell-writer xref boundary

## Finding

The retained PE gives a concrete post-construction **candidate updater**, but the available decompilation does not prove it writes cells or resolve the derived `slotItemChanged` body. No populated-cell transition is source-closed yet. Keep the refusal and use the addresses below as the next bounded native query.

## Newly bound call path

The original binary `evidence/gui_mo/DEF_GUI_MO_64.exe` hashes to `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- `SfObjectManager` vtable `0x142a3f748` has slot `+0x140 = 0x140d78220` (`evidence/gui_mo/s1/vtables.tsv`). The `.pdata` range for this slot target is `0x140d78220..0x140d7848a`.
- Native `0x140c80a20` and `0x140c80b00` both call the lazy `0x140c90210` manager accessor, then dispatch `manager->vtable+0x140(manager, model)`. Their saved decompiles are `evidence/gui_mo/s1/mdt/DB/d3.c:421-477` and the same bodies in `DB/d3s.c`. This binds a real same-owner caller to slot `0x140d78220`; it is stronger than inferring ownership from the finalizer alone.
- A read-only scan of the retained `.text` bytes for direct `E8 rel32` calls confirms `0x140c80a20/0x140c80b00` as accessors' callers, `0x140d78100` as the constructor called by `0x140c90210`, and `0x140d7c260` as the finalizer called by `0x140c90bd0` (among other paths). It does not resolve virtual target bodies.
- In slot target `0x140d78220`, the bounded direct-call scan identifies calls at `0x140d7824f -> 0x140d41b30`, `0x140d78261 -> 0x140d78490`, `0x140d78270 -> 0x140d41b30`, `0x140d78284 -> 0x140d7a9f0`, and `0x140d7847b -> 0x1402cf3c0`. The next function `0x140d78490` has `.pdata` range through `0x140d7974a` and many further calls/allocations. Neither this body nor `0x140d7a9f0` has a retained decompile tying it to `setItem`, `appendRow`, or `setCheckable` on the `+0x230` object. Treat `d78490` as a high-value candidate, not a proven cell writer.

## Derived slot and remaining dependency

The retained strings snapshot contains `SfObjectManager` and `slotItemChanged(QStandardItem*)` (`evidence/gui_mo/strings.txt`, entries at raw offsets `0x2a787b0` and `0x2a787c6`). The existing construction receipt records the derived object's `itemChanged -> slotItemChanged` connection, in addition to the base `SfItemModel` connection. Base slot body `0x140d3d8c0` is retained and calls `0x140d41670` then notifier `0x1402ceae0`. The derived signal-string/meta-call has not been mapped to a code address or body; its state effects and ordering against the base handler remain unknown.

The precise missing proof is whether `d78220`/`d78490` writes checkable items into the same `SfObjectManager` returned by `c90210`, which View/settings values choose row and columns, and which address implements the derived item-change slot. Resolve by bounded decompilation of `0x140d78220`, `0x140d78490`, `0x140d7a9f0`, and the `SfObjectManager` meta-call path, then confirm the writer's receiver equals the `c90210` result. The present `.xref.pkl` scan only indexed LEA-style references; it does not close indirect calls or identify setter calls in unavailable function bodies.

No source or evidence file other than this report was changed. No build, GUI, original launch, or mutable REA session was used.
