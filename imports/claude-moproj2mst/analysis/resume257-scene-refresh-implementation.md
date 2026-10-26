# CP257 scene refresh implementation

Implemented the approved fit refresh prefix in `source/apps/moproj2mst/src/engine/viewer_camera.cpp`. `fit` now refreshes a live, shown main model, then a live, shown secondary, then a live, shown third model, all before `sceneBox` and camera work. Each slot keeps an independent guard-before-shown check. The existing `viewerRefresh` behavior and the scene-box/camera/reset-view implementation are unchanged.

Added `display_refresh_scene_refreshes_only_shown_main_secondary_and_third_caches` to `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`. It exercises the production `displayRefreshScene` caller with mutated curve coordinates and cached per-model boxes. It proves hidden secondary and third caches stay stale, shown secondary refreshes while third stays hidden, both shown slots refresh in one call, and a shown third is still visited after the canonical secondary is removed through `displayDropSecond`. The test does not infer secondary/third contributions to the fitted camera box. This avoids an artificial null-guard/live-owner state.

## Validation

- `admin/check_new_cpp_bindings.py` passed for the changed test file with no issues.
- `admin/build_candidate.py --label cp257-scene-refresh-final --target m2m_mdt_viewer_box` configured and built the focused target successfully. Receipt: `analysis/cp257-scene-refresh-final-build.json`.
- `ctest --test-dir build/candidate -R '^m2m\.mdt\.viewer_box$' --output-on-failure` passed: 1/1 test. Output: `analysis/cp257-scene-refresh-focused-ctest.log`.
- After the review correction, binding preflight and `git diff --check` passed again. `admin/build_candidate.py --label cp257-scene-refresh-reviewfix --target m2m_mdt_viewer_box` succeeded with `source_stable_during_build: true`; receipt: `analysis/cp257-scene-refresh-reviewfix-build.json`. The focused CTest again passed 1/1; output: `analysis/cp257-scene-refresh-reviewfix-ctest.log`.
- The review-fix build ran with CP258 source edits held. These were focused target builds against a stable shared source tree, not a frozen or isolated whole-tree candidate; no full suite or freeze was run.

## Source hashes after validation

- `viewer_camera.cpp`: `4a5032f70afd4008057d3c213f7b4f1a812edfc765085b41c0c18aed291ec668`
- `test_viewer_box.cpp`: `8acd6417e58a61e3946fffc696691f2b9c057304ec8ca0119f842a3a3814cf7d`

Hashes are also recorded in `analysis/cp257-scene-refresh-hashes.json`. The review-fix preflight, build, and CTest receipts are available for integration review. The source ownership can now be released.
