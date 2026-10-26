# CP125 — bounded Qt meta-object route after DBLIST admission

## Result

No retained static artifact binds the post-gate Qt meta-object/virtual route to the DBLIST setter or singleton field. The available handler/callee captures identify the indirect calls and exact method/type literals, but the retained source-corpus search for `SfTaskReportGenerate` and `2signalGetTaskBase()` finds only the flattened string index, not a class meta-object table, generated metacall body, receiver constructor, or concrete `+0x218` implementation. This is a corpus-limited absence; it does not prove the original PE has no such metadata or effect.

The first `+0x1c` read is still before this route: DoeOutput caller `0x140651eba` admits only on zero, then calls handler `0x140655df0`. Consequently the meta-object route cannot change that same occurrence's gate decision. It remains a possible influence before a later gate read, with no setter caller or later value qualified.

## Retained route and results

The bounded handler assembly `analysis/resume56-dblist-handler.asm` (SHA-256 `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a`) covers `0x140655df0..0x1406561ec`. It initializes local `[rsp+0x78]` to null, supplies it in a generic argument typed `SfTaskBase *`, calls the wrapper, and conditionally dispatches vtable `+0x218` on the non-null result. The handler instructions do not explicitly reference `0x1402cbf30`, `0x143d766b8`, or `+0x1c`.

The retained wrapper range `analysis/resume56-dblist-callee-cf740.asm` (SHA-256 `a61b6198d563dc3853599f95393eb5d597f7662a39f9d0b05dc3e9a03f6b751d`) identifies the dynamic boundary: import slot `0x141e05348` is Qt `QMetaObject::invokeMethod`, per `analysis/resume57-dblist-import-map.txt`. The exact argument strings and their raw locations are in `analysis/resume58-dblist-literals.txt` (SHA-256 `3968c7c718dfc5e20ef4ccbac9fb08677442c39018909adfdd7587d5e90cf8d9`): `SfTaskReportGenerate`, `QString`, `SfTaskBase *`, and `2signalGetTaskBase()`.

I searched the retained static export corpus under `evidence/gui_mo` for the class and method names across `.c`, `.asm`, `.json`, and `.txt`. The only hit was `evidence/gui_mo/strings.txt` (SHA-256 `677d8a9a2731aec19bff0075df8b7d886754d9f44798b76d661c9ef29b41a982`). It provides string identities and multiple occurrences, not a relation from a `QMetaObject` table to a receiver instance or slot implementation. The corresponding retained disassembly/callee files and CP58 literal map likewise stop at the dynamic boundary.

The original PE's known setter `0x1402cbf30` still stores one through its incoming receiver at `+0x1c`. CP117/CP120's bounded PE data/caller searches did not bind a static callsite or function-pointer entry to that helper. The positive constructor reset is CP116: CRT initializer passes `0x143d766b8` through `0x1402c7ee0` to `0x1402c7f80`, which stores zero at `this+0x1c`. This proves startup value after constructor completion only.

## Smallest next evidence

The next static artifact that would advance this route is the concrete native `QMetaObject`/`staticMetaObject` data and `qt_metacall`/slot implementation for the object receiving `2signalGetTaskBase()`, followed through the resulting `SfTaskBase*` vtable `+0x218` target. It must also bind that object's or callee's write destination to `0x143d766b8+0x1c`; finding a class string or a virtual call alone is insufficient. No retained export currently supplies that table-to-receiver generation or write edge.

If the private analysis export does not contain this data, the alternative witness is an original same-instance trace through construction, first gate, meta-object dispatch, setter/alternate writer, and a subsequent gate. Until one of those closes the destination and chronology, retain the public DBLIST guard and do not assume the post-constructor zero survives later dispatch.

## Scope

Read-only inspection of retained PE assembly/decompilation/string exports only. No GUI, mutable REA session/database, original execution, source edit, or build was used. Conclusions are limited to the artifacts and text-search scope named above.
