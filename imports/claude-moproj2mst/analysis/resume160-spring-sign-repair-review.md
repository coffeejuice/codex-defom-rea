# CP160 SPRING2 sign test repair review

## Result

No blocker found. The repaired test file matches its receipt SHA-256 `55C52C1F64672654FA00ED36C86BAADBE76020CECFBABE9246925D349056BF4B`; the source remains at CP160 SHA-256 `26CFABD97925362007D8FEC1EFBFFF6880C975E765B8060F123D3B51B014DE96`.

## New cases reach the intended gates

- The count-one `++1\r\n` case uses the same valid one-row primary as the surrounding lone-sign cases. It invokes the public native `readSprings` boundary, requires successful read and one returned row, asserts flag zero, and compares both input files byte-for-byte. In the released source, primary count one enables the sign/no-digit transition; the first `+` is consumed and the second remains unread when the count limit ends.
- The count-three `++2\r\n` case supplies three complete primary rows and exact companion bytes. It expects `M2M_E_UNSUPPORTED`, empty output, and unchanged primary/companion files. Since the reader derives `allowQQuestionCursor` from `count == 1 || count == 2`, this reaches the intended count-three exclusion before any sign consumption.

Both test cases create their own `Rig`, and the three-row case restores no shared state because scratch files are per-instance and removed by the destructor. The sign test's earlier loops leave no stale parser state: each `readSprings` call clears output and creates fresh local cursor/status projection state.

## Retained controls

The CP158 q/question retry test remains in the same test file and is unchanged: one-row `1q`/`1?`, two-row `1q`/`1?` plus unreachable-tail cases, q-leading retries, neighboring z/x refusals, and count-three refusal are still asserted. The CP160 tests also retain count-two sign pairs, repeated signs, tab continuation, comma exclusion, Unicode-sign BOM refusals, and byte preservation. The repair changed tests only; production source and public interfaces are unchanged.

## Limits

This read-only review verifies that the new cases reach the stated gates and assert the expected candidate result. It does not verify them by execution; root owns the build/full suite. No source/test edits, GUI control, or mutable REA action occurred in this review.
