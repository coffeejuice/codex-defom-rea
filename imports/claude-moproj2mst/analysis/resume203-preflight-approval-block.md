# CP203 native exception-handler preflight approval block

The guarded no-Interceptor original crash-stack probe passed independent source review at runner SHA-256 `381a456c649c104c1fffaa25e2f9c472951188121181200272478e0b60270ec2`. Its execution guard remains false. A separate harmless preflight was prepared to validate Frida `Process.setExceptionHandler` against a newly owned Python child that intentionally raises an access violation, with exact retained-handle cleanup and independent absence checks.

The standard sandbox could not read the installed Frida package (`EPERM`) before child creation. The preflight worker's exact elevated command was rejected by automatic review with the stated reason that intentionally launching and crashing a child process lacked user authorization in that subagent context and should not be bypassed. The worker made no indirect retry. No child, original application, GUI-control session, or Frida session was started by that preflight. `node --check` passed; the native exception-callback and cleanup behavior remain unverified. See `admin/exception_handler_owned_preflight.mjs` and `analysis/resume203-exception-handler-preflight.md`.

The coordinator has asked the user for explicit authorization for this bounded child-only test. Until a response or another approved validation route is available, the crash-stack original run remains disabled. Other source and static recovery work continues. This local approval block does not complete or pause the converter objective.

## Separate no-crash registration check

The independently reviewed `admin/exception_handler_no_crash_preflight.mjs` was run once with a newly owned Python sleeper. Frida 17.23.2 registered `Process.setExceptionHandler` before the single resume; the child raised no exception. The helper terminated the exact child through its retained handle, verified exit and closed handles, and independent Windows/Frida checks reported PID 40696 absent. The bounded report at `analysis/resume203-exception-handler-no-crash-preflight.md` has `captureComplete=true` and no failures. The script guard was returned to false after the run.

This verifies registration and cleanup only. It does not validate exception callback invocation, access-violation payload, backtrace, or forwarding behavior, and it does not resolve the automatic-review block on the deliberate child crash test or authorize the original crash-stack run.
