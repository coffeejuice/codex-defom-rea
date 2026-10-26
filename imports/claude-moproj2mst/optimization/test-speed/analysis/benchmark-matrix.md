# Paired full-suite worker matrix

`admin/benchmark_converter_suite.py --matrix` measures hash and Save-folder worker configurations against an existing isolated build. Every sample invokes the complete runner with fixture hashes before and after, all registered converter tests, full output, and retained JUnit/case logs. This harness performs no build, fixture edits, original-app launch, cache flushing, or changes to the paused recovery source.

Each treatment is paired with a fresh control on the same build: serial SHA-256 and one folder worker. Pair order alternates control/treatment, then treatment/control. Treatment configurations cross serial plus threaded hash worker counts with the requested folder worker counts. The control configuration itself is excluded from treatments. Default hash worker counts are 1/2/4/8/10; default folder count is 1. Folder concurrency requires explicit `--folder-worker-counts 1 2 4 8 10`. Every child receives the declared `M2M_FOLDER_WORKERS`; inherited `M2M_FOLDER_PROFILE` is removed to preserve ordinary diagnostics and avoid profiling overhead. Ordinary non-matrix behavior remains available.

First inspect a plan. This launches no runner and writes nothing:

```powershell
python admin/benchmark_converter_suite.py --build build/test-speed-candidate `
  --matrix --plan-only --runs 1 --jobs 8 `
  --hash-worker-counts 1 2 4 8 10 --folder-worker-counts 1 2 4 8 10
```

The full grid has 29 treatments and 58 complete suite invocations per repetition; five repetitions need 290 invocations. Use bounded screens before committing to that cost. For example, measuring just threaded hashing at four workers requires two invocations per repetition:

```powershell
python admin/benchmark_converter_suite.py --build build/test-speed-candidate `
  --matrix --runs 5 --jobs 8 --hash-worker-counts 4 --folder-worker-counts 1 `
  --cache-state warm --note "same host; no competing suite runner"
```

Adding folder counts 2/4/8/10 with this one hash count creates nine treatments and 18 invocations per repetition. Existing short-path per-test scratch settings and fixture-root requirements remain enforced by the complete runner. Avoid concurrent suite runners, builds or captures during measurements. The harness runs its own samples sequentially.

Every run retains stdout/stderr, a bound copy of the runner receipt, a copy of `timing.json` when available, external process wall time, CTest time, full artifact paths, comparator command, comparator stdout, and comparator stderr. Prefer the runner's own `run/receipt.json`; legacy shared-receipt fallback requires a fresh modification timestamp and matching run/build. Requested hash settings must match the receipt. The normal run directories retain all input/output evidence.

After every invocation, the harness calls `admin/compare_test_speed_runs.py` against preserved checkpoint-45 full-run evidence. Defaults are `build/test-runs/1792854086489696400` and `analysis/wave3-candidate45-doe-fix-d-tests.json`, with explicit source roots `source` and `optimization/test-speed/source`. Override these using `--reference-run`, `--reference-receipt`, `--baseline-source`, and `--candidate-source` for a different preserved baseline. The comparator retains raw changes and hashes, permits only its declared normalization, and compares semantic diagnostics independently of timing. `--baseline` refers to the older ordinary timing-summary comparison and cannot be combined with `--matrix`.

The matrix stops on the first failed/incomplete run or unsuccessful comparator result, preserving a partial summary and returning 1. Every successful complete matrix returns 0. `all_evidence_equivalent` requires every planned sample to finish and pass its comparator. Configuration summaries separate observed valid-run timing from timing of equivalent evidence. Pair timing deltas appear only when both samples are valid and equivalent. No timing result alone implies equivalence, correctness, fresh original parity, or scientific completion. Existing oracle, ABI, skip and diagnostic qualifications remain attached to the evidence.

Cache state is an operator label. Record preparation and host load in `--note`; the harness cannot establish a cold cache. With five treatment samples, nearest-rank p95 equals their maximum; the control pool is larger because every treatment has a paired control. Interpret individual pair changes and each configuration's sample count alongside pooled control statistics.

Implementation validation used `--plan-only` and mocked Python runner/comparator functions: configuration counts, alternating pair order, complete summary, and early stop on failed equivalence passed. No full suite or build ran during this harness change. Performance and equivalence measurements remain pending real authorized runs.
