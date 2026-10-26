# Checkpoint 65 successor: isolated GetTaskBase prefix

## Reconstructed prefix

The `SfMoMainWindow::slotGetTaskBase(QString)` meta-call entry is source-qualified to method record 17 and target `0x1400db504`. That target copies the QString argument and calls `FUN_14003c630(this,key)`. The getter obtains `this+0x4a0` through `FUN_14001d960`, then calls `FUN_1407c7d00(container,key)`. The lookup returns the stored pointer on a key hit and null on a miss. The meta-call writes that result through the return argument when the return slot pointer is non-null.

Pseudocode limited to these proven operations:

```text
slotGetTaskBase(key):
    registry = ensure_or_get_mainwindow_member_0x4a0()
    result = lookup(registry, key)  // stored pointer, or null
    return result
```

## Why I did not add converter source/tests

This is a generic keyed-pointer lookup, but the converter subtree has no `SfMoMainWindow`, `SfTaskBase`, or native task-registry type to which it can be attached. The static caller bridge is also not closed: the ReportList handler's `QMetaObject::invokeMethod` names `signalGetTaskBase()` on global `0x143d76700`, while the constructor connection literal is the no-argument `slotGetTaskBase()` and the class meta table contains the return-typed QString overload. The source getter alone therefore does not establish the behavior of the ReportList invocation. A standalone converter test would verify a newly invented map wrapper, not original behavior.

## Exact next targets

1. Resolve whether the handler's invoke on `0x143d76700` reaches this `SfMoMainWindow` QString meta-call entry, including the signature/overload bridge.
2. Trace creator case 18 (`0x1400db573` → `FUN_14003c6b0`) and direct insertion/factory callees to show what concrete vptr is stored for key `SfTaskReportGenerate`.
3. Only if the insertion path maps that key to `SfTaskReportGenerate`, join it with its RTTI slot-67 target `0x1407a0150`; otherwise leave dynamic dispatch polymorphic.

Public ReportList/DBLIST activation remains guarded. No implementation, CMake, build, GUI, runtime, or mutable REA change was made.
