# Checkpoint 59: DBLIST receiver vtable candidate

## Static invocation arguments resolve a concrete subtype candidate

The handler literals recovered in `analysis/resume58-dblist-literals.txt` are used as call arguments, not just nearby strings. The `FUN_1402cf740` wrapper's register/stack shuffle puts the incoming `r8` generic record in the `QGenericReturnArgument` position of its Qt `QMetaObject::invokeMethod` call. The handler passes the record typed `SfTaskBase *` with storage `[rsp+0x78]` in that position. The next incoming record, passed as `r9`, becomes the first `QGenericArgument`; it is typed `QString` and points to `SfTaskReportGenerate`. The member name is `2signalGetTaskBase()` and the wrapper uses Qt connection value `1` (DirectConnection in the retained Qt enum evidence).

This is strong static evidence that the method requests a task base by the QString selector `SfTaskReportGenerate` and returns it through the `SfTaskBase*` result slot. It is not a runtime observation of what the signal implementation returns.

## RTTI and vtable mapping

The bounded raw PE receipt [resume59-dblist-vtable-bytes.txt](resume59-dblist-vtable-bytes.txt) records the source PE SHA-256, the `.data` TypeDescriptor, complete-object locator, class hierarchy descriptor, vftable pointer, and slot bytes. The ReportGenerate class hierarchy entries are:

```text
SfTaskReportGenerate
SfTaskReportBase
SfTaskBase
QObject
```

Its primary vftable is at `0x14269e558`. Since `0x218 / 8 = 67`, the handler's virtual slot selects entry 67. That entry is `0x1407a0150`. The base `SfTaskBase` primary table has a different entry 67 (`0x14140cba0`), so the ReportGenerate candidate overrides this slot. This makes `SfTaskReportGenerate` the best source-qualified subtype candidate for the handler's receiver, while the actual returned instance remains unobserved.

The unique bounded capture [resume59-dblist-reportgenerate-slot67.asm](resume59-dblist-reportgenerate-slot67.asm) covers target `0x1407a0150` through its `ret` at `0x1407a02bb`. It iterates child DOM elements, compares each tag with the exact literal `OutputList` at `0x14269e3c8`, and only on a match calls the same object's vtable slot `+0x240`. The ReportGenerate table's slot 72 (`0x240 / 8`) resolves statically to `0x1407a0500`; that table value is included in the byte receipt. The implementation of `0x1407a0500` is not examined here. No direct filesystem call appears in the captured `+0x218` target, but its conditional callee and subsequent behavior remain open.

## B127 reachability qualification and unresolved boundary

The retained B127 `.DS` DBLIST has a direct `DBNAME` child and no `OutputList` child. If the original gate is open and `signalGetTaskBase` returns the selected ReportGenerate instance, the identified `+0x218` implementation's `OutputList` match branch would not be taken for that retained DBLIST shape. This is a static shape-to-branch comparison only; it does not establish original runtime dispatch, the signal result, or all side effects before/after the branch.

Remaining evidence gap: establish whether the original `signalGetTaskBase()` maps the selector to the `SfTaskReportGenerate` instance in this lifecycle, and whether any other `+0x218` override can be returned. The global gate still requires its `+0x1c` field to be zero. If a `DBLIST` contains `OutputList`, the next indirect target is now known to be vtable slot `+0x240`, candidate address `0x1407a0500`; its effects are unresolved. Keep public B127/Type2 handling guarded and preserve the current reader as detached parsing only.

## Provenance and scope

All observations are from retained static evidence and bounded read-only PE parsing. Original image SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No mutable REA/Ghidra session, original runtime, GUI, source, build, or public candidate path was used or changed.
