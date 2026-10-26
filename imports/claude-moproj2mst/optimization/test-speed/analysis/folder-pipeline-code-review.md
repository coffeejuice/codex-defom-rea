# Folder pipeline implementation review

The current opt-in implementation keeps conversion in coordinator order and bounds outstanding cases at two. Static review found and reported a capture-sharing problem; the implementing worker corrected both capture open paths before this report. No additional deterministic ordering/IPC defect was established by this read-only pass. The pipeline remains experimental because raw generated-file parity and changed scratch lifetime have not been established by source inspection.

Reviewed file: `optimization/test-speed/source/apps/moproj2mst/tests/s/test_save_folder.cpp`. Final observed SHA-256: `ADD16C67075856E1321D85A1C835D72B0FC5CBAC59EF67D052575D945E0692E6`. Line references below refer to this revision. No build, test, fixture write or original application was run. Only this review document was written.

## Capture finding and correction

The first reviewed revision (`0E4CDEB43FF522DC19D575C02A3270424605A002DBB1DAB1C6622C775A675452`) read the helper's stdout capture while its secure `freopen` write stream remained open. The existing sharder already documented and worked around its exclusive sharing. This was reported immediately to root. The coordinator also opened its live cumulative capture with secure `fopen` while reading it through a separate stream, exposing the same sharing concern.

The corrected helper flushes, redirects stdout to NUL, updates the Win32 handle, then performs the strict read at lines 606–611. The coordinator now explicitly permits read sharing with `_wfsopen(..., _SH_DENYNO)` at line 767. `_dup2` retains a separate descriptor to that open file before `fclose(capture)`, so the converter-visible descriptor remains live across cases. `next()` flushes before reading and tracks the consumed byte count. These changes resolve the identified static sharing/lifetime issue; they were not runtime-tested by this reviewer.

`FolderCrtCapture` saves the original descriptor and flushes before redirection. Its destructor attempts restoration on exception. On successful `_dup2` restoration, the saved descriptor is closed. The outer owned-child object outlives captures, so exception unwinding restores captures before terminating/reaping the helper. Scratch root removal happens later in the caller on success. This is a coherent ownership order.

One remaining actionable robustness gap is that `SetStdHandle` return values are ignored at lines 778 and 799, and helper lines 602/610. `restore()` can return true after CRT restoration even if Win32 handle restoration failed, potentially leaving native writes directed to a stale handle. Check these results and include the restoration result in `restore()`'s boolean. Also preserve the originally observed Win32 destination if the contract includes callers whose Win32 standard handle differs from their CRT descriptor. This is a failure-path gap, not evidence that normal CTest handles currently differ.

## Conversion, case state and IPC

`runPipeline` calls `runCase` synchronously for each discovered case at lines 885–886 in this revision's neighborhood. The helper has a separate `--folder-post-worker` dispatch and invokes `compareCase`, not conversion. The opt-in setting is restricted to `0`/`1`, and conversion sharding cannot be combined with it. Ordinary serial execution remains the default.

Successful conversion publishes an ordinal request after returning, and the coordinator records its own converter output before assigning the terminal Compared state. Copy skips and refusals publish no-comparison requests; their coordinator tallies and messages remain intact. Compared cases receive the helper's tally exactly once. No second conversion call was found on the helper route.

Mailbox messages are closed and renamed before becoming visible. Result readers validate schema, manifest identity, ordinal, comparison terminal state and end-of-message; count/range validation is reused. A malformed result fails before final replay/summary aggregation. All results and successful child exit plus empty child stderr are checked before ordered event replay. An early child exit is checked while waiting for absent results. On exception, the owned-child destructor terminates/reaps its helper and retains IPC artifacts.

The queue waits on the oldest outstanding case when it already contains two entries. It therefore limits uncollected cases, while the single helper processes ordinal requests serially. Completed mailbox files remain until IPC-root cleanup, which is bounded by this sweep's case count; the implementation does not use an unbounded output pipe susceptible to a request/result pipe deadlock.

Manifest construction stores paths supplied by the trusted coordinator. The post helper does not independently re-check leaf containment. This is consistent with the private helper boundary, but it should not be described as validating arbitrary externally supplied manifests. No adversarial-manifest claim was tested here.

## Output and failure behavior

Coordinator converter stdout is captured per stream into stable cumulative files, then sliced after each case. Comparator stdout is appended to that same case's output only after the corresponding result validates. Event replay preserves manifest order and uses binary mode for case bytes to avoid repeated CRLF translation. Summary/notice positions remain in the original event stream.

Coordinator stderr causes an immediate harness failure rather than an attempted combined-stream replay. Nonempty helper stderr also fails the experiment. This conservative rule avoids claiming cross-stream byte-order preservation for successful runs with stderr; such runs are currently rejected. Native runtimes retaining private handles or delayed buffers are still an empirical capture gate.

Strict transport reads reject unreadable/truncated captures, while `compareCase` retains the historical permissive `readAll`. The extraction preserves comparison decision ordering and diagnostic truncation in the reviewed source. No implicit change to comparator read-failure semantics was found.

`next()` re-reads the cumulative capture each time and applies the transport's 64 MiB bound to the entire capture. This can add growing I/O overhead and reject unusually large aggregate stdout even when each case is small. It is not an established current-corpus failure, but should be included when measuring or documenting the transport limit.

## Remaining acceptance gates

The implementation deliberately overlaps comparison/cleanup of a previous case with later conversion. Old case paths remain visible longer, and comparator allocations no longer occur in the converter process. Full discovery also changes heap/preflight timing. These are acknowledged experimental changes, not eliminated risks.

Compared leaves are removed by the helper; refused leaves are removed synchronously by the coordinator; partial-copy failures remain until final root cleanup, matching the historical serial behavior. Removal errors remain ignored. Receipt or state names must not imply that every removal succeeded.

The result protocol transports tallies and truncated human diagnostics, not complete per-file decision vectors or raw generated inventories. Apart from the targeted B:107 capture, generated trees are deleted during comparison. Thus matching displayed rows cannot establish the design's raw whole-output parity gate. Add or separately run complete inventories before cleanup, preserving the original relocation target, before accepting this optimization. Repeated same-path raw parity, copy/refusal/IPC/crash/timeout failure tests, and realistic capture/restore validation are still required from the execution lane.
