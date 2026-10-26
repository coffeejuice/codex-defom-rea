# CP145 DBLIST registry-item and returned-vptr frontier

## Result

No same-occurrence identity witness was found in the retained CP144 material or the immutable PE captures reviewed here. The useful additional crosswalk is `evidence/gui_mo/s1/s1-04/spec.md` §8: it documents the sibling dispatcher `0x1402cf3c0` looking up live receivers in the shared `obj+0x88` registry via `0x1402d03f0`, then invoking each registered receiver method. This strengthens the registry interpretation, but it does not close DBLIST's `0x1402cf740` receiver item or the vptr of its returned `SfTaskBase*`.

## Evidence reconciliation

- The retained main-window constructor call at `0x140011fa7` registers `0x143d76700`, the `2signalGetTaskBase()` key, `param_1` as receiver, and `1slotGetTaskBase()` as method (`analysis/resume64-mainwindow-gettaskbase-connect.asm`; constructor identity in `analysis/resume60-dblist-signal-next.md`). `param_1` is the MainWindow under construction. Thus static registration for that key exists in this constructor path.
- The DBLIST wrapper `0x1402cf740` does lookup through the same `0x1402d03f0` helper on the registry rooted at `0x143d76700+0x88`, then iterates returned items and calls Qt `QMetaObject::invokeMethod` on an object derived from each item (`analysis/resume56-dblist-callee-cf740.asm`; prior dataflow in `analysis/resume131-dblist-metaobject-next.md`). This is compatible with finding the registered MainWindow entry.
- The extra registry contract in `s1-04/spec.md` §8 is specifically for sibling dispatcher `0x1402cf3c0` (Convert2Dto3D status-file notification). It describes lookup by `QString(name)`, dead-receiver filtering, and invocation of registered receiver methods. Although both paths use `0x1402d03f0`, that sibling description does not establish that `0x1402cf740` uses the same item layout/method-selection sequence, that the MainWindow entry is present/live in the DBLIST occurrence's list snapshot, or that this occurrence's effective method signature is `slotGetTaskBase(QString)`. The DBLIST wrapper's invoke arguments and return storage are distinctive.
- `slotGetTaskBase(QString)` meta-call record 17 reads an already-stored pointer from the MainWindow member map at `this+0x4a0`; it does not create or type-check the object. The creator record 18 and `FUN_14003c6b0` path have not been linked to the value stored for the `SfTaskReportGenerate` key. No retained evidence joins the lookup result pointer to a specific vptr.
- If that returned pointer is dynamically `SfTaskReportGenerate`, its `+0x218` call resolves to `0x1407a0150`; that remains a conditional target only. Vtable membership and selector spelling do not prove object identity.

The reviewed original PE is `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The reused wrapper and connection captures are hash-qualified in their existing receipts. `evidence/gui_mo/s1/s1-04/spec.md` SHA-256 is `111DBCE67E22BD6DB0386E24A9DBC051AA48E6A09C9D2352368133A5EC13ED12`.

## Exact remaining dependency

The needed static witness is a dataflow join for the same DBLIST handler occurrence: the lookup key and list snapshot returned from `0x1402d03f0`; the specific live record/item selected; its receiver QObject and registered receiver-method signature; the result written to handler `[rsp+0x78]`; and that returned pointer's actual vptr (or null). Existing evidence stops between generic registry semantics and this particular wrapper/result. Even a successful join would not settle the separate AppState `+0x1c` public admission lifecycle.

No source-ready integration cut follows from this evidence. Keep the public DBLIST/ReportList guard. CP144 contains no DBLIST/ReportGenerate receiver receipt; this review used only existing reports, retained static exports and immutable original PE import metadata. No REA, source, build, GUI, or runtime action was performed.
