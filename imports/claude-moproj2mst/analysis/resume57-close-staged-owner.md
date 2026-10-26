# Checkpoint57 staged OPI owner and first public Close prefix

The next owner design must separate native visible pointers/guards from C++ allocation leases. Preserve OPI current through Props, model and member cleanup; expire it only at the explicit OPI QObject boundary, before the outer item finishes. The first modal-free public Close work is a qualified not-busy prefix followed by the Mo child’s null-Project can-close leaf. It permits continuation, not completed Close. Busy rejection itself executes a modal warning and is not a safe immediate-false shortcut.

This report is the sole write. No source/build/test/GUI/mutable REA change occurred. Original reference DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Static evidence/qualifications for OPI/cached-model destruction remain in resume50-close-destructor-next.md and resume56-opi-cache-lifetime.md.

## Exact staged owner sequence for current C++ reconstruction

Stages must be tied to the same live OperationInfoIdentity and actual slot identities. Default unqualified effects stop BEFORE mutation; a receipt string or `complete=true` is not a completed native prefix. Keep ordinary adapter destructors nonthrowing and separately classified.

| Stage | Required source order and state visible to callbacks | Current-owner consequence |
| --- | --- | --- |
| Entry | Only after accepted Close predecessors and operation-manager selection/model stages admit this row. Outer SfMoOprItem owns OPI; its diagName/base still exist. | Keep the folded Operation allocation alive; do not free the vector row or publish expiry yet. |
| Props0512c0 | If+10 null, source skip. If nonnull, invoke deleting virtual+18(flag1), THEN write+10=null after return. OPI current remains bound throughout. | Existing Props-first helper is the correct guarded entry. Nonnull payload cannot become a plain props.reset(). |
| Engine cache05f980 | Read actual guarded+38; null skips, live target gets deleting virtual+18. Its own QObject boundary can null the outgoing+38 guard. | Reuse actual cached GuiModel identity; keep its guard separate from the allocation lease. A classModel/virtual+e0 borrowed return is not automatically this OPI’s owned cache. |
| Objects/positions |061180 then061f70, including their native element/list effects. | Preserve native container/element order; empty modern vectors do not prove all original effects absent. |
| Recovery owners | Delete nonnull owned+20 wrapper then null it; then+28 wrapper then null it. | Shared vector lease reset is not by itself native wrapper deletion or callback ordering evidence. |
| Current+160 encounter | Only NOW read the actual guard at05079e, after preceding callbacks. Do not reuse a defaults-era KnownNull if its validity chain was interrupted. | Owner observation must be current or source-preserved across every admitted effect. Existing isolated cache helper is not a whole-lifecycle certificate. |
| +160 null branch | Skip BOTH target virtual delete and explicit cbc920(null), jumping to05080e. OPI identity remains live. | A qualified null-target skip may advance only this stage. Do not expire current or declare whole OPI cleanup complete. |
| +160 nonnull branch | Invoke target deleting virtual+18 at0507e5. Target’s QObject destruction may already clear the guard. After return, explicitly call cbc920(null) at050809; it may be an equal-null no-op. | Keep target/guard state visible during derived cleanup; do not clear before destructor effects. SfMoItemModel’s selection-model+48 must be deleted before its helper/base cleanup. Arbitrary Qt callbacks remain guarded. |
| +160 member guard | After050690 returns,050478 calls cbc900/removeGuard on+160. If it is null, Qt6712d96d/971 fast-returns before registry access. | Guard registration destruction is distinct from target deletion and OPI identity expiry. |
| OPI members | Strings+158,+148,+130,+128,+120,+118; list storage+60,+50,+40; then outgoing+38 guard removal. | Explicit native-member stages must finish while incoming current guards still observe this OPI. |
| OPI QObject | At050531 enter QObject; clearGuards6712f8bd invalidates incoming OPI guards BEFORE admitted destroyed delivery6712f97b. | Invoke existing commitOperationInfoQObjectBoundary only here, then recover actual recipient effects. Do not create a synthetic original destroyed signal from a fixture observer. |
| Outer item | Return OPI delete, set item+48=null, then outer diagName QString and SfMoItem/QStandardItem base destruction. | Folded Operation fallback expiry after all members is later/coarser and remains AdapterIdentityLifetime. |
| Remaining managers | Finish objects/material/equipment/lubricant manager slots, base Mo model cleanup, manager list clear and current reset. | Do not collapse any of these into Project reset, registry reset or timer/dispatcher teardown. |

