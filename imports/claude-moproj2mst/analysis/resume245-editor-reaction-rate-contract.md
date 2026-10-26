# CP245: finite reaction-rate coordinator contract

## Selected boundary and ownership

Place a **private, named synchronous coordinator as a `StagedEditor300` private member**, declared in `simcontrol300_owner.hpp` and implemented in `simcontrol300_owner.cpp`, rather than adding this cross-domain editor command to the general `GuiModel` API or exporting a new namespace function. `StagedEditor300` already owns the completed `InitialEditorSession` through `pages_`; that session owns `EnvironmentCurveTemporaries`. The coordinator can use those exact owners synchronously without a new lifetime or include dependency.

Proposed finite signature:

```cpp
enum class ReactionRateCommit {
    MissingOrReplacedControl,
    LiveCurveAlias,
    Complete
};

ReactionRateCommit commitReactionRateSelection(GuiModel& model, int selection);
```

Keep the nested result type and method private to `StagedEditor300`; do not expose a new namespace API, general model primitive, or C ABI. The model and selection are the only explicit inputs. The method reads `pages_->sourceControl` as an opaque identity token and accesses `pages_->environmentCurves` directly. Require it only be called after the existing callback preflight and cached widget projection. It is synchronous and non-retaining: no derived pointer/reference is stored, queued, returned, or passed to a callback. Its only raw control borrow is local to the method.

This placement gives one named operation authority over the already-coupled domain suffix while leaving general `GuiModel` independent of editor page-owner types. It is more than a forwarding wrapper: it performs exactly one fresh model admission/identity check, live-curve collision validation, and the whole ordered suffix, returning only a finite result. Keep the pre-projection temporary/page shared-owner checks in the public `StagedEditor300::reactionRateClicked` entry point, where those checks already occur.

## Exact call and effect order

Retain `StagedEditor300::reactionRateClicked(binding, selection)` admission exactly:

1. Validate binding identity, selection range, complete page/model/remesh binding.
2. Require temporary 3 to exist; require page curve 3 to identify the same allocation and shared owner; require it differ by raw address from temporary curves 0–2.
3. Project `pages_->pages.environment.acvcofControls = {selection, selection == 0, selection == 0, selection != 0}`. Set report stage/flag to `ControlsProjected`.
4. Call the private `commitReactionRateSelection(binding.model, selection)`.
5. Map `MissingOrReplacedControl` or `LiveCurveAlias` to the existing unsupported report at `ControlsProjected`; leave current index and all live/temporary curve fields untouched. Map `Complete` to `Complete`, `modelUpdated=true`, and `ApplyResult::Completed`.

Inside `commitReactionRateSelection`, preserve this sequence:

1. Call `model.simCtl()` once. This must retain its current geometry-barrier-first behavior: `requireGeometrySyncAvailable()` runs before nullable control lookup. Do not use `matchesCurrentSimControl` first, because it adds a separate lookup/admission and changes the sequence.
2. If the fresh pointer is null or differs from `expectedControl`, return `MissingOrReplacedControl`. Do not dereference `expectedControl`; it is an identity token only.
3. Compare temporary curve 3's raw address with the current live curves in this order: `envtmp`, `cnvcof`, `envatm`, `acvcof`. On any collision return `LiveCurveAlias` before any index or curve-data stores. Do not test shared ownership here; the live curves are borrowed from the current control.
4. Store `temporaries.currentCurveIndex = 3`.
5. For selection 0 only, store live `control->acvcof.ndim = 0`.
6. Store live `control->acvcof.var[0]` from `[0, 3, 12, 1]`.
7. Store `temporaries.curves[3]->var[0]` to that same selected value.
8. Return `Complete`. No supported operation in the suffix may add a second control lookup, validation failure, allocation, callback, or return point.

The coordinator assumes its `pages_` owner has passed the entry point's preflight and remains alive for the synchronous call. It must not repeat checks after the control lookup if doing so could add a new refusal point after original-order effects. It does not modify temporary `ndim`, either table's storage/count/payload, live dimension for nonzero selections, page constant text, or unrelated curve metadata.

Parked geometry is intentionally exceptional, not a result enum: it propagates from `simCtl()` after the caller has projected the controls and before index/live/temp stores. Do not catch it in the coordinator or callback. The existing C ABI and Stopping/cache1 behavior are outside this event and remain unchanged.

## Focused tests

Use `source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp` and its target/CTest `m2m_mdt_simcontrol300_reaction_rate` / `m2m.mdt.simcontrol300_reaction_rate`.

Retain the existing cases for all four choices, constant-to-variable and repeated-click sequences, table/metadata retention, missing/replaced control partial projection, pre-projection invalid binding/selection and owner mismatch, and live-curve collision refusal. Improve the refusal cases with distinct sentinels for current index, live dimension/variable, temporary variable, and table identity so each expected unchanged field is explicit.

Add a parked-geometry callback case that retains a page reference before parking, invokes the event, catches the geometry error, and checks that ACVCOF controls were projected while index and live/temporary data remain unchanged. Do not call `pageState() noexcept` on the parked model.

Exercise null/replaced identity and live-curve collision through the event report and existing owner fixture; the private member need not be exposed to tests. Existing final-state tests cannot prove ordering among ordinary index/live/temp assignments or establish absence of every intermediate state. Review the coordinator source for that exact order. Add no production observer or callback solely to test the order.

Run only the assigned focused target when authorized. Root owns grouped/full builds and source-stable freeze.

## Original evidence and qualification

The order is qualified by `analysis/resume135-editor-reaction-rate.md` and `analysis/resume135-editor-reaction-rate-review.md`: original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; callback range `[0x140eccf10,0x140ecd2e5)`; range SHA-256 `9b53369cd493e1d3cad521a65293ab114f46edaa8dabc0e09a8b5c0b6834da38`. The bounded static trace supports controls before current-control lookup, null skipping later stores, then index 3, selection-zero live dimension clear, live variable write, and temporary variable write. It does not constitute original-process differential evidence.

The candidate's replacement-control identity result and shared-owner/page-curve preflight are adapter qualifications, not claims of native validation. The test fixture covers explicit private callbacks on a constructed staged owner, not physical QWidget state, actual Qt delivery/reentrancy, full public editor construction, or Save completion.

## Implementation boundary

This is an implementation-ready finite behavior/API contract, with a private `StagedEditor300` member as the selected placement. No source, tests, build artifacts, shared state, GUI, original process, or REA state was changed for this contract.

