# CP129 next bounded SPRING2 token: failed binary prefix rollback

## Selected one-row branch

The next distinct branch after successful `0B10` is `0B2\r\n`: an automatic-base binary prefix followed by a non-binary first digit. Qt case-folds `B` during base selection, recognizes the binary prefix, then rejects digit `2`. Its getter returns failure, restores the whole zero-plus-prefix sequence, and leaves the next unread position at byte offset 0. The int extraction operator stores zero and sets fresh conversion status 2. The original SPRING2 helper copies that zero, ignores status, and stops after the one requested flag, so it returns success with public flag 0. This exact one-row result is a candidate gap; the current reader guards the token.

The smallest proposed test is one valid ASCII primary row and companion bytes `0B2\r\n`; assert read success, flag zero, and unchanged input bytes. Keep `0B3\r\n`, bare `0B\r\n`, and multi-row extraction at the restored prefix guarded in the candidate. `0B10\r\n` is the successful control. These neighboring forms are candidate scope boundaries; the retained source indicates the same initial invalid-binary-digit rollback mechanism for `0B3` and a separate end-of-input status case for bare `0B`.

## Immutable Qt evidence and caller effects

`analysis/resume83-sw2sp-parser-independent-review.md` records independently recaptured and PE-byte-verified `QTextStream::getInteger`, `operator>>(int&)`, and `ungetChar` regions against original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its corrected table gives `0b2` -> value zero, failed getter, next offset 0 with the `0` prefix restored, and fresh status 2. The review explains that the base character is lowercased before prefix selection and that binary invalid-first-digit handling ungets both the stored `b` and leading `0` (`0x670c595f–599b`). Thus the same behavior applies to uppercase `B` in `0B2`.

The review's operator capture shows failure writes zero and status. The retained SPRING2 caller evidence in `analysis/wave2-save-diffs-300-support-1410da690.c` and `analysis/resume119-spring2-normal-path.md` shows `FUN_1410da690` copies the integer without checking stream status and exits its count loop once the one requested flag is filled. Since the failed prefix is restored but the requested count is one, it does not cause another helper extraction. Result 0 is a static PE/caller deduction, not an original-process differential. The fixture is ASCII, with no ACP inference; binary magnitude is tiny and no overflow/narrowing path is involved.

## Boundary

This recommendation is only the exact one-row `0B2` failure witness. It does not admit generic binary parse failures, any binary digit grammar, signed forms, cursor retries for multiple rows, status propagation outside this helper, codecs/non-ASCII, or other filename/count paths. No source/test edits, build, GUI, original process, or mutable REA database activity occurred for this report.
