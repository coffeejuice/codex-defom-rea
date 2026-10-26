# CP120: Project destruction precedes late operation-editor removal

Single-project Close has three different boundaries: detach the still-live diagram editor from Project's flow model, destroy Project and the Mo model's operation/item slots, then request removal of the main-owned operation-editor widget. That last removal can defer actual widget destruction. A live editor/model allocation is therefore not proof its OPI is alive, and Session exception cleanup is not this native ordering.

Only this report was written. Bounded retained PE/source/captures and offline disassembly; no source/interface/plan edit, build/test, GUI, original execution, mutable REA database or REA import. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed. Native function/site addresses below are RVAs relative to140000000.

## Ordered native owner boundaries

All accepted cleanup below is conditional on normal return from earlier busy, can-close/modal/nested Save, display/graph, true-busy/Tree and null-refresh effects. CP110's writerfalse does not itself detach Project, empty the manager or discharge those predecessors. The qualified default remesh fatal path does not return to normal Close.

| Stage | Owner/action and source qualification |
| --- | --- |
|029a76 ->c78300|Actual main-owned ModelManager deletes children in encounter order. Its owning list/current are not precleared.|
|Mo045350 ->044040/0440c0 ->044bf0|For nonnull Mo+19c0 Project,0982c0 ->0aab30 performs feature cleanup **before**0453e0 ->08a3c0 deletes Project. The actual feature context/callbacks remain a guard; destroying an adapter metadata anchor cannot substitute.|
|08a3d7..08a3f8|Project destructor obtains the current diagram editor through0a89e0. Nonnull view receives virtual+1b8(null), bound to170430. The getter/current-main/view identity is required; Session's receiver alone is not that native lookup.|
|170430(null)|While the old flow model is alive, saves view position, detaches old model, writes attachment null and performs render/selection/view effects. Its nonnull timer-registration branch is skipped; the body does not cancel earlier singleShots or destroy the diagram QWidget/receiver.|
|08a403 ->0a80e0|Only after null-rebind, destroys SubProjects and their flow models. SubProject0ac330/0b58d0 handles owned+48 guard deletion/clear, Report0b5954/0b5bb0 and flow deletion in their native sequence. The Report null-target skip is local, not proof of complete Project teardown.|
|08a412 onward|Destructs Project+60 feature member via12f0d20, then remaining members/base. This feature-member destructor is distinct from the earlier0aab30 cleanup call. Project storage is released before044bf0 clears Mo+19c0.|
|045000|Only afterward destroys Mo slots19c8 operations,19d0 objects,19d8 materials,19e0 equipment,19e8 lubricants, then remaining base/QObject state. Flow diagram items and operation-manager-owned OPI items have different owners; deleting a flow model does not certify OPI destruction.|
|c7852b/c7853e|After child deletion, clear manager list and explicitly assign current null. ModelManager itself and main remain alive through Project FileCloseAll.|
|029a83/029a93 onward|Post-clear Project lookup and main refresh effects precede the later widget-area removal request at029c8d, false-busy and status. Arbitrary receiver delivery remains qualified.|

The diagram editor above is **SfDiagCellEditor**, retained by the main/view graph. The operation editor below is the separate widget produced by SfOprBaseFactory and delivered to SfWidgetArea. Do not merge their guards, timers, QObject identities or native owners.

## OPI and editor guards must remain separate

Native130830 binds the operation editor after its preceding QWidget/base work.130857 callscbc920 on **editor+38** with the actual OPI argument. It resolves that guard and calls05f770 ->05f820, then377b20 stores the returned model into the editor's model guard. The similarly numbered fields belong to different objects:

| Field | Meaning |
| --- | --- |
|operation editor+38|Guard to OPI; does not own OPI allocation.|
|operation editor+28|Guard to GuiModel; does not establish OPI lifetime.|
|OPI+38|OPI's separately owned cached engine model guard.|
|OPI+160|Separately guarded item-model cache, preserved on class clear until destructor rules apply.|

