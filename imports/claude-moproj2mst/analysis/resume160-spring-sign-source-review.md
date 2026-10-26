# CP160 SPRING2 sign-category source review

## Review result

No source defect found in the released zero-digit sign transition. The implementation consumes exactly one ASCII `+`/`-` when no digit follows, returns zero, and leaves the next byte for the next requested extraction. It explicitly excludes comma, so the separate C-locale grouping branch is not captured as a sign failure. The implementation is restricted by `allowQQuestionCursor` to primary counts one and two; no public Session, writer dispatch, or shared interface was changed.

The test file has two coverage gaps against the accepted matrix: it does not positively test count-one sign-pair inputs such as `++1
` -> `[0]`, and it does not explicitly refuse the same sign category with a count-three primary. Current count-two all-pairs coverage exercises the sticky-status/result path, but does not replace those gate tests. These are test gaps, not a demonstrated production mismatch.

## Source checks

The relevant branch in `readSpringFlagInt` checks the recognized sign and zero-digit condition, then advances `cursor` from `start` to `start + 1`. It refuses to take that path when the next byte is comma. The caller's outer flag loop requests at most `count` values; count-one/count-two enable the sign transition, while larger counts do not. Existing q/? handling remains a separate no-advance path, so a sign followed by q/? can compose only when the next requested extraction reaches that existing marker profile. The previous negative modulo-64 branch and ASCII BOM decoder remain present and unchanged in the released source inspected here.

The test covers all four ordered sign pairs over digits 1–9, with EOF and CRLF termination, under a two-row primary. It checks `[0, signed second digit]` and file-byte preservation. It also covers lone signs under count one, `+++2`/`---2`, sign-tab-digit continuation, comma candidate refusals, and UTF-16/32 BOM-encoded non-ASCII sign controls. The plan's count-one `++1` acceptance is described in the receipt as a removed refusal, but no acceptance assertion now covers it. Likewise, no count-three sign-specific refusal assertion is present. Add those two cases before treating the matrix as complete.

## Immutable evidence and projection

The source basis remains the QtCore `getInteger`, `operator>>(int&)`, and `atEnd()` captures bound to QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, plus the original `FUN_1410da690` loop bound to executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The reviewed sign/EOF/sticky-status rationale is recorded in `analysis/resume160-spring-category-plan-review.md`.

This review checks source and test projections only; no build/CTest, original execution, GUI, or mutable REA action was performed. The tests cannot inspect Qt's internal status value directly, so they establish only the count-limited flag output and cursor consequences that affect the returned values.

## Frozen artifacts inspected

- `analysis/resume160-spring-sign-implementation.md`.
- Reader source SHA-256: `26CFABD97925362007D8FEC1EFBFFF6880C975E765B8060F123D3B51B014DE96`.
- Focused test SHA-256: `06C966941F89CB98995D910F4F5592C2F448792C1B5FF548B7B8AA33A40C7B84`.
- `analysis/resume160-spring-category-plan-review.md`.

No source or test file was modified by this read-only review.
