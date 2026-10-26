# CP175 R300 `stepOf` cut plan review

## Verdict

**Conditionally accepted as a source migration plan, with one acceptance wording correction.** The current frozen source has exactly five call expressions to `stepOf` plus the helper definition. The five listed sites and the order/null guidance match their current surrounding code. The proposed `withOpStep` seam preserves the same `opModel` lookup and nullable step, and is synchronous with a constrained `void`/`bool` callback result. No source, build, GUI, original runtime, or shared state was changed in this review.

The plan should not imply that both native and scratch SimCtrl writer paths can be moved wholesale into one callback unchanged: the native writer path and PPV handling have materially separate order. Keep every access to `step` and each writer invocation inside the callback, while retaining native-versus-capture branching, PPV, diagnostics and effect recording at their original positions. The source table already calls for that preservation, so this is a clarification rather than a blocker.

## Source verification

In `source/apps/moproj2mst/src/engine/save_current.cpp`, `rg` finds calls at the `saveSimCtrl`, `rebuildSystemContacts`, `saveObject`, `saveMeshSettings`, and `saveObjProperties` sites (lines 285, 505, 549, 568, 601 in the reviewed snapshot), plus one helper definition (line 117). Thus “five call expressions” is exact.

- `saveSimCtrl` looks up the step and returns false before path computation, native file opening, scratch capture, flag setup, or PPV. Native SimCtrl writes directly to the opened file; scratch mode calls `captureKey` with a writer closure. Recovery-B PPV work follows the KEY writer. Preserve this split and the callback's synchronous lifetime.
- `rebuildSystemContacts` returns before checking `lubSystem`, then checks the lubricant system before local-step allocation and later lubricant/default-pair work. A null step therefore must not clear or mutate contact state.
- `saveObject` looks up before item lookup and path computation. There is no null guard; the step is dereferenced only from a writer callback when path is nonempty. Keep the empty-path false result and existing native/captured writer selection.
- `saveMeshSettings` admits each slot before its individual lookup; null skips that slot. Keep lookup inside the loop and after the slot predicate.
- `saveObjProperties` likewise checks slot-record existence before an individual lookup; null skips just that slot.

`captureKey` invokes its writer synchronously before returning. In `writeTaskKey`, both the captured writer (through `captureKey`) and native stream writer are invoked before return. The `withOpStep` definition in `engine_internal.hpp` directly performs `opModel`, passes `model ? model->step.get() : nullptr`, and returns `std::invoke`'s result. Its `OpStepCallback` concept permits only void or bool, preventing a direct pointer/reference result. As with any callback seam, review must still reject storing the pointer or capturing it into escaping work.

## Test selection

The proposed focused set is appropriate for changed Save I/O, Save-current behavior, dispatch, cached Stopping behavior and ABI stability. The exact test names are project test registrations; the source tree has `tests/s/test_save_io.cpp` and `tests/s/test_save_current.cpp`, and the existing oracle-B case is explicitly `save_current_oracle_b`. Verify the CTest names against the generated test inventory at execution time; a source search of the top-level app `CMakeLists.txt` does not enumerate these registrations because they are likely registered by shared test helpers/subdirectories.

The plan correctly requires oracle-B step-2 cases to be asserted rather than merely printed/skipped. It also correctly makes a nullable-admitted-slot fixture conditional on focused tests not already reaching that case. The existing `save_io` suite contains named native SimCtrl failed-open/PPV tests and object save coverage, but this static review did not establish that it covers null-step admitted slots or every order invariant.

## Acceptance conditions

Proceed only if the implementation:

1. Removes the helper and exactly migrates those five acquisitions, with no returned/stored step alias.
2. Preserves all five caller-specific lookup order and null semantics above, including native SimCtrl versus scratch writer/PPV sequencing.
3. Confirms synchronous `captureKey` / `writeTaskKey` callback execution remains unchanged.
4. Runs the stated focused tests and root's complete source-stable suite; confirms oracle-B step-2 cases execute and pass, and adds a narrow missing-step slot fixture only if current coverage lacks it.
5. Records remaining direct step accesses and makes no claim that Save, the model step field, whole-operation gating, or public Open/Save fidelity is closed.

The plan's stated limitations on public Open/Save witness, reader-tail completion, and cache1/Stopping-barrier installation are consistent with the reviewed source scope.
