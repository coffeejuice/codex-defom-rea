# CP73: isolated B122 geometry-variable parser

## Result

Added a pure, bounded parser for exactly one retained B122 `Workpiece-Geometry` record and its one top-level operation mapping. The parser returns a value record containing variable id/name/dimension/sample count; geometry object/operation/morph metadata; upper/lower defined flags; nominal/upper/lower GEO path strings; presence of the two empty correspondence elements; and the independent top-level operation id/name binding.

The parser rejects unrecognized shape with `M2M_E_UNSUPPORTED`. It is not called by `readInputTable`, `readLazyStudy`, `parseSubProject`, `Study`, `Var`, Save, or any public dispatch. The three GEO paths are references in the returned value only; the helper performs no filesystem reads or writes and owns no geometry payload.

## Direct fixture and malformed tests

The positive test reads the retained fixture in binary/read-only mode from `M2M_ORACLE_B/122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`, passes only its `DoeVariables` and `Operations` elements to the helper, and asserts the exact one-record fields and operation mapping. It additionally checks that the three referenced historical files exist and are each 41,410 bytes; it does not open, copy, or generate them.

Malformed parser-mechanics tests use freshly parsed in-memory copies and cover duplicate variable, a missing required GEO path attribute, a mismatched top-level variable binding, and an unexpected geometry child. These are bounded parser rejection checks, not claims that the original application validates malformed files the same way.

## Static preflight and limitations

- The B122 fixture path and test's relative include path resolve.
- All owned source/test files have final newlines and no trailing whitespace.
- No source shared with the B121 TLOC prefix (`doe_study.hpp/.cpp`, common `Var`, public reader/writer) was modified.
- No CMake file was changed. Existing CMake discovers `src/**/*.cpp` and `tests/doe/test_*.cpp` with configured globbing; root must reconfigure/build before executing this test.
- No compiler, build, test, GUI, original runtime, or mutable REA was used.

The current public owner guard and nonempty `DoeVariables` / `Operations` guards remain unchanged. The result is only a parser value projection. Shared `Var` ownership, `Study::geometryVars(opId,objId)`, GEO source/copy ordering, B122's GUI edit actions, and the other DOE output files remain separate work.

## Owned files and SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/geometry_variable_prefix.hpp` | `20d6ca1624a52ff3f35a2bfe22d8525fdadd57193776094a2f64025109a024e2` |
| `source/apps/moproj2mst/src/doe/geometry_variable_prefix.cpp` | `3a0acebed323c477d695a0049d6587d34a102eacddfb71608603892f48fb4b92` |
| `source/apps/moproj2mst/tests/doe/test_b122_geometry_variable_prefix.cpp` | `81e26ae62c94bd427a8192e24bd6c75a06b188aaaf8057be4de29fd408b59d6e` |

The original fixture remains read-only. Its input XML hash is `79d875540a7c5e84442e183be527f02fa3eb345e5c25900268b8478030c3bf9b`; each of the three GEO payloads has SHA-256 `96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526`.
