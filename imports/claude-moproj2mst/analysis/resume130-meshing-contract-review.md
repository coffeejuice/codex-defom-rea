# CP130 independent review: stateful meshing selection contract

## Summary

The retained-source design is coherent for a private finite 3D selection slice: a combo index is distinct from the row whose controls were last loaded; activation and Save both commit the old visible controls and reload the requested row, even when it is the same row; only Save copies remembered values into current model objects. The native call sequence and the proposed separation from the old const Apply adapter are sound.

Before implementation is integrated, resolve two API ambiguities and one concrete shared-header/fill initialization gap. These are not claims of an original behavior mismatch; they are risks that could make the candidate update the wrong row or make first selection depend on defaults.

## High-risk points

### 1. Fill currently does not establish the new index invariant

The root-owned `simcontrol300.hpp` has `storedObjectIndex` and `comboObjectIndex`, both defaulting to `-1` (`MeshingPage`, around lines 223–235). In the frozen CP128 `fillMeshing3D` (`simcontrol300.cpp`, around lines 919–984), nonempty pages directly populate the visible controls from `objects.front()` but do not set either index. Thus a freshly filled, populated page currently has valid row-0 controls but reports stored/current index `-1/-1`.

Native Fill explicitly starts its stored index at `-1`, sets the combo to row zero, invokes the handler for row zero, and the handler stores zero. The post-Fill candidate invariant for a nonempty admitted page must therefore be `storedObjectIndex == 0` and `comboObjectIndex == 0`. Empty pages should preserve the no-selection state (`-1/-1`) and must not synthesize the positive-index temporary-array witness. Choose one initial-load producer: either have Fill prepare the row-0 controls and set the two indices as the completed InitialLoad result, or have Fill build only the row list and call `transitionMeshingObject3D(..., 0, InitialLoad)` exactly once. Do not both prepopulate the controls and run a second initial transition that could write stale/default controls.

This also affects the proposed legacy guard (`resume129-editor-meshing-selection.md`): if old `applyMeshing3D` rejects changed selection context or requires row zero, its existing filled callers need the explicit `0/0` initialization. Tests that hand-construct a `MeshingPage` must state which context they intend.

### 2. `requestedIndex` duplicates `comboObjectIndex` for Save

The shared header currently declares `transitionMeshingObject3D(GuiModel&, MeshingPage&, int requestedIndex, MeshingObjectEvent)` and an event `SaveCurrentIndex` (around lines 238–358). The contract says Save uses the actual combo current index, but the function also accepts an independent index. It does not yet say what happens if these disagree. A caller could unintentionally commit/reload the wrong record while the page claims another row is selected.

Prefer one of these explicit contracts before implementation:

- Remove the integer parameter from `SaveCurrentIndex` by using a separate overload/helper that reads `page.comboObjectIndex`; or
- Require `requestedIndex == page.comboObjectIndex` for `SaveCurrentIndex` and refuse mismatch before old-control conversion or mutation.

For `Activated`, the parameter is the delivered native `activated(int)` value; after a genuine user activation it should agree with the current combo index. State this invariant and refuse mismatches before committing the previous row. A programmatic `setMeshingComboCurrentIndex` changes only the candidate combo index and does not call the transition; the later Save must consume that index. The setter should accept only `-1` or an in-range index, as the proposal says.

The native route is important here: the inspected signal is `activated(int)`, not `currentIndexChanged`. Do not make the programmatic setter implicitly commit the old controls. Native Save separately reads the combo's actual current index and calls the same handler.

### 3. Define partial-failure stage semantics precisely

The proposed `MeshingObjectReport` has `outcome`, `stage`, and three booleans, but the contract needs to make the mutation boundary normative:

- Preflight all old visible controls and their supported numeric profile before changing the old remembered row. A late old-control conversion refusal must not partially update only some of its four values.
- Once the old row is committed, a target-profile refusal must report `PreviousCommitted`, retain those old values, and leave target controls and `storedObjectIndex` unchanged. Do not report a generic refusal that could be mistaken for rollback.
- Only after target controls have been fully loaded should the handler update `storedObjectIndex` and report `Complete`/`ControlsReloaded` consistently. `sameRow` is informational and must not skip commit/reload.

The report proposes this sequence, but should explicitly state whether `ControlsReloaded` is an intermediate stage that may return, or whether every successful transition finishes at `Complete` with `controlsReloaded=true`. That matters for tests and the lifecycle caller.

## Verified behavioral anchors

The controlling native qualification is `analysis/resume129-editor-meshing-selection.md`, based on the original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and the retained complete `0x140eceb60` handler. It establishes:

- negative requested index returns before reading old controls;
- stored index `-1` skips old writeback;
- nonnegative callback writes the prior visible values to its temporary row, then indexes and reloads the requested target without a same-row early return;
- native Fill explicitly initializes row zero through that handler;
- Save calls the handler on current combo index before copying remembered four-scalar records to current objects; missing objects skip only their copy;
- disabled global remesh still runs prior object copyback, then writes enabled zero and skips method/scaling/internal-checkbox reads.

The existing report also correctly preserves key numeric distinctions: one visible row is rounded/reformatted on reload; negative zero may first be committed and then display as nonnegative zero; unvisited payload bits must not be rejected merely because another row contains NaN. The legacy const Apply adapter should remain behaviorally distinct from the finite mutable selection path, and the actual private lifecycle must explicitly move to the new API while retaining its same-model/remesh-owner checks.

## Current integration state and identities

The shared header currently contains the index fields, event/stage/report types and three proposed declarations, but the frozen CP128 `fillMeshing3D` and lifecycle still use the old first-row helper. This is expected before the worker implementation, but it confirms that Fill/index initialization and the lifecycle call replacement are required integration steps, not optional tests.

- Shared header `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp`: `F014239ACE1DC9633DEC3033AA3744D177F188C4FD8950196ED6B4B3B234FEA7`
- Current private page source `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `260331D646FDC5EDF712278C6E056142996A8B4DF54A59BFD1DB8C96B9B52ABC`
- Current private lifecycle `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: `DD22C2E00FBD459822E2E5CF0E9325AC3E9997F5D553CE50559157CCA19AB486`
- Proposal `analysis/resume129-editor-meshing-selection.md`: `AC8A95391B93E0106AD62893303F60D8D67631A590475525EDFAF41CE97B7192`

Read-only review only: no source edits, build, tests, GUI, original execution or mutable REA operation.
