# CP114: actual constructor factory and later main-owned manager deletion

The original main+50 ModelManager factory now has a concrete constructor caller, not just the previously retained metacall case: **10490 ->111c0 ->c999e0 ->virtual+2e0 ->0133f0**. It runs before main+480 stack construction. Later Project FileCloseAll clears its children but retains the manager; main destruction first unpublishes global main, then deletes that manager and only afterward nulls main+50. These static edges clarify the shared owner bridge that the converter still lacks. No isolated helper was added because existing factory/constructor projections already cover the independently executable local pieces.

Only this report was written. Bounded retained/local source, immutable PE, DUMPBIN, import, vtable and RTTI reads; no shared source/interface, plan, build, test execution, GUI, original launch, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed. Native function/site addresses below are RVAs relative to140000000; globals/vtables/IATs use full VAs.

## Constructor caller closed

CP106/107 establish application1410's stack main ->10490 and basec98b00 ->c99530's publication of the same main atc9957b. Base initializes main+50=null afterward atc99588.10490 installs derived table1420fb108 at104dd..104e4, calls106d0 at1058e, then111c0 at10598.106d0's settings/toolbar/registration and other predecessors remain effects, as qualified by CP112.

The newly decoded sequence reloads the same incoming main receiver at each hop:

| Site | Direct/static-dispatch edge |
| --- | --- |
|111d8|111c0 calls c999e0 before its own virtual+2c0 and later setup.|
|c999f6|c999e0 dispatches virtual+2e0 on that receiver. Derived table1420fb108+2e0 contains1400133f0.|
|013407|0133f0 requests0x28 storage.|
|013419..013423|Passes the same main as second argument to045420 on the allocated manager.|
|045438|045420 invokes basec781d0 with that owner.|
|045449|After base returns, installs SfMoModelManager table142115f68. Correct section-mapped RTTI is `.?AVSfMoModelManager@@`.|
|01344c|After constructor return, stores that returned manager into the same main's qword+50. Explicit allocation-null branch supplies null instead.|

0133f0 does not test/reuse an existing main+50 or delete its prior value. Do not introduce such a safety policy when reconstructing this factory. The selected first constructor call follows its earlier null initialization; later arbitrary/metacall invocations are separate occurrences and must not inherit that first-call profile.

Basec781d0 constructs QObject(parent=null), the list+18 and current guard+20, stores owner+10=main atc7822c, then callsc782b0 atc78235. **CP80's correction applies:** assignment helpercbc920 compares the fresh null guard with null and skips changeGuard/pointer mutation on equality. Calling the assignment helper is not evidence of an actual pointer write, child clear or blanket reset. Only then does045449 install the derived manager type and01344c publish main+50.

After the factory returns, c999e0 dispatches virtual+198 atc99a09, +1a8 atc99a1c, and +1b0 atc99a2f; selected targets are012390,012580 andc9f4b0. Returning from the manager factory is therefore an earlier cursor than returning fromc999e0.111c0 next dispatches+2c0 ->011020 at111ea, performs other setup, and only reaches1121d ->18790 later. CP112 then binds18790's QStackedWidget result to main+480 at1882c. Neither the known manager nor its fresh null current establishes the stack/index or overall main readiness.

This closes the **static constructor factory-caller gap** in CP78's earlier discussion, which only named metacall01fe17. It does not observe original execution, close Qt/base/setup effects, or prove post-Open/Save field values.

## Distinct Project and main destruction occurrences

Normal FileCloseAll031f10 ->0299a0 obtains this main+50 manager, performs its can-close/busy/refresh predecessors, then029a76 callsc78300. Children are deleted before the owning list is cleared and current reset. That path does not delete the ModelManager allocation or null main+50. CP110's passing-check Save answerNo10000 can still accept despite project/MST writer false; writer failure does not make this manager/Project absent or remove earlier Close guards.

The selected regular application-frame main destruction is different:

1. After QApplication::exec returns, main1410 calls105e0 on its same main stack object at1ffc;106b3 enters basec991e0.
2. After earlier teardown/free(main+118) atc99237, c99245 calls2c8070(singleton143d766b8,null). Global main is now null **before** the subsequent manager deletion below.
3. c9924f callsc99890. That body first deletes owned+170 and+168 through their deleting virtuals and releases+178. Only afterwardc99949 callsc9b7e0 on this same main.
4. c9b7ee tests main+50. Known null skips the deletion and the later null store entirely. Nonnull reaches its deleting virtual+18(flag1) atc9b827. Only **after that returns** c9b83f writes main+50=null. Do not pre-null the field or preserve the global main through these destructor callbacks.
5. Manager table142115f68+18 resolves045bf0. It calls045460, which installscorrect derived table and calls basec78250. Base installs its own table, callsc782e0 ->c78300 again, destroys current guard+20 and list+18, then invokes imported QObject::~QObject atc7829f. The PE import at141e042b0 was independently resolved as `QtCore4_64.dll!??1QObject@@UEAA@XZ`.045bf0 then frees the manager for flag1, beforec9b83f's null store.

