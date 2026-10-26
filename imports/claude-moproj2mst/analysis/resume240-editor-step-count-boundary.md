# CP240: model-owned StepCount projection boundary for NSTART

## Resolved boundary

CP238’s apparent `engine.hpp` / editor-page type cycle is avoidable without making `GuiModel` depend on `std::optional<StepCountPage>` as a return type. Use forward-declared page/context types and an output reference:

```cpp
namespace m2m::engine::simcontrol300 {
struct StepCountContext;
struct StepCountPage;
}

// In GuiModel:
bool refillEditorStepCountIfControlMatches(
    const def::model::Cls_1401e7b10* expected,
    const simcontrol300::StepCountContext& context,
    bool priorNstartChecked,
    simcontrol300::StepCountPage& result) const;
```

`StepCountContext` and `StepCountPage` are already defined by `m2m/engine/simcontrol300.hpp`. The method declaration needs only references to incomplete types, so `engine.hpp` need not include that editor header and does not create the header cycle. `gui_model.cpp` can include the complete editor header to call the existing `fillStepCount` overload and assign its result to the output page. Using an output reference avoids instantiating `std::optional<StepCountPage>` where the page is incomplete.

## Required model method behavior

Implement the method as one short-lived, model-owned current-control borrow:

1. Call `simCtl()` exactly once. This preserves its single `requireGeometrySyncAvailable()` check before nullable control lookup. Do not precheck `expected` for null before this call.
2. Return `false` if the current control is null or its address differs from `expected`. This includes the both-null case; current control must exist. Compare but never dereference `expected`.
3. Call the existing `fillStepCount(*control, context, priorNstartChecked)` synchronously while the control remains local to the method. Do not duplicate its computation or convert to the fresh-fill two-argument overload.
4. If the optional result is empty, return `false` without changing `result`. Otherwise assign the completed `StepCountPage` to `result` and return `true`.

Do not catch geometry or fill exceptions. Do not expose a pointer, reference, view, callback, or control value derived from `SimControl`. The returned page is value-owned for this operation, but it is not wholly pointer-free: `StepCountPage::mo` intentionally retains the existing `Operation*` in `MoStepCountContext`. This leaf confines the native control alias; it does not alter or certify OPI ownership.

## NSTART caller ordering

In `StagedEditor300::toggleNstart`, retain all existing binding, event, page/context, and cached-checkbox guards. Keep this exact effect sequence:

1. Store `binding.operation.nstartGiven` and commit report stage/flag `OperationFlagStored`.
2. Initialize a local `StepCountPage` output value and call `binding.model.refillEditorStepCountIfControlMatches(pages_->sourceControl, currentContext, checked, refill)`.
3. On ordinary `false`, return the existing unsupported report with `OperationFlagStored` retained. The cached page remains untouched.
4. On `true`, move the completed page into `pages_->pages.stepCount`, then report complete/refilled/completed.

A parked geometry model throws from `simCtl()` at step 2; the operation flag therefore remains stored, the exception propagates, and the cached page remains untouched. Do not call `matchesCurrentSimControl()` before the operation-flag store or use it as a separate preflight; either would move the geometry/current-control query ahead of its original point. The model command must use the cached checkbox delivered value as `priorNstartChecked`; key/null-operation branches preserve that prior history while non-key branches keep the established class/operation rules.

## Exact implementation files and tests

- `source/apps/moproj2mst/include/m2m/engine.hpp`: forward declare `simcontrol300::StepCountContext` and `StepCountPage`; add the method declaration with reference parameters only.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: include `m2m/engine/simcontrol300.hpp`; implement the local `simCtl()` borrow, identity refusal and synchronous existing fill call.
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: replace the local `simCtl()` pointer and direct `fillStepCount` use with the command, keeping flag store and page assignment points.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_nstart_toggle.cpp`: add direct command checks for matching/mismatched and missing control, output unchanged on refusal, nonempty output on success, and prior checkbox history through starts-from-key versus non-key cases. Existing caller tests remain evidence for operation-flag-before-missing/replaced-control prefix, whole-page replacement, repeat delivery, wraps/clamps, classes, context, and binding refusal.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: add a valid-context parked call using expected `nullptr` and the existing `refused` helper. It must observe `Error(M2M_E_UNSUPPORTED)`, establishing the geometry guard precedes null/current-control rejection. Keep the existing stopping-cached regression in root’s integration group.

This keeps the editor page types in their current header and requires no new CMake target or C ABI change. Coordinate common header/implementation edits with their assigned owner. Focused targets are `m2m_mdt_simcontrol300_nstart_toggle` and `m2m_mdt_viewer_box`; root owns build/CTest and freeze sequencing.

## Source and original evidence limits

The reconstructed source is `StagedEditor300::toggleNstart` plus `fillStepCount`’s three-argument overload in `src/engine/simcontrol300.cpp`. CP238’s independent review confirms the existing partial prefix, geometry guard point, control mismatch policy, and prior-checkbox behavior. CP132/CP133 retain original DEF_GUI_MO_64 v11.0.2 identity and bounded evidence for the NSTART callback `[0x140ecf910, 0x140ecf977)`, operation flag setter `[0x14005a2c0, 0x14005a2d7)`, refill `[0x140ebb110, 0x140ebb3dd)`, and current-control getter/provider. Those records support store-before-SC/refill ordering and checkbox history; they do not establish a fresh original execution, physical Qt signal behavior, reentrancy, native combo enumeration, or public editor/Save completion. This extraction does not alter Stopping/cache1 or authorize continuation.

Read-only design only: no source, test, build, shared recovery state, original process, GUI, or REA state was changed.
