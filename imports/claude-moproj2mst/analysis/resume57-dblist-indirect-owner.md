# Checkpoint 57: DBLIST dynamic dispatch and receiver source

## IAT slot identity

The callee capture `analysis/resume56-dblist-callee-cf740.asm` calls through IAT slot `0x141e05348`. DUMPBIN `/IMPORTS` lists the `QtCore4_64.dll` IAT base as `0x141e03b60`; the byte delta is `0x17e8`, or 765 eight-byte entries. Entry 765 in the QtCore import order is exactly:

```text
?invokeMethod@QMetaObject@@SA_NPEAVQObject@@PEBDW4ConnectionType@Qt@@VQGenericReturnArgument@@VQGenericArgument@@444444444@Z
```

The bounded import-order calculation and neighboring entries are preserved in [resume57-dblist-import-map.txt](resume57-dblist-import-map.txt). Together with the callee disassembly at `0x1402cfa5b`, this identifies the unresolved IAT edge as Qt `QMetaObject::invokeMethod`, not an arbitrary application file writer. The wrapper prepares arguments and invokes a method by name; its returned byte is saved locally. The exact runtime method name string passed by the DBLIST handler at `0x1424e3fd8` has not been decoded, and the invoked QObject target is the result of an unresolved object-access path inside the wrapper.

## The `+0x218` receiver is argument-backed, not a demonstrated part field

In the full handler capture, stack slot `[rsp+0x78]` is first initialized to zero (`0x1406555e1e`). At `0x1406555f01`, its address is passed as the third argument to `FUN_1400ac6f0`, alongside type-name pointer `0x1424e3fc8`; the result is copied into a 16-byte argument record and later passed among the generic arguments to `FUN_1402cf740`. Retained call-site analogues in `evidence/gui_mo/out_scope/opsave2.c` and `evidence/gui_mo/s1/mdt/cur_d2.c` use `FUN_1400ac6f0` in the same construction position as explicit `QGenericArgument` constructors (the former supplies the known `int` type-name literal). This supports identifying the helper as a QGenericArgument-style constructor; the exact type name at `0x1424e3fc8` remains unknown.

After the `QMetaObject::invokeMethod` wrapper returns, the handler tests the qword stored in `[rsp+0x78]`. Only if non-null does it call the indirect method at vtable offset `+0x218` on that qword (`0x14065618c`–`0x1406561a4`). Thus the eventual virtual receiver is sourced from the local storage supplied as one generic argument to the dynamically invoked method. The assembly does not prove whether the invoked method writes or otherwise changes that storage. It does not show this receiver being loaded from a stable offset of the incoming part pointer. No concrete vtable/class candidate can be selected without decoding the type/member constants or resolving the invoked method and its implementation.

## Exact unresolved branch and consequence

Static evidence now closes the following path:

1. The caller passes its part-level object and the direct DBLIST node to `FUN_140655df0`, but dispatch occurs only when the global object's 32-bit `+0x1c` is zero (checkpoint56 gate capture).
2. The handler builds a generic argument whose data storage is `[rsp+0x78]` and calls the application wrapper `FUN_1402cf740`.
3. The wrapper reaches the exact Qt `QMetaObject::invokeMethod` import at IAT `0x141e05348`.
4. If the local argument storage is non-null afterward, the handler dispatches vtable `+0x218` on that value; otherwise it skips that call. The handler's common visible return is true.

Still unresolved: global gate field semantics/state; the member string at `0x1424e3fd8`; generic type string at `0x1424e3fc8`; object-access path that supplies the QObject receiver to `invokeMethod`; whether/how the method changes `[rsp+0x78]`; class and target at virtual `+0x218`; and all side-file/exception effects of those dynamic targets. The DBLIST value is not yet mapped to an owned candidate field. Keep the pure parser detached and do not enable public B127/Type2 handling.

## Scope and provenance

The original PE SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Evidence is DUMPBIN static import/disassembly plus retained static call-site analogues only. No original runtime, REA/Ghidra session, GUI, source, or build was used or changed. New evidence artifact: `analysis/resume57-dblist-import-map.txt`; no additional assembly range was required because the handler tail and direct callee were already preserved in uniquely scoped checkpoint56 captures.
