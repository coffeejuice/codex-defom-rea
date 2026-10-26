# CP120 DOE Report / Optimisation active-Save ordering audit

## Finding

A bounded, source-closed private contract is the Save preflight that requires `SfOprShapeOptim`, `SfOprDoeOutput`, and `SfOprReport` to be last in each sub-project flow. The implementation scans each flow chain before choosing the current operation and records the unresolved warning-modal boundary on the first violation. This contract is deterministic from the already built flow and item class names; it does not require Report-document or optimisation-study payloads.

It is not enough to admit either operation to public Save. In the active `WRITE_SAVE` path, `openActiveOperation` runs before `saveCurrent`. Report currently refuses during its Open/load slot when it needs the `SP+0x30` report document. A type-3 ShapeOptim refuses during its Open/load slot when it needs the `SP+0x28` optimisation study. Even if those Open prerequisites were satisfied, a non-last special operation encounters an unresolved modal decision before step 2; if last, its operation-specific Save behavior still has separate payload dependencies.

## Source-backed ordering

The current active path in `src/core/convert.cpp` is:

1. Load and activate the project/sub-project, then call `engine::openActiveOperation` (`0x14003a3c0` / `0x140047f80`) before Save. That resets/loads the active operation and invokes its class `loadOperation` virtual (`loader.cpp` around `openActiveOperation`).
2. Apply Open-produced effects, then enter Save. `saveCurrent` checks `openedOk`, scans each sub-project with `lastOperationCheck`, and only after all checks selects the active flow item and calls `saveCurrentOperation` (`save_project.cpp`).
3. If step 2 returns, native Save writes the `.moproj`, then step-4 property writers, then step-5 MST and remesh work. A guard in either the Open loader or the preflight prevents those later phases from being reached.

The private last-operation rule in `src/engine/save_project.cpp` is explicit: `mustBeLast` matches exactly `SfOprShapeOptim`, `SfOprDoeOutput`, and `SfOprReport`; `lastOperationCheck` scans every chain item except its final item. A match marks `lastOperationModalPending`, records `DialogAnswer::Yes` as the candidate default, then throws `M2M_E_UNSUPPORTED` because modal delivery/result is not recovered. It does not select a current operation, enter `saveCurrentOperation`, or write the project file. This is the smallest source-closed branch: class-key classification, flow position, and the pre-step-2 stop are all directly represented.

## Report and ShapeOptim boundaries before the preflight

The earlier Open stage is the current first obstacle on an active operation:

- `ReportClass::loadOperation` (`src/ops/report.cpp`, native `0x1412add20`) calls `loadClassData` first. With a live Project and sub-project, `loadClassData` reaches the `SP+0x30` report-document read (`0x1400b5ad0` / `0x140643100`, `<project>.DS`) and refuses because that GUI object is not reproduced. Its later load steps are therefore not entered.
- `ShapeOptimClass::loadOperation` (`src/engine/shapeoptim_load.cpp`, native `0x14013beb0`) initializes its class state, resolves a predecessor/engine model, sets the dimension, and calls `loadClassData` (`vt+0x78`) before `loadData` (`vt+0x1b8`). If the active sub-project is type 3, `loadClassData` refuses at the missing `SP+0x28` optimisation study (`0x140917ff0`). If that is not type 3, the function has a distinct, narrower path; that fact does not qualify a type-3 study or public active Save.

Thus Save's “must be last” preflight is a closed private rule but is reached only after the operation's Open/load path in the normal active Save schedule. It should not be presented as proof that Report or type-3 ShapeOptim currently reaches the Save preflight in public conversion.

## Save bodies if earlier gates were later qualified

`saveCurrentOperation` in `src/engine/save_current.cpp` has distinct per-class branches. Report's `vt+0x68` immediately refuses because it runs Report `vt+0x80` using the report document plus viewer models. ShapeOptim has a special path: update data in the widget area or copy props to the class, then its `vt+0x80` and `vt+0xe8` slots, and returns without falling into the base `vt+0xe8`/`vt+0x80` chain. Its current `saveOperationFiles` first derives the study folder and obtains the ROI model, then refuses if any nonempty ROI geometry payload needs `.GEO` output. This establishes useful internal ordering, but does not grant a valid type-3 study, ROI data, or viewer owner.

The `ReportClass` and ShapeOptim MST methods are separate, bounded no-op writers (`src/ops/report.cpp`): their `writeMst` returns without output and `simList` is empty. They do not imply successful operation Save or make earlier Open dependencies optional.

## Next public dependency

Keep public Report and type-3 ShapeOptim activation guarded until the source-owned Open prerequisites are bound to the selected active operation and exact sub-project owner: Report document construction/read (`SP+0x30`, its main-window parent and file effects), and ShapeOptim's type-3 optimisation-study construction/population (`SP+0x28`) plus any required ROI model/read effects. Separately, for a special operation that is not last, bind the warning modal's actual dispatch, answer, and continuation before allowing `lastOperationCheck` to proceed. For a last operation, qualify its operation-specific Save body and its owning payloads before reaching project XML, step 4, or MST.

No public Save guard should be relaxed on the strength of the private ordering rule or the empty MST writer. This audit makes no claim about native runtime reachability beyond the source-ordered active path and the retained class-slot contracts.

## Limits

This is a read-only source audit of `convert.cpp`, `loader.cpp`, `save_project.cpp`, `save_current.cpp`, `ops/report.cpp`, and `shapeoptim_load.cpp`. No GUI, original execution, mutable REA operation, source edit, build, or test was run.
