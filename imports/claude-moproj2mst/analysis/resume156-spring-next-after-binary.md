# CP156 follow-up: explicit-positive decimal wrap beyond INT64_MAX

## Recommended next family

After the complete unsigned binary family, the next large reachable numeric gap is count-one explicit-positive decimal magnitudes greater than `INT64_MAX`. Use canonical ASCII tokens `+[1-9][0-9]*` followed by ASCII whitespace or EOF. The candidate's CP152 explicit-plus branch currently checks the magnitude against `INT64_MAX` and refuses larger tokens, while the original Qt decimal accumulator wraps in 64 bits and the positive-sign completion path does not negate. `QTextStream >> int` then stores the low dword. Consequently the visible result is the signed interpretation of the token's value modulo `2^32`, even after the intermediate accumulator has wrapped modulo `2^64`.

The first discriminator is `+9223372036854775808` -> flag `0`; a useful full-64-wrap pair is `+18446744073709551616` -> `0` and `+18446744073709551617` -> `1`. `+36893488147419103233` (2*2^64+1) also yields 1 and verifies repeat wraps. The nonzero cases reach the normal SPRING2 Save path and serialize as decimal `1`, so Open-to-Save output is observable.

This family does not include leading-zero signs such as `+010` (covered separately by CP153 in-range signed-leading-zero behavior), negative overflow (conditional sign handling differs), commas, or malformed suffixes.

## Original Qt machine-code path

Original `QtCore4_64.dll` SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The byte-matched capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` establishes the following:

- `getInteger` skips leading `QChar::isSpace` characters before sign parsing (`0x670c5280–52b9`).
- For a recognized positive sign, the locale-sign comparison enters the decimal path around `0x670c545f–54d0`; it sets base ten and consumes the sign before decimal digit extraction at `0x670c54d5`.
- Decimal digits accumulate in 64-bit `RDI` with multiply/add `LEA`s at `0x670c56f6–5704`. The machine operations wrap naturally; the valid-digit loop has no arithmetic-overflow rejection.
- At completion the input sign is compared with the active negative sign at `0x670c57a5–57b5`. The signed-positive `test/jle/neg` at `0x670c57bb–57c7` runs only when the sign was negative. A positive sign skips that transform and returns the accumulated 64-bit value, including wrapped values.
- `QTextStream::operator>>(int&)` stores the low dword on successful getter return (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670c6166–616a`).

The existing corrected table in `analysis/resume83-sw2sp-parser-independent-review.md` explicitly confirms positive decimal `18446744073709551616` -> flag 0 and `18446744073709551617` -> flag 1 for the no-sign route. The `+` route reaches the same decimal digit loop and the same no-negation completion path; the prefix does not alter the low-dword relation. This is a machine-code composition, not a fresh original-process differential.

The reached SPRING2 caller `FUN_1410da690` extracts a single int for a one-row primary file, stores it as the record flag, ignores stream status, and returns success after companion open (`analysis/wave2-save-diffs-300-support-1410da690.c`). The retained writer scans for nonzero flags and serializes them as decimal (`analysis/resume120-spring2-writer-order.md`). Thus `+18446744073709551617` becomes flag 1 and Save writes companion value `1`.

## Candidate difference and test proposal

Current frozen post-binary reader SHA-256: `0870C34A0C18DCBA3A2026BA562AC6A2E3C8AE575A100BEE67A3919B6396745D`. Focused reader test SHA-256: `5779941FCFE83FBA867E3655E8D98021203DD29BE1CCEEDAC23FE72860FF768F`.

`readSpringFlagInt` already has a count-one explicit-plus decimal route for canonical non-leading-zero values through `INT64_MAX`, but its checked magnitude accumulator throws above that limit. A source-ready successor can replace that magnitude cap with direct `uint32_t` decimal accumulation for this same explicit-plus token class. Since unsigned modulo-32 arithmetic matches the low dword of Qt's wrapping 64-bit accumulator, this handles arbitrarily long finite ASCII digit runs without signed arithmetic. Keep the `allowSingleRowQualifiedToken` gate and the no-leading-zero rule.

Suggested tests use the existing complete one-row ASCII `SPRING.INI` fixture:

| `SPRING2.INI` bytes | Expected flag | Purpose |
|---|---:|---|
| `+9223372036854775808\r\n` | 0 | First magnitude above signed 64-bit max |
| `+18446744073709551616` | 0 | 2^64 wrap with EOF terminator |
| `+18446744073709551617\r\n` | 1 | Low dword after one 64-bit wrap and public Save effect |
| `+36893488147419103233\t` | 1 | Multiple 64-bit wraps with tab delimiter |
| `+4294967297\r\n` | 1 | Existing CP152 behavior remains unchanged |
| `+18446744073709551617x\r\n` | remain guarded | No malformed suffix admission |
| `+010\r\n` | existing CP153 result 10 | Leading-zero signed decimal remains separate |
| count-two `+18446744073709551617\r\n` | guarded | Preserve the one-row gate |

Check both primary and companion source bytes remain unchanged on accepted cases. Do not extend the negative branch: when the negative magnitude accumulator wraps into signed-negative 64-bit space, Qt's conditional `jle` skips negation, so mathematical low-dword negation is no longer the right model. Keep sign-high-bit, `-INT64_MIN`/beyond, locale signs, malformed delimiters, non-ASCII/codecs, BOMs, other files, and multi-value retry/status outside this proposal.

## Limits

The recommendation closes positive-sign arithmetic, completion and caller-visible value for the canonical, complete ASCII class, while preserving count-one and codec boundaries. It does not claim all Qt locale signs/digits or malformed-stream status parity. No source, test, build, GUI, or mutable REA state changed for this read-only plan. The prior `analysis/resume156-spring-next.md` remains the evidence contract for the now-implemented binary family; this follow-up preserves that receipt by using a distinct filename.
