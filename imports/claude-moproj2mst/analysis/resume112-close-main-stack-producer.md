# CP112: actual main busy-input producer and its first store boundary

The next concrete producer is main-window setup111c0 ->18790, which constructs the **actual QStackedWidget stored at main+480**. Close's busy predicate reads that widget's QStackedLayout private current-index field, not a Session/MainView value. This audit closes the allocation/getter ownership chain, but not the constructor's callback effects or its live post-Open/Save value. No detached model/test was added: without the shared actual main/cursor it would duplicate supplied facts rather than advance public Close.

Only this report was written. Retained static contracts/current source and bounded offline PE/DUMPBIN/import/export/RTTI reads; no source, shared interface, plan, build, test, GUI, original execution, REA retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. No reconstructed executable/deployment was inspected or changed. Native application addresses below are RVAs relative to140000000; Qt addresses are full VAs.

## First actual-main writes, before the stack exists

CP106/107 bind CRT initializer141ca0bb0 ->2c7ee0/2c7f80 on singleton143d766b8, then application1410/pre-app7fc450/SfApplication21c0 and its QApplication base, then primary SfMoMainWindow10490. Basec99530 publishes this into singleton+0 atc9957b before main+50 initialization; that publication is not a completed derived constructor.

The same10490 object receives its derived table1420fb108 after basec98b00 returns.10490 calls106d0 at1058e, then111c0 at10598. At106e6,106d0 first calls virtual+2c8; immutable main table+2c8 resolves10aa0 ->40810(global143d768f0).40810 calls settings getters808270/808860/8085c0 and further providers. The retained bool-getter contract says missing keys preserve destination entry values; it is not an unconditional zero reset. This predecessor must return before the following direct main stores:

| Site | Exact field write |
| --- | --- |
|106f1|qword+2c8=null|
|10701|DWORD+270=0|
|10710,1071c|byte+242=0, byte+243=0|
|10728,10734,10740|byte+3b0=0, byte+3b1=0, byte+3c0=0|
|1074c,1075c|qword+220=null, qword+228=null|

This straight-line store block contains no call. Its next effect is virtual+1b8 at10774, bound by CP77 to toolbar producercaa9e0, followed by QWidget::hide and connectSlotsByName. Later106d0 work includes1080c ->184f0 ->184fa's qword main+3c8=null. Thus even that simple null store is later than real toolbar/provider effects. The earlier block does **not** initialize main+274 or construct main+480, and +2c8 (diagram-view field) is distinct from virtual-slot offset+2c8.

The first source-closed store interval for a future actual-main cursor is therefore **after106e6 returns, before106f1**, preserving the ordered block and stopping before10774. It is not callable now from late Session construction or Close. It requires the earlier real application/base/derived owner and settings continuation; no source-backed admission for those phases was acquired here.

## Actual +480 allocation and current-index ownership

111c0 reloads its saved incoming this for each call. Before1121d ->18790 it callsc999e0, virtual+2c0,12040,138c0,13560 and13b00. These are explicit setup predecessors, not bypassed constructor defaults.

At18790:

1.187b9 requests0x28 storage through146eabc. On its nonnull branch,187d2 calls imported QStackedWidget(QWidget*) at IAT141e0bb50 with parent=null.187dd/187ec install primary/secondary tables1420fb9d8/1420fb998; their correctly section-mapped RTTI identifies **QStackedWidget**, not a new application subclass.
2.1882c stores that same result into saved main's qword+480. The explicit allocation-null branch supplies null to this store; it does not prove subsequent null-widget method calls are valid.
3.18888,188df,18904 and18929 invoke imported QStackedWidget::addWidget, IAT141e0bb58. Their page providers include138e0 plus its virtual-return chain,18460,13290 and18b10. Later18790 also performs dock/layout/virtual work before returning18ad4. No final current-index value is inferred from allocation or the count of these calls.

Immutable QtGui gives the exact getter/owner chain:

```text
main+480 -> QStackedWidget
  object+8 -> its private block
  private+208 -> QStackedLayout object
  layout+8 -> layout private block
  layout private DWORD+c8 -> currentIndex
```

QStackedWidget::currentIndex65405af0 loads object+8, then private+208, and tail-jumps650501e0. That getter reads layout+8 then DWORD+c8. This is the first term of native main01daf0's busy predicate; only when the value is not2 does it test main DWORD+270==1. This +c8 is a **QStackedLayout private** field, unrelated to unresolved QWidget/status-item cache+c0 ownership.

QStackedWidget constructor65405990 allocates a0x218 private block, runs653b20e0, initializes private+208=null at654059d4, and calls653b2470 before installing widget tables. It then allocates0x20 for QStackedLayout, calls its QWidget-parent constructor65050810 at65405a35 with this widget as parent, and stores the result into widget private+208 at65405a53. Connection calls follow at65405a76/65405a9c. The constructor's private208-null is consequently an intermediate state, not the ready getter input.

