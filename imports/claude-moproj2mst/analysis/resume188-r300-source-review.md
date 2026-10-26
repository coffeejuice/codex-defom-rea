# CP188 R300 `contactFlagsFromEngine` independent source review

**Decision: pass.** The current implementation matches the selected scoped-borrow plan and the implementation report. The nullable step borrow and caller refusal remain scoped to `saveInterObject`; this review supports helper-only qualification.

## Source and ordering

In `source/apps/moproj2mst/src/engine/save_current.cpp`, `contactFlagsFromEngine` clears `icb.flag` first and immediately invokes a local lambda with `m.step.get()`. Pair count and all following step-derived reads stay inside that synchronous lambda. The lambda returns no pointer or reference and is not stored. The original order is retained: read pair count at `inter_obj+0x10`; for zero pairs enumerate objects in ascending index order and flag ID-1 only for kinds other than 7; otherwise, only when enabled, traverse `pair_nth(i+1)`, skip null records, set slave ID-1 for kind 2 and master ID-1 for kind 1, and ignore other kinds. No new range check, index convention, or helper-level null guard appears.

The sole production caller `saveInterObject` remains unchanged: it does one `opModel` lookup, passes the nullable step to its immediate synchronous callback, and refuses null before optional lubricant rebuild, flag mutation, path derivation, and writing. It calls the helper only after that admission. No extra model lookup or step alias survives the helper callback.

## Fixture and scope of evidence

`tests/s/test_save_current.cpp::save_interobject_step_scope` extends the existing copied R46 operation-2 fixture. It uses actual loaded object IDs, establishes valid kind-2 and kind-1 pair records if the fixture has none, validates pair IDs/membership and computes expected flags, then seeds all flags with 7 before exercising `saveInterObject`. Assertions cover resulting flags, one expected path/write with nonempty bytes, no deletes, pair-count stability, and stable model/step identity. The noncopyable local `DetachedStep` RAII guard restores ownership on normal and exceptional exits. Detached probes call `saveInterObject` only, verify refusal/no effects/no flag changes, and check restoration; a post-restoration positive call exercises the pair path again. There is no direct helper call with a null step.

The test does not directly exercise zero-pair object enumeration. That branch is covered by static comparison only, as the plan permits. The retained native qualification in `source/docs/re/moproj2mst/s/spec.md` §1 places native `0x1400655d0` after optional system-lubricant pair rebuild and before `W_INTEROBJ`; it does not make this fixture a fresh original-process comparison, public Save proof, native-byte oracle, or helper-null qualification. The implementation report accurately limits its claim to reconstructed helper mechanics and leaves build/regression/freeze to root.

No source, test, build, GUI/REA database, or deployment state was changed. This report is the only write.
