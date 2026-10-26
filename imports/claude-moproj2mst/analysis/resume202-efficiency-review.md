# CP200–202 converter efficiency review

## Scope and measured outcome

This read-only review compares frozen CP200/CP201 build and full-suite receipts and CP202 owned-process probe preparation/failed pre-resume run with `analysis/resume199-efficiency-review.md`. The converter test disposition stayed stable: 361 registered tests, 354 clean passes, six passes with logged gaps, one pass with logged differences, and zero failures, skips, or incomplete output. Historical comparison remained 93 SAME/five DIFF, with no fixture changes. These imported checks are not fresh acceptance in the original application.

| Checkpoint | Build workflow elapsed | Focused CTest command sum | Full CTest elapsed |
|---|---:|---:|---:|
| CP200 | 26.845 s | 3.094 s | 16.875 s |
| CP201 | 60.106 s | 3.234 s | 17.046 s |

Build-workflow elapsed includes configure, preflight, four focused CTest launches and build. The build phase was 0.812 s in CP200 and 31.125 s in CP201; most of the 33.261 s workflow increase is therefore build work, not test growth. Full suite duration moved only 0.171 s. CP200/201 each had one configure, no reconfiguration, and stable source fingerprints. The CP200 receipt's focused preflight was 9.063 s and CP201's 11.156 s. CTest sums are the recorded per-command elapsed values, not a separate wall-clock measurement.

The CP202 owned-process probe had a clear bounded failure outcome but no receipt for total preparation wall time. Its attempt record reports a hook error at EXE RVA 0x154a4 while suspended, then terminated the single owned PID through its retained handle. It never resumed the target and observed no Open/Save/dock event or accepted Close. The initial runner's Windows CIM output was empty, yet that version treated it as absence; its aggregate absence result stayed false because `sessionCleanupSettled` was unset. Independent exact-PID checks later found the process absent. The 27 disposable fixture hashes remained unchanged. The subsequent repair explicitly fixed the `Session.isDetached()` method/property mistake and made Windows absence parsing fail closed. This is concrete review/rework caused by an API-contract mismatch; the first hook failure's underlying cause remains unresolved, so no runtime diagnosis or time saving can be assigned to it.

## Workflow trend and bounded improvement

Compared with CP197–199, there is still no meaningful full-suite speed trend: 17.172–17.750 s previously versus 16.875–17.046 s here. The full test remains a roughly 17-second fixed cost. The focused invocation sum fell from about 8.1 seconds to about 3.1 seconds, but this could reflect changed focused target coverage and warm build state; it is not a controlled speedup claim. CP201's full build phase also demonstrates why the full workflow cannot be summarized by a single improving trend.

One bounded experiment remains justified: run the same five focused targets in one anchored CTest process versus the five separate launches on the same frozen tree. CP200/201 used four separate focused CTest invocations, whereas CP197–199 used five; invocation aggregation may reduce startup overhead. Before adoption, preserve per-test status and logs, exact target coverage, source/executable binding, full-suite regression, ABI checks, raw comparisons and freeze rules. Compare repeated same-tree measurements and accept the change only if receipts remain equivalent and show a reliable wall-time reduction. The current 3.1-second sum bounds the focused test commands in these receipts; it is not an estimate of savings.

CP202 also shows a review-cost opportunity: qualify observer API assumptions with local declarations and mocked contract checks before an owned-process attempt. The repaired method-state and CIM parser checks caught a contract error without another target run. This is a process observation, not proof that review or capture will be faster; keep independent source/lifecycle reviews and evidence requirements.

No implementation, test, integrity check or original-runtime evidence should be removed on these measurements. No x10, token-savings or end-to-end speed claim is supported.

## Evidence

`analysis/resume199-efficiency-review.md`; `analysis/resume200-jne-schema-coverage-build.json`; `analysis/resume200-jne-schema-coverage-tests.json`; `analysis/resume201-edge-effect-build.json`; `analysis/resume201-edge-effect-tests.json`; `analysis/wave3-checkpoint200-validation.json`; `analysis/wave3-checkpoint201-validation.json`; `analysis/resume202-original-probe-attempt1.md`; `analysis/resume202-original-save-probe-preparation.md`; and the retained raw run under `analysis/resume202-passive-oracle-run-2026-10-26T04-28-23-604Z/`.
