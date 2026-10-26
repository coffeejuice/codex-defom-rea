# CP273 HeatTreat private owner consolidation review — PASS

The implementation matches the restricted CP273 design review. No source fix is required. This is a source-level and focused-build verdict; the exact CTests and root's source-stable integration and freeze remain separate gates.

## Structural checks

- `GuiModel::setHeatTreatPropertyDeformation`, `setHeatTreatPropertyDiffusion`, and `setHeatTreatPropertyPhaseTransformation` remain the same public named commands in `include/m2m/engine.hpp`. Each adapter calls the new helper once with a fixed index: 1, 3, and 2 respectively. The helper is declared in the class's `private` section; no caller-visible selector, pointer, or offset interface was added.
- The shared helper calls `simCtl()` exactly once, before accessing control, and writes only `control->trans[mode] = enabled ? 1 : 0` when non-null. `simCtl()` first calls `requireGeometrySyncAvailable()` and only then resolves the nullable step. Thus the existing geometry refusal still propagates before step resolution, and a missing step still skips the native word write. The fixed indices preserve Deformation at SC+0x108, Diffusion at SC+0x110, and PhaseTransformation at SC+0x10c.
- `props_heattreat_slots.cpp` is unchanged. Its setter still performs pending engine defaults, property flag assignment and bool snapshot, owner synchronization, then props refresh. Its getter still snapshots the flag, synchronizes the owner, then returns a Bool. `syncOwnerMode` still resolves `opModel` once and dispatches through the named commands. No STEPDEF, schedule, default/reset, or external result path changed in this diff.

## Coverage and result boundary

- The retained `m2m.mdt.l6_slots` caller test covers all three setter/getter paths with true and false values, selected-word writes, unchanged unselected-word sentinels, and detached-step no-write behavior. The retained `m2m.mdt.viewer_box` test calls all three named commands on a parked model and checks geometry refusal and unchanged words. Neither test was edited.
- `analysis/cp273-heattreat-owner-build.json` records a source-stable focused configure and build of `m2m_mdt_l6_slots` and `m2m_mdt_viewer_box`, both exit code 0. At this review checkpoint that receipt contains no CTest result. Run the exact `m2m.mdt.l6_slots` and `m2m.mdt.viewer_box` CTests before treating focused validation as complete; root's grouped/source-stable run and freeze are still required.
- The slot mapping and order are previously qualified static evidence as summarized in `analysis/resume273-next-owner-leaf-review.md`. This private refactor supplies no fresh original-runtime, GUI, or lifecycle acceptance evidence.

Only this review file was written. No source, tests, build, GUI, original application, or mutable REA state was changed by this reviewer.
