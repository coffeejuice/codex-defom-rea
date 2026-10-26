# CP233–235 efficiency review

## Scope and confidence

This review uses the retained full build/test receipts, launcher phase receipts and checkpoint freezes. It distinguishes the CP233 ops-only change from CP234's shared-header change as identified in checkpoint context, but the receipts do not provide a matched control or causal Ninja explanation. Therefore the observed fan-out contrast is descriptive only. Token telemetry is unavailable; no token or effort claim is made.

## Measured results

| Checkpoint | Configure | Full build command | Logged build edges | CTest | Full launcher | Tests |
|---|---:|---:|---:|---:|---:|---:|
| CP233 ops-only | 1.656 s | 27.625 s | 347 | 18.032 s | 35.063 s | 369 |
| CP234 shared-header | 2.610 s | 118.328 s | 551 | 18.219 s | 36.735 s | 369 |
| CP235 page-state identity | 1.719 s | 111.563 s | 551 | 18.062 s | 35.672 s | 369 |

All full suites passed with 362 clean, 6 logged-gap, 1 logged-difference, zero incomplete outputs, skips or failures. Each freeze has `objective_complete: false`; none of these checkpoints establishes full project completion. CP234 also has an earlier configure/build attempt (1.782/0.109 s, build exit 1), followed by the successful focused-preflight/full-build receipt (2.610 s configure, 20.172 s preflight, 118.328 s full build). The failed attempt is repeat work and must remain visible in accounting, though it is not part of the successful build duration.

CP233's 347 logged build edges and 27.625 s differ substantially from CP234's 551 edges and 118.328 s. CP235 also logged 551 edges and took 111.563 s. This is consistent with the ops-only checkpoint producing lower incremental fan-out than the subsequent shared-header checkpoints, but it does not prove the header alone caused the difference: source edits, build-tree state, generated dependencies, and the CP234 retry differ. CP234–235's similar edge counts and command times provide no evidence of improvement from one to the other.

CTest time stayed tightly grouped at 18.032–18.219 s. Full launcher time varied more (35.063–36.735 s); fixture hash phases were 11.749 s (CP233), 12.594 s (CP234), and 12.391 s (CP235), with execution bindings 4.031, 4.640, and 4.016 s. Thus part of launcher variance is directly accounted by integrity-phase variance; these checks remain required. The previous CP232 arithmetic is corrected in its report: all six recorded configure/build commands sum to 140.781 s, while the two focused receipts alone sum to 26.547 s.

## Recommendation

Keep the current workflow and evidence gates. The CP233–235 observations do not establish a build optimization or a new test-time optimization. The prior 4/8/4 resource benchmark remains validated only for suite execution on its unchanged 151-test workload; it is not evidence that these full builds became faster. Do not treat fewer build seconds in CP233 as a reusable gain without a controlled fan-out comparison.

## One safe next experiment

At a later authorized candidate point, use the same candidate build tree, compiler, cache state, and build command to measure two isolated no-op-equivalent edits: one ops-only source edit and one edit to the suspected shared header. Save Ninja `-d explain` output and compile/link edge counts with each duration. If header fan-out is confirmed, evaluate a narrow compatibility-preserving internal header split against the same baseline. Retain source/binary bindings, uncached fixture hashes, the entire suite and freeze evidence. Report causal improvement only if the matched measurement supports it.

## Evidence

CP233: `analysis/cp233-heattreat-dimension-full-build.json`, `cp233-heattreat-dimension-full-tests.json`, and `wave3-checkpoint233-validation.json`.
CP234: `analysis/cp234-db-step-owner-build.json`, `cp234-db-step-owner-focused-build.json`, `cp234-db-step-owner-focused-tests.json`, and `wave3-checkpoint234-validation.json`.
CP235: `analysis/cp235-page-state-identity-full-build.json`, `cp235-page-state-identity-full-tests.json`, and `wave3-checkpoint235-validation.json`.
Previous review and policy: `analysis/resume232-efficiency-review.md`, `analysis/efficiency-policy.md`.
