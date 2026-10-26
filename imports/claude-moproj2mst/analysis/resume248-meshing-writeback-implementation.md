# CP248 meshing writeback implementation

Implemented the approved bounded migration in `source/apps/moproj2mst`.

## Change

`GuiModel::copyEditorMeshingObjectValues(const MeshObjectSnapshot&)` performs one `object(value.number)` lookup and, when present, writes the four doubles in the retained source order: interference `+0x2dc8`, size ratio `+0x2dd0`, tolerance `+0x2de0`, node count `+0x2dd8`. The command retains no native alias and does not add conversion, admission, or failure policy. The const Apply and mutable Selection Apply call it at their former helper locations; their surrounding preflight, per-row continuation, conversions, and singleton update order remain as before. Added a parked-geometry refusal call with distinct local values in `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`.

## Validation

- `admin/check_new_cpp_bindings.py` over the four owned C++ files: passed (no reported issues).
- `admin/build_candidate.py --label cp248-meshing-writeback-focused --target m2m_mdt_simcontrol300_disabled_remesh --target m2m_mdt_simcontrol300_meshing_selection --target m2m_mdt_viewer_box`: passed; focused target build exit code 0.
- Focused CTest `m2m.mdt.simcontrol300_disabled_remesh`, `m2m.mdt.simcontrol300_meshing_selection`, `m2m.mdt.viewer_box`: all 3 passed.
- `git diff --check`: passed.

## Evidence limits and integration note

These reconstructed-source tests validate the finite writeback paths and parked owner-entry guard; they do not establish physical QWidget delivery, original object allocation/lifecycle parity, broader temporary-record compatibility, or fresh original execution. The four-store order is established by inspecting the implementation source, not by the final-state tests. CP165's boundary remains unchanged.

The shared engine header, `gui_model.cpp`, and viewer-box test already contain concurrent CP246 changes outside this CP248 assignment; those edits were preserved and not modified. The hashes below identify the current complete files, not CP248-only diffs:

- `include/m2m/engine.hpp`: `d316b6c7ded917604f31cc3e812ee39a7cebc88718ba48a11085eb6fed156208`
- `src/engine/gui_model.cpp`: `6c51506fedf9f345186ca009e0d241b09519f8c76eb3fef2d966b06d0ac0252e`
- `src/engine/simcontrol300.cpp`: `0be9da5a139f032aba123eb3da7cb122da9963c6068786c5ef7d4feafced2036`
- `tests/mdt/test_viewer_box.cpp`: `09a709ad34b4214abf4fdc382a4cf588a59cd3c8ec3f020126c9196e35f1f5d1`

No full build, checkpoint freeze, or shared plan/state edit was performed.
