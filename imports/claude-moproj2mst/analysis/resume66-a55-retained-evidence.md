# CP66 A55 retained-evidence recheck

## Finding

No retained artifact positively binds A55's original `MoInputMode` to 0 or 1. The CP63 conclusion stands. I found one useful adjacent state receipt: A55's `opened.png` was captured at 2026-10-22 12:23:24, two seconds before the `rs_log.tsv` saved entry, and visually shows the Simulation Controls pane on `Operation forming (2)` / `Operation 2`. This makes Task00002 being the displayed operation plausible at the captured moment, but it does not show the mode preference, editor300's CG radio state, an unchecked→checked event, or whether the screenshot followed the exact same event ordering that produced the saved bytes.

The visible **Deformation** control in the screenshot is an operation mode label; it is not evidence of global `MoInputMode`. The screenshot cannot bridge the missing settings/process binding.

## Archived settings and run records

The generic settings backup `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` records `MoInputMode=1` and has timestamp 09:12:04, about three hours before A55's 12:23 capture. Its RecentProjects list contains multiple Oracle-B cases but not A55. This is consistent with an environment snapshot, not a receipt of which INI the A55 process read. The run ledger lines around A55 record close dialog response and saved byte count (`3541`); they do not record process ID, settings path/hash/value, mode actions, or editor selection/fill callbacks.

The screenshot plus timestamps improve the state context only: the displayed operation was forming (2) close to Save. The original mode remains a first-order unresolved input because factory selection routes mode 0 to editor100 and nonzero to editor300. Even if the selected operation was Task00002, the retained screenshot cannot establish that editor300 was constructed or that the CG button changed state during initial fill. Therefore the two raw `SimCtrl.KEY` byte differences at offsets 1439 and 1455 remain unassigned to an implementation defect or a confirmed mode mismatch.

## Smallest decisive fresh receipt

After the established app-control release gate is satisfied, capture the actual original process's INI path, hash, and parsed `MoInputMode` before launch; log mode-setting actions/signals through Open; then record first active tree operation, editor factory choice, CG control state before and after Iteration fill, and delivery of the toggle callback. Preserve the post-fill values and saved raw `Task00002/SimCtrl.KEY` bytes/hash. The gate remains closed now; this note does not authorize or schedule GUI input.

Evidence reviewed: `evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1/opened.png`, `evidence/gui_mo/oracle_b/_tools/rs_log.tsv`, `evidence/gui_mo/oracle_b/_tools/rs_run.out`, the L5 settings backup and its hash receipt, `analysis/resume63-a55-mode-qualification.md`, and `analysis/wave2-save-diffs-diagnosis.json`. No GUI/app control, launch, build, source edit, or mutable REA operation was performed.
