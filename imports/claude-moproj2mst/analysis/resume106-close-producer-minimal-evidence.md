# CP106: the real fresh-application producer gate

The original fresh-application producer is **application entry140001410, reached from executable CRT startup**, followed by actual `SfApplication` constructor1400021c0 and primary main-window constructor140010490. A one-project fresh-app profile is a valid reconstruction target, but current candidate run/Session construction does not implement or reach that producer. No invocation-local ApplicationState insertion or constructor-literal reset is recommended from this evidence.

Only this report was written in the CP106 lane. Retained sources and bounded offline PE/import/export/RTTI/DUMPBIN reads only; no mutable REA database, GUI, original execution, source edit or build. Immutable DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. No reconstructed executable/deployment was inspected or changed.

## Actual producer/caller chain

1. PE AddressOfEntryPoint is141470dc0. Its startup stub reaches CRT141470c28. At1470d53 the CRT calls146e780; its successful initialization path precedes1470d7e's call to application1410 with CRT argc/argv.
2.146e780 first has earlier initialization/error gates, then iterates qword range **141e0c858..141e14d88**, end exclusive. At146e7e6 it loads each pointer, skips zero and invokes it at146e7ee. The range has4262 slots, not4262 proven nonnull constructors. Slot **141e0c950**, index31, contains **141ca0bb0**. This gives the actual indirect caller of CP101's global initializer:141ca0bb0 ->2c7ee0 ->2c7f80 on singleton143d766b8. A direct E8 scan alone cannot find this table invocation.
3.Application1410 calls pre-application helper **7fc450 at1461**, passing argc and argv[0]. Retained bounded evidence shows executable-directory/environment processing; its nested effects are not closed as neutral. This occurs **before** the Qt application object exists.
4.At147e,1410 constructs a stack object at **rsp+520** through **21c0**. This constructor installs table1420edeb8; its COL1430d3f70 resolves to **`.?AVSfApplication@@`** using the correct PE section mapping.
5.SfApplication first passes its same this, mutable argc reference, argv and literal **0x01040804** to IAT141e0bf90 at **21f1**. The exact import is `QApplication(int&,char**,int)`; its immutable QtGui export is **65028aa0**. The flag literal is not evidence of completed initialization.
6.Only after that base returns does21c0 install its table, call **7ffc10(143d78670,0)** at2210, install message handler1020 through **qInstallMsgHandler** at221c, call **24f0** at2227 and **2290** at2231, then return at2240. These application/provider/handler effects cannot be replaced by an AppState value constructor.
7.Later1410 constructs the distinct **SfMoMainWindow** stack object at **rsp+40**, calling10490 at1b39. Its table1420fb108/COL1430d46d8 has RTTI`.?AVSfMoMainWindow@@`. Retained10490 ->c98b00 ->c99530 preserves the primary this; c9957b publishes that same main through2c8070(143d766b8,this), before main+50 initialization/factory. The application object and the application-state singleton are distinct owners.

The selected application frame retains the main through QApplication::exec at1fb6; its regular return path destroys main105e0 at1ffc and SfApplication2250 at2036. CP107 separately follows the registered global destructor and Close-call boundary. This static lifetime is conditional on reaching the regular path; it is not an observed accepted Close occurrence.

## First unavailable candidate gate

Current core/convert.cpp::run serializes engine work and constructs per-call ConversionSession. `GuiStartupUnitScope` only sets/restores the separate GUI unit global143d79724; it constructs no QApplication, canonical primary main, current-main pointer or native event owner. Session's MainView/ModelManager are adapter objects; WidgetAreaPrimaryWindow is explicitly an identity projection whose Qt-base/widget requirements refuse. No current production constructor cursor reaches1461,147e/21f1, or the actual10490/c9957b publication sequence.

Thus the first source-backed new application-start entry is **1410 before7fc450**, or **21c0 with a genuinely produced pre-application continuation**. Within21c0 the first unavailable Qt producer is **21f1 ->65028aa0**. The Qt constructor requests0x1d0 private storage, constructs it through6501c760, invokes its QCore base through IAT65696e30 and runs65028860 before returning. Locating these calls does not execute or qualify their thread/application/widget/platform effects. Later SfApplication helpers and main constructor/setup/factory remain additional gates.

The current C ABI's selected project path/settings are not native argc/argv, executable-directory/environment setup or Qt application state. Its mutex does not create a fresh app instance, and a call's scope is not the original CRT/application/main lifetime. The user's selected fresh-app/one-project scope determines what must be reconstructed; it does not prove that current run already represents it. A process-static owner would likewise need an actual constructor and app-instance lifecycle.

A source-qualified candidate must introduce a **real reconstructed startup runner** that produces these application/main contexts and passes the same actual main/manager through Open, explicit Save and Close, or consume a qualified same-instance startup/command trace at an explicitly bounded replay seam. It may model the effects headlessly; this does not require executing the original or launching a GUI. It may not skip unknown predecessors using a `fresh=true`/`constructorComplete=true` label. No such runner or trace is present now.

## Exact consequence for normal Close

The native Open/Save/Close singleton chain from CP101 remains valid. It proves where the live fields belong, not that current candidate defaults are their live values. Reaching Close still requires current+9/current-main and the actual main+480/+270 predicate; successful moProject/process5 additionally requires default-question dispatch, nested Save and callbacks. Initializer-only main=null or busy=0 cannot satisfy those reads after the missing main/application phases.

The productive next source/evidence target is the **21c0/QApplication startup producer and its actual pre-application caller continuation**, followed by10490 main setup/publication—not an inert owner in run. No safe public normal-Close callsite slice is established until that producer reaches the existing Open/Save path on the same owner.

## Byte and source qualifications

DUMPBIN14.44.35228.0; spans below are end exclusive. Table and RTTI facts were read directly with section-aware PE mapping; no new REA Evidence ID is claimed.

| Span | SHA-256 |
| --- | --- |
|141470dc0..141470dd2 startup stub | `42ae9783bb15e68c9ef167a8f602401116289a05b5138fb3183e7cbff6bb9a79` |
|141470c28..141470dbf CRT caller | `95aaa24056eed3900632efa69a02f76b4e0eb4ce381ed9eef16a5538bbb5f9f9` |
|14146e780..14146e82f initialization walker | `65f36312076717709e4c8b2cde94dd435f3f0952a413a6b8a108054c59f02f03` |
|141e0c858..141e14d88 qword table bytes | `83cf66852ded2c930ee10d1ab8ac3e952ec8b5d090ba9e769435d7efd707dd20` |
|1400021c0..140002241 SfApplication constructor | `d6cb9fc2436200145231c4162d6e7abb547cf53df0c907baee4c88d461598765` |

Retained source qualifications: `resume83-doe-pre-application-static.json` SHA-256 `4f39168f7678dc9b82705091356de884e56b700691b31b254a6303b67696bf39`; `resume77-boolean-mainwindow-qt-lineage.md` `c618e26719aa38bcb4871dc85c42c6654dce7fcc8093af4e4f25647ae9da13c1`; `resume104-application-owner-integration-review.md` `1351664e8dba14e984a7964a74b70241bc5156aa50a565c549049ab632f2bb3c`; core/convert.cpp `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30`; model/units.cpp `4a55b37b657545a00d6dab1cd639efc56a57a5dfc5b219e1906ad0e133820059` (source prefix `source/apps/moproj2mst/src/`).
