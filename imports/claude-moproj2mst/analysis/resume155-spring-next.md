# CP155 next SPRING2 numeric family: complete unsigned hexadecimal

## Recommended family

The next substantial SPRING2 gap is a complete unsigned hexadecimal token in the count-one route: `0[xX][0-9A-Fa-f]+`, followed by ASCII whitespace or EOF. The source getter recognizes the prefix and digits case-insensitively, accumulates each digit in base 16, and the int extraction stores the low dword. The candidate currently admits only selected literals such as `0xA`, `0XA`, and `0xA0`; ordinary values such as `0xA1` and `0xFFFFFFFF` remain in its one-row refusal list.

This family avoids expanding the pile of exact hex tokens. With a complete token and at least one valid hex digit, the original getter returns success and leaves the following delimiter unread. Since the SPRING2 caller requests one value for the one-record fixture, the unread delimiter is not retried. Modulo-32 accumulation in the candidate is equivalent to Qt's 64-bit left-shift/add followed by `int&` low-dword store.

## Immutable Qt evidence and conversion result

Original `QtCore4_64.dll` SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Original executable `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The PE-byte-checked `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows:

- Base-zero selection case-folds the character after leading zero through `QChar::toLower` at `0x670c53bf`, and selects base 16 when it is `x` at `0x670c53cf–53e5`.
- The getter consumes the `x` prefix, then reads the first hexadecimal digit. Each candidate digit is case-folded with `QChar::toLower` at `0x670c559d`.
- Digit classes 0–9 and a–f are checked at `0x670c55a5–55be`; accepted digits accumulate with `shl rdi,4` and add at `0x670c55c0–55ca`. No overflow rejection occurs in this loop.
- The first non-hex character is ungot at `0x670c55e6–55f1`; at least one accepted digit takes the successful getter exit at `0x670c55f6–55f9`.
- `QTextStream::operator>>(int&)` writes the low dword at `0x670c6166–616a`.

Independent case-fold and parser details appear in `analysis/resume83-sw2sp-parser-independent-review.md`, including the table entries `0xA` -> 10, `0XA` -> 10, and the asymmetry between hex prefix failure and binary prefix rollback. Keep the prefix case fold and digit case fold in the proposed range; ASCII upper/lower hex spelling is directly qualified. Non-ASCII digits remain outside this candidate decoder profile.

The reachable original caller is `FUN_1410da690` in `analysis/wave2-save-diffs-300-support-1410da690.c`: it opens `SPRING2.INI`, extracts one int for a one-record primary file, copies the result into the row flag, ignores QTextStream status, and returns success after open. `analysis/resume120-spring2-writer-order.md` qualifies the downstream writer: any nonzero flag causes companion output, and the integer is serialized in decimal. Thus `0xA1` loads as 161 and Save normalizes it as `161`; `0xFFFFFFFF` loads as -1 and Save writes `-1`.

## Candidate gap and bounded acceptance plan

Current frozen reader SHA-256: `8AD57C52B787DD68B49AE3B44BF201C614C6ADC0AC299BF4FCF1ED53B8A31841`. Focused test SHA-256: `7A5994C23A94095B64EE938A5A2E649C44E686675A86161B8CAAB82FA64EEFB8`.

The reader has exact paths for a few hex values and malformed prefixes, then general leading-zero/radix refusal. A reusable family should be gated by `allowSingleRowQualifiedToken` and match only an unsigned `0x`/`0X` prefix with one or more ASCII hex digits and a full-token whitespace/EOF terminator. It should accumulate low 32 bits with repeated shift-left-four and digit-add. Keep the existing early bare-prefix and zero-valid-digit malformed handlers before the new complete-token route so their already qualified cursor/status approximations remain unchanged. Keep signed-prefixed hex unsupported: Qt sign handling chooses decimal, not auto-detected hex.

Suggested matrix with a complete one-row ASCII `SPRING.INI` primary file:

| `SPRING2.INI` bytes | Expected flag | Purpose |
|---|---:|---|
| `0x1\r\n` | 1 | First ordinary hex digit family case |
| `0xA1\r\n` | 161 | Extends the exact `0xA` witness |
| `0XfF\t` | 255 | Uppercase prefix and mixed-case digits |
| `0x80000000\r\n` | INT32_MIN | High bit of the low dword |
| `0xFFFFFFFF` | -1 | EOF terminator and full low dword |
| `0x100000001\r\n` | 1 | Narrowing beyond 32 bits |
| exact `0xA` / `0XA` / `0xA0` | 10 / 10 / 160 | Preserve earlier exact behavior |
| `0x` / `0xG\r\n` | prior bounded behavior only | Bare/invalid-first-digit failures remain separate |
| `0xA0G\r\n` | remain guarded in this cut | Do not broaden valid-prefix/unread-suffix handling |
| `+0xA\r\n`, `-0xA\r\n` | remain guarded | Sign selects decimal; this is not signed hex |
| count-two `0xA1\r\n` | guarded | Keep this admission count-one-only |

Assert byte-for-byte preservation of primary and companion files for each successful full-token case. Keep malformed/attached suffixes, zero-digit conversion status, multi-row retries, BOM/codec, non-ASCII text, locale-sensitive signs, other filenames, and original-process comparison claims outside the family.

## Qualification boundary

This is a static inference from byte-matched Qt code plus the retained original SPRING2 caller and writer. It is not a fresh original-process differential. The complete-token family is source-closed for value, success, and one-row delimiter/caller effect. Hex digit parsing after a valid prefix but before a malformed suffix (`0xA0G`), a prefix with no digits, and multi-value status/cursor effects remain distinct and should stay guarded until separately assigned. No source, test, build, GUI, or mutable REA state changed for this analysis.
