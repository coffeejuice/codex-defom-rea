# CP88: Boolean failure branches are not all blocked by callback timing

There is an independent implementation increment: preserve the native Boolean side-file helper's integer result and early owner-failure branch while retaining BWZ-then-INI continuation. It requires no decision about whether an Open timer fired. Current candidate code already implements most physical attempt/failure order, so those branches must not be reported as newly missing. The remaining first-Open mutation/reached-public-owner question is separately schedule/state dependent; it does not dominate every local Open/Save contract.

Read-only lane: only this report was written. No source/build/tests/GUI/native execution/mutable REA. One bounded DUMPBIN read of immutable1405f8d90 through its RET confirmed the owner-guard resolver; tool banner14.44.35228.0. Original EXE identity remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Native independent branch:1411e1820 result/failure order

Retained complete `wave3-boolean-lifecycle-1411e1820.asm` establishes:

1. Read BooleanData owner-properties pointer at data+10. If null, take1866 and return integer-1 through1879/1954 without side-file attempts.
2. Otherwise resolve the properties' owner using1405f8d90. New direct decode proves this adds0x20 to properties `this`, calls guard resolver140c1aef0 at5f8da2 and returns at5f8dab. A null resolved owner takes the same-1 branch. This is a guarded owner relation, not just arbitrary pointer presence or a timer-due value.
3. On valid owner, derive task path, call BWZ writer1411e1970 at1411e18fd, and save its integer result at1411e1902.
4. Call control-INI writer1411e0e40 at1411e1938 regardless of BWZ's returned0/1. There is no BWZ-result branch before this call.
5. Reload the saved BWZ result at1411e193d/1950 and return it. INI's returned status does not replace the result.

Thus the source-closed result algebra is:

| Owner resolution | BWZ returned result | INI attempt/result |1411e1820 result |
| --- | --- | --- | --- |
| Null data-owner pointer or resolved owner null | No attempt | No attempt |-1|
| Valid owner |0|Still attempted; either result|0|
| Valid owner |1|Still attempted; either result|1|

Unknown owner evidence is not the native null/expired branch. A faithful typed projection must keep unavailable resolution guarded rather than defaulting it to-1 or inventing a live owner.

Individual BWZ/INI writers open the final destination in WriteOnly mode without creating parents. Failed open returns failure. BWZ's opened path explicitly closes, then stores result1 at1411e1c62; selected code does not branch on stream status/close result. Therefore this result means the native writer's selected return path, **not** a promise of durable complete bytes. Do not replace it with `!Stream::writeFailed`, a flush bool, a generic M2M error or the later INI result. Failure exceptions from a test callback are not source-equivalent returned QFile failure; no catch-and-continue policy is inferred for them.

## Current candidate: implemented order versus missing result

`booleanSideFiles` in `engine/boolean_editor.cpp` is void. Its nativeMstSideFiles path uses FileDevice, skips serialization after failed open, closes the direct device and continues to the independent second emit. The helper's void signature discards the native integer-result contract. Its early `!op.project || !op.props` guard is an adapter check; it is not a complete representation of native data+10 ->properties+20 guarded owner resolution and unavailable state.

Current physical failure order is already present and has named tests in source:

| Contract | Current inspection |
| --- | --- |
| MST open failure prevents MST generation/its side effects but preserves earlier property-pass files | `save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects`, `save_missing_mst_parent_keeps_step4_boolean_files` |
| BWZ open failure still attempts INI and finishes MST | `save_bwz_open_failure_still_attempts_ini_and_finishes_mst`, current-Boolean variant |
| INI open failure preserves BWZ and finishes MST | `save_ini_open_failure_keeps_bwz_and_finishes_mst` |
| Project writer failure does not itself suppress later Boolean/MST work | `save_project_open_failure_preserves_project_but_continues_boolean_and_mst` |
| Stream failure after a successful MST open retains generation side effects | `save_mst_stream_failure_after_open_keeps_generation_side_effects` |

