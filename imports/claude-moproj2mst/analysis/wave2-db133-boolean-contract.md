# Boolean Object editor and side files

This package recovers the Boolean editor's filled selection state and its proven Save apply behavior, then restores the two side-file writers at their original point during MST generation. It does not infer an early Open-time apply merely to match the historical screenshot.

The [machine-readable contract](wave2-db133-boolean-contract.json) contains source/evidence hashes, original addresses, tests and explicit limits. The [factory](wave2-save-diffs-300-support-1411e0510.c), [fill](wave2-save-diffs-300-support-1411e25b0.c), and [apply](wave2-save-diffs-300-support-1411e2870.c) are private static exports acquired by the coordinator's worker. Their [project receipt](wave2-doe-ghidra-receipt.json) and [text verification](wave2-doe-ghidra-text-verification.json) bind them to the fixed original executable. No original runtime or GUI control was used in this package.

## Editor contract

Factory `0x1411e0510` routes the `Object` tree key to `SfObjectSetupBool`, constructor `0x1411e2480`, which calls fill `0x1411e25b0`. Fill starts each combo with literal `Add New Object`, then includes defined slot objects using `(%d) %s`. Workpiece matching has precedence over cutter matching. A number absent from that list leaves the corresponding combo at the first entry.

Apply `0x1411e2870` uses the filled combo text rather than rereading the current data number. It processes workpiece first, then cutter. Existing selections parse the parenthesized object number. `Add New Object` snapshots `firstFreeSlot` before `addObject`, stores the new object's number, writes literal `Workpiece` or `Cutter` into its name, then calls the existing `renameSlot` at the captured slot. The operation model and manager updates remain in the existing add/rename helpers.

[boolean_editor.cpp](../source/apps/moproj2mst/src/engine/boolean_editor.cpp) implements those two bodies separately. `BooleanObjectEditor` stores the filled texts per class/project. The Boolean Object-tree path captures the snapshot during `postLoadViews`; `updateDataInWidgetArea` applies it at Save before the caller's existing props-load and file-save sequence.

The historical DB133 `opened.png` already contains object 4 `Cutter`; its exact earlier editor replacement/apply event is not yet traced. The constructor fill itself only populates controls. This implementation intentionally does not insert creation into `booleanPostLoad` or claim full Open-state equivalence. Re-show and repeated-Save lifecycle coverage also remains bounded by the current fresh, single-Save workflow.

## Side-file contract

The retained [writer source](../evidence/gui_mo/s1/s1-04/d2c.c) and [writer specification](../source/docs/re/moproj2mst/s1-04/spec.md#12-side-files-written-during-mst-generation-1411e1820bd) establish `0x1411e1820`: first `Boolean.BWZ` (`0x1411e1970`), then `DEF_MACHDIST_CTRL2.INI` (`0x1411e0e40`). The Boolean MST caller invokes it after the `KFWRIT`/`Step.KEY` block and before `EXECMD 4`. It remains at that exact call site in `ops/boolean.cpp`; it is not moved into the earlier property-save pass.

Both files use WriteOnly without Text, hence LF bytes. The control INI uses the relative Step.KEY/output path and includes the existing default free-distortion BCC block only for a 3D operation class. That block's field order/defaults are already recovered in `save_l6.cpp`, `data/distortion.hpp` and [L6/p2.c](../evidence/gui_mo/s1/mdt/L6/p2.c).

The coordinator added an optional internal `W.sideEffects` sink and final Save step-5 effects integration. Existing writer aggregate initializers retain a null sink. The helper uses `Ctx.codepage`, records each generated path in `pendingWrites`, and recognizes explicit earlier `pendingDirectories`. It creates no directories. A missing parent skips that file, and the next writer is still attempted.

Actual permission failures, partial writes and failed earlier staged directory creation remain limitations of the adapter's existing deferred atomic effects model. The new directory list represents planned successful earlier effects, not proof of future physical writes. No broader failure-side-effect closure is claimed.

## Focused tests

[test_boolean_editor.cpp](../source/apps/moproj2mst/tests/mdt/test_boolean_editor.cpp) checks snapshot application after data changes, existing cutter selection, workpiece-match precedence, workpiece-before-cutter creation when both are missing, and missing/staged parent directories with 2D/3D side-file formatting. The coordinator reported the combined build and all 100 checks passed; its v2 changes only a separate test executable, with production hashes unchanged from v1. Historical DB133 saved project files match exactly in [the capture receipt](wave2-db133-boolean-v1-capture.json); only original capture PNG extras differ. The broader historical sweep reports 93 SAME and five DIFF. This worker did not build or launch the application, and these training comparisons are not fresh original acceptance.
