# CP74: Save operation/property dispatch inventory after Boolean step 4

## Conclusion

I found no additional source-closed, reachable operation/property Save gap to implement after the Boolean step-4 integration. The known file-producing `properties vt+0xb8` families have candidate handlers: Cogging, Boolean, L6 (Distortion, Diestress, DiestressBM, HeatTreat), and Express. The remaining reported property slots in the retained Save inventory are no-op/base bodies, not missing output handlers. I recommend that CP75 not add a speculative new Save dispatcher. The next incomplete public Save work is DoeOutput or Report, but both are still blocked by runtime state whose source contract is not closed.

## Reconciled candidate step-4 path

Native File > Save calls `0x1400982e0` after `.moproj` Save and before MST generation. The recovered mdt spec says it walks the operation manager in row order and calls properties `vt+0xb8` for each property object. The candidate `src/core/convert.cpp` step-4 loop now has these routes:

| Property/output family | Candidate route | Result/qualification |
|---|---|---|
| Cogging.MDT | `datafiles::saveCoggingMdt` in native row loop; aggregate `saveCoggingMdts` in recording mode | Candidate handler present. Native open is before writer/global effects; recording order is separately aggregated.
| Boolean.BWZ + DEF_MACHDIST_CTRL2.INI | `g_l5Slots.booleanSideFiles` for `BooleanProps` with data | Added at CP72 before MST open; the independent step-5 Boolean writer call remains.
| DieStress.Wzd, DieStressBmWizard.Wzd, MachDistortion.MDWZ/INI, HeatTreatWizard.HTWZ | `engine::l6PropsSave` | Handler present in `src/engine/save_l6.cpp`.
| Forming.FWZ | `engine::expressPropsSave` | Handler present in `src/engine/save_l3.cpp` for `ExpressProps` subclasses.
| Cycle and remaining base/no-op properties | No custom output branch | No source-supported file output is omitted here.

`src/engine/save_current.cpp` separately handles active operation `vt+0x68` and has its own unsupported branches. Those are not step-4 property dispatch misses and should not be treated as resolved by the table above.

## Why the apparent remaining candidates are not source-closed

### DoeOutput active Save

Native call site is generic Save step 2, `0x140052ec0`, dispatching `SfOprDoeOutput vt+0x68` at `0x140e2e730`; it writes a sample object through a viewer model and the study's `.DS` output document. Candidate `saveCurrentOperation` retains an explicit `M2M_E_UNSUPPORTED` guard for this branch. The `.DS` path and serializer have partial static evidence, but the viewer sample-object update, study/manager binding, DBLIST admission and process-global `+0x1c` state are not closed. CP65/66 explicitly retain the DBLIST refusal because the process global is not bound to a fresh candidate Save session.

The retained `89_B_MB2EN/BEND_2D.moproj` is a distinct complete project with a DoeOutput operation (ID 3), but its active item is the first diagram item, `SfOprHeatTransferExpress2D`, not DoeOutput. It therefore does not exercise the DoeOutput `vt+0x68` Save guard. It also has no retained `.DS` output at the project root. Do not cite it as an active DoeOutput Save test fixture.

### Report active Save

Native Save reaches Report `vt+0x68` at `0x1412adf90`, which calls `vt+0x80` and then `0x1412af0c0`. Candidate guard in `saveCurrentOperation` remains correct pending evidence: the second call uses viewer model helper `0x140d0d6f0`, distinct from the already-ported ROI helper `0x140d0d670`; returned viewer/model identity, object loop effects and failure order are not established. The Report `.DS` path helper is pure and does not release this Save guard. The retained CP65 audit calls out the unresolved helper and subsequent per-object helpers.

### FormingExpress active Save subbranches

Candidate `feSaveSimNames` contains conditional unsupported branches for mechanical-press stroke and the 2D model helper. The source save note identifies those branches, but the available retained FormingExpress 2D projects appended an inactive operation or otherwise do not establish an active save through the branch. They are not an immediate evidence-backed public Save delta, and a synthetic state would not qualify original output/failure behavior.

## Distinct test/fixture qualification

The Boolean step-4 failure tests updated at CP72 use the retained 133 Boolean3D project and specifically cover that output ordering. For a different family, the corpus has L6 outputs such as `101_B_DIS3F` and `102_B_DIS2F` with candidate failure tests already present in `tests/s/test_save_io.cpp` (`save_distortion_mdwz_failure_still_writes_ini_and_mst` and related cases). I found no source-closed untested distinct family from the property-slot inventory that warrants selecting a new production test now.

The useful next action is evidence collection, not another dispatcher: qualify a disposable original active DoeOutput Save through the sample-object update and actual `.DS` writer, including DBLIST global admission and partial-file effects; or resolve the Report viewer helper and document lifetime through normal Save/Close. Until one of those inputs is bound, keep both public guards.

## Evidence

- Native step-4 operation-row traversal and property slot call: `source/docs/re/moproj2mst/mdt/spec.md` §1, SHA-256 `607731629D7382770222FC223E5174FB14D82B9F7D7010C0126566CCEDBF7A4C`.
- Candidate dispatcher: `source/apps/moproj2mst/src/core/convert.cpp`, SHA-256 `C1B946637405BE3D4991DA4DF2DE1CC83BC81AE0993D7C48367845BA90031F47`.
- Candidate L6 and Express routes: `source/apps/moproj2mst/src/engine/save_l6.cpp` SHA-256 `900A1E46A36A3925443B2D5937A234686B6708FE0AA54A066A159366C7DA266E`; `source/apps/moproj2mst/src/engine/save_l3.cpp` SHA-256 `D63AC30C8DF84A84AAAB71890D8B2C9CA4595692801C77674BA51CF43AF9668D`.
- Current active operation Save guards: `source/apps/moproj2mst/src/engine/save_current.cpp`, SHA-256 `BA944DADE590FD73CBE16C95A2A83D7F50868B2F1B05370BD477F7611D7F9812`.
- DoeOutput process-global qualification: `analysis/resume65-dblist-global-owner.md` and `analysis/resume66-dblist-global-lifecycle.md`.
- Report viewer/owner qualification: `analysis/resume65-report-ownership.md`.
- The `89_B_MB2EN` fixture is not an active DoeOutput Save case: its `.moproj` has `Active="0"`, and the first diagram item is `SfOprHeatTransferExpress2D`.

Read-only inventory only. No source, build, test, GUI, fixture, or mutable REA database was changed or run.