05f820 first resolves the OPI class, calls class virtual+e0 at05f859, and returns a nonnull result immediately. Only the null result falls through to OPI+38's cache path. Thus an editor can refer to a class-provided/borrowed model whose lifetime is not identical to the OPI's cache. **A live GuiModelGuard cannot certify a live OPI.**

The retained OPI destructor contract is Props0512c0 ->owned engine-cache05f980 ->object/position/recovery cleanup ->current+160 deletion/clear when nonnull ->member/outgoing-guard destruction ->OPI QObject. Incoming OPI guards expire at that QObject boundary, before destroyed-recipient delivery and before the outer operation item's later destruction finishes. Any actually deleted GuiModel's own QObject boundary independently expires its incoming model guards; a borrowed surviving class model is not declared dead merely because OPI dies. Registry current guard likewise survives the earlier payload cleanup and expires at its actual OPI QObject boundary.

By the time manager child teardown has completed and the root reaches late widget-area removal, native OPI destruction has already occurred for deleted operation rows. The still-live operation editor can therefore have null OPI guard while another borrowed model guard remains live. This is not permission to skip its leave/Qt cleanup effects or to dereference a raw historical Operation pointer.

## Late widget removal is conditional and can remain deferred

Factory5ff0b0/130740 creates the operation editor parentless; returned-widget notification5f499f later delivers it to SfWidgetArea. The retained attach pathd14fc0/d14f70/dadc20 reparents it into the area's Qt subtree and calls QScrollArea::setWidget. Factory return and area ownership are separate occurrences; the current candidate has no completed native attach certificate.

For a reached actual area implementationdae450:

1.dae46a reads current guard+18; null branches to return atdae836 with no editor effects. A known current editor is dynamically cast; successful cast invokes leave virtual+1d0 atdae4d6. Layout removal follows atdae4fc. Neither effect is discharged by a later deletion-mode observation.
2.Only **after those effects**, dae50a reads impl byte+10. False jumpsdae836 and skips pending copy, detach/timer/hide/notifier/deleteLater **and current-guard clear**. This is not a zero-effect modefalse shortcut. Current remains represented after that branch.
3.True mode copies current+18 to pending+38, detaches the widget with setParent(null), starts the implementation's timer with15, removes/hides the widget and emits dock-removal notification. Atdae81d it requests QObject::deleteLater on pending; only afterwarddae831 assigns current+18=null. Actual QWidget destruction has not been proven by the signal, detach or null current.
4.Separate timer handlerdae850 reads pending+38. Null skips deletion; nonnull invokes its deleting virtual+18(flag1) atdae8a3, then explicitly assigns pending null atdae8c1 after return. It does not globally cancel every timer or mark the dispatcher destroyed.

Timer15 delivery and DeferredDelete are alternative later lifetime opportunities. No ordering between them is inferred from the request sequence; actual QObject guard clearing must prevent a second target use when it has already died. The operation editor also has separately qualified constructor timers, and the main-owned diagram's pending timers remain separate. An original modal/processEvents interval can deliver events; it is not evidence of an empty due set.

## Actual candidate ownership and smallest next executable action

`MoModelSlots` already supplies one stable storage capsule, with Project borrowing rows and the registered Mo child retaining the capsule. Its explicit release primitive enforces Project storage expired and operations→objects→materials→equipment→lubricants order. Its ordinary destructor is expressly adapter storage release, not native payload/QObject execution.

`ConversionSession::releaseProjectStorage` nulls its diagram binding, retains Display, removes ProjectScope, resets Project, then clears the matching Mo backlink. The Session destructor separately destroys its event receiver/dispatcher and releases adapter scopes. Session member order later destroys Editor300 setup/factory before ModelManager's final shared release, while an actual registered model can still retain operation-slot storage. This is a useful adapter resource order, **not** the native operations-before-late-widget-area order. Project::~Project only invalidates feature-context metadata; it runs neither native0aab30 nor12f0d20 payloads. `releaseProjectAfterCloseEffects` remains storage-only behind its null-diagram guard.

