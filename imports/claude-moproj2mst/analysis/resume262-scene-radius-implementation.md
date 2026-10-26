# CP262 independent scene radius implementation

The reviewed scene-radius leaf is implemented and its focused Release regression passes. Source ownership is released to root for independent review, grouped/full integration and freeze; this worker ran no full suite and created no candidate freeze.

## Owner and fit behavior

`source/apps/moproj2mst/src/engine/engine_internal.hpp` declares private `viewerAllViewBox`, implemented in `viewer.cpp`. It admits through `viewerOf`, retaining geometry-sync and pending-removal refusals. For dim2 it visits every settings-bearing view without ObjectOn/object-kind filtering, calls existing `recompute2D`, unions the resulting rectangle, and after each admitted contribution applies the axis gate using **cached modelRect**, without overwriting that model cache. The completed local rectangle converts through existing `rectBox`, including valid degenerate conversion. For dim3 it unions settings-bearing cached view boxes without refresh, fallback or outer visibility/kind filtering. Other dimensions return an invalid box. Existing helper exceptions propagate; no unsupported contribution is silently replaced.

`viewer_camera.cpp` independently unions these owner contributions in main/secondary/third/named order with existing live-pointer/shown/holder-flag selection. Aliases retain separate visits. Fit preserves shown fixed-model refresh, scene-present guard, independent query, first scene query/store, second scene query/window, reset and stored-box origin order. Camera width and height are evaluated before validity. Store uses rejection form `valid && !(0.001 > radius) && !(0.001 > width) && !(0.001 > height)`, preserving unordered comparison acceptance at the value-predicate level. Existing BoxD union/radius arithmetic remains canonical.

The behavior derives from the independently PASS-reviewed static original evidence in `resume262-scene-radius-next-leaf.md` and its review: original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No original application or mutable REA session was run for this implementation.

## Executed caller tests

Four new Testkit cases cover five profiles through `displayRefreshScene` and observe `displayCamera`:

- Translated dim3 curve fallback `[10,14]`: private all-view result invalid, scene fallback valid; window is `9.9/14.1/2.1/-2.1` and fresh origin remains zero. Supported populated imported rep then establishes origin -12; a later fallback `[22,26]` updates its window while retaining -12.
- Populated 3D uses existing `M2M_DB_FIXTURE/Objects/Object00001.KEY` via `loadObjectKeyGeometry`, which drops mesh through the existing owner API. Its 242-node/480-face surface has literal extents x±1000 and y/z±250. Public surface scale/translate plus normal `viewerSync` establish the supported rep; private owner result confirms translated bounds [-988,1012]. No synthetic native layout or public cache setter was added.
- Natural dim2 constructor/object initialization is verified by `geo_kw::object_dim==2`. Explicit geotyp2 excludes axis extension. A second view caches [-8,8]; display-option importer sets ObjectOn/GeoOn/MeshOn to zero, with snapshot verification, while the old view rectangle remains. Actual fit resets this hidden view to zero, and shown first view still determines window [-2.1,2.1].
- For geotyp1 and geotyp3, fresh empty dim2 main plus flagged named model establishes cached modelRect [0,14] from a translated [10,14] view while display-associated. Contribution flags are then disabled without refresh. Actual fit resets the view rectangle but must use cached modelRect for axis extension, accepting origin -7. The private owner result also remains [0,14]. An accumulator-derived axis extension would be degenerate and reject this fresh store.
- A surface scaled by 6e-7 and translated to x=.01 has scene width above .001 but independent radius below .001. A larger hidden settings-bearing view enables acceptance while leaving shown scene-window values unchanged. Fresh independent displays for secondary, third and named cases prove hidden display-model rejection followed by enabled-model acceptance, with invalid model scene boxes and valid cached all-view boxes asserted before the caller. Third/named cases include a shown null secondary. Fresh stores prevent prior acceptance from masking missing contributions.

The existing CP259/CP261 fixed/named scene-window cases remain intact; their obsolete unresolved-radius comments were updated.

## Validation and retained failures

`admin/check_new_cpp_bindings.py` passed the changed Testkit file: supported entry point, include resolution and macros. No C assert or REQUIRE_EQ was added.

All compilation used `admin/build_candidate.py --target m2m_mdt_viewer_box`, never direct CMake and never --then-full:

| Attempt | Result | Receipt |
| --- | --- | --- |
| focused1 | Compile failure: Windows `small` macro expanded a local test variable; renamed `smallCamera`, corrected bool comparison warnings | `resume262-scene-radius-focused1-build.json` and retained build log |
| focused2 | Build passed; first CTest 64/65 cases passed. Named fixture retained a large **model** cache after hiding, so its expected tiny window was incorrect. | `resume262-scene-radius-focused2-build.json`; `resume262-scene-radius-first-ctest.log` |
| focused3 | Build passed; corrected fresh per-slot displays and explicit pre-call hidden model refresh. Focused CTest passed all 65 cases (1/1 registered test), 0.45 seconds test time. | `resume262-scene-radius-focused3-build.json`; `resume262-scene-radius-focused-ctest.log` |

Final command: `ctest --test-dir build/candidate -R '^m2m.mdt.viewer_box$' --output-on-failure`. Source-stable build fingerprint: `f18953172cba096fb725d805918307eba03e8b79e6080f909d51f9c2c2f1e442`. Build receipts report source stability on all three attempts. Elapsed build receipt times were 12.791, 8.934 and 9.281 seconds; token measurement is unavailable. No measured efficiency improvement is claimed.

## Limits

These are constructed source-bound caller regressions, not naturally reached original GUI observations. Static unordered comparison behavior is implemented but no unsupported NaN input or signaling-NaN/FPU exception test was introduced. Object-kind filter absence is directly reviewable in source but not separately exercised by mutating a bound object's kind to7. Missing-settings logging, exceptional unwind behavior, original view-owner callbacks, no-step/currentness profiles, geometry continuation, patch ordering and full floating-point behavior retain prior qualifications. The dim3 independent query intentionally reads cached boxes and cannot discover/recompute a hidden view's unsupported builders; no new admission for those builders is claimed. The dim2 recompute uses existing helpers and propagates their explicit failures.

No GUI control session, original process, shared plan/state edit, installation/deployment or reconstructed DEF_PRE instance was involved. Only converter source/tests and owned reports/receipts changed. Root retains full integration and freeze responsibility.
