# Checkpoint 63: native Report viewer body and owner effects

## Result

The immutable original PE yields the complete native body at `0x1412af0c0` (through its epilogue/RET at `0x1412af6a0`) and constructor at `0x140643100` (through RET at `0x1406432e5`). The owner-side retained decompilation shows lazy creation and storage of the constructed 0x70-byte object. These facts identify a concrete owner and initialization path, but do not establish that this object is the active Report Save target or what its Save action does. Keep that behavior guarded pending a resolved action/dispatch target.

## Artifact and capture

Artifact: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The unique bounded raw capture is [resume63-report-viewer-bounded-capture.txt](resume63-report-viewer-bounded-capture.txt). It contains the DUMPBIN disassembly from each entry to its own RET and the two retained decompilation excerpts cited below. The first bounded disassembly was `0x1412af0c0..0x1412af750`; the second was `0x140643100..0x140643400`. Excerpts in the capture are clipped at the observed function RETs (`0x1412af6a0` and `0x1406432e5`), excluding adjacent functions and padding.

## What the native body establishes

`0x1412af0c0` is a substantial routine with a `0x1e0`-byte local frame. Its early path obtains values through calls at `0x1405ef2b0`, `0x14005f770`, and `0x1412b0fc0`; null checks branch to a shared cleanup epilogue. It then performs a series of indirect helper calls using stack-local objects, calls `0x140c7ae00`, `0x140d0d6f0`, `0x140c7c1a0`, `0x140c7ae80`, `0x140819600`, `0x1408f37d0`, `0x1408f4760`, and `0x1408f3820`, and has conditional cleanup paths before the common RET. The captured machine code proves those operands, branches, and calls. Without resolved helper/IAT semantics or a complete action path, assigning those calls a Report Save meaning would exceed the evidence.

The retained caller `FUN_1412adf90` first invokes the object's virtual slot `+0x80`, then calls `FUN_1412af0c0` with the same object pointer. This establishes ordering and receiver identity only; it does not identify the vtable target of slot `+0x80` or associate this caller with the user-visible Save command.

## Constructor and owner

Retained `FUN_1400b5ad0` (`evidence/gui_mo/s1/mdt/L7/d3.c`) checks a QPointer-like member at `param_1 + 0x30`; when empty, it obtains mode from `FUN_140010330(param_1)`, allocates `0x70` bytes via `FUN_14146eabc`, calls `FUN_140643100`, and stores the returned pointer back through `FUN_140cbc920(param_1 + 0x30, ...)`. `FUN_140010330` is retained as `*(int *)(param_1 + 0x18) == 2`. Constructor arguments in this owner call are global object/string lookup at `0x143d766b8`, that mode value, the inverse of the first mode check, and zero.

Native constructor `0x140643100` calls the QObject base constructor, installs vtable pointer `0x1424e0098`, initializes members at offsets `0x18`, `0x20`, `0x38`, `0x40`, `0x50`, `0x58`, and `0x60`, and stores the three byte arguments at object offsets `0x28`, `0x29`, and `0x2a`. It then calls `0x1406433a0` with `this` and makes seven calls to `0x1402cfbe0` using the main-window global at `0x143d76700`, paired static metadata addresses spanning `0x1424decd8..0x1424dee70`, and the newly constructed object. This proves connection-registration calls are made during construction; the capture does not resolve their signal/slot strings or establish which, if any, is Save.

## Relation to the ReportList audit

This owner path is distinct from the ReportList dynamic task lookup in [resume63-reportlist-dynamic-owner.md](resume63-reportlist-dynamic-owner.md). That audit's unresolved `slotGetTaskBase(QString)` result and possible `SfTaskReportGenerate` vtable must not be conflated with this `0x70`-byte owner-held object. Neither path currently supplies the missing dynamic/action link needed to claim the Report Save implementation.

## Smallest next evidence step

Resolve the seven constructor connection metadata pairs (including their encoded signal/slot text) and the runtime/static mapping of the relevant receiver slot to a body. Separately close the path from the public Report Save dispatch to this owner/vtable. If either lookup remains indirect, preserve the guard; the current evidence is sufficient to state construction and caller ordering, not action equivalence.

No source, build, tests, GUI, or mutable REA state was accessed or changed.
