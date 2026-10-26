# CP164 format/path boundary: ShapeOptim study-folder mkpath

## Selected independent boundary

The next independent source-backed path-conversion boundary is ShapeOptim `studyFolder`: form `<project folder>/DoeStudies/<study>/Control/Opt/` as a Qt string, then pass that exact path to the native `QDir::mkpath` helper at the point reached during Open. This remains relevant to the single-project Open→Save→Close scope because Open may create the study directory before a later payload guard, and the attempted filesystem effect survives that later refusal. It is independent of SPRING2 parsing/codec work owned in CP162/CP163.

This boundary is already implemented for the supported owner profile in the current candidate; it is not an unimplemented source change. The value for the next worker is a bounded integration/qualification edge: retain exact path construction and attempt timing, and keep failed-mkpath modal delivery guarded. Do not broaden the Type2 study-name owner predicate or claim Save/Close parity from this isolated effect.

## Original qualification and exact limit

Original `DEF_GUI_MO_64.exe` v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained static report `analysis/resume122-shapeoptim-mkpath-original.md` (SHA-256 `2d201ce1158b6c0197eab9ecde7db01884527258cb2cd61066485f2d83ff945e`) establishes:

- `ShapeOptimClass::loadData` reaches `studyFolder` before checking the stored ROI geometry-name list.
- `studyFolder` constructs the rooted path in component order `DoeStudies`, study name, `Control`, `Opt`, with Qt separators and trailing separator; the call supplies `dialogs=true`.
- Shared helper `0x14081afb0` invokes `QDir::mkpath`. Success continues without a dialog. Failure creates a one-OK modal, ignores its result, and returns true after the modal.
- The static record does not establish an actual resolved path from a fresh original run, modal event delivery, or a failed-mkpath continuation occurrence.

The companion helper correction `analysis/wave3-earlier-save-mkdir-contract-correction.json` (SHA-256 `e1cf5cd05222646a48268ec257a4667ca85bf8aa2b31826cd582126f411a9bce`) rules out a separate `QDir::exists` precheck. `analysis/wave3-directory-failure-dialog-contract.json` (SHA-256 `4111e716cc5d5482433f33c6330a63ec4a9f618b592cd3a335b7f88887bf8ec4`) qualifies QMessageBox construction/defaults while leaving actual modal event delivery unknown.

## Candidate binding and current tests

- `source/apps/moproj2mst/src/engine/shapeoptim_load.cpp` SHA-256 `1428fd84e341ccba66d1203243e1836a32a0292ffa44898d927378faadcb1367`. `studyFolder` queues the ordered path; `loadData` calls `completeShapeOptimStudyFolderEffect` before the later geometry-name guard. The immediate native attempt is selected for native directory/open profiles.
- `source/apps/moproj2mst/src/qt4/native_directories.cpp` SHA-256 `48ba21411d091ec7c4139de5dcbeb7dc4fb18a4c712bf62a3d5d572bfc84f2ec`; `source/apps/moproj2mst/include/m2m/native_directories.hpp` SHA-256 `8d705e756c7845432950727ead3df47bc98edd1806f4442ab586cbad0a94ba20`.
- `source/apps/moproj2mst/tests/mdt/test_shapeoptim_study_folder_effect.cpp` SHA-256 `7c9c298509eb563d3de65d5c645b0c431163876258ab16077dd13f4f320ed2fd` covers deferred recording, successful immediate creation, persistence across a simulated later guard, actual-class load reachability, and failed attempt recording/stopping at the unrecovered modal boundary.

These are source/test bindings, not a claim that this test file was executed in this investigation. No build or test run occurred here.

## Exact guard and next tests

Keep the attempt gated by the existing exact Type2 SubProject/owned DOE-study alias and nonempty study name. Preserve the path under the project root and retain the original effect order. Keep type-3 studies, unowned/mismatched aliases, absent studies, and empty names behind current refusals or empty-path behavior; none is justified by this static witness.

The source-ready positive test matrix is already present: (1) effect remains deferred for non-native recording; (2) supported native profile creates precisely the rooted `.../DoeStudies/<name>/Control/Opt/` directory before the later `.GEO` guard; (3) the later guard cannot remove that directory; (4) false mkpath records the failed attempt and stops at the modal-delivery boundary. At integration, run this focused test with the root's source-stable checkpoint group. Do not turn the simulated guard into a claim about an original end-to-end Open run.

The exact missing witness for extending the false-mkpath case is a same-occurrence original observation binding the `QDir::mkpath` false return to the constructed QMessageBox, its one-OK/default result, and continuation after modal dismissal. Until that exists, preserve the candidate stop and do not claim native failure-path parity. A fresh resolved path/open/save/close comparison is a separate end-to-end acceptance witness, not needed to implement the already bounded helper behavior.

## Investigation limits

Read-only inspection of retained evidence and current source/tests only. No source/tests/shared plan/state, original evidence, GUI/app control, live REA session, build, or test was modified or run. No reconstructed executable version is implicated.
