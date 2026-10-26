# CP60 — next inherited property dispatch candidate

## Result

The next source-qualified property boundary outside PPV is the HeatTransferExpress step-data subslot, `SfOprHeatTransferExpressProperties` vt+0x70 (`0x1413ed710`), reached from operation props vt+0x60 `0x1413ed900`. The six-field conversion itself is already present; the remaining guarded case is a missing `HeatTransferExpressProps::step`. Static evidence does **not** support treating that null case as a valid omitted conversion to implement. The original body dereferences the step block whenever a SimCtrl exists.

This is a boundary finding rather than a proven user-visible regression: the normal property factory always creates the step object, so an operation that has valid factory-created properties should not enter the missing-step branch.

## Evidence and current dispatch

- Native dispatch table: `source/docs/re/moproj2mst/s/spec.md`, section 3, rows for HeatTransfer/HTExpress. It identifies props vt+0x60 `0x1413ed900`, common F80/F90/FA0 handling, followed by own vt+0x70 `0x1413ed710`. The slot description gives the target fields: SimCtrl `+0x1c8` (step definition plus one), `+0x1d0`, `+0x270`, `+0x2a0`, `+0x2a8`, and `+0x164`.
- Native decompile: `evidence/gui_mo/out_s/s9.c`, `FUN_1413ed710`. With non-null `param_2` (the SimCtrl), it unconditionally dereferences `param_1+0x58` (properties step block) and copies the six values. It has no null-step fallback. With null SimCtrl it returns false before dereferencing the step block.
- Property construction: `source/apps/moproj2mst/src/model/htexpress_props.cpp`, `heatTransferExpressProps`, creates `std::make_shared<data::HeatTransferStepData>()` before building the property tree; the step object corresponds to props `+0x60` per MDT L3 section 5.
- Native property load routing: `source/apps/moproj2mst/src/ops/family.cpp`, `HeatTransferExpressClass::propsLoad`, routes to L3 `htePropsLoad`. That function invokes the three shared property readers then calls `hteStepDataFromSimCtl` only when an owner exists.
- Current Save dispatch: `source/apps/moproj2mst/src/engine/save_current.cpp`, `propsToEngine`, routes HeatTransferExpress and 2D through the HT common property branch, then `hteStepDataToSimCtl` when the concrete props type is `HeatTransferExpressProps`. It throws unsupported if `step` is null. The implementation in `source/apps/moproj2mst/src/engine/save_l3.cpp` copies the six documented fields and checks null SimCtrl before dereferencing `step`.
- Operation Save reaches props vt+0x60 through `saveCurrentOperation` in `save_current.cpp`; with no property widget the base slot selects the props-to-engine path. The call is performed twice in the native index-zero path, and the port preserves that ordering.

## Settled versus still open

Settled: the valid-data mapping and ordering are implemented, the native target fields are source-bound, and the normal factory initializes the step pointer.

Open: whether any supported import, mutation, or malformed-project path can retain a `HeatTransferExpressProps` object with `step == nullptr` while its operation has a SimCtrl. The native path would dereference in that state; the current explicit unsupported guard is safer than inventing defaults. No evidence found in this read-only pass proves such a reachable state is a valid operation property branch.

## Smallest safe next contract

Do not remove the null-step guard based on an inferred default. If a later source path demonstrates that null is reachable for a valid operation, first repair/qualify that constructor or ownership path so a `HeatTransferStepData` object exists, then exercise the existing six-field mapping. Otherwise keep the guard as an unsupported-state boundary.

Acceptance for the next implementation increment should be a focused Release-active property test with non-default values for all six fields, verifying props-to-SimCtrl and SimCtrl-to-props round trips for both HeatTransferExpress and HeatTransferExpress2D, plus a null-SimCtrl case (no mutation, false result). Keep a null-step/non-null-SimCtrl case asserting the explicit unsupported result; it must not silently fabricate native values. Do not widen any public Save/Open guard until a reachable owner path is established.
