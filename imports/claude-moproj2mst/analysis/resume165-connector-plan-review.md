# CP165 connector independent plan review

## Decision

**Conditionally source-ready after CP164 root-association repair is implemented and its negative test passes.** The proposed cut is a coherent next normal-path increment on the same sparse private object, but the current CP164 predecessor is not yet an accepted frame: the recorded source review demonstrates that a factory can pair an allocation store from scope A with `WidgetRuntimeState` from scope B and charge B's QList record. Do not implement CP165 against that uncorrected admission path. The plan correctly makes the root repair a prerequisite and does not treat the production `ConversionSession` pairing as enforcement of the general factory API.

This is a static independent review. I did not edit source/tests/shared plan/state, build, control GUI, or use live REA.

## Native slice and supported effect

The selected QtCore slice is exact: RVA `[0x131198, 0x1311a2)`, 10 bytes `48 8d 05 71 8b 07 00 48 89 03`, raw offset `0x130598`, SHA-256 `f64d5bb907ab12eadf9b4549a2e4122c93b3a193a1bbb35010d2add052277ec6`. At preferred VA `0x67131198`, the RIP-relative LEA resolves to QtCore RVA `0x1a9d10`, exported as `??_7QObjectPrivate@@6B@`; at `0x6713119f`, the store writes that address point to `[RBX]`. Prior code has retained the same private allocation pointer in RBX. The proposed end cut at RVA `0x1311a2` is immediately before the QString shared-null load, so this slice supports only changing that object's private address point from `0x1a9a30` to `0x1a9d10`.

The byte evidence does not establish effects from the following QString path, a new owner/reference, a string, counters, a task, backlinks, or QObject completion. Avoid claiming any such change. Do not infer whole-function or unwind behavior from the displayed disassembly/unwind extent. Keep the binary identity qualified by QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and store the module digest plus RVA, never a host pointer.

## Required source admission and once-only checks

Before the CP165 write, the implementation should establish all of the plan's frame guards together: one pending Report and unchanged private-object identity; cursor exactly `BeforePrivateFinalAddressPoint`; `qobjectBaseCompleted == false`; CP164 committed marker true; a live weak binding whose referent is pointer-identical to the qualified QList record in the expected session/allocation scope; and that root record still has the expected QtCore digest and RVA `0x2f0a50`. The expected scope/root identity must have been bound at CP164 allocation admission, before a foreign root could be selected. Checking only the private weak binding against the factory's currently supplied widgets would preserve a bad CP164 association and is insufficient.

On refusal, preserve the pending/private identities, CP164 binding and one symbolic delta, address point `0x1a9a30`, and cursor; do not replay the CP164 increment. On admission, mutate only `addressPointRva`, advance to an explicit once-only phase at the pre-`0x1311a2` cut, and leave the CP164 root delta unchanged. Reentry with same and different keys must refuse while preserving that state.

## Focused acceptance coverage

The listed tests are appropriately scoped if they assert state after each attempt, not only the final result:

1. Repair CP164's store/root scope association. Pair store A with widgets B; require refusal before the CP164 `+0x18` event, with no binding/delta/cursor advance. Also prove the valid `ConversionSession` pair remains admitted. This is a hard prerequisite, not a CP165 test substitute.
2. On the valid CP164 frame, capture pending/private/root identities and delta; advance once and require the same identities, unchanged root delta, address point `0x1a9d10`, and cursor before `0x1311a2`.
3. Attempt same-key and different-key reentry, checking the retained frame state after each refusal. Check absent/expired/mismatched/uncommitted root binding refuses without the address-point write. Retain assertions for no SessionTask, empty maps, false completion, and unavailable backlinks.
4. Run the focused factory and root-owner tests, then the normal source-stable checkpoint build/regression gate. This review does not report those executions as done.

The plan appropriately does not claim an absolute QList count, live constructor occurrence, null-allocation result, reference release, TaskInfo owner, or effects from the next instruction window. It should continue to avoid string or counter effect claims.

## Conclusion

The native instruction selection and expected narrow model update are supported. Source readiness is conditional on repairing CP164's root association at the earlier admission seam and passing the foreign-pair negative test. Until then, CP165 must not advance from the currently unaccepted predecessor.
