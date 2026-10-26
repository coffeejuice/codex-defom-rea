# CP235 page-state identity implementation review

**PASS — the source extraction preserves the reviewed predicate and admission point; no unresolved defect found after test repairs.** This is an independent static source/test review. Root owns integration, the complete suite and freeze.

Reviewed the CP233 editor proposal and independent design review, CP235 implementation note, current declaration/body/caller diff, owner tests and parked-viewer direct assertions. This is a reconstructed C++ adapter contract, not an original native editor handler or new original-parity result.

## Source assessment

`pageState()` keeps its OPI expiry check first and the eager `model_.get()` statement immediately afterward, before the conditional chain. The chain remains constructor state, page existence, page completeness, non-null model, source-model identity, then control identity. Only the final operand changes from raw inequality against `simCtl()` to negation of `matchesCurrentSimControl`. No earlier model query, recursive page query, refresh or page mutation is added.

The const model query calls `requireGeometrySyncAvailable()` exactly once, resolves `step ? &step->sim_ctrl : nullptr`, and returns direct pointer equality. It never dereferences expected or returns a control alias. Unlike the mutation commands, null/null deliberately returns true after admission. Expected null or mismatch cannot bypass the geometry guard. No control field, table, widget, Stopping/cache1 or coordinate state is accessed or changed.

The query remains potentially throwing and `pageState()` retains `noexcept`. Therefore a reached parked guard throws directly from the query, while the same exception crossing `pageState()` invokes termination. No catch/translation or exception-policy change was introduced. This is source-derived pre-existing C++ behavior; no original-binary termination behavior is established. Existing earlier guards still short-circuit the query; source inspection, rather than a parked owner death-test, establishes their ordering.

## Tests and repairs

Direct owner-file query cases cover live match, different expected object, live/null mismatch, absent-current/non-null expected refusal and absent-current/null expected success. The replacement owner fixture now retains the old step in a local unique_ptr while installing a fresh step, then checks null `pageState()` and accessible `partialPageState()`. This makes the replacement identity case explicit without address reuse or dereference of an old control.

Review found that the initial parked assertions were duplicate null-expected calls on the same model, despite the implementation note claiming a missing-step case, and that the replacement fixture did not retain its original allocation as proposed. Both are repaired. The parked fixture now calls the direct query with null expected while the step is live, moves the owned step into a local, calls the query again while current step is absent, then restores it immediately before later fixture operations. The existing `refused` helper requires Error(M2M_E_UNSUPPORTED), so ordinary false/true results cannot pass; these are meaningful checks of guard precedence over both mismatch and null/null equality. No nonexistent barrier helper remains. No parked `pageState()` call is made in the shared test process.

The changed test uses existing Testkit assertions/entry point and resolvable includes. Existing partial-page, OPI expiry and model-guard cases remain. No new C assert or unsupported assertion macro was introduced. The repaired implementation note accurately qualifies focused validation and the unexecuted noexcept termination case.

The owner reports Release build `cp235-page-state-identity-repair` and 2/2 focused CTest targets (owner and viewer_box) passing. This reviewer did not build or execute tests and does not independently certify runtime receipts. Root's stopping_cached regression, complete suite and freeze remain separate gates.

## Reviewed identity and limits

Full-file SHA-256 values match the repaired implementation note:

- `engine.hpp`: `953E9319F02452C11E77B3052394D548DD9A8D85C7A66E875DD2FBDB7F4DC781`
- `gui_model.cpp`: `DF037FF2D4BA12291057C99B8FB89D825496AA6F8E1B020F0662C9BE9504E9ED`
- `simcontrol300_owner.cpp`: `EB324255B67B6E205886AD28709A56B15F16E12F35B8AAD5BFA187ECBDC5417A`
- `test_simcontrol300_owner.cpp`: `AF1F0DFBFE652DE6C7C4E3724B704400C3CE3A081CC090A1F8D6BC68BA2F070A`
- `test_viewer_box.cpp`: `443D09152F97FF10DC777DF560E9915A89915982D6B7DDA77DA14A83FCF32903`

No C ABI or Stopping/cache1 implementation changes belong to this leaf; simcontrol300.cpp and stopping_cached test have no working diff. Raw expected tokens, public step ownership and remaining consumers prevent a broader ownership-closure claim. No original process, real widget delivery or fresh native comparison was used.

Only this report was written. No source/test/build/shared recovery state, original, GUI or REA state was changed by this reviewer.
