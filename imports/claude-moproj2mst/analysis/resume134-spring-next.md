# CP134 next SPRING2 frontier: two extractions from `018`

## Selected count-two continuation

The next bounded profile is a two-row primary SPRING record with the entire companion byte string exactly `018\r\n`. The first Qt integer extraction selects octal because the digit after the leading zero is `1`. It consumes `01`, stops at invalid octal digit `8`, ungets that character, and succeeds with value 1; the next unread offset is 2 and status is zero. Since `8` remains available, `atEnd()` is false. The second extraction starts at that nonzero character, uses decimal parsing, consumes it, and succeeds with value 8; the next unread byte is CR at offset 3, status remains zero. The helper stops at count two and returns flags `[1, 8]`.

This is a meaningful continuation of the already admitted one-row `018` witness: it qualifies that the ungot suffix becomes the next flag. The candidate currently only allows the token under its count-one gate. Proposed focused test: two complete ASCII primary rows and exact `018\r\n` companion; assert row flags 1 and 8 in order and exact preservation of both files. Keep other counts and companion spellings outside this cut; e.g. `019` also has a distinct octal-prefix/decimal-suffix path and is not admitted by this recommendation.

## Source basis and loop behavior

`analysis/resume83-sw2sp-parser-independent-review.md` contains the independently recaptured, PE-byte-verified Qt 4 `QTextStream::getInteger`, `operator>>(int&)`, `atEnd()` and `ungetChar` evidence. It binds to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The corrected Qt table explicitly records `018\r\n` -> 1, successful, next offset 2 at `8`, status 0. Its base-selection trace at `0x670c5417–0x670c5448` states that a leading zero with second digit 1 chooses octal, while a nonzero first digit uses decimal parsing; the later `8` terminates the octal scan and is ungot.

`analysis/resume119-spring2-normal-path.md` and `analysis/wave2-save-diffs-300-support-1410da690.c` bind the public loop: it prezeros flag slots and extracts while `!atEnd()` and index<count, incrementing after each extraction without inspecting stream status. Therefore the unread 8 is consumed by the second requested slot and the helper returns true after two values. This result is static PE plus caller-control-flow inference, not an original-process differential run. The fixture is ASCII and both outputs are small in-range integers; no ACP, overflow or narrowing claim is involved.

## Candidate boundary

Admit only the exact count-two whole companion bytes `018\r\n` with a two-row primary file. Do not infer general octal, all leading-zero spellings, arbitrary partial-token composition, count-three EOF behavior, other paths, codecs or non-ASCII handling. Candidate refusals outside this slice are coverage limits, not claims about original rejection.

No source/test edits, build, GUI, original process, or mutable REA database activity occurred for this read-only report.
