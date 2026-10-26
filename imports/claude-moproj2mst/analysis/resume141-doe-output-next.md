# CP141: active DOE Output Save next cut

## Finding

The candidate's `SfOprDoeOutput` Save guard in `engine/save_current.cpp` is reached before the Project-file writer: `engine/save_project.cpp` calls `saveCurrentOperation` and only then calls `moproj::saveProjectFile`. The native current-operation slot `0x140e2e730` therefore precedes Type2 InputTable/`DoeTableRefresh` and the later main-MST/`DEF_DOE.INI` outputs. Neither CP131's private notifier result nor the private INI Constraint plan supplies this earlier operation owner.

## Smallest isolated ordered effect

The first native DoeOutput effect is the immediate `Control/Regions` `QDir::mkpath` request at `0x140e33210 -> 0x14081afb0(path,true)`. Unlike the subsequent cleanup and geometry branches, this request precedes the current model check. A bounded private prefix can be source-qualified using the already-owned Type2 Study only:

1. Validate `Project` → active `SubProject` → `ownedDoeStudy`, `sub.doe == ownedDoeStudy.get()`, `Study.sub == &sub`, and `sub.project == &Project`; require the Study name used by the original path.
2. Derive the Control directory with the existing `mst::doeControlDir(sub, &study.name)` and append `Regions` with Qt separator/clean-path rules.
3. Perform the reached immediate `qt::nativeMkpathAbsolute(path)` attempt and retain its Boolean at the call position. Do not defer it in `FileEffects.mkdirs` and later lose it when a following guard fires.
4. On success, return only a private `DirectoryAttemptReturned` boundary. On failure, retain the attempt and stop at `WarningModalRequired`: original helper shows a one-OK `QMessageBox::exec()` and then returns true. This helper/report does not establish modal dismissal or authorize resuming later effects.

This is a genuinely disjoint private prefix because it needs the Study/path owner but not the viewer model, output document, Constraint writer, or notifier. It does **not** close current-operation Save. Also do not substitute `StudyIo::ensureDirectory`: its candidate contract performs a prior existence probe and uses `std::filesystem`, whereas the retained shared native helper directly attempts `QDir::mkpath`; failure warning/continuation is a separate modal boundary. `FileEffects.directoryAttempts` plus `nativeMkpathAbsolute` is a closer existing callsite pattern, though its current generic helper does not deliver this warning dialog.

## First blocker after that prefix

On the native continuation, model-dependent cleanup begins: if a model is present, enumerate `Control/Regions`, skip dot entries, recursively remove child directories and remove regular files (individual removal results ignored), retain the top directory, then independently remove both Common_Mesh files. Only later does ROI selection plus a nonnull cached common-mesh object gate geometry writing, followed by the current-operation helper and `.DS` writer.

The missing public state is the **same-occurrence output context**: actual current viewer `GuiModel`/ROI model and cached common-mesh object, plus the active SUB's live `Study.outputDocument` containing its selected `SfTaskDoe` and manager-linked values. `output_data.cpp::finishCurrentOutputClassData` is a load-side state prefix and cannot provide this Save state. On retained B120 the earlier `.DS` `DBLIST` child stops candidate class-data loading before that helper, so an empty/unit `SessionDocument` is not a justified substitute. After cleanup, `.DS` generation/update and `0x140645030`'s immediate UTF-8 QFile open/write/close/error path also need actual owners and ordered failure effects.

## Separate later branches

The native Type2 `DEF_DOE.INI` writer at `0x1400ba800` is after main-MST/template/parser work and accepts a different manager/design/task context. The current Constraint extension is only a buffered `FileEffects` value plan, with no production callsite or actual QFile/TextStream failure behavior. Type3's separate `0x140907a10` `.OPT` output is not the `.DS` or `DEF_DOE.INI` branch. Keep all three behind their own earlier owners and ordering.

## Read-only source receipts

- `engine/save_project.cpp`: `90A0DDBF6B4C92B4F4FE8BC106AA2E9248F31C84D2A0981D0EE10EDA0BAF2337`.
- `engine/save_current.cpp`: `BA944DADE590FD73CBE16C95A2A83D7F50868B2F1B05370BD477F7611D7F9812`.
- `doe/session.cpp`: `C57ADD172AE9B94C4AC6B731BD6E66AC7CF0886A5004F92C8886E056CDBEDEAC`.
- `qt4/native_directories.cpp`: `48BA21411D091EC7C4139DE5DCBEB7DC4FB18A4C712BF62A3D5D572BFC84F2EC`.
- `mst/doe.cpp`: `A8CB15E78F8955A41B58C1D5DFACE2193A5B645A931D380E71517682D39A872D`.
- Original order/path/failure receipt: `analysis/wave2-doe-lifecycle.md` and `analysis/resume122-shapeoptim-mkpath-original.md` (the latter establishes the shared `0x14081afb0` modal contract; ShapeOptim's callsite is distinct).

No source, tests, build, GUI, original application or mutable REA state was changed.
