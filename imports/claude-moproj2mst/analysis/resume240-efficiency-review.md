# CP237, CP239, and CP240 efficiency review

## Scope and limits

CP238 is superseded and excluded. This review uses the full build, CTest, phase-timing, source-fingerprint, rerun and validation receipts. Token measurements are explicitly unavailable in the suite receipts, so no token claim is made. Incremental build times span different edits and build-tree states; they do not establish a clean-build or source-specific speedup.

## Measured full runs

| Checkpoint/run | Configure | Build command | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP237 | 2.016 s | 112.562 s | 18.360 s | 38.938 s | 369 |
| CP239 | 1.718 s | 27.750 s | 18.703 s | 35.750 s | 369 |
| CP240 initial build | 1.734 s | 111.985 s | — | — | — |
| CP240 initial suite attempt | — | — | 18.078 s, exit 8 | 35.265 s | 369 |
| CP240 repaired full rerun | 1.703 s | 0.172 s | 18.109 s | 37.672 s | 369 |

The initial CP240 suite attempt failed with CTest exit 8 after 18.078 seconds. The follow-up was a test-only timing repair, with a separate small repair build receipt (2.015 s configure, 2.203 s build). The subsequent repaired full-build receipt took 1.703 s to configure and 0.172 s to build, then the full suite passed. This records the failed attempt and rerun cost; it does not imply the 0.172 s build reconstructed the full application from scratch. The initial substantive CP240 build took 111.985 s.

## Integrity and validation

The successful CP240 full suite receipt binds source before and after to the same SHA-256 (`f8e4f887867d93e326bfeea194f4c795a9696dc565a953ecbbcf46b6ecc88f2f`, 3,563 files), and records stable binary bindings during the suite. The preceding full build receipt fingerprints source as `b62049c82d0e3e5e1a2eb890ec68736a7be477f153ee750bf83aafe35fd2a3ec` (3,563 files). These hashes differ across the timing-repair interval; the successful suite's own before/after binding is stable, and the repair is documented as test-only. Do not describe the two different full-tree fingerprints as identical.

The successful CP240 freeze has 369 registered tests: 362 passed cleanly, 6 with logged gaps, 1 with logged differences, and zero incomplete outputs, skips, or failures. Historical comparison is 93 SAME / 5 DIFF; objective completion remains false.

Measured fixture SHA phases were 14.281 s for CP237, 11.735 s for CP239, 13.359 s for the failed CP240 attempt, and 13.359 s for the successful CP240 rerun. Before/after execution-binding phases totaled 4.968, 4.078, 4.907, and 4.907 seconds. These required checks explain material portions of launcher time and remain part of validation. CP237's launcher is 3.188 s longer than CP239's; lower fixture and binding phases account for 3.436 s of that difference, while CTest is 0.343 s faster in CP237. These single-run differences do not demonstrate a stable suite optimization.

Build commands vary sharply: CP239's 27.750 s build is much shorter than CP237's 112.562 s and CP240's initial 111.985 s, but the receipt set does not provide a matched same-edit comparison. Treat that as observed incremental work, not a claimed tenfold optimization. The previous CP232 arithmetic is corrected: all six configure/build command durations total 140.781 s; the two focused receipts alone total 26.547 s.

## Recommendation

No workflow change is justified. Preserve the test-only timing repair and successful full-suite evidence. Keep the complete source/binary binding and fixture hash phases. Before claiming any build gain, compare the same controlled edit on the same configured build tree and compiler, retaining the same validation gates. The existing benchmark for CTest parallelism is suite-stage evidence on a different unchanged 151-test workload and does not explain these build durations.

## Evidence

CP237: `analysis/cp237-forming-express-flags-full-build.json`, `cp237-forming-express-flags-full-tests.json`, its `timing.json`.
CP239: `analysis/cp239-operation-cache-fences-full-build.json`, `cp239-operation-cache-fences-full-tests.json`, its `timing.json`.
CP240 initial: `analysis/cp240-editor-step-count-full-build.json`, failed run `build/test-runs/1793037803632603400/timing.json` and `receipt.json`.
CP240 repair/rerun: `analysis/cp240-remesh-v2-timing-repair-build.json`, `cp240-editor-step-count-repaired-full-build.json`, `cp240-editor-step-count-repaired-full-tests.json`, its `timing.json`, and `analysis/wave3-checkpoint240-validation.json`.
Prior arithmetic correction: `analysis/resume232-efficiency-review.md`.
