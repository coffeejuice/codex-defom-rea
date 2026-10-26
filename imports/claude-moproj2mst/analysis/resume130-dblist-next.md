# CP130 — independent receiver and setter review

## Result

The retained DBLIST route supplies no source-ready public integration cut. It contains three distinct identities which must not be collapsed:

1. AppState/gate singleton `0x143d766b8`, whose `+0x1c` is checked before entering the handler and explicitly initialized to zero by its pre-main constructor (CP116).
2. Wrapper argument/global `0x143d76700`, passed as `rcx` to `0x1402cf740`. The wrapper uses its fields to prepare/iterate generic arguments; the retained body does not prove this global itself is the Qt `QObject*` passed to `QMetaObject::invokeMethod`.
3. The handler's output location `[rsp+0x78]`, initially null and typed as `SfTaskBase *`; after the dynamic invoke, it is conditionally used as the receiver for vtable `+0x218`. This is not shown as a stable field of either global.

No retained code connects those meta-object/virtual calls to `0x1402cbf30` or to a write of AppState `+0x1c`. Therefore startup zero is qualified only immediately after construction. A later gate value remains unknown, and the public parser refusal remains the right candidate boundary.

## Receiver construction review

The complete wrapper capture `analysis/resume56-dblist-callee-cf740.asm` shows the distinction in its dataflow:

* Entry saves `rcx` (the `0x143d76700` wrapper context at the handler callsite), then reads byte `+0x141` as an early gate and passes `wrapper+0x88` into helper `0x1402d03f0` to prepare state.
* The wrapper iterates the resulting list with `0x1402eee10`. Each returned item is dereferenced into local `[rsp+0x80]`, checked through helper `0x140c1aef0`, and used to build generic argument records.
* At `0x1402cfa5b`, the function calls the imported Qt `QMetaObject::invokeMethod` overload. The call uses an object value derived from the current generic-list item, plus a member string and generic argument records. The callsite is not a direct use of the AppState singleton and does not identify the concrete dynamic receiver class.

The companion handler capture `analysis/resume56-dblist-handler.asm` shows `[rsp+0x78]=0` at `0x1406555e1e`, then constructs an output generic argument typed `SfTaskBase *` whose data address is that stack slot. After the wrapper returns, the non-null payload is used for `+0x218` at `0x1406561a4`. This establishes an output-producing dynamic call chain; it does not show that `SfTaskReportGenerate` or any generated receiver is the concrete class executing the method, nor that the implementation writes the global gate.

The original QMetaObject import identity is retained in `analysis/resume57-dblist-import-map.txt`; the exact method/type literals are in CP58. The `strings.txt` entries are not a QObject construction or vtable receipt. No retained export found in the scoped corpus identifies the exact `staticMetaObject`, receiver instance, generated `qt_metacall`, or concrete `+0x218` body for this route.

## Setter and gate lifecycle

* Gate read: Type2 DoeOutput caller `0x140651eba` passes `0x143d766b8` to `0x1402cbec0`; only zero admits the handler. It precedes the QMetaObject call.
* Startup writer: CRT initializer `0x141ca0bb0` passes this exact singleton through `0x1402c7ee0` to constructor `0x1402c7f80`; constructor stores DWORD zero at `this+0x1c` (`0x1402c8030`). This is the only receiver-bound `+0x1c` state transition currently closed.
* Known one-writer: helper `0x1402cbf30` stores DWORD one through its own incoming receiver `+0x1c`; its caller and `rcx` identity are unresolved. CP117/120 searches did not connect it to the singleton; the negative coverage does not rule out aliases or computed indirect dispatch.
* The handler's dynamic effects occur after the current predicate has already run. At most they could change what a later occurrence observes; there is no static field write, setter target, or callback chronology in the retained call sequence.

The process-type setter at `+0x0c` and app event/dispatch global `0x143d76700` are distinct from the gate field. Neither closes the DBLIST owner state.

## Smallest next witness

The smallest useful static witness is not another string or wrapper callsite. It is the concrete meta-object receiver generation and slot table for the object selected from the wrapper's generic-argument list, followed through its `qt_metacall` target and the returned `SfTaskBase+0x218` implementation. To close DBLIST, that chain must show a write/call to `0x1402cbf30` (or an alternate write) with the effective destination proven to be `0x143d766b8+0x1c`, and establish order before a subsequent gate check. No current retained receipt does this.

If native meta-object exports cannot bind those targets, the smallest conclusive witness is a same-instance trace from constructor completion through first handler entry, dynamic dispatch, any setter/alternate writer, and the next gate read. Until such evidence exists, retain startup-zero as a point observation, keep later admission unavailable, and leave public DBLIST guarded.

## Evidence identity and scope

Original binary: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Receipt | SHA-256 |
|---|---|
| `analysis/resume56-dblist-handler.asm` | `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a` |
| `analysis/resume56-dblist-callee-cf740.asm` | `a61b6198d563dc3853599f95393eb5d597f7662a39f9d0b05dc3e9a03f6b751d` |
| `analysis/resume57-dblist-import-map.txt` | `f39f39a2ff5b0b47f44e5fdb092932f4ba8001b3e508bd3247b71bba92a3dbaa` |
| `analysis/resume58-dblist-literals.txt` | `3968c7c718dfc5e20ef4ccbac9fb08677442c39018909adfdd7587d5e90cf8d9` |
| `analysis/resume116-dblist-setter-slice.md` | `38b5a2c21b327afcc6d7150ea27a6b7d1fbb1366b43730245c92c9b5cd345de1` |

Read-only retained-PE review. No GUI, mutable REA database/session, original app execution, source, project data, or build was used.