### Why shared_ptr::reset is not the native deletion seam

Native0512c0 retains raw owner+10 during the deleting virtual and stores null only after it returns. `shared_ptr::reset()` publishes an empty shared_ptr before destruction of its released pointee. A callback reading op.props therefore sees a different state if reset is used as the entire native effect. Extra shared aliases may prevent destruction altogether.

The same distinction applies to native guarded+38/+160: the pointer/guard remains valid through the target’s earlier destructor payload, then the target QObject boundary invalidates it; allocation leases can survive independently. Reconstruct owner/effect/liveness separately rather than inferring deletion from reference counts.

Minimal next owner record needs: a visible Props pointer/slot view; its same-object storage/effect owner; guarded cache+38 plus actual cache allocation provenance; qualified current cache+160 plus same SfMoItemModel owner/guard; and distinct OPI/outer-item lifetime stages. Do not duplicate the native model merely to add liveness state. A future wrapper may preserve current `.props.get()` syntax, but native destruction must control its visible pointer’s post-return null store. The current shared_ptr API alone is not sufficient to claim that ordering.

Unknown Props subtype destructor, GuiModel reset/Qt payloads, objects/positions, recovery wrappers, item-model/selection/Qt owners and actual destroyed recipients remain hard guards. This design does not authorize manually invoking a C++ destructor twice while shared leases still exist; native payload effects and adapter allocation release need distinct responsibilities.

## Busy rejection is modal: correction to an attractive first branch

Native public0299a0 passes show=true at0299ba..bf and calls2ca920 at0299cb. The helper:

1.Tests byte global143d766b8+9. If nonzero it short-circuits the next query.
2.Otherwise calls2ca840, which gets its main-window pointer through pure2c8090. Null returns false; nonnull dispatches virtual+300.
3.For the retained main table1420fb108, +300 resolves01daf0. It tests QStackedWidget::currentIndex(main+480)==2 OR the result of0327c0. IAT141e0ba38 is currentIndex, not QProcess state. The0327c0 closure/current UI facts remain to be bound; do not hardcode fresh=false from the existence of a new session.
4.Stores the combined busy result locally. Since show=true, a true result calls2caf10 at2ca982 BEFORE returning that latched result.

2caf10 constructs QMessageBox/QCheckBox and unconditionally reaches QDialog::exec at2cb42a on the inspected body. Its counter changes message/checkbox behavior, not a proven modal-free early return. After delivery, the checkbox branch can write the busy flag through2ca9f0(false), invoke main virtual+368 and update/reset global143d91230. The original busy result in2ca920 remains latched, so public0299a0 subsequently returns false even if that dialog cleared busy.

Therefore do NOT implement `if (busy) return false` in public Close without the warning effects/modal delivery. Do not substitute another helper’s show=false behavior: public Close passes true. This new fact refines earlier short descriptions of “busy guard returns false.” The outer false return was correct; omitting the guard’s internal effects was not established.

## First public Close branch that can progress without a guessed schedule

There is currently no source-ready terminal successful Close for the normal fresh opened project. The source-ready partial public entry is:

