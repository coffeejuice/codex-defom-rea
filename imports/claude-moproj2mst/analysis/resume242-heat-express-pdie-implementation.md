# CP242 HeatTransferExpress primary-object command implementation

## Change

Moved HeatTransferExpress's final post-load control access into `GuiModel::setHeatExpressPrimaryObject()`. The caller keeps its order: `expressCopyPrevWizard`, `expressBasePostLoad`, `setTrans1`, model lookup, then the command. The command performs one `simCtl()` access first (therefore retaining the geometry barrier check), returns without mutation when the control is null, performs the unused control `+0x240` read, and sets `pdie1` from the current `objectCount()` (`1` only for exactly one object, otherwise `2`). It does not expose the native control pointer.

The CP242 review passed and specified existing caller fixtures as sufficient. No test source change was made; AirConvect 2D and HeatExpress 3D exercise one-object PDIE 1, and HeatResting 2D exercises two-object PDIE 2. The assigned files already had unrelated uncommitted CP241 non-type-7 count changes; those were preserved. The focused candidate build includes those working-tree changes, as reflected in its source fingerprint.

## Validation

- `python admin/build_candidate.py --label cp242-heat-express-pdie --target m2m_mdt_engine_l3` succeeded (no `--then-full`). Receipt: `analysis/cp242-heat-express-pdie-build.json`; source tree SHA-256: `595bd9ff1e138bf53f8258af578e192529d04e5dcca98eca0575bdc914419ed6` (3,563 files, stable during build).
- `ctest --test-dir build/candidate -R '^m2m.mdt.engine_l3$' --output-on-failure` passed: 1/1 registered test.
- Executable SHA-256: `BC89B45AB782E80758585A0BBE9120DA8232EB4B2FC8B17A28EE653CDE01B274` (`build/candidate/bin/m2m_mdt_engine_l3.exe`).

## Source hashes at validation

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `FCA855AFDC5528FA11B0A2E812579E047F24937CDA9F558489F9C3A0DF2DB9D4`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `025FE876F3D5243D29C468AEA2A23E68391D882415B744D095B1F378DFBBB1A9`
- `source/apps/moproj2mst/src/engine/loader_l3.cpp`: `F307B33E64DF7CF57CF2CA87F1050D0798D0F0B966966EB9B95F93FB6C982C86`
- `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp`: `17C95D2AC6AAB7559E2662DF47D8427089ABC059497DB7938CF89B5B9DFBC517` (pre-existing working-tree changes; unchanged by this task).

## Evidence limits

This validates compilation and the converter's retained caller fixtures. It is not a fresh original application execution or a new original dynamic capture. The original behavior remains qualified by the callback annotations, L3 spec, and ledger identified in `analysis/resume242-loader-next-leaf.md`; no broader parity claim follows from this focused test.
