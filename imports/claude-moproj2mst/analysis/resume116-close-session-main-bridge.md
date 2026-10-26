# CP116: the missing Session/main bridge has two distinct main edges

Native construction binds manager G permanently to its constructor main M, while normal Close separately reads the then-current application main W at several sites. The candidate implements a real Session/G relationship but produces neither native M nor mutable AppState.main W. Treating these as one diagnostic lease would lose original identity and reentrancy behavior. This audit closes their static connection/lookup paths; the live bridge and public Close remain unresolved.

This report is the only write. Bounded retained/local PE/source reads, offline DUMPBIN, RTTI and descriptor verification only; no source/interface/plan edit, build, test, GUI control, original execution, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable or deployment was inspected or changed. Application function/site addresses below are RVAs relative to140000000; globals/vtables/descriptors use full VAs.

## Initial identity and child receiver chain

CP114 closes10490 ->10598/111c0 ->111d8/c999e0 ->c999f6 virtual+2e0 ->0133f0. The derived table is1420fb108. Base construction had already published that same main M into singleton143d766b8+0 atc9957b, then initialized M+50=null atc99588. Factory0133f0 creates G, passes M to045420/c781d0, stores G+10=M atc7822c, performs the fresh-null current assignment's equality-skip, installs manager table142115f68, then stores M+50=G at01344c. QApplication/base/settings/setup effects precede this selected constructor phase; these identities do not bypass them.

Open01ca30 obtains the existing G throughc9b7c0 at01d078, whosec9b7ca reads incoming M+50;01d085 calls child factoryc79390. The new independently decoded receiver sites are:

| Site | Concrete identity use |
| --- | --- |
|c79416|Loads receiver r8 from this manager's+10.|
|c79426|QObject::connect registers the actual new child sender's ModelChanged signal to that receiver's slotModelChanged, raw connection type3.|
|c79440|Reloads receiver from the same manager+10 for the next registration.|
|c79450|Registers child ModelUpdated ->receiver slotModelUpdated, raw type3.|

Registration follows the child creation/path/ID/list prefix; registration is not an emitted callback. The retained actual-open contract binds ModelChanged through main Qt metadata/localmethod98 ->metacall0dc5cf ->virtual+728 ->03cd30;03cd30 first calls basec9c880 and has further Ptrk/toolbar branches. The selected ModelUpdated handlerc9cfe0 is a no-op body, but its dispatch/lifetime is a separate dependency. No new Qt type3 delivery semantics or event schedule is claimed here.

These receiver registrations use **G+10=M**, not a fresh singleton lookup. Changing AppState.main later does not source-qualify retargeting existing connections. Conversely, retaining a manager/connection does not establish what AppState.main currently contains.

## Close uses incoming M and fresh W for different work

The actual FileCloseAll metacall20 ->031f10 ->0299a0 preserves its incoming main receiver M. Its order is:

1.0299cb calls global2ca920(show=true). It reads singleton+9, then, where required, obtains **current W** through the main getter/busy route. Nonnull W dispatches its own virtual+300; selected Mo main01daf0 reads its actual+480 currentIndex and, only on non2, its+270 mode. CP112's pure evaluator models those integer comparisons only. Nonzero global+9 or main busy retains the busy-warning effects before refusal.
2.0299e6 callsc9b8c0 on **incoming M**.c9b8ce reads M+50 throughc9b7c0; null manager returns true atc9b8f9. Nonnull path rereads that field atc9b8dd and dispatches G virtual+58 atc9b8f4. It does not dynamically cast here or obtain G from W.
3. G'sc7a100 iterates its children; c7a2d5 invokes each actual child's virtual+68 on the child itself. The selected SfMoModel target044110 checks its own moProject+19c0 first, then application process type. It does not receive M as a separate can-close argument.
4. For the selected current-operation question,0444b6 calls2c8090 on singleton143d766b8 to get the **then-current dialog parent W**. After the modal answer/nested Save, later sites again call that getter:044731,04477a and0447c1 belong to the alternate/accepted routes;0447c1 supplies the main used for accepted virtual+288. They are not loads from manager+10. Unknown modal callbacks cannot be replaced with an assertion that all these W values remain M.
5. Only after every child allows, root commits true-busy/Tree/null refresh effects, then029a6e calls0133a0 on incoming M before029a76's manager clear.0133c6 loads **M+50** and0133ca invokes dynamic_cast with source descriptor1435693f8 `.?AVSfModelManager@@` and target143569420 `.?AVSfMoModelManager@@`, verified directly from descriptor bytes. This later typed wrapper is distinct from the earlier base virtual can-close lookup.

