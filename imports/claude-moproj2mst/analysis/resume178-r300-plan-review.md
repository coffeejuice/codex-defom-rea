# CP178 R300 plan review: L6 `simCtl` callback cut

## Decision

**Accept the proposed boundary and behavioral constraints; implementation appears already present in the frozen CP177 files.** The plan describes a migration from raw `simCtl(p, ...)` returns, but the files at the exact plan baseline hashes already contain `withOpSimCtl` and its migrated call sites, plus the requested Distortion null-step test. Treat CP178 as a source/test review of that existing state, not as authorization to repeat the migration. No test/build was run in this read-only review.

## Evidence

- `source/apps/moproj2mst/src/engine/l6_slots.cpp` SHA-256 is `42b36efbffcfbf997fed53416459670d58db31ce0c235026baacc9bd8ee37e58`, matching the plan's stated baseline hash. It contains a translation-unit-local `withOpSimCtl` constrained with `std::is_same_v<std::invoke_result_t<F, Cls_1401e7b10*>, void>`. It calls `engine::opModel(p, op)` once and immediately invokes the writer with `m ? m->simCtl() : nullptr`.
- `rg` finds 13 `withOpSimCtl` acquisitions: four HeatTreat, five Distortion, two post-load slot-name writes, and two Diestress. No raw `simCtl(p, ...)` helper or call remains.
- Distortion retains `baseLoadPrologue` first; TRANS writes remain 1 then 0; the four defaults remain grouped and ordered at `+0x16c`, `+0x164`, `+0x160`, `+0x1d0`; `+0x2e0` remains before the `c.current` guard; and the final acquisition still uses `*c.current` after the guard.
- `GuiModel::simCtl()` calls `requireGeometrySyncAvailable()` before returning `step ? &step->sim_ctrl : nullptr` (`src/engine/gui_model.cpp`, lines 179–182). The callback therefore preserves the geometry refusal and null-step behavior. Replacing it with direct step access or `withOpStep` would violate this property.
- `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` SHA-256 is `ecc675d7711b40370efc170e8a38b76a7bb46c4a7375918791e615fa5cf0ff3e`, matching the plan's stated baseline hash. It already contains `distortion_prologue_null_step_preserves_simcontrol`: it reclasses an admitted Distortion owner, obtains its cached model and registered prologue, checks `cls->current == op`, snapshots distinct control sentinels, moves out the actual step only during direct slot invocation, restores on normal and exceptional exits, then verifies sentinels remain unchanged.
- Existing positive fixtures named in the plan are present: `heattreat_schedule_flag_set`, `heattreat_engine_defaults`, `distortion_props_from_sim_control`, and `diestress_without_db`.

## Acceptance and remaining work

Static source/test review meets the proposed acceptance criteria. No source-level blocker found. The plan's “add” instructions are already satisfied at the exact listed hashes, so an implementation turn should first reconcile that the source is already in the desired state and avoid editing these paths. Runtime acceptance remains unverified here: run the listed focused CTests, then the specified source-stable suite/freeze in the authorized implementation workflow. This report makes no claim that those tests pass.
