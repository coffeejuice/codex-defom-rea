# CP125 next bounded `SPRING2.INI` Qt integer branch

## Selected branch: case-folded base-0 hexadecimal

The next source-ready token branch after the bounded signed low-dword witness is a one-digit hexadecimal token: `0xA` (and its case-folded spelling `0XA`) extracts the integer flag `10` successfully. The current companion parser rejects both because it deliberately admits only canonical decimal flags plus the earlier bounded prefix/narrowing cases.

This is a separate Qt base-selection branch, not an extension of signed overflow. It is a good next source increment if the candidate is ready to admit a bounded hexadecimal profile. A narrow initial profile can admit exactly `0xA`/`0XA` and retain all other prefix forms, digit sequences, malformed hex, and overflow guarded. If root instead wants a reusable grammar, the retained PE evidence below supports case-folded `0x` recognition and hexadecimal digit classification; overflow/narrowing still needs a separate explicit limit.

## Retained Qt evidence

Original executable: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtCore: `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume83-sw2sp-parser-independent-review.md` records an independent recapture and PE-byte verification of `QTextStream::getInteger` and `operator>>(int&)`. The getter calls QChar lowercasing before checking the automatic base prefix (`0x670C53BF`) and again before hex-digit classification (`0x670C556B`, `0x670C559D`). It therefore recognizes uppercase `X` and uppercase hex digits after case folding. The corrected one-value table in that report explicitly records `0xA` -> `10` and `0XA` -> `10`, both successful, leaving the CR/LF delimiter unread. The same getter/operator is called for SPRING2 flags by `FUN_1410da690` (`analysis/wave2-save-diffs-300-support-1410da690.c`); the helper copies the int result, increments its index, and ignores conversion status. Thus for a one-row SPRING record with a readable companion, the caller-visible flag result is statically predicted as 10.

The Unicode bytes are ASCII and the default stream locale is C (`analysis/resume67-sw2sp-locale.md`), so this profile does not depend on process ACP or a runtime locale witness. This remains static PE inference, not an original-process comparison.

## Minimal focused candidate tests

Use a complete valid one-record ASCII `SPRING.INI` and present ASCII companion fixtures:

- `0xA\r\n` -> success, flag 10, both files byte-for-byte unchanged;
- `0XA\r\n` -> success, flag 10, both files byte-for-byte unchanged;
- `10\r\n` -> existing canonical decimal control, flag 10.

Keep `0xG`, bare `0x`, signed-prefixed hex such as `+0xA`, leading-zero base selection, multi-digit arbitrary hex values, overflow, non-ASCII, and BOM cases guarded until their distinct status/cursor/narrowing contracts are admitted. In particular, invalid hex is not an original rejection at the file-helper level: Qt writes zero and sets stream status, while `FUN_1410da690` still returns true after open. Do not call a candidate unsupported result source-equivalent for those invalid forms.

## Evidence gap / scope boundary

For the exact one-digit `A` witness, no missing source dependency remains. A broader `0x[0-9A-Fa-f]+` implementation would need its own bound for 64-bit accumulation and low-dword narrowing plus tests for invalid first digit and later delimiter/cursor behavior; it should not be inferred merely from the `0xA` row. The candidate can safely keep those additional cases guarded while deciding whether the next implementation is a literal witness or the bounded reusable base-16 branch.

No source, test, build, GUI, original process, or mutable REA state changed for this read-only selection.
