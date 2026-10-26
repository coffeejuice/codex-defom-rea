# CP95: SPRING positive count-one boundary and public dependency

## Decision

The original count-one reader path is statically bounded through one record, but it is not a safe public Extra3D increment. A successful positive spring read populates a QListWidget and invokes selection behavior; the candidate explicitly refuses the populated branch in `fillExtra3D`. Supporting only the numeric reader would therefore leave the reachable public Open path unsupported, and would not close Save serialization. Keep positive spring counts guarded until the spring item and selection callback are recovered.

The zero/nonpositive branch is already supported, including CP94's exact malformed-count case. This report does not propose another source change.

## Original positive count-one read

The retained original PE decompiles qualify this order:

1. `FUN_1410dab40` opens `SPRING.INI` ReadOnly and constructs one QTextStream.
2. `FUN_1410da800(...,1)` initializes the count destination to zero, checks `atEnd`, extracts an int if the initial stream is nonempty, and returns based on that initial check. On exact ASCII `1\r\n`, the extracted count is 1.
3. For positive count, the reader calls `FUN_14147489c(count << 2)` for an auxiliary int array, and `FUN_1410db4f0(...,count)` for the spring record storage. The result pointers are checked for zero. The helper body/allocator failure behavior is not retained here; do not extrapolate large-count or allocation-failure behavior from count one.
4. It invokes `FUN_1410da690(...,"SPRING2.INI",aux,count)`. That helper zero-initializes its outputs. If the optional file is missing or fails to open, `FUN_1410dab40` explicitly fills every auxiliary int with zero and continues; a missing `SPRING2.INI` therefore yields record flag 0 for count one.
5. For each record, the same open `SPRING.INI` stream is read in this order: one int to record offset 0; two doubles to offsets 8 and 16; one int to offset 24; three doubles to offsets 32, 40, and 48; the auxiliary int to offset 56. The loop stride is `0x40` bytes. This matches the field order and expected x64 layout of candidate `SpringRecord`.
6. `FUN_1410da800` and `FUN_1410da8f0` zero-initialize each requested destination group and return based on whether the stream was at end before extraction, not whether every requested token parsed. Each wrapper increments its extraction index after calling QTextStream. If a wrapper begins at end, the row is abandoned; if it begins with data but extraction later fails/ends, it can still report true with zero-filled fields. Status is not inspected by the spring reader. This establishes only the helper ordering and branch condition; a full malformed/partial-row output matrix is not claimed.

For a bounded valid row fixture, the bytes could be `1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n`, with `SPRING2.INI` absent. Static prediction: one record with firstObject 10, firstValue 0.5, secondValue 1.5, secondObject 20, direction [0,1,0], flag 0. This would test only the native reader output, not public Extra3D Open/Save.

## Public caller and candidate guard

Original `FUN_140ec8110` clears the spring QListWidget, calls `FUN_1410dab40`, and, on true, loops over the reported records. It copies each 0x40-byte record and passes it to `FUN_140eb2470` to construct/insert an item. It then derives the checkbox from list count, resets its selected-index field, selects row zero when any item exists, and calls `FUN_140ecdee0` on that item.

Original `FUN_140ec9040`'s spring Save path checks the checkbox and list count. For a populated list it invokes `FUN_140ecdee0` for the current item, allocates a count-by-`0x40` snapshot, copies 0x40 bytes from each item at offset `+0x30`, and passes the records to `FUN_1410db030`. That writer formats and writes `SPRING.INI`, and conditionally writes/removes `SPRING2.INI` based on per-record flags.

Candidate `fillExtra3D` currently returns no page when `readSprings` succeeds with nonempty rows. This is intentional containment for the unrecovered populated editor. Removing that guard after adding a count-one native reader would not be faithful: the candidate does not yet construct or select the native list item, and Save reads per-item 0x40-byte state plus checkbox/current-item state.

## Exact remaining dependency

No retained body-level qualification for the spring editor functions `FUN_140eb2470` and `FUN_140ecdee0` was found in the reviewed support captures. The next necessary evidence is a bounded original-PE capture/decompile of:

- `FUN_140eb2470`: item creation, text/metadata and record storage at item offset `+0x30`;
- `FUN_140ecdee0`: current-item selection to widget state/selected-index mapping, including its behavior when called during initial row selection;
- the corresponding widgets/checkbox and row-index state consumed by `FUN_140ec9040` before `FUN_1410db030` serializes the current list.

If the objective is actual Open→Save parity rather than an isolated reader test, an original-process comparison is also needed for a disposable one-record project after those static dependencies are recovered. Until then, keep positive counts unsupported. No source-only public count-one change is justified by the reader evidence.

## Evidence and scope

Primary retained files:

- `analysis/wave2-save-diffs-300-support-1410dab40.c`
- `analysis/wave2-save-diffs-300-support-1410da800.c`
- `analysis/wave2-save-diffs-300-support-1410da8f0.c`
- `analysis/wave2-save-diffs-300-support-1410da690.c`
- `analysis/wave2-save-diffs-300-support-140ec8110.c`
- `analysis/wave2-save-diffs-300-support-140ec9040.c`
- `analysis/wave2-save-diffs-300-support-1410db030.c`
- `analysis/wave2-save-diffs-300-extra3d-keywords.json`

The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. These are static PE/decompile results, not an original-process run.

Candidate guard references: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` `readSprings`; `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp` `fillExtra3D`; Save serialization: `analysis/wave2-save-diffs-300-support-140ec9040.c`.

No source/test edits, build, test run, GUI action, or mutable REA action occurred.

