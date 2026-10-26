# CP160 SPRING2 sign test repair

Closed the two gaps from the independent source review in the owned test file only:

- Added an explicit one-row `++1\r\n` accepted case asserting one row with flag zero and byte-for-byte preservation of both files.
- Added a three-row primary fixture with `++2\r\n` in SPRING2; the candidate refuses, returns no rows, and preserves both files. This checks that the sign-category profile does not widen beyond one/two rows.

No production source was changed. The test still uses the existing Testkit `CHECK`/`REQUIRE` macros and scratch-only fixtures. `git diff --check` passed. No build, test, GUI, or mutable REA operation was run.

Updated test SHA-256: `55C52C1F64672654FA00ED36C86BAADBE76020CECFBABE9246925D349056BF4B`.

Test source is released to the root-owned next integration step.
