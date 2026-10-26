# CP101: original Close owner chain and current-state writers

The original static owner chain now spans application startup, main publication, Open, explicit Save, Close and main teardown. New bounded PE reads also close CP98's three pre-busy initializer providers as pure stores. The remaining public Close dependency is **the candidate's actual shared application/main producer and its current callback-sensitive state**, rather than an unlocated native publication function. Startup defaults or the final Save reset cannot certify the live Close predicate.

Within the converter tree, only new analysis receipts/report were written. No production edit/build, GUI, original execution or annotation occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## REA target admission, limitation and verified release

Following the reverse-engineer-anything skill, the first bounded binary_session check found **open=false, cleanup_pending=false**, so no unrelated target was replaced. open_binary selected the immutable converter original above, PE/x86_64, with Ghidra12.1.4. Run ID `ee535413-a764-4a4d-a27c-9cd1be1158c9`, analysis-profile digest `bb0af08834149e89edadd849fc76d93580191b315769be5638e63d0f4ebf4051`.

The three focused xrefs requests (143d766b8,2c8070,2ca9f0) each failed `provider_timeout`: Ghidra import/analysis did not finish within its330000ms startup deadline. **No successful REA Evidence IDs were generated.** Exported `resume101-close-rea-evidence.json` explicitly has0 records/0 unknowns/105 bytes; it must not be cited as function evidence. `resume101-close-rea-status.json` retains bounded admission/errors/profile/release fields. No analyzer/configuration change or further import attempt was made after root's stop on retries.

Supported `close_binary({})` returned result:null. A subsequent binary_session verified **open=false, cleanup_pending=false, analysis_run=null, provider_bound=false**. The REA target is closed; ordinary PE observations below are separately byte-hashed tool evidence, not fabricated REA Evidence.

## Same native owner through the lifecycle

| Phase | Exact source chain / ordering |
| --- | --- |
| Application state |141ca0bb0 passes143d766b8 to2c7ee0 at141ca0bbb. Constructor member setup precedes2c7f27 ->2c7f80. Newly expanded helpers:2cb760 stores DWORD+0xc=0;2c81f0 stores qword+0=null;2cb900 stores byte+18=0. They contain no callee or callback. Then2c7fac stores+9=0,2c7fb6 stores+A=1,2c7fbf stores+B=1. Later initializer/member phases still exist; this is not whole-constructor completion. |
| Main lifetime | Application1410 passes its stack object at rsp+40 to10490 at1b39. Retained primary lineage10490 ->c98b00 preserves this. c98d23 ->c99530 publishes this through2c8070(143d766b8,this) atc9957b; c99588 only then initializes main+50=null. QCoreApplication/c993d0 work precedes publication and remains a separate constructor dependency. |
| Main/manager | Derived table1420fb108 and actual main+50 factory/backlink chain are qualified by CP77/80. Publication of the global main is earlier than the final manager store01344c. Candidate manager owner assignment is not native-main publication or Qt constructor completion. |
| Open | Selected table1420fb108+378 ->0271b0.028371 calls2ca9f0(143d766b8,true),028383 calls01ca30 on that same caller,02839a calls02a0e0 with the returned Project,0283ac calls01dc00, then0283ba calls the **same singleton** false setter.02a0e0's post-open02a9b0 also brackets active-subproject/view effects with nested true2aa8a/false2aacb setters. These are overwritten bytes, not a restored nesting count. |
| Explicit Save | Same table+380 ->028520. Current Project comes from01d510; process5 gate precedes0285c4 true busy.0285ce calls0980d0 on that resolved Project. The integer result selects status reporting; only after that returns02867d calls the same false setter. A completed ProjectSave alone does not certify this enclosing main-window setter/status sequence. |
| Close |031f10 preserves/reloads its caller and calls0299a0 at031f1e.0299cb reads singleton143d766b8 through2ca920(show=true); only unbusy and allowed actual model guards reach029a54 true setter. That setter later calls notifier143d76700 at2cac94. CP88's notifier owner is distinct from the application singleton. |
| Main teardown | Normal application-frame path calls QApplication::exec through IAT141e0bff0 at1fb6. After it returns and intervening cleanup,1ffc invokes105e0 on the **same rsp+40 object**;106b3 ->c991e0. After freeing main+118, c99245 clears143d766b8 through2c8070(null), beforec99890/ca72b0. Project Close is not this main teardown. Exceptional/alternate application exits remain separately qualified. |

This closes the selected original **static identity/order**, conditional on reaching each phase. It does not observe an accepted live occurrence, prove all synchronous callbacks preserve the state, or publish a candidate native main by assigning a Session address.

## Busy/current-main writer findings

2ca920 first reads singleton+9. Nonzero short-circuits the main query but public show=true still executes the busy-warning modal before its latched rejection. Zero reads qword singleton+0 through2c8090. Nonnull calls virtual+300; the retained table maps that to01daf0:

```text
busy = (QStackedWidget::currentIndex(main+480) == 2)
       OR (DWORD(main+270) == 1)
```

The first term short-circuits the second. Neither is a counter or the candidate MainView. main+480 and main+268 below are **different QStackedWidget fields**.

Bounded raw store scan of main-code VA10490..40000, followed by function-entry decoding, recovered:

