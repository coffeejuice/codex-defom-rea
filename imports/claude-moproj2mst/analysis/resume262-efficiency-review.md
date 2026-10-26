# CP260/261, CP263, and CP262 efficiency review

## Scope and limits

The combined CP260/261 receipt represents one full build and suite for combined changes. CP263 and CP262 each have their own full run. Token telemetry is unavailable; no token or effort claim is made. Build times are incremental observations for distinct edits, not matched speed measurements.

## Measured timings

| Validation unit | Configure | Focused build | Full build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|---:|
| CP260/261 combined | 1.953 s | CP261 receipt build 0.125 s (focused logs also retained) | 26.328 s | 18.515 s | 35.765 s | 369 |
| CP263 meshing-selection | 1.734 s | 1.625 s | 27.953 s | 18.188 s | 35.375 s | 369 |
| CP262 scene-radius | 1.719 s | 7.938 s failed, then 4.421 s and 4.359 s successful | 39.328 s | 18.110 s | 35.172 s | 369 |

Each freeze reports 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, no incomplete output, skips or failures, and `objective_complete: false`.

## Retry accounting and stale build failure

CP262 focused build receipts show one failure (exit 2 after 7.938 s) followed by two successful focused builds (4.421 and 4.359 s). The integration note records the earlier Windows macro collision and cached large model-box test issue and describes the corrected focused target. These durations are recorded command costs; no counterfactual saving is inferred.

CP263's worker direct `cmake --build` lacked the MSVC developer environment and failed before compiling project code. Its first CTest ran the preceding binary and is explicitly not evidence for the edit. The qualification notes that root subsequently performed a focused build and exact test successfully. The retained formal focused build receipt is 1.625 s; no elapsed time is available for the stale direct-build failure, so it cannot be added to a numerical total.

The combined CP260/261 sample has one full build and one full suite, not independent per-checkpoint costs. The short full builds (26.328 s combined and 27.953 s CP263) are similar to CP253/254/259's ~28 s incremental builds. CP262's 39.328 s full build is shorter than the ~110 s builds at CP256/CP257–258 but longer than those short samples. Different changed sources, build state and edge sets mean this does not establish a speedup.

## Integrity phases and suite comparison

Fixture SHA phases total 11.922 s (CP260/261), 11.922 s (CP263), and 11.749 s (CP262). Source/binary execution bindings total 4.063, 4.046, and 4.047 s. These checks are material portions of the 35.172–35.765 s launcher totals and remain required. CTest is tightly grouped at 18.110–18.515 s, within CP256–259's 18.000–18.188 s range. Launcher time overlaps CP256–259's 35.219–35.360 s apart from CP260/261's higher 35.765 s; this single combined run does not show a sustained change.

## Next action

Keep validation unchanged. On the next naturally occurring broad build, retain Ninja `-d explain` and compile/link edge counts for the exact changed source, then compare an equivalent edit under the same compiler and configured build tree. This will separate edit fan-out from build-state effects before proposing an optimization. No token savings or general build improvement is measurable from the current receipts.

## Evidence

CP260/261: `analysis/cp260-261-combined-full-build.json`, `cp260-261-combined-full-tests.json`, `wave3-checkpoint260-261-validation.json`, and its `timing.json`; CP261 supporting receipts `cp261-named-box-build.json` and `cp261-named-box-receipt-build.json`.
CP263: `analysis/cp263-meshing-selection-focused-build.json`, `cp263-meshing-selection-full-build.json`, `cp263-meshing-selection-full-tests.json`, `wave3-checkpoint263-validation.json`, and `resume263-meshing-selection-integration.md`.
CP262: `analysis/resume262-scene-radius-focused1-build.json`, `resume262-scene-radius-focused2-build.json`, `resume262-scene-radius-focused3-build.json`, `cp262-scene-radius-full-build.json`, `cp262-scene-radius-full-tests.json`, `wave3-checkpoint262-validation.json`, and `resume262-scene-radius-integration.md`.
Comparison: `analysis/resume259-efficiency-review.md`.
