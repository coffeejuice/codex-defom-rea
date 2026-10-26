# Checkpoint 58: DBLIST invoke literals and receiver candidate

## Exact literals recovered from the original PE

The handler's constants at `0x1424e3fa8`, `0x1424e3fc0`, `0x1424e3fc8`, and `0x1424e3fd8` were read directly from the original executable's `.rdata`, using the PE image base and section raw offset. The input SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Their bounded raw bytes and file offsets are preserved in [resume58-dblist-literals.txt](resume58-dblist-literals.txt).

The exact NUL-terminated values are:

| VA | Bytes decode as | Handler use |
|---|---|---|
| `0x1424e3fa8` | `SfTaskReportGenerate` | Data string passed into a generic argument construction sequence |
| `0x1424e3fc0` | `QString` | Type string paired with the preceding data string |
| `0x1424e3fc8` | `SfTaskBase *` | Type string paired with local pointer storage `[rsp+0x78]` |
| `0x1424e3fd8` | `2signalGetTaskBase()` | Member/signal name passed to the `QMetaObject::invokeMethod` wrapper |

This directly resolves the member string and the pointer type used in the handler, correcting checkpoint56/57's unresolved-string boundary. `SfTaskReportGenerate` is the exact data literal. Its role as a task/report selector is suggested by the spelling and generic-argument setup but is not independently proven by a runtime observation.

## Candidate receiver class and exact remaining virtual target

The wrapper's IAT edge is the decorated Qt `QMetaObject::invokeMethod` overload, as mapped in `resume57-dblist-import-map.txt`. The handler creates a 16-byte generic argument using type `SfTaskBase *` and data address `[rsp+0x78]`. That stack slot starts null. After the invoke wrapper returns, the handler tests the qword there; if non-null, it calls the object at vtable offset `+0x218`.

The narrow candidate receiver type is therefore **`SfTaskBase*`**. The static evidence does not establish the concrete dynamic subtype, nor prove whether the invoked method writes or otherwise changes this argument storage. It does establish that the eventual `+0x218` call is made on the local payload typed `SfTaskBase *`, rather than on a directly loaded stable field of the part object.

The exact remaining indirect target is **the function pointer in the vtable at byte offset `0x218` on the non-null `SfTaskBase*` value in `[rsp+0x78]`**. The PE import data cannot turn this into one concrete code address because the instance's runtime vtable pointer is not present in the static callsite. The type spelling is a base pointer and the actual object may have a derived vtable. Resolving the target requires either the concrete result object/vtable from a source-qualified construction path or an original runtime observation; do not substitute the base-class vtable or guess a virtual method name.

This leaves side-effect semantics open. `QMetaObject::invokeMethod` success/failure behavior, the actual method implementation, any change to the pointer payload, and the virtual `+0x218` implementation's effects remain unobserved. No static evidence here demonstrates a file write, but the dynamic targets are not closed. Keep public B127/Type2 handling guarded and the pure DBLIST parser detached.

## Scope

Only PE header/section metadata, four bounded original `.rdata` strings, the retained handler/callee DUMPBIN captures, and already imported static analysis notes were used. No REA/Ghidra session, original runtime, GUI, source, or build was used or changed.
