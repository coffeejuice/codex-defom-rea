# CP87: Source-closed Open-time Setup Type decision

## Concrete decision

The Open-time **“Setup Type (Interactive/Batch)”** question has an implementable Yes/default contract without a new GUI capture for the retained Oracle B 124 case. It is reached during the selected operation-load path when the operation is not the first chain item, is restart-capable, `OprFlag == 0`, and `dbHasStepsFor()` finds a stored DB step. In a fresh process the “Keep my choice” global is zero. Native `0x140055360` explicitly sets Yes as default; the retained `loader-spec.md` records that Oracle B case 124 pressed Enter and the operation loaded from the DB step.

This is a case-specific branch contract, not proof that every application style/focus profile selects Yes or that every default-dialog path is closed.

## Candidate behavior and exact guard

`source/apps/moproj2mst/src/engine/loader.cpp::decideAndLoad` enters the policy after chain-head/non-restart branches and restart-file handling. Its Setup Type condition is `op.oprFlag == 0` plus `dbHasStepsFor(p, op, ...)`. On this branch, candidate behavior records the default-answer warning, sets `interactive=true`, `useDb=true`, and loads the selected DB step. If no DB step is available, no Setup Type question is represented and the loader follows the existing non-dialog path. The “Missing Restart File” critical message is a separate earlier decision and must not be conflated with Setup Type.

## Focused test

The exact existing candidate regression is `oracle_b_124_db_step_load` in `source/apps/moproj2mst/tests/mdt/test_db_load.cpp` (CTest target `m2m.mdt.db_load`). It:

- copies the retained `124_B_DBSTEP_before` fixture and protects the copied DB as read-only;
- opens the project and activates the eligible operation;
- requires one warning for the question, matching `[Op. 4] EXIT`;
- checks DB-loaded operation state (`StartStepNo=-4`, restart flag, object/material/equipment values);
- compares load-time Material KEY effects and Save-step-2 files against the retained GUI-saved Oracle B folder;
- verifies the DB bytes and timestamp remain unchanged.

The complementary `oracle_b_125_126_key_load` test in the same file covers the no-DB/no-question side: `OprFlag==1`, no Setup Type warning, and KEY load. The 124 test skips when Oracle B artifacts are unavailable, so the focused source guard remains the predicate `!isChainHead && restartCapable && !restartGiven-with-file && OprFlag==0 && dbHasStepsFor`. A fully self-contained branch test would need a deterministic DB-step fixture; do not substitute a test that merely injects `ctx.warn` or manually sets `interactive`.

## Evidence and limits

- `source/docs/re/moproj2mst/mdt/loader-spec.md` §6: explicit `setDefaultButton(QMessageBox::Yes)`, fresh-process “Keep my choice” state, and DB-step path.
- `source/apps/moproj2mst/src/engine/loader.cpp::decideAndLoad`: candidate guard and continuation.
- `source/docs/re/moproj2mst/mdt/DB/spec.md` §6 / `source/docs/re/moproj2mst/mdt/spec.md` dialog summary: operation-load branch policy.
- `source/apps/moproj2mst/tests/mdt/test_db_load.cpp::oracle_b_124_db_step_load` and `oracle_b_125_126_key_load`: concrete positive/negative regressions.

The branch can be implemented and tested against retained evidence without an additional GUI session. Keep the claim bounded to this OprFlag-0, stored-step case and retained Oracle B Enter behavior; no full default-dialog matrix follows. This lane was read-only; no source, build, test, GUI, or mutable REA operation was performed.