The native bodies do not assert M==W at these sites. On the selected uninterrupted single-main constructor profile they initially coincide, but the candidate must preserve the separate mutable global edge through delivered effects. It may guard an unrepresented state before its effect; it must not “repair” the original by interning different mains, retargeting G, inserting a same-owner native check, or replacing fresh loads with the constructor address.

CP110's failure distinction still applies. Passing nested validation returnsNo10000 even when a later project/MST writer Boolean is false; exactly nestedYes4000 becomes CloseCancel. Writer failure neither detaches Project nor erases G. A later FileCloseAll repeats fresh busy/can-close/modal/nested-Save work. Default remesh process termination is a separate no-return profile, with no subsequent normal Close or ordinary unwind.

## What the candidate actually binds

`ConversionSession` creates one actual ModelManager with `owner_=ConversionSession*`. `registerModelSignals` requires an appended real child and records that same owner_ as receiver. `emitModelSignal` explicitly refuses matching emissions at the unresolved receiver/type3 dispatch boundary. This faithfully retains a partial candidate relationship, not an implemented SfMoMainWindow receiver.

`SessionModelManagerReadReceipt` revalidates the exact live Session, canonical manager pointer/control block and manager->ownerSession()==Session. Its escaped identity is an opaque uintptr_t token. Its static qualifications explicitly leave native factory/concrete class/wrapper/main readiness/public Close false. That receipt cannot promote Session to M, supply W, or certify native command reachability. The core now has its early supported-suffix gate before Session construction; even suffix acceptance does not supply the application/main producer.

The existing Close entry helper similarly reads the real Session manager but takes separate supplied globalFlag9/globalMainWindowPresent/mainBusy300 observations. The Session pointer and nonempty qualification labels do not authenticate a native main identity. WidgetAreaPrimaryWindow remains an identity-only projection; MainView is a different adapter owner. Source contains no executable publication of one real primary main into a live AppState owner spanning Open, explicit Save, modal nested Save and FileCloseAll.

**Exact missing edge:** a source-driven constructor/lifetime must create canonical main M, bind its actual M+50 to the Session's same G and G's native main receiver/backlink to M, and separately publish/update/unpublish live AppState.main W. The native manager backlink must remain distinct from its adapter Session backlink. All command/connection consumers must use the correct edge at the correct occurrence. This is shared application/main/Session construction work, not a new receipt or pure local field model.

The earlier producer blocker remains CP107's real application startup1410/pre-app7fc450 ->SfApplication21c0/QApplication21f1 ->main10490/base/setup. CP114 locates the manager factory within that chain; it does not execute those predecessors. No new isolated implementation is justified or permitted in this report-only lane.

Future integration validation should retain M/G/W identities across actual Open/Save; confirm child receiver remains constructor M while fresh global reads use W; refuse unknown Qt/main input before effects; test reentrant W changes without retargeting G; and preserve CP114's later main teardown order: global W cleared atc99245, manager deletion atc9b827, M+50 nulled only after return atc9b83f. Project FileCloseAll retains M/G. Synthetic identity fixtures alone are not an accepted-Close witness.

## Bounded receipt

DUMPBIN14.44.35228.0, section-aware mapping, end-exclusive native spans:140c79390..140c79460 SHA-256 `0236f68c848d6db45223c9c2ef55a7c5187924577429edb72567a6c4194a8470`;140c9b8c0..140c9b900 `59157c3fe5b827e9b1722b8b0cc4df6663801ecf6fde1dc7d37df466e52f8e45`;1400133a0..1400133de `6b6904774a8f9b2108dc414f05ea8357bc09d7ac47f04d43727fcc027898ac69`.

Retained source qualification: wave2-doe-model-manager-actual-open-contract.json (`29847379ac215992643348f01e6aa1f78455ade03e2e6d692eddf60a5d9ab19e`); wave2-doe-model-manager-path-open-signals-contract.json; wave3-normal-close-guard-contract.json; wave3-closure-next-140c7a100.asm; wave3-closure-next-140044110.asm; CP107/110/112 reports; resume114-main-manager-constructor-lifetime.md (`ec7b113df38b7c7e69103809178f60fef16f0024ca2575874a4978feae889291`).

Current source read hashes under source/apps/moproj2mst/: src/engine/model_manager.cpp `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4`; src/engine/session_model_manager_read.cpp `ef86f587106679a6c33a6abe724c0a6e25ea51c72eddeb3d65db48203fa51725`; include/m2m/engine/session_model_manager_read.hpp `f9d8c014b7f6fc4bd14e0795cb7b9c3ad1e4317d3a7bdf6d08771dbcca698750`; src/engine/close_entry_prefix.cpp `3af630e8a3ac1729d939342de5344efdbbe781cf4a3ba3358f43d222124ebc3e`. These are read identities, not a candidate freeze or new runtime Evidence IDs.
