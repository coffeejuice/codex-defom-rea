# CP126 next bounded SPRING2 token: invalid hexadecimal first digit

## Selected source branch

The smallest distinct branch after the admitted `0xA`/`0XA` value is a failed hexadecimal conversion with a consumed prefix and an unread invalid first digit. For a one-row record with readable companion bytes `0xG\r\n`, original Qt `QTextStream::operator>>(int&)` stores zero; the helper copies that integer, ignores stream status, and returns success because the file opened. Publicly, the row flag is therefore predicted as `0`. The current candidate rejects this companion spelling, so it remains a concrete reachable mismatch.

This is intentionally a one-row malformed-prefix case. Do not infer the behavior of a second requested extraction, where the unread `G` is retried and conversion status is already sticky, or of later tokens. The smallest follow-up test should use valid one-row ASCII `SPRING.INI`, companion `0xG\r\n`, assert successful read and flag `0`, and verify both source files remain byte-for-byte unchanged. Neighboring controls: admitted `0xA\r\n` and `0XA\r\n` still yield `10`; bare `0x\r\n` fails at end-of-input and should remain guarded until its public helper outcome is separately admitted; `0xA0\r\n` is a valid broader hex value but remains outside the current candidate profile.

## Native evidence and parser mechanics

The retained independent PE review is `analysis/resume83-sw2sp-parser-independent-review.md`. It records PE-byte-verified `QTextStream::getInteger` and `operator>>(int&)` regions from the original QtCore4_64.dll (SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`) paired with original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

For the token `0xG`, base-0 dispatch recognizes the case-folded `0x` prefix. In the hex loop, `G` fails digit classification and is ungot while the prefix stays consumed. The getter returns failure with next unread byte offset 2 (the `G`). The int extraction operator writes zero and sets its fresh stream status to the nonzero conversion status (2 here; internal Qt status, not an adapter code). The original SPRING2 helper `FUN_1410da690` invokes this same extraction, copies the destination and does not inspect stream status; the count-one helper loop then completes and returns true. Caller-visible flag zero is a static call-path deduction, not an original-process differential observation.

The input is ASCII. The retained `analysis/resume67-sw2sp-locale.md` evidence establishes the default `QTextStream` C locale relevant to numeric classification; no process ACP assumption is needed. File open, exact public filename, count-one scope, decoder admission and no-input-mutation behavior are independently retained in `analysis/resume119-spring2-normal-path.md` and the current reader path.

## Candidate scope recommendation

Admit only the exact one-row `0xG` companion witness for this increment. Preserve the current refusal for other invalid hex letters, bare prefix, arbitrary hex values, signs before prefixes, multi-row retry behavior, non-ASCII and BOM inputs. In particular, the original accepts case-folded valid hex digits beyond `A`, so candidate refusals for those are coverage boundaries rather than original-format rejection claims.

No source or test edits, build, GUI, original process, or mutable REA database were used for this static audit.
