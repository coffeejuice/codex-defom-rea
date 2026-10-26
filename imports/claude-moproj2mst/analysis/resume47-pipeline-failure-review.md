# Resume 47: checkpoint 46 folder-pipeline failure review

## Scope and file identity

Static comparison only. No source, build, application or REA state was changed; no tests were run. The current checkpoint 46 test file is SHA-256 `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`; the accepted isolated reference is `8a27894ee6a289d844914bfaff1984b20b922cf8a15ef44d3287827002176dbb`.

The checkpoint 46 implementation keeps the accepted normal-path scheme: serial `runCase` calls on the coordinator, a separate post-comparison helper, a maximum of two queued results, and output replay only after the helper completes. Its prefetch and sharding branches are absent. The established synthetic receipt (`optimization/test-speed/analysis/folder-pipeline-failure-plan.md`) covers matching serial/pipeline stdout and stderr for refusals, a copy failure and preflight omissions, plus direct rejection of truncated helper input. The receipt expressly does not establish parent behavior after helper crash or incomplete IPC.

## Concrete unverified failure difference

The original serial fallback completes `compareCase` and prints each case result before starting the next case. The pipeline defers comparisons and retains diagnostics, then replays the buffered case output and summaries only after all queued helper results and worker exit validate. With two results allowed in flight, a comparison/helper failure for case N may occur after conversion of N+1 has begun. On that infrastructure failure the coordinator throws before replaying any buffered output; the serial fallback would already have emitted earlier case output and would not have started N+1 if comparison N itself had failed.

This deferral is also present in the accepted isolated implementation, so it is not a new checkpoint 46 divergence from that candidate. It is a concrete failure-mode difference from the serial fallback and is not covered by the accepted normal-path equivalence results. It should remain explicitly qualified until exercised.

## Focused test needed

Add a deterministic, source-owned test seam in a future authorized test change that makes the post helper fail or omit a result after at least one successful request, while the coordinator has one next conversion pending. Assert:

- the coordinator returns failure and does not print section summaries or replay buffered successful case rows as a completed run;
- the test records that no more than the one already in-flight successor converter call occurred after the failing comparison;
- both coordinator stdout/stderr CRT descriptors and Win32 standard handles are restored after unwind;
- the owned helper process is terminated/reaped before the IPC/scratch root is eligible for cleanup;
- the failure leaves enough IPC state to distinguish completed, pending and uncertain case work, and never reports a successful complete summary.

Do not use PID races or external mailbox edits to induce the condition. The prior isolated failure plan explicitly identifies those as unsafe and says a deterministic source seam is absent. Until such a seam exists and the test runs, parent behavior under helper crash, missing result, timeout, partial replay and cleanup is unverified. The isolated malformed-manifest test validates child input rejection only.

## Cleanup comparison

On the normal path both implementations remove the IPC root after helper validation and replay, then remove the sweep scratch root after `runPipeline` returns. `FolderChild` owns and reaps its process handle during stack unwinding; the stdout/stderr capture objects restore CRT descriptors and corresponding Win32 handles in their destructors. These are source-level cleanup paths, not runtime proof for helper failure. If `runPipeline` throws, the outer `fs::remove_all(scratch, ec)` is bypassed, intentionally retaining the run scratch; whether IPC artifacts are complete/useful after each failure point is not established. A focused fault test should verify restoration/reaping and characterize retained artifacts rather than assume cleanup succeeds.

The accepted isolated design has the same broad unwind/retention limitation. No additional concrete failure-path regression from the checkpoint 46 minimal adaptation was found in this static review.
