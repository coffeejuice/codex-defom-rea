# CP107: application/main lifetime and the actual FileCloseAll caller

The concrete project-Close caller is **SfMoMainWindow metadata method20, slotActionFileCloseAll() ->031f10 ->0299a0**, on the actual primary main produced by startup. Its global state and stack application/main owners have distinct lifetimes. Current run has no equivalent startup producer, so a mutex, per-call Session or new owner allocation cannot authorize this call.

Read-only retained PE/source and bounded offline disassembly/import/RTTI/metadata reads. Only this report was written for CP107. No code/build, GUI, original execution, mutable REA database or REA retry. Immutable DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. No reconstructed executable/deployment was inspected or changed.

## Three real owners and their source boundaries

| Owner | Construction / lifetime boundary |
| --- | --- |
| Application-state singleton143d766b8 | C++ initializer-table slot141e0c950/index31 ->141ca0bb0 ->2c7ee0/2c7f80, before main1410. After construction141ca0bc0 supplies thunk141df4700 to helper146ed38 at141ca0bc7. This is separate from either stack object below. |
| SfApplication stack object | main1410 passes rsp+520 to21c0 at147e, with mutable argc/argv.21f1 constructs the QApplication base, then derived table1420edeb8 is installed and application helpers/handler setup run. On the selected regular return path, after the main window is destroyed,2036 calls2250 on that same rsp+520 object.2250 runs2420 and calls imported QApplication::~QApplication at2281. |
| SfMoMainWindow stack object | main1b39 passes rsp+40 to10490.104d2 enters basec98b00, which callsc99530 atc98d23. c9957b publishes the same this into143d766b8 through2c8070; **c99588 only then initializes main+50=null**. After base return,104dd..104e4 installs derived SfMoMainWindow table1420fb108 and later106d0/111c0 performs derived setup. |

On this constructor path, publication is not main readiness. Atc9957b the object is in base initialization; the eventual derived+300 busy target01daf0 and its+480/+270 fields must not be assumed ready because the address is already canonical/published. A later metacall can also reachc99530; that occurrence must not inherit the constructor cursor automatically. SfApplication likewise installs its derived table only after its QApplication base returns. A faithful constructor cursor needs those dynamic-type phases, not just alive/dead or a main-present Boolean.

On the normal application-frame path, QApplication::exec is called at1fb6. After return and intervening cleanup,1ffc invokes105e0 on the same rsp+40 main;106b3 calls base destructorc991e0. That base frees+118, clears singleton main through2c8070(null) atc99245, then continuesc99890/ca72b0/member/base teardown. The later SfApplication destructor is not Project Close.

The global thunk141df4700 calls2c7f40 on143d766b8.2c7f40 calls2c8060 (decoded save-argument/return leaf, no field mutation), then QString destructor on global+20 at2c7f66. Its direct body does not clear main, delete ModelManager or reset busy fields.146ed38 delegates146ec30 and normalizes nonnull result to0/null to-1; the initializer ignores that result. Full registration-success/CRT-exit traversal was not expanded here, so callback execution on every exit path is not claimed. These constructor/registration/thunk identities still establish a global-lifetime object, not per-Project storage.

## Exact project-Close call boundary

The SfMoMainWindow string table is VA14216f2d0/raw216dad0; metadata is VA14216e830/raw216d030, method count0x85 and records start at DWORD offset0x0e. **Record20** is raw216d1f8, five DWORDs:

```text
0x2d2, 0x0f, 0x0f, 0x0f, 0x0a
```

The first field points to **slotActionFileCloseAll()** at string-relative2d2. The associated static metacall pointer selects0db1a0. For call-kind0 and local method20, jump-table DWORD at0dc950 (rawdbd50) is0x000db5fa. The selected body:

```text
0db5fa: reload the original incoming receiver from saved local
0db5ff: call031f10 on that receiver
0db604: branch to common metacall epilogue
031f1e: call0299a0 on that same saved/reloaded receiver
```

The metacall's earlier04b80 helper is a single RET; it contributes no hidden owner/effect. The method20 case performs no result test. Do not invent a public cancellation/status mapping from that fact.0299a0 itself retains its busy/question/accepted-cleanup return contract.

A whole-.text raw E8/E9 scan found another direct0299a0 caller at010b63, within010b10. Selected main vtable1420fb108+2d8 points to010b10. That wrapper first executes virtual+778, may copy its result+210 to global143d77553, then executes virtual+2d0, calls0299a0 and explicitly normalizes its returned Boolean. This is an **additional pre-close route with preceding effects**, not the same FileCloseAll slot. No broader shutdown classification or skipped precursor effects are inferred solely from the address.

The retained FileCloseAll/0299a0 direct bodies return without directly destroying main/SfApplication. Main/global teardown remains at its later application lifetime boundary. Synchronous recipients/reentry can still have effects and remain separately guarded; the direct call sequence is not an observed accepted Close.

## Qt startup effects that the candidate must actually produce

CP106 locates application entry1410 before pre-application7fc450 and SfApplication21c0. Its first base producer is21f1 ->QtGui **65028aa0**, the imported QApplication(int&,char**,int) constructor with literal01040804. Its bounded body:

1. Requests **0x1d0** private storage at65028ad3.
2. For a nonnull result, calls private constructor6501c760 at65028af5 with argc/argv, literal1 and the saved flags; null takes its explicit branch.
3. Calls protected **QCoreApplication(QCoreApplicationPrivate&)** at65028b04 through IAT65696e30, on the unchanged primary this. The import name was independently resolved, not guessed from the callsite.
4. Installs QApplication table6569f3b8, reads its private+8 and calls65028860 before return65028b36.

