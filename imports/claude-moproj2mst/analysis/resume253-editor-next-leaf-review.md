# CP253 initial editor admission leaf review

**PASS — replacing the selected conditional with `!control || model.dimension() != 3` preserves the supported synchronous adapter admission.** No additional model API or geometry admission is needed. This review did not implement or execute the change.

Reviewed CP252 against current `openInitialEditorInto`, `GuiModel::dimension`, the initial session/report definitions, lifecycle fixtures and existing dimension tests, with CP226/242 retained native-versus-adapter qualifications.

## Exact equivalence and order

Keep report reset first, AlreadyStarted refusal second, and `session.constructionStarted=true` before the single `model.simCtl()` call. The new condition must retain `!control` as its first short-circuit operand. For missing step, simCtl returns null and suppresses the scalar query. For a live step, actual 3 passes and every other dimension, including zero, refuses. The unguarded getter returns the same live integer; its absent-step zero also would fail equality-to-three, although the supported missing-step path short-circuits first.

There is no callback between the current lookup and scalar condition in the inspected source. This equivalence does not establish behavior for concurrent destruction or undefined native lifetimes. Do not substitute `dimensionOr(3)`: that fallback belongs to the separate Environment assembly and is inappropriate admission policy here.

Keep numeric-locale admission after model validity, then the local control reference, source model/control/remesh publication and page construction. An invalid model with unrecovered locale remains InvalidModel, while live 3 with unrecovered locale remains UnrecoveredNumericLocale. Neither reaches page/report reached-page markers or publishes source metadata. AlreadyStarted resets the passed report and returns that issue without another native lookup or replay, preserving the existing one-attempt contract.

`dimension()` is `const noexcept`, returns the direct nullable scalar, and has no geometry guard. Do not add one or hoist the scalar read ahead of simCtl. A parked simCtl exception propagates after constructionStarted was committed and before model/locale classification or metadata publication. This migration adds no catch/rollback/noexcept policy and makes no new parked path reachable.

## Meaningful caller verification

The proposed fresh-model/session/report matrix directly exercises the changed admission rather than only a standalone getter: live 2 and live zero, missing step, invalid model plus invalid locale, live 3 plus invalid locale, and a second call on a refused session. Compare default report stage and all page/refill/backup markers, reachedPage absence, session completeness and null source identities. Retain ordinary complete-3D lifecycle tests for the downstream success route.

Use an I/O test double that counts all read/write/remove entry points when claiming no I/O. Current fixture's empty effect vector does not establish no read calls. Test zero by changing/restoring the owned fixture field rather than introducing unrelated setter initialization effects; never dereference a control retained across step deletion. The absent-step case should also verify storage was not recreated. An AlreadyStarted retry with previously invalid state made otherwise admissible is a useful discriminator against unintended reconstruction; its report should still reset to the AlreadyStarted preflight result.

These tests establish outcomes and effect prefixes. They cannot count whether the inline nonvirtual scalar getter was invoked on a short-circuited path; the literal `||` source order establishes that. Existing parked model/viewer tests support simCtl's preexisting guard; no parked lifecycle callback test is supplied or claimed by this proposal. No new production observer is needed.

The currently added fallback query tests in `test_gui_model_dimension.cpp` and existing setter cases provide useful scalar regression coverage, while the lifecycle matrix remains necessary for locale/report/publication order. Root should confirm configured target names and run the planned focused/integration group; this review records no runtime result.

## Provenance and scope

Current source/session definitions establish the InvalidModel/locale/one-attempt adapter policy. Retained getter attribution to original GUI twins `0x140c7c0e0/0x140c7c0a0` does not establish the original editor gracefully accepted/refused zero/null models through this same preflight. Preserve that qualification; no new original routine or fresh original observation is recovered here.

The selected direct scalar/presence access disappears, but the local/native cached control, later backup owner manipulation, object traversals, constructor/palette admission and public Save boundaries remain separate. No ObjectKey/incarnation capability, ownership closure, meshing-field expansion, Stopping/cache1 continuation, or Report accepted-Close witness is advanced.

Only this review report was written. No source, tests, build artifacts, shared recovery state, GUI, original process, or mutable REA state was changed.
