# CP109: remesh worker failure-order review

## Project and MST prefix timing

The four added worker cases are in `tests/s/test_terminal_remesh_worker.cpp`; the candidate ordering is visible in `src/core/convert.cpp`. In WRITE_SAVE, `saveCurrent` and project serialization run before the operation-property loop and MST. The remesh writer is called after MST handling, and `RemeshTerminalForwarder` captures the live `SaveCallState` only when the enabled remesh open has failed and the private backend dispatches.

Cases 19 and 20 use `ProjectFailureFactory`'s project-device `aboutToClose` callback. The callback fires while the real project output device is still in its close path, after Open and Save-current; it is before the project writer's error query and well before the step-5 MST open. It is an adequate point to arm the MST lock without intercepting Open. Case 19 returns declared error 7 only from the post-close `error()` query; the underlying FileDevice error is required to be 0. Thus its `projectWriteSucceeded=false` bit is a scripted adapter observation after a successful physical project write, not a physical project-write failure. Case 20 writes the MST sentinel and acquires a read handle with `FILE_SHARE_READ` there. The actual later MST write-open is denied; the project error query returns 0. The remesh primary lock is acquired by the parent fixture before child conversion; its compatible read sharing allows Open to scan the unmarked primary and fallback, while denying the remesh writer's write-open.

The candidate snapshot expected at the failed remesh-open dispatch is:

| Case | Project attempted/returned/succeeded | MST attempted/returned/succeeded | Snapshot flags (decimal) |
|---|---|---|---:|
| 19 `ConversionDeniedProjectFalse` | true / true / false | true / true / true | 951 |
| 20 `ConversionDeniedMstFalse` | true / true / true | true / true / false | 503 |
| 23 `ConversionPublicationFailure` | true / true / true | true / true / true | 1015 |
| 24 `ConversionEndpointReturned` | true / true / true | true / true / true | 1015 |

All four also expect `checkedSubprojects=1`, `answer=DialogAnswer::No` (`0x10000`), `pendingDefault=DialogAnswer::NoButton` (`0`), `opened=true`, `validationComplete=true`, and `lastOperationModalPending=false`. The fixture contains one sub-project. The snapshot flag set contains Present, Opened, Validated, ProjectAttempted/Returned, and MSTAttempted/Returned; success bits vary by row. ModalPending is absent. A must-be-last warning would set a pending modal/default and throw before these writers, so it cannot reach this remesh boundary. The source name `pendingDefault=NoButton` is a candidate state default, not a delivered dialog result.

These are candidate-only observations. Original static Save evidence establishes project → properties → MST → remesh and does not branch around remesh for returned project/MST false values. It does not contain a `SaveCallState` object, the injected project status 7, or a runtime capture for these conjunctions. The original failed enabled remesh `fopen` still enters the conditional invalid-parameter path; under the qualified default handler it terminates instead of returning a Save status. The candidate worker's `0xC0000417` endpoint remains explicitly candidate-fixed, with the native default-handler prelude unimplemented.

## Backend failures and unwind/Close claims

Cases 23 and 24 are harness/backend-failure injections after successful project and MST writes. Case 23 records the snapshot to the private ledger, deliberately returns `PublicationFailed` before sending the RemeshV2 Boundary frame, then `failClosed` publishes BackendFailure and the writer parks. Consequently the protocol transcript correctly has no Save snapshot for case 23; the private ledger records the dispatch and repeated fail-closed snapshot. Case 24 sends Boundary with the all-success snapshot, returns `EndpointReturned`, then follows the same fail-closed-and-park path. The supervisor expects BackendFailure, requests a stop, and treats the result as harness failure rather than terminal success.

The worker checks that neither backend-failure case creates `adapter-returned.txt` or the `UnwindMarker` file, that `transcript.returned` is false, and that the terminal backend ledger contains exactly dispatch then failClosed. The writer's implementation makes failClosed sticky: after it returns, `dispatchRemeshTerminal` sleeps indefinitely without unwinding. The outer converter contains no native normal-Close coordinator; absence of a child stack-unwound marker proves only that this candidate child did not unwind before supervisor stop. It does not establish native GUI Close behavior.

For cases 19/20, the backend dispatch itself calls the opted-in candidate `TerminateProcess` endpoint after publishing Boundary, so no adapter return or C++ stack unwind is expected. This is separate from cases 23/24 and from the original PE's conditionally terminating CRT handler. In all four cases, the input/after manifest comparison and terminal receipt are candidate worker evidence; they do not turn the constructed remesh overlay into a retained original enabled-remesh capture.

## Review result

By source and fixture inspection, the `aboutToClose` timing correctly arms the case-20 MST denial after Open/Save-current and before step 5; case 19's error-7 observation is sampled after project close and before later stages. The expected snapshot masks the right success bit for each writer, and the one-subproject/default/no-pending-modal fields match candidate Save validation. Cases 23/24 exercise sticky backend failures with no stack unwind. No ordering defect was found in these four cases by this read-only review.

No source/test/build, GUI, original execution, or mutable REA operation was performed for CP109.
