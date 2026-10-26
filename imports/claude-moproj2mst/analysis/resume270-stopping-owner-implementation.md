# CP270 Stopping owner-query implementation

## Change

Implemented the PASS-reviewed rank-3 substitution in `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `applyStopping3D` now begins admission with `model.dimension() != 3`. The existing left-to-right `model.object(0)`, MDS mode bounds and widget-profile checks remain in place. No subsequent validation, stores, selectors, callbacks or rotation behavior changed.

Added actual-entry coverage to `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`. A valid page filled from a live 3D model is applied to (1) an ordinary 2D model and (2) a model whose step is reset. Each call receives a separately owned live `SimControl`; its full object bytes are compared before and after. The missing-step case also asserts that Apply does not recreate the step. Existing success, rotation reselection and partial-validation tests remain present and passed.

The production source already contained the separately owned CP269 `fillMeshing3D` dimension-query edit before this work began. It remains untouched. The CP260/263/268/269 meshing changes and CP269's separate tests were preserved.

## Validation

- `python admin/build_candidate.py --label cp270-stopping-owner --target m2m_mdt_simcontrol300_snapshot` — configure and focused target build both exit 0; source fingerprint stable.
- `ctest --test-dir build/candidate -R '^m2m.mdt.simcontrol300_snapshot$' --output-on-failure` — 1/1 passed.
- Build receipt: `analysis/cp270-stopping-owner-build.json` (SHA-256 `C5E6E0146A6E642316A2ED609D8BE09E38F9F8C536F4D9F46B2BAB74EC0295D3`).

## Scope and limits

This checks the reconstructed actual Apply entry and owner-query admission only. It does not establish ordinary public MO UI reachability, new original-runtime behavior, or guard behavior before earlier lifecycle pages. No full build/suite, GUI, original process, mutable REA, shared recovery-state edit or freeze was performed.

Final working-file SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `4127CAB7601C3444AA88FEAA2571414722B9B1D6298DF2FE334887E692D2EE4A`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: `6E69C5AA17742209A59727A990FE29A882E69BEB23CFDAD2852546640826CB15`
