# CP187 R300 independent source review

Decision: **PASS for the helper-local source and fixture change. No source blockers found.** Build, focused execution, integrated regression and freeze remain root-owned gates; this review ran none of them.

## Reviewed identity and delta

Compared `resume187-r300-next.md`, `resume187-r300-plan-review.md` and `resume187-r300-bcc-implementation.md` against the current two files and the CP186 verified `source.zip`. The archive independently confirms the initial hashes in the implementation report. The current hashes also match that report:

- `src/engine/save_current.cpp`: `46265debeae2d39625cdc4e33e08970eab0bbe8540b6752a1015fa36f3f1ae36`
- `tests/s/test_save_current.cpp`: `25db08fcac4a602731c45fc2fa480fca4af3143f766340b44d9feddb0ab93c79`

The production delta is solely the nullable immediately invoked callback in `saveBcc` (lines 577–595). The test delta is solely the helper declaration and `save_bcc_step_scope`. CP186's existing inter-object change is not reintroduced or modified. The next-note prose describing an alias before object checks is imprecise; the frozen implementation, its static acceptance paragraph, the plan review and this patch all acquire the alias after object/mesh checks.

## Acceptance findings

- Entry presence, DefineBCC and type 5 remain first, followed by one model lookup and model/step refusal, object lookup at `s + 1`, then mesh refusal. Only then is `m->step.get()` passed to the callback. Its nullable parameter is used exclusively inside that lexical scope. `captureKey` calls each writer directly and retains no callback, so both nested reference captures finish before the immediate callback returns. No pointer escape or later alias use appears in this source.
- No geometry, Stopping, aggregate Save or extra object-kind guard was added. Ascending independent slot processing, `slotKey(..., s)`, both writer indices `s + 1`, ignored return values and RzElmCon-before-BccDef order are unchanged. Two distinct `captureKey` calls remain. Their target paths are recorded in `FileEffects`; scratch files are removed. `saveBcc` adds no deletion effects.
- The direct fixture (lines 637–721) performs the normal R46 operation-2 load before setting admission flags. It requires the actual type-5 entry, non-null loaded object, real `objHasMesh`, kind other than 7, and object-list identity at number 1. This prevents a banner-only success caused by absent mesh/object from satisfying the admitted-path premise. The loader source constructs/copies the inherited object through `llobj_ctor::get_or_create` and `llobj_copy_keep_number`, then sets kind 5; it refuses a preexisting inherited BCC definition. Enabling the flag after load is therefore necessary and appropriately qualified. Clearing other slots' flags isolates one slot; no geometry, mesh bytes or zero-filled engine structures are fabricated.
- The fixture requires exactly two ordered expected task-directory paths, nonempty bytes and no deletes. Disabled-flag and wrong-type cases require zero effects. The detached-step guard is noncopyable, retains ownership in a `unique_ptr`, restores in a noexcept destructor (including Testkit REQUIRE exception unwinding), and invokes only `saveBcc` while detached. Afterwards it checks exact model, step and object identity plus mesh presence. Testkit entry point, existing includes, REQUIRE/CHECK support and absence of C assert were checked statically.
- Project and entry mutations remain in the discarded in-memory project. The test does not apply `load` or `saved` effects to the corpus. `FileEffects` defaults native removals and native task keys to false; loader removal paths then queue effects rather than delete corpus files. The helper writes only its existing temporary captures, and CTest redirects TEMP/TMP/TMPDIR beneath its disposable scratch. This is source-level filesystem qualification, not a new runtime mutation audit.

## Evidence limits

Read retained `spec.md` sections 1–2 and the actual RzElmCon/BccDef writer bodies. They support the entry filters, numbered paths, object-by-number lookup, banner then mesh/kind admission, node/element and BC serialization, and existing port closure of native return-3 leaked handles. The RzElmCon implementation comment further qualifies missing-object behavior and its lack of a native name check; this patch changes neither writer. No complete native error-path parity is inferred.

The new case is explicitly synthetic helper mechanics. Nonempty output can include a banner and does not prove nonzero BC content, native bytes, public Save dispatch, aggregate admission or pointer non-escape by runtime observation. Static source inspection establishes the limited alias closure. No source, test, build, GUI/app-control session, REA database, corpus or shared state was changed during this review; only this report was written.
