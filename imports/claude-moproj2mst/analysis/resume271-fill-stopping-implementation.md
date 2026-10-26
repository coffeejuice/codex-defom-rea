# CP271 fill stopping dimension owner implementation

## Change

Changed only the first admission term in `fillStopping3DInto` from the direct `step` null/dimension read to `model.dimension() != 3`. The existing `model.object(0)` remains the next short-circuit term, and the remainder of fill order/effects is unchanged.

Added an actual-entry snapshot test covering an ordinary 2D model and a model with `step.reset()`. Both use a separately owned, live `SimControl`; each asserts `InvalidModel` at Preflight, page-value and control-byte preservation, and no step recreation. The existing fill success, partial-fill, callbacks, Apply and lifecycle cases remain intact.

The working source already included the separate CP270 `applyStopping3D` change and CP269 `fillMeshing3D` change when this leaf began. Those predicates and the CP260/263/268/269 meshing work were preserved; no other files were edited for CP271.

## Validation

- `python admin/build_candidate.py --label cp271-fill-stopping-owner-retry --target m2m_mdt_simcontrol300_snapshot` — configure and focused target build passed; source fingerprint remained stable. Receipt: `analysis/cp271-fill-stopping-owner-retry-build.json`.
- `ctest --test-dir build/candidate -R '^m2m.mdt.simcontrol300_snapshot$' --output-on-failure` — 1/1 passed.
- An earlier build attempt exposed the test framework's inability to format `StoppingFillStage` through `CHECK_EQ`; assertions were changed to boolean equality and the retry passed. A repeated receipt label was avoided because the first attempt had already created its receipt.

## Scope and limits

This validates the reconstructed fill entry's absent/non-3D admission and preservation cases. It does not establish public MO UI reachability, original-runtime behavior, parked-geometry behavior by runtime test, GUI behavior, or full-project regression. No full build/suite, freeze, GUI, original process, mutable REA, or shared recovery-state change was performed.

Working-file SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `64F7A3FBED21F62316D1C1B2EB8CAB181608247009D1F87ED115705B154FCDD1`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: `B096E407DF2E8AA0D5F1DDA9493C44D14885346DF800CE855434AB56181A8708`
