# CP268 transition owner implementation

## Change

In `source/apps/moproj2mst/src/engine/simcontrol300.cpp`, the first dimension
term in `transitionMeshingObject3D` now uses `model.dimension() != 3`. Event
admission, requested-index/combo matching, and the negative-index early success
remain ahead of that query. Its position before row bounds/stored-index checks,
control conversion, and previous-row commit is unchanged. The current
`GuiModel::dimension()` returns zero for a missing step, preserving that
nonnegative refusal result.

The working diff also retains the separately reviewed CP260 const
`applyMeshing3D` and CP263 mutable `applyMeshingSelection3D` substitutions.
There are exactly three one-term hunks in `simcontrol300.cpp`; no other source
or test file was changed for this task. The existing actual-entry
meshing-selection tests were sufficient, so no test addition was needed.

## Validation

- Test source preflight: `python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp` passed.
- Focused target build: `python admin/build_candidate.py --label cp268-transition-owner --target m2m_mdt_simcontrol300_meshing_selection` passed; source tree was stable during the build. Receipt: `analysis/cp268-transition-owner-build.json`.
- Exact CTest `m2m.mdt.simcontrol300_meshing_selection`: passed, 1/1. Log: `build/logs/cp268-transition-owner-ctest.log`.
- `git diff --check`: passed.

## Hashes and qualification

- `simcontrol300.cpp` SHA-256: `78c8cf27fd0292429385abe231111f06ba6bd9bc3d8533bd3e8cb29fd6fbf651`
- Focused test executable SHA-256: `2d66ced99ffc19ce2cc8c9fb04839097854da516e28ef04777eb199ddfc73510`
- Build receipt SHA-256: `f8cc119589904086c6f8c021c6ed9de0ee8b4f5cc589db28ae5e2781d02bc34f`
- CTest log SHA-256: `71cd3dd04f92d0c8f83b2a9d8e299ec17bd75500068639aec18e7bb921b50739`

This validates the reconstructed C++ owner entry in the focused Release build.
It does not establish public ordinary-MO GUI reachability, fresh native runtime
parity, lifetime closure, or completion of the broader recovery objective.
Root integration/build/freeze gates remain separate.
