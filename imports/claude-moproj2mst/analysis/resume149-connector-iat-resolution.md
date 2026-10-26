# CP149 DBLIST QtCore IAT resolution

## Result

The two intermediate QtCore imports resolve the wrapper's registry-item method calculation. For each item returned under the `signalGetTaskBase()` key, `cf740` converts the item's stored QString method to an ASCII method name, retrieves the item's live QObject through the QPointer accessor, and invokes that method directly with the prepared generic arguments. This joins the MainWindow constructor's registered `slotGetTaskBase` receiver/method entry to the DBLIST generic-call path at the static registry-contract level.

It still does not bind the `SfTaskBase*` returned from `slotGetTaskBase(QString)` to a concrete runtime vptr: the getter returns the pointer already stored under selector `SfTaskReportGenerate`; the insertion/factory result for that map key is not retained. Therefore `ReportGenerate +0x218` remains a conditional target, and public DBLIST admission remains independently guarded by AppState `+0x1c`.

## Exact IAT and instruction evidence

Original image: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. DUMPBIN 14.44.35228.0 `/IMPORTS` reports the `QtCore4_64.dll` IAT base `0x141e03b60` and 977 entries.

| Callsite / IAT slot | Delta and import index | Imported function |
|---|---:|---|
| `0x1402cf981` → slot `0x141e05820` | `0x1cc0 / 8 = 920` | `?toAscii@QString@@QEBA?AVQByteArray@@XZ` |
| `0x1402cf9af` → slot `0x141e04228` | `0x6c8 / 8 = 217` | `?data@QByteArray@@QEAAPEADXZ` |
| `0x1402cfa5b` → slot `0x141e05348` | `0x17e8 / 8 = 765` | `?invokeMethod@QMetaObject@@SA_NPEAVQObject@@PEBDW4ConnectionType@Qt@@VQGenericReturnArgument@@VQGenericArgument@@444444444@Z` |

The existing wrapper byte capture `analysis/resume56-dblist-callee-cf740.asm` (SHA-256 `A61B6198D563DC3853599F95393EB5D597F7662A39F9D0B05DC3E9A03F6B751D`) supplies the call dataflow:

1. `0x1402cf981` is encoded `FF 15 91 5E B3 01` after `rcx=[rsp+0x80]+8`; the QString at that member is converted to QByteArray.
2. `0x1402cf9af` is encoded `FF 15 73 48 B3 01`; it calls `QByteArray::data()` on that result, and `rdi` receives the C-string method name.
3. At `0x1402cf9b8`, `rcx=[rsp+0x80]` (the current registry item) is passed to `0x140c1aef0`; `rcx` is then loaded from the returned pointer. No intervening call changes it before `0x1402cfa5b`.
4. At `0x1402cfa58`, `rdx=rdi`; at `0x1402cfa5b`, the wrapper calls `QMetaObject::invokeMethod` with that method name, the QPointer-derived QObject receiver, direct-connection value 1, and the prepared generic return/argument array.

This corrects the earlier CP131/CP145 reading that treated the final receiver as simply `item+8`. `item+8` is the stored method QString for this sequence; the receiver is the result of `0x140c1aef0(item)`.

## Join to the MainWindow registration

The constructor capture `analysis/resume64-mainwindow-gettaskbase-connect.asm` (SHA-256 `8CF1FF76DBD0223F4F142BB90B71121E7EEABF84241846BD804ED42CE3B3589D`) shows `0x140011fa7` calling the shared registration helper with global `0x143d76700`, signal key `2signalGetTaskBase()`, MainWindow `param_1` as receiver, and slot string `1slotGetTaskBase()`.

`analysis/resume101-boolean-direct-dispatch-pe.md` records the shared `0x1402cfbe0` registration behavior: the slot signature is stripped to the bare method name; the `()` text in the registration string does not constrain argument arity. Its broader dispatch review is for separate Boolean signals, so only that helper behavior is applied here. The DBLIST wrapper looks up the same registry by the same signal key, obtains each entry's method QString and QPointer receiver as above, and calls the method with its own supplied generic types. The handler's exact generic payload literals are `QString` with data `SfTaskReportGenerate` and return type `SfTaskBase *` (`analysis/resume58-dblist-literals.txt`). The retained MainWindow meta-call record 17 is `slotGetTaskBase(QString)` and returns `SfTaskBase*` (`analysis/resume65-gettaskbase-dispatch.md`). This is a source-supported path to the MainWindow QString getter for the matching live registration; no separate no-argument bridge is needed because the registry stores the bare method name and the invocation supplies typed generic arguments.

## Remaining object identity and acceptance boundary

The getter looks up `SfTaskReportGenerate` in the MainWindow member map at `this+0x4a0` and returns the stored pointer or null. The current evidence does not trace the insertion/factory path for that key to the exact returned addresspoint or class. Thus we can identify the receiver method route and selector, but not prove the result is dynamically `SfTaskReportGenerate`; do not dispatch as though that vptr were established. The conditional slot-67 address `0x1407a0150` remains applicable only if that type is independently bound.

This resolves the *registry method calculation* requested for `cf740`; it is not an original runtime observation of a particular list snapshot, returned pointer, `+0x218` target, or successful DBLIST. It does not change the public guard. No source, build, GUI, runtime, or mutable REA action was performed.
