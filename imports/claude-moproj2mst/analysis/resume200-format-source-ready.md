# CP200 RFORMAT source readiness audit

## Result

No new source change is justified by the historical Save DIFFs or the retained format evidence reviewed. The one concrete CP123 SPRING2 decimal-narrowing defect is already resolved in the current source and focused test, so it must not be reopened as a new task. The remaining defensible extension is blocked on a same-occurrence original `SW2SP.DAT` witness for an exact token outside the finite CP84 oracle, as CP196/171 specify.

## New check: CP123 boundary resolution

The retained original static evidence in `analysis/resume123-spring2-unsigned-review.md` establishes that positive decimal `4294967296` passes the original 64-bit getter and is narrowed by the `int&` stream operator to zero. CP123 had identified the candidate's previous refusal of that value as a precise defect.

The current `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` no longer has the old `uint32_t`-only rejection described in CP123. Current `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` includes an accepted one-row case for `4294967296` with expected flag zero (around lines 389–394); neighboring cases cover `4294967297` → 1 and `18446744073709551616` → 0. This resolves the prior narrowly identified gap in source and test. I did not run tests or build.

## Historical DIFF disposition

The current historical DIFF frontier remains the one summarized in `analysis/resume148-historical-diff-next.md` and rechecked in `analysis/resume196-rs55-diff-audit.md`: RS55's two `SimCtrl.KEY` bytes lack the process-bound editor mode/toggle witness, DOE after-action trees lack matched pre-state, and B98 is the already-qualified project-root path difference. These do not produce a new format/parser rule. B98 and RS55 are not proposed for reallocation here.

The nearby numeric, BOM/Unicode, and ShapeOptim path cuts are also closed at their established limits: current code routes the one-value `SW2SP.DAT` profile through `parseQt4Base0Int`; explicit SPRING2 BOM profiles and the recovered BMP digit category are implemented; rooted `studyFolder` construction and successful `mkpath` are implemented for the admitted owner. The false-`mkpath` modal continuation still lacks its same-occurrence delivery/dismissal trace. These boundaries are consistent with the CP171/196 frontier and do not justify extrapolation.

## Exact next witness

For an incremental `SW2SP.DAT` parser change, capture one exact ASCII spelling absent from the CP84 oracle at the original `FUN_1410da690` one-integer call. Bind original GUI/QtCore hashes, filename and requested count, fresh stream state, raw bytes, selected `getInteger` branch, extracted value and status, unread cursor/QChar, and caller destination/return. Only a new caller-visible result supplies a focused test case. Without that witness, the source-ready result is **no change**.

## Scope and read-only boundary

Read CP196 and CP171 format frontiers; CP165's prior mismatch audit; CP123's retained Qt static-evidence review; CP148's historical DIFF classification; CP196's RS55 DIFF audit; and current reader/test call sites. No source, test, fixture, build, GUI/app-control, original process, mutable REA, parent DMO, or Claude state was changed or run. This report is the sole write.
