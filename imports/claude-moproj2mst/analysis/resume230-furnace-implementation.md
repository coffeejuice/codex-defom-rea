# CP230 furnace owner-command implementation

## Change

Added `GuiModel::initializeFurnacePostLoadDefaults()` in `engine.hpp` and `gui_model.cpp`. It calls `simCtl()` exactly once before any mutation, sets `furnace_on = 1`, and queries `curve_count()`. Only an empty curve list triggers `setup_curves()` with a local `const std::int32_t` value of `0x16`, followed by `set_defaults()`.

`furnacePostLoad` now calls the command after the unchanged `opModel` lookup and null return. The later `furnaceProps(c.propsOf(nullptr))` expression remains after initialization. The existing `furnace_postload_step_scope` test already covers the zero-curve defaults path and populated-curve preservation path, so no test-source change was necessary. No C ABI or cache1 changes were made.

## Contract and limits

The command preserves the existing geometry barrier because its first operation is `simCtl()`. It retains the legacy unchecked null-step precondition: with no `step`, `simCtl()` returns null and the following dereference is unchecked. This command does not claim behavior for a null step, and that condition was not runtime-tested. Furnace native calls remain inside the owner method; no control pointer is returned or cached.

This is source reconstruction evidence and a focused regression, not a new original-runtime observation or full lifecycle acceptance result.

## Validation

- Testkit preflight: `test_loader_l2.cpp` includes `testkit/check.hpp`, has a `testkit::run_all()` entry point, and uses the supported Testkit assertion macros; no C `assert` or missing-file probe was added.
- `git diff --check`: passed.
- Focused Release build: `python admin/build_candidate.py --label cp230-furnace-focus --target m2m_mdt_loader_l2` passed. Its bound source hash before and after the build was `bd48a72ac3e59496ae09897e27d39ab99ff7f2169757619fca5e34672e35b9ab`; the build took 18.984 seconds. Receipt: `analysis/cp230-furnace-focus-build.json`.
- Focused CTest: `ctest --test-dir build/candidate -R '^m2m\\.mdt\\.loader_l2$' --output-on-failure` passed, 1/1 registered test.
- A direct CMake build without the repository's Visual Studio environment wrapper initially failed resolving standard-library and Windows SDK headers. The established `admin/build_candidate.py` wrapper supplied the environment and the focused build passed; no source correction was needed.

## Source identity

The three assigned files are stable after the focused build. Their SHA-256 values are:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `9de751f5a8176308f82a5e27207da6b47970fbcee3616bb112d628e1a07abc3e`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `d0121d57935382c1a6cc00b92d3b187bf97d03e6efa17a86cc4dce69d78fc803`
- `source/apps/moproj2mst/src/engine/loader_l2.cpp`: `4ca2869d371694d9c700ee520177dfce5395a572a6b05b085323fd8966dfcde2`

The two shared model files contain prior CP225–CP229 changes outside this checkpoint in addition to the new declaration/definition. Those changes were preserved. Complete source-stable integration, full CTest and checkpoint freeze remain with the integration owner.
