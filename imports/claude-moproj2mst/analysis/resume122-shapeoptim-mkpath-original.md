# CP122 original ShapeOptim `studyFolder` mkpath contract

## Finding

Original ShapeOptim load calls the shared directory helper immediately while building the optimization study folder, before its later ROI `.GEO` filename guard. The call passes `dialogs=true`. Successful `QDir::mkpath` skips the dialog; failed `mkpath` with that flag enters a modal one-OK QMessageBox. The dialog result is ignored and the helper returns true, so folder creation failure does not abort `studyFolder` or `loadData`. Candidate integration must preserve that continuation and earlier timing; a deferred `FileEffects` mkdir applied only after `loadData` succeeds does not.

## Native call timing and path

Original `DEF_GUI_MO_64.exe` SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

Bounded DUMPBIN inspection of `0x14013cbe0..0x14013d2b0` and `0x14013d285..0x14013d650` covers the complete selected `ShapeOptimClass::loadData` body (`0x14013cbe0..0x14013d174`) and folder helper (`0x14013d180..0x14013d645`). In `loadData`, the native order is: require `current`; get its operation model (null returns); resolve Project and active SUB (missing returns); call `studyFolder` at `0x14013cc66`; then select the ROI model and inspect its stored geometry-name list. The nonempty-list route is later and may fail; it does not undo the already-called directory helper.

The `studyFolder` body resolves the Project folder and active SUB, obtains the DOE study name through the SUB's DOE study object/name path, and assembles the path from the Project folder plus `DoeStudies`, study name, `Control`, and `Opt`, with Qt separators and a trailing separator. The helper call is `0x14013d5e8 -> 0x14081afb0`; immediately before it, `0x14013d5e1` loads `dl=1`, so warning/dialog behavior is enabled. The native call is made on the actual assembled QString at this point, not recorded for later effect application. The helper sets its QDir path to the supplied target and calls `QDir::mkpath(target)`; the corrected receipt explicitly rules out a separate `QDir::exists` check in this helper. Static evidence establishes the component order, not a per-run resolved path string. It also does not show an independent absolute-path validation before the QDir call; the path is rooted in the project-folder value.

## Success, failure, and continuation

The complete helper body `0x14081afb0..0x14081b2a8` stores the `mkpath` Boolean, tests it, and branches directly to its common true-return tail when successful. When it fails, the Boolean controls the warning branch; the caller-supplied flag `1` enables it. The failed branch builds a QMessageBox with icon value `1`, button value `0x400` (the single OK button), and null parent, then calls `QDialog::exec()` at `0x14081b268`. It discards the exec result, destroys the box, and reaches the common tail that writes Boolean true before returning. Thus:

| Native result | UI/error route | Effect on ShapeOptim continuation |
| --- | --- | --- |
| `mkpath == true` | No failure dialog | Helper returns true; `studyFolder` returns the folder string and `loadData` continues. |
| `mkpath == false`, `dialogs == true` (the ShapeOptim call) | Modal one-OK QMessageBox; exec result ignored | After the dialog ends, helper still returns true; `studyFolder`/`loadData` continue to the next native branch. |
| `mkpath == false`, `dialogs == false` | No dialog | Helper still returns true. This is not the ShapeOptim call profile. |

This is not a QFile error return and not an abort-on-mkdir-failure policy. If `loadData` later reaches unsupported stored ROI geometry, that later refusal remains the next failure; the earlier directory attempt and any dialog interval are already past. The static QMessageBox route proves a nested modal `exec` opportunity, not which event, user action, or elapsed time ends a real dialog. Retained dialog analysis shows a one-OK construction and event-loop dispatch, but has no event schedule for a ShapeOptim occurrence.

## Candidate boundary

The retained candidate source currently appends this path to `fx.mkdirs`; the outer Open caller applies effects only after `openActiveOperation` returns. A later geometry-list refusal prevents that application point, losing an effect that native code already attempted. The source also still refuses when the active SUB has a DOE study whose name is needed to construct this path; consequently, the retained candidate cannot yet reach the named-study case represented by the native path. Do not remove that guard merely to make the directory attempt run. Integrate only after the actual study-name owner is represented, by performing the reached `mkpath` attempt at the `studyFolder` call boundary and preserving its source-qualified false-result continuation. Keep the later `.GEO` payload, type-3 optimization-study, and modal event-delivery boundaries separate.

## Evidence and limits

- Original binary identity: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 above. Fresh execution was not performed.
- ShapeOptim call and path source: `src/engine/shapeoptim_load.cpp`, SHA-256 `228C67E1A545228593379794778E5643235C41B4C7A6301871F81BE9F4427A0C` (source annotations were checked against the bounded native call sequence).
- Shared helper capture: `analysis/wave3-earlier-save-14081afb0.asm`, SHA-256 `A9DF7D8C1F7B7E73F7070A4361E60726427035C3631B5505577F9716395AD156`.
- Corrected helper contract: `analysis/wave3-earlier-save-mkdir-contract-correction.json`, SHA-256 `E1CF5CD05222646A48268EC257A4667CA85BF8AA2B31826CD582126F411A9BCE`.
- Modal/button qualification: `analysis/wave3-directory-failure-dialog-contract.json`, SHA-256 `4111E716CC5D5482433F33C6330A63EC4A9F618B592CD3A335B7F88887BF8EC4`; this confirms construction/default behavior but explicitly leaves actual modal event delivery unknown.
- Candidate ordering frontier: `analysis/resume122-doe-report-owner-frontier.md`, SHA-256 `1DD47F99CAB5FD75F08D4E9B96A09C40ADC51A68341CFC90A58423A64FA30113`.

Read-only source/PE inspection only. No source edits, builds, GUI/original execution, or mutable REA operation were performed.
