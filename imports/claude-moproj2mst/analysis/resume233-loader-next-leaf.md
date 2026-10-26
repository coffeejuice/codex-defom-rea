# CP233 loader next-leaf design (revised after review)

## Recommendation and exact command contract

The `decideAndLoad` DB-step write is suitable only as an explicitly admitted continuation command. Add a narrow `GuiModel::setLoadedStepNumberAfterAdmission(int)` (prefer private/internal visibility if the project structure allows) whose sole body is the direct `step->u744 = stepNumber` assignment. It must **not** call `requireGeometrySyncAvailable()`: the existing caller has already admitted the model by calling `m->simCtl()` in the `if (!m || !m->simCtl())` check and again to obtain `sc`, before reading or mutating `nstart`. A guard inside the new command would be a new throw point after `sc->nstart` has been changed, changing the mutation prefix. The intended contract is synchronous call from this already-admitted DB branch, with no intervening owner replacement or geometry-barrier mutation; the source has no concurrent/reentrant transition in this path. It is not a general-purpose independently admitted setter. It keeps the existing unchecked step assumption and adds no null fallback.

The exact ordering in `source/apps/moproj2mst/src/engine/loader.cpp::decideAndLoad` is: `loadDbStep`; reject missing model or failed `simCtl()` with the existing `unsupported`; obtain `sc`; derive `ns` from `sc->nstart` and `lastOfPrev`; assign `sc->nstart = ns`; assign `m->step->u744 = step`; set `tnow3`; set `tnow2`; update `op.nstartGiven` and `op.startStepNo`; then continue operation-specific handling. Replace only the `u744` assignment at its existing position. Keep the method non-throwing by construction (no new guard or validation), and do not merge adjacent SC or Operation state into it.

## Direct caller-level test

Extend `TEST_CASE(oracle_b_124_db_step_load)` in `source/apps/moproj2mst/tests/mdt/test_db_load.cpp`. This test opens the actual project/operation path, which reaches `decideAndLoad` through `openActiveOperation`; the existing assertions already check `nstartGiven == 1`, `startStepNo == -4`, restart state, objects, equipment, Save outputs, and read-only DB immutability. Add an assertion on `engine::opModel(o.p, *o.op)` being non-null and its `step->u744` matching the DB-selected step. Prefer deriving the expected selected step from the loaded DB reader and operation index using the same public read-only `def::dbio::query::op_first_step`/appropriate query after confirming the branch for this fixture, or compare against the source GUI fixture's recorded step if such a manifest exists. Do not treat `test_db_load.cpp` lines 179–196 or 240 as caller-level proof: those are lower-level DB reader tests.

The focused verification target is the existing MDT DB-load test target containing `test_db_load.cpp`; resolve the CTest name from configured metadata at implementation time. Existing coverage suffices for ordinary DB read, rejected read, and selected-step semantics; no null-step test is appropriate because this admitted path assumes a step.

## Source files and ownership

Production: `source/apps/moproj2mst/include/m2m/engine.hpp`, `source/apps/moproj2mst/src/engine/gui_model.cpp`, and `source/apps/moproj2mst/src/engine/loader.cpp`. Test: `source/apps/moproj2mst/tests/mdt/test_db_load.cpp`. Internal C++ only; no C ABI changes. Coordinate header/model implementation ownership before source edits.

## Evidence and qualification

`source/apps/moproj2mst/src/engine/gui_model_access.hpp` shows the distinct callback seam calls `requireGeometrySyncAvailable()` before borrowing; this command is **not** using that seam. In `loader.cpp`, both `m->simCtl()` calls occur before `sc->nstart` is read or written, so the geometry refusal remains ahead of the entire mutation sequence. This report intentionally makes the command admission precondition explicit rather than introducing a new post-mutation refusal. Current source and `oracle_b_124_db_step_load` establish the available caller path; no fresh original runtime capture is claimed.

Only this report is changed. No source, test, build, shared-state, original, GUI, or REA artifacts were modified, and no tests were run.
