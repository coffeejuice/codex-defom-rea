# CP158 SPRING2 q/question cursor repair review

## Result

No blocker found in the repair. The frozen reader/test hashes match `analysis/resume158-spring-cursor-repair.md`:

- Reader: `657685968B5296D519D87A7014735FF2BC76368C846FC25DEE9A4FEBA21DD7DA`
- Test: `A84E6C758688782352ADCFE5954B7EE038119411B513FD98CAAA9AC7181AF752`

## Verified cursor behavior

The implementation replaces the full-file whitelist with `allowQQuestionCursor = (count == 1 || count == 2)`. On a no-digit extraction, it checks the actual current byte for q or ?, returns zero, and leaves the cursor unchanged. After a successful numeric prefix, the suffix branch accepts q/? as the first unconsumed byte and assigns `cursor = digit`, preserving that marker for the next iteration. Later input remains unreachable while extraction retries at the same marker. This matches the repaired scope and the retained Qt behavior for both valid-prefix unget and invalid-first-byte failure/unget.

Tests now verify:

- one row: `1q`, `1?` -> `[1]`, and `q 2` -> `[0]`;
- two rows: `1q`, `1?`, `1q 2`, `1? 2`, `1??`, and `q 2` -> the planned `[1,0]` or `[0,0]` values;
- three-row `q 2` remains refused.

Every accepted and refused case checks the original SPRING and SPRING2 bytes are unchanged. Neighboring `1z`, `1z?`, and `1x?` remain unsupported. The suffix branch acts only after numeric conversion; no-digit tokens beginning at q/? use the separate zero/no-advance branch. Count-three calls do not enable either route.

## Preservation checks

The inspected source retains the CP157 SPRING2 BOM opt-in and decoder code. The focused test retains UTF-8/16/32 BOM cases, truncated/non-ASCII UTF-16 guards, and byte-preservation assertions. The negative-overflow `uint64_t` wrapping and conditional sign application remain in place, with their boundary-value tests and preservation checks. The repaired q/? branch does not alter those paths.

## Limits

Read-only source/test review only; no build or CTest was run. This verifies the bounded value/cursor model, not QTextStream sticky status bits or arbitrary malformed characters.
