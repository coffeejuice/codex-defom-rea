# CP260 Editor300 next-leaf design after CP257/258

## Decision

No new Editor300 Advanced/remesh/reaction implementation leaf is both smaller and source-ready from the retained evidence reviewed here. The source-backed finite candidate is a compatibility-preserving owner query cleanup in the existing meshing Apply admission: replace the raw `model.step` null/dimension read with `model.dimension()`. This removes one public native-step alias at a real Apply caller, but it does not add behavior or close a UI lifecycle gap. Treat it as an optional model-ownership increment, not as progress on public Editor300 completion.

A substantive Environment Apply/Advanced/reaction leaf is not ready: Environment page composition/Apply lacks an evidenced actual public caller, while the remaining Advanced path is already represented by CP258's final fixed-false remap command. Do not invent a caller to make those lower-level functions appear reachable.

## Candidate exact change

In `source/apps/moproj2mst/src/engine/simcontrol300.cpp`, `applyMeshing3D` currently begins with:

```cpp
if (!model.step || model.step->dimension != 3 ||
    (form.remeshEnabled && form.remeshMethodRadio != 1 && form.remeshMethodRadio != 3) ||
    (!form.objects.empty() && (form.storedObjectIndex != 0 || form.comboObjectIndex != 0)))
    return ApplyResult::UnsupportedSnapshot;
```

The smallest owner leaf is to use `model.dimension() != 3` in place of the two direct step reads, preserving the same zero dimension for a missing step and the same `UnsupportedSnapshot` result. `GuiModel::dimension() const noexcept` already returns `step ? step->dimension : 0`, so this preserves the exact admission truth table for this expression. Do not move validation, change the subsequent sequential object copies, or merge this with `applyMeshingSelection3D`; that function has its own mutable selection transition and remains a separate effectful path.

Files if implemented: production `source/apps/moproj2mst/src/engine/simcontrol300.cpp`; actual-caller test `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp` only if the existing coverage does not already distinguish the new query route. Existing tests at approximately lines 996, 1025, 1046, 1062, 1070, 1082, and 1085 call the actual `applyMeshing3D` entry point and cover enabled/disabled remesh controls and object writeback. Prefer extending an existing case with missing-step admission only if the fixture can express a missing step without fabricating unsupported state and if the test asserts no stores. Otherwise no new test is justified for this expression-only extraction; source review plus existing actual-caller suite is the proportionate check.

This change does not justify a GUI run or a new original capture. Normal root ownership, source-stable integration and the focused `m2m.mdt.simcontrol300_snapshot` regression gate still apply before implementation is accepted. Do not edit or build as part of this design audit.

## Rejected larger leaves and precise evidence limits

- `applyMeshingSelection3D` has the same direct admission reads, but then performs `transitionMeshingObject3D(...SaveCurrentIndex)` and sequential remembered-object copies. Combining it with the scalar page path would span a different state machine and test contract. If selected later, first compare its own source path and actual-caller test coverage.
- Advanced Apply's native remap was extracted in CP258 as `GuiModel::remapEditorAdvancedState()` and is called after ELMNOD/PDIE2 at the original final position. The retained CP258 review and actual-caller tests already cover its skip-on-earlier-return and resulting remap effects. Reopening it without a new mismatch or requirement is unwarranted.
- The isolated Environment page model contains `fillEnvironment3D` and `applyEnvironment3D`, but the retained public-route audit `analysis/wave3-editor-public300-ordinary3d-reconciliation.json` keeps the normal `FormingClass` route guarded and identifies unresolved interaction-vtable/empty-recovery constructor work. No inspected current actual Editor300 caller invokes Environment Apply. This is the exact reachability witness missing for a substantive page-store leaf: a retained source route from registered public Open/Apply entry to this handler (or an authorized native runtime trace proving it), plus an actual-caller test seam. No recovery behavior should be inferred from matching function names.
- Reaction-rate work is already identified as a coordinator requiring controls projection before its guarded update in prior checkpoint notes; changing its admission/order would need the exact route and failure-prefix evidence. It is not a one-expression independent leaf in the inspected sources.
- Advanced, meshing and reaction are separate contracts. Do not bundle meshing scalar owner cleanup with Environment TIMEID tables, Advanced remap, recovery-tail resizing, or reaction mutation.

## Qualification

This is a read-only design audit after frozen CP257/258. Source evidence reviewed includes current `simcontrol300.cpp` meshing Apply definitions, `GuiModel::dimension()` in `gui_model.cpp`, the Editor300 public-route reconciliation, the CP258 Advanced implementation review, and existing actual-caller snapshot-test locations. The audited code is reconstructed source; no claim of fresh native execution, GUI evidence, public ordinary-MO-route reachability, or behavior beyond the admitted selected profiles is made.

Only this report is written. No source, tests, build artifacts, shared state, GUI, original process, or mutable REA state is changed.