Thus even a manager emptied by earlier FileCloseAll has a later destructor/clear occurrence. Container/guard/QObject callbacks and physical Qt bookkeeping remain qualified; the empty child population alone does not make the whole destructor a no-op. Normal Project Close must retain this manager and main lifetime, while later main teardown must preserve the distinct global-null-before-manager-delete order. Session's final shared_ptr release is an adapter cleanup boundary, not proof of either native occurrence.

## Actual Session bridge and smallest remaining task

Current `ConversionSession` constructs `modelManager_ = make_shared<ModelManager>(this)` after WidgetRuntime and before MainView. ModelManager's owner_ is a `ConversionSession*`; CP80 implements the actual candidate assignment and fresh-current equality-skip receipt. CP78's Open receipt reads this same actual manager before child allocation. Those are useful real adapter facts. They do not change that native manager+10 names the actual primary main, whose lifetime and constructor phase are separate from the Session.

Existing WidgetAreaPrimaryWindow is an identity-only projection that refuses Qt/base/widget effects. CP77's main-manager factory binding records diagnostic identities rather than executing this constructor caller. CP112's new pure busy predicate compares explicit values without producing them. Creating a second lease or renaming a Session/MainView address cannot join these owners.

The smallest meaningful shared integration must bind **one actual main constructor owner to the one existing manager**, at111d8/c999e0's factory occurrence, after admitted application/base/106d0 predecessors. It must distinguish the native main backlink from the Session adapter backlink, preserve base/list/guard/owner/derived/publication order, and give Open'sc9b7ca field read and Close's wrapper that same published manager. It cannot allocate a second manager during Open or call factory observation methods to certify already completed C++ construction. Such work needs root-owned shared construction/interface changes and the still-unimplemented QApplication/main producer, not an independent new helper file.

The source-closed constructor continuation is exact: after01344c, stop beforec99a09's virtual+198. But no production cursor currently reaches it. The first earlier actual producer gates remain CP107's pre-application7fc450 and SfApplication21c0/QApplication21f1, followed by real QMainWindow/base/setup effects. A late fixed-null/fixed-unbusy owner would fabricate those phases. Consequently no extra isolated model/test is warranted in this lane; the new evidence narrows the existing integration dependency.

Future focused integration tests must drive that real constructor owner, reject derived/stack use before its phase, assert manager/backlink/publication identity before18790, then keep main+50 through Project Close. A separate main-teardown fixture must observe global main null before manager deletion while main+50 still names the manager during its deleting virtual, and null+50 only after return. Constructor-null allocation and later factory reentry must remain separate profiles. Public FileCloseAll still needs fresh busy/current-main/modal/nested-Save/display/destructor effects; neither tests nor static factory reachability supply them.

## Compact byte receipt

DUMPBIN14.44.35228.0, section-aware PE mapping; end-exclusive spans. Retained CP112 qualifies10490/106d0/111c0/18790 and table phase. New bounded body hashes:

| Span | SHA-256 |
| --- | --- |
|140c999e0..140c99a3a constructor setup dispatcher|`073af7e4e10b7f4f4e5b932b477671e0d861670e4137493edccba522f8f617e6`|
|1400133f0..140013455 factory|`69786225c6006fac0152b3be6819a1a0085d1556a286b5c69b8b2310f30f1905`|
|140c9b7e0..140c9b84c manager deletion/field-null wrapper|`1857a2563e1707128c7b1b8e22ac455af358df13d5e5fa3e2d7a04d8d76dccb5`|
|140045bf0..140045c26 manager deleting destructor|`d68487f412b1fe76a95ef94665c13ae1753ad5711e2486bd40227f744d529a64`|
|140045460..140045487 manager destructor|`7c74ba2f0459e41f3a125f5bf3d546b8a810bcb15105c98bf708477f1c77a4fc`|
|140c78250..140c782aa base manager destructor|`fd9372ec4a47116d1beb312c42cd4ce90c23852c556d8a0311c8057886fcecf2`|
|140c782e0..140c782f8 clear wrapper|`59833ef656454fd06604021ac86205e667e774c66cda163c945f5510adc806eb`|

Retained qualifications: resume77-close-main-wrapper.md; resume78-session-manager-open-integration.md; resume80-session-manager-producer.md; resume107-app-main-lifetime-call-boundary.md; resume110-close-after-save-failure.md; resume112-close-main-stack-producer.md; resume112-close-pure-decision-implementation.md. Current source read: engine/model_manager.hpp/.cpp, engine/conversion_session.cpp and engine/widget_area_primary_window.hpp/.cpp under source/apps/moproj2mst. No new runtime Evidence ID, tested-candidate freeze or accepted-Close witness is claimed.
