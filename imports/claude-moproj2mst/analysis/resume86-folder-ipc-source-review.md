# CP86 independent source review: bounded open-only IPC retry

The corrected source has no remaining blocker visible in this review. Retry is confined to unsuccessful mailbox open, both message directions use it, and opened frames retain fatal parsing/identity/tally/EOF checks. The initial released source had a compile blocker in the done producer; it was reported immediately and corrected by the owner before this final source binding.

Only this report was written by the reviewer. No source/test edit, worker build, executable test, GUI or mutable REA action occurred. Root separately reported that its compile confirmed the initial blocker; no build/test success is inferred here. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Source bindings and repaired blocker

Baseline CP85 archived test_save_folder.cpp SHA-256: `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`. The CP85 source tree receipt is `48d7798c0d97342a288a1c11561ac5eabdd4f3a2860a211d949ea7e8250ab567`.

Initial CP86 release SHA-256 `27c1d2e1ed12bced88b21af81d1b966abf683b9a4e0f9a63e52dd6e7f9674952` referenced the deleted local compare variable when publishing the done record near lines 572–573. readPostRequest had replaced that local with parsedRequest, leaving two names out of scope. The corrected file uses `parsedRequest.compare ? 1 : 0` for the terminal field and `if (parsedRequest.compare)` for tally/output publication.

Final reviewed source: `source/apps/moproj2mst/tests/s/test_save_folder.cpp`, 1,011 lines, SHA-256 `dd44cc93dce3ebabf12dbb9ec51ef6c54efda07b65377694c7a0861c1bd97ac0`. This is a source snapshot, not an immutable build acceptance. Root must bind its rebuild to this corrected hash rather than the original implementation receipt hash.

## Retry boundary and timing

openMailbox constructs a fresh ifstream for each attempt and invokes only an opener callback. If is_open succeeds, it returns that stream immediately. All getNumber/getString/schema/ordinal/operation/tally/output/EOF parsing happens after openMailbox has returned. There is no parser catch-and-retry loop and no later reopen after bytes have been consumed. An exception from an opener also propagates; it is not converted into a successful empty frame.

The constants are 10 total attempts and Sleep(10) only between unsuccessful attempts. Therefore persistent denial requests exactly nine 10 ms sleeps, or 90 ms of nominal sleep, plus ten open operations. A third-attempt success requests two sleeps. This satisfies a short bounded attempt budget within a nominal 100 ms sleep allowance; it is not a strict 100 ms wall-clock deadline because open calls and OS scheduling add time. The existing independent waitMailbox existence/process checks and ten-minute infrastructure limit are unchanged.

Both actual consumer sites use the same opener:

- folderPostWorker waits for the final .ready path, opens with openMailbox, then calls readPostRequest.
- Coordinator collect waits for .done with the child-process handle, opens with openMailbox, then calls readPostResult.

Neither path uses the retry for input fixtures, conversion output, captured stdout/stderr or job identity reads. The producer still publishes only through the existing temporary file close/check/rename sequence.

## Opened-frame validation stays fatal

readPostRequest requires the exact ready schema, a nonzero bounded ordinal equal to the expected ordinal, compare in {0,1}, and EOF. The explicit ordinal bound strengthens the stated invariant; accepted old frames already inherited that bound through equality to the checked manifest ordinal. A schema mismatch now has a more specific Unknown post request schema diagnostic, retaining failure behavior.

readPostResult requires the exact done schema, exact complete job identity bytes, ordinal, compared in {0,1}, expected terminal state, bounded/consistent tally, no comparator refusal, bounded output length and exact EOF. It reads into a local PostResult and only commits the event's tally/output after full validation. The old code assigned some event fields before requireEnd; delaying commit changes no accepted frame and makes partial failure state less observable.

An opened empty/truncated frame, bad schema, stale identity, wrong ordinal/terminal state, oversized field, invalid tally or unexpected tail cannot reach another open attempt in this control flow. getNumber/getString exceptions propagate normally. No missing field becomes zero and no partial payload is accepted.

For compare=false, collect retains the previously captured converter refusal tally/output rather than assigning the default empty PostResult fields. For compare=true, it still adds exactly the validated comparator tally and captured output. This preserves refusal accounting and case order.

## Producer, comparison and fixture checks

The following function slices were compared directly to the actual CP85 archived source and are exact normalized-text matches: compareCase, runCase, finishMailbox, publishPost, waitMailbox, readHarnessFile, getNumber, getString, requireEnd, putTally, getTally and addTally. The final done producer uses the same schema/identity/ordinal/0-or-1/tally/output frame and calls the unchanged finishMailbox.

Consequently this edit does not alter conversion order, work-folder copies, raw/tree comparison, declared view/banner normalization, missing/extra file checks, refusal behavior, close/check/rename publication or fixture source paths. There is no fixture-output bypass or newly accepted diagnostic difference. One new Testkit case will add its normal case line and change the test summary count in whole executable stdout; that expected harness text delta must not be confused with a changed folder-case result or an identical raw full-log hash.

## Release-active fault checks and limits

New case folder_pipeline_mailbox_open_and_truncation_checks has these deterministic checks:

| Probe | Active assertion |
|---|---|
| Two injected failed opens, then valid .ready open | Exactly three opener calls; correct ordinal 42 and compare=true after full parse |
| Permanently failed open | No opened stream, exact constant-bounded attempt count, distinct open diagnostic and mailbox path |
| Opened truncated .ready | Exactly one open and Truncated folder IPC |
| Opened truncated .done tally | Exactly one open and Truncated folder IPC |

The tests use actual file reads after deterministic failed-open injection; they do not reproduce a measured Windows transient sharing failure. The new case does not separately inject every mismatch/malformed variation or a transient valid .done, but the common open helper plus parsers outside its scope makes no-read-retry apply to both directions. Existing direct-worker protocol checks remain relevant and should be rerun by root against the corrected executable. This review does not claim those runs occurred.

Checks use supported Testkit REQUIRE/CHECK/CHECK_EQ with no raw C assert or REQUIRE_EQ. Both quoted includes resolve. The entrypoint remains Windows wmain, with the worker argument branch before Testkit execution; the generic preflight missing_int_main warning does not indicate a missing program entrypoint. The ordinary run now has two cases, including the original full folder sweep.

Fault files stay below the configured scratch root in a newly created PID/tick child. The cleanup guard removes only a directory this case successfully created; open streams are destroyed before that guard. No global folder-case discovery/tally state is modified by the new fault case.

Root can rebuild the corrected file, run the focused folder/worker protocol checks and full source-stable suite, and compare complete per-case output/tallies/fixtures before the exact freeze. Open retry does not establish the root cause of CP85's earlier failure, and this source review makes no test-pass, speed or token-saving claim. Token measurements are unavailable.
