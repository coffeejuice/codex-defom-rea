# CP230–232 efficiency review

## Scope

This three-checkpoint review uses the retained full build/test/timing receipts and CP232 validation freeze. CP232's full suite passed with 369 registered tests: 362 clean, 6 logged-gap, 1 logged-difference, no incomplete output, skips or failures; historical folder comparison was 93 SAME / 5 DIFF. These are incremental builds with distinct source changes, not matched build benchmarks. Token telemetry is unavailable; no token or effort claim is made.

## Timings and repeat work

| Checkpoint | Full configure | Full build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP230 | 1.703 s | 111.469 s | 18.156 s | 35.328 s | 369 |
| CP231 | 1.719 s | 113.187 s | 18.578 s | 35.921 s | 369 |
| CP232 | 1.672 s | 112.562 s | 17.890 s | 35.016 s | 369 |

CP230–232 full builds sit in a narrow 111.469–113.187 s band, versus CP227–229's 110.219–115.969 s. Suite times likewise overlap: CTest is 17.890–18.578 s and launcher is 35.016–35.921 s in this interval. No end-to-end speed change is demonstrated. CP232 fixture SHA snapshots total 11.875 s and execution bindings 4.001 s, comparable to CP230 (11.751/4.187 s) and CP231 (11.938/4.156 s); these required integrity costs remain intact.

There was repeat build work within each incremental checkpoint. Focused receipts show CP230 build 18.984 s; CP231 first focused build 18.906 s, then second focused build 1.875 s; CP232 implementation build 19.344 s followed by parked-guard build 3.563 s. CP232 full build then took 112.562 s. The receipts establish these elapsed values, but do not say the earlier focused builds were waste: they served focused validation/guard steps. CP232 total recorded configure/build command durations across its two focused receipts and full build sum to 140.781 s (configure included: 1.859+19.344+1.781+3.563+1.672+112.562). The two focused receipts alone sum to 26.547 s; these are separate command durations, not a measured wall-clock interval because phases may be nested or intervening work may occur. Do not infer duplicated compilation from elapsed time alone.

The CP232 freeze has `objective_complete: false` despite successful suite and historical counts; efficiency review does not change that status or imply objective completion.

## Adjustment

No workflow change is supported by these three observations. Prior 4/8/4 CTest parallelism benchmark evidence remains a validated suite-stage optimization on the same 151-test workload, but CP230–232 all use `-j 4`; they provide no new comparison. It is not a build-speed result. Build costs remain unoptimized and unproven: the similar totals reflect differing changed-source/fan-out sets, with no matched-edit benchmark. Keep the current gates and measure one controlled future edit instead of introducing an unvalidated cache or reducing evidence.

## One safe next experiment

For the next naturally occurring broad incremental build, record Ninja `-d explain` plus scheduled compile/link edge counts for the exact source/header edit, then compare against an equivalent isolated edit on the same candidate tree/compiler and cache state. Retain the full source/binary binding, uncached fixture hashes, full suite and freeze checks. Attribute a speed change only to the matched comparison; otherwise report the command timings as observations.

## Evidence

CP230: `analysis/cp230-furnace-focus-build.json`, `cp230-furnace-full-build.json`, `cp230-furnace-full-tests.json`, and its `timing.json`.
CP231: `analysis/cp231-furnace-prologue-focus-build.json`, `cp231-furnace-prologue-focus2-build.json`, `cp231-furnace-prologue-full-build.json`, `cp231-furnace-prologue-full-tests.json`, and its `timing.json`.
CP232: `analysis/cp232-frequency-text-implementation-build.json`, `cp232-frequency-text-parked-guard-build.json`, `cp232-frequency-text-full-build.json`, `cp232-frequency-text-full-tests.json`, its `timing.json`, and `analysis/wave3-checkpoint232-validation.json`.
Prior comparison and policy: `analysis/resume229-efficiency-review.md`, `analysis/efficiency-policy.md`.

