# Checkpoint 47: source-free pipeline fault-test plan

The checkpoint46 harness exposes real filesystem IPC and a dedicated post-worker entry point. Deterministic worker-side truncation and failure tests can run without invoking conversion. Parent-side result truncation and live-child cleanup are feasible without source changes if a Windows debugger launch provides a verified child-creation barrier; a file-polling injector alone is not a reliable acceptance test.

This is a proposal only. No executable, script, GUI, REA session or build was run and no source was edited. The only worker output is this report. Inspected source SHA-256: `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`.

## Existing seams and expected behavior

| Source seam | What can be tested | Qualification |
|---|---|---|
| `wmain` 840: `--folder-post-worker <job.bin>` | Feed exact valid/malformed private jobs and ready messages | Worker dispatch bypasses `testkit::run_all` and never calls conversion. No corpus is needed. |
| `getNumber` 344 / `getString` 374 / `requireEnd` 382 | Truncated lengths/payloads, oversized payloads, trailing bytes | Protocol corruption is expected to fail, not become an empty result. |
| `folderPostWorker` 447 | Invalid ordinal/operation, comparator failure, stderr receipt and exit 2 | Its catch prints `Folder post helper infrastructure failure: ...`. |
| `collect` 683 | Truncated/wrong-identity results and invalid terminal state/tally | A parent-owned `.done` file is read once it exists; no helper output is trusted without parsing. |
| `waitMailbox` 433 | Child exits before `.done` | Parent checks its exact child handle and throws promptly; the ten-minute timeout need not be exercised. |
| `FolderChild` 557 | Parent exception while helper is still alive | Destructor terminates with code 2, waits, and closes its process handle. External observation can establish exit/reaping, not directly prove every `CloseHandle`. |
| `FolderCrtCapture` 577 and `testkit::run_all` 77 | Restoration of stdout/stderr during exception unwinding | Post-exception `[FAIL] save_folder_sweep` and the exception diagnostic must appear in the launcher's outer streams, rather than only the coordinator capture files. |

There is no hidden fault-injection environment setting. Do not add one just for this test. There is also no supported automatic fallback/retry after conversion has begun: the intended result is a failing test process with retained diagnostic scratch.

## Proposed bounded script

A new, root-owned `admin/test_folder_pipeline_faults.py` could take:

```
--exe <absolute already-built m2m_s_save_folder.exe>
--scratch <new child of this checkout's build directory>
--environment-json <exact focused-test environment receipt>
--mode worker-only|parent-barrier|all
--timeout-seconds 20
```

Do not implement it in this worker lane: that file is outside this assignment. The script should require an existing explicit executable, record its hash and the bound source hash, allocate a new run directory, and refuse an existing directory. All protocol files, captured streams, temporary directories and copied test data belong below that directory. The environment receipt is needed only for parent tests and must come from the root's accepted focused configuration; the script must not guess fixture paths or launch an original DEFORM executable.

The driver launches only this test executable with `subprocess.Popen` or `CreateProcessW`, shell disabled, and a hidden console window. It never enumerates or kills unrelated applications. Each attempt gets its own scratch directory and timeout; no ten-minute waits are accepted. On a timeout, terminate only process handles obtained from that attempt, record `timeout`, and fail the attempt. Keep the run's files for diagnosis. Never count timeout or a missed injection as a pass. Suggested limits are 5 seconds per direct worker case and 20 seconds per parent case, with a 120-second suite deadline.

### Wire helpers

Numbers are native Windows x64 unsigned 64-bit little-endian values. Strings are length-framed bytes, not NUL-terminated strings. Paths/name/schema strings are UTF-8; the echoed manifest identity is arbitrary binary bytes.

The following Python is sufficient to construct all direct-worker cases; it is embedded guidance, not an executed script:

