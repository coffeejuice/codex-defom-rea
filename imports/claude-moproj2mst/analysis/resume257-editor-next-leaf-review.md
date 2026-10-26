# CP257 Advanced remap owner-command review

**PASS — the proposed fixed-false command preserves the existing final Advanced operation when called at the same point.** This approves the bounded extraction, not native rewriting, new admission policy, or a runtime result.

Reviewed CP257, current `advancedApplyProfile`/`applyAdvanced3D`, native remap declaration/body, the two named snapshot tests, and retained Advanced contract/twin-comparison evidence.

## Exact command and caller position

`void GuiModel::remapEditorAdvancedState()` should contain the literal `def::model::step_remap_material_state(*step, false)`. Keep false fixed, void return, existing native dispatch, and no alias result or retained native pointer. It requires an existing live step. It must not add simCtl, identity/dimension/geometry admission, null-step no-op, allocation handling, rollback, or noexcept.

The old final direct dereference is unguarded. Missing storage has no defined graceful-failure contract; do not execute a deliberate null dereference or label its result a recovered native failure behavior. The earlier Advanced profile currently checks nullable step/dimension, guarded current-control identity, time profile, radio/options and user text before mutation. Keep that preflight unchanged rather than repeating its guard after the page stores. A direct new command can therefore operate where an ordinary admitted Advanced call cannot; document this staged internal precondition, not a general parked-access permission.

Replace only the final call, after the sequential TNOW writes/radio, GEOERR stores, ten user strings, seven output flags, all three ELMNOD options, and PDIE2. Completion follows command return. Both unsupported profile refusal and every earlier ValidationStopped return must still skip remap. Exceptions during conversion/native remap continue to propagate with the already-reached stores retained; no completed result is returned if the native call throws. Do not merge this into recovery-width resizing or trigger it during Fill.

The declaration has no editor-type/include cycle. **Include correction:** current `gui_model.cpp` does not explicitly include the declaring `defmodel/material/state_remap.hpp`. Add that actual resolved declaration header for the new definition rather than relying on unrelated step headers/transitive includes. The file exists under the target's existing defmodel include tree. Current `simcontrol300.cpp` includes it; remove that include only if a scoped search proves no other declaration from it is needed after migration.

## Native behavior and tests

The current native body snapshots old widths, conditionally snapshots material-number/index information only for true, rebuilds widths, and remaps each eligible non-kind-7 object array. False still performs real width/array work. Do not replace it with only a width calculation or reset, broaden it to true, rewrite its iteration, or preserve potentially invalidated interior arrays through a new snapshot mechanism.

The unconditional `advanced_invalid_returns_keep_exact_earlier_stores_and_skip_final_remap` test uses width 1234 and checks two meaningful stop locations: invalid TNOW2 retains only earlier TNOW1; later invalid GEOERR retains time/radio/earlier GEOERR and skips later strings/flags/remap. Its nondefault width would detect eager remap. It does not independently enumerate every unsupported preflight branch or every invalid field; retain ordinary surrounding tests for those.

`advanced_final_remap_rebuilds_widths_and_allocates_live_object_state` actually calls Advanced Apply with a changed remembered ELMNOD choice, deliberately stale width, two nodes and distinctive coordinate payload. It requires widths 3/1, node count/payload retention and zeroed newly allocated state storage. It catches omitted/no-op command or moving remap before the ELMNOD stores. It does not distinguish false from true in all material configurations, nor prove PDIE2-before-remap through an observer. Source inspection must establish the literal false and exact final call order; no production observer is warranted solely to test ordinary stores.

No direct empty-model test should replace these caller tests. A new direct test is optional. Do not add a parked-refusal expectation to an intentionally unguarded command; existing Advanced preflight/viewer admission checks have separate contracts. Reacquire any remapped array pointer before inspecting it. The native aggregate/object ownership remains unchanged and storage reallocation does not establish global alias safety.

Root should serialize the three source files with the active SPRING owner and run snapshot plus lifecycle/recovery-tail regressions, followed by its normal integration/freeze. This review ran no configure/build/test or state mutation and asserts no target pass.

## Retained original qualification

The Advanced contract explicitly places GUI remap `(step,0)` after prior page stores. The comparison records 268 equal normalized instruction shapes for the bounded GUI/native remap ranges, while expressly limiting paired external absolute targets to correspondence rather than transitive equivalence. This supports the retained source mapping; it is neither full call-graph correctness nor fresh original differential execution.

The new method name is reconstruction API structure. Preserve the existing GUI image identity and retained native-image hash qualification in those evidence files; do not assign a newly invented reconstruction version to historical bytes. No cache1 continuation/publication, native interior-pointer proof, broader meshing witness, physical Qt behavior, public editor/Save completion or accepted-Close witness is gained.

Only this review report was written. No source, tests, builds, shared recovery state, GUI, original process, or mutable REA state was changed.
