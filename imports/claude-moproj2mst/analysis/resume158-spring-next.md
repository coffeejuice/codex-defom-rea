# CP158 SPRING2 malformed ASCII prefix/ignore branch

## Recommended next cut

The next source-closed SPRING2 gap is the generic ASCII non-digit cursor path around an otherwise valid decimal prefix. It is distinct from extending numeric radix/value families: Qt's integer getter returns a parsed decimal prefix and ungets the first non-digit that is not consumed as the C-locale group separator; the companion helper ignores conversion status and stops after the requested row count. The current candidate models this only for narrow `x`/comma/uppercase-X cases and refuses other suffixes, so e.g. `1q` is a concrete mismatch.

Bound the first implementation to present ASCII `SPRING2.INI`, supported primary count 1 or 2, canonical signed decimal prefix already supported by the candidate, and the exact ASCII letters `q` and `?` as the first unconsumed invalid character. Do not admit comma (separate group-separator behavior), signs, base prefixes, whitespace, high bytes, or arbitrary malformed alphabets in this first cut. This is a small reusable extraction/cursor branch, not a numeric grammar expansion.

## Original extraction and caller evidence

Original executable `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume120-spring2-malformed-flags.md` derives the cursor/status behavior from the retained `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, and `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm`:

- Decimal digit accumulation stops at the first non-digit. If at least one digit was read and the character is not the C-locale grouping separator consumed by the numeric branch, Qt ungets that character and returns the parsed prefix (`getInteger` `0x670C5780–5790`).
- With no digits, the failed character is ungot and the getter fails; `operator>>(int&)` writes zero (`0x670C6125–6133`).
- `atEnd()` depends on buffered/device input, not conversion status; an ungot character keeps the loop live. The companion helper increments its extraction index, ignores stream status, and tests `!atEnd()` on the next iteration (`FUN_1410da690`, `analysis/resume119-spring2-normal-path.md`).

For ASCII `q` and `?`, neither is a digit, a C-locale sign, whitespace, or the comma grouping separator. Thus the bounded expected outcomes are:

| Primary record count | `SPRING2.INI` bytes | Original-static flags | Cursor effect |
|---:|---|---|---|
| 1 | `1q\r\n` | `[1]` | First getter consumes `1`, ungets `q`; count limit ends before retry |
| 2 | `1q\r\n` | `[1, 0]` | Second extraction retries ungot `q`, fails, writes zero, leaves `q` unread |
| 2 | `1?\r\n` | `[1, 0]` | Same digit-prefix then failed retry, with independent punctuation witness |
| 2 | `q 2\r\n` | `[0, 0]` | Each extraction fails at the same ungot `q`; later `2` is never reached |

The stream's numeric locale is explicitly C for this path (`analysis/resume67-sw2sp-locale.md`), so excluding comma isolates the non-group delimiter case. This is a static deduction from Qt plus the original caller; it is not a fresh original-process comparison.

## Candidate mismatch and focused test plan

The current decoder already contains a narrow `x` retry profile (CP121) and `1X` count-two profile (CP145); its final suffix path accepts selected `x`/comma cases and otherwise throws. The SPRING2 callsite is `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` around the flag loop; focused tests are in `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`.

For a source cut, add a private bounded branch for `q`/`?` at the extraction stop point, preserving the raw byte cursor and returning zero without advancing when either is the next requested extraction. Permit the matching suffix after a parsed canonical decimal prefix only under count-one/count-two gates. Tests should assert the four table rows above, both input files unchanged, the existing x/comma cases unchanged, and refusal for a neighboring high byte or unrelated malformed token. Keep all prior radix/overflow/BOM/path gates untouched. A nonzero `[1]` flag serializes as ASCII `1` through the retained writer; `[1,0]` serializes `1\t0\t\r\n` if the public Save path is under test.

Current frozen CP157 source/test hashes before any CP158 implementation: reader `5FBE75CD40647CD063AE27AB0128592BC6AA564D0B8CD499082C9B9B8F88BC43`; focused test `348CF582BBE0BF6CD6915DC3FC187A3521A638AF88B027D6A24C7A380B49C154`.

## Limits

This closes only two ASCII invalid-character witnesses and their prefix/retry ordering. It does not claim all punctuation, UTF-8/ACP decoding, malformed BOM handling, comma grouping, overflow status, or full QTextStream state parity. No source, test, build, GUI, or mutable REA state changed in this CP158 evidence pass.
