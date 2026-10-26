# CP138 next SPRING2 frontier: bare hexadecimal prefix at EOF

## Selected exact one-row token

The next distinct status/cursor case is a readable one-row companion whose complete bytes are exactly `0x` (no newline). Qt recognizes and consumes the hexadecimal prefix but reaches EOF before any digit. The getter fails with value zero and next unread position at EOF (offset 2); because `atEnd()` is true after failure, the fresh int operator status is 1. The original SPRING2 helper entered the extraction because the stream initially contained two bytes, copies zero, ignores status, completes its one requested slot and returns true. The predicted public flag is zero.

This is distinct from `0xG\r\n`: that failure leaves G unread at offset 2 and sets status 2 because input remains. The candidate has an explicit bare-hex-prefix guard and rejects `0x`. A minimal test is a valid one-row ASCII `SPRING.INI` and `SPRING2.INI` bytes exactly `0x`; assert read success, flag zero, and byte-for-byte preservation. Keep `0x` with trailing whitespace, multi-row retry, and other bare/invalid prefix spellings guarded until their cursor/status sequences are admitted.

## Immutable Qt and caller evidence

The corrected one-value table in `analysis/resume83-sw2sp-parser-independent-review.md` gives `0x` -> value zero, getter failure, next unread offset 2 at EOF, fresh status 1. The same report records PE-byte-verified `getInteger`, `operator>>(int&)`, `atEnd()`, and `ungetChar` regions from the original QtCore4_64.dll. QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/resume119-spring2-normal-path.md` and retained `FUN_1410da690` decompile show the helper prezeros outputs, tests `!atEnd()` before extraction, then copies the destination and does not inspect stream status. Since the file initially has bytes, the first extraction occurs; the prefix is consumed to EOF, and the helper returns true after its single requested slot. The public result is a static combination of Qt PE and caller decompile, not an original-process differential. The input is ASCII and no ACP, overflow, or narrowing issue is involved.

## Candidate scope

If assigned, limit admission to count one and the exact complete companion bytes `0x`. Do not infer a general incomplete-prefix decoder or multi-row behavior. No source/test edits, build, GUI, original-process execution or mutable REA database activity occurred in this read-only selection.
