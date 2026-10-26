# CP242 dimension fallback query review

**PASS — `dimensionOr(3)` is an exact scalar relocation for the selected expression, provided it stays unguarded and at the existing argument-construction point.** This is a design/source review, not an implementation or runtime pass.

Reviewed CP242, the current lifecycle Environment Apply expression and surrounding page order, `GuiModel::dimension()` and its declaration, `applyEnvironment3D`, the model-dimension tests, lifecycle view-factor tests, and retained CP226 dimension contract qualifications.

## Exact scalar behavior

`return step ? step->dimension : whenNoStep;` preserves the current conditional for every live dimension integer, including zero and values outside 2/3. Only an absent step uses the supplied fallback. With caller argument 3, live dimension zero must return zero; `dimension() ? dimension() : 3` would be wrong and also introduces multiple reads. Do not invoke `setDimension`, create storage, validate/clamp the dimension, or substitute a geometry getter.

The query may be `const noexcept` just like the existing scalar getter. Do not add `requireGeometrySyncAvailable()` or delegate through an admitted native accessor. Both the current direct conditional and `dimension()` are unguarded; introducing a barrier changes the reached operation's failure behavior. The proposed method does not replace earlier Apply preflight admission or authorize any parked continuation.

## Caller order and original distinction

Replace only the first aggregate argument in `EnvironmentViewFactorState`. It is evaluated after successful/non-unsupported Stopping, Meshing and Iteration processing, during argument construction before `applyEnvironment3D` enters. Keep curve pointer and definition arguments, record/result handling, later Advanced and Extra3D calls, and partial page effects unchanged. Do not move the query to preflight or into the Environment implementation as part of this leaf.

There is a relevant provenance distinction: current `applyEnvironment3D` comments describe the **original** live dimension query after environment curve/scalar writes and `vfactr_mode = 1`. The adapter currently assembles the dimension value before entering that function, even when the checkbox is unchecked or Environment later stops. This leaf preserves the existing adapter evaluation point; it does not recover or prove native live-query timing. Moving it inside that branch would require separate review and evidence. The proposed no-guard scalar extraction does not add an observable native effect at its present position.

Earlier Apply/model guards may prevent absent-step or non-3D cases from reaching this expression in ordinary supported flows. That does not justify deleting its fallback or zero distinction, nor does a direct query test establish whole-lifecycle support for those states. Keep all cached-control lifetime qualifications attached.

## Test corrections and useful checks

`test_gui_model_dimension.cpp` currently contains equal/changed **setter** tests that assert ordinary `dimension()` results. It does not yet directly cover absent-step or live-zero getter behavior, and does not by itself establish unguarded parked reads. Correct the proposal's broader statement that this file already covers the unguarded getter contract.

Add direct tests with distinct fallback values (for example 3 and a negative sentinel): live 2/3 ignores either fallback, live zero remains zero, missing step returns exactly each supplied fallback. Set zero directly in the owned fixture's native dimension field rather than invoking a setter whose initialization effects are unrelated to the query. Check the existing `dimension()` still returns zero when absent. Restore a temporary zero field before unrelated fixture/native cleanup if required by its supported invariants.

A direct `dimensionOr` call on an existing parked viewer fixture would meaningfully catch accidental admission; the expected live dimension must still be returned without throwing. Existing parked setter refusal tests alone do not test this new method. Existing lifecycle checked/unchecked view-factor tests remain appropriate ordinary caller regressions; they do not demonstrate absent-step/zero fallback reachability through full Apply.

CP226/current getter declaration records GUI twins `0x140c7c0e0/0x140c7c0a0` and the existing unguarded zero-without-step behavior. The new configurable fallback and the caller's value 3 are adapter API semantics, not a new recovered native getter or fresh original observation. No lifecycle ownership closure, Stopping/cache1 change, or broader alias-safety proof follows.

Only this report was written. No source, tests, build artifacts, shared recovery state, GUI, original process, or REA state was changed.