These are inspected test definitions, not tests run in this lane or new original validation. The older lifecycle-failure report's “current difference” table described its earlier candidate; current native FileDevice/Stream/coordinator code supersedes those implementation-difference claims. Its original PE contracts remain evidence. Do not re-implement already corrected deferred-output behavior merely from that old table.

The property pass before MST and the Boolean MST writer's later same-helper call remain distinct occurrences. MST writeBlock ignores the helper result and emits EXECMD afterward on its selected normal path. Preserving the integer internally must not newly stop generation, change m2m status mapping, or remove the second occurrence.

## Smallest implementable increment without a timer witness

Add an isolated native side-file result/owner-resolution seam, or an internal returning helper beneath the existing void adapter. Its input must explicitly represent the actual data-properties/operation owner chain as available, null/expired or unavailable; unavailable refuses. With an admitted owner/path, it invokes the existing BWZ writer, retains that integer, invokes INI unconditionally and returns the retained integer. A void compatibility wrapper may discard the result where the original caller ignores it. Keep recording/staged adapter behavior separate: planning a future write is not an observed native writer result.

Focused deterministic tests can cover null properties owner, resolved-null owner, unavailable guard, BWZ0/INI0, BWZ0/INI1, BWZ1/INI0, BWZ1/INI1; attempt order and exact returned integer; and opened writer with declared stream/close failure still taking the source-defined return1 path. Foreign or stale owner generations must not be treated as live. No test needs a timer, modelUpdated occurrence, synthetic due list, first-Open object mutation or GUI. This is result-contract recovery, not an assertion of a currently observable public-output regression.

Actual owner binding/physical I/O integration remains separately scoped work. The new seam must not accept a bare `ownerReady=true` as public constructor completion or invent task path from an unavailable owner. No source was assigned/changed by this report.

## What still needs runtime timing, and what does not

- **Runtime timing required:** choosing the first fresh-Open Apply/addObject occurrence, whether a registered1000ms event fires before a selected Open completion or Save-side boundary, and which current model/item/editor it sees. CP82/87's exact enclosing-boundary and positive mutation witness remain necessary. `1c94` is loader return, not whole Open completion.
- **State/lifetime witness or recovered constructor execution required, not intrinsically a timer witness:** current window+3c8, actual area/impl registration, current widget and QObject guard resolution. CP75/77 identity helpers do not populate them. A direct field/guard observation or source-qualified constructor continuation could supply these without measuring a timer deadline.
- **Local source contracts independent of callback schedule:** side-file owner-failure/result algebra above, direct failed-open continuation, serialization/close order, fixed formatting and loader token/default branches given their actual inputs. Open's current BWZ parser/default/missing-file behavior exists in `datafiles/wizards.cpp`; its warning/dialog and broader TokenReader qualification are separate dependencies. No new native parser bug is claimed here.

Therefore it is not valid to conclude that every missing public step depends on the callback schedule. Full fresh Open-to-Save acceptance remains blocked by the unresolved state/timing/effect composition, but bounded failure/return contracts can still be implemented and reviewed independently. A local result seam would not authorize public first-Open activation or declare native due IDs.

## Inspection hashes

Paths relative to `source/apps/moproj2mst/`: `src/engine/boolean_editor.cpp` SHA `5aab83cb71d5eedc69ad1476c4e5ff2adc566687655694e59e3c3d949ff9a44f`; `src/ops/boolean.cpp` `4d881b968ca13eb9e78fb57114b4e062358f870e2a075f7c9ce7740cecb2ab8e`; `src/core/convert.cpp` `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30`; `src/datafiles/wizards.cpp` `ed4dbcd5eaed65f3083758909129fd51b7b1d0d0a45e2692eb0bd5bab95841af`; `tests/s/test_save_io.cpp` `83fec33ee033409b1c0faf90aa9e15d8d82200800a0c2b3c50e55367f745cafb`.

Native retained helper asm SHA `b401d8348f9e0e7d84eb75f4e4415b270177ff4010b43f13278216a9d9a6824b`; original failure-static receipt SHA `e76a5ad3d9003adb0f60ddb08cc41238650161c23c5b048b2c845648c5d452ef`. No current build/test or native-execution claim follows from inspection.
