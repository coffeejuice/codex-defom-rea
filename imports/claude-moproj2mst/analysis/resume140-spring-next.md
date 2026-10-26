# CP140 next SPRING2 frontier: uppercase bare hexadecimal prefix with CRLF

## Selected exact one-row case

The next bounded token is `0X\r\n` as the complete companion bytes for one primary row. Qt case-folds the lookahead before automatic-base prefix recognition, so uppercase `X` selects the same hexadecimal path as lowercase `x`. CR is then not a hexadecimal digit; the getter consumes `0X`, ungets CR, fails with zero, and leaves the next unread byte at offset 2. Data remains, so the fresh extraction status is 2. The one-row SPRING2 helper copies zero, ignores stream status, stops at its requested count and returns true. Expected public flag: 0.

This is distinct from a non-prefix decimal fallback on uppercase `X`: it qualifies the exact case-folded prefix-selection call. It is also distinct from lowercase `0x\r\n` already admitted, though both have the same public zero and cursor/status prediction. The candidate currently only admits the lowercase exact prefix form; the generic parser refuses uppercase `X`.

Suggested test if assigned: valid one-row ASCII `SPRING.INI` and exact companion `0X\r\n`; assert success, flag zero, and byte identity. Keep exact `0X` at EOF, count-two retries, `0XG`, and mixed/other prefix spellings guarded. Those have distinct EOF/invalid-digit/loop effects and should not be implied by this test.

## Retained source evidence

The immutable Qt evidence in `analysis/resume83-sw2sp-parser-independent-review.md` records `QChar::toLower()` before automatic base selection (`0x670c53bf`) and before hex-prefix validation (`0x670c556b`). The same report records `0xG\r\n` failing after the consumed prefix with the invalid following character ungot at offset 2/status 2, and explains the equivalent lowercase-prefix branch. Applying these exact native branches to uppercase `X` plus CR yields the offset/status above. The report's QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/resume119-spring2-normal-path.md` and `analysis/wave2-save-diffs-300-support-1410da690.c` show the helper attempts extraction for initially nonempty input, copies the int result, ignores conversion status and returns true after a one-slot count. This public result is static PE/caller-flow inference; no original-process differential was run. The bytes are ASCII, with no ACP, overflow or narrowing issue.

## Candidate scope

Limit any future admission to count one and the complete byte string `0X\r\n`. This report changed no source/test files and used no build, GUI, original process or mutable REA database. Current CP139 source/test hashes remain unchanged: `636CB27AEF27E60F023676351710180C5DB39FB2E91AF6DBEE409D234D39DEA7` and `7C3B51170949ED88A759FC63E1696A18E08A989983F31B183BE2E8723690C670`.
