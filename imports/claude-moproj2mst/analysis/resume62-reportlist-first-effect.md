# Checkpoint 62: ReportList handler first reachable dispatch

## Route and input scope

Keep this distinct from DBLIST. In the retained `FUN_140650e60` call-site, `0x1424e3cb8` is the `ReportList` literal and this branch calls `FUN_140655df0(param_1, node)`; the adjacent `DBLIST` route calls `FUN_1406554a0(param_1, node, param_3)`. The bindings and caller are retained in `analysis/wave2-doe-part-state-leaves-receipt.json` and `analysis/wave2-doe-session-valid-existing-input-decompile.json`, bound to original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

New exact handler capture: [resume62-reportlist-handler-exact.asm](resume62-reportlist-handler-exact.asm), `0x140655df0` through its `ret` at `0x1406561eb`, SHA-256 `8080C8858863141AF989720729C8E2740CBB759DEAF5610359BD16D97A03B2B3`. A wider overlapping capture [resume62-reportlist-655df0.asm](resume62-reportlist-655df0.asm) also exists; it continues into unrelated following functions and should not be treated as one function body past `0x1406561eb`.

## Earliest handler guards

The parent-level global gate is evaluated in `FUN_140650e60` before this handler call. Inside `FUN_140655df0`, the saved second argument is first passed to an unresolved QtCore IAT target at `0x141e0c538` (`0x1406555e42`); a nonzero result branches to common cleanup (`0x1406561aa`). It then obtains a child node through IAT target `0x141e0c5b0` (`0x1406555e63`), tests that result through the same `0x141e0c538` target (`0x1406555eae`), and again skips to cleanup if nonzero. The target identities were not looked up in this task, so describe these as opaque node/child validity checks rather than assigning exact Qt method names.

After those checks, it constructs stack-local argument objects from the node and several strings; this is still local preparation. The first unresolved call boundary that can cross into the main-window/task object path is `FUN_1402cf740` at `0x1406556151`. It receives the global object `0x143d76700` in RCX, an address-like method/member literal `0x1424e3fd8` in RDX, and a stack array of generic arguments. Prior static work identifies this wrapper/IAT edge as Qt `QMetaObject::invokeMethod`; the exact string at `0x1424e3fd8`, concrete invoked method, and behavior are not proven by this capture. The output pointer is initialized to null at `0x1406555e1e`; when it remains null, the handler skips the later object dispatch.

## First observable model dispatch and owner

If the invoke output at stack `+0x78` is non-null (`0x140656156`), the handler builds another local value and calls an unresolved QtCore function at `0x141e0c698` (`0x14065617e`), apparently deriving a value from the earlier node. It then loads the vtable from the returned object and invokes virtual slot `+0x218` at `0x1406561a4`, with that returned object as RCX, the locally derived value as RDX, and `r8b=1`. This is the first explicit nonlocal virtual dispatch in the bounded handler. Its receiver owner is the object returned through the `QMetaObject::invokeMethod` output; statically it is reached via the main-window/global receiver path, but its dynamic type, returned ownership, and slot implementation are unresolved here. Do not call `param_1` the storage owner based solely on the caller's arguments: it is not the receiver at this virtual call.

The handler then sets its local return byte to one and cleans up. All paths converge on a byte return. No direct filesystem API or side-file write is established within the exact handler range, but unresolved invoke/virtual callees prevent a no-side-effect claim. A successful/non-null dispatch is not evidence that a ReportList side file was persisted.

## Limits and next evidence

This is bounded static evidence only. It identifies parent gate → local node checks → invokeMethod boundary → returned-object `+0x218` dispatch as the reachable chain. Resolve the exact `0x1424e3fd8` member string and the `+0x218` implementation/receiver class before describing ReportList ownership or effects. Keep public DOE activation guarded; this task did not use GUI, source, build, runtime, or mutable REA.
