# CP149: post-event-loop destruction is a distinct native route

One narrow conditional native route can destroy a Project/SUB left attached after Open/Save **without proving a later FileCloseAll was accepted**: the regular application frame returns from QApplication::exec, destroys its same main object, and that object's ModelManager destructor clears remaining Mo children. This route is statically closed through the known Project/SUB disposer, but its application-lifetime trigger and feature/Qt owners are not represented by candidate Session cleanup. No new behavioral source increment is ready from this route alone.

Only this report was written. No source/build/test/GUI/app-control/original execution or mutable REA access. Retained CP107/110/114/143 evidence was reconciled with bounded DUMPBIN14.44.35228.0 and section-aware PE/.pdata/vtable reads. Original retained DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## The selected reachable static path

The main function140001410 constructs the primary main on its stack and enters QApplication::exec through IAT141e0bff0 at140001fb6. **If that call returns normally** and intervening cleanup returns,140001ffc calls1400105e0 on the same `rsp+40` object. This is a conditional native call path, not evidence that Save itself causes exec to return or that a live original occurrence has been captured. Explicit Save140028520 has no ensuing FileCloseAll/main-destruction call in its returned path.

1400105e0 first performs its derived teardown, including1400108e0's ordered owned-component helpers, then1400106b3 calls base140c991e0. After preceding work/free(main+118),140c99245 calls1402c8070(singleton143d766b8,null). The current global main is now null while the concrete main allocation and its manager field still exist.

140c9924f next calls140c99890. That body deletes owned+170/+168 and releases+178 before140c99949 calls140c9b7e0 on this same main.140c9b7ee tests actual main+50. Null skips manager deletion **and** its later null store. Nonnull calls the current manager's deleting virtual vt+18 with flag1 at140c9b827; only after return does140c9b83f write main+50=null.

For the source-created SfMoModelManager, actual table142115f68+18 contains140045bf0. Its sequence is140045bf0 ->140045460 ->140c78250 ->140c782e0 ->140c78300. Child clearing therefore occurs while global main is null but main+50 still names the manager. Manager's current guard/list/QObject destruction and scalar allocation free follow; the outer main field is cleared only afterward. A manager already emptied by accepted FileCloseAll still has this separate destructor/clear occurrence.

For any remaining actual SfMoModel child,140c784f2 invokes child vt+18(flag1), table142112878+18 ->140045350 ->140044040 ->1400440c0 ->140044bf0. A nonnull current child+19c0 P receives1400982c0 ->1400aab30 ->1412f1190(P+60) feature release before its deleting wrapper1400453e0. Project14008a3c0 does diagram null-rebind, then1400a80e0's ascending Project+58 SUB deletion through1400ac330 ->1400b58d0, then inline feature-container/member destruction. Mo+19c0 is cleared after Project destruction; the five Mo slots follow. CP142/143 qualify the detailed inner sequence and raw DOE-slot limits.

The Project at that later read must be the actual then-current P. No cached explicit-Save pointer may replace source rereads after event-loop/derived cleanup/feature callbacks. This path establishes conditional native identity flow, not that all intervening callbacks preserve the originally saved Project or that every nested effect returns normally.

## Why this does not authorize Session destructor replay

Current `core/convert.cpp` runs a per-invocation Session around Open/Save and maps errors/results. Its stack `ProcessCodecOwner`, added at CP147, is inert Unknown state, allocated without native effects and borrowed only by the full WRITE_SAVE Session. It has no application/main/manager publication, widget phase, exec-return, busy/current-main or feature-provider semantics. Its lifetime across Session/catch/details is not the original QApplication/main lifetime witness.

`ConversionSession::~ConversionSession` still explicitly performs adapter cleanup: revoke receipts, release Project storage, then receiver/dispatcher/MainView/adapter scopes. `releaseProjectStorage` retains Display and actual model lease, removes ProjectScope, destroys Project and clears a still-matching backlink. `Project::~Project` invalidates feature-anchor metadata; `SubProject::~SubProject` records flow receiver death, followed by ordinary C++ member cleanup. These are useful storage guarantees, not this native main teardown/feature/Qt protocol.

The adapter's ModelManager still holds shared child leases; resetting one lease need not invoke immediate native deletion while external owners exist. Its `clear()` is expressly a projection. Empty-tail Close helper refuses the live post-Open nonempty child before deleting virtual effects. It cannot be promoted because the original virtual target is now known. Feature anchor remains ProxyAllocationPending/HeapProfileUnavailable with native construction/feature values/registration unavailable; isolated FeatureCrtRuntime/ContextEnvelope test frames are not this Project's owner.

