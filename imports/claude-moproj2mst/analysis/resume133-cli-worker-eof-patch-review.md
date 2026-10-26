# CP133 isolated-worker EOF race patch review

## Review result

The patch closes the reported status race without accepting incomplete protocol as success. The EOF failure remains sticky (`StopReason::Protocol`, invalid `v2 EOF without final outcome` transcript); the 1-second natural-exit grace is capped at the original absolute deadline and checks captured output sizes while polling. If the child exits naturally, its actual status (including 66) is retained, but classification remains `HarnessFailure` because the transcript is invalid and stop reason remains Protocol. If it does not exit, the existing owned-worker termination path runs.

One actionable cleanup robustness issue remains: the newly added grace calls `fileSize()` outside the main `try`/`catch`. A Win32 `GetFileSizeEx` error throws from `require()` and can escape `supervise()` before it returns an `Observation` or runs normal stop/inspection handling. This path is rare for the owned regular-file handles, but it defeats the supervisor's error-to-observation contract. Catch failures within the grace cleanup, keep the existing sticky failure, and proceed to owned-child termination/cleanup; do not let a capture-inspection error clear or replace the original protocol failure.

## Bounded-wait checks

- `eofExitGraceMs` is 1,000 ms. `graceEnd` is `min(began + deadlineMs, now + eofExitGraceMs)`, so the grace cannot extend the configured end-to-end child deadline.
- The loop polls the process handle with a maximum 10 ms wait, rereads stdout/stderr sizes on each iteration, and breaks if either exceeds the validated capture limit (1 MiB maximum).
- After grace, if the process is still live, the supervisor requests termination; a separate 2,000 ms cleanup wait remains bounded. The sticky stop reason is never cleared.
- The test asserts that stale-nonce EOF stays invalid, is classified `HarnessFailure`, preserves natural exit 66 and does not request supervisor termination. Root reports the focused test and 20 repetitions passed; this review did not rerun them.

## Evidence and binding

The prior failed run is retained at `build/test-runs/1792930766955422100/ctest.log`: missing-caller-directory expected child status 66 but observed `0xe1050001`. CP132 full-run and retry JSON receipts named in `resume133-cli-worker-eof-race.md` are preserved separately.

Frozen supervisor SHA-256: `a4eb3b2a9010179ca9cf5dc6d1587dab28169d214fcd1c81df00d246481ec83d`.

Frozen test SHA-256: `506ed3841129e3fb6bc558850b2be3600ff17c4b22c85d6b8af5abf72d285583`.

No source/test/build changes were made in this review. The reported 20-repeat result is root-provided runtime evidence; the exception-path finding is static review only.
