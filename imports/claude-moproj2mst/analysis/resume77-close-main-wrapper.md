# CP77: fixed Mo main/manager factory witness and next main-field boundary

The native factory supplies a statically closed same-owner witness. SfMoMainWindow virtual+2e0 allocates a concrete SfMoModelManager, passes the same main window to its base constructor, and stores the returned same manager at main+50. The base writes manager+10 back to that same main; the derived constructor installs the exact type requested by0133a0's cast. This is stronger than assuming a generic manager can be downcast. It is still a static source relationship, not a captured current native object or a represented adapter factory handoff.

The smallest next adapter prerequisite is an isolated observation of this concrete identity chain, with all fields initially Unknown and native wrapper execution still guarded. Once an actual main-owner field exists,02a107's unconditional main+3c0=0 is a separate direct prefix; stop before013d40 until its process-context/provider dependencies are bound. A null Project does not make013d40 an immediate process-type reset.

This evidence report was the only write during investigation. No source/build/tests/GUI/original execution/mutable REA ran. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded offline PE/.pdata/RTTI/import/DUMPBIN supplied new views, retained inline below. No reconstructed executable/deployment was inspected or changed. A subsequent separately authorized source lane has its own implementation receipt.

## Static factory/backlink/concrete-type chain

PE RTTI names primary table1420fb108 `.?AVSfMoMainWindow@@` (COL1430d46d8). Its+2e0 qword at1420fb3e8 is0133f0. A raw qword search found this factory pointer once in this original file; that is a bounded pointer-search result, not proof of complete indirect reachability.

```text
013407 allocate0x28 manager block
013419 rdx = original main this
013423 call045420(manager block, same main)
013438 retain returned manager
01344c [same main+50] = returned manager

045438 callc781d0(same manager, same main)
c781ee QObject constructor(parent=null)
c78201 install SfModelManager table1428fa478
c7820d/c7821c construct list/current members
c7822c [same manager+10] = supplied same main
c78235 further base initialization
045449 install SfMoModelManager table142115f68
04544c return unchanged manager this
```

RTTI identifies142115f68/COL1430d55e8 as SfMoModelManager and1428fa478 as SfModelManager. The Mo manager hierarchy contains SfMoModelManager, SfModelManager and QObject, each PMD(0,-1,0).0133a0's source descriptor1435693f8 names SfModelManager; target143569420 names SfMoModelManager. The factory's normal returned object therefore has the exact zero-offset concrete relationship requested by the wrapper cast, provided that actual factory occurrence/field value survives to the read.

0133f0 also has an allocation-null branch that stores null at main+50. It is not a successful nonnull binding.0133a0/01d510/045780 do not supply a convenient safe wrapper result for an unavailable/null concrete manager:045780 immediately calls the manager current getter. Preserve an adapter guard rather than inventing native failure recovery.

At the native wrapper,0133c6 freshly reads that main+50;0133ca executes its cast;01d526 enters045780. CP76 implements only the same-actual-manager current-null inner leaf. The missing adapter witness is the canonical main/manager association **at this occurrence**, carrying the observed main-field store/read, manager backlink and concrete profile. The C++ Session ownerSession pointer and nonblank labels alone do not record those native events or prove full QObject/manager constructor completion.

## Main+3c0 and013d40: exact next guard

After wrapper return,029a93 calls02a0e0(main,Project).02a107 always writes main+3c0=0, then02a11b calls013d40(main,mode0,showBusy=false). Only afterward does02a120 test the passed Project pointer. Thus no old+3c0 value is needed for the direct zero store, but its actual canonical main owner/cell is needed. It is not global busy+9, MainView state, widget-area+3c8 or a status-private field.

013d40(false) skips its optional initial busy-warning gate, then:

