# CP261 named-holder scene-box implementation

Implemented the independently PASS-reviewed named-holder contribution after main, secondary and third in `source/apps/moproj2mst/src/engine/viewer_camera.cpp::sceneBox`. Each canonical key-ordered holder checks its flag before obtaining its owned model, skips null, queries the current cached box and applies the existing `boxOf`/`boxUnion`. This non-mutating refresh=false specialization uses immediate visits without retaining holder/model collections, changing owners, deduplicating fixed aliases, refreshing named models or adding admission checks. The native map-value snapshot order is represented by canonical vector order only within this non-reentrant path; this does not establish lifetime/ownership parity or a refreshing traversal.

Added `display_refresh_scene_fits_cached_named_box_union` in `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`. The actual `displayRefreshScene` caller is exercised with canonical Zeta-before-Alpha creation and explicit canonical key-order checks, hidden named models, a shown +/-8 cache, two shown boxes with cached union [-8,14], false-flag and shown null owner, live invalid cache preserving valid main, and all-hidden restoration. Directly editing Alpha geometry to +/-30 after caching +/-8 confirms the actual caller continues consuming the named cache. Assertions use literal camera window bounds only and confirm named models do not replace main. No exposed main-shown setter is available, so this test does not manufacture a hidden-main fixture. Finite union bounds do not independently demonstrate visit order or flag-before-query; those are source/static-evidence properties.

The retained native qualifications and PASS review are in `analysis/resume260-named-box-next-leaf.md` and `analysis/resume260-named-box-next-leaf-review.md`. Independent native radius, stored-box gate, origin, named refresh and full fit parity remain unresolved; no claims or assertions on those paths are added. Geometry admission remains unchanged.

## Validation

- Read-only binding preflight passed: `analysis/cp261-named-box-binding-preflight.json`.
- `git diff --check` passed.
- Root released the second build slot after CP260 validation. Focused target `m2m_mdt_viewer_box` configured and built successfully. Initial wrapper receipt writing failed because the output Tee held its same receipt path open; configure/build themselves both returned 0. A receipt recovery invocation under `cp261-named-box-receipt` completed successfully with a no-op build; canonical successful receipt is `analysis/cp261-named-box-receipt-build.json`. The initial `cp261-named-box-build.json` is console JSONL, not a completed wrapper receipt.
- Focused CTest `m2m.mdt.viewer_box` passed 1/1 (0.43 seconds total); receipt `analysis/cp261-named-box-focused-ctest.log`.
- Source/test SHA-256 remained unchanged through validation.

## Source hashes

- `viewer_camera.cpp`: `98298e7a87a1e2d9254dd4f5cc4d09b8bbfadaf615929d59854357df7979f0e5`.
- `test_viewer_box.cpp`: `357a6d0e97e864fe48af261fad729437fd367f58cb41232cc1ff27ec56a2aaa8`.
- Machine receipt: `analysis/cp261-named-box-hashes.json`.

No full suite, freeze, shared plan/state change, GUI, original process, mutable REA, reconstructed application executable build or deployment was performed. Source/test and shared build-slot ownership are released for root integration and review.
