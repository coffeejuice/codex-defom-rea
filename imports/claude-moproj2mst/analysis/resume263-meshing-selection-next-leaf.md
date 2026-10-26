# CP263 meshing selection owner next leaf

## Decision

The remaining finite owner/migration leaf is the first admission expression in `applyMeshingSelection3D`. Replace its direct nullable-step/dimension read with the existing `model.dimension() != 3` query. Keep this separate from CP260's completed const `applyMeshing3D` extraction: the mutable entry executes an observable `SaveCurrentIndex` transition, which must retain its exact position and failure prefix. This is an ownership cleanup, not a new behavioral claim or public Editor300 completion.

## Exact scope

In `source/apps/moproj2mst/src/engine/simcontrol300.cpp`, change only:

```cpp
if (!model.step || model.step->dimension != 3 ||
```

to:

```cpp
if (model.dimension() != 3 ||
```

Do not move the admission ahead of or behind the remesh-method condition. Do not change the subsequent `transitionMeshingObject3D(model, form, form.comboObjectIndex, SaveCurrentIndex)` call, its immediate refusal return, the remembered-object copy loop, or `applyRemeshPageControls`.

`GuiModel::dimension() const noexcept` is implemented in `source/apps/moproj2mst/src/engine/gui_model.cpp` as `step ? step->dimension : 0`. For this admission expression, missing step and non-3 dimension therefore remain rejected as `UnsupportedSnapshot`, and dimension 3 remains admitted. The new query does not add a geometry guard or move any writes. On a transition refusal (including the retained unmasked nonfinite-node case), the previous-row commit may remain, while target controls, object copies, and remesh-state application remain skipped. Successful transitions still commit the previous controls first, retain the transition's combo/stored-index and loaded-control updates, copy every remembered row with missing live-object skips, and finally update remesh controls.

## Existing actual-caller evidence and test gate

`source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp` calls `applyMeshingSelection3D` as the Editor300 Meshing page inside the ordered lifecycle apply sequence. This establishes a current reconstructed caller, not public ordinary-MO GUI reachability.

The focused actual-entry test is `source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp` (Testkit target `m2m.mdt.simcontrol300_meshing_selection`). Existing cases call the real mutable function and cover:

- successful actual-combo `SaveCurrentIndex`, raw target preservation, remesh disabled-tail behavior, and updated selection/control state (`meshing_stateful_save_uses_actual_combo_and_does_not_commit_fresh_target_twice`);
- negative combo behavior and remembered-row copies;
- a transition refusal after the previous commit, with model and RemeshState effects absent;
- method-preflight refusal with no commit;
- separation from the legacy const first-form entry;
- missing live-object continuation and admitted empty-object 3D state;
- masked and unmasked nonfinite node conversion profiles, including the unmasked refusal prefix.

These actual-caller tests make an added test for the literal query substitution unnecessary unless review finds they cannot represent a missing-step state. If such a test is warranted, it must assert no page/model/remesh stores at the rejected admission and use a valid `GuiModel` missing-step construction already supported by the test seam; do not fabricate object state to force coverage. Run the focused target after implementation and follow root's source-stable integration/freeze gates. This design audit itself runs no tests.

## Reconciliation and limits

CP248 already relocated the four-field meshing-object write into `GuiModel::copyEditorMeshingObjectValues`, preserving its guarded by-number lookup and write order. CP260 already changed only the corresponding admission in const `applyMeshing3D`; its independent review confirms the mutable selection function was untouched. CP248's review covers both callers' shared copy relocation. CP260's design explicitly deferred the mutable transition path. CP263 must not repeat either owner-copy work or broaden the scalar Apply extraction.

The available source and tests establish the current reconstructed behavior and caller order. No new original-process execution, native runtime capture, GUI evidence, public UI reachability proof, or behavior outside the finite tested profiles is available or claimed. The exact useful leaf is this one owner-query migration; no additional migration should absorb `transitionMeshingObject3D` itself, alter its commit/reload failure effects, or combine unrelated Environment/Advanced/reaction work.

Only this design note is written. No source, tests, build artifacts, shared state, original process, GUI, or mutable REA state is changed.
