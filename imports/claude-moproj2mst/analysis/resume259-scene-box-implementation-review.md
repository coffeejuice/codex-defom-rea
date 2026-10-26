# CP259 fixed-slot scene-box implementation review — PASS

The implementation matches the approved secondary/third union design. No fixes are required for this bounded leaf. Actual-caller window tests cover both added contributions and guard/invalid-box behavior; the implementation report preserves the unresolved native radius/store/origin qualifications.

Only this review was written. This review ran no build, tests, original process, GUI control or mutable REA session, and changed no source/test/shared state.

## Source and test assessment

`source/apps/moproj2mst/src/engine/viewer_camera.cpp::sceneBox`, around lines 253–276, now visits main, secondary, then third. Each block checks its live guard before its shown flag, performs the corresponding `viewerSceneBox` query, and folds `boxOf(valid,mn,mx)` through the existing `boxUnion`. No refresh, slot creation, dimension/step check, all-slot preflight, alias deduplication, new owner, public API or retained borrow was added. CP257's separate refresh prefix and the existing fit/store/window/reset/origin sequence remain unchanged. The named-tail comment correctly identifies the remaining exclusion.

This matches the fixed-slot refresh=false branches of retained `evidence/gui_mo/s1/viewer_rect/a1.c::FUN_140cfd750`: main before secondary before third, guarded pointer before shown flag, model-box query followed by accumulator union. The current union retains existing invalid-operand identity and derivation helpers. This review reuses the qualified native design review; it does not independently execute or disassemble the original.

`test_viewer_box.cpp::display_refresh_scene_fits_shown_fixed_slot_box_union`, around lines 691–754, invokes production `displayRefreshScene` and reads `displayCamera().lrtb`. Its literal expectations distinguish hidden slots, the shown secondary extent, translated union `[-2,14]`, restoration to main-only when secondary is hidden, and a shown third with a distinct extent. The translated case checks asymmetric L/R and symmetric T/B, rejecting a max-of-individual-radii replacement for box union. It does not use production arithmetic helpers as its oracle or make an origin assertion.

The same test drops the canonical secondary through `displayDropSecond`, requires both secondary guard and owner empty, then tests a shown null secondary while third still contributes. It creates a temporary ordinary third owner in a scope and destroys it naturally before requiring the third guard expired. A live empty third then checks invalid-box identity against valid main. The permanent main/third/empty models are constructed before `Project`, so they outlive display teardown; canonical secondary remains display-owned. No null-guard/live-owner mismatch is manufactured in this added test. Named models are absent.

The new `<cmath>` include resolves the explicit `std::fabs` tolerance checks. The test retains Testkit's entry point and supported checks. Source review establishes ordered independent slot reads; these ordinary finite tests are not an original callback/failure-disposition witness, and no such claim is made.

## Focused evidence checked

Read `analysis/resume259-scene-box-implementation.md`, the approved design/PASS review, and the exact focused receipts:

- `cp259-scene-box-binding-preflight.json`: `ok=true`, changed test has no issues.
- `cp259-scene-box-build.json`: configure/build exit codes 0; target `m2m_mdt_viewer_box`; source stable during build, with matching before/after tree SHA-256 `8b093dea86db8c8406ee6b35a8495f54e94212b487f9451e6ea57e13c1820341`.
- `cp259-scene-box-focused-ctest.log`: `m2m.mdt.viewer_box` passed 1/1, 0.38 seconds test time / 0.40 seconds total.
- Current focused executable SHA-256 matches the build receipt: `74f0c99fa489914ccc1fc77c62d984f832b00df4b29627a4707f6c79491b63c3`.

Current source hashes independently match `cp259-scene-box-hashes.json` and implementation receipt: `viewer_camera.cpp` `4b229c374228cbd02cbbfb7ce38f0e5b74f1b3a1f3e0dc6a9d870d628c73f415`; `test_viewer_box.cpp` `c7514f2c28d97b0c731f572ab77a7acba05bfc42a2fbc4416a9d5c6c2b186509`. These confirm the inspected source and recorded focused artifact, not a complete integration/freeze receipt. Root's grouped/full checks and source-stable freeze remain required.

## Qualification retained

The implementation report explicitly leaves independent native `cfe0f0` radius aggregation/store gating, native two box queries versus converter one, named traversal, exceptional floating behavior, pending geometry continuation and complete multi-model camera parity unresolved. The test's absence of an origin assertion avoids reopening the corrected native-origin claim. No new geometry admission, live ObjectKey/currentness contract, named no-step exceptional behavior or naturally reached original multi-model occurrence is claimed.

Original evidence remains qualified as retained DEF_GUI_MO_64.exe **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this review relies on the existing static evidence identity, not a fresh original run. PASS is for this implementation and its focused evidence within the approved fixed-slot union scope.
