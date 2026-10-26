# CP270 reaction-rate owner-query next-leaf review

## Decision: HOLD

There is no finite `GuiModel::dimension()` substitution in the CP245 reaction-rate coordinator that preserves its established decision and validation order. Do not change source or tests for this leaf.

## Current boundary and order

The event is implemented in `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`, not in `simcontrol300.cpp`. `StagedEditor300::reactionRateClicked` first checks binding identity, selection range, owner/page state, and page/model/remesh identity. It then checks temporary curve 3 and its page alias/shared owner, and verifies it is distinct from temporary curves 0–2. Only after those checks does it project `acvcofControls` and set report stage `ControlsProjected`.

The private `commitReactionRateSelection` then performs exactly one `model.simCtl()` lookup. That method is the intended geometry-barrier-first admission. A null or changed current control returns `MissingOrReplacedControl`; otherwise the coordinator checks live-curve aliases in `envtmp`, `cnvcof`, `envatm`, `acvcof` order, then writes current index 3, the selection-zero live `ndim` clear, the live variable, and the temporary variable. The caller maps either refusal to the already-projected unsupported report. Parked geometry propagates from `simCtl()` after control projection and before suffix stores.

There is no raw `model.step` or `step->dimension` decision in this callback/coordinator path. Adding `model.dimension() != 3` would introduce an additional refusal before (or instead of) the single `simCtl()` admission. It would change the CP245 partial-effect/error order and would not preserve the required callback behavior. Replacing the `simCtl()` identity lookup with `dimension()` is also invalid: dimension does not return the current control pointer and cannot establish the cached `sourceControl` identity required by the coordinator.

## Nearby code is a different boundary

`source/apps/moproj2mst/src/engine/simcontrol300.cpp::fillEnvironment3D` accepts `SimControl& sc` and an explicit `int dimension`; it checks those inputs before reading the four curves and building the page, then applies the initial ACVCOF selection stores. It has no `GuiModel` parameter or control-identity token. Migrating it to `GuiModel::dimension()` would require changing its signature/caller and establishing which model owns `sc`. No evidence in this review justifies that new owner/callback assumption, and it would not be an alias-only migration of the CP245 event.

Other `dimension()` predicates in `simcontrol300.cpp` (including the current Advanced/Stopping/Meshing families) are separate leaves and are outside this reaction-rate decision.

## Existing caller-level evidence

- `analysis/resume245-editor-reaction-rate-contract.md` records the private coordinator boundary, the callback preflight/projection order, the one barrier-first `simCtl()` lookup, the control identity and curve-alias refusals, and the exact store suffix.
- `analysis/resume245-reaction-rate-coordinator-implementation.md` records implementation and focused validation; it reports the reaction-rate and viewer-box focused build/test runs, while explicitly qualifying original evidence as the bounded CP135 static trace rather than fresh differential execution.
- `analysis/resume245-reaction-rate-coordinator-implementation-review.md` independently reviews the exact source order and test shape, without claiming original-process comparison.
- `analysis/wave3-checkpoint245-validation.json` records the full-build/test receipt and qualifications. The focused `m2m.mdt.simcontrol300_reaction_rate` case reports 0/8 failures; the linked stopping-cached and viewer tests also report no failures in that receipt.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp` exercises the actual `StagedEditor300::reactionRateClicked` entry, including all choices, repeated clicks, missing/replaced control after projection, preflight refusals, live alias refusal, and retained state. `test_viewer_box.cpp` adds the parked-geometry callback exception-prefix case. These tests support the current callback contract; they do not authorize inserting a dimension guard.

The actual-caller tests are already sufficient to detect changes to callback report stage and partial state for current behavior. There is no meaningful actual-caller test for a dimension migration here because no dimension decision exists in this path. A new test would encode an invented admission rule rather than establish equivalence.

## Exact files and test boundary

No implementation file or test should change for CP270. Retain these as the evidence surface if the leaf is revisited:

- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`
- `source/apps/moproj2mst/include/m2m/engine/simcontrol300_owner.hpp`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp`
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`
- `analysis/resume245-editor-reaction-rate-contract.md`
- `analysis/resume245-reaction-rate-coordinator-implementation.md`
- `analysis/resume245-reaction-rate-coordinator-implementation-review.md`
- `analysis/wave3-checkpoint245-validation.json`

The single new artifact is this note, `analysis/resume270-reaction-alias-next-leaf.md`. No tests were run for this read-only review.

## Limits

The source and test evidence establishes the reconstructed synchronous callback contract, not fresh native differential confirmation. The CP245 tests do not establish physical QWidget delivery, Qt reentrancy, public editor construction, or Save completion. The cached control pointer remains an identity token, not a generation identifier. This HOLD does not block separate, independently qualified owner-query migrations in other `simcontrol300.cpp` functions.
