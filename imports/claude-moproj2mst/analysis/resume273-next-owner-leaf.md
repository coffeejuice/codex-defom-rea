# CP273 next owner-command leaf: heat-treat mode synchronization

## Candidate

The smallest new internal owner-command consolidation is to replace the three duplicated `GuiModel::setHeatTreatProperty{Deformation,Diffusion,PhaseTransformation}` implementations with one private/internal `setHeatTreatPropertyMode(HeatTreatMode, bool)` command, while retaining the existing three call sites as thin adapters or converting the single `syncOwnerMode` dispatch to the command. This is an implementation-boundary cleanup only: preserve the property slot API and every value/refresh ordering. The caller remains `HeatTreatProps::applyValue` / `getValue` through `syncOwnerMode` in `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp`.

The command must perform exactly one `simCtl()` admission, return on null, then update only `trans[1]` for Deformation, `trans[3]` for Diffusion, or `trans[2]` for PhaseTransformation to 1/0. No generic pointer/offset argument should escape the owner. Preserve the caller order: for setters, engine defaults if needed, property flag assignment, owner synchronization, then `ht::propsRefresh`; getters synchronize the current flag before returning it. Do not merge STEPDEF or schedule state into this command.

## Evidence

- `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp::setMode` records the exact sequence: optional schedule engine-default initialization, flag update, `syncOwnerMode`, then props refresh. `getMode` writes the engine mode before returning the property flag.
- The same file's `syncOwnerMode` resolves the actual operation model, then dispatches to one of the three named owner methods. This is an actual production caller path, not a fabricated direct helper test.
- `source/apps/moproj2mst/src/engine/gui_model.cpp` lines near 696–705 contain three near-identical methods. They each admit through `simCtl()` once and update only one documented TRANS word; no additional effects occur.
- `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp::heattreat_property_simcontrol_writes_and_null_step` is the concrete test seam. It exercises all three setter and getter paths through `HeatTreatProps`, checks the selected word and sentinels in the other two, and checks detached-step no-write behavior.
- Original qualification is retained under `source/docs/re/moproj2mst/` for the HeatTreatProperties L2 slots; `source/docs/re/moproj2mst/mdt/L2/ledger.tsv` identifies the relevant native mode setter/getter entries and the base TRANS setters. The slot behavior and destinations are already represented in the test and source notes; this proposal does not assert new runtime behavior.

## Files and validation seam

Production edits, if selected: `source/apps/moproj2mst/src/engine/gui_model.hpp` (declaration/type visibility), `source/apps/moproj2mst/src/engine/gui_model.cpp` (single owner command and removal of duplicate bodies), and `source/apps/moproj2mst/src/model/props_heattreat_slots.cpp` (dispatch). The actual caller regression is `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp`; retain and run `m2m_mdt_l6_slots` / `m2m.mdt.l6_slots`, then root's grouped/source-stable validation and freeze.

The narrow regression should keep the existing per-mode sentinels and null-step assertions. Add no broader property behavior, test fixture, public ABI, or GUI claim. Verify exact command admission count structurally and preserve detached-step unchanged controls. This is a refactoring seam; it does not improve or claim lifecycle acceptance.

## Screening and limits

This is intentionally separate from CP272's Save four-field tuple and Slice dimension gate, and from CP271's stopping Fill owner query. It does not revisit those candidates. It does not touch original processes, GUI control, REA mutable state, source, tests, builds or shared recovery state. This read-only scan made no behavioral claim beyond retained source and test evidence. Since the three methods are already narrowly named owner commands, consolidation is a modest architecture improvement rather than a missing correctness fix; do not prioritize it ahead of unresolved higher-value lifecycle evidence if root's queue has a stronger ready leaf.
