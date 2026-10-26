# CP149: local immutable PE property-cell static recovery

## Result

This offline pass adds no cell-writer or derived-slot body. It does confirm the derived connection's exact static Qt signature pair and identifies a class-name virtual function as distinct from the missing callback body. The decisive writer edge remains unclosed; keep the populated-cell refusal.

## Identity and method

The inspected immutable PE is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC` (verified unchanged at the end of the read-only pass). I reviewed `analysis/resume145-property-cell-rea-status.md`, `analysis/resume146-property-cell-rizin.md`, CP142–144 receipts, the retained constructor disassembly, the `SfObjectManager` vtable rows, and raw initialized PE bytes using Python standard-library section mapping. No local `dumpbin`, `llvm-objdump`, GNU `objdump`, `ndisasm`, Rizin, or WinDbg command was present; Python `capstone`, `iced_x86`, and `pefile` modules were also absent. Thus I did not decode fresh instruction streams from raw bytes.

Existing bounded constructor capture `analysis/wave2-doe-stepdata-table-constructor.asm` establishes the specific same-object registration in `0x140d78100`: RCX is the `SfObjectManager` being constructed; RDX is `0x142a3f1a8`, whose bytes are `2itemChanged(QStandardItem *)`; R8 is the same manager; R9 is `0x142a3f180`, whose bytes are `1slotItemChanged(QStandardItem *)`; stack argument `+0x20` is `3`. This is concrete class/receiver/signature evidence, but a Qt string-based connection does not by itself expose the derived dispatch body's state effects.

The vtable inventory `evidence/gui_mo/s1/vtables.tsv` assigns `0x140ddb250` to `SfObjectManager` slot `+0x8`. A bounded PE `.text` scan for RIP-relative `LEA` destinations found one candidate instruction at `0x140ddb26a` targeting the class-name string at VA `0x142a79fb0` (`SfObjectManager`, raw file offset `0x2a787b0`). The containing `.pdata` function range is `[0x140ddb250,0x140ddb29a)`. This binds a nearby vtable method to class-name data, not to `slotItemChanged`; it is not evidence of cell population or callback behavior.

The same narrowly defined `.text` scan found no RIP-relative `LEA` candidate directly targeting the class-specific method-name bytes at VA `0x142a79fc6` (raw offset `0x2a787c6`). This is only a negative result for that instruction form and target address: it excludes neither references to the surrounding string table, computed offsets, `.rdata` metadata, nor indirect Qt meta-call dispatch. A separate identical scan found a candidate at `0x140783862` to a different string-table copy at `0x1426949c8`; its containing range is `[0x140781070,0x140783901)`. No retained function identity or same-manager call edge ties that occurrence to the `+0x230` object, so it is not promoted as the callback.

## Remaining blocker and next useful edge

The known updater call chain still terminates at `0x140d78220 -> 0x140d78490` and `0x140d7a9f0`; no available local decoder or retained body capture here proved a `QStandardItem` allocation, checkable/role setup, or insertion into the lazy manager returned by `0x140c90210`. The constructor does prove the derived connection string and same receiver, but not the meta-call's concrete method body or any post-connect state effect.

A useful next recovery requires a functioning local x64 decoder or a retained bounded disassembly/decompile for `0x140d78220`, `0x140d78490`, `0x140d7a9f0`, and the relevant Qt meta-call route. Preserve the receiver identity from `c90210` through the insertion and the derived slot target. Until those edges are recovered, synthetic populated-cell behavior is not justified.

No source, build, test, GUI, original process, mutable REA database, or executable bytes were changed.
