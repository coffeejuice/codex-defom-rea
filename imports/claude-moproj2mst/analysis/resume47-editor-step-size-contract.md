# Editor300 step-size scalar contract (checkpoint 47)

## Contract recovered from source-bound receipts

The original executable identity for the saved step-size analysis is DEF_GUI_MO_64.exe SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `wave2-save-diffs-300-step-stop-increment.json` qualifies the recovered fill/apply behavior to 3D, KnownC locale, constant DTTYPE with `type=0x80`, `ndim=0`, no owned object/table pointers/counts, `ncomp` 0 or 1, accepted mode/STPDEF ranges, and finite filled scalars. Tabulated curves, 2D, and unrecovered curve-form callbacks are explicitly excluded.

For the narrow time-maximum case:

- Fill obtains the display scalar from model `DTMAX1` using the recovered Qt general-format, precision-6 seam. It also normalizes `DTTYPE.u58` through the original fill callback before the rest of constructor refill; step page state is temporary and is later restored from the constructor backup.
- Apply with `modeRadio=1` selects the time branch. It stores normalized `DTTYPE.u58=1`, parses `timeMaximum` into `DTMAX1`, then writes `DSMAX=0.0`. It subsequently applies other step-size fields in recovered order. An `Invalid` field stops at that point and retains all prior writes; a valid scalar alone does not establish completion if a later page field is invalid.
- The constructor refresh order is part of the state contract: StepSize is refilled once after the mode callback and again in the backed-up refresh; the final page curve is copied before the refresh and then assigned to the page snapshot, while the model is restored from the backup afterward. Tests of the final editor snapshot must go through `openInitialEditorInto`/`openInitialEditor`, not assume the first fill is final.

## Enable-state disposition

There is no supported effective-enable contract for the step-size scalar widget in the current receipts. The recovered page value object (`StepSizePage`) has mode/definition selectors and validated text values, but no enabled flags. The saved setup/remesh radio contract resolves only two unrelated remesh radio groups and expressly distinguishes requested flags from effective parent-tree state. Therefore this audit does not claim that `timeMaximum` is enabled for `modeRadio=1`, disabled for another mode, or that native callbacks/parent state have been reproduced. Those would require source-qualified widget identity, signal/slot or setter evidence for the step-size page controls, and the runtime/parent-tree effects through final setup.

Current owner wiring places `StepSize` in the initial editor page state and applies it after General and StepCount, before Stopping/Meshing/Iteration/Environment/Advanced. `applyInitialEditorPageState` first verifies complete page state and exact model/control/remesh-owner identity. Any focused test of one isolated scalar should call `fillStepSize3D` and `applyStepSize3D` directly; a lifecycle-level test requires every later page's dependencies and would not isolate this scalar.

## Exact focused state test

Add or retain a single direct helper test with this precise arrangement:

1. Construct a `SimControl` whose DTTYPE and STPDEF satisfy the receipt guards and whose step fields are finite; select `DTTYPE.u58=1`, `STPDEF=2`, and `DTMAX1=1.2345678` (the rest of the step-size fields use valid distinguishable finite sentinels). Use `GuiModel` dimension 3 and `NumericLocale::KnownC`.
2. Call `fillStepSize3D`; require success, `modeRadio==1`, and `timeMaximum.text` equal to the recovered `g6` rendering (`"1.23457"`). This is an explicit isolated helper test, not a proof of the complete constructor's final state.
3. Change only `timeMaximum` to `{qt::L("8.25"), ValidatorState::Acceptable}` and call `applyStepSize3D` once.
4. Require `Completed`, `DTTYPE.u58==1`, `DTMAX1==8.25`, `DSMAX==0.0`, and unchanged distinguishable values for fields not implicated by the selected branch, except fields the recovered later apply-order writes from their unchanged snapshots. In particular assert `STPDEF==2`, `DEMAX`, `DPLEN`, `DTSUB`, all three maximum-change values, and `SLDERR` equal their filled values; verify the DTTYPE curve payload remains the same and its mode byte remains 1.
5. In the same test, do not assert a QWidget enabled bit. The value contract concerns the selected branch and ordered model writes only.

A separate exact prefix test can mark a later field (for example `strainMaximum`) `Invalid` after changing `timeMaximum` to `8.25`; expected result is `ValidationStopped`, with `DTMAX1==8.25`, `DSMAX==0`, normalized mode retained, and all fields after DEMAX unchanged. This covers the receipt's partial-write rule without combining another GUI setting.

## Evidence ceiling

The original receipts are static native decompiles plus isolated helper/test contracts; the worker recorded no runtime execution for these tests, and public editor300 Save is still guarded. Original probes 003/004 stopped at first editor-grid publication, had current user settings in effect, and performed no GUI input, event pump, Qt calls, or target-memory assignment. They do not establish default `DTMAX1`, actual text after final refresh, effective enablement, a user edit, or post-Apply UI behavior. The test above is a source-derived focused contract for implementation validation, not original-runtime parity evidence and not proof of complete editor lifecycle behavior.

## Read-only basis

`analysis/wave2-save-diffs-300-step-stop-increment.json`; `analysis/wave3-editor-setup-remesh-radio-contract.json`; `analysis/wave3-original-editor-probe-003-receipt.json`; `analysis/wave3-original-editor-probe-004-receipt.json`; current `src/engine/simcontrol300.cpp`, `src/engine/simcontrol300_lifecycle.cpp`, `include/m2m/engine/simcontrol300.hpp`, and `src/engine/editor_setup.cpp`. No source/build/app-control/REA mutations were performed.
