# CP143 private Type3 REGIONS sequence helper

Added `appendType3RegionRows` over an already-converted `int32` count and an explicit span of raw row QStrings. It consumes at most the declared count, applies CP141's `deriveType3RegionValue` to each raw line, and appends `{0, value}` with the existing prepared-row helper in order. The result distinguishes a satisfied count from exhausted supplied input and reports the number consumed, so a caller can continue top-level dispatch at the first unconsumed line. A zero or negative count consumes no rows. Earlier appended rows and unrelated state are retained if the supplied span ends before a positive count is reached.

The helper deliberately does not split or read file lines, parse the count, model QFile/QTextStream, create an optimizer owner, change parser/public admission, or classify malformed input as a native failure. Its `RowCountUnrepresentable` outcome is a candidate guard protecting the existing prepared append function's int32 return-index domain; no huge allocation test was attempted. `NUM_CON` remains in its existing bounded path.

The focused tests cover ordered transformed values, zero/negative counts, early input end with prior state retained, and leaving a following `ENDDATA` line unconsumed for the caller. The already-existing CP141 helper tests separately cover delimiter order, UTF-16 preservation, and QString null/empty behavior.

Static preflight: `git diff --check` passed for the three owned files. Recursive source and test globs include the new files; no CMake/shared-header edits are needed. No build or test was run. No GUI, original process, or mutable REA action occurred.

SHA-256:

- `source/apps/moproj2mst/src/ops/type3_region_sequence.hpp`: `6790D7689F65A3493A9829F4D91DDD6C98F720A1ED5B0A695F1EFD80D90E0797`
- `source/apps/moproj2mst/src/ops/type3_region_sequence.cpp`: `FFA5C09816B02E33859D617BC8CD84CDB9838EADD86209474331D7FD7E1D5EB1`
- `source/apps/moproj2mst/tests/doe/test_type3_region_sequence.cpp`: `8789E4FA618860E1A292A474535E8054E68B481EEBF6C98608A1884CC82C34A4`