```python
import struct
from pathlib import Path

Q = lambda n: struct.pack('<Q', n)
def S(value):
    if isinstance(value, str):
        value = value.encode('utf-8')
    return Q(len(value)) + value

JOB = 'M2M_FOLDER_POST_JOB_MINIMAL_1'
READY = 'M2M_FOLDER_POST_READY_MINIMAL_1'
RESULT = 'M2M_FOLDER_POST_RESULT_MINIMAL_1'

def job(cases):
    data = S(JOB) + Q(len(cases))
    for ordinal, expected, work, name in cases:
        data += Q(ordinal) + S(str(expected)) + S(str(work)) + S(name)
    return data

def ready(ordinal, compare):
    return S(READY) + Q(ordinal) + Q(compare)

# Write job.bin and all test ready files before launching the direct worker.
# Launch [exe, '--folder-post-worker', str(job_path)] with shell=False.
# Capture launcher stdout/stderr separately and then read worker.stderr after exit.
```

The result reader must implement bounded length reads and exact EOF too. A successful compare result is `S(RESULT), S(job_bytes), Q(ordinal), Q(1), six Q tally fields, S(output)`. A no-compare result omits tally/output and ends after `Q(0)`. Tally field order is cases, identical, refused, files, banner, viewProfile. Assert exact job-byte identity, not a decoded/reencoded substitute.

### Deterministic direct-worker cases

Use a fresh attempt directory for every row. For cases needing paths, use an empty owned expected directory and an empty owned work directory. These tests invoke no converter.

| ID | Injected job/request | Expected evidence |
|---|---|---|
| W0 | Valid job with count 0 | Exit 0; empty worker.stderr. Establish executable/dispatch validity. |
| W1 | Job contains only 3 zero bytes | Exit 2; worker.stderr contains `Truncated folder IPC`; no `.done`. |
| W2 | Job is `Q(10)` plus 3 payload bytes | Exit 2; `Truncated folder IPC field`. |
| W3 | `S(JOB) + Q(100001)` | Exit 2; `Oversized folder post job`. |
| W4 | Valid one-case job and empty `1.ready` | Exit 2; `Truncated folder IPC`; no `1.done`. |
| W5 | Valid one-case job and `ready(2, 0)` at `1.ready` | Exit 2; `Post request identity mismatch`. |
| W6 | Valid one-case job and `ready(1, 2)` | Exit 2; `Invalid folder post operation`. |
| W7 | Valid one-case job and `ready(1, 0) + b'x'` | Exit 2; `Unexpected folder IPC tail`. |
| W8 | Valid one-case job and `ready(1, 0)` | Exit 0; exact no-compare `1.done`, empty worker.stderr, owned work directory retained. |
| W9 | Valid compare job with expected as an owned regular file, work as an owned directory; `ready(1, 1)` | Exit 2 with filesystem exception; no done result. The comparator's expected-directory enumeration fails before its work cleanup. |
| W10 | Owned expected/work directories each containing identical `a.txt`; `ready(1, 1)` | Exit 0; tally `(1,1,0,0,0,0)`; exact `SAME unit: 0 differ, 0 missing, 0 extra\r\n`; work directory removed; expected preserved. |

W9 is a comparator failure test, not evidence about converter failures. The helper's own stdout capture is text mode, so W10's result payload contains CRLF on Windows. Tests should compare bytes and retain stdout/stderr without normalizing away unexpected differences.

## Parent failure and cleanup: verified launch barrier

The production parent creates its job before `CreateProcessW`, then starts the post helper before opening stable coordinator captures. This is a natural launch boundary. A robust source-free injector can use Windows `DEBUG_PROCESS | CREATE_NO_WINDOW` on a newly owned headless test process and service events only for that process tree. This is OS debugger launch control, not REA or GUI app control; it would require explicit implementation/validation in the root's new test script. It must never attach to an existing process.

The proposed barrier algorithm is:

