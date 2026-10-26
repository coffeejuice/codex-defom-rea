# CP189 R300 restart independent source review

PASS for the bounded restart lexical closure. No source-review blocker found. Build, Testkit preflight, focused save_io/save_current execution and freeze remain root work; this review did not execute tests or establish the R300 barrier complete.

Compared corrected-next, restart-plan-review and restart-implementation reports with both current assigned files. Independently rehashed `build/wave3-checkpoint188-verified/source.zip` to `b58d51b9ef44fe78850cbd0dddc4a23e5a8efa369f85025219cbed7e051dff0a` and compared its corresponding entries to live source using a bounded unified diff. Only the restart call and the existing restart test changed. Live hashes match the implementation report:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `1f7a02ece2e6f240cf5ade0339d460432e8804ba908438922a5591823b0d0298` |
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `61405f012034b143d8d25d15ab7fb18c595bf868faa5388dcee103991f0185a9` |

## Borrow, order and unchanged policy

At save_current.cpp:727 the helper still caches `m` through its existing `opModel(p, op)` acquisition and returns false if absent. The restart edit at lines 749–754 introduces no further lookup: it derives `path` before evaluating `m->step.get()`, immediately invokes a lambda with that pointer, and calls only `saveWholeModelKey(p, *step, path, 0, fx)` inside. Its deduced return is void because the call is an expression statement without a return. The lambda is neither named nor stored, and its step parameter is neither retained nor returned. This establishes the requested local nonescape boundary; identity assertions alone would not establish it.

The restart predicate, cached model, single restart step acquisition, flags zero and ignored writer status remain intact. Equipment, materials and earlier writes precede restart; the existing NSTART copy still follows restart. No `withOpStep`, geometry guard or new null branch was introduced. A null step would still be dereferenced, so this is unchanged nullable policy, not null safety. There is no ownership pin or protection against arbitrary destructive reentry.

The archive diff proves equipment admission and serialization, saveMaterials and saveWholeModelKey internals unchanged. In particular, the existing equipment object alias and subsequent simCtl borrow remain successor audit work. This cut does not close those lifetimes.

## Fixture and assertion coverage

TaskKeyRig at test_save_io.cpp:1188 owns its files, Project and ProjectScope throughout the test. Its `Rig files{true}` selects configured corpus `49_MO2_Boolean_Oprations`, project `MO2_Boolean_Oprations.moproj`, task `Task00001` (Rig constructor at lines 70–91). The implementation report correctly incorporates the plan review's corpus correction.

The extended restart test at lines 1529–1587 requires the loaded model/step and nonnull simCtl. It reseeds NSTART sentinels `(17, -23)` separately immediately before all three sync calls: recording, native success and native denied-open. After each it verifies model/step identity and the NSTART copy `(1, sc->nstart)`. The native hook filters on the restart path, checks identity and both pre-copy sentinels, and increments a counter asserted as one after success and two after denied-open. This avoids stale sentinel coverage caused by the preceding recording call. Recording mode has post-call checks; the before-write sentinel observations are specifically native, consistent with the available hook.

Existing nonempty recorded restart output, final native writer slot, recorded/native byte consistency, overlay removal, empty deferred writes and ignored denied-open status remain checked. The hook runs before native open, so the denial case still observes the restart boundary. No detached-step or owner-replacement fixture was added. Whole-model writer tests and equipment writer tests are byte-for-byte unchanged from the archive.

## Evidence limits

Read the retained wrapper static and qualification records. Their exact runtime-function-region limitation and absence of fresh execution remain applicable. The original same-model two-getter/null-success wrapper behavior is separate from this reconstruction's single step acquisition; this patch does not recover those policies. The retained caller-source qualification in corrected-next remains unchanged. Tests here encode reconstructed helper mechanics and internal recording/native consistency, not fresh original byte parity or public GUI Save admission.

Only this report was written. No source, test, shared plan/state, build, GUI/app-control, deployment, original-process capture or REA database was modified or run.
