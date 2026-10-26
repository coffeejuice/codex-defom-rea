# CP146 Rizin attempt: bounded updater disassembly

## Result

The single requested Rizin attempt was unavailable for this target; it returned a non-retryable provider error before producing command output. No Rizin disassembly or Evidence ID exists, so there is no new code edge to add. The prior +0x230 cell-writer and derived-callback unknowns remain open.

## Exact request and identity check

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`.

The pre-call SHA-256 was `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The only command submitted was `s 0x140d78220; pd 80`, a seek followed by 80 disassembly instructions. REA returned `capability_unavailable`, category `unsupported_provider`, non-retryable, with detail `provider_id=rizin`, `operation=execute_rizin_command`, `reason=Unable to run configured command (ERR_REA_WINDOWS_NATIVE)`. It returned no stdout/stderr capture, Evidence ID, or completion status for command execution.

The post-call SHA-256 remained `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, matching the pre-call identity. This confirms the artifact bytes did not change; it does not imply the requested disassembly ran.

## Remaining dependency

The next missing evidence remains a completed function-body capture for `0x140d78220` and its bounded calls to `0x140d78490` / `0x140d7a9f0`, plus the derived `SfObjectManager::slotItemChanged(QStandardItem*)` meta-call target/body. Since this one alternate method was unavailable, no retry or broader query was attempted. Keep the current populated-cell refusal.

No source, build, test, GUI, original process, mutable target state, or REA analysis session was opened or changed by this attempt.
