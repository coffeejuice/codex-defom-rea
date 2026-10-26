# CP117: canonical main construction, phase-specific busy, and the separate W writer

Canonical main M has distinct primary QObject/QWidget, QPaintDevice+10 and **ATL event-proxy+28** subobjects. The initial publication of M as application current-main W occurs during base construction, when virtual+300 is a constant-false base leaf. The derived constructor later replaces that slot with01daf0 before creating+480. Neither a c99530 call nor a retained manager G therefore certifies the dynamic phase or later W. The candidate still lacks the real application/main constructor bridge; no source was changed.

Only this report was written. Bounded retained/local static source, PE/DUMPBIN, RTTI hierarchy/COL and jump-table reads; no GUI, original execution, mutable REA database/import/retry, source/interface/plan edit, build or test. Immutable DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed. Application function/site addresses are RVAs relative to140000000; tables/globals use full VAs.

## Exact canonical M construction and G publication

CP107 establishes main1410's stack object at rsp+40 ->10490; its basec98b00 receives the same primary this. The newly reconciled subobject phases are:

1. c98b4f constructs QMainWindow on primary M. c98b5e passes **M+28** tocbc7a0, the secondary eventing-base constructor. That constructor callscbcBF0, installs its base table and initializes two secondary fields; this is separate from QObject private construction.
2. c98b77 installs primary SfMainWindowBase table14290ac88; c98b89 installs its QPaintDevice+10 table14290ac50; c98b9c installs event-proxy+28 table14290ac40.
3. Laterc98d23 callsc99530 on primary M. It first checks QCoreApplication::instance and conditionally callsc993d0(M,true). Only afterwardc9957b calls2c8070(global143d766b8,M), publishing **W=M**, followed byc99588's M+50=null. Those predecessors remain guarded; no null Qt application or neutral callback assumption is supplied.
4. While that base table is active, table14290ac88+300 resolves **ca44f0**. Its complete body is saved-this, xor al,al, ret: a source-closed constant-false busy leaf. This is a dynamic **base-construction phase** result, not a default value for arbitrary M.
5. After base return,104e4 installs primary derived SfMoMainWindow table1420fb108,104f3 installs its QPaintDevice table1420fb0d0, and10503 installs proxy table1420fb0c0 atM+28. Derived table+300 resolves **01daf0**, which reads actual QStackedWidget+480 currentIndex and, on non2, main+270==1.
6.10490 then calls106d0 at1058e and111c0 at10598. As CP114 closes,111d8 ->c999e0 ->c999f6 virtual+2e0 ->0133f0 constructs G with G+10=M, performs the fresh-current equality-skip after base/list/guard construction, installs concrete manager table142115f68, then stores **M+50=G at01344c**. This precedes1121d ->18790's later construction/publication of main+480.

Thus W can name M before G exists and before derived main/stack readiness. A constructor cursor may model the base+300 false result only at that actual phase; it must stop before unrepresented later effects. It may not publish a late fixed false into Close or treat a derived main whose stack producer is unavailable as that base phase. No evidence of an actual early reentrant busy read is claimed.

## Correction: the second c99530 caller is an ATL proxy dispatcher

Older CP98/107 prose called the large01e910 body a “metacall.” **That shorthand is not source-qualified as Qt metacall and is narrowed here.** This does not alter CP107's separately proven genuine Qt FileCloseAll metadata20 ->0db1a0 ->031f10.

Direct table/COL/hierarchy reads show:

- Derived secondary table1420fb0c0 and base secondary14290ac40 both have first target **01e8c0**. Their COL subobject offset is0x28, with complete types SfMoMainWindow and SfMainWindowBase respectively.
- Both RTTI hierarchies identify bases at primary offset0x28 as **ATL::__ComEventingImpl<SfMainWindowBase>** and **ATL::__EventHandlerProxy**. This is not the primary QObject Qt metacall slot.
- 01e8c0 receives that proxy address, subtracts0x28 at01e8e6 to recover primary M, prepares argument storage and calls01e910 at01e8f3.
- 01e910 adds1 to the incoming selector and jumps through the DWORD RVA table at0245cc. Retained v6.c explicitly switches on `param_2 + 1`. Consequently its **case0x45 means incoming selector0x44**, not selector69. Table index0x45 points01fc13, which reloads primary M and callsc99530 at01fc1b. Incoming selector0x45 instead selects table index0x46 ->01fc2c ->c99890 at01fc34.

The proxy entry/type/index branches are now static source facts. **The registration/event source that supplies these selectors, and any actual invocation in the selected fresh one-project run, remain unknown.** Do not label either branch a runtime occurrence, startup event or modal due-event without that witness.

