# CP189 R300 restart lexical borrow implementation

Implemented the corrected restart-only cut. The two owned files are released for root review, focused build/tests and freeze. No build or test was run by this owner.

## Frozen baseline

Before editing, rehashed `build/wave3-checkpoint188-verified/source.zip`: `b58d51b9ef44fe78850cbd0dddc4a23e5a8efa369f85025219cbed7e051dff0a`. Read both matching archive entries (archive paths omit the leading `source/`) and compared them byte-for-byte with live files; both matched:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `2031229bdecbca9e30a90aae8c4dcde16cf455118fbc73403027e6a5df75eb4f`.
- `source/apps/moproj2mst/tests/s/test_save_io.cpp`: `b3bb63a55b79d9b530f360338e8398adb36a141bf95587ddf02e3a236a112fd4`.

## Change and scope

After restart path derivation, a directly invoked void lambda takes `m->step.get()` and contains only the `saveWholeModelKey(p, *step, path, 0, fx)` call. It stores or returns no step alias and performs no additional operation-model lookup or geometry guard. Cached model identity, restart predicate, one step acquisition, flags zero, ignored writer result and following NSTART order are preserved. Null-step dereference policy remains unchanged; no detached-step sync probe was introduced. The pointer parameter is nullable at the type boundary, but this cut does not establish null-step safety.

Extended the existing `native_restart_key_runs_at_the_operation_save_slot_without_replay` test using TaskKeyRig. `Rig files{true}` selects configured corpus `49_MO2_Boolean_Oprations`, project `MO2_Boolean_Oprations.moproj`, task `Task00001`; it is not the corpus46 fixture. Loaded model/step identity and a nonnull simCtl are required. NSTART fields are reseeded before each of the recording, native success and native denied-open sync calls (`nstartGiven = 17`, `startStepNo = -23`). The native before-write callback filters on the restart path, verifies identity and both sentinels, and counts restart attempts. After every sync, assertions require preserved model/step identity and NSTART copy (`1`, `sc->nstart`). Existing byte consistency, final writer slot, overlay removal, empty deferred writes and ignored denied-open status checks remain.

Positive helper mechanics are encoded in the extended test; execution remains pending root's focused validation. No existing null-model admission case was present in this owned test section, and root clarified that no new null policy or detached-step sync call should be manufactured. Existing whole-model writer tests and writer internals remain untouched. Equipment is a separate successor: its future borrow must include object lookup and the full eo lifetime. Later simCtl borrowing and cached-model lifetime remain audit work.

## Evidence qualifications and checks

Read the corrected-next report, independent plan review, S spec section 1, whole-model wrapper assembly and its static/qualification JSON. The retained wrapper region supports same-model getter ordering and status mapping; its original two getter calls and null-success branch are not reproduced by this lexical single-acquisition cut. The historical caller source remains unavailable as qualified in the plan. No fresh original execution or public GUI Save equivalence is claimed. Identity checks do not prove callback nonescape; the immediate body provides that bounded static evidence. This closure provides no ownership pin or arbitrary destructive-reentry protection.

Compared the resulting owned files against the CP188 archive using a bounded unified diff: only the restart call and its existing test changed. Checked owned files for trailing whitespace and malformed CRLF; passed. CMake requests C++20, which supports the existing String equality operator's rewritten inequality used in the hook. Build, Testkit preflight, registered test inventory, focused save_io/save_current execution, independent implementation review and freeze are root work.

Post-edit SHA-256:

- save_current.cpp: `1f7a02ece2e6f240cf5ade0339d460432e8804ba908438922a5591823b0d0298`.
- test_save_io.cpp: `61405f012034b143d8d25d15ab7fb18c595bf868faa5388dcee103991f0185a9`.

Only these two assigned files and this report were written. No shared plan/state, executable, deployment, GUI/app-control, original process or REA database was changed.
