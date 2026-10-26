# Resume 205 AV child source re-review

## Decision: HOLD

Static-only review of `admin/exception_handler_owned_preflight.mjs` (SHA-256 `0584b7cc26bacbc78265f0d7a2168fd4a98610104bc0a09912ffd2de072924dc`) and its helper. No process or GUI was run.

The repaired source now writes the resume205 report path with exclusive-create (`wx`), waits for the helper's `close` event, retains Windows process creation FILETIME in its presence receipt, and separately records natural AV exit versus cleanup by termination. The pass predicate requires the helper receipt to report exit code `0xC0000005`, zero termination attempts, closed handles, successful helper exit, qualified AV callback evidence, both process views absent, and a detached session. These checks prevent termination cleanup from being reported as a pass.

### Remaining concrete issue: event-to-termination race

The callback sends `first-access-violation` before returning `false`. On receipt, the converter resolves `capture`, immediately continues to `finally`, and sends the helper a `terminate` command. The helper can consume that command while the child is still executing the callback / returning the unhandled exception. In that ordering, its `close()` calls `TerminateProcess`, so the natural `0xC0000005` exit is replaced by the cleanup exit code and the result becomes `INCOMPLETE_CLEANED_BY_TERMINATION`.

This does not create a false PASS, but it makes the intended natural-exit preflight race-prone and could invalidate a valid AV observation. After callback observation, wait for the helper's natural-exit receipt (with a short bounded wait) before requesting termination; if the wait expires, retain the termination cleanup and incomplete status. Alternatively, only send terminate on an actual failure/timeout path, while allowing the helper to observe natural exit on the successful AV path.

### Disposition

**HOLD** pending the bounded natural-exit wait / cleanup ordering fix. The identified race is the only concrete blocker found in this re-review. The report-path, exclusive-retention, creation-time receipt, helper-close wait, and natural-exit-versus-termination distinctions are otherwise present.
