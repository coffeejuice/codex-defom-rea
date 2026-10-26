# CP132 Type3 optimizer tag classifier implementation

## Change

Added private pure helper `classifyType3OptimizerRowTag(std::int32_t)` to the existing Type3 scalar-state module. It maps the recovered `DefOptModel +0xa0` table result without constructing an optimizer or changing the `NUM_CON` parser guard:

- result `0`: tags `0x3f2`, `0x3f4`, `0x438`;
- result `1`: `0x3fd–0x3ff`, `0x406`, `0x410`, `0x41f–0x421`, `0x424`, `0x442–0x444`, `0x44c–0x44e`;
- result `2`: `0x439`;
- result `-1`: all other int32 tags.

Implementation uses explicit ranges and returns `-1` for tags below and above the mapped interval, matching the native subtract/unsigned-bound/table-default path.

## Tests

Added two cases to `test_type3_optimizer_scalar_prefix.cpp`:

1. Exhaustively compares all 93 values from `0x3f2` through `0x44e` against the byte-derived output table captured from the original PE. Raw table value 3 is represented as final native result `-1`, because its jump-table target preserves the initialized -1 result.
2. Checks `INT32_MIN`, -1, `0x3f1`, `0x44f`, and `INT32_MAX` all return -1.

The existing prepared-row append test still passes the classifier result as a prepared value; no parser invocation, QRegExp object, or public Type3 path is wired.

## Evidence and limits

Evidence is the original PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, `evidence/gui_mo/s1/vtables.tsv` (`DefOptModel +0xa0 -> 0x14091f0b0`), bounded dumpbin body `0x14091f0b0–0x14091f120`, and directly read table bytes at VA `0x14091f134` / file offset `0x91e534`. The exact map is summarized in `analysis/resume132-type3-regex-next.md`.

This is only the value classifier. It does not reproduce native virtual dispatch, input parsing, row allocation/ownership, Qt cleanup, native error behavior, or public ShapeOptim/Type3 support. File tokenization and `.OPT` loading remain disconnected.

## Static preflight

The test retains the `m2m-doe` label, Testkit include, identifier-form test cases, `testkit::run_all()` entry point, and contains no C `assert`. The source-relative private include resolves, and CMake's existing globs discover the `.cpp` and test. A local static script compared the test's 93-entry oracle to the original PE byte table; all entries matched. Whitespace checks passed. No build/test execution was run; root owns validation.
