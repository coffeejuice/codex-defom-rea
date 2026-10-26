# CP151 DBLIST task-pointer identity frontier

## Result

A retained constructor capture now binds one factory prototype to the `SfTaskReportGenerate` address point: constructor `0x14079f1e0` writes `0x14269e558`, which is the primary table/address point represented for `SfTaskReportGenerate` in the retained vtable inventory. The registry-content capture shows that this constructor is among the prototypes passed to `FUN_1407c8140`, and the factory create routine can also construct and register it on a key match.

This is a concrete class-to-constructor witness, but it does not yet prove that the DBLIST selector `SfTaskReportGenerate` selects this branch in the same getter occurrence, nor that the resulting object is the value returned from the MainWindow `this+0x4a0` map. Keep the dynamic return type conditional and retain the public `+0x1c` gate separately.

## Evidence and bounded join

All native static captures qualify against `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

- `analysis/wave2-doe-task-prototype-ctor-14079f1e0.asm` (SHA-256 `0072B6C52A9C19DD54714036FDFCEABC7F034A50D2A8DF23C4DA53590225DB4B`) shows the constructor storing address point `0x14269e558` at object offset zero. The vtable inventory `evidence/gui_mo/s1/vtables.tsv` (SHA-256 `B5C2274F95BEBD3A0D7F16917937D0974D5F16E1840125F6E314A21787A571F4`) labels that table `SfTaskReportGenerate`; its slot `+0x218` is `0x1407a0150`.
- `analysis/wave2-doe-session-task-registry-content-decompile.json` (SHA-256 `C7C14861013EF85E57C7741EF3980990DB9BBEFFFFE42FC0B1A3896057FDD08B`) captures `FUN_140137610` registering a sequence of 17 constructed prototype pointers through `FUN_1407c8140`. The sequence includes `FUN_14079f1e0`. Its second function, `FUN_140137ca0`, compares the requested QString against a decision chain, constructs one of the same prototypes on a match, and passes that object to `FUN_1407c8140`. This establishes the constructor as a member of the task registry/factory mechanism, not its selector mapping for this DBLIST occurrence.
- `analysis/wave2-doe-session-task-registry-create-decompile.json` (SHA-256 `4EE1C25053F2B28733B048B4FDD6561A67CEB209DC280D2D389F77EE4099C346`) identifies the create-on-miss route through the factory virtual at `+0x10` and returns the stored pointer. `analysis/resume65-gettaskbase-dispatch.md` (SHA-256 `E88FE6C24238B0B3E04EB0B5611B9C8C6B776C6BB0CCE73000AE1093C3FDE495`) describes the distinct getter: it reads the existing value by selector from MainWindow `this+0x4a0`, then returns it without constructing or checking the vptr.
- CP149 IAT resolution (`analysis/resume149-connector-iat-resolution.md`) closes the DBLIST generic-invoke method route to `slotGetTaskBase(QString)` with selector `SfTaskReportGenerate` and return type `SfTaskBase*`. That route still does not show an insertion call or a returned pointer value.

## Exact remaining witness

The missing edge is the factory comparison branch mapping the selector text `SfTaskReportGenerate` to constructor `0x14079f1e0`, followed by proof that this factory result is the value stored under the same key in MainWindow `this+0x4a0` and returned at the DBLIST getter occurrence. The decompile's `UNK_...` comparison operands are not independently mapped to the selector by the retained receipt, and no same-occurrence runtime pointer/vptr capture is available. Constructor membership or class-name proximity is insufficient to fill those links.

The next decisive static witness would be a byte-verified mapping of the `FUN_140137ca0` comparison operand to the exact selector and branch target, plus the caller/write path that inserts that returned object into the getter's map. If these cannot be recovered statically, the remaining identity needs one original occurrence capture of selector, returned pointer, and `[pointer]` vptr. Either can close identity without asserting that the independent AppState `+0x1c` admission gate is open.

## Limits

Read-only retained static PE/decompile captures were inspected. No original runtime, GUI, mutable REA database, candidate source, or build was touched. No successful DBLIST or full project parity is claimed.
