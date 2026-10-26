# CP120 independent review: CP119 SPRING page Save

## Result

No byte-order or failure-branch discrepancy was found between the frozen candidate path and the retained original contracts for the qualified positive-count profile. This is a static source/test comparison, not fresh original parity.

## Primary `SPRING.INI`

The candidate `applyExtra3D()` builds a QString list as: count, LF; for each row, first object, LF, first value, second value, second object, direction[0], LF, direction[1], direction[2], LF. The `Extra3DIo::writeStringList()` backend appends TAB after every non-LF item and then CRLF. This matches `FUN_1410db030` line-by-line and `FUN_1410da9e0`: the separators after direction[0] and direction[2] are the exact two per-row LFs (with another after the first object), not after the second object. The one-row and two-row expected raw strings in `tests/s/test_spring_page_io_roundtrip.cpp` match CP115/CP116 byte contracts. The general-record expected output also preserves all tab/LF positions while applying `QString::number(..., 'g', 6)` formatting.

The flag is omitted from the primary row list and accumulated in a separate ordered companion list, matching the native record offset `+0x38` field and `FUN_1410db030`.

## Companion flags and failure ordering

The candidate computes `flagged = any(row.flag != 0)`, writes `SPRING.INI` first, and branches on the primary open result exactly as the retained code does:

- All flags zero: readable-delete `SPRING2.INI` is attempted after the primary attempt even if the primary open fails; delete result is ignored.
- Any flag nonzero and primary open fails: companion write is skipped, and an existing companion remains untouched.
- Any flag nonzero and primary open succeeds: ordered decimal flags are written to `SPRING2.INI`; a companion open failure does not roll back primary output.

`test_spring_page_io_roundtrip.cpp` asserts these sequences with injected I/O and retained-file locks. It covers zero-flag primary denial followed by companion removal, nonzero primary denial leaving the old companion, companion denial after successful primary, and the raw `[0, -7]`, `[1, 0]`, and selected `-1` normalization cases. The exact mixed companion output is `0\t-7\t\r\n`; the exact `[1,0]` output is `1\t0\t\r\n`.

The native `FUN_1410da9e0` return is based on QFile open success; close has no checked return, and QTextStream status is not inspected. Candidate `writeStringList()` likewise returns after successful open/close without representing later write failure. The page Save caller ignores the native SPRING helper result. Candidate `applyExtra3D()` therefore keeps the outward `Completed` result while tests verify side effects and order; no rollback or false short-write signal is introduced.

The original outer Save path separately removes both spring sidecars when the checkbox is off or row count is below one. Candidate absence of `form.spring` removes both files in that represented branch. The helper's count-zero early return is not reached by the native outer caller when count is below one.

## Limits

The supported candidate page profile uses its qualified label/selection projections and bounded parsed rows. This review does not establish native GUI item population, arbitrary combo contents, invalid pointers, unsupported snapshots, or fresh original runtime equality. No source, build, GUI, mutable REA state, or test execution changed in this review.
