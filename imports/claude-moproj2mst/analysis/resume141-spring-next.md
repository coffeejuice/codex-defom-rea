# CP141 next SPRING2 frontier: uppercase bare hex prefix at EOF

## Selected exact one-row case

The next bounded spelling is the exact companion content `0X` (two bytes, no newline) for a one-row SPRING record. Qt calls QChar lowercasing before base-prefix selection, so `X` is recognized as the hex prefix character. With no following digit or delimiter, the getter consumes both prefix bytes, fails at EOF, returns zero, and leaves next position at offset 2/EOF. The fresh integer operator status is 1 (EOF-side failure). The original SPRING2 helper attempts extraction because the file initially contains bytes, copies zero, ignores status and returns true after its one requested flag. Predicted public result: flag zero.

This differs from the admitted uppercase `0X\r\n` form, where CR remains unread and failure status is non-EOF. The candidate currently admits lowercase `0x` at EOF and uppercase `0X` followed by CRLF, but not this uppercase EOF spelling. Proposed one-row test: primary one-row ASCII record and exact companion bytes `0X`; assert successful read, flag zero and byte preservation. Keep count-two, uppercase prefix plus other suffixes, whitespace, and other malformed forms guarded.

## Retained evidence

`analysis/resume83-sw2sp-parser-independent-review.md` records PE-byte-verified Qt 4 `getInteger`, `operator>>(int&)`, `atEnd()` and `ungetChar` regions from QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, paired with original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The report locates `QChar::toLower()` before automatic base-prefix selection at `0x670c53bf` and records lowercase bare `0x` at EOF as conversion failure, offset 2/EOF, fresh status 1. Therefore uppercase `0X` takes the same EOF branch after the source-qualified case fold. This uppercase token is a deduction from those retained branches, not a separate table row or original-process differential.

The `FUN_1410da690` call path summarized by `analysis/resume119-spring2-normal-path.md` checks the input stream before extraction, copies the operator destination and ignores conversion status. It returns true after open success; with initial bytes `0X`, one extraction occurs and produces zero. The public flag is statically predicted as zero. Input is ASCII; no ACP, overflow, or narrowing issue is involved.

## Candidate boundary

If assigned, keep admission to one requested flag and exact full companion bytes `0X`. Do not generalize case-folded incomplete prefixes or multi-row behavior. No source/test edits, build, GUI, original-process execution, or mutable REA state changed for this read-only report.
