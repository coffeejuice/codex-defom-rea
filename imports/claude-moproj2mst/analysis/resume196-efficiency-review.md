# CP194–196 converter efficiency review

## Scope and result

This read-only review compares retained build and full-suite receipts for CP194–196 with the CP191–193 review. All three frozen checkpoints retain the same 361-test disposition: 354 clean passes, six passes with logged gaps, one pass with logged differences, and zero failures, skips, incomplete output, or fixture changes. Historical comparison remains 93 SAME/five DIFF. These synthetic/imported checks do not establish fresh acceptance in the original application.

## Measured command receipts

| Checkpoint / receipt | Focused preflight | Focused CTests | Build | Full CTest | Result |
|---|---:|---:|---:|---:|---|
| CP194 | 9.250 s | six tests; 10.578 s summed receipt elapsed | 36.578 s | 19.375 s | completed with recorded gaps |
| CP195 | 2.656 s | six tests; 9.813 s summed receipt elapsed | 33.234 s | 17.968 s | completed with recorded gaps |
| CP196 initial Report-null build | 9.422 s | five test receipts; 8.904 s summed receipt elapsed | 30.812 s | no separate full-test receipt file | initial build completed before source-review blocker/repair |
| CP196 final repaired build | 9.140 s | five tests; 8.374 s summed receipt elapsed | 31.359 s | 17.375 s | completed with recorded gaps |

The focused CTest totals are sums of individual step `elapsed_seconds`, not a single uninterrupted wall-time interval. Build receipt elapsed times were 75.771 s (CP194), 63.955 s (CP195), 65.117 s (CP196 initial Report-null build), and 64.561 s (CP196 final repaired build); these include workflow steps beyond compilation. The final repaired build and its full CTest receipt total 81.936 s. No focused test failed in these receipts. The intervening CP196 source review found a same-address replacement qualification issue; the repair added an allocation-occurrence witness and re-review passed. Review and repair labor has no timed receipt, so it is not included in command totals.

The previous CP191–193 review measured final CP193 build workflow at 63.367 s and full CTest at 17.406 s; its initial failed attempt cost 17.437 s, including source instability and 7.766 s across 11 fingerprint calls. Across these checkpoints, full-suite times are close (17.375–19.375 s) and do not show a trend. CP194 had a higher focused preflight than CP195, while both CP196 builds again took about nine seconds for preflight. Existing evidence does not isolate why, and no speedup is inferred.

## One candidate workflow improvement

Consider one anchored CTest invocation for the already-required focused targets, using an exact anchored alternation regex and retaining per-test logs/results. Before adopting it, compare that launch against separate invocations on the same frozen tree. This could reduce repeated CTest startup overhead; it must preserve all focused targets and their diagnostics, source/executable binding, the full regression suite, ABI checks, raw comparisons, and freeze rules. The receipts report individual elapsed values but do not provide a fair paired benchmark, so this remains a proposal rather than a measured saving.

No token-savings, x10, or end-to-end speed claim is supported. Review again after CP199 or one hour, at the next safe boundary.

## Evidence

`analysis/resume193-efficiency-review.md`; `analysis/resume194-dual-build.json`, `analysis/resume194-dual-tests.json`; `analysis/resume195-dual-build.json`, `analysis/resume195-dual-tests.json`; `analysis/resume196-repair-build.json`, `analysis/resume196-repair-tests.json`, `analysis/resume196-report-null-build.json`; and `analysis/wave3-checkpoint196-validation.json`.
