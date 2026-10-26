# CP160 SPRING2 recognized sign implementation

## Change

Implemented the bounded one/two-row recognized ASCII sign/no-digit transition in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`. When the current byte is `+` or `-` and no digit follows immediately, the converter returns zero and consumes exactly the sign. A following nondigit remains unread for the next requested extraction. Comma is explicitly excluded from this branch and continues to refuse under the existing candidate profile. Existing q/? no-advance behavior, numeric parsing, and BOM decoding paths were not modified.

Updated `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: all four ordered ASCII sign pairs are exercised against digits 1 through 9 with EOF and CRLF endings; count-one lone signs, sign-tab-digit continuation, and repeated-sign cases are covered. The previous one-row `++1` refusal entry was removed because this branch now accepts it as a zero-valued first flag. Added `+,2`/`-,2` candidate refusal controls and BOM-bearing UTF-16/UTF-32 U+2212/U+FF0B/U+FF0D controls. Accepted/refused cases check both files remain byte-identical.

These are converter candidate behaviors and guards; refusal controls do not claim that the original application rejects those spellings.

## Preflight

The test is already registered by the `tests/*` CMake discovery, links `testkit`, and receives the project include paths. Its direct headers resolve relative to the test file or through those target include paths (`../support/test_paths.hpp`, `testkit/check.hpp`, `m2m/common.hpp`, `m2m/engine/simcontrol300_io.hpp`). Assertions use Testkit `CHECK`/`REQUIRE`; no C `assert` or unsupported `REQUIRE_EQ` is present. No build, test, GUI control, or mutable REA operation was run.

`git diff --check` passed.

## Source release

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` SHA-256: `26CFABD97925362007D8FEC1EFBFFF6880C975E765B8060F123D3B51B014DE96`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` SHA-256: `06C966941F89CB98995D910F4F5592C2F448792C1B5FF548B7B8AA33A40C7B84`

CP160 owned source files are released for the root-owned focused build, tests, and freeze. No status-bit or attempt-count API was added; sticky status is exercised only through the second extraction's value projection.
