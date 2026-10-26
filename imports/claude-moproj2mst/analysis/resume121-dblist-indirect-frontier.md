# CP121 — DBLIST setter's remaining indirect frontier

## Result

Retained PE evidence places the DBLIST handler's dynamic calls **after** the `+0x1c` admission read, but does not connect either dynamic target to setter `0x1402cbf30`. Thus the constructor-established value is zero before `main`, and the handler may later execute unresolved Qt meta-object/virtual behavior only after the admission gate has already allowed that handler call. Whether that behavior changes the field for a subsequent DBLIST occurrence remains unknown.

The current PE receipts do not show a static setter target/receiver edge. CP117's narrow address-form checks found no setter VA/RVA operand or absolute/RVA function pointer in file-backed non-executable sections. CP120's `.pdata` correlation found no function containing both a direct singleton-base LEA and an immediate `+0x1c` store. This report follows the retained handler's unresolved indirect-call chronology instead of repeating those searches.

## Static chronology on the recovered DoeOutput path

1. The Type2 DoeOutput caller supplies literal zero as the DBLIST-enable argument and calls predicate `0x1402cbec0` with singleton `0x143d766b8` at `0x140651eba`. The predicate admits iff `*(DWORD*)(singleton+0x1c) == 0`. Only then does it call DBLIST handler `0x140655df0`.
2. Handler `0x140655df0` initializes local `[rsp+0x78]` to null. It prepares generic arguments and calls the wrapper `0x1402cf740`; that wrapper reaches Qt `QMetaObject::invokeMethod` through IAT `0x141e05348`. The handler's exact string is `2signalGetTaskBase()` and the pointer argument is typed `SfTaskBase *` (retained CP58 literals/call setup).
3. If the local pointer is non-null after the dynamic invoke, handler instructions `0x14065618c..0x1406561a4` dispatch vtable offset `+0x218` on that pointer. Both the meta-object target/receiver and concrete vtable implementation remain unresolved. The handler then returns its common true result at `0x1406561eb`.
4. The retained handler assembly contains no explicit operand reference to setter `0x1402cbf30`, the gate global `0x143d766b8`, or the `+0x1c` field. No direct setter call appears in the visible body. The two unresolved calls could have effects beyond the visible handler; the capture does not identify them as DBLIST-field writers.

Therefore the meta-object/virtual path cannot change the already-completed gate read for its current handler occurrence. It is a possible but unproven influence on the field before a **later** occurrence. Nothing in this static sequence establishes that the setter is called, establishes its `rcx`, or proves another gate reset.

## Positive initialization and value boundary

CP116 binds the pre-main CRT initializer to singleton `0x143d766b8`, then through `0x1402c7ee0` to `0x1402c7f80`; that constructor stores `this+0x1c=0` at `0x1402c8030`. A source-backed statement can therefore say the field is zero when the AppState constructor completes. It cannot be extended to a later public import without closing the intervening field writers or following the dynamic target's effect.

The exact missing edge is either:

* a static dispatch target/receiver chain from the retained `QMetaObject::invokeMethod` or `SfTaskBase+0x218` path to `0x1402cbf30`, proving its receiver is the AppState singleton; or
* a source-qualified other writer/reset chain to singleton `+0x1c` that determines the value before the next gate read.

If PE evidence cannot recover this chain, a same-instance runtime trace must capture field value and setter call stack/`rcx` from constructor completion through first and subsequent DoeOutput gates. Until then, preserve the DBLIST public guard. Startup zero is not a per-run candidate default.

## Exact range and evidence receipts

Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Evidence | Covered span or fact | SHA-256 |
|---|---|---|
| `analysis/resume56-dblist-handler.asm` | Handler `0x140655df0..0x1406561ec`; exact full disassembly capture | `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a` |
| Original PE bytes for handler span | File offset `0x6551f0`, length `0x3fc`, end exclusive | `892a0e0f88022aea7d8d2a1f20e9e02c9d731537407639e1777c4fb64d0a0b76` |
| `analysis/resume62-dblist-register-and-caller.asm` | Admission callsite context, including `0x140651eba` | `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1` |
| `analysis/resume57-dblist-indirect-owner.md` / CP58 literals | QMetaObject import, `2signalGetTaskBase()`, `SfTaskBase *`, unresolved dynamic targets | Captures and limits summarized there |
| `analysis/resume116-dblist-setter-slice.md` | Constructor chain and `+0x1c=0` reset | `38b5a2c21b327afcc6d7150ea27a6b7d1fbb1366b43730245c92c9b5cd345de1` |

Read-only retained PE/disassembly analysis. No GUI, mutable REA database, original app execution, source, project data, or build was used or changed.
