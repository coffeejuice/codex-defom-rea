# CP156 next SPRING2 numeric family: complete unsigned binary

## Recommended family

After the complete unsigned hexadecimal route, the next substantial numeric family is a count-one complete unsigned binary token: `0[bB][01]+`, followed by ASCII whitespace or EOF. Qt recognizes the binary prefix case-insensitively, accumulates each `0`/`1` digit by shift-left-one/add, and stores the low dword in the int destination. The candidate currently admits one exact uppercase token (`0B10`) and a few malformed/partial witnesses, but rejects other complete binary values such as lowercase `0b10` and `0B1010`.

This family is distinct from the existing invalid-first-digit rollback (`0B2`) and valid-prefix/unread-suffix case (`0b12`). Those exact routes must stay ahead of a complete-token family so their caller-visible outcomes do not change. The new branch should only accept at least one valid binary digit and a complete token terminator; malformed and no-digit prefixes remain separately guarded.

## Qt source and caller evidence

The shipped `QtCore4_64.dll` hash is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the original application `DEF_GUI_MO_64.exe` hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The byte-matched getter capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows:

- In the automatic-base path, `QChar::toLower()` is called for the character after leading zero at `0x670c53bf`; a normalized `b` selects base 2 at `0x670c53ef–5405`.
- The binary body is `0x670c5888–5969`. It reads the first post-prefix character, normalizes it at `0x670c5912–591f`, and accepts only ASCII `0` or `1` at `0x670c5924–592f`.
- Each accepted bit is accumulated as `rdi = rdi * 2 + bit` at `0x670c5931–5946`; the loop reads the next character and repeats.
- On the first nonbinary character, Qt ungets it at `0x670c5954–595f`. At least one accepted bit takes the successful path at `0x670c5964–5969`; no accepted bit rolls back the stored `b` and leading zero at `0x670c5980–599b`.
- The int extraction stores the low dword on success at `0x670c6166–616a`.

This gives a complete-token value/cursor/status contract: for a valid binary run followed by whitespace/EOF, extraction succeeds, consumes the prefix and all bits, leaves the terminator unread, and starts with successful stream status. On a one-record primary file, the original SPRING2 caller requests one int, writes it to the row flag, ignores stream status, and returns success after opening the companion (`analysis/wave2-save-diffs-300-support-1410da690.c`). A nonzero result reaches the companion Save branch and is serialized as decimal (`analysis/resume120-spring2-writer-order.md`). For example, `0B1010` loads as 10 and Save writes `10`; a 32-bit all-ones run loads as -1 and Save writes `-1`. This is static qualification, not an original-process differential.

## Candidate gap and bounded test plan

Candidate reader SHA-256: `62F972D295798FA02E84EA84FE09D1E6243D382903C4BD57AAF0DB36B91B17D4`. Focused test SHA-256: `A528A831FEE8F024F6C9EA52D0A1A819C1CD3B721C9680268A70B676E8854912`.

`readSpringFlagInt` has exact count-one `0B10`, invalid-prefix `0B2`, and `0b12` partial-read routes. The count-one unsupported matrix still includes complete `0b10`. A reusable complete-binary route can be gated on `allowSingleRowQualifiedToken`, match `0` followed by either-case `b`, require at least one bit, accumulate modulo 2^32, and require ASCII whitespace/EOF after the digit run. Place it after the exact `0B2` and `0b12` handlers to preserve their behavior. Retain count-two restrictions; the separately qualified exact 0b12 count-two continuation is outside this complete-token family.

Suggested matrix using a valid complete one-row ASCII primary fixture:

| Companion bytes | Expected flag | Purpose |
|---|---:|---|
| `0b1\r\n` | 1 | Lowercase base prefix |
| `0b10\r\n` | 2 | Existing one-row refusal; common binary value |
| `0B1010\t` | 10 | Uppercase prefix, multiple bits, tab delimiter |
| `0b10000000000000000000000000000000` | INT32_MIN | Low-dword high-bit boundary at EOF |
| `0B11111111111111111111111111111111\r\n` | -1 | Full-width low-dword result |
| `0B100000000000000000000000000000001` | 1 | Narrowing beyond 32 bits |
| `0B10\r\n` | 2 | Retain existing exact result |
| `0B2\r\n` | prior zero/failure approximation | Preserve invalid-first-bit rollback witness |
| `0b12\r\n` | prior value 1/unread-2 approximation | Preserve valid prefix with malformed suffix witness |
| `0b10x\r\n` | remain guarded in this cut | Do not broaden valid-prefix/unread-suffix handling |
| `0b\r\n` / `0B` | remain on existing bounded/guarded paths | Do not generalize zero-digit rollback |
| count-two `0b1010\r\n` | guarded | Preserve count-one boundary |

Accepted and refused cases should assert exact preservation of both original files. Keep explicit signs, invalid digits beyond the exact witnesses, BOM/non-ASCII codecs, locale-specific characters, other filenames, and multi-value status/cursor propagation outside the new path.

## Limits

The complete binary value path is source-closed for ASCII digits, success, cursor boundary, low-dword result and the one-row public flag. It does not close the internal retry behavior after a malformed prefix or delimiter in a multi-row caller. No source, test, build, GUI, or mutable REA state changed for this read-only report.
