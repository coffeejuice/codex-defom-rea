# CP231 furnace prologue owner-command implementation

## Change

Added `GuiModel::setFurnaceLoadTnow3()` in `engine.hpp` and `gui_model.cpp`. It writes `step->sim_ctrl.tnow3 = 1` only when `step` exists; otherwise it returns without mutation or creating a step.

`furnaceLoadPrologue` now resolves `opModel` and invokes the command after the unchanged sequence `baseLoadPrologue`, `setTrans2(false)`, `setTrans1(true)`. TNOW3 remains in the load prologue and is not moved into furnace post-load. No C ABI, cache1, or unrelated aliases changed.

Extended the existing `heat1_as_furnace` Testkit case to temporarily move the model's step out, call the command, verify the step remains null, then restore it before fixture teardown. The existing full-load assertions continue to cover TNOW3 becoming 1.

## Contract correction and limits

The CP231 design note asked to preserve a geometry-barrier refusal in `withOpStep`. Inspection of the current implementation disproved that premise: `engine_internal.hpp` explicitly says the wrapper does not add a geometry guard, and its body resolves `opModel` then passes `model ? model->step.get() : nullptr` to the callback. The original callback null-checks before writing. Therefore adding `requireGeometrySyncAvailable()` would introduce behavior not present in the current source. The owner command intentionally has no geometry guard and preserves the nullable no-op.

This is current-source reconstruction and focused regression evidence. No original runtime occurrence or fresh original comparison was performed. The null-step test establishes no-op behavior for the command, but does not add a null-step full-loader scenario or a geometry-barrier fixture.

## Validation

- Testkit/static binding preflight: `python admin/check_new_cpp_bindings.py source/apps/moproj2mst/src/engine/gui_model.cpp source/apps/moproj2mst/src/engine/loader_l2.cpp source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp` passed; all reported files had no issues and includes resolved.
- `git diff --check`: passed.
- Focused Release build: `python admin/build_candidate.py --label cp231-furnace-prologue-focus2 --target m2m_mdt_loader_l2` passed. Source hash before and after the build was `0dc18833632c231a0366f61b49465b76e6a5ae760c92ff2c99a0e727c143f155`; the build took 1.875 seconds. Receipt: `analysis/cp231-furnace-prologue-focus2-build.json`.
- Focused CTest: `ctest --test-dir build/candidate -R '^m2m\\.mdt\\.loader_l2$' --output-on-failure` passed, 1/1 registered test.
- No `--then-full` build or full suite was run.

## Source identity

The four assigned files are stable after the focused build. Their SHA-256 values are:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `807e741eb234c933ea7bf9eb53edd0d23d8f589360bdab4fed7fe18d77b14dce`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `5bfabd17c4f4d7ee0c9ccc1929497024dd7ee685d592401d41f88990548d78d7`
- `source/apps/moproj2mst/src/engine/loader_l2.cpp`: `5aa1a6e574c52dfc9e22244380f7aedf90c78f297884772fb89bc678a80fdd32`
- `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp`: `f4da9ef211f8f9653fc20d6241a675c06308c95ab86763c517b15d6db0e72db0`

The shared model files also contain prior CP225–CP230 work, which was preserved. Complete source-stable integration and checkpoint freeze remain with the integration owner.
