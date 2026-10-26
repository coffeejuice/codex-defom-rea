# CP143 review: private Type3 REGIONS sequence

## Result

The new `appendType3RegionRows` helper matches the assigned in-memory sequence contract: it consumes at most the positive declared count from already-decoded raw row strings, applies the CP141 rewrite to each whole raw line, and appends value rows in encounter order. Nonpositive counts consume nothing; short supplied input returns `InputEnded` after preserving already-appended rows. The helper has no file, tokenizer, optimizer owner, or public loader side effect. I found no blocking mismatch in source or focused tests.

## Review against the source contract

`analysis/resume143-type3-file-parser.md` explicitly distinguishes a whole `QTextStream::readLine()` QString from its separately split key/value fields. `type3_region_sequence.cpp` accepts `span<const qt::String>` named `rawRows`, passes `rawRows[consumed]` directly to `deriveType3RegionValue`, and never tokenizes or joins fields. This preserves the required raw-row binding. The reused CP141 helper applies first-`#` prefix removal, last `/` prefix removal, last backslash prefix removal, and last-dot truncation in that order; those operations are reviewed in `analysis/resume141-type3-region-value-review.md` and remain separated from this count loop.

The loop continues only while `count > 0`, raw input remains, and consumed rows are fewer than the declared count. It increments `consumed` only after a prepared row is appended. It returns `InputEnded` only when a positive declared count remains unsatisfied, so count zero/negative and exact count satisfaction return `CountSatisfied`. Before append, it refuses when `state.regionRows.size()` is at least `INT32_MAX`; this prevents the prepared-row helper's size-to-`int32_t` return-index narrowing from overflowing. Earlier region rows and unrelated state are left intact. The early-input-end fixture confirms the prior scalar value and both successfully appended rows remain.

`appendType3RegionPreparedRow` appends `{0,value}` and returns the new last index, matching the stated prepared-row shape. The focused tests check combined delimiter order on raw lines (`discard#folder/first.ext` → `first`, `root\\second.part.opt` → `second.part`), declared-count truncation while leaving a following `ENDDATA` line for top-level dispatch, nonpositive counts, and early-input-end partial effects.

Static reference search finds `appendType3RegionRows` used only by the focused test and its implementation/declaration. `applyType3OptimizerScalarLines` remains separate and still refuses `REGIONS` row bodies; the loader path has no call to this helper and retains Type3's separate owner/optimizer guard. Therefore this helper/test does not admit a Type3 file or public ShapeOptim path.

## Boundaries and evidence limits

This validates behavior over supplied `qt::String` rows only. It does not establish QFile `Text` line translation, QTextStream decoding/locale, the discarded Type3 header read, stream EOF/error behavior, optimizer ownership, allocation failure, the native parser's return byte, or public Type3 admission. The function itself does not process `ENDDATA`; the test verifies that it leaves the next supplied raw line untouched for a separate dispatcher.

No build or tests were run as part of this static review. No source, GUI, original-process, or mutable REA state changed.

## Reviewed hashes

- Parser boundary: `analysis/resume143-type3-file-parser.md` — `455D56FED0541CF40768D3C018799A48D75A9D2F484D6E368CBC431E8FB1CB06`
- Sequence implementation: `source/apps/moproj2mst/src/ops/type3_region_sequence.cpp` — `FFA5C09816B02E33859D617BC8CD84CDB9838EADD86209474331D7FD7E1D5EB1`
- Sequence interface: `source/apps/moproj2mst/src/ops/type3_region_sequence.hpp` — `6790D7689F65A3493A9829F4D91DDD6C98F720A1ED5B0A695F1EFD80D90E0797`
- CP141 row transform: `source/apps/moproj2mst/src/ops/type3_region_value.cpp` — `8B33A6107A9EF88E21032E304CF709E950B137C2F4D43403B66D7997651EB7CC`
- Prepared-row append: `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.cpp` — `715033D53F0B265C49E6FB075F9C32BC4317BB4787A2C40D4BFD5F2A317E7EB6`
- Focused sequence test: `source/apps/moproj2mst/tests/doe/test_type3_region_sequence.cpp` — `8789E4FA618860E1A292A474535E8054E68B481EEBF6C98608A1884CC82C34A4`
- Existing scalar parser: `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_sequence.cpp` — `B4305AFF7678D0F082ED7602C2DB6E9562D6969D53868934CE8977FD17D20E28`
- Existing loader gate: `source/apps/moproj2mst/src/engine/shapeoptim_load.cpp` — `1428FD84E341CCBA66D1203243E1836A32A0292FFA44898D927378FAADCB1367`

Root owns build and integration validation.
