# CP159 empty-operation reader implementation handoff

Implemented a separate strict parsed-empty profile in `keyword_reader_prefix.cpp`. Admission requires the existing exact scalar TLOC attributes and NominalData text plus a `KwOperations` node with no attributes and no raw children. The existing single-operation-id-1 path remains selected separately and still follows its notifier guard.

For the empty profile, the helper creates the actual empty operation vector, appends the canonical Keyword on the original-name owned HeaderVariableRecord, re-fetches that owner by the original name, sets selector `-1`, false function/enum flags, and exact nominal text, then throws at `0x140910abf`. The empty branch exits before any notifier access. Empty operation union preserves existing `Var.ops`.

Added `tests/doe/test_keyword_reader_empty_operations_prefix.cpp` with focused private-reader cases for the empty scalar prefix and preserved operation-1 guard. The test's A/B setup checks the Keyword lands on A while the modeled outer row effect remains on B. These are synthetic exploratory cases; they do not claim original observation, native allocation parity, lazy Type2 integration, or successful parse.

## Preflight

- Test uses `<testkit/check.hpp>`, registered `TEST_CASE`, `CHECK`, `CHECK_EQ`, and `REQUIRE`; no `REQUIRE_EQ` or C `assert`.
- Project C++ standard is 20. Private relative includes resolve from `tests/doe` to `src/doe`; project include roots supply `m2m/*` and `testkit/*`.
- No build or test was run, per assignment. Root owns CMake integration and focused validation.
- No GUI or mutable REA operation was performed.

## Source identities after this increment

- `keyword_reader_prefix.cpp`: `8FB6412E44EE3720C6740ABA04A8E1B63FF0AFAC9026C11EDB55644F3899A1F4`
- `keyword_reader_prefix.hpp`: `C14E6FA1DE5EC009F89271FA47B61B4D23C80271976F56CEE28CC9C6A431AC58`
- `keyword_constructor_record.hpp`: `039F049DD4E7C39560BB0313BA88C8ACF0F5ACDAAFCEFB16A11C612BF5B3593C`
- `test_keyword_reader_empty_operations_prefix.cpp`: `A46F991FAF0C98E099A74C4FFBFE910F77D4C40405E876BD11A35B003FED6C2B`

## Limits for integration

The new test calls the private reader helper directly and does not yet exercise the actual `ConversionSession`/Type2 lazy-reader route. In particular, the operation-1 test passes no notifier context and checks the legacy guard, not notifier entry. Root should add/adjust integration coverage and confirm compile/API details in the assigned grouped build. The function remains noreturn, so this candidate cannot advance to later sample/PAR/DOM work or imply parser success.
