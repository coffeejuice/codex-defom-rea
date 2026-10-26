# CP122 ShapeOptim study-folder immediate effect implementation

## Change

`ShapeOptimClass::studyFolder` still derives the same bounded absent-study-name path and initially queues it in `FileEffects`. At each native caller, a private helper consumes that exact trailing queue entry and invokes existing `qt::nativeMkpathAbsolute` immediately:

- Open `loadData`: immediate only when `ctx.nativeOperationBlobs || fx.nativeDirectories`. `convert.cpp` binds `nativeOperationBlobs = writeFile && M2M_FLAG_WRITE_SAVE` before `openActiveOperation`, so this identifies the native full-Save Open callback. The call precedes the later `listF0_` GEO refusal.
- Save `saveOperationFiles`: immediate only under its existing `fx.nativeDirectories` profile.
- Recording paths retain the existing deferred `fx.mkdirs` entry.

The helper removes the queued entry before making the single immediate attempt, records exactly one `directoryAttempts` item, and does not replay the attempt later. A successful attempt returns to the original subsequent code. A failed attempt is recorded and stops at the unqualified one-OK warning-modal boundary (`0x14081afb0`); it is not treated as completed continuation. The actual warning-dialog event/result is not modeled. Earlier path components created before a failure remain untouched.

No type-3 optimization-study or Report behavior was changed. The type-3 guard in `loadClassData`, Report document guard, later geometry guards, and public Save gates remain as before. The scope is only the source path where `studyFolder` passed its existing DOE-study-name guard and produced a path.

## Focused tests

New `tests/mdt/test_shapeoptim_study_folder_effect.cpp` exercises the file-local seam without changing a shared/public header:

1. Non-immediate mode keeps the queued path and performs no filesystem mutation.
2. A real `ShapeOptimClass::loadData` call with an initialized Project/SUB0, the owned ShapeOptim class inserted under its key in `Project::registry`, a bound current Operation, and an actual `GuiModel` reaches `studyFolder`, performs one successful native mkdir, and returns with the directory present and the event recorded once. Registry insertion is required because `engine::opModel` resolves `classOf(op)` through that registry before reading the operation's cached class pointer.
3. Immediate successful creation through the private seam remains present after a simulated later GEO guard.
4. A path blocked by a file records failure, consumes the queue entry, and stops at the explicit unsupported warning-modal boundary without retry or cleanup.

All test outputs use only leaves under the PID-specific `C:\PROBLEM\M2M_SO_<pid>` root. Before any `remove_all`, the helper resolves the candidate and verifies it remains inside that canonical PID root; it also verifies the PID root itself resolves directly beneath canonical `C:\PROBLEM`. Both the leaf and the actual generated study-folder path are checked to stay below 100 characters. Cleanup removes only the named test leaf. No original project fixtures are touched.

The real-class test cannot reach the later populated-GEO refusal without fabricating private `listF0_` contents. A repository search finds only its declaration, the `loadData` guard/use, and `ShapeOptimClass::init` clearing it; no producer currently populates the list. Therefore the test proves the real class/caller mkpath path and persistence through normal `loadData` completion. The isolated seam test separately proves persistence across a later simulated guard. Recovering the missing `listF0_` owner is still required for a real-class mkpath-then-GEO-refusal integration test.

## Static preflight and limits

`git diff --check` passed for the owned source and new test. Static inspection confirms the `nativeOperationBlobs` source assignment precedes `openActiveOperation`, the native attempt occurs before the later geometry guard, the false result is recorded before throwing, and no other ShapeOptim or Report paths were modified. The focused test, build, and full suite were not run; root owns validation.

No GUI, original execution, mutable REA action, or shared-header/convert/CMake change was made. The UI warning itself and the original modal event continuation remain unrecovered.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/shapeoptim_load.cpp` | `A3B55140570AF081FCD2C3290AF56EC350F05BDBEA71F61284A8D9DD9A5D6280` |
| `source/apps/moproj2mst/tests/mdt/test_shapeoptim_study_folder_effect.cpp` | `4771A12A0B43489FF4AB3829B022CCAEB272C78F3BE9B01B6D57C4022CD8D099` |
