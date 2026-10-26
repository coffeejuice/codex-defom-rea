# CP203 original crash-stack probe preparation

## Disposition

Prepared for independent source review only. **No original process was launched,
no GUI input was sent, and no Frida session was created.** The executable probe
is `admin/original_crash_stack_probe.mjs`. Its `ORIGINAL_RUN_REVIEWED` constant
is `false`; it also requires the exact CLI argument
`--execute-reviewed-probe`. A run is therefore blocked before inspecting or
launching any target unless a later reviewed source change deliberately changes
the guard.

The probe fixes the target to the verified B98 converter launch:

- Image: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`
- SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
- Working directory: `C:\PROBLEM\p202a`
- Arguments: `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`
- Project SHA-256: `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`
- Retained source and disposable fixture content manifest:
  `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`

These identities and the same executable hash are recorded in the previous
baseline crash evidence. That evidence establishes a repeatable access violation
at `QtCore4_64.dll` RVA `0x32c0`, while leaving the faulting instruction context
and call stack unknown. This probe is intended to obtain that missing context.
It cannot establish instrumentation-free behavior because Frida is present; its
exception callback deliberately returns `false` so the application's normal
exception path continues.

## Ownership and capture sequence

1. Before any child creation, the source requires Windows, the exact opt-in CLI
   argument, and the false review guard. Once separately authorized in source,
   it checks the fixed image, project and helper hashes, canonical image path,
   exact Frida package version `17.23.2`, and B98 source/copy manifests.
2. The existing `admin/owned_suspended_process.py` launches exactly one child
   with the fixed image, cwd and argument vector. It verifies the executable
   hash, retains process and primary-thread handles, and initially holds the
   child suspended. The Node runner validates the helper PID association,
   positive creation FILETIME and primary TID, image hash/path, cwd and exact
   argv before proceeding.
3. Direct Frida attaches only to that helper-created PID while it is suspended.
   The injected script installs only `Process.setExceptionHandler`; it declares
   no `Interceptor` usage, target API calls, replacements, or GUI input. A
   same-PID `handler-ready` message must arrive before the helper receives the
   single resume command.
4. The exception callback records the first `access-violation`: exception
   address and module, RIP, readable register context, and
   `Thread.backtrace(context, Backtracer.ACCURATE)` addresses with symbol and
   module-relative details where available. It emits the observation and
   returns `false`; it does not mark the exception handled. If no exception
   record arrives within five seconds, the runner asks the helper to terminate
   its exact owned child. The helper's hard watchdog remains 20 seconds.
5. Cleanup goes through the helper's retained process handle, waits for verified
   exit and closed handles, detaches Frida, then performs fresh Windows CIM and
   same-device Frida PID-presence checks. A successful capture requires both
   absence checks, unchanged before/after fixture content digest, and the helper's
   successful cleanup receipt. Timeouts, query failures, fixture changes,
   protocol errors and unresolved cleanup remain incomplete.
6. The run directory retains runtime identities, injected source, JSONL helper
   receipts after stdout drainage, helper stderr, first exception record, and deterministic before
   and after fixture manifests. The after manifest records whether the B98
   disposable directory changed.

This lane is headless and does not acquire or release GUI control. The separate
GUI-control release gate remains in force for GUI work. This probe does not
exercise GUI control.

## Source qualifications consulted

- `analysis/resume202-original-probe-attempt2-crash.md`: Windows Event 1000 and
  WER Event 1001 establish the reported exception/module/offset and identify
  that attempt's executable identity; they contain no stack or registers.
- `analysis/resume202-baseline-crash-evidence.md`: the no-Frida baseline reports
  the same image hash, B98 project command, cwd, fixture manifests, and matching
  QtCore exception offset. This supports reproduction without Frida under the
  recorded conditions, not proof of a specific root cause.
- `analysis/resume202-qtcore-crash-static.md`: RVA `0x32c0` maps to
  `QListData::size() const`; prior hook RVAs were elsewhere. Static symbol
  attribution does not identify the caller or failing memory operand.
- `analysis/resume203-original-runner-receipt-review.md`: the existing helper
  uses held-handle create/resume/terminate and watchdog semantics; previous
  source review approves helper integration only as preparation, not original
  execution authorization.
- `analysis/resume56-b98-scratch-qualification.md`: the converter B98 disposable
  fixture and runtime path are separately qualified; the converter's fixture
  is copied beneath `C:\PROBLEM\p202a` for the original comparison.

## Review checks and run gate

Before any original launch, independently review the exact runner and helper
hashes, the guard and CLI condition, fixed executable/project identity, helper
protocol, Frida 17.23.2 API semantics, first-AV record fields, callback's
`return false`, five-second window, 20-second helper watchdog, fixture manifest,
and all cleanup/absence requirements. Any source change invalidates that
review. This preparation does not authorize changing the guard or running the
original.

Requested harmless checks are limited to JavaScript syntax validation and a
negative-guard check using the exact opt-in argument, which must exit before
the helper or target is created. Do not execute the original during these
checks. No original run or GUI session is part of this deliverable.

## Preparation checks recorded

- `node --check admin/original_crash_stack_probe.mjs`: passed.
- An isolated Node VM mock of the injected handler passed: it installed the
  callback, emitted one first-AV record with a module-qualified RIP, 18 register
  fields and two accurate-backtrace frames, returned `false` for both the first
  and a subsequent exception, emitted no second AV record, and propagated a
  mocked registration exception instead of claiming readiness.
- Running the runner with the exact `--execute-reviewed-probe` argument failed
  closed at the review guard with `BLOCKED before any target launch`. That check
  did not create the helper, process, output directory, or Frida session.
- The checks above are source-only/mock evidence. They do not qualify Frida on
  the installed original process or authorize execution.
- Cleanup checks `Session.isDetached()` after the helper's held-handle exit. If
  the process exit already detached the Frida session, script unload is recorded
  as not applicable; it is not claimed as a successful unload. Otherwise the
  runner unloads the script and verifies session detachment.
- The [Frida JavaScript API documentation](https://frida.re/docs/javascript-api/)
  specifies the callback's `true`/non-`true`
  exception disposition but does not document a setter return value. Readiness
  is therefore the PID-matched message sent after the registration call returns
  without throwing. The callback returns `false` to forward the exception to
  the hosting application/OS.
