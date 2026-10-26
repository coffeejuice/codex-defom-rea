# Checkpoint 65: GetTaskBase meta dispatch and returned pointer source

## Static meta-call mapping

The bounded PE receipt [resume65-gettaskbase-meta-receipt.txt](resume65-gettaskbase-meta-receipt.txt) binds the `SfMoMainWindow` metadata strings and record to original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The metadata record at file offset `0x216d1bc` has name/signature offset `0x27e`, which points to `slotGetTaskBase(QString)` in the class string table. Its zero-based method record index is 17. The associated meta-call body is `0x1400db1a0`; its initial dispatcher checks the call kind, bounds the method id to `0x84`, then indexes a relative jump table. Entry 17 maps to `0x1400db504`. Evidence: [resume65-mainwindow-metacall-head.asm](resume65-mainwindow-metacall-head.asm) and [resume65-metacall-slot17-exact.asm](resume65-metacall-slot17-exact.asm).

Case 17 copies the first QString argument from the meta-call argument array, calls `FUN_14003c630(this, key)`, and, if the meta-call return slot is non-null, stores the returned pointer into that slot. This is direct evidence that the QString slot returns a pointer through Qt's return argument.

## Getter return owner

`FUN_14003c630` calls `FUN_14001d960(this)` to obtain a container at object offset `+0x4a0`, then calls `FUN_1407c7d00(container,key)` and returns its result. The accessor initializes/attaches the `+0x4a0` member if it is null. The exact bounded captures are [resume65-gettaskbase-getter-exact.asm](resume65-gettaskbase-getter-exact.asm), [resume65-gettaskbase-map-owner-exact.asm](resume65-gettaskbase-map-owner-exact.asm), and [resume65-gettaskbase-map-lookup-exact.asm](resume65-gettaskbase-map-lookup-exact.asm).

The lookup first tests whether the key exists in an internal container view. On a hit it obtains the entry-value address and loads its first qword as the return value; on a miss it returns null. Therefore this getter's return owner is the value stored under the QString key in the `SfMoMainWindow` instance's container member at `+0x4a0`. The getter does not construct a task or check its dynamic type. The map-like implementation's exact C++ container class and full entry-value layout are not assigned here.

## Does this prove the ReportList receiver is ReportGenerate?

No. The ReportList handler passes the key string `SfTaskReportGenerate` and return type `SfTaskBase *`, which is consistent with this lookup. But the handler calls `QMetaObject::invokeMethod` on global `0x143d76700` with member literal `2signalGetTaskBase()`; retained constructor code connects that global's no-argument signal to receiver `param_1` using slot literal `1slotGetTaskBase()`. Separately, the `SfMoMainWindow` string/meta table identifies `slotGetTaskBase(QString)` as a return-typed slot. The static artifacts do not yet prove that the handler's invoke reaches this QString slot on this same main-window object (or explain the no-argument registration versus QString meta signature).

Even if the lookup path is the path reached, the returned pointer is whatever object was previously stored under `SfTaskReportGenerate`. `FUN_14003c630` does not validate that object's vptr. The `SfTaskReportGenerate` vtable candidate at slot `+0x218` remains plausible from the key spelling and RTTI, but the insertion/creation path that binds this key to a concrete object instance has not been closed. The neighboring meta-call entry 18 dispatches `slotCreateTaskBase(QString)` to `0x1400db573`, which calls `FUN_14003c6b0`; that creator and the map insertion/type registration path are the next exact static targets to trace.

## Safe isolated prefix and guard

The source-qualified behavior prefix is: copy QString key → obtain/lazily initialize this main-window container at `+0x4a0` → return the mapped entry pointer or null → write it through Qt's return slot when provided. This prefix is enough to guide a detached generic lookup helper, but the current converter source has no `SfMoMainWindow`, `SfTaskBase`, or native task registry abstractions. Adding a converter helper or test now would invent an integration contract and still would not test the native key-to-vtable association. Keep the public ReportList/DBLIST path guarded; next trace `FUN_14003c6b0` plus its direct map insertion/factory edge and verify which object class is stored for key `SfTaskReportGenerate`, then close the call-signature/object identity bridge.

Static PE evidence only; no GUI, source edit, build, runtime, or mutable REA/database access occurred.
