# CP132 bounded two-row SPRING2 binary rollback implementation

## Change

The reader now admits the exact companion `0B2\r\n` when the primary SPRING record count is two. Each integer extraction sees the exact same bytes: Qt fails the binary parse, restores the full prefix at the current cursor, and the int operator writes zero. The native adapter returns two zero flags and preserves the exact primary and companion file bytes. This profile has two explicit gates: the existing one-row token admission remains unchanged, and the added retry gate requires both a requested count of exactly two and the entire companion byte string exactly `0B2\r\n`. Mixed-token companions, leading/trailing whitespace variants, and a count-three `0B2` request still refuse. Tests cover the exact bytes and these broader variants.

The regression asserts both flags are zero, both input files are unchanged, and the same companion with three requested records is rejected without publishing partial rows. Neighboring binary forms and other two-row values remain guarded by the existing tests. No generic sticky-status or arbitrary repeated-error parser was introduced.

## Source qualification

`analysis/resume132-spring-next.md` combines the Qt integer state machine in `analysis/resume83-sw2sp-parser-independent-review.md` with original helper `FUN_1410da690` in `analysis/resume119-spring2-normal-path.md` / `analysis/wave2-save-diffs-300-support-1410da690.c`. The retained QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

For `0B2`, Qt case-folds the prefix, selects binary, rejects the first digit `2`, and ungets the `b` and leading `0`, leaving the read position at token offset 0. The getter reports failure; `operator>>(int&)` stores zero and sets nonzero status. The helper initializes both destinations to zero and iterates while `!atEnd()` and index<count. The restored bytes keep `atEnd()` false. The operator attempts another getter even though status is already set, and preserves that existing status when the second attempt fails. The loop advances to index two and returns true after successful file open. Therefore `[0,0]` is the static expected result; no original-process differential run was performed.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `14446E8DB05DCF4F6D43955268DF7C67E6AAE43B44C86C765E5792384F553D2A`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `78AD9CC8A9B2F49EAE7331D9FD16E749ED7FB69F9D5238113FC491E3CE033AB2`


