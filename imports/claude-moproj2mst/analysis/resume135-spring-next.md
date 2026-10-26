# CP135 next SPRING2 parser frontier: two-row `0b12` continuation

## Selected bounded profile

The next useful profile is a two-row primary SPRING record with the entire companion byte string exactly `0b12\r\n`. The first Qt extraction recognizes the binary prefix, consumes binary digit `1`, then ungets invalid binary digit `2` and succeeds with value 1. Its next unread offset is 3, at `2`, and status is zero. Because `2` remains available, the helper's `atEnd()` loop enters a second extraction. The second extraction starts with nonzero digit `2`, parses it as decimal, and succeeds with value 2, leaving CR at offset 4. The helper reaches count two and returns flags `[1, 2]`.

The current candidate admits this token only for one row; it does not yet model its unread suffix as the next flag. Proposed focused test: two complete ASCII primary rows and exact companion `0b12\r\n`; assert `[1,2]` in row order and byte-for-byte unchanged inputs. Retain count-three, whitespace/mixed/repeated spellings, alternate binary digits and other paths guarded. These are scope boundaries, not source claims that Qt rejects them.

## Retained evidence

`analysis/resume83-sw2sp-parser-independent-review.md` records the corrected Qt 4 integer table and independently recaptured, PE-byte-verified code for `getInteger`, `operator>>(int&)`, `atEnd()` and `ungetChar`. The reference original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its table explicitly gives `0b12\r\n` -> value 1, successful extraction, offset 3 at unread `2`, fresh status 0. The PE analysis shows QChar case-folding before binary-prefix selection and the binary digit loop ungetting a nonbinary following digit only after at least one valid digit has accumulated. A fresh decimal extraction at `2` is successful, with no further character required to establish its value.

The original helper `FUN_1410da690`, documented in `analysis/resume119-spring2-normal-path.md` and retained decompile `analysis/wave2-save-diffs-300-support-1410da690.c`, prezeros flags, loops while not atEnd and index<count, extracts and increments without inspecting status, and returns true after successful open. With exact count two, this consumes the ungot `2` as the second flag. The result `[1,2]` is static Qt/caller inference, not an original-process differential. Both values are ASCII/in-range; no ACP or overflow claim is needed.

## Boundary

The recommended admission is limited to exact count two and exact whole companion bytes `0b12\r\n`. It must not imply general multi-value parsing, arbitrary binary prefixes, malformed input, codecs/non-ASCII, or count-three EOF behavior. No source/test edits, build, GUI, original process, or mutable REA database activity occurred; the CP134 source hashes were observed and left unchanged.
