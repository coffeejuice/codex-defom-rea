# CP103 old DOE direct-Keyword expectation reconciliation

Updated only the four assigned DOE test files. These expectations now reflect the CP103 reached-prefix contract: direct `Keyword` entry refuses, but prior Variable/header/Description effects (including the current row's owner creation) persist. The enclosing DoeVariables index/link finalizer and outer optional-ID repair remain unreached.

## Adjustments

- `test_variable_unique_append.cpp`: the fresh C row before Keyword is retained with header number 9, sample count 5, index -1, and selected index 2; prior B Description remains.
- `test_variable_duplicate.cpp`: after initial A/B and duplicate A setters, B receives number 9 and Description `committed`; the subsequent fresh C row is also retained with number 10 and index -1. Selected index is 2. The test's independent mixed Geometry/foreign-owner preappend guards were left unchanged.
- `test_variable_optional_id.cpp`: B with missing varId is retained at constructor number 0 and index -1; A's earlier Description persists. Outer repair remains unreached. Invalid present-ID guard coverage is unchanged.
- `test_variable_optional_dimension.cpp`: B is retained with default dimension 3, constructor number 0, index -1; A state remains and selection reaches index 1. Invalid dimension guard coverage is unchanged.

No production source, fixture, CMake or other test was edited. No build/tests, application/GUI, or mutable REA action was performed. This updates candidate expectation coverage only; it makes no original-parity claim.

## Freeze hashes

SHA-256:

- `source/apps/moproj2mst/tests/doe/test_variable_unique_append.cpp`: `F4F847087934873A88BDA226FC24873996304FFFEECCDEB889791112394FCFFD`
- `source/apps/moproj2mst/tests/doe/test_variable_duplicate.cpp`: `B628C23EA5B0DB295B5178E423E144C2B672847CABEEF6DB2A1180A1187A917A`
- `source/apps/moproj2mst/tests/doe/test_variable_optional_id.cpp`: `96A8B78CD8C6C9B21C5926CCEB7F7626CA73EAFFE988212C07789BC1798EB000`
- `source/apps/moproj2mst/tests/doe/test_variable_optional_dimension.cpp`: `10CFB18323D1092E61892047282C218EC9047B7E4CE49ADA2FAA93991014E86C`