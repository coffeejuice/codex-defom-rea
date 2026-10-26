# CP253–255 efficiency review

## Scope and limits

This review uses the existing focused/full build receipts, full-suite timing receipts, and checkpoint freezes. All suites passed. The incremental builds involve different edits and states, so their durations cannot establish a generalized speedup. Token telemetry is unavailable; no token or effort claim is made.

## Measured timings

| Checkpoint/run | Configure | Build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP253 focused | 1.750 s | 2.453 s | — | — | — |
| CP253 focused r1 | 1.703 s | 2.437 s | — | — | — |
| CP253 full | 1.688 s | 28.187 s | 17.593 s | 34.594 s | 369 |
| CP254 focused | 1.797 s | 4.094 s | — | — | — |
| CP254 focused final | 1.703 s | 1.734 s | — | — | — |
| CP254 full | 1.703 s | 28.141 s | 18.203 s | 35.953 s | 369 |
| CP255 focused | 1.672 s | 21.687 s | — | — | — |
| CP255 focused LF | 1.687 s | 2.532 s | — | — | — |
| CP255 full | 1.797 s | 111.485 s | 18.718 s | 36.750 s | 369 |

Each freeze reports 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, and zero incomplete outputs, skips, or failures. All retain `objective_complete: false`.

## Comparison with CP250–252

CP250 and CP252 full builds were 28.343 and 28.093 s, close to CP253/254 at 28.187/28.141 s. CP251 was 111.000 s, close to CP255's 111.485 s. This alternation is consistent with differing incremental work; it is not evidence that one checkpoint's implementation or method is faster. Focused build durations also vary widely, from roughly 1.7–4.1 s in CP253/254 final attempts to 21.687 s in CP255 before its 2.532 s LF follow-up. Receipts do not establish that the longer focused build is redundant or identify a stable cause for the difference.

Full CTest is 17.593–18.718 s, overlapping the CP250–252 range of 17.813–18.625 s. Launcher time is 34.594–36.750 s. Fixture SHA phases total 11.703 s (CP253), 12.328 s (CP254), and 12.516 s (CP255); source/binary execution-binding phases total 4.079, 4.094, and 4.251 s. These remain material required validation costs. CP255's longer launcher includes CTest 0.515 s above CP254, fixture hashing 0.188 s higher, and binding 0.157 s higher; these are single-run differences, not a demonstrated regression.

## Next efficiency action

No evidence justifies changing the workflow or reducing validation. On the next naturally occurring broad incremental build, retain Ninja explanation and compile/link edge counts for the exact source edit, then compare an equivalent edit on the same configured tree, compiler and cache state. This will help distinguish source fan-out from build-state effects before proposing a header split or other change. Keep all source/binary bindings, uncached fixture hashes, full suite, and freeze gates intact.

## Evidence

CP253: `analysis/cp253-editor-dimension-preflight-focused-build.json`, `cp253-editor-dimension-preflight-focused-r1-build.json`, `cp253-editor-dimension-preflight-full-build.json`, `cp253-editor-dimension-preflight-full-tests.json`, `wave3-checkpoint253-validation.json`, and its `timing.json`.
CP254: `analysis/cp254-second-publish-focused-build.json`, `cp254-second-publish-focused-final-build.json`, `cp254-second-publish-full-build.json`, `cp254-second-publish-full-tests.json`, `wave3-checkpoint254-validation.json`, and its `timing.json`.
CP255: `analysis/cp255-spring-labels-focused-build.json`, `cp255-spring-labels-focused-lf-build.json`, `cp255-spring-labels-full-build.json`, `cp255-spring-labels-full-tests.json`, `wave3-checkpoint255-validation.json`, and its `timing.json`.
Comparison: `analysis/resume252-efficiency-review.md`.
