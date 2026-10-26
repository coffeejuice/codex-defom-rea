# CP216–218 efficiency review

## Scope and comparison limits

This review uses the CP215 report as the baseline and the retained CP216, CP217, and CP218 build and full-launcher receipts. It does not run builds/tests, inspect GUI state, or execute the original application. Builds are incremental and source changes differ, so their durations are task observations, not clean-build speed comparisons. Token and active-effort telemetry are unavailable; no token or labor-efficiency claim is made.

## Measured checkpoint costs

| Checkpoint | Build receipt (configure + build command) | CTest | Full launcher | Test result |
| --- | ---: | ---: | ---: | --- |
| CP215 baseline | 33.042 s (1.687 + 28.672) | 17.203 s | 34.562 s | 362 registered; 355 clean, 6 logged-gap, 1 logged-difference; 0 failed/skipped |
| CP216 | 4.695 s (1.719 + 0.156) | 17.609 s | 38.297 s | 363 registered; 356 clean, 6 logged-gap, 1 logged-difference; 0 failed/skipped |
| CP217 | 4.516 s (1.703 + 0.141) | 17.235 s | 34.828 s | 364 registered; 357 clean, 6 logged-gap, 1 logged-difference; 0 failed/skipped |
| CP218 | 170.350 s (1.859 + 165.609) | 17.844 s | 35.125 s | 365 registered; 358 clean, 6 logged-gap, 1 logged-difference; 0 failed/skipped |

CP215 is the closest recorded baseline, but its 28.672-second build command and CP218's 165.609-second command represent different changed-source/build states. CP218's build log reaches 749 Ninja steps, including relinking many test executables. The receipt does not establish whether a broad shared dependency, generator state, or another cause forced that work. Against CP217, CP218 adds 0.609 s to the full launcher and 0.609 s to CTest; these are single runs with one additional registered test, not evidence of a stable regression.

The CP216 launcher is the runtime outlier: 38.297 s total, 3.735 s above CP215. CTest accounts for only 0.406 s of that increase. Before/after fixture hashing totals 14.266 s in CP216, versus 11.969 s in CP215; source/binary bindings total 5.219 s versus 4.172 s. Inventory/report work is about 1.2 s in both. Thus the observed launcher increase is concentrated in integrity snapshot phases, whose timing varies and whose full checks remain required. CP217 and CP218 launcher totals return to 34.828 and 35.125 s.

Across CP216–218, CTest is tightly grouped from 17.235–17.844 s. The full launcher is 34.828–38.297 s. CP215 through CP218 grow from 362 to 365 registered tests while preserving the same six logged-gap cases, one diagnostic-difference case, and zero failures, skips, or incomplete output. The historical folder comparator remains 93 SAME / 5 DIFF; these suites are candidate regression evidence, not fresh original-session acceptance.

## Retries and rework

CP217 retained several separately named negative-ID runner build attempts (`cp217-negative-id-build`, `full-build`, and `rerun-build` through `rerun3-build`). The full-build receipt selected for this checkpoint records 4.516 s, while the CP217 validation explicitly says the original comparison runner remained disabled and unreviewed; therefore those runner repairs produced no original execution evidence. CP218's full build is the major measured build cost. No receipt reviewed here provides comparable elapsed time for original-probe setup or for each runner repair, so those intervals cannot be totaled or attributed more precisely.

## One measured improvement to try

The strongest actionable cost is CP218's 165.609-second incremental build command (749 Ninja steps). First use the existing Ninja explanation/graph output on the next naturally occurring long build to identify which changed dependency fans out to the relinked test targets. If a shared implementation header is the cause, isolate the evolving Open-directory decision code behind a narrow internal interface/translation unit, then compare the same incremental edit before and after on the same build tree. Keep full source/binary binding, fixture hashing, all registered tests, and the historical comparison unchanged. Do not claim a gain unless the matched measurement confirms it and the full regression still passes. This is a proposed experiment; no savings are measured here.

## Evidence

`analysis/resume215-efficiency-review.md`; CP215 `cp215-doe-io-build.json`, `cp215-doe-io-tests.json` and its timing receipt; CP216 `cp216-opdir-build.json`, `cp216-opdir-tests.json` and timing receipt; CP217 `cp217-negative-id-full-build.json`, `cp217-negative-id-full-tests.json` and timing receipt; CP218 `cp218-staged-open-dirs-full-build.json`, `cp218-staged-open-dirs-full-tests.json` and timing receipt; `build/logs/cp218-staged-open-dirs-full-build.log`; `analysis/wave3-checkpoint216-validation.json`, `wave3-checkpoint217-candidate-validation.json`, and `wave3-checkpoint218-validation.json`.
