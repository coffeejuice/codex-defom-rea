# CP154 next SPRING2 numeric family: unsigned leading-zero decimal fallback

## Recommended family

The next larger count-one gap is a complete unsigned ASCII decimal token beginning with `08` or `09`: grammar `0[89][0-9]*`, followed by ASCII whitespace or EOF. Qt's base-zero parser selects decimal when the second digit is 8 or 9; it then accumulates the remaining decimal digits. Consequently `08` -> 8, `089` -> 89 and `090` -> 90. The current candidate handles only the exact one-row `09` token (plus its separately qualified count-two witness) and refuses `08` and longer 08/09-prefixed decimal tokens.

This is distinct from CP150's unsigned octal family `0[0-7]+`, and does not alter CP149's partial octal prefix cases `018`/`019`. The leading zero is a selector character here: after an 8 or 9, the integer's decimal value is determined by the suffix beginning with that second digit.

## Source evidence and public result

Original `QtCore4_64.dll` SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The byte-matched `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows base-zero selection peeking the second character, testing `QChar::isDigit`/`digitValue` at `0x670c5417–542d`, and choosing octal only when its value is 0–7 (`0x670c5431–5446`). The other branch sets base 10 at `0x670c5448`, ungets the probe character, then resumes the decimal extraction path. The corrected `analysis/resume83-sw2sp-parser-independent-review.md` explicitly confirms the consequence: `08` and `09` select decimal and parse as 8 and 9; `018` selects octal because its second digit is 1.

For the selected decimal route, the same capture shows digit accumulation in 64-bit `RDI` at `0x670c56f6–5704`, and `QTextStream >> int` stores the low dword at `0x670c6166–616a`. Therefore the family may safely use decimal modulo-2^32 accumulation for positive ASCII digits. That low-dword result matches the earlier CP148 source-qualified positive decimal family, now combined with a separate source-proved base selector. ASCII-only inputs avoid making claims about Qt's Unicode digit semantics or host codec behavior.

The original caller is `FUN_1410da690` in `analysis/wave2-save-diffs-300-support-1410da690.c`: a one-record primary file causes one companion integer extraction, copies its result to the row flag, ignores QTextStream status, and returns true after companion open. A nonzero flag reaches the retained Save branch; `FUN_1410db030` writes companion flags in decimal in row order (`analysis/resume120-spring2-writer-order.md`). Thus `08` loads as 8 and is later saved as `8`; a large member may narrow to a negative low-dword flag and be saved in its normalized signed decimal form. These are static predictions, not original runtime comparisons.

## Candidate gap and bounded proposal

Frozen reader candidate SHA-256: `E1BD637DBE55A875C9BBED604663ECACD3599647AF77DBADCF0D365479B4AD8E`. Focused test SHA-256: `4C946D622384E5BD30DC9599D17998D98287167BA3A87D35D6A78749F7502508`.

`readSpringFlagInt` currently has a count-one exact `09` branch, while the one-row unsupported list still contains `08`. The generic leading-zero guard rejects other zero-prefixed tokens. A bounded next implementation can route only the one-row, unsigned `0[89][0-9]*` complete-token family into a decimal low-dword accumulator. Keep the existing exact count-two `09` witness separate; do not open count-two `08` or generic count-two leading-zero parsing. No sign should enter this family because explicit-sign decimal follows a distinct selector path already handled by CP152/153.

Suggested one-row matrix, all with a valid complete primary `SPRING.INI` row:

| Companion bytes | Expected flag | What it distinguishes |
|---|---:|---|
| `08\r\n` | 8 | Missing smallest decimal-fallback case |
| `089\t` | 89 | A complete decimal suffix after second-digit selector |
| `090` | 90 | EOF terminator and longer decimal suffix |
| `09223372036854775807\r\n` | -1 | Decimal low-dword narrowing on this base-selection family |
| `080x\r\n` | guarded | Do not broaden malformed suffix handling |
| `0\r\n` | 0 | Existing single-zero case; outside this family |
| `+08\r\n` / `-08\r\n` | 8 / -8 | Existing CP153 signed-leading-zero path; outside this family |
| count-two `08\r\n` | guarded | Preserve the count-one gate |
| count-two `089\r\n` | guarded | Preserve multi-row input boundaries |

For accepted and refused cases, assert byte-for-byte preservation of both sidecars. Keep signs, malformed suffixes, BOM/non-ASCII decoding, locale-specific digits, other filenames, comma behavior and all unrelated retry/status profiles guarded. The one-row `09` witness should continue to return 9 unchanged, while exact count-two `09` retains its existing separately qualified behavior.

## Limits

No source, tests, build, GUI, or mutable REA state changed for this read-only analysis. There is no fresh original-process comparison. The proposed range is statically closed by the digit-selector, decimal accumulator and low-dword store; broad Qt Unicode digit handling, codec detection and multi-value status/cursor behavior remain unclaimed.
