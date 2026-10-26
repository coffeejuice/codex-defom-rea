# CP155: stale-SC repair verified by static review

The identified stale-SC blocker is closed in the reviewed source. No new blocker was found in this focused repair review. Root's full-suite execution is separate; this lane performed no build or test run.

The owner-bound lifecycle now checks `model.simCtl() != session.sourceControl` after Extra3D and the existing mesh-temporary clear, before calling the tail with the earlier SC reference. On mismatch it records recoveryTail/outcome UnsupportedSnapshot and returns. This is a pointer identity comparison against the borrowed source, not a read through the old control. `GuiModel::simCtl()` reads the current step's control address. The mismatch path neither resolves A/B nor dereferences/clears recovery tables, and preserves completed page/IO reports and the already-reached temporary cleanup.

The helper's initial owner predicate was independently corrected: current `model.simCtl() != &sc` precedes `sc.owner != model.step.get()` in the short-circuit OR chain. A mismatched current control therefore refuses before accessing a field through the supplied SC reference. The subsequent checked-table sequence and native rebuild/Advanced/resize order remain unchanged. The root lifecycle guard prevents the original integrated path from passing a dangling reference in the first place.

`initial_editor_actual_operation_rechecks_control_after_extra3d_callback` uses a real Operation-owned GuiModel. Its first Extra3D IO effect replaces `model.step` with a newly constructed DB_STEP_DATA. The replacement is allocated before the old unique_ptr is released, making this a distinct live current control rather than a fabricated pointer value. The regression requires the callback and IO effects, reached Extra3D/recoveryTail, UnsupportedSnapshot outcome, and fresh current NRECID count0 despite the actual root row that would otherwise rebuild it. It does not inspect the destroyed old SC. The source still retains the earlier private page effects and mesh-clear position rather than moving admission to entry or rolling back IO.

The fixture includes `<functional>` and uses its existing once-only `duringFirstEffect` hook. Static inspection found32 unique lifecycle Testkit cases/one main; the ten-case focused recovery test is unchanged. No compile assertion or test-pass claim follows from this inspection.

The legacy arbitrary-vector path was not widened by this repair. Public mode300 palette/Save guards and `completeForPublicSave=false` remain the controlling qualification from the preceding review. This accepts only the precise repaired private lifecycle defect, not full native/public Save completion or arbitrary callback-driven destruction of other borrowed owners.

Only this report was written; no source/test edit, build, GUI/original execution or mutable REA occurred. No reconstructed executable/deployment was inspected. The review supersedes the blocker in `resume155-editor-recovery-tail-source-review.md` (SHA-256 `6706031dab2aa802ca7a4f9d92b70fa2b13d6501b6e9f889b54727c864a8caca`) only for the hashes below:

| File under source/apps/moproj2mst/ | SHA-256 |
| --- | --- |
|src/engine/simcontrol300.cpp|`bd41db8ef7c06523b7c4f97f99bfbabbb939a35ebf78e03e4a9cc56f682f9eb2`|
|src/engine/simcontrol300_lifecycle.cpp|`10299266be63d0fc85c0474776492bc30946b8f5fa52d541a00ea4f6341c49dc`|
|tests/mdt/test_simcontrol300_lifecycle.cpp|`2f45b992d9f4efc29acccaf56e76ca48214531d1148460ece6e5e98f6702259c`|
|tests/mdt/test_simcontrol300_recovery_tail.cpp, unchanged|`5642d540fdac01a5b1707437d7227d9e43e531d4d5b9ab430259c6d484643dde`|
