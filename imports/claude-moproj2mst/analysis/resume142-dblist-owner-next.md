# CP142 DBLIST owner and returned-task identity frontier

## Finding

The retained call path resolves the Qt invoke receiver only to an object derived from the current dispatcher-registry item. It does not bind that item to `SfMoMainWindow`, identify the selected method implementation, or bind the returned `SfTaskBase*` to a concrete vptr. The selector `SfTaskReportGenerate` and the `SfTaskReportGenerate` vtable therefore remain suggestive, not an identity proof. There is no safe new candidate dispatch cut; preserve the DBLIST/ReportList guard.

## Reconciled call chain

1. Handler `0x140655df0` creates a null `SfTaskBase*` output slot and calls `0x1402cf740` with dispatcher global `0x143d76700` and method literal `2signalGetTaskBase()`; see `analysis/resume131-dblist-metaobject-next.md` and `analysis/resume66-report-taskbase-bridge.md`.
2. Wrapper `0x1402cf740` uses state rooted at `wrapper+0x88`. Its loop gets a current item through `0x1402eee10`, reads an item-associated object through `0x140c1aef0`, and passes a receiver derived as that object plus eight to `QMetaObject::invokeMethod`. CP131 explicitly does not identify the item class, receiver class, or relationship to the AppState/MainWindow singleton.
3. The retained constructor connection registers `2signalGetTaskBase()` to `1slotGetTaskBase()` without arguments. Separately, MainWindow meta-call record 17 is `slotGetTaskBase(QString)` and returns `SfTaskBase*`; its implementation looks up the QString in `this+0x4a0` and returns the stored pointer. The existing evidence does not establish an overload bridge from the registered no-argument slot to this QString method or establish that the dynamic receiver is that MainWindow.
4. Creator record 18 (`slotCreateTaskBase(QString)`, target `0x1400db573`) calls `FUN_14003c6b0`. The retained bounded body obtains the MainWindow container and delegates to `0x1407c7d90`, but does not expose the key-specific constructed object or show the resulting map value's vptr. The getter can only return that pre-existing value; it has no type check.
5. If the result were dynamically `SfTaskReportGenerate`, handler slot `+0x218` would resolve to `0x1407a0150`. The slot capture proves behavior conditional on that type; it does not identify the object delivered by the preceding invoke.

The native image qualification recorded by the relevant receipts is `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

## Exact missing identities

A decisive retained witness must join the same DBLIST occurrence across these boundaries:

- current registry item returned from `0x1402eee10` → concrete QObject and its meta-object;
- emitted no-argument method spelling → exact method selected by `invokeMethod`, including where the QString selector is consumed and where the output pointer is stored;
- selector-key insertion/creation → exact pointer later returned for `SfTaskReportGenerate` → runtime vptr (or null).

No retained capture located in the CP65/66/131/136 evidence set closes any complete join across those three boundaries. The existing constructor prototype inventory is not a substitute: it lists candidate constructors but does not bind a constructor to this key's map insertion or the active result pointer.

Separately, public admission still depends on the AppState `+0x1c` gate value at the Open occurrence. Establishing the returned-task identity alone would not close that gate lifecycle.

## Limits

This was a read-only reconciliation of retained static captures and receipts. No new PE/REA query, runtime action, source edit, build, or GUI operation was performed. No full project or original parity behavior is claimed.
