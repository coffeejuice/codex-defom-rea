# CP79: full-run lower bound and exact-hash optimization candidate

## Purpose

Use the bound CP75–78 run receipts to estimate the attainable time for the unchanged full verification suite and select one hashing optimization that can be tested without reducing coverage or changing the binding contract.

## Bound run measurements

| Checkpoint | Tests | Full launcher (s) | CTest (s) | Fixture SHA before + after (s) | Source/binary binding (s) |
|---|---:|---:|---:|---:|---:|
| CP75 | 228 | 30.859 | 12.578 | 6.328 + 6.703 | 1.828 + 2.297 |
| CP76 | 230 | 29.953 | 12.297 | 6.406 + 6.407 | 1.860 + 1.843 |
| CP77 | 234 | 31.969 | 12.860 | 6.718 + 6.609 | 2.297 + 2.281 |
| CP78 | 237 | 30.422 | 12.359 | 6.735 + 6.406 | 1.906 + 1.844 |

Every receipt used the same exact SHA-256 fixture snapshot contract, threaded with four workers and `parent-cache`, with no persistent content cache. All report unchanged fixture manifests and stable source/binary bindings. CP78 bound to source SHA-256 `b9f087725a19b42c0eff627d71f1bc2ba4ae83f540d1fe7500ea463f23e9916d` across 3,344 files, registered 237 tests, had no failures, skips, or incomplete outputs, and retained 6 logged-gap and 1 logged-difference statuses. CP78 validation still records `objective_complete: false`; these timing receipts are not a claim of overall recovery completion.

The CP76 controlled `-j 4`/`-j 8` pair is in [resume76-ctest-parallelism-benchmark.md](resume76-ctest-parallelism-benchmark.md). It showed `-j 8` 1.062 seconds slower overall and 0.328 seconds slower in CTest in one ordered pair; it did not establish a worker-count effect. No CP76–78 observation justifies changing CTest concurrency.

## Lower bound relative to the 10x goal

At CP78's observed rates, fixture hashes before/after cost 13.141 seconds and CTest costs 12.359 seconds. Together they take 25.500 seconds, or about 83.8% of the 30.422-second launcher. If all other phases were somehow reduced to zero while these required exact fixture scans and the complete CTest suite retained their current times, the resulting estimate would still be about 25.5 seconds. That is only about a 1.19x reduction from CP78, not 10x.

The 10x target relative to the historical 25.250-second complete-launcher reference is 2.525 seconds. Even against CP78's current 30.422 seconds, a tenfold target would be 3.042 seconds. Both targets are far below the 12.359-second CTest phase alone and the combined 25.500 seconds of CTest plus exact fixture hashing. This is an observed-throughput lower bound, not a physical limit: faster hashing or faster execution could reduce those phases. But no optimization limited to inventory, parsing, or other small launcher overhead can approach 10x. Reaching that target while preserving the scope would require very large improvements to the underlying full test execution and full fixture reads themselves; these receipts provide no evidence of such improvements.

## One safe, causally testable hashing candidate

Benchmark a 4 MiB read buffer in the fixture hash helper against the current 1 MiB buffer. Keep its root enumeration, `parent-cache` path handling, SHA-256 algorithm, four-worker limit, bounded pending queue, and per-file size/mtime capture unchanged. This targets the measured 13.1 seconds of repeated fixture scans while still reading every file's bytes both before and after CTest. It introduces no persistent cache, skip, sampling, or weaker manifest check.

Use a benchmark-only implementation on the frozen CP78 fixture roots: alternate 1 MiB and 4 MiB runs three times, clear neither filesystem nor Python caches, and record the cache/order limitation. For every pass, require equality to the existing exact manifest including path keys and insertion order, size, nanosecond modification time, and SHA-256. Confirm the source/binary hashes and fixture topology remain unchanged. Compare per-pass medians and observed spread; reject the variant unless its time gain exceeds run-to-run noise. Only if that isolated result is material should the buffer change be integrated into a disposable full-launcher run, with the entire fixture snapshots, source/binary bindings, 237-test inventory, and CTest outcomes intact. This is a proposal only; no benchmark or source edit was performed for CP79.
