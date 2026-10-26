# CP197–199 converter efficiency review

## Scope and result

This read-only review compares the frozen CP197–199 build and full-suite receipts with `analysis/resume196-efficiency-review.md`. Each checkpoint retains 361 registered tests with 354 clean passes, six logged gaps, one logged-difference pass, and no failures, skips, incomplete output, or fixture changes. Historical results remain 93 SAME/five DIFF. These imported/synthetic checks do not establish fresh acceptance in the original application.

## Measured command receipts

| Checkpoint | Focused preflight | Five focused CTests (sum) | Full build workflow | Full CTest |
|---|---:|---:|---:|---:|
| CP197 | 8.468 s | 8.171 s | 61.110 s | 17.172 s |
| CP198 | 9.875 s | 8.267 s | 64.265 s | 17.218 s |
| CP199 | 0.750 s | 8.109 s | 67.821 s | 17.750 s |

Focused totals sum separate test step elapsed values and are not a single measured wall interval. The build workflow includes configure, focused preflight and tests, and full build. The full build phase itself took 29.421 s, 30.437 s, and 43.984 s respectively. CP199's overall build workflow rose despite a much shorter preflight; evidence points to its full build phase, but does not explain the cause. Configure reuse was enabled, with one configure, zero reconfigurations, and no drift in all three receipts. Each suite finished with the same disposition.

Compared with CP194–196, full-suite elapsed remains in the same narrow range (17.172–17.750 s here versus 17.375–19.375 s previously). There is no demonstrated speed trend. Fingerprinting consumed 17.580, 18.186, and 17.469 seconds across 26 calls per build receipt; the repeated checks are retained source-stability evidence, so removing them is not recommended absent an equivalent integrity method.

## Actionable workflow experiment

The prior review proposed comparing one anchored CTest invocation for the five focused targets against the separate launches. CP197–199 still use five separate invocations, so the experiment remains open. A bounded same-frozen-tree A/B can compare wall time while retaining each per-test log and status, exact target coverage, source/executable binding, full regression, ABI checks, raw comparisons, and freeze rules. Adopt only if the harness preserves those receipts and repeated measurements show a reliable saving. The measured focused sums are about 8.1–8.3 seconds; that is an upper bound on the work being considered, not a predicted saving.

No x10, token-savings, or end-to-end speed claim is supported. No implementation or validation step should be dropped based on these measurements.

## Evidence

`analysis/resume196-efficiency-review.md`; `analysis/resume197-repair-build.json`, `analysis/resume197-repair-tests.json`; `analysis/resume198-callee-prefix-build.json`, `analysis/resume198-callee-prefix-tests.json`; `analysis/resume199-integrated-build.json`, `analysis/resume199-integrated-tests.json`; and `analysis/wave3-checkpoint197-validation.json` through `analysis/wave3-checkpoint199-validation.json`.
