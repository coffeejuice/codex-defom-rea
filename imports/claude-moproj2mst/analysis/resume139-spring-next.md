# CP139 next SPRING2 frontier: hex prefix followed by CRLF

## Selected exact one-row case

The next distinct case is a one-row companion whose complete bytes are `0x\r\n`. Qt recognizes and consumes the `0x` prefix, then fails to read a hex digit at CR. Unlike bare `0x` at EOF, it ungets CR and leaves the next unread byte at offset 2. Since bytes remain, the fresh integer operator status is the non-EOF conversion status 2. The operator writes zero; the original helper entered the extraction because the stream initially had data, copies zero, ignores status, and returns true after its one requested flag. Predicted public flag: 0.

This differs from CP138's exact `0x` bytes, which reach EOF after the consumed prefix and set status 1. The candidate currently rejects `0x\r\n` under its general bare-prefix guard. Minimal test if assigned: valid one-row ASCII primary and companion exactly `0x\r\n`; assert success, flag zero, and both input files byte-for-byte unchanged. Keep other whitespace, bare-prefix variants, count-two retry, and other malformed forms guarded.

## Evidence qualification

The original PE-verified Qt review is `analysis/resume83-sw2sp-parser-independent-review.md`, bound to executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its table records `0xG\r\n` as a failed hex conversion with prefix consumed, invalid post-prefix byte ungot at offset 2 and status 2 while data remains. It separately records bare `0x` at EOF as offset 2/status 1. The retained getInteger analysis explains that hex failure leaves the recognized prefix consumed and ungets the invalid following character. Applying that same branch to CR (not a hex digit) yields the expected offset/status above. The exact `0x\r\n` bytes are a static deduction from the parser path, not a separately captured table row or original-process differential.

`analysis/resume119-spring2-normal-path.md` and retained `FUN_1410da690` decompile show that the helper prezeros the slot, performs extraction when the stream is not initially atEnd, copies the integer and does not inspect stream status. Thus the predicted public flag is zero despite extraction failure. The input is ASCII; no ACP or numeric overflow behavior is involved.

## Scope and hashes

If assigned, keep admission to one requested flag and the exact full companion bytes `0x\r\n`. Source/test owners remain the native reader and focused SPRING test. No source/test edits, build, GUI, original process, or mutable REA state changed for this read-only analysis.

Current CP138 frozen source SHA-256: `B666F52FE00A0F59C60BE1E64746098EC66E5130E254DAFD583C9B54B34A63FF`.
Current CP138 frozen test SHA-256: `E918760E41613538B82D7141E40ABB27C127D0203C6CD914382A59E80CDB81CC`.
