# CP133 isolated-worker EOF race review

## Result

The retained failure is explained, but the proposed bounded EOF fix is not present in the current `terminal_harness_supervisor.cpp`; this remains a blocking review item. At the end of this inspection the supervisor still calls `finish()` immediately after observing pipe EOF, and turns an incomplete Started-only transcript into a protocol stop before checking whether the child has naturally exited. That ordering can replace an early worker return code 66 with the supervisor stop code.

## Bound failure evidence

`build/test-runs/1792930766955422100/receipt.json` identifies the 318-test run. Its `ctest.log` records `cli_isolated_worker_missing_caller_directory_has_no_trustworthy_returned_record` failing at `tests/g0/test_cli_isolated_worker.cpp:194`: expected child status 66, observed 3775201281 (`0xe1050001`, `supervisorStopStatus`). The same run shows malformed-job and stale-nonce cases passing, demonstrating that the failure is scheduling-sensitive. CP133 build receipt `analysis/resume133-final-build.json` binds the then-built source tree to SHA-256 `1dd83bf56b74242d66f0d01862e2777671854604aa2cab6b4d614ef7f9c71854` (3,474 files); that receipt does not include a fix after the failed run.

The worker source confirms that setup exceptions, including failure to restore the caller directory, print a diagnostic and return 66 without a Returned record. The child first emits Started; therefore the parent sees a valid Started followed by EOF with no final frame when setup fails. This is a harness/setup failure, never a successful conversion result.

## Current source ordering and bounds

In `src/engine/terminal_harness_supervisor.cpp`, the receive loop checks the absolute end-to-end deadline (`began + deadlineMs`) before each iteration and checks captured stdout/stderr sizes against `captureLimitBytes`. Protocol reads are limited to currently available pipe bytes and at most 256 bytes per read. The options constrain the deadline to 50–20,000 ms and each capture limit to 1 MiB.

However, after `PeekNamedPipe` reports `ERROR_BROKEN_PIPE`, the code sets `eof=true`. When no bytes are available, it calls `finish()` immediately (before the process-exit check). A Started-only transcript fails `finish()` with “v2 EOF without final outcome”; the loop records `StopReason::Protocol` and breaks. Cleanup then requests job termination if the process is not yet signaled. This matches the observed replacement status.

The necessary fix must wait for the child to signal natural exit after EOF before finalizing a no-final-outcome transcript, while repeatedly checking the same absolute deadline and output capture bounds. On signaled natural exit it should finalize the transcript as invalid/missing-final, classify the observation as harness failure, and retain the child's actual exit code (66). It must not treat incomplete protocol as success. On deadline/capture breach, the corresponding sticky stop reason should remain failure and owned-child cleanup must remain bounded. The current cleanup wait of 2,000 ms is separate from the child deadline and does not clear a sticky stop; the new EOF wait should not add another unbounded or post-deadline grace interval.

## Review disposition

No proposed fix was available to verify against these constraints. This receipt records the specific outstanding issue for the next source freeze; no source or tests were changed, and no build was run.
