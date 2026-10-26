# Exception-handler no-crash preflight re-review

Reviewed the repaired `admin/exception_handler_no_crash_preflight.mjs` and pinned `admin/owned_suspended_process.py` statically. No process or GUI was executed. The Windows argv comparison now handles the helper's `normcase` normalization by comparing the executable path case-insensitively and retaining exact, ordered checks for interpreter arguments. The prior review's concrete blocker is resolved.

## Review result: PASS for the guarded runner's static lifecycle gates

The reviewed source pins the helper digest; validates the full created identity and identity schema; rejects duplicate, malformed, unknown, or out-of-order helper JSONL receipts; routes injected script errors to handler-readiness rejection and failure capture; validates Windows PID presence rows; waits for stdout completion and rejects trailing partial data; and requires resume, the expected termination endpoint, the exact final-cleanup termination source, helper exit/handle closure, detach, and independent Windows plus Frida PID absence before PASS. The helper's current SHA-256 matches the configured pin: `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`.

`RUN_NATIVE` remains `false`. This review approves the static structure of the guarded no-crash registration/cleanup path; it does not authorize a native run. The injected callback returns `false` and the test induces no exception. Even a later successful run would establish registration and owned-child cleanup only, not callback invocation, access-violation payload behavior, or converter exception handling.

The source repairer reports `node --check` and `git diff --check` passed. I did not invoke the script, start a process, run a test, or use GUI control.
