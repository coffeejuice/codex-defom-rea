# Remaining test costs and next candidate

## Evidence and ranking

Parsed the retained full-run JUnit file at `build/test-runs/1792854086489696400/junit.xml` (150 test cases). This is a historical baseline; it was not rerun. Excluding `m2m.s.save_folder`, `m2m.s.save_io`, `m2m.s.save_current`, `m2m.release.unchanged`, and all `m2m.mdt.*` tests, the other 80 tests total about 11.25 seconds. The highest remaining observed costs are:

| Rank | Test | Wall time |
|---:|---|---:|
| 1 | `m2m.g0.runner_self_test` | 2.112 s |
| 2 | `m2m.moproj.moproj_write` | 1.163 s |
| 3 | `m2m.moproj.moproj_ops` | 0.734 s |
| 4 | `m2m.moproj.deflate` | 0.696 s |
| 5 | `m2m.p08.corpus_lines` | 0.563 s |
| 6 | `m2m.p09.corpus_lines` | 0.526 s |
| 7 | `m2m.moproj.moproj_lists` | 0.524 s |
| 8 | `m2m.p04.corpus_numbers` | 0.521 s |
| 9 | `m2m.p11.xcheck` | 0.480 s |
| 10 | `m2m.release.exports` | 0.452 s |

`m2m.s.save_answer` ran in 0.026 s; `m2m.s.save_dispatch` in 0.021 s; `m2m.s.native_directories` in 0.057 s. The selected `save_answer` source has four concise state/contract cases, including explicit no-write assertions. No redundant expensive operation or removable coverage was evident. At 26 ms total, changing this test has negligible suite impact and a material risk of weakening useful answer-state coverage. I made no source edit.

## Recommended next profiling step

Inspect and phase-profile `m2m.g0.runner_self_test` first: it is the highest remaining measured test and about 64% of the combined 3.275 s for the top two candidates. Preserve its complete self-test cases and failure assertions. If its time is predominantly fixed interpreter startup or setup, further test-source edits may not be worthwhile; if a repeated expensive operation is demonstrated, optimize that isolated test after confirming file ownership. `m2m.moproj.moproj_write` is the next source-level candidate at 1.163 s.

The JUnit values rank test processes only; they do not identify where time is spent. No build, test, or fixture mutation was performed in this lane.
