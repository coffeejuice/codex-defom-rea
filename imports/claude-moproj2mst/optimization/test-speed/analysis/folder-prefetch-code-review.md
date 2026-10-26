# Folder copy-prefetch code review

No new blocking deterministic protocol defect was found in the reviewed opt-in prefetch implementation. It preserves coordinator conversion order, requests at most one future case beyond the current case, retains partial failed copies, and validates helper completion before replay. This is a static result, not acceptance of behavioral parity or a speed claim.

Reviewed `optimization/test-speed/source/apps/moproj2mst/tests/s/test_save_folder.cpp`, SHA-256 `8A27894EE6A289D844914BFAFF1984B20B922CF8A15EF44D3287827002176DBB`. Line references identify that revision. Review was read-only; no build, tests, fixture writes or application execution occurred. Only this report was written. Findings were sent to root before its build decision.

## Ordinal order and outstanding work

The coordinator builds the copy manifest from the same `child.events` order used by conversion and post-processing (lines 1243–1249). It primes only the first ordinal at line 1265. During each iteration it requests only that ordinal's immediate successor (1294–1297), then calls `runCase` for the current ordinal. `runCase` consumes the matching copy result before conversion (635), and the consumer verifies schema, exact manifest bytes and ordinal (850–851), metadata and message termination.

The persistent copy helper iterates its manifest serially (891 onward), waits for each corresponding request, verifies the manifest identity/ordinal and end-of-request, invokes the filesystem-only `copyCase`, then publishes that result before proceeding. It has no converter entry. Therefore ready results cannot reorder conversion calls, and no request beyond the single successor exists while a case converts. The current case's copy may still be pending when the successor request is published, but the helper finishes current first.

The existing two-case pending post-processing bound remains in effect. It is distinct from the single-future-copy bound. Old compared leaves can remain while the helper is reading/removing them; failed-copy leaves can accumulate until sweep cleanup. Thus “one future tree” does not mean only one or two case directories exist in total. No unsupported total disk-space bound should be inferred.

Manifest path validation calls the existing `scratchOutput` helper for each exact copy leaf. That resolves containment without creating every case leaf; it does not pre-copy or pre-create all future work directories.

## Copy success, failure and partial trees

`copyCase` at 480–501 extracts the historical remove/copy/profile/candidate-file check. Both serial and prefetched execution use it. It performs the pre-copy removal attempt, recursive copy, optional profiling census, and check for the selected `.moproj` filename. Its returned failure flag/message correspond to the error-code path. Ordinary copy failures remain counted skips rather than infrastructure failures.

The helper does not remove a partial tree after copy error (898–901). The coordinator receives that failure and writes any raw receipt, emits the original SKIP/profile output, and returns without conversion (638–643). A no-comparison post request follows, so the post helper neither compares nor cleans that leaf. Final whole-sweep cleanup retains its original role. This matches the previous failure-lifetime requirement.

Thrown filesystem/profile/transport exceptions remain helper infrastructure failures rather than synthetic copy skips. A copy result is closed and renamed before publication; consumer waits also notice helper exit. A malformed/mismatched result aborts instead of authorizing conversion. The normal protocol supplies the identity and filenames from one trusted manifest; this review does not claim it accepts arbitrary hostile manifests safely.

## Output and helper lifecycle

The copy helper redirects both stdout and stderr to its private files and binds the Win32 standard handles before work (866–873). Copy logic returns data rather than printing case rows. The coordinator prints SKIP diagnostics at the current case's existing ordered position using that result. After all case processing, it waits for successful copy-helper exit and strictly reads both diagnostic files; any unframed bytes fail the experiment (1319–1327). These reads occur after process exit, avoiding the prior live secure-stream sharing issue.

The post helper continues to capture comparator output privately. Converter stdout remains coordinator-captured; nonempty converter stderr still rejects the experiment. No copy helper is allowed to write coordinator descriptors. Replay happens only after both helpers' results, exit statuses and diagnostic checks have succeeded, so helper failures do not produce a complete successful sweep summary.

The copy child is an owned `FolderChild` object. If copy launch fails, the already-started post child still unwinds through its destructor. If conversion, IPC or capture fails later, local capture guards restore streams, reset guards clear prefetch globals, then owned copy/post children terminate/reap before control leaves `runPipeline`. On success both helpers are already exited before IPC-root removal. Caller work-root cleanup occurs after pipeline return. Partial uncertain artifacts remain on exception for diagnosis.

The shared destructor's ignored termination failure followed by infinite reap wait remains a preexisting failure-path limitation; this pass found no new prefetch-specific bypass of ownership or premature scratch deletion. The mailbox waits themselves have the existing ten-minute bounds.

## Configuration and raw identity

With sweep execution enabled, `M2M_FOLDER_PREFETCH` accepts only `0` or `1`; empty/other values fail (1457–1460). Enabled prefetch requires enabled pipeline (1461), and pipeline cannot combine with conversion sharding (1462). The default remains disabled. As before, the whole test returns before configuration validation if the sweep/corpus prerequisite is absent; that is not evidence that an invalid configuration was accepted for an executed sweep.

Both case receipts (300–301) and complete manifests (364–365) record a boolean prefetch field. `workerEnvironment` retains non-temp environment settings, so coordinator copy/refusal receipts and helper compared receipts inherit the same selector in the normal launch path. Existing mode remains `pipeline`; the separate flag distinguishes pipeline with and without prefetch. No helper mode silently enables raw recording by itself.

## Acceptance limits

Future leaf creation happens earlier, and copy allocations/I/O move out of the converter process. Neither disjoint ordinal naming nor serial converter calls proves that stale engine paths, process allocation history or filesystem timing are irrelevant. Require repeated same-path full raw inventory parity, full status/message and ordered diagnostic comparison, and the targeted sensitive-case captures before promoting the option.

Execution-lane validation should exercise partial copy failure, missing selected `.moproj`, copy-helper crash/timeout/malformed result, unframed stdout/stderr, zero eligible cases, and abort while the successor is being copied. Verify owned helper exit and artifact retention. This read-only review did not run those checks or infer their outcome.
