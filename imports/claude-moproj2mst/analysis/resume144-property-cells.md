# CP144: property-manager cell insertion and callback boundary

## Result

The retained evidence narrows the next query to the exact `SfObjectManager` virtual update path, but does not prove that the updater inserts cells or establishes their checkable state. No derived `slotItemChanged(QStandardItem*)` implementation is bound. Keep the populated-cell refusal; do not infer behavior from the neighboring finalizer or from unrelated item models.

## Bound owner and dispatch

The immutable original `evidence/gui_mo/DEF_GUI_MO_64.exe` is identified by SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (also recorded in CP142/143). `analysis/wave2-doe-stepdata-statevar-callbacks-decompile.json` binds `0x140c90210` as the lazy accessor that stores the object returned by `0x140d78100` at `GuiModel+0x230`. The constructor evidence in `analysis/wave2-doe-stepdata-ctors-remesh-decompile.json` and `analysis/wave2-doe-stepdata-checkbox-empty-contract.json` establishes the actual `SfObjectManager` type and both base/derived `itemChanged -> slotItemChanged` registrations; it also establishes that construction leaves the Qt model empty.

The vtable inventory `evidence/gui_mo/s1/vtables.tsv` maps `SfObjectManager` vtable slot `+0x140` to `0x140d78220`. The retained decompile `evidence/gui_mo/s1/mdt/DB/d3.c` shows two callers, `0x140c80a20` and `0x140c80b00`: each obtains the `0x140c90210` result and invokes its slot `+0x140`, passing the model as the second argument. This proves that the actual lazy manager receives update calls; it does not prove what the slot writes.

The retained `.pdata` boundaries recorded in `analysis/resume143-property-cell-writer.md` put `0x140d78220` in `[0x140d78220,0x140d7848a)`, `0x140d78490` in `[0x140d78490,0x140d7974a)`, and `0x140d7a9f0` in `[0x140d7a9f0,0x140d7aad6)`. The CP143 bounded byte inspection records direct calls from `0x140d78220` to `0x140d78490` at `0x140d78261` and to `0x140d7a9f0` at `0x140d78284`. These make the latter bodies focused candidates in the slot's update path. There is no retained decompile or bounded call-body receipt for either candidate that identifies a `QStandardItem` allocation, `setCheckable`, `setItem`/`appendRow`, item role initialization, or a check-state write into the model returned by `c90210`.

## Callback edge

The evidence only binds registration, not invocation behavior. The base slot at `0x140d3d8c0` is separately identified in `evidence/gui_mo/s1/mdt/L4/tr.c` and CP142 as calling `0x140d41670` and notifier `0x1402ceae0`. That does not identify the derived receiver's method body, its meta-call dispatch entry, or ordering/effects relative to the base slot. `evidence/gui_mo/strings.txt` contains the relevant class and method strings, but those are raw file offsets and do not bind a code VA by themselves.

## Exact unresolved edge and next query

The missing proof is the body-level path from `0x140d78220` through `0x140d78490`/`0x140d7a9f0` to a cell installed in the **same** model owned at `GuiModel+0x230`, followed by the derived `slotItemChanged` body. The minimum useful recovery is a bounded decompilation/capture of those three functions plus the `SfObjectManager` meta-call dispatcher, preserving receiver and argument flow; specifically identify any Qt item constructor and model insertion/checkable-state calls, and the derived slot target address. If those routines do not contain the writer, the next missing witness is the caller that supplies the updater's view/row data. Do not promote the slot call alone to proof of populated cells or callback semantics.

No source, build, test, GUI, original execution, or mutable REA database was used. This report is based on retained static evidence and the existing CP142/143 receipts; no new PE capture was produced.
