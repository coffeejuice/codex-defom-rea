# CP132 independent repair review: exact two-row SPRING2 gate

The repaired admission now matches the promised narrow fixture. `exactTwoRowBinaryRetryProfile` is true only when the primary count is two and the entire decoded ASCII companion is byte-for-byte `0B2\r\n`. `readSpringFlagInt` receives this false for the listed mixed-token inputs, leading/trailing whitespace variants, and all count-three cases, so none can enter the new repeated-failure branch. The call still passes the separate existing one-row gate only for a one-row primary at index zero; that older one-row profile is unchanged.

## Cursor and two attempts

For the exact companion, each call begins at offset zero: the special branch sets `cursor = start`, and no whitespace or other token was consumed. It returns zero for each requested slot. Thus both slots see the same restored prefix, as Qt's failed binary extraction does; the helper's count loop performs exactly two attempts and produces flags `[0, 0]`. The source-qualified Qt state/status evidence and caller loop support this as a static deduction. The input files are only read and remain unchanged.

## Regression and surrounding behavior

The updated tests retain the exact success case, assert both flags and unchanged file contents, and assert rejection plus empty output/file preservation for `1 0B2`, `0B2 1`, `0B2 0B2`, leading whitespace, trailing whitespace, and a count-three primary. The full-string gate is evaluated after `asciiBytes` rejects the companion UTF-8 BOM and non-ASCII payload profile; no extra BOM/codec spelling is admitted through this special case. Row parsing, default-zero flags, existing one-row token admissions, other companion parsing, and public integration are untouched.

The implementation receipt's narrowed wording now matches the code and tests. No discrepancy found. Validation remains bounded: source/test were reviewed statically, and this independent review ran no build, tests, GUI, original process, or mutable REA action. The predicted native result remains based on PE-verified Qt internals plus the static original helper loop, not an original-process differential.