QStackedLayout65050810 allocates0xd0 private storage, calls65050740 and then65046b10 before installing its tables.65050740 first runs65043890, initializes/acquires its shared list storage, then writes private DWORD+c8=-1 at65050781 and DWORD+cc=0 at6505078b. Those stores qualify the selected private-constructor phase only.

The first addWidget is already effectful:65405ab0 obtains that real layout and tail-jumps65050e80, which obtains the list size and delegates65050d80 (insertWidget). Its child-adoption/list/item work and virtual invalidation precede65050e0f's current-index test. Negative index calls65050990 (setCurrentIndex); nonnegative insertion can increment+c8 at65050e2b and takes further widget effects.65050990 itself has chained unwind/code continuations beyond its first .pdata fragment; this audit does not declare it a fully closed leaf. It cannot be replaced with a direct private+c8=0 store or a fresh-window “not busy” fact.

## Actionable integration prerequisite and limit

A production slice must first provide the **same actual main constructor cursor**, downstream of CP107's QApplication/pre-application gates, and bind that owner to the existing actual Session manager/Open/Save driver. Then it can execute the106f1..1075c store block at its precise phase, preserve the guarded10774 toolbar boundary, and later reach1121d/18790 to bind the distinct stack/layout/private owners. A value-only private-index model would require those actual identities and lifetime phases; it cannot make them available. Current WidgetAreaPrimaryWindow remains identity-only and explicitly refuses Qt-base/widget effects, while ConversionSession/MainView owns different adapter objects. Any real integration therefore touches shared ownership/interfaces and is outside this isolated-file authorization.

No purely isolated new implementation is justified by this audit. In particular, initializing an invocation-local main index or using the already initialized main+270=0 would skip settings, widget, layout, adoption, signal and callback predecessors. A focused future test should prove publication during base phase cannot admit derived busy dispatch; guard before settings preserves every later field; post-settings continuation performs only the ordered store block and stops before toolbar; stack allocation does not admit currentIndex until actual layout/private construction; insertion refusal preserves earlier effects and blocks a fabricated late index. It must use the production constructor owner, not a new detached fixture offered as public parity.

CP110's corrected Save failure distinction remains unchanged: project/MST false leaves Project bound and can still yield native answerNo10000. Later FileCloseAll031f10 ->0299a0 reads the then-current singleton+9 and this live main predicate, then can-close/modal/nested Save. The present audit supplies no modal schedule, accepted-Close root, or permission to invoke cleanup from converter catch/destruction.

## Byte receipt

DUMPBIN14.44.35228.0; section-aware mapping; end-exclusive spans below. Constructor/setup **prefixes** are labelled and are not whole-function closure. Raw direct-E8 scans only located candidates1121d ->18790 and1080c ->184f0; both were decoded within their real enclosing functions. This does not claim indirect-call completeness.

| Span | SHA-256 |
| --- | --- |
|1400106d0..1400108d8 initializer|`ca96cd55d05c58bdbbbe46cfd80a658885b1a590f8a99b99cae362f6a1fd17fe`|
|140010aa0..140010aba settings wrapper|`b6132232d495a96db7ec299f85874a820ade8d51b62222f1ca3b4241e78ab33c`|
|1400111c0..140011222 setup prefix through18790 call|`b947230f308dd6469164d55d088de3da9279e067d775d8ad2142b20de58b4535`|
|140018790..140018ad5 stack producer|`63ea4c8830856bc72ab360c0d1d83d0befd96dc85beca5041c427c94121571d6`|
|65405990..65405aae QStackedWidget constructor|`8f7bcb16871d871e6ec62103ec5458a8bc0a51957e7318dda88a2ebdbad523f7`|
|65405af0..65405b00 currentIndex wrapper|`b2bbcd8ac2e54017789440e895172cccc119cb6b296e59a85ef3e1505ba2e3df`|
|650501e0..650501eb currentIndex getter|`65f5763c17c8c47b38ad4d608e06a7521f9dd999d85a85f7176a0d35d62ab640`|
|65050740..6505079e layout private constructor|`ac7bc5dc837e53c6a2fdad92e3a3dc214cbcc7a77108010d3184cf335505f537`|
|65050810..6505087e layout constructor|`62d817b14ff99db6c2e75e0ef126255f2e0aa4290c35dce60e50468cefbd429f`|
|65050d80..65050e62 insertion body|`0b049ec24f0613f1ec3eec18865cb759e1db2086d4ccfd5efa3bc99a6294c9ef`|

Retained qualifications: resume77-boolean-mainwindow-qt-lineage.md; resume47-doe-preferences-test-design.md; resume101-close-live-owner-chain.md and its bounded writer inventory; resume106-close-producer-minimal-evidence.md; resume107-app-main-lifetime-call-boundary.md (`38fb86b21db7230f37a8b29eb7dc3065e2ae5eb4996efa38668fdd0450d7090c`); resume110-close-after-save-failure.md (`818298bbcb5f73fe11d1c98fd2c9754ea5d79ef6f99d61394ee8137e7a57aa0c`). No new runtime Evidence IDs or candidate freeze is claimed.
