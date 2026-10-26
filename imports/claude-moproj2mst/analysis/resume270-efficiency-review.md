# CP268–270 efficiency review

## Scope and receipts

CP268, CP269, and CP270 all have successful full-suite freezes. Their build times are incremental observations for different source edits. Token telemetry is unavailable; no token or effort claim is made.

| Checkpoint/run | Configure | Focused build(s) | Full build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|---:|
| CP268 | 1.922 s | 1.875 s | 28.282 s | 17.781 s | 35.407 s | 369 |
| CP269 first full build | 1.766 s | 1.812 / 1.813 s focused receipts | 30.953 s | — | — | — |
| CP269 final build and suite | 1.750 s | — | 0.156 s | 17.875 s | 35.000 s | 369 |
| CP270 | 1.703 s | 2.656 s | 29.704 s | 19.172 s | 37.266 s | 369 |

Each freeze records 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, no incomplete output, skips or failures, and `objective_complete: false`.

## CP269 HOLD and rebuild

CP269's initial 2D/absent-step focused test was held because infinite enabled scaling would fail independently of the dimension condition. The test input was corrected to finite scaling; the corrected focused target built on a stable source tree and the exact CTest passed. The already-completed full build took 30.953 s. The subsequent final source-stable build took 0.156 s and the bound full suite passed. This is the observed test-correction/rebuild sequence; the 0.156-second build is not a clean rebuild. The held test is not a failed full-suite run.

## Suite and integrity timing

CTest ranges from 17.781 to 19.172 s. Launcher time is 35.000–37.266 s. Fixture SHA phases total 12.250 s (CP268), 11.733 s (CP269 final), and 12.562 s (CP270); source/binary bindings total 4.141, 4.079, and 4.297 s. CP270's launcher is 2.266 s longer than CP269's; CTest is 1.297 s longer, and fixture/binding phases together are about 1.047 s longer. These single runs do not establish a stable regression, but identify where the measured launcher difference occurred. Integrity phases remain required.

Compared with the prior CP260/261, CP263, CP262 review, full CTest times were 18.515, 18.188, and 18.110 s, and full launcher times 35.765, 35.375, and 35.172 s. CP268/269 remain close to that range; CP270 is a single higher run. Full build times here are around 28–31 s, compared with 26.328–39.328 s in those prior units. Different edit and build scopes prevent a causal or general speed conclusion.

## Next measurement action

Keep the current validation gates. For a future suite-time investigation, compare repeated launcher phase timings under the same test selection and machine load before changing anything; preserve fixture hashes and bindings. For build performance, measure Ninja's explanation and compile/link edge counts for one controlled edit on the same configured tree and compiler. No speed or token savings are established by these receipts.

## Evidence

CP268: `analysis/cp268-transition-owner-build.json`, `cp268-alias-repair-full-build.json`, `cp268-alias-repair-full-tests.json`, and `wave3-checkpoint268-validation.json`.
CP269: `analysis/cp269-fill-meshing-build.json`, `cp269-fill-meshing-finite-scale-build.json`, `cp269-fill-meshing-full-build.json`, `cp269-fill-meshing-full-final-build.json`, `cp269-fill-meshing-full-final-tests.json`, `resume269-fill-meshing-integration.md`, and `wave3-checkpoint269-validation.json`.
CP270: `analysis/cp270-stopping-owner-build.json`, `cp270-stopping-owner-full-build.json`, `cp270-stopping-owner-full-tests.json`, and `wave3-checkpoint270-validation.json`.
Prior comparison: `analysis/resume262-efficiency-review.md`.
