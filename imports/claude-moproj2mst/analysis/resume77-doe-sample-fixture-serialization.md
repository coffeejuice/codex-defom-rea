# CP77 mode-zero sample serializer fixture test receipt

## Result

Added one isolated DOE test file that reads retained B121/B122 `DEF_DOE.PAR` and `Samples_LHS.par` files in binary mode, parses each as mode 0 with the existing `parseSampleText`, and plans current then LHS writes through `writeSampleTable` into an in-memory `FileEffects`. It directly compares both planned payloads against the independently read fixture bytes. Both pairs are verified to have 100 parsed rows and are byte-identical within their own case. Candidate output destinations are synthetic strings; the test does not write or modify oracle files.

Expected fixture receipts:

| Case | Bytes per file | SHA-256 per file |
|---|---:|---|
| B121 | 1,396 | `1B6887361A11FB31DDD728AB10C4C35F6405A5399A849E4F6D2B4285D0C50791` |
| B122 | 1,389 | `770D49EAEDED318B08B88EEC289FF26F133BF874F098A795E7E757A61F20D56F` |

The test checks two ordered writes (`DEF_DOE.PAR`, then `Samples_LHS.par`) and no planned deletions, directory creation, or renames. It does not create or infer FF/USR outputs, sample-design generation, or validity flags for absent files.

## Source/preflight boundary

The test exercises the already existing `parseSampleText` and `writeSampleTable` APIs only. `qt::String::latin1` is used because the retained PAR fixtures contain ASCII bytes. The exact comparison covers the mode header, row numbering, numeric formatting, trailing tabs and CRLF behavior; the source formatter's six-significant-digit numeric rule is not treated as generic locale-independent evidence.

Static preflight: Testkit main and `REQUIRE`/`CHECK` are present; no C `assert`; quoted includes resolve through the test file's directory or configured include roots; source/test CMake globs include this `test_*.cpp` file. It ends with a newline and has no trailing whitespace. No build or test executable was run.

This is a detached serializer comparison. Native Type2 Save places `DoeTableRefresh` after InputTable XML and before the current/PAR sample writers (`resume63-doe-public-save-order.md`, `0x140099770` -> `0x1400997b2` -> fixed modes). Keep the public path blocked until that actual ordered notification owner is qualified. `SetOprHasDoe` remains a distinct B121 variable-reader dependency; this test supplies sample rows directly and does not establish public project loading or Save.

## File hash

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/doe/test_doe_sample_fixture_serialization.cpp` | `3843ADBD45050E46D7C2963D3B2028F180CF3627B7599D7AA878BCBF9409BF55` |

Only this test and this receipt were added. No production source, CMake file, public guard, GUI, build, full test suite, or mutable REA state was changed or used.
