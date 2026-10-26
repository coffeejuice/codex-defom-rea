# CP242: next Editor300 model-alias leaf after CP240

## Recommendation

The smallest new, independently reachable step-alias migration is the scalar read in `applyInitialEditorPageStateBound` at `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp:315`:

```cpp
EnvironmentViewFactorState{model.step ? model.step->dimension : 3, ...}
```

Replace only that expression with a model-owned value query, for example `model.dimensionOr(3)`. This consumes the step internally and returns one integer; it neither exposes a `DB_STEP_DATA*` nor touches the more coupled `reactionRateClicked` callback. CP222’s broad alias ledger identifies direct step reads as migration targets. CP238’s reaction-rate review remains accurate: its widget projection, control/curve identity checks, owner-index update, native writes, and temporary-curve write are coupled, so that callback is not the next single-effect leaf.

## Required contract and call order

Add `int GuiModel::dimensionOr(int whenNoStep) const noexcept` beside `dimension()`. Implement it with the same unguarded read contract as the existing scalar getter:

```cpp
return step ? step->dimension : whenNoStep;
```

Do not call `requireGeometrySyncAvailable()` here. At this call site, `dimension()` is documented as unguarded and the current conditional field access is unguarded; adding a barrier would create a new refusal point after the already-reached page operations. Preserve the current fallback value 3, and preserve a present step whose actual `dimension` is 0 as 0. `dimension()` itself returns 0 for both absent step and actual zero, so replacing the expression with `dimension()` or a truthiness fallback would lose that distinction.

At the caller, change only the expression supplying `EnvironmentViewFactorState.dimension`. Keep it after the Stopping, Meshing and Iteration Apply calls, and before `applyEnvironment3D`; do not move it into earlier page preflight or after the Environment Apply. The view-factor curve/definition arguments and page result ordering stay fixed. This leaf is a value-access migration, not an identity guard and not a change to Stopping/cache1.

## Exact files and test seam

- `source/apps/moproj2mst/include/m2m/engine.hpp`: declare `dimensionOr(int whenNoStep) const noexcept` adjacent to `dimension()`.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: implement the scalar query with no geometry guard and no returned alias.
- `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: replace only `model.step ? model.step->dimension : 3` with `model.dimensionOr(3)`.
- `source/apps/moproj2mst/tests/mdt/test_gui_model_dimension.cpp`: add direct cases for a live 2/3-dimensional step, a live step whose dimension is 0, and an absent step returning the supplied fallback. This file already covers the unguarded `dimension()` behavior and is the focused model-value test seam.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp`: retain its existing Environment Apply/view-factor definition cases (around 628–666) as caller regression. Root’s integration group continues to include `simcontrol300_stopping_cached` and the parked viewer barrier suite.

No new test target or C ABI signature is needed. If the model API/header files are assigned to another lane, coordinate the declaration before editing rather than making overlapping changes.

## Evidence and limits

Current reconstructed-source evidence is the expression at lifecycle.cpp:315, `GuiModel::dimension()` in `gui_model.cpp`, its declaration/qualification in `engine.hpp`, the `EnvironmentViewFactorState` use in `simcontrol300.cpp`, and existing GUI-model/lifecycle tests. CP226 records the dimension getter’s original mapping to GUI twins `0x140c7c0e0/0x140c7c0a0` and explicitly describes it as unguarded, returning 0 without a step. The fallback value 3 belongs to this adapter’s `EnvironmentViewFactorState` assembly; this note does not claim it is an original native getter fallback or a fresh original observation.

The change removes one public-step scalar access. It does not close other lifecycle aliases: `openInitialEditorInto` still holds a control reference across its page fills, Apply still uses the cached `session.sourceControl`, and `reactionRateClicked` still has its separate interleaved owner/control/temporary-curve effects. No source, test, build, shared state, original process, GUI or REA state was changed for this report.
