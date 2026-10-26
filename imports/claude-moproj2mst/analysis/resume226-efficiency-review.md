# CP224–226 efficiency review

## Scope and measurement limits

This three-checkpoint review uses the retained configure/build/focused/full-suite receipts and launcher timing phase files only. Builds are incremental and each checkpoint has different source edits and scheduled work; their build times are observations, not a matched speed benchmark. Token and active effort telemetry are unavailable, so no token or labor-efficiency claim is made.

## Measured timings

| Checkpoint | Configure | Focused stage | Full build | Full CTest | Full launcher | Tests |
| --- | ---: | ---: | ---: | ---: | ---: |---:|
| CP224 | 2.406 s | 3.078 s preflight; focused CTest 1.172 / 0.734 s | 150.437 s; 696 logged Ninja edges | 17.765 s | 34.797 s | 367 |
| CP225 | 2.406 s | 21.187 s preflight; focused CTest 1.093 / 0.765 s | 134.609 s; 662 logged edges | 17.875 s | 34.828 s | 367 |
| CP226 | 1.719 s focused build configure; focused build 22.235 s / 85 edges; full configure 1.750 s | — | 109.157 s; 543 logged edges | 18.063 s | 35.234 s | 368 |

Each full suite returned exit code zero. CP224–226 full launcher timings remain close (34.797–35.234 s). CTest rose from 17.765 to 18.063 seconds as one test was added. Required integrity work is stable: fixture SHA-256 phases total 11.765, 11.766, and 11.921 seconds; source/binary bindings total 4.032, 4.000, and 4.000 seconds. These are accounted validation costs, not optimization targets to remove.

Compared with CP219–222, full-suite duration remains in the prior range (34.859–35.906 s) and CTest remains around 17.4–18.4 s. The build remains expensive but varies with fan-out: CP224 150.437 s/696 edges, CP225 134.609 s/662, CP226 109.157 s/543. The lower sequence is not evidence of a build optimization because edits and scheduled edges differ. CP225's fixed focused preflight took 21.187 s, while CP224 took 3.078 s; both led to multiple focused tests and a successful full suite. CP226's distinct focused build took 22.235 s. These focused stages should not be conflated with CTest suite time.

## Validated test-time optimization versus build work

The existing CTest resource policy benchmark validates 4/8/4 parallel jobs at 35.95/22.67/35.57 seconds for the unchanged 151-test workload, with all tests passing. This is a measured suite-stage result, not an end-to-end recovery or build speedup. CP224–226 used `-j 4`; no new test-parallelism improvement is established here. Build optimization is likewise unmeasured: the recorded edge counts and build durations only quantify current incremental workload.

## One safe next experiment

Run a matched incremental fan-out experiment in a later authorized candidate phase: hold the candidate tree, compiler, source snapshot, and build command constant; collect Ninja `-d explain` output and compile/link edge counts for each controlled, one-header edit. Test `engine.hpp` and `model.hpp` separately, then evaluate one narrow compatibility-preserving internal header split for whichever header is shown to fan out broadly. Compare before/after wall time and edge counts. Keep source/executable bindings, uncached fixture hashes, full suite and historical diagnostics intact. Until this controlled comparison exists, make no causal or speedup claim for either header.

## Evidence

CP224: `analysis/cp224-two-owner-fixed-full-build.json`, `cp224-two-owner-fixed-full-tests.json`, `build/logs/cp224-two-owner-fixed-full-build-05.log`, and its `timing.json`.
CP225: `analysis/cp225-two-owner-full-build.json`, `cp225-two-owner-full-tests.json`, and its `timing.json`.
CP226: `analysis/cp226-dimension-focused-build.json`, `cp226-dimension-full-build.json`, `cp226-dimension-full-tests.json`, and its `timing.json`.
Prior comparison: `analysis/resume223-efficiency-review.md`; suite-job benchmark: `analysis/efficiency-policy.md`.
