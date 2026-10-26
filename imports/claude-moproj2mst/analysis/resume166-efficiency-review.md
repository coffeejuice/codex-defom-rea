# Converter efficiency review after CP166

## Scope and evidence

Read-only review of the three completed checkpoint records and their build/test timing receipts: `wave3-checkpoint164-validation.json`, `wave3-checkpoint165-validation.json`, `wave3-checkpoint166-validation.json`, their referenced build/test JSON, and the source-review notes. This does not change implementation, tests, shared state, build outputs, GUI or live REA. The root recovery status currently marks its GUI/controller lane blocked on the unavailable GUI-release lifecycle; this report concerns the converter lane only.

## Measured build envelopes

All builds reused the same configured Ninja build directory (`configure_count=1`, `reconfigure_count=0`) and each successful completed build kept its source fingerprint stable. Recorded launcher elapsed times:

| Checkpoint attempt | Build workflow elapsed | Component envelope | Outcome |
|---|---:|---|---|
| CP164 grouped | 66.305 s | configure 2.281; grouped focused preflight 7.485; three separate focused CTest launches 0.750/0.750/0.907; build 43.515 | success |
| CP165 final | 44.695 s | configure 2.250; focused preflight 4.484; one focused CTest launch 0.719; build 29.828 | success |
| CP166 initial grouped | 15.438 s | configure 2.235; focused preflight 7.094; focused CTest failed (exit 8) | rejected at preflight; no full build |
| CP166 repaired | 60.226 s | configure 2.187; focused preflight 3.063; three separate focused CTest launches 0.719 each; build 42.734 | success |

The CP166 failed preflight plus repair attempt consumed 75.664 seconds of recorded build-launcher time before reaching the successful build. The initial independent source review accepted the test design. The focused CTest run then exposed a test assumption error: it expected later foreign/null root reassociation to replace an already-associated root, contrary to the store's one-shot association behavior. The reviewer accepted the subsequent test repair before the complete build/regression.

## Full regression / launcher timing

The 361-test source-bound CTest runs took 16.203 s (CP164), 16.031 s (CP165), and 15.984 s (CP166 repaired). Their separate timing receipts report `launcher_elapsed_seconds` and phase timing, but not model/token usage. Every validation records 354 clean passes, six suites with logged gaps, one suite with logged diagnostic differences, and zero failed, skipped, incomplete-output, or fixture-change cases. Those suite labels and the recorded historical 93 SAME/five DIFF remain qualified in the checkpoint validations; they do not constitute a fresh original-process acceptance run.

## Assessment and one optimization

Across these checkpoints the clean build workflows range from 44.695 to 66.305 seconds, but they are not controlled comparable measurements: target sets and focused-test counts differ. The CP166 repair adds a concrete 15.438-second failed attempt. Individual focused CTest invocations have about 0.72–0.91 seconds of recorded launcher time while their command-level execution receipts are much shorter; CP164's three separate launches sum to 2.407 seconds. A single focused CTest invocation using the same three exact test-name regexes (or one equivalent anchored alternation), with the same captured per-test logs and assertions, is the one measured optimization to consider for subsequent checkpoints. This is only a small launcher-overhead opportunity; benchmark it against the current separate-launch baseline before adoption, and retain source review, each focused test, complete regression, candidate freeze, and original-comparison gates unchanged.

The records do not support an x10 speedup claim. No complete worker wall-time or token telemetry is present: build/test launch envelopes cover recorded commands, not the full investigation, drafting, waiting, or review interval. Unattributed elapsed time cannot be assigned to model reasoning, provider latency, approvals, or other causes from these receipts. No token-savings claim is made.

