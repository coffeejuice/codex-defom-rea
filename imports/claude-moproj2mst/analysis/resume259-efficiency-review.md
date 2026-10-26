# CP256–259 efficiency review

## Scope and receipts

The reviewed units are CP256, combined CP257/258, and CP259. The combined receipt is one build and one full suite for the grouped CP257–258 changes; it is not two independent full-run timing samples. Token telemetry is unavailable, so no token or effort claim is made. All build measurements are incremental and should be read as task observations.

## Timings

| Checkpoint/run | Configure | Focused build(s) | Full build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|---:|
| CP256 | 1.672 s | 21.656, 2.547, 2.437 s | 110.531 s | 18.188 s | 35.219 s | 369 |
| CP257–258 combined | 1.782 s | CP257: 14.391, 4.187, 4.156 s; CP258: 2.719 s | 109.843 s | 18.031 s | 35.360 s | 369 |
| CP259 | 1.813 s | 4.281 s | 28.594 s | 18.000 s | 35.250 s | 369 |

All three freezes report 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, and zero incomplete outputs, skips, or failures. Each reports objective completion as false.

## Comparison with CP253–255

CP253 and CP254 full builds were 28.187 and 28.141 s, while CP255 was 111.485 s. CP259's 28.594 s is close to the two short builds; CP256 and combined CP257–258 are 110.531 and 109.843 s, close to CP255. This repeating short/long pattern is consistent with differing incremental build work; it does not establish a speed change. Focused receipts show several follow-up builds after earlier focused builds, but no matched edit establishes that the earlier work could have been skipped.

CTest (18.000–18.188 s) and full launcher (35.219–35.360 s) are tightly grouped and overlap CP253–255 (CTest 17.593–18.718 s; launcher 34.594–36.750 s). Fixture SHA phases total 11.688 s for CP256, 11.954 s for CP257–258, and 11.937 s for CP259. Source/binary execution binding phases total 4.124, 4.094, and 4.078 s. The full validation remains required; the receipts show no meaningful suite-time change.

## Efficiency action

The combined CP257–258 receipt demonstrates that two related checkpoint changes were validated under one full build and suite, but it does not quantify savings against separate runs. Continue grouping compatible work only when ownership/contracts permit it, and record a matched cost comparison before claiming a benefit. For any proposed build optimization, capture Ninja explanation and compile/link edge counts for the same edit on the same build tree/compiler/cache state. Do not reduce full-suite, source/binary binding, fixture hashing, or freeze requirements.

## Evidence

CP256: `analysis/cp256-multiblow-save-focused-build.json`, `cp256-multiblow-save-focused-final-build.json`, `cp256-multiblow-save-focused-verified-build.json`, `cp256-multiblow-save-full-build.json`, `cp256-multiblow-save-full-tests.json`, `wave3-checkpoint256-validation.json`, and its `timing.json`.
CP257–258: `analysis/cp257-scene-refresh-focused-build.json`, `cp257-scene-refresh-final-build.json`, `cp257-scene-refresh-reviewfix-build.json`, `cp258-advanced-remap-focused-build.json`, `cp257-258-combined-full-build.json`, `cp257-258-combined-full-tests.json`, `wave3-checkpoint257-258-validation.json`, and its `timing.json`.
CP259: `analysis/cp259-scene-box-build.json`, `cp259-scene-box-full-build.json`, `cp259-scene-box-full-tests.json`, `wave3-checkpoint259-validation.json`, and its `timing.json`.
Comparison: `analysis/resume255-efficiency-review.md`.
