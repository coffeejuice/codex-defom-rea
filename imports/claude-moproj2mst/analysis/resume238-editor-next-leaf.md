# CP238: next Editor300 ownership leaf after CP236

## Finding

CP232 frequency-text access, CP235 `pageState()` control identity, and CP236 lifecycle entry/tail identity checks are already implemented in the current source. The next smallest remaining editor callback that exposes a direct control alias is `StagedEditor300::toggleNstart` in `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp` (around line 266, native annotation `0x140ecf910`). It stores `Operation::nstartGiven`, then obtains `binding.model.simCtl()`, checks the cached source-control address, and passes the raw control reference to `fillStepCount` to produce a copied `StepCountPage`.

This is a real, bounded alias consumer, but it is not yet a safe one-command `GuiModel` leaf. A command returning/refilling the page must preserve the native-order partial prefix and the checked-state history while respecting type ownership: `StepCountPage` and `StepCountContext` live in `m2m/engine/simcontrol300.hpp`, while the shared `GuiModel` interface is in `engine.hpp` (included by the editor declarations). A thin wrapper returning or accepting a `SimControl*` would only move the alias, and a command that accepts an already-built page would leave all current-SC reads in the caller. The next implementation therefore depends on a reviewed value boundary for the StepCount input/page projection, or on a separately owned editor-domain command whose contract explicitly owns that short-lived model borrow. Do not duplicate `fillStepCount` or add a generic callback merely to avoid resolving that type/API boundary.

## Required source order and why a shortcut changes behavior

`toggleNstart` retains these source effects:

1. Check Project/OPI/model/remesh identity, completed page state, mode 5 and matching context operation, and cached checkbox delivery.
2. Immediately store `binding.operation.nstartGiven = checked ? 1 : 0`; report stage becomes `OperationFlagStored`.
3. Only then call `model.simCtl()` (geometry barrier before nullable lookup) and compare against `pages_->sourceControl`.
4. On absent/replaced control, return unsupported at `OperationFlagStored`; keep the operation flag store and leave all cached page controls unchanged.
5. On a matching control, compute a fresh `StepCountPage` using current native `nstart`, `nstep`, `stpinc`, `pdie1`, current MO context and the prior checkbox state; only after successful fill replace the page and report completion.

The legacy `fillStepCount(const SimControl&, const StepCountContext&, bool priorNstartChecked)` also preserves its qualified unsigned ADD/NEG calculation, operation override, primary-die selection, clamping, and hidden-checkbox history. Replacing it with a fresh two-argument fill, a snapshot that defaults the previous checkbox, or an Apply/save route changes delivered-toggle behavior. Do not move the control lookup before the `nstartGiven` store or roll that store back on a null/replaced control.

## Next test seam and provenance

If the StepCount value boundary is resolved, the focused consumer/test seam is `source/apps/moproj2mst/tests/mdt/test_simcontrol300_nstart_toggle.cpp`, target `m2m_mdt_simcontrol300_nstart_toggle`. It already tests operation-flag-before-missing-control refusal, distinct replaced-step refusal while retaining the original allocation, all-control refill, repeated delivery, wrap/clamps, starts-from-key checkbox history, nonwriting/missing-class behavior, wrong context/event/binding, and partial/expired owner gates. Add command-level coverage for the value-only result and confirm page state changes only after a completed projection. Keep `m2m.mdt.simcontrol300_stopping_cached` and the parked geometry regression in root’s integration set.

Native qualification is retained in `analysis/resume132-editor-next.md` and `analysis/resume133-editor-nstart-toggle.md`: original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; callback body `[0x140ecf910, 0x140ecf977)`, operation-flag setter `[0x14005a2c0, 0x14005a2d7)`, refill `[0x140ebb110, 0x140ebb3dd)`, and current-SC getter/provider spans and hashes are recorded there. The bounded private candidate does not establish physical checkbox signal blocking, actual Qt delivery/reentrancy, native model enumeration, or public editor/Save completion.

The other remaining obvious callback, `reactionRateClicked` (`0x140eccf10`), is more coupled: cached widget projection precedes SC lookup; live curve identity checks precede owner-index mutation; control `acvcof` writes precede a temporary-curve write. Splitting those across an owner command without a coordinated value/result protocol would move the early widget effects or reorder the owner/control/temporary stores. It is not a smaller independent leaf than NSTART.

## Guardrails and scope

CP222’s alias ledger lists the callback ingress families; CP232/235/236 remove the already-migrated pieces without closing the remaining raw routes. The NSTART proposal is not a cache1/Stopping change. Preserve `fillStopping3D` behavior and the explicit cached-Stopping refusal exactly; do not imply that this editor callback authorizes a continuation. No one-file migration here proves alias closure.

This is a read-only selection/blocker report. No source, test, build, shared recovery state, original process, GUI, or REA state was changed.
