# CP228 loader unit owner implementation

## Change

Added `GuiModel::setModelUnit(std::int32_t, bool)` as a `void` owner command that delegates directly to `def::key::set_model_unit(*step, unit, reset)`. `newModel` now calls `m->setModelUnit(p.unit, true)` after constructing the shared owner and before returning it. The constructor and GUI simulation-control initialization still run before the unit command; equality handling, reset behavior, uncaught exceptions, and return of the same shared object are unchanged.

Added `tests/mdt/test_gui_model_unit.cpp`. It compares changed-unit behavior against a separately initialized `GuiModel` using the native setter, compares equal-unit behavior including preservation of a seeded reset-sensitive value, and invokes the installed `mst::g_loaderSlots.newModel` seam for changed and equal `Project.unit` values. The test uses the Testkit `TEST_CASE`, `REQUIRE`, `CHECK_EQ`, and `CHECK_BITS` macros. Its quoted includes resolve through the target's declared include directories. No C `assert` is used.

## Validation

- `git diff --check`: passed.
- Focused candidate build: `python admin/build_candidate.py --label cp228-loader-unit-owner --target m2m_mdt_gui_model_unit` completed successfully. No `--then-full` option was used. Receipt: `analysis/cp228-loader-unit-owner-build.json`.
- Build receipt: source fingerprint `fa038370d00de1f38ba3ef316b2d0e3e4f4599cf13b1b9b94e26246dcde8bee8`, 3,563 source files before and after; `source_stable_during_build: true`.
- Focused CTest: `ctest --test-dir build/candidate --output-on-failure -R '^m2m\.mdt\.gui_model_unit$'`: 1/1 test passed, 0.01 sec; the executable reported `0 of 3 cases failed`.

Current SHA-256 values:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `AF2C218CC4DD0A8DF758F8CA15641ECA3F61E9AA0D093B125C76ADC3D8FBD544`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `FC1541E671A312E3AEC53DEE461FEFE0964D41A1F8B116FF4494C21F6B9A1042`
- `source/apps/moproj2mst/src/engine/loader.cpp`: `66C4F7D3D4F7A85AC786CA99BA45FD64563A115C5EDF46BD3E6EBFAC96389C05`
- `source/apps/moproj2mst/tests/mdt/test_gui_model_unit.cpp`: `0C6A5FC0EB8778386C6EE7332660497D968452E14FC648BF775F5C1F54952B19`

## Scope and limits

The C ABI headers/exports and cache1 storage/publication were not edited. No corpus, original executable, GUI, REA, or shared-state operation was used. Exception propagation and local-owner unwinding remain code-derived; there is no injected setter failure seam and this test does not claim runtime failure-path coverage. Other in-progress source and analysis changes are present in the shared checkout; the candidate receipt establishes source stability only across this focused build, not an integration freeze. Root integration should run the changed-target group and create the checkpoint freeze under the source-stable workflow.
