# CP150 independent review — one-row unsigned SPRING2 octal family

## Result

No blocking correctness defect or guard leak found within the intentionally bounded `<= INT32_MAX` one-row profile. The implementation receipt is the repository-root `analysis/resume150-spring-octal-family-implementation.md`; its scope and hashes match the reviewed source/test. Reader SHA-256: `C4340EA223AB9B6D701EB770328E5C888C7F0609BD07FEE6F17BA5A762FC968C`. Test SHA-256: `55F724E24288ABF8CBDBEFE6FB83AEF2953442D0F4099FD1F3A651E4B85B4261`.

## Value, cursor, and range

The complete-token branch requires the already-qualified one-row flag, at least two bytes, initial `0`, and a second byte in `0..7`. It consumes only consecutive octal digits, accumulating `value = value * 8 + digit`. Before each operation it checks `value <= (INT32_MAX - digit) / 8`; this prevents overflow and admits values through `INT32_MAX`, while intentionally refusing larger valid octal tokens. It then accepts EOF or an ASCII whitespace terminator, sets the cursor to the first terminator (or EOF), clears comma state, and returns the accumulated value.

For the exercised inputs this gives `02` → 2 with CR unread at offset 2; `010` → 8 with CR unread at offset 3; `017` → 15 with CR unread at offset 3; and `010` at EOF → 8 with cursor at EOF. The test asserts returned flags and unchanged primary/companion bytes; cursor is not externally observable through this API, so that part is verified from the source rather than a test assertion. The retained Qt source supports base-zero selection and octal accumulation described by the CP150 proposal.

## Precedence and guards

Exact `018` and `019` handlers precede this branch. That preserves their unread-`8`/unread-`9` behavior and the existing two-row `018` retry (`[1,8]`). The complete-token branch is unavailable for count two; tests refuse count-two `010` and `010 1`, while retaining the separately tested exact `018` case. Signed tokens, leading whitespace, non-octal suffixes, and above-`INT32_MAX` octal values are still refused. Tests cover `+010`, `-010`, ` 010`, `010x`, and `037777777777` among the negative cases. The latter is a deliberate safe-range refusal, not a claim that original Qt rejects that syntactically valid octal value: the receipt explicitly excludes overflow behavior from this cut.

## Coverage note

The focused tests verify representative values, EOF, CRLF terminators, byte preservation, partial-token precedence, signed/leading-space/suffix/overflow refusal, and count-two isolation. They do not test the exact accepted `INT32_MAX` octal boundary or each other ASCII whitespace byte; the range formula and terminator check are direct and correct for those cases. These are nonblocking boundary-coverage opportunities, not observed implementation defects.

No build/CTest, source edits, GUI, original runtime, or mutable REA action was performed.
