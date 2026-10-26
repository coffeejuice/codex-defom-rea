# Runner self-test profile and optimization

## Baseline evidence

The retained JUnit baseline `build/test-runs/1792854086489696400/junit.xml` reports `m2m.g0.runner_self_test` at 2.112 s end-to-end. Its retained log is `build/test-runs/1792854086489696400/m2m.g0.runner_self_test.log`; the test passed and exercised four dummy CLI modes:

- `ok`: 3 PASS cases and 1 NOORACLE case
- `flip`: 2 DIFF-at-line-3 cases
- `touch`: 2 CHANGED cases
- `none`: 1 NOOUT case

The baseline log has no phase timings. No before/after runtime measurement is available.

## Changes

In the isolated `optimization/test-speed/source/docs/re/moproj2mst/runner/run_corpus.py` copy:

- `M2M_RUNNER_SELF_TEST_TIMING` enables stderr receipts for corpus TSV loading, case selection, each mode run, and total self-test time. Timing is disabled by default and does not change normal output.
- `self_test()` now passes its already-loaded case table into each `run_all()` call. Previously `run_all()` reread and reparsed the same `cases.tsv` four times across the four modes, after `self_test()` had already loaded it. Ordinary `run_all()` calls still load cases from disk as before. Each mode receives the same immutable case dictionaries and applies the same filters and assertions.
- Removed the unused corpus-wide `copied` ID list. It traversed all cases but was not read afterward.

The test still launches the dummy CLI subprocesses for every case and mode and retains the existing PASS, NOORACLE, DIFF, CHANGED, and NOOUT checks. No corpus, oracle, or fixture content was changed.

## Validation

No build or test was run, per instruction. `git diff --check` was clean. The new timing receipts and the optimization's actual runtime effect remain unmeasured; the 2.112 s JUnit figure is baseline only. A scoped `m2m.g0.runner_self_test` run with `M2M_RUNNER_SELF_TEST_TIMING=1` is needed to quantify phase costs and confirm execution after this edit.
