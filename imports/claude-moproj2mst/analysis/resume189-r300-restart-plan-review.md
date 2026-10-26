# CP189 R300 restart scope plan independent review

PASS for the bounded lexical restart-call closure in `analysis/resume189-r300-corrected-next.md`. No implementation blocker remains. This is plan approval only; implementation review, focused tests, regression and freeze remain root work.

## Frozen identity and ownership

Rehashed `build/wave3-checkpoint188-verified/source.zip` to `b58d51b9ef44fe78850cbd0dddc4a23e5a8efa369f85025219cbed7e051dff0a`, matching its receipt. Read the two archive entries and compared each byte-for-byte with live files:

| Assigned file | CP188 and live SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `2031229bdecbca9e30a90aae8c4dcde16cf455118fbc73403027e6a5df75eb4f` |
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `b3bb63a55b79d9b530f360338e8398adb36a141bf95587ddf02e3a236a112fd4` |

These are the appropriate exclusive implementation pair. The restart test and TaskKeyRig are in save_io; save_current's CP188 contact test is not the restart fixture. saveMaterials already uses separate count, eligibility and write callbacks. Do not edit it in this cut.

## Model, borrow and order

The current sync helper acquires its cached `m` with one direct `opModel(p, op)` call before folder creation. Other invoked save helpers have their own model lookups; the plan's count concerns this cached acquisition and the restart boundary, not the transitive total. Restart currently derives `path` first, then dereferences the cached `m->step` exactly once for saveWholeModelKey, passes flags zero and ignores its result. A directly invoked void lambda taking `m->step.get()` after path derivation preserves that local lookup/borrow count, model identity, writer slot and following NSTART work. Do not replace it with withOpStep, which performs another opModel lookup.

The retained S spec section 1 supports operation-call order and ignored restart result, with the historical-source limitation explicitly retained in the plan. Read the wrapper assembly plus static/qualification records. At `0x140c7d125` the same supplied model's engine getter is called; null branches to success mapping. On the nonnull path, flags zero, locale conversion and constData precede the second same-model getter at `0x140c7d169`, then writer at `0x140c7d179`. The plan accurately distinguishes this original two-getter wrapper from the reconstruction's single step acquisition. The retained artifact establishes static instruction order, not execution or the full logical function.

No new null-step branch belongs in this lexical cut. Current cached-model admission is not a lasting nonnull-step guarantee, and the original wrapper's initial null success is not a policy currently implemented by this adapter. Keep these qualifications visible. The closure provides lexical alias confinement, not ownership pinning or protection against destructive reentry. Existing equipment and later simCtl borrowing remain separate work.

## Fixture and checks

TaskKeyRig owns its copied files, Project, ProjectScope and loaded operation for the full test lifetime. The existing native_restart_key_runs_at_the_operation_save_slot_without_replay test already exercises recording/native restart output, last native attempt, overlay erasure, no deferred writes and ignored denied-open status. Extending that test with model/step identity and NSTART ordering assertions is appropriate. Seed NSTART sentinels separately immediately before each recording/native sync call; recording sync already mutates those fields. In the native hook, filter on the restart path before checking sentinels. Require the loaded model's simCtl before using its nstart as the post-call expectation. restartGiven ensures startsFromKey is true independently of chain-head status. Keep owner replacement and detached-step sync probes out of this cut.

One nonblocking factual correction: TaskKeyRig uses `Rig files{true}`, whose constructor selects configured corpus `49_MO2_Boolean_Oprations`, not R46. Update that fixture description when documenting implementation. Its project filename remains MO2_Boolean_Oprations.moproj and selected task is Task00001. This does not change the selected fixture or owner files.

Existing whole-model tests cover empty-name flag preservation, denied-open retained flags/target/overlay, short writer and ignored flush/close, and unsupported deferred DB rejection. Keep those contracts unchanged. Test output compares reconstructed paths; it is not fresh original byte parity or public Save admission. A static implementation review must establish callback nonescape; identity assertions alone cannot prove that property. Root should confirm registered save_io and save_current test names and Testkit preflight before execution, as the plan requires.

Only this review report was written. No source, tests, parent analysis, build, GUI, app-control, REA database or deployed artifact was changed or run.
