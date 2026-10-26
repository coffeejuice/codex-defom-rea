# CP202 helper race preflight

Scope: helper-only cleanup repair and harmless Python-child validation. No original executable, GUI, or app-control input was used.

## Diagnosis

The prior helper called `TerminateProcess` on its retained `CreateProcessW` handle, then immediately sampled status. If the process was in the narrow transition to natural exit, the termination request could fail with `ERROR_ACCESS_DENIED` while the zero-time status sample still reported `WAIT_TIMEOUT`. The helper raised at that point and closed its handles in `finally`, leaving no held-handle exit receipt. Later PID absence cannot repair that missing proof.

## Change

After a failed termination API call, the helper now records the error and waits up to five seconds on the same retained process handle. It accepts the race only when that wait returns `WAIT_OBJECT_0`; otherwise it raises the original Win32 error. The ordinary final wait and `GetExitCodeProcess` verification still run, and the receipt retains the failed request, error code, race wait result, and final exit code. There is no PID lookup or PID-based termination path.

## Validation

- Added a deterministic fake-kernel test where the initial held-handle probe is `WAIT_TIMEOUT`, `TerminateProcess` fails with error 5, and the subsequent held-handle wait is signaled. It checks the complete receipt and wait sequence.
- Ran the full helper lifecycle integration suite on Windows. All nine tests passed; tests launch only owned no-window Python children in scratch.
- The existing tests cover resume/terminate, EOF cleanup, malformed input, deadline/watchdog cleanup, natural exit, and handle closure. The race-specific receipt test uses a fake kernel to exercise the otherwise narrow ordering deterministically.

## Limits

This establishes the helper's handling of a simulated natural-exit race and the normal lifecycle with disposable Python children. It does not reconstruct the exact timing of the earlier target's termination failure, and it does not justify any claim about that target's held-handle exit state.