Actual current-QCoreApplication identity/private/thread/event/widget/platform initialization belongs to these providers, not a Session backlink. Their full effects are not closed by this bounded constructor shell. SfApplication then calls7ffc10, installs message handler1020, and calls24f0/2290 before return. Main construction additionally needs QMainWindow/QWidget/QObject/private, global context, toolbar/setup/registration and concrete main+50 factory effects already qualified as gates in CP77/106.

There is also a distinct known startup event: main1410 requests **QTimer::singleShot(1000, &main, "1slotRelaxDockSizeLimit()")** at1faf before QApplication::exec. IAT141e04478 resolves to the QObject receiver overload of singleShot. This is **not** an automatic Open/Save/Close request. Its actual due/delivery state and callback effects were not observed here. Current DiagramLifecycleEvents models diagram modelUpdated timers only; its pending vector is not evidence of a complete application-wide due set. An empty diagram schedule cannot certify zero startup-main delivery during later modal/event intervals.

## Source-backed candidate slice and exact blocker

The bounded future integration slice is:

```text
real reconstructed startup1410/pre-app/SfApplication producer
 ->same canonical primary main10490, publication and derived/setup/factory return
 ->one-project Open and explicit Save on that main/manager
 ->headless command selection corresponding to metadata20/031f10
 ->0299a0 busy/can-close/question/nested-Save prefix on that SAME owner
```

The headless command adapter may choose the recovered FileCloseAll entry directly once the actual main exists; selecting that command does not require synthesizing a native QAction or claiming a QMetaObject event was observed. It must preserve internal pumps, modal effects, source-read timing and owner identities. It cannot bypass startup by passing ConversionSession as this or by allocating an ApplicationState and declaring it ready.

Current core/run has per-call Session/manager/MainView, a startup-unit override and serialized engine access, but no SfApplication/QApplication producer, actual primary-main constructor cursor or normal-Close callsite. Existing canonical primary aliases explicitly refuse Qt base/widget effects. Thus **no presently executable public Close slice is source-qualified**. The first candidate gate remains application startup before7fc450, or21c0 only with its actual pre-application continuation. Within21c0 the first unavailable Qt call is21f1. This result agrees with the independent CP104 review; no invocation-local inert owner is proposed.

A fresh-app/one-project candidate profile becomes justified only when a real reconstructed runner executes the relevant startup providers, carries the same actual app/main/manager through its command occurrences, and accounts for delivered state writers. Source identity and initializer literals alone do not determine current main+480/+270, Close question due events or nested Save results. Static recovery can implement deterministic constructor/writer contracts; runtime-sensitive schedules need a qualified same-instance trace or fully justified input profile. No such new witness was acquired in this lane.

Focused integration checks after that producer exists: publication during base phase cannot admit derived busy dispatch; command20 uses the exact produced main/manager; Project FileCloseAll retains application/global/main/receiver lifetimes and pending startup events; global clear occurs only at the source main-teardown boundary; startup QString/global destructor is not replayed per Project; Close refuses missing main/Qt/modal state before the next unavailable read/effect, retaining already committed writes. A test calling another detached helper would not exercise this slice.

## Source/byte receipts

DUMPBIN14.44.35228.0, exact end-exclusive source-byte spans; these are ordinary static-tool evidence, no new REA Evidence IDs.

| Span | SHA-256 |
| --- | --- |
|141ca0bb0..141ca0bd1 constructor/registration thunk | `efdec6f9e0e77365cb0f53c675c7dd281a4c6c87840230aa4350166d25fed9c8` |
|141df4700..141df4715 global destructor callback | `db8f2a170579772da9043a2ee53b659ae9037949a996c03c5a4adf61cb64f484` |
|1402c7f40..1402c7f71 global destructor body | `875ccd10dec061b0e9ec78f51195bd1af11c4a5f6cd9b887e73031a87681d04f` |
|14146ed38..14146ed4f registration wrapper | `5013347d962aa00152f0423468700f8aef2f587e3ae9087a713bc9150cfdf345` |
|1400db5fa..1400db609 method20 body | `c293b36f9578bf82064afbdb0c56400ecf93fa63431cc50237d013e38119976a` |
|1400dc950..1400dc954 method20 table entry | `af0bdf93d72ac0c877805fe3105d5fd5b064b7162c68ce3b5204f835a5cb47eb` |
|140031f10..140031f28 Close wrapper | `04dd3035662edf8caff271e6a476b625294ce8e76f63c09b4eb43edabbaee3d7` |
|140010b10..140010b7a additional caller | `950ffc2095952a9d14cfaaca0f0e4e41509fe3bb9f4c992a4aadbd9600551480` |
|QtGui65028aa0..65028b37 QApplication shell | `bf170153a0508bb3547067a09fdd2357f177a10951d59df5640e0b3a3733c55e` |

Retained source hashes: CP106 report `f69a3499b8d456c8be2dc6e48f80120e7be82e0c9d7708c1907a7acf0e43fed2`; resume65-gettaskbase-meta-receipt.txt `a03b7a2bac232a454491811e5048fb94fe2abcf3943d464f76c9d42625219942`; wave3-editor-owned-open-entry-140001410.asm `a18e1a6b48e042f2795bf9536d7bcc299d4e9ba8532a9b189912a719a161778e`. Current candidate audit hashes: core/convert.cpp `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632`; engine/conversion_session.cpp `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`; engine/lifecycle_events.hpp `61c574811261d54da39c989f4326ca82c17613564e48f1aee2f75096c373fe09` (prefix source/apps/moproj2mst/src/ for cpp, include/m2m/ for the engine header). Source hashes are read-time qualifications, not a claimed freeze of other lanes.
