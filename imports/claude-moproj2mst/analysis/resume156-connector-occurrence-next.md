# CP156 DBLIST occurrence identity frontier

## Finding

The selector and constructor path are closed by CP152: factory `0x140137ca0` compares the exact `SfTaskReportGenerate` literal, calls constructor `0x14079f1e0` on match, then passes that pointer to `0x1407c8140`. The constructor writes the known `SfTaskReportGenerate` primary address point `0x14269e558`.

The new bounded follow-through confirms that factory insertion uses a TaskInfo-owned QString, but retained bytes do not reveal that field’s concrete value for this task instance. Nor is there a same-occurrence pointer/vptr capture for the later `slotGetTaskBase(QString)` call. The `+0x1c` admission field also remains independently unresolved. Keep public dispatch guarded.

## TaskInfo name source and exact limit

Original target qualification remains `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

The retained helper captures identify the relevant source of the insertion key:

- `analysis/wave2-doe-task-prototype-taskinfo-getter.asm` (SHA-256 `E23C73E0AD73D6A57E7A629BA87ACECD715B9AB8B2AD8A769B82DD5FE6B0F9CF`): `0x141403790` returns `task+0x20`, the TaskInfo object.
- `analysis/wave2-doe-task-prototype-taskinfo-name.asm` (SHA-256 `85F0D0060BBD7E230AA0EE179E8BCE5E5A085D70385D19C42C85E04466422C78`): `0x14140e5d0` copies the QString at `TaskInfo.impl+0x10` into its output. It reads through the `TaskInfo+8` implementation pointer. This is the name string consumed by `0x1407c8140` when it indexes both maps.
- `analysis/wave2-doe-session-task-payload-entry-decompile.json` (SHA-256 `ED5205A9142040BB4CACB28D68B6351175F3F0F1A300BBCA30408FE9C99ECEF5`): `0x1407c8140` obtains the TaskInfo pointer, sets its factory backlink, calls `0x14140e5d0`, inserts the TaskInfo in map `+8` under that name, then calls the same accessor and inserts the raw task pointer in map `+0x10` under that name.
- `analysis/wave2-doe-task-prototype-taskinfo-ctor.asm` (SHA-256 `7F395FCE24A655724492B720CEFD36664223C47A84F99B208456E446D40A1ADA`) and `analysis/wave2-doe-task-prototype-taskinfo-private-constructor.asm` (SHA-256 `172E1DAAF0E6D57FDE2153EB64694DA2486BC4500D2D840B5F064000B83D980A`) show TaskInfo's generic implementation/string storage construction. They do not establish the concrete QString value for the ReportGenerate object.
- The ReportGenerate constructor (`analysis/wave2-doe-task-prototype-ctor-14079f1e0.asm`, SHA-256 `0072B6C52A9C19DD54714036FDFCEABC7F034A50D2A8DF23C4DA53590225DB4B`) installs its class address point and invokes shared TaskBase initialization. It does not directly store the selector string in the inspected constructor body.

So the exact native insertion key is the TaskInfo name field, not the branch argument copied directly into map insertion. The create routine immediately calls registration for the matched ReportGenerate constructor, while its caller later looks up the originally requested key. That is strong control-flow evidence of the intended correspondence, but the retained static slice does not independently compare the TaskInfo string at `impl+0x10` against `SfTaskReportGenerate` or preserve the resulting pointer. The initializer also registers a prototype sequence through this same insertion helper, so a static constructor list alone does not identify which pointer the occurrence returns.

## Returned pointer and occurrence binding

CP149 resolves the DBLIST generic method invocation to MainWindow `slotGetTaskBase(QString)` with selector `SfTaskReportGenerate` and result type `SfTaskBase*`. The getter is lookup-only in the lazy MainWindow factory’s raw-task map `+0x10` (`analysis/wave2-doe-live-task-registry-ownership-contract.json`, SHA-256 `23494CB67A8A58FFF7F70ECACBCD6BE6FDE4C1ABFBD80F1E434D6FEDA9913E2A`; `analysis/resume65-gettaskbase-dispatch.md`, SHA-256 `E88FE6C24238B0B3E04EB0B5611B9C8C6B776C6BB0CCE73000AE1093C3FDE495`). It returns the stored map pointer without constructing or checking its vptr.

The bounded static sequence establishes a constructor of the expected type and a path that registers it under a TaskInfo-derived name. It still lacks (a) the concrete TaskInfo name value, and (b) a DBLIST-occurrence observation of the getter’s returned address and `[address]` vptr. No immutable static disassembly can provide those occurrence values. A runtime trace would be needed for strict same-occurrence binding if the metadata value cannot be derived from the class-specific virtual metadata providers.

## Separate AppState gate

DBLIST admission is a prior check of AppState singleton `0x143d766b8+0x1c`; it occurs before the ReportGenerate dispatch. `analysis/resume108-dblist-admission-writer.md` records that constructor initialization does not establish a `+0x1c` reset and the setter `0x1402cbf30` has no retained caller proving its receiver is the singleton. This identity investigation does not change that result. Even closing the ReportGenerate TaskInfo string and returned pointer would not admit public DBLIST while the gate value/lifecycle is unknown.

## Scope and next witness

This was an immutable original-PE/retained-capture audit only. No GUI, original runtime, mutable REA database, candidate source, or build was used.

The smallest next static witness is the class-specific metadata provider path used while `0x14079f1e0` initializes TaskInfo, followed to the actual QString written at `TaskInfo.impl+0x10`. If no static provider exposes that value, the decisive occurrence capture must include the returned TaskInfo name, factory map value for that key, getter return pointer and its first qword vptr. The separate `+0x1c` writer/caller/reset remains its own required witness; no public dispatch is cleared here.
