# CP250–252 efficiency review

## Scope and limits

This review uses the focused/full build receipts, full-suite timing receipts and checkpoint freezes. Token telemetry is unavailable. Incremental builds represent different edits and states, so durations are observations rather than controlled speed comparisons.

## Measured checkpoints

| Checkpoint/run | Configure | Build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP250 focused attempt | 3.813 s | 2.031 s, exit 2 | — | — | — |
| CP250 focused v2 | 2.390 s | 2.047 s, exit 0 | — | — | — |
| CP250 full | 1.735 s | 28.343 s | 18.234 s | 38.172 s | 369 |
| CP251 focused | 1.719 s | 21.234 s | — | — | — |
| CP251 full build | 1.625 s | 111.000 s | — | — | — |
| CP251 repair build | 1.703 s | 1.984 s | — | — | — |
| CP251 repaired full run | 1.719 s | 0.172 s | 17.813 s | 34.953 s | 369 |
| CP252 focused | 1.781 s | 1.750 s | — | — | — |
| CP252 full | 1.688 s | 28.093 s | 18.625 s | 35.703 s | 369 |

All three checkpoint freezes record 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, zero incomplete outputs, skips or failures, and `objective_complete: false`.

CP250's initial focused build exited 2 after 2.031 s; the focused v2 build succeeded in 2.047 s. CP251 has a successful repair build (1.984 s) and repaired full build/test receipts. The freeze refers to the repaired outputs. No separate failed CP251 full-test timing/receipt is present among the listed retained artifacts, so a failed-suite duration or cause cannot be quantified from this evidence. CP251's initial and repaired full build receipts carry different source-tree hashes, while the repaired full-suite execution binding is stable during its run. Do not merge those snapshots or characterize the 0.172-second build as a complete rebuild.

## Test and integrity overhead

Suite runtimes are CTest 17.813–18.625 s and launcher 34.953–38.172 s. CP250's launcher outlier includes 13.531 s in fixture SHA phases and 5.063 s in execution bindings; CP251 has 11.813 s and 4.031 s; CP252 has 11.812 s and 3.969 s. These phases explain part of CP250's launcher increase and remain required evidence. CTest itself varies by only 0.812 s across the three. The source fingerprints in the full build receipts cover 3,563 files and take 1.421, 1.407, 1.422, and 1.437 seconds for CP250, CP251 initial/repaired, and CP252 respectively; this is a separate measurement from test-launcher source/binary binding and fixture hashing.

Compared with CP247–249, full CTest remains similar (17.390–18.187 s there versus 17.813–18.625 s here). Launcher time overlaps except for CP250's higher 38.172 s; measured hash and binding phases account for about 3.0 s more than CP247 and 2.8 s more than CP249. Full build durations here are mixed: CP250/252 about 28 s, CP251 111 s. Prior CP247–249 durations were 39.797, 115.922, and 112.704 s. Different changed-source and build work means these observations do not demonstrate general build acceleration or regression.

## Recommendation

No general speed or token claim is supported, and no workflow gate should change. Preserve full source/binary binding, uncached fixture hashing, the complete suite and freeze criteria. If a future optimization is considered, compare the same controlled edit on the same configured tree and compiler, record scheduled compile/link edges, and retain all validation phases.

## Evidence

CP250: `analysis/cp250-scene-dimension-focused-build.json`, `cp250-scene-dimension-focused-v2-build.json`, `cp250-scene-dimension-full-build.json`, `cp250-scene-dimension-full-tests.json`, `wave3-checkpoint250-validation.json`, and its `timing.json`.
CP251: `analysis/cp251-multiblow-control-focused-build.json`, `cp251-multiblow-control-full-build.json`, `cp251-multiblow-control-repair-build.json`, `cp251-multiblow-control-repaired-full-build.json`, `cp251-multiblow-control-repaired-full-tests.json`, `wave3-checkpoint251-validation.json`, and its `timing.json`.
CP252: `analysis/cp252-second-default-focused-build.json`, `cp252-second-default-full-build.json`, `cp252-second-default-full-tests.json`, `wave3-checkpoint252-validation.json`, and its `timing.json`.
Prior comparison: `analysis/resume249-efficiency-review.md`.
