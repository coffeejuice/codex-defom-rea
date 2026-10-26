# CP149 DBLIST receiver and returned-vptr next witness

## Finding

The DBLIST wrapper does carry the current registry item's embedded object directly into the Qt invocation, but the exact invoked method and returned task instance remain unbound. The receiver and output are distinct identities: the `QObject*` for `invokeMethod` comes from the current registry item, while the later `+0x218` receiver comes from handler output storage `[rsp+0x78]`. No retained static artifact joins them or identifies the output object's vptr.

## New narrow boundary from the callsite dataflow

In `analysis/resume56-dblist-callee-cf740.asm`:

- At `0x1402cf975..0x1402cf981`, the wrapper sets `rcx` to the current registry item at `[rsp+0x80] + 8`; this is the receiver passed to `QMetaObject::invokeMethod` at `0x1402cfa5b`.
- Before invocation, it calls IAT slot `0x141e05820` with that same receiver and a local method/argument descriptor (`0x1402cf975..0x1402cf989`). It then calls IAT slot `0x141e04228` with the returned value (`0x1402cf9a7..0x1402cf9b5`) and places that call's result in `rdi`. At `0x1402cfa58`, `rdx=rdi`, so the actual method-name argument is derived by these receiver-specific lookups. The available `resume57-dblist-import-map.txt` maps the final `0x141e05348` slot to `QMetaObject::invokeMethod`; it does not identify these two intermediate imports. Do not assume `rdx` is the raw `2signalGetTaskBase()` literal or the registered `slotGetTaskBase()` string.
- The main-window constructor statically registers `param_1` under signal key `2signalGetTaskBase()` with method `1slotGetTaskBase()` (`analysis/resume64-mainwindow-gettaskbase-connect.asm`). This is a potential matching record, but no retained evidence identifies the `0x1402cf740` loop's current item as that record in the DBLIST occurrence or resolves the receiver-specific method lookup above to `slotGetTaskBase(QString)`.
- Handler output `[rsp+0x78]` starts null, is passed as a generic `SfTaskBase*` argument, and is tested after the wrapper; only then does the handler call `+0x218` on its value (`analysis/resume56-dblist-handler.asm`, `analysis/resume58-dblist-invoke-target.md`). That value is not the wrapper's QObject receiver. Even if the receiver is proven to be MainWindow, `slotGetTaskBase(QString)` returns the pointer stored under its map key and does not validate or expose that pointer's concrete vptr.

The existing MainWindow getter/meta-call evidence (`analysis/resume65-gettaskbase-dispatch.md`) and conditional ReportGenerate slot evidence (`analysis/resume63-reportgenerate-slot67.asm`) therefore still do not bind the active returned object to `SfTaskReportGenerate`.

## Exact smallest missing witness

First resolve the two intermediate imports `0x141e05820` and `0x141e04228` from the immutable QtCore import table, then trace only their inputs/outputs far enough to establish the method-signature bytes used at `invokeMethod`. The decisive occurrence-specific endpoint is a join of: the current registry item's identity; its effective receiver/method pair; the generic return storage written by that selected method; and the returned object's vptr (or null). The current retained capture provides the item-derived receiver and the two unresolved method-derivation edges, but no such join.

If the resolved method is the MainWindow QString getter, the next concrete missing link is still the map insertion for key `SfTaskReportGenerate` to the exact object pointer/vptr; the getter itself is only a keyed pointer lookup. Independently, AppState `+0x1c` public admission remains unresolved.

## Qualification

CP149-adjacent retained reports contain no newer DBLIST receiver/vptr witness. This is a bounded review of retained original-PE disassembly/import records and existing source qualifications only; no source, build, GUI, runtime, or mutable REA action was performed. Original PE SHA-256 remains `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
