# CP201 owned suspended launcher source review

Decision: **PASS for the bounded single-owned-process launcher, subject to the stated runtime gates and limitations.** The prior watchdog-provenance blocker R1 is resolved in the reviewed revision. This is a source review, not authorization or evidence of an original run. No launcher, test, original, reconstructed executable, build, REA session or GUI was run for either review. Only this report was written.

## Reviewed revision

SHA-256, read directly from the working files during re-review:

| File | SHA-256 |
|---|---|
| `admin/owned_suspended_process.py` | `305d1036dc65ede44485aed49714f07290d65aaadbf1dbac11b3867a8e0c31e0` |
| `admin/tests/test_owned_suspended_process.py` | `d4e03e068cfb53f3e21dbd33ba4e03ad8fa873c418cc8a91253fa19a0692dfe3` |
| `analysis/resume201-owned-launcher-implementation.md` | `39650b50504aafa3e4a6b63572d0e0f0fae6f27aed8daa5f3106c334ac5ea8ab` |

Also compared the contract and outstanding gates in `analysis/resume201-passive-oracle-probe-plan.md`.

## R1 correction verified in source

`OwnedProcess.termination_attempts` is now held under the same lifecycle RLock. Every actual TerminateProcess invocation records its source and requested code, API success/error, then wait and exit results when those operations complete. The watchdog supplies `watchdog_deadline`. A final close after watchdog exit retains the earlier attempt and computes cumulative `termination_requested=true`. Natural exit without any invocation returns false and an empty attempt list. A failed request racing natural exit remains recorded separately from verified exit. This resolves the original incorrect false final receipt.

The added eighth test delays only the controller main thread after its created receipt while the real watchdog acts. It opens an independent query/synchronization handle, compares creation FILETIME before accepting that handle's observation, verifies exit and code 0xE201, then requires the final JSON to retain exactly one successful watchdog attempt. It checks that subsequent OpenProcess fails with error 87 after handles close; an openable/reused PID fails conservatively and is never killed. This exercises the original receipt failure, while the natural-exit test requires false plus an empty attempt list. This review did not independently execute the implementation report's eight passing tests.

## Findings that pass

- Explicit canonical application name, mutable Windows-quoted command line, absolute cwd and CREATE_SUSPENDED are supplied at creation. No shell is inserted. CREATE_NO_WINDOW suppresses a console allocation for console targets; it does not suppress GUI application windows.
- STARTUPINFO/PROCESS_INFORMATION use pointer-sized handle and pointer fields; kernel32 argtypes/restypes match the calls, including DWORD suspend sentinel and wait results. WinDLL captures last error. Failed TerminateProcess now saves its error before making another Win32 call.
- PID/TID, creation FILETIME and QueryFullProcessImageNameW derive from the freshly created process and its retained handle. Lifecycle calls never reopen a PID, adopt an enumerated process, use an image-name kill or traverse descendants. PID reuse cannot redirect the retained handle.
- Resume uses the held primary-thread handle exactly once and requires previous suspend count 1. A second resume or unexpected count fails into cleanup. Reported suspended/running state is the launcher's primary-thread lifecycle view, not proof about every thread an instrumentation system might introduce.
- Exit proof uses a signaled held process handle and exit-code retrieval, rather than interpreting STILL_ACTIVE alone. Termination waits on that same handle. The RLock serializes watchdog/finalizer access and handle closure. Errors cannot produce `handles_closed=true` or verified absence.
- Input line and queue sizes are bounded; EOF, read errors, malformed JSON, unsupported objects/commands and duplicate resume lead to controlled cleanup. Output escaping preserves JSONL framing. Lifecycle locks are released before stdout writes, allowing the watchdog to act when the consumer stalls.
- The source test scope remains newly owned harmless no-window Python children in unique converter scratch paths. It contains no original/reconstructed application launch, GUI input, broad termination or installed-file changes.

## Remaining qualifications and concrete nonblocking corrections

1. Duplicate JSON keys collapse under json.loads. `{"command":"status","command":"resume"}` therefore resumes. Reject duplicate keys with an object-pairs hook if textual single-command framing is required, and add a regression. This is protocol hardening, not an exact-process ownership escape.
2. Structured attempts are emitted on successful cleanup. If watchdog error or final cleanup failure causes `cleanup_error`, that event currently contains only an exception string and `exit_verified=false`; the cumulative attempt list is not emitted. For fuller forensic receipts, include a locked snapshot of attempts and any known exit/handle-close outcomes in cleanup_error. The implementation report's statement that each invocation retains wait/verified-exit fields should say those fields are conditional on reaching those operations. These paths already fail conservatively and do not claim successful cleanup, so this does not recreate R1's false successful receipt.
3. The watchdog is armed after construction and identity collection. The 20-second deadline is not a hard guarantee if initialization/kernel/filesystem work stalls. A tiny accepted deadline can also yield an already-exited created event; qualify the report's unconditional first suspended-state wording.
4. The digest is a precreation disk read. Canonical path equality does not remove replacement/reparse races or prove loaded-memory bytes. cwd is the creation request; the harmless-child test qualifies effective cwd only for that test. Windows CRT-style list2cmdline quoting does not prove compatibility with arbitrary custom command-line parsers.
5. Abrupt launcher death remains uncontained; no kill-on-close job object or descendant containment exists. A stalled stdout consumer can keep the launcher alive after the watchdog ends the child. API failures may prevent termination. Failed cleanup still closes handles and therefore may sacrifice retry capability, but returns error rather than verified absence. These limitations are largely explicit in the implementation report and must remain explicit.
6. The regression's 0.5-second deadline/1-second controller delay is a practical scheduling margin, not a formal deterministic scheduler guarantee. It checks identity before accepting observations and fails conservatively if the handle cannot be acquired in time. Its local OpenProcess absence check does not establish REA-device enumeration absence.

## Plan comparison

The helper satisfies the plan's alternative explicit-cwd, CREATE_SUSPENDED, creation-time held-process/primary-thread-handle architecture. This PASS does not establish current original/module identity, REA host/PID visibility, passive pre-resume hook readiness, bounded complete hook delivery, independent same-device absence, Frida session closure or separate original-run authorization. These gates remain before an original probe.

No accepted application Open, Save or Close is established by process termination or a signaled handle. No GUI-control acquisition/release occurred. No reconstructed DEF_PRE instance is identified and no deployed-version claim is made.

Win32 contract references: [CreateProcessW](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw), [process creation flags](https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags), and [process termination and signaled process handles](https://learn.microsoft.com/en-us/windows/win32/procthread/terminating-a-process).
