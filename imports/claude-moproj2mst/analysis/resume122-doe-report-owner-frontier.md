# CP122 DOE Report / ShapeOptim owner and Save-effect frontier

## Finding

The smallest actionable private frontier is ShapeOptim's `vt+0x1b8` `loadData` path for a live current operation with Project, active sub-project, and engine model. It calls `studyFolder`—whose native contract is `QDir::mkpath`—before checking whether stored ROI geometry names require unrecovered `.GEO` loading. The current adapter queues the folder in `FileEffects`; the active Save caller applies Open effects only after `openActiveOperation` returns. Therefore, if the later geometry guard throws, the candidate can lose this already-reached directory effect, while the source-identified native order makes the directory creation immediate. This is a narrow effect-order gap to isolate privately; it does not justify public ShapeOptim or Report admission.

## Exact owner and order

`core/convert.cpp` calls `openActiveOperation(p, ctx, opened)` before Save's step 2. `engine/loader.cpp::openActiveOperation` resolves the active flow item and operation, installs the class's `current` owner, resets its model, then invokes `classOf(*op)->loadOperation(...)`.

For ShapeOptim, `src/engine/shapeoptim_load.cpp::ShapeOptimClass::loadOperation` (`0x14013beb0`) performs its current/predecessor/model/dimension setup and `loadClassData` before `loadData`. A live type-3 active sub-project refuses inside `loadClassData` at the missing optimisation study `SP+0x28` (`0x140917ff0`), so it never reaches the folder branch below. The branch applies only after that guard has not fired, e.g. a non-type-3 active sub-project.

In `ShapeOptimClass::loadData` (`0x14013cbe0`), the ordered prefix is:

1. Require `current`; get `opModel(p,*current)`. Missing current refuses; missing model returns without effects.
2. Get the Project and active sub-project. Missing owner returns without effects.
3. Call `studyFolder(p,fx)` (`0x14013d180`). It rechecks current/model/Project/sub-project, obtains the DOE study name through `SP+0x20`, and for the represented absent-name case records the `<project>/DoeStudies/<study>/Control/Opt/` directory. The native call is `QDir::mkpath(path,1)`; the current adapter adds the path to `fx.mkdirs`.
4. Select the ROI model. If `listF0_` is nonempty, refuse because those study geometry files are not reproduced. On that failure, `loadOperation` does not return, so `openActiveOperation` does not return.
5. Only when the filename-list guard is passed does it continue to `postLoadViews`, collect ROI object IDs and clamp the selected fields.

After `openActiveOperation` returns, `convert.cpp` applies `opened` effects. It does not reach that application point when a preceding load slot throws. Thus the `fx.mkdirs` entry is not applied on the candidate failure path. The retained source annotations for `studyFolder` identify a direct `QDir::mkpath` call before the later geometry-file branch, so the native directory effect is already committed before that guard. The smallest private correction point is the `studyFolder` call/effect boundary with an explicit immediate-directory service or event; the later GEO load remains guarded. A focused test should prove directory attempt precedes the geometry refusal and survives it, without claiming successful geometry loading.

## Report remains a separate earlier stop

`ReportClass::loadOperation` (`0x1412add20`) calls `loadClassData` first. With its actual Project and active sub-project, `loadClassData` reaches the `SP+0x30` report-document path (`0x1400b5ad0` / `0x140643100`, `<project>.DS`) and refuses because that GUI-owned document is not represented. It does not enter its later viewer/report UI work. Report's MST `writeMst` and `simList` are empty, but those private no-op outputs do not bypass its earlier active-operation load or Save's `vt+0x68` document/viewer dependency.

## Save order and public dependency

After Open succeeds, `saveCurrent` runs the already integrated `mustBeLast` checks before selecting the current operation. If they pass, `saveCurrentOperation` handles ShapeOptim specially: update editor data or copy props to the class; call ShapeOptim `vt+0x80`; call `vt+0xe8`; return without the base slot chain. In `vt+0x80`, ShapeOptim synchronizes the property/class fields, inspects the ROI model, and then refuses when the active sub-project is type 3 because the optimization study/objective data owner is missing. Therefore a normal active type-3 Open currently stops earlier in `loadClassData`, and even a later qualified Open would still need the `vt+0x80` owner before public step 3 / step 4 / step 5 can be reached.

For Report, after a future successful load and the Save preflight, `saveCurrentOperation`'s Report `vt+0x68` still needs the report document and viewer models. These are independent dependencies from Report's empty MST writer and from the already-integrated must-be-last warning.

Do not relax public Open/Save guards based on the private directory-prefix observation. The isolated next increment is the immediate directory-attempt event at `studyFolder` while retaining the subsequent geometry, report-document, optimization-study, viewer, and modal guards. It should not claim type-3 study completion, ROI payload import, or Report/ShapeOptim Save completion.

## Evidence scope

Read-only source review of `src/core/convert.cpp`, `src/engine/loader.cpp`, `src/engine/save_project.cpp`, `src/engine/save_current.cpp`, `src/engine/shapeoptim_load.cpp`, and `src/ops/report.cpp`, together with the retained CP120 ordering report and CP51 ShapeOptim property-copy report. No source, test, build, GUI, original execution, or mutable REA state was changed. The conclusion is source-level ordering only; it does not claim a successful native directory creation result on any retained run.