If selector0x44 is reached, c99530 republishes its actual receiver M as W and then writes M+50=null. It does not reinstall a base vtable and contains no manager deletion before that store. An already-derived receiver therefore remains derived; the call target alone cannot certify base-phase busyfalse, a fresh manager or native constructor completion. Do not add old-manager reuse/deletion to “repair” this decoded branch.

If selector0x45 is reached, its directc99890 cleanup includes manager deletion/nulling as recovered in CP114, but it does **not** carry the earlierc991e0 global-main clear. Conversely, the regular base destructorc991e0 calls2c8070(null) atc99245 beforec99890. That setter is a raw qword write, with no W==M conditional. A candidate must not move global clearing into every cleanup call or add a compare-before-clear policy.

## Candidate bridge and exact prerequisite

CP116's identities remain distinct: G+10 and its child connection receivers name constructor M; AppState.main W is mutable and is freshly consulted for busy, dialog parents and accepted callbacks. Normal Close's incoming command receiver M supplies M+50 for can-close and later typed manager lookup. These relationships must not be interned just because the selected initial construction writes W=M.

Current ConversionSession creates G with an adapter Session backlink and uses that same actual manager for Open, preserving real candidate child/connection identity. It has no canonical native M constructor, ATL/QObject receiver graph or live AppState.main producer. WidgetAreaPrimaryWindow recognizes primary Qt roles/QPaintDevice+10 only and explicitly refuses native base/widget effects; its identity projection is not this constructed main. Extending an enum with the now-known proxy+28 would establish another alias at most, not proxy dispatch, Qt startup, G ownership or W freshness.

The next meaningful integration requires root-owned shared construction, after CP107's actual pre-app7fc450/SfApplication21c0/QApplication21f1 gates: retain primary M with phase-specific base/derived tables; publish W atc9957b; initialize M+50; construct/bind the one actual G at111d8/0133f0; and later produce the stack before deriving a ready Mo busy result. Keep native main backlink separate from adapter Session ownership. Later W updates and any proxy callbacks need their own reached-source cursor; they must not replay constructor effects on Session entry/exit.

The exact additional evidence gap is now the **ATL event registration/selector caller**, alongside the still-unimplemented real Qt/main producer. There is no new independent source increment authorized by this report. CP110's Save-failure behavior and fresh FileCloseAll admission remain unchanged: writerfalse cannot supply current W, main readiness, modal schedule, nested Save or accepted cleanup.

Future production tests should distinguish initial base publication from later derived publication/stack creation; verify M+28 proxy canonicalization without treating it as QObject; guard unknown selector registration; and separately test declared initialization/cleanup occurrences without collapsing their different W/main+50 effects. They must use the actual production constructor owner to establish lifecycle progress. No test was run here.

## Compact evidence receipt

DUMPBIN14.44.35228.0; section-aware PE mapping and RTTI hierarchy PMDs; spans end-exclusive:

| Span | SHA-256 |
| --- | --- |
|1400104dd..140010507 derived three-table installation|`5dbabdc3ff1c9784f054662b489225ea9cc391e30532675b1fd57bb920fbebd4`|
|14001e8c0..14001e90e proxy canonicalization wrapper|`08a503dc13d22c513fa138306d7f124a3d9c67cea798cd1e42d4cfcd03ea6cd6`|
|14001e910..14001e97e dispatcher selector prefix|`5a212b5ecc8be1768bc9cf19ab88ae81e5d63d492a320f5e3dd65bd3bff94c85`|
|14001fc13..14001fc40 selected two branches|`f1d3bbfbc88faab4900aa14ede203a9d7d2d69f128974e6b0e6f5786429b3ef8`|
|140c99530..140c99590 publication/manager-null body|`d0cec6effeaeeed96144f74bfb264211ff8f617d65759d87afe628854e854d8c`|
|1402c8070..1402c8088 raw main setter|`dc6f5af7ad035e82f8c3c8397d73b6c187ec5a8d949b7a76ef4a31ef4407ca56`|
|140ca44f0..140ca44f8 base busy leaf|`bda5135adee0e1dc7d938d8d839bae743ef85fbccb23cbad3f3c831b1809efee`|

Retained v6.c SHA-256 `cef772daeb877e3dc2b0a76c1c98de8261e337a7e3501e9640636f99aa9ce9aa`; CP116 report `78cf09f33a4e35a38ca0ba49e01e687fd18148a681dd8a441dc50f4a080c5371`. Read-only current source identities: widget_area_primary_window.hpp `0262846de326e9d401850d59424dfc225ee0dcf2e8aadaa71ad4f32a07a0d422`; model_manager.cpp `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4` (under source/apps/moproj2mst). Retained CP107/110/112/114 qualifications remain attached. No runtime Evidence IDs or candidate freeze is claimed.
