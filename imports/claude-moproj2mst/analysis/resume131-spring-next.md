# CP131 next bounded SPRING2 token: two-digit hexadecimal accumulation

## Selected one-row branch

After the exact binary partial-prefix witness `0b12`, the next small, distinct successful branch is `0xA0\r\n`. Qt recognizes the automatic hexadecimal prefix, converts `A` case-insensitively to digit 10, consumes the following hex digit `0`, and returns 160. The getter succeeds with next unread byte offset 4 (the CR) and the fresh int extraction status remains zero. The one-row SPRING2 helper copies 160, ignores status and stops at its requested count, so the public row flag is statically predicted as 160. The current candidate keeps this broader valid hex value guarded despite admitting exact `0xA`/`0XA`.

Minimal candidate test: one complete ASCII primary row plus `0xA0\r\n`; assert read success, flag 160, and byte-for-byte preservation of both inputs. Retain arbitrary longer hex strings, signed prefixes, malformed digits, overflow and multi-row behavior as guards. In particular, the fact that `0xA0` is valid does not qualify a general hex grammar or low-32-bit overflow handling.

## Retained source evidence

The immutable PE review in `analysis/resume83-sw2sp-parser-independent-review.md` is bound to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its `QTextStream::getInteger` disassembly was independently recaptured and byte-verified. It locates `QChar::toLower()` before prefix selection (`0x670c53bf`), and before hex digit classification (`0x670c559d`); normalized hex digits are classified as 0–9/a–f. The getter's accumulation loop adds each valid digit in the selected radix and completes successfully after the final valid digit.

The exact `0xA0` token therefore consumes four ASCII bytes and yields 10*16+0 = 160, with CR unread at offset 4. The int extraction operator writes the successful int value and retains fresh status zero. The SPRING2 caller binding is `FUN_1410da690`, retained in `analysis/wave2-save-diffs-300-support-1410da690.c` and summarized in `analysis/resume119-spring2-normal-path.md`: it copies the flag, ignores stream status, and ends after one requested companion value. Thus 160 is a static call-path deduction, not an original-process differential result. No host ACP or overflow claim is needed for this ASCII in-range input.

## Guard boundary

The recommended increment is this single exact token only. Do not admit arbitrary multi-digit hex, malformed hex, signed forms, values requiring narrowing, additional rows, BOM/non-ASCII input, or other file/count paths from it. Existing candidate guards for those cases remain coverage limits and should not be represented as original rejection behavior.

This read-only selection changed no source or tests and used no build, GUI, original process, or mutable REA database.
