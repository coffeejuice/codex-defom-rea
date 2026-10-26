# CP132 independent review: two-row SPRING2 `0B2` retry

The repeated-failure cursor model and predicted values are consistent with the retained Qt/parser evidence for the exact fixture `SPRING2.INI = "0B2\r\n"` with a two-row primary file. The first failed binary extraction rolls the entire `0B2` prefix back, leaves unread bytes so `atEnd()` stays false, and writes zero. The helper increments its requested-row index despite stream status and performs a second extraction at the same bytes; sticky status does not prevent the getter call. That second extraction also writes zero. At count two the helper stops, ignores stream status, and returns success, yielding `[0, 0]`. This is a static combination of PE-verified Qt mechanics and the retained caller decompile, not an original-process differential.

## Cursor and current candidate behavior

The candidate’s exact `0B2` branch returns zero and assigns `cursor = start`, which preserves the unread prefix. The primary companion loop then invokes it again for the second slot when count is two. The test checks successful row construction, both zero flags, and byte-for-byte preservation of both files. This matches the two-attempt sequence.

## Admission scope issue

The implementation currently calls `readSpringFlagInt(..., count == 1 && i == 0, count == 2)`. Thus `allowTwoRowBinaryRetry` is true for every extraction in every two-row companion file, and the exact `0B2` branch accepts that token wherever it appears. This is broader than the receipt’s promised exact two-row companion fixture. For example, a two-row companion with an ordinary first flag and `0B2` as the second token is now admitted by the new branch; a companion with another token after a failed first `0B2` may also be accepted while the second extraction repeats the first token and ignores the suffix. Those combinations can have sensible Qt outcomes, but they were not part of this finite source-qualified cut and should not be silently claimed by the current test.

To keep the implementation aligned with the narrow receipt, gate the special retry on the complete companion byte string being exactly `0B2\r\n` (or another explicitly enumerated line-ending form only after evidence) together with primary count two, and then permit both extraction attempts over that one frozen input. An equally explicit per-call state can require the first failed extraction and unchanged cursor before enabling the second call, but that still needs a whole-input/trailing-token boundary if the advertised profile is the exact fixture. Keep count-three, alternate invalid digits, bare prefixes, and mixed companion tokens unsupported.

## Broader guard review

The one-row `0B2` case is separately bounded by `allowSingleRowQualifiedToken`. Count-three primary input remains outside the new branch and is tested as unsupported. The reader still initializes flags to zero, preserves files, and keeps parser/public integration unchanged. The issue is only that the new two-row predicate is based on row count rather than the exact two-row fixture specified by the evidence report.

No source edits or tests were run during this review. The retained source and test are candidate evidence only; root should decide whether to narrow the gate before integration validation.
