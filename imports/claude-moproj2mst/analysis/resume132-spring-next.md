# CP132 next SPRING2 branch: bounded two-row binary rollback retry

## Selected continuation

A useful next step is the exact two-row companion fixture `0B2\r\n` after the one-row invalid-binary case has been admitted. On the first Qt extraction, automatic-base parsing sees the binary prefix and invalid first binary digit `2`; it fails, stores zero, and restores the entire prefix to the stream. Because the restored prefix is still unread, `atEnd()` remains false. The original two-row helper therefore attempts its second extraction at the same bytes. `operator>>(int&)` calls the getter despite the already-set conversion status; it fails again, stores zero, and preserves the existing nonzero status. The helper increments its row index each time, stops at the requested count of two, ignores status and returns true. The predicted flags are `[0, 0]`.

The candidate currently admits `0B2` only under its one-row gate and rejects the two-row form. A focused test should use a valid two-row ASCII `SPRING.INI` and companion `0B2\r\n`, assert successful read with both flags zero, and verify both input files are byte-for-byte unchanged. Keep count-three, alternate invalid digits, bare `0B`, mixed tokens, status reporting, and other malformed inputs guarded; this is the exact repeated-token case only.

## Retained evidence

The Qt mechanism is documented in `analysis/resume83-sw2sp-parser-independent-review.md`, which independently recaptured and PE-byte-verified Qt 4 `getInteger`, `operator>>(int&)`, `ungetChar`, and `atEnd()` regions. It binds to original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The one-value table records `0b2` as failure, destination zero, next offset 0 with prefix restored, status 2. The review also states that the integer operator's getter call is based on buffered/device input, not prior conversion status, and only writes a new nonzero status when no prior status exists. `atEnd()` examines buffer/device end state, not conversion status, so restored unread bytes prevent loop termination.

Original caller behavior is retained in `analysis/resume119-spring2-normal-path.md` and `analysis/wave2-save-diffs-300-support-1410da690.c`: `FUN_1410da690` initializes all slots to zero, loops while `!atEnd()` and index<count, extracts one int and increments the index regardless of stream status, then returns true after a successful open. For the two-row fixture this yields two zero slots. This is a static deduction combining the exact Qt state machine and helper decompile, not an original-process differential run. The fixture is ASCII and does not involve codec, ACP, numeric range, or narrowing questions.

## Candidate limits

Admit only the exact count-two `0B2` companion fixture, with the same primary count of two. This captures one repeated failure and sticky-status/cursor behavior but does not claim a general multi-read parser or arbitrary persistent-error loop. The original can repeatedly retry restored bytes, while this increment should remain closed to this exact two-attempt case. No source/test edits, build, GUI, original process, or mutable REA database activity occurred for this read-only report.
