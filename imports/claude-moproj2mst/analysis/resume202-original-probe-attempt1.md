# CP202 original Save-route probe attempt 1

One reviewed owned original process was created suspended with the disposable
`C:\PROBLEM\p202a\Gear_blank.moproj`. Frida attached, and the loaded original
EXE path, image size and four selected 24-byte prefixes matched the retained
identity. Installing the first required visibility hook at EXE RVA `0x154a4`
failed with `unable to intercept function`. The runner stopped with
`endpoint=hook_error` before the helper sent ResumeThread. No original Open,
Save, dock event, or accepted Close was observed. Raw bounded receipts are in
`analysis/resume202-passive-oracle-run-2026-10-26T04-28-23-604Z/`.

The owned process was PID 33884. The helper used its retained handle to
terminate it, reporting `exit_verified=true`, wait result 0, exit code
`0xE201`, `handles_closed=true`, and helper exit 0. The runner's same-device
Frida listing found no PID. Its Windows CIM absence helper returned an empty
raw result and null parsed value, which that runner version interpreted as
absent. The actual false-negative in `processAbsenceVerified=false` was an
unset `sessionCleanupSettled`: that version treated Frida's `isDetached()`
method as a property. The hook failure independently kept
`captureComplete=false`. Subsequent
independent exact-PID `Get-Process`, elevated CIM and REA local process-list
checks found no PID 33884. The 27 disposable file content hashes still equal
the before manifest; no added or deleted files were found. This establishes
cleanup for this one attempt, not application Close.

The script's explicit execution guard was returned to false after the failed
attempt. The cleanup receipt was repaired to call `isDetached()` and require
an explicit successful Windows presence query. A static hook-site
investigation selected a new candidate for a later guarded run. The original
interception error alone does not establish
whether the cause is instruction relocation, page protection, target state,
or another Frida limitation.
