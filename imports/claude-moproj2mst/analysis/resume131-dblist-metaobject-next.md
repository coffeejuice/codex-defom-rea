# CP131 — retained meta-object receiver frontier

## Finding

The retained wrapper disassembly resolves one useful identity boundary: `0x143d76700`, passed as the first argument to `0x1402cf740`, is **not passed directly** as the first `QObject*` argument to the imported `QMetaObject::invokeMethod`. The wrapper reads a list-like member at `wrapper+0x88`, obtains the current item, and constructs the invoke call's `rcx` receiver from that item (`[rsp+0x80]+8` at `0x1402cf975..0x1402cf981`). The dataflow does not identify the item class or connect it to AppState `0x143d766b8`.

That narrows the static frontier, but no retained meta-object table or generated slot body follows the item to setter `0x1402cbf30`. There is no source-ready DBLIST integration cut. Preserve the public guard.

## Evidence chain

1. Handler `0x140655df0` runs only after the outer DBLIST gate has read `0x143d766b8+0x1c`. It prepares an output argument typed `SfTaskBase *` at `[rsp+0x78]`, initially null, and calls wrapper `0x1402cf740`.
2. Wrapper `0x1402cf740` saves its incoming first argument, whose known callsite value is `0x143d76700`. It checks byte `wrapper+0x141`, then passes address `wrapper+0x88` into helper `0x1402d03f0` to obtain state used in the later loop.
3. The loop at `0x1402cf820..0x1402cf870` gets an item through `0x1402eee10`, reads its first qword into `[rsp+0x80]`, calls accessor `0x140c1aef0` on the item, and makes an indirect call through the returned object's first vtable slot. The exact item/accessor types remain unresolved.
4. Immediately before invoking Qt at `0x1402cfa5b`, the wrapper loads `rcx=[rsp+0x80]` and adds 8. It supplies `rdx=rdi` (the exact member name passed through the wrapper), connection type 1 in `r8d`, and the prepared argument records. Thus the Qt receiver is derived from the current list item, not directly the global wrapper context.
5. Handler `0x14065618c..0x1406561a4` later checks the returned `[rsp+0x78]` `SfTaskBase *` and invokes that object's `+0x218` vtable entry. This output object is also not proven to be the wrapper item or AppState.

Retained CP58 literals resolve the dynamic method string to `2signalGetTaskBase()` and the output argument type to `SfTaskBase *`. The exact concrete QObject receiver item, its class's `QMetaObject`, the method implementation, and the `SfTaskBase+0x218` target are not present in the retained class/method string index or selected handler captures.

## Setter and lifecycle boundary

CP116 proves AppState construction writes `0x143d766b8+0x1c=0` before `main`. The active gate tests that DWORD before the wrapper invocation. The only retained `+0x1c=1` helper writes through its incoming receiver; no retained caller establishes that receiver as `0x143d766b8`, and no later same-instance reset is established. The wrapper's post-gate invoke can, at most, affect a later gate occurrence; no retained code shows it writing the gate.

The distinct process-type field `+0x0c` and the wrapper context `0x143d76700` do not establish the gate field's later value. Converting the constructor zero into a per-Session/per-run default would not preserve the native application lifetime.

## Smallest next witness

The smallest useful artifact is the concrete object type/creation path for an item returned by `0x1402eee10` from `wrapper+0x88`, followed by its `QMetaObject` method implementation for `2signalGetTaskBase()`. If that route yields `SfTaskBase`, its concrete `+0x218` target must then be followed. The required proof endpoint is still a write/reset of `0x143d766b8+0x1c` before a later gate read, or a call to `0x1402cbf30` whose effective `rcx` is proven equal to that singleton. Existing receipts stop earlier.

If the retained PE exports do not supply that owner/dispatch chain, an original same-instance trace from constructor completion through dynamic invocation and a later gate read is the smallest conclusive witness. Until then, keep DBLIST admission unavailable and preserve the public refusal.

## Identity and limits

Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Receipt | SHA-256 |
|---|---|
| `analysis/resume56-dblist-callee-cf740.asm` | `a61b6198d563dc3853599f95393eb5d597f7662a39f9d0b05dc3e9a03f6b751d` |
| `analysis/resume56-dblist-handler.asm` | `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a` |
| `analysis/resume57-dblist-import-map.txt` | `f39f39a2ff5b0b47f44e5fdb092932f4ba8001b3e508bd3247b71bba92a3dbaa` |
| `analysis/resume58-dblist-literals.txt` | `3968c7c718dfc5e20ef4ccbac9fb08677442c39018909adfdd7587d5e90cf8d9` |
| `analysis/resume116-dblist-setter-slice.md` | `38b5a2c21b327afcc6d7150ea27a6b7d1fbb1366b43730245c92c9b5cd345de1` |

Read-only retained PE review. No GUI, mutable REA session/database, original runtime, source edit, or build was used.
