# CP135 private Type3 scalar sequence driver

Added a private in-memory driver that tokenizes each supplied BMP `qt::String` line with the recovered `\\s+`/SkipEmptyParts helper and applies the existing scalar dispatcher in order. It stops on ENDDATA, surrogate input, or the existing row-body guard and returns the stop kind plus the next/unapplied line index. Earlier scalar writes remain in state if a later line reaches a guard. The API accepts an explicit span of already-decoded lines; it does not open files, model QTextStream, instantiate a native optimizer, parse NUM_CON/REGIONS rows, or connect public Type3/ShapeOptim behavior.

Added four focused Testkit cases for ordered scalar overwrites with skipped/unknown lines, ENDDATA stopping before a later mutation, earlier scalar retention when a row key is guarded, and surrogate refusal after a prior effect. These cases specify the private in-memory prefix only; no original parser result/status is assigned to the helper outcomes.

CMake registration is not needed: `m2m_core_srcs` recursively glob `src/*.cpp`, and tier-1 tests glob `tests/<packet>/test_*.cpp` with `CONFIGURE_DEPENDS`. The added source and test therefore enter the existing core/test target set after reconfiguration.

## Static preflight and limits

`git diff --check` passed. Include paths resolve by source-relative layout: the implementation includes its sibling header, and the test includes `../../src/ops/type3_optimizer_scalar_sequence.hpp`. Test source uses the existing `m2m-doe` label and Testkit `main`. No CMake, existing source, public API, file reader, build, test run, GUI, original process, or mutable REA database was changed or used. Root owns compilation and focused/full validation.

The driver intentionally does not model the native parser's QFile open/close, ReadOnly|Text decoding, separate first-line trim/conversion, parser return-false behavior, read failures, or Type3 row allocations/ownership. It is not evidence of file-level parser parity.

SHA-256:

- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_sequence.hpp`: `B2A1605AA4851167E33E6E9F059F294FC73F06D433E9C6B6C9CD66402BCBD7AD`
- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_sequence.cpp`: `AB3251E3F8534AD65C0D25919E9415457FE18C7E91224F8AE1AF7156835610A6`
- `source/apps/moproj2mst/tests/doe/test_type3_optimizer_scalar_sequence.cpp`: `970C31D372CC0A86AEAD0231557DB9BAEEAB548119266CBDCABC303C3F9F5B25`

