# Resume 205 AV child source review

## Decision: HOLD

The reviewed files match the supplied SHA-256 pins:

- Converter: `imports/claude-moproj2mst/admin/exception_handler_owned_preflight.mjs` — `7d09633454a159c118de5bd3f7d309b0c0bca8f95cbeb3840cea5376761766a8`
- Helper: `imports/claude-moproj2mst/admin/owned_suspended_process.py` — `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`

This was static source review only. No child, converter, or GUI was run.

## What is adequately constrained

- The helper creates a new process with `CreateProcessW(..., CREATE_SUSPENDED | CREATE_NO_WINDOW, ...)`; it never opens a caller-supplied PID. It verifies the held process image and executable SHA-256, records PID, creation FILETIME, image, cwd, argv, host, and launcher PID, and keeps the process/thread handles for cleanup.
- The converter checks the created receipt against its own launcher PID, executable hash/path, cwd, and child code, attaches directly to that PID, loads the callback, and only then sends one `resume`. The helper permits one resume and requires the prior suspend count to be exactly one.
- The callback filters for the first `access-violation`, emits structured context, and returns `false`, so it does not claim the exception or suppress natural handling.
- Helper communication is JSONL with a 4096-character command-line bound and 64 KiB stdout bound. Cleanup receipts preserve helper output. The converter checks both Windows CIM and Frida process enumeration for PID absence and requires the helper's exact-handle natural-exit/closed-handle receipt for a pass.
- The overall timeout is bounded to 20 seconds in the helper. Failure paths request termination of the owned child and retain failure details.

## Blocking issues to fix before a one-run preflight

1. **Wrong report destination and overwrite risk.** The converter writes to `analysis/resume203-exception-handler-preflight.md`, while this review is for resume205. `writeFileSync(..., {flag:'w'})` silently truncates an existing report. Change `REPORT` to the specifically approved resume205 output path and use exclusive creation (`flag: 'wx'`) or an equivalently explicit collision check. Do not overwrite prior evidence.
2. **Strict-absence checks are PID-only.** The helper records `creation_filetime`, but the converter does not validate it against the launched process or include it in the Windows absence receipt. A reused PID could make a new unrelated process look like the child, or make absence conclusions ambiguous. Preserve the creation identity in the converter's owned-child record and make the Windows check distinguish the owned process instance; treat inability to establish that distinction as incomplete. Frida enumeration can remain a PID check but should be labeled as such.
3. **Failure/cleanup ambiguity needs explicit status.** `cleanupVerified` requires zero termination attempts, which is appropriate for proving natural exit, but the report's overall status is only `INCOMPLETE`; make the receipt explicitly distinguish natural-exit pass from cleanup-by-termination/incomplete. This prevents a bounded safety cleanup from being mistaken for a natural exception-handler result.

## Review by requested property

- **Exact owned-child identity/cleanup:** Mostly strong. Exact process handles stay in the helper; creation FILETIME is captured but discarded by the caller, which weakens independent identity/absence evidence. Cleanup checks handle closure, wait result, zero termination attempts, and helper exit code.
- **JSONL:** Bounded and line-oriented; malformed JSON and oversized output are failures. The final unterminated stdout fragment is retained but is not parsed as a receipt, which correctly prevents it from satisfying cleanup evidence.
- **Native callback:** Registration occurs before resume; architecture/platform and registration completion are checked. The callback returns `false`. The callback's error event is captured as an exception event but cannot qualify as an AV.
- **First AV evidence:** Captures exception address, RIP/instruction, registers, accurate backtrace, memory operation, PID, and disposition. Qualification is intentionally strict (module attribution, at least ten registers, nonempty backtrace). This may produce an incomplete capture on limited symbols/backtrace data, which is acceptable if reported as incomplete rather than pass.
- **Strict absence:** Requires both Windows and Frida PID views to show absence, plus helper-owned natural exit and closed handles. Add process-instance identity to the Windows evidence as above.
- **Output retention:** Raw helper receipts and messages are embedded in a JSON result, but the fixed `resume203` path and truncating write make the current output unsafe for the requested run.
- **One-run safety:** The child is fixed to a new Python interpreter running a deliberate null function-pointer call, with no converter or GUI target. The wrong, truncating report path is the blocking one-run issue; fix it before execution.

## Exact disposition

**HOLD.** Do not run this pinned converter unchanged. Make the three changes above, review the resulting diff and new hashes, then perform any authorized preflight as a single controlled run. No execution was performed for this review.
