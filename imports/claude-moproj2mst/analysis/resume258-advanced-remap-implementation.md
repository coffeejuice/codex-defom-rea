# CP258 Advanced Apply final remap owner command

Implemented the reviewed fixed-false Advanced Apply remap extraction. `GuiModel::remapEditorAdvancedState()` calls `def::model::step_remap_material_state(*step, false)` with a direct include of the declaring `defmodel/material/state_remap.hpp` in `gui_model.cpp`. `applyAdvanced3D` calls that command at the former final native call position: after all three ELMNOD stores and PDIE2, immediately before `Completed`. No guard, null handling, exception policy, return alias or `noexcept` was added.

The existing unconditional actual-caller tests in `test_simcontrol300_snapshot.cpp` already verify both meaningful early-stop/remap-skip cases and successful remap effects with distinctive seeded state. No extra direct command test was added. Source inspection establishes the literal false argument and final ordering.

## Validation

- Binding preflight passed: `python -B admin/check_new_cpp_bindings.py --project-root source/apps/moproj2mst tests/mdt/test_simcontrol300_snapshot.cpp`. Receipt: `analysis/resume258-advanced-remap-binding-preflight.json`.
- `git diff --check` passed for the four assigned files.
- Focused candidate configure and target build passed (exit 0) via `admin/build_candidate.py --label cp258-advanced-remap-focused --target m2m_mdt_simcontrol300_snapshot --target m2m_mdt_simcontrol300_lifecycle`. Receipt: `analysis/cp258-advanced-remap-focused-build.json`; logs: `build/logs/cp258-advanced-remap-focused-configure.log` and `build/logs/cp258-advanced-remap-focused-build.log`.
- Exact CTest discovery found `m2m.mdt.simcontrol300_lifecycle` and `m2m.mdt.simcontrol300_snapshot`. Both passed, 2/2, in 0.07 seconds total. Log: `analysis/resume258-advanced-remap-focused-ctest.log`.
- Build source fingerprint was stable: SHA-256 `f6cd8c0e49ca627e7839b304dbb69dbccf881907728351d7e5b8167127a517bf` before and after, 3563 files. The validated tree includes the CP257 scene changes; this is combined-tree focused validation, not an isolated CP258 candidate.
- Candidate artifacts: `m2m_mdt_simcontrol300_snapshot.exe` SHA-256 `7654c0308f44ffc0956e8596738b4951094f99689a26f6cbd7e521e9a09c2b86`; `m2m_mdt_simcontrol300_lifecycle.exe` SHA-256 `4a71d0c1020d932a3b643ff7df6924e8fed8576808a17198621cc46a406fb573`.

No full suite or freeze was run. The shared build/candidate slot is released to root.

## Working-file hashes

These are the shared working-file hashes at final validation. They include other in-progress changes in shared files and are not CP258-only patch hashes.

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `011459d4cc34936a937e5bda58f87978c4fa449d5006f7dcbfced3c00fa28c6d`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `879e2d7e7ac7099e0d0591f094d890ab9b68df7fa446a061e882babca2ab92d4`
- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `7e5e26cfa9070935fe9f29cebcfb05b59b780e7db7e5411c267a9cd95f61419f`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: `0f52dff5154f5fb70c4566be753f03d0183a9fc0fba82c0ea66e4032530ac250`
