# CP243 editor initial-page dimension query implementation

Implemented the reviewed scalar migration. `GuiModel::dimensionOr(int whenNoStep) const noexcept` reads the live step dimension without a guard and returns the supplied fallback only when the step is absent. The Environment view-factor aggregate now calls `model.dimensionOr(3)` at its existing argument-construction point after Stopping, Meshing, and Iteration and before Environment Apply. Existing curve and definition arguments and caller lifecycle cases remain unchanged.

Added direct tests for live dimensions 2 and 3 (regardless of fallback), live dimension zero, and absent-step return values 3 and -17. The absent-step test also confirms the existing `dimension()` zero behavior.

## Focused validation

- `python admin/build_candidate.py --label cp243-dimensionor --target m2m_mdt_gui_model_dimension --target m2m_mdt_simcontrol300_lifecycle --target moproj2mst_cli` — configure and focused build succeeded (exit 0); no `--then-full` used.
- `ctest --test-dir build/candidate --output-on-failure -R "^m2m\\.mdt\\.(gui_model_dimension|simcontrol300_lifecycle)$"` — 2/2 passed.
- Build receipt: `analysis/cp243-dimensionor-build.json`; source tree SHA-256 `0450cfba4be3a69c2e202531999639e776800e3db119445197f48b04598e0b00`, source-stable during build.
- Executable SHA-256: `moproj2mst.exe` `0d8502663e26e8c11c75e0262f4a298800c6896ac92023aed9cce5f135cd6e24`; `m2m_mdt_gui_model_dimension.exe` `c26ef56524646f985bdf9ae9473ba762037782d0301181913d2eca2aa0317a82`; `m2m_mdt_simcontrol300_lifecycle.exe` `8da3e5cf4d16157776d638bb2d44e09a5d7949775b293d48a19c53070061af7a`.

Modified-file SHA-256 values:

- `engine.hpp`: `b2787dca14fc84a8afa247af453b803135f1a465feaf62c3132cf050c848a7ee`
- `gui_model.cpp`: `69d5262acd3bcbb16f19930b404783378ce9b8aca2a769f4fb90cdf15b3b2fae`
- `simcontrol300_lifecycle.cpp`: `81f03757856b0e0d1808f4f29f9d5638ab728452a4d088bee21cabc63e6acdb5`
- `test_gui_model_dimension.cpp`: `bd5827ba97e03e6b1c40eb0bfeab9a26c1ce746dbfe0b8a404f5dc152c699a0a`

## Limits

This is focused build and test evidence only. No full suite, candidate freeze, original application, GUI, or REA comparison was run. The fallback 3 remains this adapter's EnvironmentViewFactorState assembly behavior; this does not establish an original getter fallback or whole-lifecycle support for absent-step/zero states. The reviewed timing qualification remains: the scalar query stays at the current adapter argument-construction point and does not prove native live-query timing. No C ABI/cache1 behavior was changed.