1.013d86 reads the **previous** process type through2cb7a0(global143d766b8).
2.Before its fresh Project lookup, it dispatches type5 ->03aeb0, type1 ->03b230, type4 ->03b590, type3 ->03b920. Other values skip these selected calls. These are actual provider branches, not a generic zero-reset.
3.013dfe again obtains this main's manager through0133a0;013e06 calls045780. This is a new current-Project lookup **after** the type-specific provider. An earlier CP76 null result cannot replace it without closing intervening effects.
4.For mode0 and then-current null Project,013e4d..013e61 stores process type5 through2cb780. A Project still current selects its own open-success-dependent value. Other later UI/context effects remain dependencies.

The first type5 provider is not closed by this report. Its bounded prefix reads another global owner through2c80f0, may callc8c6d0, and retrieves main+480's QWidget at index1. Import141e0b820 is **QStackedWidget::widget(int)**, a retrieval, not setCurrentIndex. The later provider body/callback population is not replaced with an empty effect. The process-type owner is a separate global context; Session construction supplies no late value or reset.

Existing WidgetAreaWindowOwner represents+3c8 and+480 diagnostic cells; it has neither+50 nor+3c0 and is not a Session bridge. CloseStatusMainOwner similarly represents a separately declared main/layout projection. Do not create multiple supposedly actual main identities merely to populate separate helpers or borrow MainView's QWidget/private lifetime.

## Smallest isolated observation interface

Use one factory-occurrence identity record with a canonical main-allocation **diagnostic lease** and a weak canonical actual candidate ModelManager owner. Start with unknown manager identity, unknown backlink/class and unknown main+50. Source-point observation APIs may record:

1. c7822c's same manager+10 -> same main association.
2.045449's concrete Mo class store on that same manager.
3.01344c's same main+50 -> that same returned manager store; the allocation-null alternative records KnownNull without constructing a manager.

Require matching pointer and shared-control-block identities; preserve nonnull/unavailable versus observed null on weak expiry. Repeated identical observations may be idempotent, while conflicting owners/profiles/order/rebinding refuse without partial changes. Do not infer native liveness, parenting, counters, Qt construction, current original-object observation or completed factory execution from this record. Creating another record/lease must not authenticate or intern an unrelated owner.

An inspection result can report a **known concrete identity projection** and retain `nativeWrapperQualified=false`. Source observations are not executable constructor/destructor cursors. Actual production integration still needs the real producer/capture to populate this one shared main/manager graph; the observation API must not be called merely to self-certify public Close. A later isolated main+3c0 write can use that same canonical owner, commit zero once and stop before013d40; this report does not implement or release it.

Focused tests: unknown defaults; actual candidate manager/backlink/class/field identity; source order; different main or manager/control-block aliases; repeated observation receipt retention; allocation-null branch; nonnull target unavailable after lease expiry remaining nonnull; and inspection never enabling native wrapper/public Close or changing ModelManager list/current/connections. None were created/run in the evidence lane.

## Evidence receipt

New bounded views are retained inline here; no raw file was added. DUMPBIN14.44.35228.0 from MSVC14.44.35207 used the .pdata bounds below. PE imports identifyc781ee as QObject::QObject(QObject*) and03af00 as QStackedWidget::widget(int). Code bounds are end exclusive and do not imply transitive constructor/provider closure.

| Native region | Code SHA-256 |
| --- | --- |
| 1400133f0..140013455 | `69786225c6006fac0152b3be6819a1a0085d1556a286b5c69b8b2310f30f1905` |
| 140045420..140045456 | `0052fb0036768823e5b72a0754727b632cc19067bbbf1b6ebab32f074a2c96c7` |
| 140c781d0..140c78245 | `f2ebcdb78fe524c06569c83be243c295961ebca31029de67dfb461d1b1c04bde` |
| 140013d40..140014119 | `a1e5ae45dec8b8fe002b7d50530f8dfc8c55208878845ca207e9e17e0f76426a` |

The static same-owner chain is closed. Its actual original/candidate occurrence binding, constructor completion, main+3c0 owner cell, prior process type/provider effects, fresh post-provider Project lookup, subsequent notifications/status/global-main/private/cache lifetime and one-project native acceptance remain unresolved.
