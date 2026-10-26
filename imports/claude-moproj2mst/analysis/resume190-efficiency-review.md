# CP188–190 converter efficiency review

## Scope and result

This read-only review covers the final CP190 build and full imported-converter CTest receipt, CP189 queue-correction evidence, CP190 partial-state tests, and the CP187 efficiency review. The final CP190 test receipt reports 361 registered tests: 354 clean passes, six passed with logged gaps, one passed with logged differences, and zero incomplete, skipped, or failed tests. The build receipt records an unchanged source-tree fingerprint of 3,554 files throughout. These are synthetic/imported regression results; they do not establish fresh original-application acceptance.

## Measured command timings

| CP190 final workflow command | Receipt elapsed |
|---|---:|
| Configure | 2.469 s |
| Focused build preflight | 8.859 s |
| Focused CTests (six separately recorded) | 0.765, 0.734, 5.625, 5.156, 1.032, 0.766 s |
| Full build | 30.985 s |
| Full CTest suite | 16.765 s |

The recorded build workflow sums to 67.156 s. That is the sum of receipt step elapsed values, not an independently measured end-to-end wall time. The two save-I/O focused gates account for 10.781 s together; the other four account for 3.297 s. CTest's command-only times are separately recorded and lower than outer elapsed values, so the table consistently uses outer receipt elapsed values. No timing from CP189 queue correction was found that supports a before/after efficiency claim.

## Retry and queue quality

CP187 documented repeated `m2m.s.save_current` focused failures followed by successful final validation, and recommended retaining compact failure classification before retrying. By CP190, the final focused sequence passes both save-I/O gates, including `m2m.s.save_current`, with stable source/cache fingerprints through the build. This is a favorable outcome, but the available final receipts do not establish how many retries were avoided or whether queue correction reduced elapsed time. The queue correction is therefore a reliability/provenance observation, not a measured speedup.

The CP189 and CP190 factory tests explicitly exercise reentry and corruption refusal. CP189 checks the control-flow jump changes only the cursor while preserving predecessor observations, tests intermediate budgets through resumed execution, and refuses corrupted predecessor/terminal state without mutation. CP190 tests publication limited to task+8 and cursor, default/intermediate-budget stops, non-replay on reentry, premature-publication refusal, and expired-widget-root refusal. All eight CP189/190 cases appear as passing in the focused log. That gives good evidence that retry/resume boundaries are explicit and mutation-safe in these cases; it does not measure operational retry frequency.

## One safe change

For the next failed focused gate, emit one compact retry receipt with test name, failure class, executable/source/cache fingerprints, fixture identity, and whether the owning target was rebuilt immediately before execution. Use that receipt to decide whether to rerun the implicated focused target or continue to the full gate; never waive a failing gate or change frozen fixtures. Compare the count and elapsed time of retries at CP193 against this checkpoint before claiming an improvement.

No x10 efficiency or token-saving claim is supported by these measurements. Next review: after CP193 or one hour, at the next safe checkpoint.

## Evidence reviewed

`analysis/resume187-efficiency-review.md`, `analysis/resume190-dual-build.json`, `analysis/resume190-dual-tests.json`, `analysis/wave3-checkpoint189-validation.json`, `analysis/wave3-checkpoint190-validation.json`, `analysis/resume189-connector-plan-review.md`, `analysis/resume190-root-integration.md`, and the CP190 `m2m.mdt.doe_task_factory` focused test log under `build/test-runs/1792977490291359600/`.
