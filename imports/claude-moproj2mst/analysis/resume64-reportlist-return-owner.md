# Checkpoint 64: ReportList signal return owner remains unproven

## Evidence examined

The ReportList handler's generic return argument is typed `SfTaskBase *`, with selector string `SfTaskReportGenerate`, and its subsequent `+0x218` virtual call uses the pointer returned through that argument. The literal mappings are preserved in `analysis/resume58-dblist-literals.txt`; the wrapper argument mapping is described in `analysis/resume59-dblist-vtable-next.md`. This is strong evidence of the requested base-pointer type and selector, but does not reveal the object actually supplied at runtime.

The retained direct-key candidate inventory identifies a main-window constructor connection registration at call `0x140011fa7`: sender/global `0x143d76700`, signal literal `0x1420f9918` (`2signalGetTaskBase()`), receiver `param_1`, slot literal `0x1420f9900` (`1slotGetTaskBase()`). The unique bounded capture [resume64-mainwindow-gettaskbase-connect.asm](resume64-mainwindow-gettaskbase-connect.asm) includes the call and surrounding setup, SHA-256 `8CF1FF76DBD0223F4F142BB90B71121E7EEABF84241846BD804ED42CE3B3589D`. The retained string inventory also contains `SfTaskBase*`, `qsClassName`, and `slotGetTaskBase(QString)` near the `SfMoMainWindow` meta-string data (`evidence/gui_mo/strings.txt`, lines around 51337–51360). The registration literal is the no-argument slot spelling `1slotGetTaskBase()`, while the meta-string table includes the QString overload. The retained constructor connection is not itself an implementation body or proof that the handler's invocation returns that slot's value.

## Candidate class comparison

The earlier RTTI/vftable receipt `analysis/resume59-dblist-vtable-bytes.txt` proves that if the handler's `SfTaskBase*` is dynamically an `SfTaskReportGenerate`, virtual slot `+0x218` selects `0x1407a0150`; the base `SfTaskBase` table instead selects `0x14140cba0`. The selector literal `SfTaskReportGenerate` favors that subtype as a candidate. However, no retained implementation of `signalGetTaskBase()` or `slotGetTaskBase(QString)` was found in the inspected DOE decompilation/analysis artifacts, and no static object construction/field-return path connects this selector to the `SfTaskReportGenerate` vtable pointer. Therefore the dynamic class identity is neither proven nor refuted. Other derived `SfTaskBase` objects remain possible.

## Exact next static seam

The smallest useful follow-up is to recover `SfMoMainWindow`'s Qt meta-call dispatch for the `slotGetTaskBase(QString)` method index, then inspect only that slot body and its direct return/accessor callees. This would establish the actual lookup key, selected task collection, and returned pointer source. If the slot metadata implementation cannot be mapped from retained static data, the return class must remain unresolved. A connection literal and selector string alone do not justify selecting the ReportGenerate vtable entry.

## Guard and scope

Keep the public ReportList/DBLIST DOE path guarded until the return-object class and the `+0x218` target are source-qualified. This task used retained static PE strings/decompilation and a bounded DUMPBIN callsite capture only. Original binary SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No source, build, GUI, runtime, or mutable REA/database access occurred.
