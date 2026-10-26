# CP104: exact run/Session insertion and startup producer boundary

The safe immediate integration is one **inert, value-owned ApplicationState in run**, borrowed by the actual Session and retained across Project teardown. It starts with unavailable native facts and leaves public outcomes unchanged. Source-closed startup/busy stores have precise later callsites, but cannot be spliced into prepareProjectModel or exception cleanup. A fuller fresh-start bootstrap is statically recoverable if its actual QString shared-null acquire is included; it still supplies no completed main-window constructor or late current-main value.

This is read-only design. Only this report was written; no REA retry/session, GUI, original execution, production edit or build occurred. Source qualifications: CP103 design SHA-256 `75decaa6e8004e99031830ac3a1b8818100ae7c033887ad77023da9b2317c3a7`; CP101 chain `8d893447af81cedd885b112609f7a7de709c1e2ab9b9f54a456746c4e2c7f793` and PE receipt `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable/deployment was inspected or changed.

## Immediate ownership integration

Line numbers below describe the source hash pinned at the end of this report; use the named statements if other lanes move them.

| Current source location | Safe insertion/change |
| --- | --- |
| core/convert.cpp::run, after engineLock at160, before try at172 | Declare a noncopyable/nonmovable **value** ApplicationState, default constructor noexcept/Unknown/no native effects. This declaration outlives every Session constructed inside try and remains present through catch/details handling. No new heap allocation, native zero reset, current-main publication or public error is introduced here. |
| core/convert.cpp:197, `ConversionSession session(diagramEffects)` | Use an internal `ConversionSession(driver, applicationState)` overload. It borrows this same actual owner. No second manager, notifier or application owner is created. |
| conversion_session.hpp, immediately after driver_ and before existing owner members | Add a nonowning application-state link, declared early enough to exist before modelManager_ construction. Forward declaration suffices; no model/header inclusion cycle is needed. The explicit lifetime contract matches the existing borrowed DiagramEffectDriver contract. |
| conversion_session.cpp:146 constructor | Preserve `Session(driver)` as an unbound diagnostic/legacy constructor. Delegate both forms through one internal implementation; the borrowed overload records only the actual C++ owner relationship. It must not allocate a fallback owner, mark native startup/main ready or execute native stores during ordinary child construction. |
| conversion_session.cpp:429 releaseProjectStorage | Preserve the application link/storage. Project reset, moProject reset and diagram storage cleanup do not recreate, clear or reinitialize application state. |
| conversion_session.cpp:207 destructor | Invalidate only any C++ admission metadata as appropriate. Do not synthesize native current-main=null, false busy, counter reset or original application teardown. The run-owned value leaves scope later. |

Use an internal const snapshot/opaque identity comparison for tests; no escaped raw AppState/main pointer getter or mutable public setters. If diagnostic progress needs stale-address protection, acquire a C++ scope identity lazily at explicit progress creation, rather than adding an allocation to every default construction. Scope expiry is unavailable authority, not a native null/clear event.

The default `Session(driver)` has no application owner and must not self-manufacture one when a production Close read is requested. An internal source command should report that missing binding. Existing isolated Close tests can retain their declared observation APIs; they are not promoted into the production owner.

This increment is necessary ownership plumbing, **not a Close behavior milestone**. It is safe only if it does not add a normal-Close callsite or consume the unknown state to select a native branch. It also avoids broadening current early argument/suffix handling with an allocation/failure point.

## Startup writes: exact predecessor and a bounded next option

CP101 already closes2c7f80's first six effects: process+0xc=0, current-main+0=null, byte+18=0, busy+9=0, A=1, B=1. But original2c7ee0 constructs members before entering that initializer. Newly bounded PE reads distinguish those predecessors:

- 546c70 writes DWORD0 and returns; it initializes+10 then+14.
- QString's imported default constructor is QtCore67003030..67003046: store pointer672f0b30 at the actual QString field, **lock add1 to that same shared-null resource at6700303a**, then return. Original field is application+20. Current qt::String() models null text only; it does not perform this source resource acquire.
- 004a60 is a plain DWORD argument store;318d0 ORs a source DWORD with the supplied value and constructs the result through004a60;31680 composes those calls;d64030 copies one DWORD. Thus the remaining initializer flags produce+10=0x3003,+14=0x1003, followed by the observed byte/DWORD resets. These helpers have no allocator, event loop or callback.

Consequently this is a **static producer dependency**, not an intrinsic need for a runtime sample of each startup store. Two honest implementation scopes are available:

1. **Minimal owner only:** keep all native fields unavailable. Do not call the six-store initializer from the C++ AppState constructor; its source member-prefix entry is not yet produced.
2. **Actual source bootstrap:** include a source-owned shared-null resource/field lease in the fresh-application runtime, execute the exact member acquisition and initializer on that actual ApplicationState, and retain the reached constructor cursor. The nonbranching acquire can preserve an unknown base refcount plus the one performed acquire; it must not invent a known total, use shared_ptr's use_count, duplicate another pool or imply all Qt reference writers are recovered. It does not require constructing a main window. If that resource producer is excluded, the exact guard is before6700303a/member-prefix completion, before the six initializer writes.

For the second scope, the application-start coordinator belongs before main construction and before the current GuiStartupUnitScope at173; source layout may require a reserved run refactor. Do not add it late at Close, inside Project construction or as an implicit retry after Save. A value owner's constructor must not silently claim this source bootstrap if it only initializes modern placeholder fields.

Even a completed application-global bootstrap yields **startup-null**, not a late Close main observation. The lifecycle remains BeforeMainPublication until the same canonical primary main's c9957b publication and subsequent constructor/factory effects occur. A Close command may not enter using the startup snapshot after those unimplemented phases. Retain startup history separately from current-at-command knowledge; missing publication/writers make the later read unavailable. No late clear/null assignment should be invented to express that loss of knowledge.

## Busy writes: exact caller placements, not cleanup hooks

| Native occurrence | Required production placement / blocker |
| --- | --- |
| Open028371 true ->01ca30 | The true setter is **outside and before**01ca30's inner suffix/manager logic. Current run checks suffix at190 before Session and calls prepareProjectModel at204. Therefore putting true busy inside prepareProjectModel or merely after the current suffix gate cannot claim full original caller ordering, especially for rejected suffixes/partial failure. A future actual Open coordinator must encompass that inner gate with its real application/caller-main root. |
| Open0283ba false | Only after01ca30,02a0e0 and01dc00 return; nested02a9b0 has its own true/false occurrences. Do not reset on every C++ exception or reuse one cursor for both nested and outer setters. |
| explicit Save0285c4 true | Wrap the actual selected main-window Save invocation before ProjectSave0980d0. Current run's work is distributed from saveCurrent at339 through project/MST/property/remesh stages. No single new call inserted beside339 covers the enclosing original main command automatically. |
| explicit Save02867d false | After raw0980d0 returns **and its selected status reporting returns**. Current r.message assignment at458 is not native2d0bd0 or its callbacks. File completion alone cannot authorize this false setter. |
| accepted Close029a54 true | After current busy read, actual manager can-close and successful Project's modal/nested-Save/callback stages. Only then write the same application's9=1,A=0,B=0, preserve counter including unknown, construct the closed borrowed arguments and enter the existing actual notifier. |
| late Close029c9b false | After its own prior cleanup/notification phases. Commit9=0/counter=0 first, then guard missing current main; do not preflight all main fields before those stores. |

The true setter itself needs no main read because its just-written nonzero+9 short-circuits show=false. The false setter needs fresh current main and, if nonnull, actual+480 stack index and conditionally+270. Its source-closed stores can be implemented on the shared owner once the actual occurrence is reached; reaching that occurrence is not supplied by a renamed copy of CloseBusyNativeFields.

Do not route production through CP88 by first fabricating a copied CloseBusyGlobalOwner prerequisite. Its direct-callee primitive needs a source-reached shared-owner command cursor/caller argument frame. Current-main, manager and Project aliases must be the same actual runtime objects, not just equal diagnostic addresses.

## Focused tests and exact public guard

For the minimal immediate ownership increment:

- Construct an actual AppState value and Session's borrowed overload; prepare/select a real model and verify the Session's opaque application identity stays the same through Project storage release. Default Session remains unbound; it creates no hidden second owner.
- All native facts remain unavailable and native-main/normal-Close completion remains false. In particular, neither the new overload nor Source/Project construction turns startup-null into a late current-main result.
- Session destruction before the lexical runtime owner performs no source busy/current-main/QString-resource cleanup. Escaped historical diagnostics are opaque and cannot admit a replacement owner at the same address.
- No public result/status/message/file change is introduced. Root should run existing core rejected-suffix/open-failure/Save guarded tests, in addition to the new focused Session binding target. No new CallServices input that accepts arbitrary busy/current-main values is necessary.

If the bootstrap producer is included in a later source scope, test actual shared-null acquire-once/order, unknown total refcount retained, exact flag/zero writes, retry without replay and the **BeforeMainPublication command guard**. If it is not included, prove missing member-resource producer refuses before initializer stores. Keep primitive tests distinct from actual public command integration.

The future public Close insertion is run's same live Session after the selected complete Save command/effects and before its scope exits near458. At present both the actual main constructor/publication/factory cursor and live stack state are absent; successful moProject/process5 also requires the default question's real dispatch and nested Save result. A real Close call must stop at those dependencies. Do not add an always-unavailable diagnostic and describe it as native cleanup progress, or call Close from Session destruction.

Recommended next scope: reserve core/convert.cpp and conversion_session.hpp/.cpp for the one actual runtime-owner binding, with new application_state.hpp/.cpp/test. Keep native writes staged unless the same source-owned bootstrap/member producer is implemented in that scope. Any actual Open/Save/Close callsite work additionally reserves the existing busy/entry/notifier files and retains the native current-main/modal guards. No production file was edited here.

## Source and byte receipt

New disassembly used DUMPBIN14.44.35228.0; ranges are end exclusive and exclude padding:

| Span | SHA-256 |
| --- | --- |
| DEF_GUI546c70..546c8b | `5331f97ee18ac0341fa00b25c6351363fb72d59c550f2e716ce0211963f64b22` |
| DEF_GUI004a60..004a7a | `ff8ad4c55c4a0417ec68d56b9b8725dad0c83c0303c9b351ee3a1eb0be7e22c6` |
| DEF_GUI031680..0316bc | `f175380bab5e1a4ce881642f7ae163c1cc6ee36762daf1690a96b6b9d9bac4b5` |
| DEF_GUI0318d0..031904 | `8e37b4fe90bfd056d2194ef6ad1216cfac5150f5ad73090f321d8488d92bc2b5` |
| DEF_GUId64030..d6404e | `f735e8a8dbbae0e39446ca0e179e99362000535c3b1c1928704528dfec7cdc7b` |
| QtCore67003030..67003046 | `4d87b1f86ac4ac31cc28206f7afc89a936ea6a199b7a69a8fb0056d7768da7c7` |

Current source hashes at the cited insertion points: core/convert.cpp `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30`; engine/conversion_session.cpp `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`; engine/conversion_session.hpp `46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31`; qt4.hpp `72ad25fd2ae6412c2c7043dd73a43f53398a9de9373a7834ad1faab2061e6261`. Prefix source path for these files is `source/apps/moproj2mst/`.
