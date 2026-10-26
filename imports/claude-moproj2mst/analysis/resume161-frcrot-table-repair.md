# CP161 FRCROT table projection repair

Repaired the two blockers in the independent source review, touching only the assigned private helper and test. Mode-0 checks now inspect the actual consumption outputs (`destinations` and `scanReturnCodes`). The implementation rejects any observation vector whose length differs from the number of scan calls consumed, including extra entries after X or Y EOF; focused cases cover both tails.

Expanded coverage includes count 0, count 7 success, count 8 refusal, missing observations with allocation/zero-fill qualified, positive-count complete scans without the required trailing-line boundary, Y-EOF explicitly supported before its effects are inspected, and return-0 destination/return-code outputs. The existing full row order, partial writes, mode code, and allocation/code-shape guards remain covered.

Preflight: `<testkit/check.hpp>` exists; the quoted private header resolves relative to the test file; direct standard declarations include `<string>`, `<utility>`, and `<vector>`; assertions use supported Testkit macros; `main()` calls `testkit::run_all()`; no C `assert` exists. No build/test, GUI, live REA, or mutable database action was run, as instructed.

SHA-256:
- `frcrot_table_prefix.hpp`: `a155a0e468dea82bbe1148afb51ce491c0c0c042a25db096ae2448361f6e216c`
- `frcrot_table_prefix.cpp`: `b20c309704cf7e512e2bc0c57afaa8adaeadb550f4b90ddb24ea24663aa664a9`
- `test_frcrot_table_prefix.cpp`: `27b1dab1b68e0393af64ef4cff652dfd3fb559e92da79309ff14c6417e31ac5f`

Release: repaired source is ready for bounded independent re-review and the root integration gate. No existing pair helper, public header, CMake file, or unrelated path was edited.