Current `StagedEditor300` records raw Project/Operation identity tokens plus GuiModelGuard, and `identityMatches` checks those tokens/model/current binding. It does not retain a typed OPI identity guard. The smallest useful **actual candidate owner increment** is to bind an `OperationInfoGuard` snapshot from the supplied actual OperationInfoIdentity into this existing record, then require the same live OPI generation before any continuation dereferences the caller's Operation. This uses an existing actual owner, rather than inventing main/application state.

Keep that snapshot diagnostic before native130857 is reached: it must not certify a native editor+38 QPointer assignment, advance QWidget/constructor stages, commit OPI QObject expiry or enable Close. The native guard-binding occurrence remains after its real base predecessors. Suggested root-coordinated write set is existing engine/simcontrol300_owner.hpp/.cpp and tests/mdt/test_simcontrol300_owner.cpp; another editor lane may own them, so no edit was made here.

A focused test can retain actual Project/Operation/model/factory allocations, explicitly commit the existing OPI QObject-boundary fixture while leaving the model guard live, and require identity/continuation refusal before widget/page/file effects. Also assert that merely releasing Project scope or finishing an earlier Props/cache stage does not expire the OPI guard. These are candidate guard tests, not an accepted-Close or destructor-order witness. This increment must not normalize a missing native identity into successful cleanup.

No new **public native cleanup** edge is source-ready from this audit. Actual application/main/manager construction and accepted root precede feature cleanup, diagram rebind, Report/OPI/item payloads, receiver delivery and area effects. The next native destructive call still has to be driven through those source phases, not invoked from catch/finally. Source/phase-qualified null leaves remain local until that coordinator exists.

## Source receipt and limits

Controlling retained inputs: wave3-normal-close-diagram-lifetime-contract.json; wave3-close-save-return-contract.json; resume47-close-staged-teardown-design.md; resume54-opi-expiry-owner.md; resume57-close-staged-owner.md; resume110-close-after-save-failure.md; wave3-editor-native-owner-contract.json. Selected direct Project destructor bytes08a3c0..08a466 retain SHA-256 `d33ef435aea6da822f0aa985431d82cba18f8a18374e724d0d76030e986614f9`.

| Retained artifact | SHA-256 |
| --- | --- |
|wave3-editor-native-owner-contract.json|`3d70cad9c2c8d1c0984c9936351baf0d3f80dea31c6d31be4b2e9b25554c766a`|
|wave3-editor-widget-delete-140dae450.asm|`07297281e756fe36048592be840bb6a57a218b21c6ad9f3e31f452b82a6d36b2`|
|wave3-editor-widget-delete-140dae850.asm|`2fdcd22970cf209200321f8c8b3a571f78cceb98b4474b5251c87dae4242d91c`|
|wave3-editor-native-parent-140130830.asm|`89d7d9f853c6bcc74b0923ec767cfb9d84165731b4b4c3f628240a454d525ca4`|

Current read-time source hashes under source/apps/moproj2mst/: include/m2m/engine/simcontrol300_owner.hpp `828de6f07636337c204e85ba2db3c78c10708dd74d71cedccffe9cb4c1b02bac`; src/engine/simcontrol300_owner.cpp `7d2e2e91174b5441ec5c00da8018b12b9c7bfaf7ed40cae41174872c03d5e958`; src/model/project_owned_storage.cpp `d8a98bcba68e52c8c1197a74df4a56a43874a91594be25f7aedb3dd7f24f69ac`; include/m2m/engine/conversion_session.hpp `46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31`. These are static qualifications/read identities, not runtime Evidence IDs, candidate freeze or completed native Close. No test was executed.