1. Launch the test parent with the exact focused environment plus a new isolated `M2M_TEST_SCRATCH`, TEMP/TMP/TMPDIR and `M2M_FOLDER_PIPELINE=1`. Retain the returned parent process/primary-thread handles. Record the executable hash before launch.
2. Service initial debug events; continue the normal initial breakpoint and loader events, close event file handles, and treat other exceptions as failures. Do not mask access violations with `DBG_CONTINUE`.
3. On the descendant's `CREATE_PROCESS_DEBUG_EVENT`, verify its executable image against the exact `--exe` path and verify that it is the sole child launched by this attempt. Retain the supplied child process/thread handles. Verify the parent-PID-derived IPC directory and job exist, the job schema is correct, at least one case is present, and there is no `1.ready`, `1.done`, `coordinator.stdout` or `coordinator.stderr`. A mismatch or missed barrier is `inconclusive`, followed by owned-process cleanup; no mutation is attempted.
4. Suspend the retained parent primary thread and the child primary thread, recording the successful suspend receipts. Continue the child's creation event. The child must remain alive and unable to run the worker. Use only returned handles, never process-name matching. If the gate cannot be established, abort this mode and retain worker-only results.
5. Inject the selected fault below, then resume the parent thread. Keep servicing debug events while waiting so its helper-exit event cannot deadlock the parent's `WaitForSingleObject`. The deliberately suspended child is either explicitly killed by P0 or left for the parent's destructor in P1/P2.
6. Drain the exact parent/child exit events, record exit codes and liveness, and close every driver-owned handle. A last-resort suite cleanup may terminate only these recorded handles. Receipt must distinguish a child killed by the injector from one killed by the parent. Do not label last-resort cleanup as successful parent cleanup.

Do not base a claimed pass on polling for `1.writing` or rewriting an already produced `1.done`: publication and collection can complete between polls. Keeping the child suspended before its worker begins makes the forged-result cases independent of comparator scheduling and avoids altering conversion inputs, arguments or call order.

| ID | Fault at barrier | Required result |
|---|---|---|
| P0 | Terminate owned child explicitly with distinctive exit code 113; resume parent | Parent fails with `helper exited before result` (or checked helper failure if its control point differs), exits nonzero, and observed child exit is 113. This tests child-failure propagation; it does not test parent-initiated child termination. |
| P1 | Leave child alive/suspended; create zero-byte `1.done`; resume parent | Parent collect throws `Truncated folder IPC`, parent returns nonzero, and its destructor causes child exit code 2 before parent exits. This exercises parent-reader truncation and live-child cleanup. |
| P2 | Leave child alive/suspended; create `1.done` with correct result schema and deliberately wrong manifest identity; resume parent | Parent fails with `Folder post result identity mismatch`; live child exits 2 due to parent destructor. Distinguishes schema framing from identity validation. |

The parent ordinarily queues at most two cases before collecting the first, so these attempts can perform up to two normal conversions before failing. The supplied fixture environment must already be accepted for scratch-only effects. Do not create malformed conversion projects or change converter flags to make these tests faster. Use a known successful first case or verify its outcome from retained coordinator output; a refusal still leads to result parsing, but an earlier conversion exception would miss the intended collect seam and must be marked inconclusive.

For P1/P2, require the normal `testkit` failure row on the launcher's outer stdout and an exception diagnostic on outer stderr. This confirms observable restoration after the stack unwinds. IPC/case scratch should remain available after failure; retained scratch is intended failure behavior here, not a cleanup defect. Record file paths/hashes before the script's eventual administrative cleanup. Do not require empty scratch or claim kernel handle-close coverage from process exit alone.

## Receipt and scope of claims

Each receipt should contain test ID, attempt UUID, source/executable hashes, environment digest, launch command, parent/child PIDs and creation timestamps, injection gate verification, injected file hash/length, elapsed time, outer and worker stream hashes, status/exception evidence, parent/child exit codes, whether last-resort cleanup was used, scratch manifest, and pass/fail/inconclusive. No fixture manifests are weakened; parent attempts retain the root's ordinary pre/post fixture hash checks.

Direct-worker tests are ready to implement from the existing protocol and need no source modification. Parent-barrier tests are a bounded implementation proposal, not an already validated debugger workflow. If debugger launch is unavailable or the barrier receipts fail, ship the worker tests and report parent cleanup/truncation as untested; do not replace the barrier with a race and call it deterministic. These tests establish harness failure handling only, not scientific converter parity, global-state isolation, performance or physical resource-failure ordering.
