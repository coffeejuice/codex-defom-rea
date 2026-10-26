# CP127 next bounded SPRING2 token: octal selection with unread suffix

## Selected branch and caller result

The next distinct source-qualified token is `018\r\n` for a one-record `SPRING2.INI`. Qt's base-0 integer getter examines the digit after the initial zero: `1` selects octal, then `8` is not an octal digit and is returned to the stream. The getter succeeds with value `1`, leaves byte offset 2 (the `8`) unread, and the fresh int extraction status remains success. The one-record SPRING2 helper copies that flag, performs no further extraction because its requested count is one, ignores status, and returns true. The candidate currently rejects this spelling through its leading-zero/radix guard, so it is a bounded reachable gap.

Suggested minimal test: valid one-row ASCII `SPRING.INI` plus companion `018\r\n` must read successfully with flag `1`; assert both files are byte-for-byte unchanged. Retain `08\r\n` as a separate guarded candidate neighbor for now: the Qt selection branch chooses decimal when the second digit is 8, so the original result is 8, but admitting it is a separate decimal-fallback decision. Keep multi-row SPRING2 outside this proposed cut. The original second extraction after `018` would see the unread `8` and can parse it as decimal 8; that is a separate loop profile, not part of the one-row expected result.

## Retained native evidence

The immutable reference is `analysis/resume83-sw2sp-parser-independent-review.md`, based on original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and shipped QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its independently recaptured, PE-byte-verified `QTextStream::getInteger` region documents automatic base selection at `0x670c5417–0x670c5446`: a second digit in 0–7 selects octal; other decimal digits fall through to base 10. The corrected witness table explicitly gives `018\r\n` -> 1, next unread byte offset 2 (the `8`), successful getter, and zero fresh status. It separately gives `08\r\n` -> 8 and confirms that 018's later 8 remains unread.

The SPRING2 helper call path is retained in `analysis/wave2-save-diffs-300-support-1410da690.c` / `analysis/resume119-spring2-normal-path.md`: `FUN_1410da690` invokes this integer extraction, copies the destination and ignores conversion status, while its count loop ends once the one requested flag has been filled. This gives the predicted public flag result. It is static PE/control-flow inference; no original-process differential run was performed. Input is ASCII, so the profile does not depend on ACP. The default C numeric locale is documented in `analysis/resume67-sw2sp-locale.md`.

## Candidate boundary

This increment can be a single exact-token witness or, if root approves, the narrowly bounded selection/cursor sub-branch that fully handles `018` only. Do not generalize all leading-zero input, arbitrary octal, signs, multi-row retry/cursor behavior, non-ASCII, BOMs, other filenames or count paths from this example. Candidate refusal of those inputs remains a coverage boundary, not a statement that the original refuses them.

No source/test edits, build, GUI, original-process execution, or mutable REA database activity occurred for this read-only report.
