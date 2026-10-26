# CP62 — property-dispatch reachability after the HTE null-step audit

## HTE null-step conclusion

The `HeatTransferExpressProps::step == nullptr` branch is not established as a valid loaded state. The only production construction site found is `tree::heatTransferExpressProps` in `src/model/htexpress_props.cpp`; it allocates `data::HeatTransferStepData` before returning the object. The class is routed through that property factory, and the inspected load/save code has no reset, reassignment-to-null, or alternate HTE property constructor. The loader's `htePropsLoad` dynamically checks the property class and, with an owner, writes engine SimCtrl values into the already-owned `step` block. The Save conversion's explicit null guard therefore covers malformed/manually assembled state; removing it is not source-supported. This confirms the CP60 guard should remain.

## One still-unreachable operation property branch: Report

A concrete remaining property-dispatch reachability gap is `SfOprReport`'s property-to-engine callback during normal operation Save.

Native contract:

- `source/docs/re/moproj2mst/s/spec.md`, section 3, identifies Report props vt+0x60 as `0x1412b1f50`. It calls the common F80, F90, and FB0 helpers (`0x1405fb980`, `0x1405fbd10`, `0x1405fc5d0`). The retained `evidence/gui_mo/out_s/s9.c` decompile for `FUN_1412b1f50` confirms those three calls and a true return.
- Native generic Save reaches props vt+0x60 through `0x140052ec0` -> base `0x1405ef510` -> `0x1400543a0` when the property-widget index is zero; the dispatch and order are recorded in `source/docs/re/moproj2mst/s/spec.md`, section 1.
- Report's own Save vt+0x68 is `0x1412adf90`; MDT L7 section 4 qualifies it as running Report vt+0x80 `0x1412aeb30`, then `0x1412af0c0`. The latter reaches viewer models. The report vt+0x78 path earlier reads the subproject report document from SP+0x30 (`<project>.DS`).

Current port boundary:

- Generic `propsToEngine` in `src/engine/save_current.cpp` already maps `SfOprReport` to F80/F90/B0 and writes the resulting SimCtrl values. The property helper is therefore implemented.
- `ReportClass::loadClassData` in `src/ops/report.cpp` explicitly refuses the `<project>.DS` report-document path when a project/subproject exists; `loadOperation` refuses the remaining viewer/UI steps. Independently, `saveCurrentOperation` refuses `SfOprReport` at its early vt+0x68 guard before the generic property dispatcher. This means a Report operation cannot currently reach the otherwise implemented property mapping through the supported Open/Save lifecycle.

This is an operation-path gap, not missing F80/F90/FB0 arithmetic. Enabling the generic property call alone would leave Report's native Save tail (report document and viewer effects) unmodeled and would not match the native dispatch order.

## Smallest implementable contract and acceptance

Keep both Report refusals in place. The next safe increment must first establish the owning report-document state for SP+0x30 and the needed viewer/model behavior through bounded native evidence and a model representation. Then bind the existing Report property route at the native position and preserve the three-helper order. The narrow acceptance case should start from a Report operation with a valid `.DS` document and loaded SimCtrl, run the supported operation Save path, and verify the exact F80/F90/FB0 target fields plus the required report/viewer side effects. Until those prerequisites are modeled, a direct unit call to generic `propsToEngine` is only a helper test and cannot justify relaxing the public Save/Open guards.

No other non-PPV `props vt+0x60` family row in the S dispatch table was selected as omitted in this pass: the remaining rows are either implemented (including Convert2Dto3D/CopyMirror and ShapeOptim) or have their own model/owner guards. The Report route is the narrowest concrete branch where the property body is known but real caller reachability remains blocked by a separately evidenced owner dependency.
