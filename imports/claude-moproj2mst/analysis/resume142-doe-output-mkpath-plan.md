# CP142: bounded DOE Output Control/Regions mkpath plan

## Scope and call position

The smallest disjoint DoeOutput Save effect is the first native operation at `0x140e33210`: request `QDir::mkpath` for the active Type2 Study's `Control/Regions` directory using shared helper `0x14081afb0(path, true)`. It precedes the viewer-model check, all Regions cleanup/Common_Mesh removal, geometry, current-operation helper and `.DS` writing. Keep the existing public `SfOprDoeOutput vt+0x68` guard in `engine/save_current.cpp`; this private prefix alone does not authorize continuing the Save.

## Proposed isolated API

Reserve NEW `src/doe/output_regions_mkpath_prefix.hpp/.cpp` and NEW `tests/doe/test_output_regions_mkpath_prefix.cpp`; no shared writer/header, CMake, notifier, INI, DoeOutput public dispatch or Session API is needed. A narrow API should accept the explicit `Project&`, `SubProject&`, actual owned `Study&`, and `engine::FileEffects&`, returning an outcome such as `AttemptSucceeded` or `WarningModalRequired` with the exact attempted path and Boolean. This outcome is a private stage result, not a Save continuation receipt.

Preflight all ownership before filesystem work:

1. Require `sub.project == &project`, `activeSubProject(project) == &sub`, and `sub.type == 2`.
2. Require `sub.ownedDoeStudy` is nonnull and refers to the supplied Study; `sub.doe == &study`, `study.sub == &sub`, and a nonempty `study.name`.
3. Require the project folder/path to be absolute before calling the current `qt::nativeMkpathAbsolute` adapter; this is a candidate-adapter precondition, not evidence that native QDir rejects relative inputs. Do not invent a fallback Study or derive a Type3 optimizer name. Empty-name and relative-path profiles remain outside this bounded helper until their intended call inputs are qualified.

Then compute `mst::doeControlDir(sub, &study.name) + qt::kSep + L("Regions")`, normalized with the existing Qt clean-path routine. If and only if `effects.nativeDirectories` selects immediate Save I/O, invoke `qt::nativeMkpathAbsolute(path)` at this boundary and append exactly one `{path, succeeded}` to `effects.directoryAttempts`. Do not call `StudyIo::ensureDirectory`: its candidate contract probes existence first and uses `std::filesystem`, unlike the retained direct `QDir::mkpath` helper. Do not append only to deferred `effects.mkdirs` on the immediate profile.

On success, return `AttemptSucceeded` and stop the private helper there. On failure, retain the failed attempt and return `WarningModalRequired`; do not throw it away, report a generic filesystem false as native abort, or continue to cleanup. Native `0x14081afb0` with `dialogs=true` enters a one-OK QMessageBox modal loop, ignores the result, then returns true and continues. UI delivery/dismissal is outside this helper and remains an explicit later boundary. If the helper is invoked in a non-immediate/planning profile, refuse before mutation or introduce a distinct `PlannedOnly` outcome; never claim the queued mkdir equals the reached native attempt.

## Focused tests

Use the existing `tests/support/test_paths.hpp` scratch root and Testkit `CHECK`/`REQUIRE`; CMake already discovers `tests/*/test_*.cpp` under each test directory. Build an actual Type2 chain (`Project` → owned active `SubProject` → `Study` with matching alias/backlink) but no viewer model, proving the first mkdir does not depend on later model state.

- Successful attempt: unique scratch project folder, expect the exact normalized `.../DoeStudies/<study>/Control/Regions` path, one successful immediate `directoryAttempts` entry, physical directory present, no `FileEffects.writes/deletes`, no Study state changes.
- Failed attempt: create a regular file at the exact `Regions` target so `nativeMkpathAbsolute` returns false; expect one retained failed attempt and `WarningModalRequired`, with no later cleanup/write mutations. This is a filesystem-prefix test only; do not simulate QMessageBox dismissal or assert native continuation after it.
- Owner rejection: wrong Project/SUB/Study identity, Type3, empty/mismatched Study alias, or relative project path must stop before new attempt/creation and preserve preseeded effects.
- Keep existing `tests/s/test_native_directories.cpp` as the lower-level QDir-adapter coverage; add no C `assert` and no direct host-temp paths outside M2M_TEST_SCRATCH.

The helper test cannot verify the public caller ran at `0x140e33210`. Public placement must retain the order relative to the viewer/model check and must hold on mkpath failure until the actual warning modal result is represented.

## Evidence and limits

Retained native order: `analysis/wave2-doe-lifecycle.md` (“Current-operation Save effects precede project/control Save”), original shared QDir failure contract `analysis/resume122-shapeoptim-mkpath-original.md`, and candidate directory primitives `src/qt4/native_directories.cpp::nativeMkpathAbsolute`, `engine::FileEffects::directoryAttempts`, `mst::doeControlDir`, and `engine::makeFolders`' immediate `nativeDirectories` pattern. The ShapeOptim callsite is distinct; it establishes only the shared helper's failure dialog/true-return behavior.

This cut closes one private directory attempt only. The next DoeOutput effects still need the same-occurrence viewer model, ROI model, cached common-mesh object, Study-owned output `SessionDocument`, selected `SfTaskDoe`/manager values, ordered Regions recursive deletion/Common_Mesh cleanup and the `.DS` writer's immediate UTF-8 QFile contract. No source, tests, build, GUI, original application, or mutable REA state was changed for this plan.