| main+270 writer | Value and required predecessor |
| --- | --- |
|10701, initializer106d0 |0, after earlier virtual/helper initializer effects. |
|32688,32600 |0 after stack+268 index0, conditional state2 receiver effects and36580. Even initial state0 still reaches the later tree-selection-mode effect; this function is not wholly a no-op. |
|3278a,326c0 |1 after stack+268 index1, conditional state2 effects,35ed0 and temporary destruction; more tree effects follow. |
|32cfd,32800 |2 after writing previous state to+274 and stack+268 index2. |
|33d7e,33cf0 |Restore current DWORD+274 after preceding stack/effect work. A “reset busy to0” replacement loses this state. |

IAT141e0bb68 is exactly QStackedWidget::setCurrentIndex;141e0ba38 is currentIndex. The bounded region's main+480 pointer store is1882c in18790; the live stack's internal index/page mutations are not recovered merely by locating that allocation slot. These candidates are not a full cross-binary field-alias write proof.

Whole-.text raw E8 scan found66 direct busy-setter candidates,12 field-A setter2cb530 candidates and2 field-B setter2cb570 candidates. Selected lifecycle sites above were independently decoded. Other+A writers exist outside the common busy setter; do not claim all application state changes are covered by Open/Save/Close's setter pairs. The scan is incomplete for indirect/tail/alias writes and raw byte matches can require validation. Full candidate lists and function bounds are in `resume101-close-pe-writer-chain.json`.

The false setter first clears+9/counter, then queries current main busy and writes+A/+B plus its conditional Tree request. That Tree callback and earlier Save status callbacks are synchronous dependencies. Thus last-known+9=0 does not prove virtual+300 false, nor do constructor+270=0 and application scope's exclusion of explicit simulation running establish the state after unknown delivered callbacks.

## Public integration seam and exact remaining gate

Current core/convert.cpp still invokes Open/Project Save on an actual Session/manager/Project, then leaves scope; it contains no normal-Close callsite. Session destruction remains adapter cleanup. CP58 busy/main values are supplied observations; CP59/61 own supplied field copies. WidgetAreaPrimaryWindow provides canonical role identity without native constructor/WidgetState/current fields or Session binding. CP88 uses the real shared notifier but cannot supply the separate application singleton. The current code hashes are pinned in the PE receipt.

The narrow real integration is **one actual application-state/main owner spanning the already-existing public Open/Save driver and the later Close call**, not another isolated receipt. Its producer must honor the startup/publish-before-main+50 order above, use one canonical main/manager identity, and retain actual+9/A/B/current-main/+270/+274/stack owner state through intervening effects. Open and explicit Save's native setter pairs must write this same storage; Close must read it. Do not materialize late constructor defaults when the driver reaches Close, substitute MainView, or implement an unconditional false reset after ProjectSave.

Once that producer exists, run's same live Session **after the selected complete Save invocation and before adapter destruction** is the public Close insertion seam. It must first execute the actual busy predicate and model can-close chain. Successful .moproj Open has nonnull moProject/process5, so shipped default Yes still requires its actual modal due-event effects and nested0980d0 Save result, followed by the source answer remap and callbacks. Existing explicit Save does not discharge nested Save. Only then can shared true stores, CP89's stack argument views and CP88's actual notifier prefix run; unsuppressed still stops at key QString2cf41f. A null-Project precommit failure seam is a different lifecycle branch.

No currently source-ready public Close success follows. The precise gap is now an **actual candidate constructor/writer/effect cursor for this recovered native owner**, plus current-stack/modal effects, rather than missing knowledge of which original object owns the bytes. Source closure of the three initializer zeroing leaves permits that owner design to start from actual fresh-constructor phases; it does not permit treating a late conversion Session as fresh-null native main.

Focused public validation after that gate: drive one actual WRITE_SAVE conversion, retain identical application/main/manager/Project/notifier identities across Open, explicit Save, Close predicate and default/nested Save; verify callbacks see each reached store in native order; stop before the first unavailable key/provider while retaining prior file and state effects. Cover state2/+274 restoration and callbacks changing the live stack/main predicate, and prove Project Close does not clear the global main or destroy the dispatcher. Declared fixture schedules remain reconstruction tests, not original accepted-event observations. No helper-only production task is proposed from this lane.

## Receipts and limits

`resume101-close-pe-writer-chain.json` SHA-256 `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1` retains exact end-exclusive code-byte hashes, raw scan scope/results and retained/current-source hashes. New leaf hashes:

- 2cb760..2cb772: `199db795020ea2f15be4fec3f0fb3f241c513a9cc7699b880dee4e4cc1efe430`.
- 2c81f0..2c8202: `445763c354d72d534608d57dded763eaf703877c3e33b7a3ff4250ac10e0a36e`.
- 2cb900..2cb90f: `0ef1f4fd2fbe5da504f095e33142ae5563e96896901280bd7665b7bf3becde45`.
- 0271b0..028515: `d4c9167bed25a9d256b57cc3fc9fddafbfedd2441aef8a41586956a2af1a80d5`.
- 028520..02868e: `e212e4eaa570c3944149a78cb08ab64e667616ec8ac9ef556c61f30caf9c9c73`.

Retained qualifications include CP98, CP88/89/90, wave3-normal-close-guard-contract.json and wave3-editor-owned-open-entry-140001410.asm. Static call/owner facts are not live callback-state coverage. REA startup failure is a provider limitation, not a negative xref result. Supported close and its verified closed status are recorded separately; no target or GUI-control session remains open from this lane.
