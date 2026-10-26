# CP224 viewer dimension review

## PASS

The added `GuiModel::dimension() const noexcept` has the requested unguarded scalar semantics: it returns `step->dimension` unchanged when `step` exists and `0` otherwise. It neither checks the geometry barrier nor returns/publishes a pointer, reference, callback capability, or identity token. The `modelDim()` helper delegates to this method, and its four callers remain in place, including viewer scene-box selection and the readiness comparison. This preserves scalar reads while geometry work is parked.

The focused model test covers dimension 3, dimension 2, and a model with no step. While the fixture is parked, it checks both `pendingModel.dimension()` and `guiModelDim(pendingModel)` against the native step dimension. These assertions are compatible with the explicitly permitted scalar observation and do not claim other parked-model access. Existing refusal assertions for `simCtl`, `simGeometryTypeIf2D`, object/material access, and mutation remain.

No C ABI declarations, exports, layout, API version, flags, or error mapping are changed by this increment. The inspected diff contains no changes to cache1 refusal or CP163 publication/early-return flow. Public `step` and retained aliases remain, as the plan explicitly scopes this as one migration step rather than alias closure. `StepIdentity` appears in the combined working diff from the prior CP222 increment; this dimension operation does not use it as an access grant or create an alias escape.

## Limits

This is a source review only. Root owns build, focused/full tests, and freeze; no CP224 validation is claimed here. The implementation report's cited CP223 validation is prior-checkpoint evidence.
