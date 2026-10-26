# CP259 fixed-slot scene-box implementation

Implemented both approved fixed-slot contributions in `source/apps/moproj2mst/src/engine/viewer_camera.cpp::sceneBox`: main, secondary, third, each independently checking live guard before shown flag, querying `viewerSceneBox`, then applying existing `boxOf`/`boxUnion`. This refresh=false specialization adds no refresh, admission preflight, deduplication, public API or owner. Named-holder traversal remains excluded.

Added `display_refresh_scene_fits_shown_fixed_slot_box_union` in `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`. It invokes the actual `displayRefreshScene` caller and inspects `displayCamera` with literal window expectations: hidden secondary/third leaves main +/-2.1; shown secondary +/-8 gives +/-8.4; translated secondary [10,14] with main [-2,2] gives L/R -2.4/14.4 and T/B +/-8.4; hidden secondary again leaves main; shown third +/-12 gives +/-12.6. The test further removes canonical secondary through `displayDropSecond`, checks the null shown slot, destroys an ordinary temporary third owner to check an expired shown guard, and binds a live empty third to check invalid-box identity. Main/third/empty fixtures outlive the project display. No guard/live-owner mismatch is manufactured; no named fixture is present.

Window arithmetic expectations follow the reviewed native union/window path. No origin assertion is made. The separate native radius query/store gate, two-query fit sequence, named traversal, exceptional floating behavior and complete multi-model camera parity remain unresolved. Current converter's union-radius substitution is unchanged.

## Validation

- `python admin/check_new_cpp_bindings.py source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` passed; receipt `analysis/cp259-scene-box-binding-preflight.json`.
- `git diff --check` passed.
- `python admin/build_candidate.py --label cp259-scene-box --target m2m_mdt_viewer_box` configured and built successfully; receipt `analysis/cp259-scene-box-build.json`. Source remained stable during build.
- `ctest --test-dir build/candidate -R '^m2m\.mdt\.viewer_box$' --output-on-failure` passed 1/1 (0.40 seconds total); receipt `analysis/cp259-scene-box-focused-ctest.log`.

This was a focused candidate build only. No full suite, freeze, shared plan/state edit, original process, GUI session, mutable REA, reconstructed application rebuild or deployment was performed. Source/test ownership is released for root integration and review.

## Source hashes

- `source/apps/moproj2mst/src/engine/viewer_camera.cpp`: `4b229c374228cbd02cbbfb7ce38f0e5b74f1b3a1f3e0dc6a9d870d628c73f415`
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: `c7514f2c28d97b0c731f572ab77a7acba05bfc42a2fbc4416a9d5c6c2b186509`

Also recorded in `analysis/cp259-scene-box-hashes.json`.
