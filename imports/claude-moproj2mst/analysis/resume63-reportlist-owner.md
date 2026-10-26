# Checkpoint 63: ReportList virtual owner and first downstream effect

## Starting boundary

The ReportList path is distinct from DBLIST. Retained `FUN_140650e60` tests tag literal `0x1424e3cb8` (`ReportList`) and calls `FUN_140655df0(param_1,node)`; `DBLIST` at `0x1424e3cb0` calls `FUN_1406554a0(param_1,node,param_3)`. See `analysis/wave2-doe-session-valid-existing-input-decompile.json` and literal receipt `analysis/wave2-doe-part-state-leaves-receipt.json`, both bound to original PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

At `FUN_140655df0+0x3a1` (call `0x1406556151`), the output storage is statically typed by generic argument as `SfTaskBase *`, initialized null. The selector is `SfTaskReportGenerate`, and the member is `2signalGetTaskBase()`; these literal bytes and the wrapper argument mapping are retained in `analysis/resume58-dblist-literals.txt` and `analysis/resume59-dblist-vtable-bytes.txt`. The invocation is through the main-window global `0x143d76700`. Afterward the handler checks for non-null output and invokes its virtual slot `+0x218` (`0x1406561a4`) with the output pointer as receiver, a DOM value derived from the ReportList node as the second argument, and `r8b=1`.

## Candidate vtable target

The receiver static type is `SfTaskBase*`; its dynamic type is not encoded at the callsite. RTTI in `analysis/resume59-dblist-vtable-bytes.txt` records a primary vftable for `SfTaskReportGenerate`, deriving through `SfTaskReportBase` and `SfTaskBase`. Since `0x218 / 8 = 67`, its entry 67 is `0x1407a0150`; the base `SfTaskBase` table has a different entry `0x14140cba0`. The exact bounded target capture is [resume63-reportgenerate-slot67.asm](resume63-reportgenerate-slot67.asm), `0x1407a0150` through `ret` `0x1407a02bb`, SHA-256 `1BB1996696A22FAD159005A87AED7A19BE5C6BAD89D5C694E5FD609098C489B7`.

The selector `SfTaskReportGenerate` plus the returned base-pointer type makes `SfTaskReportGenerate` the strongest static subtype candidate. It does not prove that the signal implementation returns this instance in the original lifecycle, nor exclude a different dynamic subtype. Keep public dispatch guarded until the return implementation/receiver is closed.

## Ordered behavior after the virtual call

For the ReportGenerate candidate, slot 67 (`0x1407a0150`) checks whether its incoming DOM node is null, obtains its first child, checks for no child, and walks sibling elements. It compares each child's tag to literal `OutputList` at `0x14269e3c8`. Only a match reaches a second virtual call at slot `+0x240` (`0x1407a0242`) on the same returned task object, with a value derived from the child node as its argument. The slot-67 body itself shows no direct store into task fields; it delegates the potential update to slot `+0x240`.

The bounded prior capture [resume60-dblist-reportgenerate-slot72.asm](resume60-dblist-reportgenerate-slot72.asm) covers ReportGenerate slot 72 target `0x1407a0500` through return. That routine builds DOM-derived records and interacts with an object-associated collection; exact collection ownership and helper semantics remain unresolved. It establishes a possible in-memory model update if an `OutputList` child exists, not an on-disk write. The retained B127 `.DS` has an empty `ReportList`, so it does not supply an `OutputList` child; this is a static shape comparison, not a runtime execution result.

Thus the first possible model mutation downstream of the ReportList handler is conditional: signal returns a task object; its dynamic vtable slot 67 must be the ReportGenerate implementation; the ReportList DOM must contain a direct `OutputList` child; then slot 72 is invoked. On the retained empty fixture, that conditional delegation is not taken under the ReportGenerate hypothesis. No side-file effect is established by these bounded routines.

## Evidence limits and action

The unresolved items are the original signal/slot return implementation and concrete object, other possible `SfTaskBase` vtable overrides, the `OutputList` record collection's owner/effect, and any effects of helpers called by slot 72. The first explicit dispatch owner is the signal-returned `SfTaskBase*`, not the part model `param_1`; the best candidate is a `SfTaskReportGenerate` instance, conditional on the unresolved dynamic return. Do not enable the public B127 path from this evidence. This task used only retained static PE/decompilation and a bounded DUMPBIN capture; no GUI, source, build, runtime, or mutable REA was used.
