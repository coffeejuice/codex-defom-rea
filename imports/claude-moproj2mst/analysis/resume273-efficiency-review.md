# CP271–273 efficiency review

## Scope and outcome

Reviewed the CP271–273 build, test, checkpoint, and retained implementation-review receipts at the source-frozen boundary. Each full suite has 369 registered tests and the same result: 362 passed, 6 passed with logged gaps, 1 passed with logged differences, and no incomplete output, skips, or failures. All three checkpoints retain `objective_complete: false`. Token telemetry is unavailable; this review makes no token or total-effort claim.

| Checkpoint/run | Configure | Build | Full CTest | Full launcher | Focused owner build(s) |
|---|---:|---:|---:|---:|---:|
| CP271 initial owner attempt | 1.718 s | 1.282 s, exit 2 | — | — | — |
| CP271 owner retry | 1.703 s | 2.562 s, exit 0 | — | — | — |
| CP271 full | 1.718 s | 28.875 s | 17.938 s | 35.047 s | — |
| CP272 scene owner | 1.890 s | 1.922 s | — | — | — |
| CP272 scene require variant | 1.953 s | 1.984 s | — | — | — |
| CP272 combined full | 1.922 s | 117.516 s | — | — | — |
| CP272 repaired full | 1.844 s | 0.203 s | 20.203 s | 37.375 s | — |
| CP273 owner | 1.812 s | 22.219 s | — | — | — |
| CP273 full | 1.875 s | 116.172 s | 19.578 s | 37.875 s | — |

These are different edits and build states, so they are not a controlled trend. CP272's 0.203-second repaired build is incremental and not comparable to its 117.516-second preceding build. CP271's first owner build failed while compiling the snapshot test; the retained retry log shows that target then built and linked successfully. The receipts do not establish a recurring infrastructure failure.

## Test and integrity timings

The full launcher times are 35.047 s, 37.375 s, and 37.875 s for CP271, CP272, and CP273. Their component receipts show:

| Checkpoint | Inventory | Fixture hashes, before + after | Bindings, before + after | CTest | JUnit/report |
|---|---:|---:|---:|---:|---:|
| CP271 | 1.079 s | 11.875 s | 4.015 s | 17.938 s | 0.125 s |
| CP272 | 1.078 s | 11.938 s | 4.000 s | 20.203 s | 0.125 s |
| CP273 | 1.140 s | 12.797 s | 4.188 s | 19.578 s | 0.125 s |

The repeated fixture-hash and binding work is an explicit cost of the integrity gate and remains required. CTest and launcher values are single observations; no performance regression or improvement is established. The test receipts record 8 fixture-hash workers, but do not measure source-owner concurrency or show that parallel assignments reduced elapsed time.

Full-build source fingerprinting completed twice per receipt: CP271 1.453 s total, CP272 1.531 s (initial and repaired), and CP273 1.484 s. The focused CP271 and CP273 owner receipts also report two calls each (1.390–1.500 s); CP272 scene focused receipts report 1.531 and 1.859 s. These scopes include hash and file count. They are not additive across overlapped workflow intervals.

Checkpoint reports list no changed diagnostics or reviewed diagnostic changes. They do not include a separate diagnostic-generation duration, so diagnostic time is unavailable. There are no byte or token savings inferred from compact receipts.

## One actionable correction

Keep the existing source-stable freeze and full regression gates. For the next review, capture one compact per-checkpoint accounting receipt that records focused build retries and their cause, full-build incremental/clean status, worker assignment/concurrency metadata, and any diagnostic-generation duration if present. Current build receipts expose the retry and build duration; checkpoint validation exposes diagnostics changed, but neither accounts for worker parallelism or diagnostic time. This will let the next review distinguish real workflow overhead without a second suite or weakening validation.

## Next review trigger

Review again after three source/fixture/contract checkpoints or 60 minutes, at the next safe frozen boundary. Do not rerun the suite solely for this review. Reassess build timing only with a same-tree, same-toolchain, controlled edit and Ninja edge counts; assess suite timing only with repeated runs under comparable machine load and retained integrity phases.

## Evidence

- `analysis/resume270-efficiency-review.md`
- `analysis/cp271-fill-stopping-owner-build.json`, `cp271-fill-stopping-owner-retry-build.json`, `cp271-fill-stopping-full-build.json`, `cp271-fill-stopping-full-tests.json`, `wave3-checkpoint271-validation.json`
- `analysis/cp272-slice-scene-owner-build.json`, `cp272-slice-scene-owner-require-build.json`, `cp272-combined-full-build.json`, `cp272-combined-repaired-full-build.json`, `cp272-combined-repaired-full-tests.json`, `wave3-checkpoint272-validation.json`
- `analysis/cp273-heattreat-owner-build.json`, `cp273-heattreat-full-build.json`, `cp273-heattreat-full-tests.json`, `wave3-checkpoint273-validation.json`
- Timing receipts: `build/test-runs/1793052653175448700/timing.json`, `1793054353158994600/timing.json`, and `1793055047878507100/timing.json`

