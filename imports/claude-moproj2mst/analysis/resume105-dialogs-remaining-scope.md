# CP105 default-dialog and split-DB remaining scope

## CP103 closes the direct DB-open decision branch

The synthetic split fixture now exercises `engine::dbOpen` from on-disk DB files: header-only `X25.DB` step 25 and `X.DB` step 26 are made by `deformdb::Writer`, then loaded through the actual reader, `has_previous_part`, and `setup_part_chain`. It asserts the configured default-Yes warning, predecessor/current part order and ranges, merged directory count, and cache/loaded state. A one-part control checks the no-warning, one-part route. This is a valid synthetic DB chain, not a capture of a database split at the production 2-GB threshold.

The retained CP103 grouped-test receipt `analysis/resume103-integrated-grouped-repair4-tests.json` records `m2m.mdt.db_load_split_default` as passed (279 registered; 272 passed, 6 passed with logged gaps, 1 passed with logged differences; 0 failed). This is candidate test evidence, not fresh original GUI evidence. The test's warning text and resulting DB state verify the adapter's default profile and synchronous chain effects; they do not execute a QMessageBox or qualify timer/event delivery during the native nested dialog loop.

## Smallest next candidate test

No production change is justified for `dbOpen`: its default-Yes route is already implemented and the CP103 test passes. The smallest source-qualified continuation is one **caller-level Open test** that drives the same synthetic chain through `m2m::openProject` rather than calling `engine::dbOpen` directly. Use a disposable copy of a retained valid `.moproj` fixture with `COG_P1_B1.DB` and its generated predecessor beside it; initialize the Project/registry as existing `test_db_load.cpp::openProj` does, but stop before `activateSubProject` so the test isolates the early Open DB branch. Assert the resolved `primaryDbPath`, actual project DB handle's two-part chain, warning/default profile, cache/loaded state, `diagramViewHeight=175`, and normal Open parse result. Guard the thread current directory with `qt::CurrentDirScope`.

This test would close the remaining caller seam: native `0x14008a9b0` opens the project DB before reading the `.moproj` walk, candidate `moproj/read.cpp:29-53` calls `engine::openProjectDb` at line 48, and `engine/db_load.cpp:207-216` derives `primaryDbPath`, calls `dbOpen`, then updates `diagramViewHeight` on success. Suggested write set if assigned: extend only `source/apps/moproj2mst/tests/mdt/test_db_load_split_default.cpp`; no production or shared CMake file is needed. Test target remains `m2m_mdt_db_load_split_default` / CTest `m2m.mdt.db_load_split_default`.

## Remaining modal and Save/Close blockers

The source-closed default branch does not determine what the original nested event loop delivers. `QMessageBox::question` at `0x140cdc480` uses a modal `QDialog::exec`; retained evidence provides the button/default and synchronous receiver call but no event IDs/receiver identities during that interval. A runtime-parity claim needs a retained trace of this modal interval. The CP103 fixture does not and should not invent its event schedule.

The higher-impact Close composition remains guarded. `engine/close_dialog_answer.cpp` requires explicit schedules for the current-operation question and any nested last-operation warning; its tested Yes-to-Cancel mapping only applies after the nested Save result and warning schedule are supplied. CP60 establishes two separate intervals—Save-entry `processEvents(flags=0)` before validation, then the warning's modal loop—but not the timer partition/receiver effects. `engine/save_project.cpp:29-39` still records pending default Yes and stops at the warning delivery boundary. Public normal Close additionally lacks a bound same-application/main/manager lifecycle (`analysis/resume104-application-owner-integration-review.md`). Do not enable the public Close branch or remove its Save guard based on CP103's unrelated DB test.

## Evidence

- `analysis/resume102-open-split-db-default.md`, `analysis/resume102-split-db-fixture-review.md`: source/default contract and valid on-disk synthetic fixture design.
- `source/apps/moproj2mst/tests/mdt/test_db_load_split_default.cpp`: CP103 direct-`dbOpen` test.
- `analysis/resume103-integrated-grouped-repair4-tests.json`: target pass and grouped-suite qualification.
- `source/apps/moproj2mst/src/moproj/read.cpp:29-53`, `source/apps/moproj2mst/src/engine/db_load.cpp:186-216`: Open caller and DB path/cache/view-height order.
- `analysis/resume60-close-schedule-next.md`, `analysis/resume102-close-partial-order-gap.md`, `analysis/resume104-application-owner-integration-review.md`: exact unresolved Close modal schedule and same-owner gaps.

Read-only scope audit. No source/test changes, builds, GUI/original execution, or REA database actions were performed for CP105.
