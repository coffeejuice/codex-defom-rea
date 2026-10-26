# CP131 independent review: SPRING2 `0xA0` bounded cut

The exact `0xA0` one-flag admission has the expected numeric value and cursor for the retained Qt integer-extraction behavior. Qt case-folds the hexadecimal prefix/digits, consumes the valid digits `A0` (10 × 16 + 0 = 160), and leaves the first following non-hex character unread. For `0xA0\r\n`, that is CR at byte offset 4. The candidate advances to `start + 4`, matching that next unread byte; the SPRING2 caller then stops after its one requested flag.

## Scope and neighbor guards

The new condition is limited to the one-row-qualified caller and exact lowercase bytes `0xA0` followed by EOF or ASCII whitespace. The tests verify flag 160 and unchanged bytes in both SPRING files. `0xA1`, `0xA00`, and `0xA0G` remain unsupported in the one-row list; `0xA0` remains unsupported in the two-row list. This is a conservative candidate profile, not a claim that Qt rejects the neighbors: the retained Qt parser supports case-folded hexadecimal and can successfully return an accumulated value before leaving an invalid following character unread. The implementation receipt appropriately calls those restrictions scope guards.

No broader numeric parser behavior is introduced. The branch does not change the existing `0xA`/`0XA` special case, row count, count-driven iteration, default-zero flags, non-ASCII/BOM handling, output assignment, or file contents. The one-row condition is passed only for `count == 1 && i == 0`; a second requested extraction would need a fuller stream-cursor model and remains guarded.

## Evidence and limits

The receipt is grounded in the byte-verified Qt 4 `getInteger`, `operator>>(int&)`, and `ungetChar` captures bound to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. That establishes the extraction mechanics statically. The SPRING2 helper's copy-result/ignore-status/stop-at-count behavior is likewise a static call-path conclusion; it is not an original-process differential. No build or runtime test was performed in this review.

No discrepancy found in the narrow cut. Keep the finite spelling/one-row admission and avoid describing the neighbor guards as native invalidity or claiming complete Qt stream emulation.