The inspected source bodies confirm CP144's guard conclusions remain valid after CP147's codec-owner change. In particular, do not clear global main at every successful/failed converter return, run29-feature cleanup on an empty fabricated context, or attach DOE manager/optimizer smart-pointer deletion order. Native SUB directly omits raw+20/+28 cleanup; CP143 does not establish permanent retention or a whole-program leak.

## Exact next witness and bounded test

For this alternate teardown route, the first missing trigger/owner is a source-produced **actual main/application frame** with admitted exec return and same actual main+50 manager—not merely unbusy observations or a Session pointer. Derived owned-component teardown before base unpublication is also a real predecessor. At the later nonnull child branch, actual Project feature construction/Open-registration/provider history remains missing before1412f1190; diagram/SUB/OPI/QObject effects remain guarded. Closing FileCloseAll's decision alone would not supply an application-exit occurrence, and vice versa.

No additional helper-only prefix is proposed. A source-ready future integration must bind the actual constructor-owned main and existing manager, preserve the separate global-main and main+50 cells, and drive its actual lifetime phases. Only then can a focused test observe global main null **before** manager deleting virtual callbacks, manager field still live during those callbacks, and null only after return; for an actual child, stop before missing feature release without clearing Project/backlink/current. Empty-manager destruction still needs its guard/list/QObject phases rather than silently skipping them.

Current regression tests can verify the negative boundary with real Session/Project/registered Mo/anchor/slot leases: CP147's inert codec borrow does not make feature/native-main facts Known, nonempty Close entry/tail remains refused, and adapter Project release expires the anchor while retained Mo slots survive. Existing registered-session, feature-anchor, empty-tail and codec-owner tests cover most of this; a combined test would protect against a wrong future callsite, not advance original lifetime coverage. No test was written or executed here.

## Native half-open byte identities

| Range | SHA-256 |
| --- | --- |
|140001410..1400020b4 application main|`fa383df94e27d4e11c061912393078109de4e5ce5abb9cd5513276ddf4cc922f`|
|1400105e0..1400106bd main derived destructor|`89427261a69c8afac10249f92d9973379660bdb47496455302c276aae1a64474`|
|140c991e0..140c993be main base destructor|`299716c09bdd725f073efdd404176385ff7a56e6b0924c919f03fc21bb9e344f`|
|140c99890..140c9997b owned cleanup dispatcher|`8f483e95a147cc07208dfe98045bb0444b730766078f66304a3eb052e5f8f453`|
|140c9b7e0..140c9b84c manager delete/field-null|`1857a2563e1707128c7b1b8e22ac455af358df13d5e5fa3e2d7a04d8d76dccb5`|
|142115f68..142115f88 manager first4 slots|`c77033b284f21b7e28a6e28c5e1d5bdf56ef0d716567440baa6fe95bfc5f11af`|
|140045bf0..140045c26 manager deleting wrapper|`d68487f412b1fe76a95ef94665c13ae1753ad5711e2486bd40227f744d529a64`|
|140045460..140045487 manager derived destructor|`7c74ba2f0459e41f3a125f5bf3d546b8a810bcb15105c98bf708477f1c77a4fc`|
|140c78250..140c782aa manager base destructor|`fd9372ec4a47116d1beb312c42cd4ce90c23852c556d8a0311c8057886fcecf2`|
|140c782e0..140c782f8 manager clear wrapper|`59833ef656454fd06604021ac86205e667e774c66cda163c945f5510adc806eb`|
|140028520..14002868e explicit Save wrapper|`e212e4eaa570c3944149a78cb08ab64e667616ec8ac9ef556c61f30caf9c9c73`|

Retained controlling reports: `resume114-main-manager-constructor-lifetime.md` for this main-unpublication/manager order; `resume110-close-after-save-failure.md` for Save/Close separation; `resume143-close-indirect-owner.md` SHA `9d7186d66aafd6b33954a96238b7cb5a43392d39765de25cf8829eff6bcb185b` for inner feature/Project/SUB chain.

Current source inspection hashes beneath `source/apps/moproj2mst`: core/convert.cpp `f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce`; engine/conversion_session.cpp `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5`; engine/model_manager.cpp `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4`; model/project_owned_storage.cpp `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`; model/project_feature_context_anchor.hpp `44883c50ec41c56d7a0f596f46647d6a931de5c7fd7862faba2a1641a30fe48a`; model/subproject_lifetime.cpp `849873b5f1f4c139bb815bc3a92b897c104a9fc38af016178aa853f89a131f64`; include/m2m/engine/process_codec_owner.hpp `5c353b68088024b586e322bcaedd055fbc220777b015e578c012270afcdea680`. These are read identities, not a new candidate freeze or deployment receipt.
