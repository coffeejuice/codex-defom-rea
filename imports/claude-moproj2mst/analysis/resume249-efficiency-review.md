# CP247–249 efficiency review

## Scope and limits

CP247, CP248 and CP249 all have successful full-suite freezes. Token telemetry is unavailable in the test receipts. All builds are incremental and source edits differ; durations are observed command times, not controlled speed comparisons.

## Full and focused timings

| Checkpoint/run | Configure | Build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP247 focused | 1.781 s | 8.328 s | — | — | — |
| CP247 full | 1.718 s | 39.797 s | 17.907 s | 35.203 s | 369 |
| CP248 focused | 1.704 s | 21.828 s | — | — | — |
| CP248 initial full build | 1.796 s | 115.922 s | — | — | — |
| CP248 failed suite attempt | — | — | 17.844 s, exit 8 | 35.344 s | 369 |
| CP248 Boolean scratch repair | 1.703 s | 2.406 s | — | — | — |
| CP248 repaired full run | 1.750 s | 0.203 s | 17.390 s | 34.891 s | 369 |
| CP249 focused | 1.750 s | 21.031 s | — | — | — |
| CP249 focused rerun | 1.765 s | 2.266 s | — | — | — |
| CP249 full | 1.718 s | 112.704 s | 18.187 s | 35.125 s | 369 |

All three freezes report 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, and zero incomplete output, skips or failures. Objective completion remains false. CP248's initial suite run exited 8 because of the Boolean scratch-name collision in test setup; the scratch repair and repaired full-suite run then passed. The observed cost includes one failed 35.344-second launcher and the 2.406-second repair build command, followed by a passing 34.891-second launcher. This quantifies recorded work, not a hypothetical saved duration. The repaired suite receipt binds source before and after to the same SHA-256; the initial and repaired build receipts have different full-tree hashes, so they should not be represented as the same source snapshot.

CP249's focused rerun followed a 21.031-second focused build with a 2.266-second build; both succeeded. No receipt evidence establishes that this was wasted work. CP247's 39.797-second full build is much shorter than CP248/249's 115.922/112.704 seconds, while the focused steps also differ. Since the source change and incremental edge sets differ, this is not a repeatable build-speed improvement claim.

## Test and integrity overhead

The full CTest times range from 17.390 to 18.187 seconds; launcher times range from 34.891 to 35.344 seconds. Fixture SHA phases total 11.906 s (CP247), 11.797 s (failed CP248), 11.797 s (repaired CP248), and 11.641 s (CP249). Source/binary execution binding phases total 4.125, 4.001, 4.438, and 4.047 seconds respectively. These measured costs account for a substantial and stable portion of launcher time and remain required evidence. The failed CP248 suite has no registered test failure breakdown in the compact receipt, so the exit code is the supported failure fact; the checkpoint freeze identifies the scratch-name collision as its cause.

Compared with CP244–246, the passing CTest range is similar: CP244 17.360 s, CP245 18.031 s, CP246 17.828 s; CP247–249 are 17.390–18.187 s. The previous full build durations were CP244 110.922 s, CP245 37.593 s, CP246 112.500 s; CP248/249 sit near CP244/246, and CP247 is shorter. Different changed sources and build work preclude interpreting this as a general speed change. No token savings can be reported.

## Recommendation

Keep the current full validation and binding gates. No workflow change or general build-speed claim is justified by this three-checkpoint sample. If another similar fixture collision occurs, retain its failed-run receipt and record collision isolation as focused test setup; do not reduce full regression coverage. A future build optimization needs a matched same-edit/build-tree measurement.

## Evidence

CP247: `analysis/cp247-forming-order-focused-build.json`, `cp247-forming-order-full-build.json`, `cp247-forming-order-full-tests.json`, `wave3-checkpoint247-validation.json`, and its `timing.json`.
CP248: `analysis/cp248-meshing-writeback-focused-build.json`, `cp248-meshing-writeback-full-build.json`, `cp248-meshing-writeback-repaired-full-build.json`, `cp248-meshing-writeback-repaired-full-tests.json`, `cp248-boolean-scratch-repair-build.json`, failed run `build/test-runs/1793042770117038300/timing.json` and `receipt.json`, and `wave3-checkpoint248-validation.json`.
CP249: `analysis/cp249-heating-unit-focused-build.json`, `cp249-heating-unit-focused-r2-build.json`, `cp249-heating-unit-full-build.json`, `cp249-heating-unit-full-tests.json`, `wave3-checkpoint249-validation.json`, and its `timing.json`.
Prior comparison: `analysis/resume246-efficiency-review.md`.
