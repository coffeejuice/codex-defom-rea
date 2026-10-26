# CP184 R300 material fixture eligibility

## Decision

The copied corpus operation 2 fixture is not yet proven eligible for `saveMaterials`. The available read-only source and corpus listing establish that the project has two shared material KEY files (`Material00001.KEY`, `Material00002.KEY`), but do not establish the loaded operation step's material count or each material's type at `+0x1cc8`. Shared project files are not a substitute for the operation step's live material list. Therefore do not claim corpus-loop coverage from the existing Save sweep or infer type eligibility from the filenames.

The minimally justified next fixture is a direct internal-helper test on the real loaded operation, labeled synthetic helper mechanics. Load and activate `46_MO2_Boolean_Oprations.moproj`, select operation ID 2, install loader slots, and call its `loadOperation` as in `base_default_pairs_step_scope`. Before invoking `saveMaterials`, inspect `step->n_materials` and `material_list_nth` for indices 1 through n, recording each non-null material number (offset 0) and type (offset `0x1cc8`). Proceed with assertions only if at least one entry is non-null and type != 2. If this live list has no eligible entry, construct a narrowly synthetic list only while preserving the real loaded operation/model invariants; do not call public Save to imply dispatch reachability.

## Evidence and limits

- `analysis/resume184-r300-next.md` calls for proving a non-type-2 material on this fixture before treating the loop as covered.
- `analysis/resume184-r300-fixture-resolution.md` explicitly says the existing fixture is known to load but its material count and non-type-2 presence are unknown; it prescribes the direct-helper route.
- `tests/s/test_save_current.cpp` loads this exact project, activates its subproject, selects operation 2, installs loader slots, and loads the operation. It asserts `SfOprHeatTransferExpress2D`, but does not inspect material count or types in that setup.
- `source/apps/moproj2mst/src/engine/save_current.cpp` iterates `1..n`, skipping null entries and type 2 before deriving the material KEY path and calling `saveMaterialKey`.
- The corpus contains two shared material KEY files, but corpus file presence alone cannot show which are represented in operation 2's loaded `DB_STEP_DATA::materials` or their filter types.
- No GUI, app-control, REA database, mutable test runner, build, or corpus write was used in this inspection.

## Exact next action

In `tests/s/test_save_current.cpp`, extend the existing real-operation setup (after `loadOperation`) to inspect and assert the loaded material count/list before calling `engine::saveMaterials(p, *op, fx)`. If an eligible item exists, assert exact expected `materialDefaultFile(p, no)` paths and nonempty captured KEY bytes for eligible entries, with skipped entries absent. Keep this explicitly a helper-level mechanics test. If none exists, use a focused synthetic material-list fixture based on that same loaded model and record the artificial list contents as synthetic. Preserve null-step coverage with RAII restoration and no-effect assertions as already specified in the fixture-resolution note.
