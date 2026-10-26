# CP75 B122 geometry writer implementation receipt

## Result

Added a separate candidate-only writer for the retained B122 geometry `Study` profile. It validates the fixed common header, empty region/constraint state, one Study-owned geometry variable, empty unrelated scalar/geometry-file/item/link state, the full B122 typed geometry payload and separate top-level operation association. It then plans exactly one `DoeInputTable.XML` write in `engine::FileEffects`. It does not apply effects or call public `writeInputTable` / `saveInputTable`.

The output template reproduces the retained B122 fixture's exact 817-byte layout and Windows CRLF bytes. Attribute/child order is explicitly fixture-pinned; no general QDom attribute-order claim follows. The serializer does not create/copy the named GEO files, write PAR or temp.KWD, refresh, activate a subproject, or claim QFile success.

## Test and static preflight

The focused test reads the actual retained B122 fixture through configured `M2M_ORACLE_B` in binary mode, parses its DOE sections with the existing pure parser, binds the result into a fresh Study using the existing isolated binder, and directly compares planned output bytes to that independently reread 817-byte fixture. It checks the synthetic output path, one write, no delete/mkdir/rename or native file-effect flags, and refuses both a changed geometry filename and a changed header before adding effects.

Static preflight only: the new test has a Testkit entry point and supported `REQUIRE`/`CHECK` macros, no C `assert`; all three new files end in newline and have no trailing whitespace; new quoted test includes resolve relative to the test directory, and the writer's private header resolves relative to its source directory. Existing CMake globs include `src/*.cpp` recursively and `tests/*/test_*.cpp` with `CONFIGURE_DEPENDS`. The retained fixture currently measures 817 bytes and SHA-256 `79D875540A7C5E84442E183BE527F02FA3EB345E5C25900268B8478030C3BF9B`.

No build or test executable was run. Output equality is an asserted test expectation pending root's grouped build/test, not a passing test result. Public B122 reader/writer guards and DOE lifecycle remain unchanged.

## Files owned in this increment

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/geometry_variable_writer.hpp` | `89E3E41F2EE420107C48471C166707DE70B5F89A771A3A66CCCEC360A760EFD7` |
| `source/apps/moproj2mst/src/doe/geometry_variable_writer.cpp` | `A425514DCE9B4CC42F563F4CE04CD63F27CF03B4C8B05CAA2B00C23C388592D1` |
| `source/apps/moproj2mst/tests/doe/test_b122_geometry_variable_writer_prefix.cpp` | `EEFCB1935986CC8ED6CEF452D901512BFC9218F0FC7903CD5F5D78DCDDD770EE` |

This receipt is the only additional analysis file for this implementation lane. No public dispatch, existing source/test, CMake file, GEO/PAR/temp.KWD/refresh behavior, GUI or mutable REA state was changed or exercised.