1.Bind actual global+9 and main-window virtual+300 result/provenance. If the global flag is false and qualified main busy result is false,2ca920 returns false WITHOUT entering2caf10. The coordinator can advance to manager can-close. If either fact is unavailable, stop; if busy is true, stage the busy-warning dependency before any terminal false result. Respect the native short circuit.
2.Iterate actual manager-owned children in native order, not only the selected model.
3.For an actual Mo child whose **native moProject+19c0 is null**,044110 returns true immediately at04413c ->044a77. It does not construct a question, call nested Save, or emit post-answer display/RemoveGraphItems callbacks. A separate modal-free allow leaf also exists when process type is neither1 nor5, but that is not the normal successful native Open profile.
4.Once every model guard allows, continue to busy=true/null refresh/model-clear stages only as their actual effects become available. Stop at the first unqualified receiver/owner effect. Do not relabel “can-close allowed” as “Close completed.”

The native null-Project field is not `session.liveProject()==nullptr`: the adapter may have allocated Project storage before native construction/assignment commits. In current session preparation, DbHandle exists before commitProjectModelConstruction stores moProject. A failure before that store is a concrete no-prompt can-close branch even if adapter Project storage exists. This is the first useful failure-lifecycle branch to bind publicly without guessing timer readiness.

For ordinary successful .moproj Open, moProject is nonnull and process type5. Its Save question/nested Save/modal delivery remains required; a prior explicit Save or reconstructed clean/dirty flag does not remove it. Due-event profiles must be supplied explicitly; no zero-delivery schedule may be inferred from choosing the default answer.

Use an internal staged session/engine entry consumed by the existing ABI; do not invent a new public C export or reinterpret ABI statuses merely to expose the prefix. Suggested outcomes distinguish AwaitBusyFacts, AwaitBusyWarning, AwaitModelCanCloseQuestion, ModelGuardAllowed and AwaitCleanupEffects. None alone means terminal Close success.

## Focused integration tests

- Qualified not-busy facts advance without dialog construction/delivery; missing virtual+300 provenance refuses rather than treating it false. A true global+9 short-circuits the main-window query and reaches a guarded warning stage, not immediate false or deletion.
- If a declared warning driver later mutates busy false, the original latched busy result still rejects this Close request after delivery. This is a fixture schedule test, not original modal timing acceptance.
- Actual registered Mo child before Project construction commit: moProject null, adapter liveProject nonnull. Its can-close leaf allows without question/Save/post-answer callback. After construction commit, the same model must reach the process5 question guard instead.
- OPI Props callbacks observe visible+10 and current still bound until the deleting effect returns; only then clear+10. Null Props advances only one stage. Do not accept a shared_ptr reset as this test’s native callback oracle.
- Fresh +160 null observation at the late teardown encounter skips delete/explicit null assignment; later null removeGuard skips registry work; current remains live. Nonnull fixture target cleanup must keep its guard until its own QObject boundary, then execute explicit guard-null and member removal in order.
- Only after qualified OPI payload/member completion does incoming current expire before a declared destroyed observer, while outer item still exists. Adapter fallback stays separately classified. Refusal/retry never replays a partial destructor.

No tests were run here. Public normal success/normal busy rejection remains guarded pending actual modal/receiver/owner integration.

## New bounded evidence hashes

Read-only DUMPBIN `/disasm:bytes /range:start,end-inclusive` and fixed PE qwords/IAT records were used. Exact unwind code-byte bounds are end exclusive; these are not closure proofs. No extra ASM files were written because this lane owns only this report.

| Region | SHA-256 |
| --- | --- |
|1402ca920..1402ca990|9918437dde0217d0f47173d73a00dd1478e99db1c3ea0a2fa43c3f42e9047434|
|1402ca840..1402ca883|dd05aeaa7d2cb198061a8671b030ab8878b39960946eac20ba6208446bef942a|
|14001daf0..14001db3c|9fde859043f5266fd0d9107338d3d28a1dd5634080b8211f4ce2919e489249f4|
|1402caf10..1402cb506|a4b9aca0320726cf22f4880eb2c440977d45e8dc33d4a1714484e14f7a8b677c|
|140044110..140044a81|bcf5727ecceb436cc0c9902c90b9ff7638c74058036deeeca569470eabb405a2|

Existing OPI/guard code receipts remain in resume56-opi-cache-lifetime.md. No reconstruction executable build or deployment occurred.
