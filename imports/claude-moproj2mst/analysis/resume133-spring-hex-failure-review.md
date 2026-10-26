# CP133 SPRING2 `0xG` retry review

## Review result

No actionable defect found in the frozen bounded implementation. The new admission is restricted to a two-row request with the whole companion byte string exactly `0xG\r\n`; the helper represents the first read at offset 0 (value zero, cursor 2) and the retry at the unread `G` offset 2 (value zero, cursor remains 2). Broader neighboring malformed or whitespace/token arrangements remain refused.

## Source and test checks

- `readSprings` sets `exactTwoRowInvalidHexRetryProfile` only when `count == 2 && flagBytes == "0xG\r\n"` (`src/engine/simcontrol300_native_io.cpp`, around line 481). It then uses that flag for both requested row reads.
- The first extraction recognizes `0xG` only for the exact qualified profile, stores zero, and sets cursor to `start + 2`, matching native hex failure consuming `0x` and ungetting only `G`.
- The second extraction is admitted only when the whole bytes remain exactly `0xG\r\n` and cursor is 2. It stores zero and leaves cursor at 2. The row loop performs the second requested extraction even though the first extraction's modeled status is nonzero, then stops at declared count. It does not parse or claim general persistent Qt stream state.
- Test `test_simcontrol300_spring_reader.cpp` now accepts the exact two-row case, asserts two zero flags and unchanged primary/companion bytes, and keeps cases `1 0xG`, `0xG 1`, `0xG 0xG`, leading/trailing spaces and other token arrangements as explicit refusals. It separately retains the three-row refusal and one-row malformed-hex behavior.

## Evidence limits

The static CP133 contract (`analysis/resume133-spring-next.md`) qualifies the expected values/cursor/status from retained Qt PE and QtCore4 evidence; it explicitly says no original-process differential was performed. This is therefore a candidate approximation of the exact two-read value/cursor behavior, not a general Qt parser or full SPRING2 Save acceptance claim. Test/build execution was not part of this review.

Frozen implementation receipt: `analysis/resume133-spring-hex-failure.md` (SHA-256 `0dcd6f5e...`, as supplied by root). Reviewed source SHA-256 `0b4832377e1eb03a5c68aeef15a4519f1d9844d1193db115857d1ba087a79b8b`; test SHA-256 `c9a4b3fa1cf51ec77b0c02eb21b942d0a7826c450d419e2ef0e535922c1496d5`.
