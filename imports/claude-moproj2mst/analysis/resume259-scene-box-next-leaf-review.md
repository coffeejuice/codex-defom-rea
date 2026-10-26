# CP259 scene-box successor review — PASS

The revised fixed-slot scene-box union design is implementation-ready. Its corrected caller-test contract distinguishes native union/window expectations from the current converter's retained radius substitution and stored-origin result. The earlier radius/origin qualification hold is resolved; no further design correction is required within the bounded secondary/third scope.

Only this review was written. No source/tests/build/shared state, original process, GUI control or mutable REA was changed or run.

## Supported contract

Current `source/apps/moproj2mst/src/engine/viewer_camera.cpp::sceneBox` handles only shown main; `fit` already contains CP257's main/secondary/third refresh prefix. Retained full `evidence/gui_mo/s1/viewer_rect/a1.c::FUN_140cfd750` visits main, secondary, third, then the named-holder snapshot. Every fixed slot checks its live guard before its shown flag. In the refresh=false specialization used by fit, it queries `c8e0f0` and unions the result into the accumulator without another refresh. The report's secondary and third query/union return addresses agree with this retained function.

Adding secondary immediately after main and third immediately after secondary is supported. Keeping independent slot visits, no alias deduplication, no dimension/step admission here, no selected-model substitution and no all-slot geometry preflight preserves the bounded native sequence. Either secondary alone or both secondary/third can be assigned, but the implementation receipt must state which was actually changed and tested. Named-holder traversal remains explicitly excluded.

Retained `V/v0.c::FUN_14121a9f0` supports invalid-operand identity and valid componentwise union through the existing min/max and constructor chain. `V/v2.c::FUN_141219af0`, together with current `boxOf`/`boxUnion`/`derive`, supports reusing the existing helper rather than introducing a new radius formula. This review does not newly establish exceptional floating-point parity or independently disassemble those arithmetic helpers. The proposal correctly leaves that claim closed.

Ownership is bounded: production changes stay inside `Display`'s implementation, canonical secondary stays owned by its existing `secondOwned`, and the borrowed third fixture must outlive the display. No public API, retained raw borrow, new owner, or live ObjectKey is needed. A later `viewerSceneBox` refusal does not roll back CP257's already-completed cache refreshes. Existing pending-refresh/geometry admission behavior must remain unchanged; no native exception/disposition parity follows from a reconstructed error.

## Radius/origin qualification resolved

The revised report explicitly states that the window setter is separate from the stored-box radius gate. The literal L/R/T/B expectations follow the retained union/window path without assuming that independent `cfe0f0` equals union radius. It now labels origin x=-6 solely as a current-converter regression expectation and rejects a native stored-box acceptance/origin inference from large union extents.

The translated union `[-2,14]` has centre x=6 and existing helper radius 8, giving current window L/R `-2.4/14.4`, T/B `8.4/-8.4`. Those are appropriate explicit arithmetic expectations for the actual caller. Current converter also stores this box and yields origin x=-6, but native fit obtains `cfe0f0` first and passes that independent value to `d69e70`. The revised qualification correctly keeps that native store/origin result open. No fresh GUI witness is needed merely to implement/test the bounded union.

The report's final section consistently leaves `cfe0f0`, `cfdc60`, and `c8f830` unresolved. Their results are not recovered from helper names, large input extents, or the substituted current radius. Native two scene-box queries versus the converter's single query remains separately qualified, as documented.

## Test and evidence assessment

The proposed `displayRefreshScene(project, &main)` → `displayCamera(project)` tests are meaningfully downstream of the changed private function. Symmetric secondary extents distinguish hidden versus shown contribution; translated extents reject a max-of-model-radii shortcut. Check `CameraState::lrtb` for these explicit L/R/T/B values, rather than confusing them with aspect-scaled projection coordinates. Use the established floating tolerance and literal expectations rather than invoking production box helpers as the test oracle.

Hidden, invalid-live and null/expired shown-slot cases exercise the actual guards and invalid-box identity. If third is included, it needs its own distinct camera contribution with secondary hidden/absent. Keep the named collection empty. Build fixture lifetimes like the current CP257 test: main/third constructed before `Project` so the display tears down first; keep secondary with its display owner. Prefer actual owner destruction for an expired guard test. CP257's manual null-guard/live-owner state is an adapter fixture state, not proof of canonical ownership or native replacement behavior.

The bounded leaf should retain the configured `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box` path, Testkit entry point and supported assertions, with root-owned binding validation and source-stable grouped/full integration. This review ran no tests or builds.

Original qualification remains **DEF_GUI_MO_64.exe v11.0.2**, retained immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, previously established SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This review read retained decompiles and current source/tests; it did not independently rehash/disassemble the PE or execute the original. Static branch/helper evidence supports this leaf, not a naturally reached secondary/third fresh-project occurrence, complete multi-model camera parity, original failure disposition, named no-step behavior, or CP225 storage-currentness closure.

PASS applies to this design and proposed regression contract. It does not establish an implemented/tested/frozen candidate or expand production scope to radius aggregation, named models or complete camera parity.
