# CP181 R300 independent plan review

Accept `analysis/resume181-r300-next-cut.md` for the narrow Express Save SimControl loop cut. No blocking findings. This accepts the plan; implementation, build and runtime acceptance remain pending.

## Frozen baseline and ordering

Read `build/wave3-checkpoint180-verified/source.zip` directly without extraction. Frozen `apps/moproj2mst/src/engine/save_l3.cpp` hashes to `7c89166e6d2fede2b110578c332ecb690e62d94d180ae6d27be72c30759684a1`; frozen `apps/moproj2mst/tests/mdt/test_engine_l3.cpp` hashes to `c24892241818ed286c8cda68136cdae53dafa2d9e3e2a179dae1bb0c4a4982cb`. Both match the plan and the working files inspected. The frozen function is lines 121–136.

`heatExpressMode(op, "SfOprHeatExpress")` and its negative early return precede the single `opModel(p, op)` lookup. That lookup can install loader slots or create a model, so preserving its position matters. The missing-model return precedes `m->simCtl()`. The proposed immediate exact-void callback, invoked with that same acquisition, preserves the sequence if its null return precedes `objectCount()` and every object lookup.

`GuiModel::simCtl()` calls `requireGeometrySyncAvailable()` before testing `step` and returning a nullable control. `objectCount()` itself only reads the step count or returns zero. Moving count acquisition before `simCtl()` therefore changes refusal ordering even if the eventual loop is empty. Direct step access bypasses the existing refusal; a helper that performs another model lookup changes lookup behavior. Keep `m->simCtl()` verbatim at the old acquisition point.

## Pointer confinement and writes

The callback may capture the existing model for immediate synchronous use. Its SimControl parameter and per-iteration object pointer must remain inside the callback, with no return, retained capture, storage or queued use. There must be no function-scope raw `sc` in this method. Null control must return before count acquisition. Preserve one count read, indices `0 .. n-1`, `objectAt(i + 1)`, and the null/kind-0/kind-3 skips.

For each eligible object, compute the original absolute difference of old `+0x1f00` and old `+0x1ef8`, divided by two, before either write. Preserve the upper-field write `+0x1f00 = SC+0x3d0 + d` followed by lower-field write `+0x1ef8 = SC+0x3d0 - d`. Keep both environment-temperature reads in place; do not hoist them as an incidental cleanup. The plan accurately limits this to one local alias and does not establish aggregate parked-model safety or Stopping cache1 readiness.

## Focused test and final acceptance

The correct test file is `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp`. Its existing `heat_express_3d` fixture loads copied corpus `53_Cogging_Multiple_Ops_Lab1`, operation 4, heat-transfer type 0, and a model asserted to have one object. That makes it an appropriate positive-path fixture, provided the new case requires a nonnull object and eligible kind before checking changed fields. Use finite, distinct old bounds and a separate finite environment value so a skipped loop cannot satisfy the expected result. If changing kind is necessary, change only the copied fixture and restore it reliably.

The direct second call with the actual step temporarily moved out tests the internal null-control skip. Keep ownership of the moved step, restore on normal and exceptional exits, and verify model identity, original step identity and untouched bound sentinels after restoration. It does not establish public Save admission with a null step. Existing CP180 TMAX coverage must remain intact. Source review establishes the before-count guard; the synthetic null case alone cannot prove that ordering because `objectCount()` also returns zero for a null step.

Acceptance requires the paired file reservation, final changed hashes, static checks above, the five specified focused CTests with proof the added case executes, and the complete source-stable suite before freeze. Check the production diff affects only `expressSaveOperation` and does not modify ABI, model layout, other Save functions or A/cache state. Any failed gate blocks implementation acceptance, not this plan approval.

Only this review report was written. No production source, build, test execution, GUI/app-control, live REA, deployment or shared state was changed.
