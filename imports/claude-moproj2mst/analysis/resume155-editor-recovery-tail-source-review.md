# CP155: recovery-tail source review

One blocking defect was found in the reviewed source: current SC identity is checked too late after Extra3D callbacks. Root was notified and accepted the finding. The remaining reviewed owner-bound table logic matches the bounded native contract; no additional compile/test defect was found by static inspection. This report does not accept a subsequent repair or claim executed tests.

Only this report was written. No source/test edits, builds, test execution, GUI/original execution or mutable REA occurred. Scope is the explicit private lifecycle; actual public Session Save remains guarded.

## Blocker: stale control read before tail-time admission

`simcontrol300_lifecycle.cpp:268` takes `auto& sc = *session.sourceControl` before Extra3D. The IO interface permits callbacks; if one replaces `model.step`/control, the old control can be destroyed before the new tail call at293/294. The new `applyOwnedRecoveryTail3D` initial condition at `simcontrol300.cpp:1427` reads `sc.owner` **before** checking `model.simCtl() != &sc`. That can access the stale control rather than refuse it. Earlier pages and IO may already have completed, so entry-only identity checks do not close this path.

Required correction: at the reached lifecycle tail, compare the current model/step/control against the session's borrowed source identity **before forming or using a stale SC reference**. Refuse with recoveryTail/outcome UnsupportedSnapshot while preserving completed pages/IO and native-position mesh temporary clearing. Do not resolve A/B or clear either recovery table on the failed admission. Also order the bound helper's current-control identity comparison before any SC-field access when coordinating its owner file.

Add a regression whose final Extra3D write callback replaces the current step/control. Verify safe refusal at the tail, completed Extra3D/page prefix, unchanged new control's recovery tables, and no replay/rollback. Existing tests cover wrong model at entry, not this after-IO replacement. A complementary late-holder replacement test should prove A/B are resolved at the tail rather than captured on entry. Root owns the lifecycle repair/test and will coordinate changes to the worker's released source.

## Reviewed behavior that matches

- The worker source/test hashes match its release receipt. The actual PPV reader populates the consumed semantic +4/+8/+10 fields. Mixed real PPV A/B tests check exact axis/payload mapping, A-before-B order and KEY-value exclusion; qNaN payload and negative-zero bits are tested without formatting.
- The helper resolves actual Project recoveryA and Operation recoveryB at the reached call, retains their control blocks, excludes type4 before the lazy-root accessor, checks live OPI/model/SC and rechecks holders before effects. Shared A/B aliases deliberately count twice while preserving persistent contents, matching the two native loops.
- Candidate record/shape/resource admission precedes table frees/stores. Positive NRECID storage is freed before n0/ncomp0, nodes rebuild before elements, zero-element count retains old ERECID, and allocation-null checks retain the reached metadata before refusing a null dereference. Ordinary valid allocator-backed storage remains an input qualification; raw shape checks do not prove arbitrary pointer extents or ownership.
- Rebuilt payloads precede the repeated Advanced occurrence; resizeRecoveryOutputs still follows ValidationStopped. TIMEID supplies node width, ERECID supplies element width. Native self-reinstall setters have no extra represented effect when reinstalling the same holders; the code does not clone/replace lists or synthesize deletion.
- The lifecycle overload preserves page order, continues after ValidationStopped, stops on UnsupportedSnapshot, clears mesh temporaries at the existing tail position, and preserves earlier report outcome when the tail completes. The legacy arbitrary-vector nonempty refusal remains.
- Shared declarations match the implementation signature. The new test has ten unique Release-active Testkit cases and one main; the lifecycle test has31 unique cases and one main. Includes, visible symbols and aggregate field order show no static compile issue. This is not compiler verification.

The two new lifecycle cases demonstrate an actual Operation/model route through the explicit private composition and an entry mismatch with no page/IO effects. They do not yet exercise mutation during IO, which is why the blocker escaped them. The existing worker validation-stop/output-array case does verify completed tables survive the later page stop.

## Public boundary

The new declaration explicitly says it does not complete the public FormingClass Save callback. `InitialEditorSession::completeForPublicSave` and InitialApplyReport's equivalent remain false. Current actual Session expects PrivatePaletteUnavailable; loader retains6504abb0 and FormingClass retains140eb5180. No StagedEditor300 public Save routing was added. Positive tests and receipt correctly describe private typed-model/lifecycle effects, not public Session/C ABI acceptance.

## Reviewed immutable identities

Implementation receipt SHA-256: `b2235fb7c4c9bb040e91eb44311f119ccc737a07f7eb72d3531896d40107d1b3`.

| Reviewed file under source/apps/moproj2mst/ | SHA-256 |
| --- | --- |
|src/engine/simcontrol300.cpp|`8a62d68c7c209d0713e11c509b2968c317b7a52339e272837fd7619f2de340f9`|
|tests/mdt/test_simcontrol300_recovery_tail.cpp|`5642d540fdac01a5b1707437d7227d9e43e531d4d5b9ab430259c6d484643dde`|
|src/engine/simcontrol300_lifecycle.cpp|`5322dc264a518e1ea8937547ee9e72e4c9a73d5c8976fed0d42aa1e72b7182d8`|
|tests/mdt/test_simcontrol300_lifecycle.cpp|`622c3193ef1b21a8e6f622b1e620db5afeba2205854d98eaccc2cbaa1ce52c26`|
|include/m2m/engine/simcontrol300_io.hpp|`10fc044a37be658faa02837f11719a206ea8cf924aa419d26c6864363e8aa8a2`|
|include/m2m/engine/simcontrol300_lifecycle.hpp|`9042f473268ca9b73fc8c4f976a74c7f0f2d06a167e2c4e8cf0f1b486642783a`|

The native contract remains bound to the independently verified ranges in `resume155-editor-recovery-tail-design-review.md` (SHA `cad67d03c18cdb980ccb7066f0499ba5d14c7114e611022d5cb81c57ad31f449`) and original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected. A changed lifecycle/helper/test source requires a new focused repair review and root build binding; this receipt applies only to the hashes above.
