# B120 post-state as a read-only input model

## Retained input identity

Use the retained post-GUI B120 project as an already-existing input state, without replaying or asserting the GUI study-creation action. Its `.moproj` is 34,774 bytes, SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`. It contains subproject 0 (Type 0, `COG_P1_B1`) and subproject 1 (Type 2, `DOE Study 1`, `Active="5"`, `Id="1"`) with `<SfDoeStudy name="DoeStudy1"/>`. The retained seven `DoeStudies/DoeStudy1/Control` files and their hashes are in `analysis/wave2-doe-evidence.json`.

This is a useful existing-study input for a fresh Open–Save–Close comparison, unlike the `_before` pair. The original UI mutation is historical provenance only; the comparison begins from the same copied post-state folder for both programs.

## Branches reached by the current converter on this input

The current `core/convert.cpp` still selects `activateSubProject(p, 0)` unconditionally. During `openProject`/`walkProject`, the converter records subproject types/ids and constructs the second `SubProject`; `makeSubProject` creates an owned DOE Study manager for Type 2. `parseSubProjectDbName` reads the second subproject's DB name only. The current driver does not activate index 1, so it does **not** call `parseSubProject` for that project's `SfDoeStudy`, does not call `readLazyStudy`, does not parse the retained DoeInputTable/sample files, and does not load its DoeOutput active item. Its `Active="5"` attribute is read-and-ignored in the nonactive DB-name pass.

The driver then activates subproject 0 and follows the main Type 0 project through Open and Save. That run can test whether ordinary Save preserves inactive second-subproject XML and existing DOE files in a copied tree. The DOE manager constructor may ensure the study directories exist; all seven files are already present, and the current code has no call on this path that regenerates them. A passing byte comparison therefore establishes inactive-state preservation for this selected path only. It does not exercise DOE parsing, Task/StepData loading, DOE Output Save, `.MST.TEMP`, or DOE writer output.

## First guard if the Type 2 operation is made current

The post-state Type 2 subproject's active item is `SfOprDoeOutput` (the retained project records it at `ClassName` under the Type 2 diagram). If a future comparison genuinely selects that subproject, `activateSubProject` first calls `parseSubProject`. The present code does have a Type 2 `SfDoeStudy` branch: it validates the owned-study/I/O binding and calls `doe::readLazyStudy` rather than refusing the child. This corrects the overly broad earlier note that labeled `SfDoeStudy` itself the first parse guard.

The current first known DOE Output guard on this fixture is later in `openActiveOperation`: the DoeOutput class resets/shows its model, then reads the `.DS` session document. The retained `COG_P1_B1.DS` has an `Image` child. `importSession` reaches its explicit QImage PNG decoding refusal at the first Image, after the bounded qUncompress/diagnostic prefix. It stops before task factory construction, StepData import, selected-session application, and the operation's Save slot. If a fixture changes/removes that Image, the native TaskFactory has the separately qualified first Report QObject guard. If Open is completed, the Save path has another explicit DoeOutput vt+0x68 guard for sample-object/document writes. These boundaries remain distinct.

The converter's public path offers no input argument to change the selected subproject: `convert.cpp` sets index 0. Therefore an exact post-state comparison through the existing ABI is specifically a main-project Open–Save preservation comparison. Do not silently edit the `.moproj` to reorder its subprojects and call that the same input. If Type 2 activation is the research target, define a separate fixture/driver-selection experiment and record the changed starting state as part of that experiment.

## One disposable comparison

Create a read-only source snapshot from `evidence/gui_mo/oracle_b/120_B_DOE` and clone it independently to two scratch roots, for example `build/reference-harness/b120-poststate-original/` and `build/reference-harness/b120-poststate-candidate/`. Verify the `.moproj` and seven side-file source hashes against `wave2-doe-evidence.json` before either run. Then:

1. Run the fresh original application on the original-copy project with the agreed default dialog answers, File > Open, File > Save, and Close. Record process outcome and a complete before/after relative-path inventory plus byte hashes; retain failures without treating a crash as a pass.
2. Run the frozen candidate on the separate candidate-copy project through `WRITE_SAVE`, with matching DEFORM/runtime settings. Record status and the same inventory. Do not use B120 `_before`, do not create a new study, and do not activate study execution.
3. Compare `.moproj` and all seven side files independently. Annotate whether the selected path was subproject 0; if so, the result is only preservation/ordinary Save evidence. If the original GUI starts with another selected subproject by persisted/session state, capture that observation before interpreting the run.

Keep both copies under the converter's build scratch. Do not operate on the retained source evidence. The original action begins with the stored post-GUI state, so no UI interaction is needed for this measurement.

## Evidence limits

The source pair is a historical GUI state, not a fresh original runtime capture of Save-on-this-state. Current candidate reachability is established from source call order. The expected saved side-file hashes do not establish what a new Save will do. No original/candidate run or mutation was performed for this analysis.
