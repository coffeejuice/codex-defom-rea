# Resume 46: operation dispatch audit

## Finding

C07 is still open after the base operation Save dispatch was integrated. The actionable reachable hole is the `SfOprShapeOptim` properties-to-engine path, not the already corrected base widget order.

In `source/apps/moproj2mst/src/engine/save_current.cpp`, `saveCurrentOperation` first requires `opBase(op)`, `ob->current == &op`, and `op.project` (around line 715). For `SfOprShapeOptim`, its own `vt+0x68` branch at lines 729–736 calls `ob->updateDataInWidgetArea(...)`; when false, it calls `propsToEngine(p, op)`, then performs the ShapeOptim after-files and operation-files slots and returns. `propsToEngine` at lines 147–179 delegates to a class handler if available, then checks the generic key families. ShapeOptim is not in those families and reaches `notPorted(op, "properties vt+0x60")`. This is a concrete unsupported branch on the active Save path, conditional on the editor-update callback returning false.

The retained original evidence qualifies the intended operation: `source/docs/re/moproj2mst/mdt/L4/spec.md` section 9 identifies ShapeOptimProperties `vt+0x60` at `0x140142d00` as copying MaxSims / MinObjFuncVal between property data and operation-class fields `+0xcc/+0xe0`. The ledger records that address as `ported`, but the active converter dispatch still has no ShapeOptim handler in `propsToEngine`; ledger status therefore does not establish integrated behavior. The ShapeOptim load/property model is real: `shapeoptim_load.cpp` implements the class and its active model path, so this is not merely a dead class label.

## Historical closure audit versus later work

The earlier `analysis/closure-audit.md` C07 entry listed missing operation property-save dispatch and MST-time model updates as candidates requiring call-chain verification; it explicitly warned that lexical omissions alone do not establish defects. The later `analysis/closure-reconciliation-checkpoint35.json` updates C07 to “base dispatch integrated; remaining families open” and directs inspection of family override/body differences. `analysis/wave3-save-dispatch-validation.json` records the later validation: base widget/property dispatch was exercised (111 passed, 6 passed with logged gaps, 1 with logged differences; zero failures), while its qualifications retain unclosed boundaries. The companion `analysis/wave3-save-widget-order-contract.json` is narrowly scoped to base `vt+0x68`/`vt+0xd0`; it expressly excludes class-specific overrides such as ShapeOptim. Thus the historical base omission must not be reopened, while ShapeOptim's own family branch remains a valid next audit target.

## Next action

Recover and integrate the ShapeOptim `vt+0x60` property-to-class copy using the retained L4 source qualification. Add a focused dispatch case with `SfOprShapeOptim` as the current operation, valid `ShapeOptimProps`, and `updateDataInWidgetArea == false`; assert that MaxSims and MinObjFuncVal reach the class data at `+0xcc/+0xe0` and that execution proceeds through the following ShapeOptim Save slots instead of returning `M2M_E_UNSUPPORTED`. Add the complementary editor-update-true case to preserve the existing props-load route. Then run the focused save-dispatch regression and the prescribed integrated regression on the frozen candidate. Acceptance requires the original-qualified field mapping and branch order plus successful focused and integrated checks; do not claim public original parity from the historical folder comparator alone.

## Evidence limits

This is a static source/evidence audit only. No app, REA session, build, converter, or test was run. The exact branch is source-reachable under the stated guard; whether a particular corpus fixture takes it remains unestablished.
