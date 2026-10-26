# CP153 next SPRING2 numeric family: signed leading-zero decimal

## Recommended bounded family

The next reachable gap is a count-one, explicitly signed decimal token with leading zeroes, limited initially to values representable as a 32-bit `int`: `+[0-9]{2,}` or `-[0-9]{2,}`, with ASCII whitespace or EOF after the digit run. The leading sign matters: Qt recognizes the sign before base-zero leading-zero selection and sends signed values through its decimal path. Thus `-010\r\n` is decimal `-10`, while the unsigned spelling `010\r\n` is octal `8` under CP150. The candidate currently rejects both signed leading-zero examples at its unconditional leading-zero guard.

The smallest paired examples are `-010\r\n` -> flag `-10` and `+010\t` -> flag `10`. These exercise a distinct base-selection path, not another overflow literal. Restricting this first increment to signed 32-bit values avoids reusing CP151/CP152 narrowing logic for zero-prefixed magnitudes and prevents any signed-64 arithmetic edge from entering scope.

## Original parser evidence

The governing `QtCore4_64.dll` hash is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the original application hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

In the byte-matched `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, the initial character scan calls `QChar::isSpace` and ends at the first nonspace before sign parsing (`0x670c5280–52b9`). The base-zero auto-detection branch for a leading `0` begins at `0x670c5350`; it is reached only when the first nonspace character is itself zero. The sign path compares the first character to the active positive/negative locale signs at `0x670c545f–54d0`; a recognized sign selects base 10 and consumes it at `0x670c54d5`. Decimal parsing then reads the following digit at `0x670c5614` and continues through digit accumulation at `0x670c56f6–5704`. Therefore a leading zero after a recognized sign is an ordinary decimal digit, not an octal selector.

The independent corrected table in `analysis/resume83-sw2sp-parser-independent-review.md` includes `-010` -> `-10`, successful getter, EOF cursor, and success status. Its static PE-byte audit ties the same getter/operator to the shipped Qt binary. The `QTextStream >> int` success path writes the parsed low dword at `0x670c6166–616a`; for the proposed int32 interval this is simply the ordinary signed decimal value.

The original SPRING2 reader call chain in `analysis/wave2-save-diffs-300-support-1410da690.c` obtains the integer flag through this Qt operator, stores it in the row, ignores stream status, and returns success after a readable companion opens. The unchanged Save path scans flags, writes SPRING2 when any is nonzero, and emits flags as decimal in row order (`analysis/resume120-spring2-writer-order.md`). Thus the input `+010` can produce the saved companion value `10`; the input's lexical sign/zero padding is normalized.

## Candidate difference and bounded tests

Frozen candidate reader SHA-256: `09D6FCD852F3D5F2C5942318816A10F734C796E5C107C3125C087A057EFA16B8`. Focused reader test SHA-256: `52F359644B6E12C7F538252A0A73062AE260420FA180813ED3F05ADA8ECE0541`.

The current `readSpringFlagInt` strips a leading `+` for `from_chars`, but before conversion it rejects any signed or unsigned token with more than one digit after its initial zero. The one-row refusal matrix explicitly includes `-010` and `+010`. A narrowly source-ready implementation would allow the existing int-range parser to proceed only when all of these hold: `allowSingleRowQualifiedToken`, an explicit leading `+` or `-`, at least two ASCII digits after the sign, a leading `0`, and a complete token ending at ASCII whitespace or EOF. It should then rely on `from_chars(int)` for range validation; if the conversion is out of range, keep the existing overflow guard. Do not relax the leading-zero rule for unsigned values or for other count/path routes.

Suggested focused matrix using the same complete one-row ASCII `SPRING.INI` fixture:

| `SPRING2.INI` bytes | Expected flag | Purpose |
|---|---:|---|
| `-010\r\n` | -10 | Negative sign forces decimal rather than octal |
| `+010\t` | 10 | Positive sign forces decimal; tab delimiter |
| ` -0012` | -12 | Leading ASCII whitespace plus signed decimal and EOF |
| `+000\r\n` | 0 | Signed all-zero decimal, if included in grammar |
| `010\r\n` | remains 8 | Unsigned CP150 octal behavior unchanged |
| `-010x\r\n` | guarded in this cut | Do not widen malformed suffix admission |
| `-02147483649\r\n` | guarded | Signed magnitude exceeds int32 and is not a CP151 canonical nonzero-leading family |
| count-two `-010\r\n` | guarded | Preserve count-one gate |

Check that accepted and rejected reads preserve both input files byte-for-byte. The original stream consumes leading ASCII whitespace and the sign, consumes all decimal digits, and stops before the whitespace delimiter; for the one-value caller, the unread delimiter does not trigger a second extraction. No stream-status change is required because all proposed conversions succeed.

## Limits

The bounded class does not claim a rule for signed leading-zero overflow, malformed suffixes, signed hexadecimal/binary prefixes, locale-specific sign characters, non-ASCII spaces or digits, BOMs, other filenames, or multi-row retry/status behavior. It relies on the existing ASCII codec gate and the current `SPRING2.INI` one-row callsite. Original runtime differential testing has not been performed; the contract is derived from static PE and call-chain evidence. No source, test, build, GUI, or mutable REA state changed for this report.
