# CP234: Boolean timing witness remains external to source

## Result

No new independent source-only step can establish whether constructor E reaches its notifier or whether Boolean Apply adds an object during the first Open. The source fixes conditional order and behavior; it contains no record of a particular original invocation, its owner identities, or event coverage. The existing static and retained-stream checks already cover those useful offline questions. Keep constructor/Open admission guarded.

CP164 zero-slot `postLoad` is reconciled as already implemented and tested: native `postLoad` checks the OPI slot-record count and calls add at `11df0e9` (normal continuation `11df0ee`) when zero, before `propsLoad` and `postLoadViews`. The candidate route is `engine::booleanPostLoad` in `src/engine/l5_slots.cpp`; `tests/mdt/test_boolean_postload_creation.cpp` exercises it through the load lifecycle and distinguishes zero slot records from nonzero records with zero engine objects. This is not the constructor notifier or Boolean Apply/add route.

## Exact missing witness

For constructor timing, no retained same-invocation receipt joins all of: successful Boolean Object-factory `0x50` allocation generation E; constructor entry `11e2480(E, OPI)`; same-E QWidget normal return `377990`; normal wrapper/setup returns through `130830`, including E+38 OPI resolution and the conditional model return if taken; E+28 bind `377b20(E,S)`; and notifier `130ad1 -> 2cf3c0` returning at caller continuation `130ad6`, tied to E and its generation. Retained Editor004 is Forming, not Boolean, and cannot fill these joins.

For Apply timing, no retained same-invocation receipt contains a complete selected fresh Open interval with current-widget UpdateData on E, Boolean Apply `11e2870(E)`, E+38 resolving to OPI O, and a normal `addObject(O)` return at `11e295a` or `11e2bcb`, with owner lifetimes and interval coverage. The `postLoad` return `11df0ee` is a distinct origin. A source call graph can show where a call would occur if reached; it cannot prove the call occurred during an original run, was first, or that unobserved intervals were quiet.

These are separate missing witnesses. One cannot satisfy the other. A positive Apply/add occurrence alone does not prove it was the first during Open; that claim additionally needs gap-free coverage from the selected Open entry through its completion. Timer attribution also needs the actual delivery/connection owner and nested callback chain.

## Next action and boundary

There is no defensible source edit or new candidate test to run now: it would only restate synthetic trace prerequisites already recorded in CP171 and would not reduce the original-evidence gap. The smallest useful next action is deferred until app-control release is supported and verifiable: obtain one fresh owned original capture with the constructor tuple above, and separately capture the complete Open/UpdateData/Apply/add chain if first-Open Apply timing is needed. Until then, preserve the existing guards and label timing unknown. Do not reacquire GUI control under the current lifecycle instruction.

No source, test, build, shared plan/state, original process, GUI session, or mutable REA state was changed. No tests were run because this read-only investigation produced no code/test change. Existing postLoad coverage is cited, not re-executed. The retained coverage miss is not evidence that either native path never runs.

## Evidence reviewed

- `analysis/resume208-rbool-next.md` and `analysis/resume214-boolean-timing.md`: constructor, postLoad and Apply-origin distinctions and their required witnesses.
- `analysis/resume223-boolean-retained-witness-audit.md`: bounded retained-stream audit and pinned checker result; no further static call-site decoding or candidate-only test was justified there.
- `analysis/resume171-rbool-next.md`: synthetic trace-test proposal and its explicit inability to establish native occurrence/timing.
- `source/apps/moproj2mst/src/engine/l5_slots.cpp` and `source/apps/moproj2mst/tests/mdt/test_boolean_postload_creation.cpp`: implemented zero-slot postLoad path and regression.
- `source/apps/moproj2mst/src/ops/boolean.cpp`: current Boolean editor fill/apply delegation; it does not create evidence of native constructor progress or first-Open dispatch.

The subtree checker `python admin/check_continuation.py` was not found at the parent checkout's `admin/` path; no completion claim is made. `python tools/recovery_status.py` reports the broader recovery blocked on unavailable GUI release lifecycle, consistent with deferring original capture.
