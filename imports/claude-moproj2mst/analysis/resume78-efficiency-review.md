# CP75–77 efficiency review

## Purpose

Measure the latest grouped build and full-launcher receipts, include CP76's controlled `-j 4`/`-j 8` comparison, and select one next experiment that retains full source/binary binding, fixture hashing, test membership, and assertions.

## Build receipts

| Checkpoint receipt | Source snapshot | Configure / focused / full build (s) | Total (s) | Outcome |
|---|---|---:|---:|---|
| CP75 `grouped-build` | `9145b698…`, 3,323 files | 1.953 / 0.703 / n/a | 6.519 | Failed focused target; source unstable |
| CP75 `grouped2-build` | same starting hash | 1.968 / 9.188 / n/a | 15.025 | Failed compile; source unstable |
| CP75 `grouped3-build` | `c86ed204…`, 3,323 files | 1.907 / 4.766 / 30.000 | 42.195 | Passed, source stable |
| CP75 `grouped4-build` | `1cf00d67…`, 3,323 files | 1.969 / 2.797 / 0.766 | 11.061 | Passed, source stable; incremental |
| CP76 `grouped-build` | `da5b4584…`, 3,327 files | 1.984 / 5.547 / 15.985 | 29.210 | Passed, source stable |
| CP77 `grouped-build` | `679a1e6f…`, 3,337 files | 1.953 / 6.141 / 16.188 | 29.850 | Passed, source stable |
| CP77 `grouped2-build` | `ac564063…`, 3,337 files | 1.922 / 3.937 / 0.782 | 12.231 | Passed, source stable; incremental |

These receipts cover different source snapshots, focused target groups, and cache/incremental work. The 0.766- and 0.782-second full-build phases are incremental completions, not comparable to the 15.985–30.000-second full phases. CP75 includes two failed attempts with source drift before successful builds; their durations are real iteration cost but cannot be assigned solely to grouped configure reuse or a stable candidate. No matched build control demonstrates a causal configure-reuse saving.

## Full test launcher receipts

| Checkpoint | Tests | CTest workers | Launcher (s) | CTest (s) | Fixture SHA before + after (s) | Binding before + after (s) | Outcome |
|---|---:|---:|---:|---:|---:|---:|---|
| CP75 | 228 | 4 | 30.859 | 12.578 | 6.328 + 6.703 | 1.828 + 2.297 | 0 failed, 0 skipped; recorded gaps/differences |
| CP76 integrated | 230 | 4 | 29.953 | 12.297 | 6.406 + 6.407 | 1.860 + 1.843 | 0 failed, 0 skipped; recorded gaps/differences |
| CP77 | 234 | 4 | 31.969 | 12.860 | 6.718 + 6.609 | 2.297 + 2.281 | 0 failed, 0 skipped; recorded gaps/differences |

All three launcher receipts used threaded fixture SHA-256 with four workers and `parent-cache`, with no persistent cache. Each reported zero fixture changes and stable source/binary bindings across the run. Registered test counts changed from 228 to 234, so the launcher observations are not a strict same-inventory trend. The recurring largest phases are fixture hashing (12.7–13.3 seconds total) and CTest (12.3–12.9 seconds). Binding phases together consume about 3.7–4.6 seconds; those phases include both source-tree and executable hashing, so the receipts do not isolate their contributions. Inventory and report parsing are small by comparison.

## CP76 controlled worker comparison

Two additional full-launcher runs used the same frozen CP76 build, source, fixture roots, and hash settings; only CTest jobs changed. The `-j 4` run took 34.469 seconds overall and 16.062 seconds in CTest. The subsequent `-j 8` run took 35.531 seconds overall and 16.390 seconds in CTest. Both registered the same 230 tests, passed with the same status counts (223 passed, 6 with logged gaps, 1 with logged differences), had no skips, failures, incomplete outputs, or fixture changes, and bound the same stable source hash (`da5b4584…`, 3,327 files). Their 227 before/after executable hashes matched; the frozen archive's three core executable hashes also matched.

In this one ordered pair, `-j 8` was 1.062 seconds slower overall and 0.328 seconds slower in CTest. This is not evidence of a general slowdown: the pair was not repeated or order-balanced. The additional CP76 `-j 4` launcher also took 34.469 seconds, versus 29.953 seconds in the earlier CP76 integrated receipt, while both used the same registered-test count and frozen source. The 4.5-second spread in the `-j 4` observations shows meaningful run-to-run or environment variation relative to the 1.1-second `-j 4`/`-j 8` difference. Do not attribute the latter confidently to worker count.

## Efficiency conclusion and next experiment

No measured optimization in CP75–77 has a causal end-to-end speedup established. The grouped build receipts are not matched source/target/cache comparisons. The CP76 worker comparison found no observed `-j 8` gain, but one ordered pair cannot distinguish worker-count effect from timing variation. Keep the existing `-j 4` default pending a repeated comparison.

The 10x objective remains unmet. The best full launcher in these receipts was 29.953 seconds. Relative to the historical 25.250-second fastest complete-launcher observation, that is about 1.19x slower, not a speedup; the historical observation is not a matched control. A tenfold target from that reference would be 2.525 seconds. CP75–77 provide no evidence near that result. Token savings remain unmeasured.

Next, run one balanced three-pair full-launcher comparison of `-j 4` and `-j 8` on a single frozen source/build snapshot. Use order `4, 8, 8, 4, 4, 8` to balance order effects. Hold threaded fixture hashing at four workers/parent-cache, source and binary snapshots, exact fixture roots, test inventory, output limits, and all other CTest options fixed. Require each run to retain every test, produce zero failures/skips/incomplete output, and show unchanged fixtures and stable source/binary bindings. Compare paired CTest and full-launcher medians; adopt `-j 8` only if the improvement repeats and exceeds observed run-to-run noise. This is a bounded benchmark only; it does not weaken coverage or assert a speedup in advance.
