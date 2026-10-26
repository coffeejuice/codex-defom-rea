# CP244–246 efficiency review

## Scope and measured full runs

Receipts cover three completed checkpoints and their retained build/test/freeze results. Token telemetry is unavailable; no token or effort claim is made.

| Checkpoint | Full configure | Full build | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|
| CP244 | 1.750 s | 110.922 s | 17.360 s | 34.250 s | 369 |
| CP245 | 1.704 s | 37.593 s | 18.031 s | 35.656 s | 369 |
| CP246 initial build | 1.734 s | 112.500 s | no retained full-suite receipt | — | — |
| CP246 repaired full run | 1.719 s | 0.156 s | 17.828 s | 35.000 s | 369 |

All three freezes record 362 clean passes, 6 logged-gap passes, 1 logged-difference pass, and no incomplete outputs, skips, or failures. Each freeze retains `objective_complete: false`.

CP245's 37.593 s full build is lower than CP244/CP246, but these are different incremental source edits and build states; it is not a controlled speedup. CP246's substantive full build took 112.500 s. Its repaired full-build receipt has only 0.156 s of build work, which is not a clean/full rebuild benchmark.

## Repeated and failed work

CP245 has a coordinator build (2.266 s), final build (3.875 s), a parked attempt failing with exit 2 after 1.547 s, and parked2 succeeding in 3.984 s, before the 37.593 s full build and passing suite. CP246 has an initial build attempt exiting 1 after 0.094 s, a focused build exiting 2 after 21.125 s, a focused2 build succeeding in 1.953 s, repair build exiting 2 after 3.625 s, and repair2/3/4 succeeding in 2.125/2.015/1.969 s. The checkpoint then records a 112.500 s full build and repaired full-build/full-test receipts. These attempts are observable rework, but the command durations do not establish a counterfactual saving from omitting them.

No separate retained CP246 failed full-suite receipt appears among the CP246 analysis artifacts; the repaired full test receipt and freeze are successful. Therefore this review does not assign a failure duration or cause to an initial full-suite attempt. The successful full-suite launcher is 35.000 s, including CTest 17.828 s, fixture SHA phases totaling 11.875 s and execution-binding phases totaling 4.078 s. Keep these integrity checks intact.

## Comparison with CP241–243 and earlier timing repair

Compared with CP241–243, CP244's 34.250 s launcher is modestly lower than their 35.203–35.562 s; CP245 and CP246 are 35.656 and 35.000 s. CTest timings (17.360–18.031 s) overlap the earlier 17.984–18.296 s range. One set of runs does not establish a stable suite-time change. The prior CP240 timing repair remains a separate documented case: its first test attempt exited 8, followed by a test-only timing repair and successful 18.109 s CTest rerun with stable before/after source binding. CP246's available passing receipt is the repaired run; without a retained failed suite receipt here, do not infer that CP240's repair mechanism or timing applies to CP246.

## Recommendation

No workflow or performance claim change is supported. Preserve full source/binary binding, uncached fixture hashing, the full suite and freeze criteria. Treat CP245's shorter build as task-specific incremental work. Any build optimization claim requires a matched same-edit comparison on a controlled build tree; test-suite timings are separate from build timings.

## Evidence

CP244: `analysis/cp244-heattreat-stepdef-build.json`, `cp244-heattreat-stepdef-full-build.json`, `cp244-heattreat-stepdef-full-tests.json`, `wave3-checkpoint244-validation.json`, and its `timing.json`.
CP245: `analysis/cp245-reaction-rate-coordinator-build.json`, `cp245-reaction-rate-final-build.json`, `cp245-reaction-rate-parked-build.json`, `cp245-reaction-rate-parked2-build.json`, `cp245-reaction-rate-coordinator-full-build.json`, `cp245-reaction-rate-coordinator-full-tests.json`, and `wave3-checkpoint245-validation.json`.
CP246: `analysis/cp246-heattreat-mode-build.json`, `cp246-heattreat-mode-focused-build.json`, `cp246-heattreat-mode-focused2-build.json`, `cp246-heattreat-mode-repair-build.json` through `repair4-build.json`, `cp246-heattreat-mode-full-build.json`, `cp246-heattreat-mode-repaired-full-build.json`, `cp246-heattreat-mode-repaired-full-tests.json`, and `wave3-checkpoint246-validation.json`.
Prior comparison: `analysis/resume243-efficiency-review.md`; CP240 repair history: `analysis/resume240-efficiency-review.md`.
