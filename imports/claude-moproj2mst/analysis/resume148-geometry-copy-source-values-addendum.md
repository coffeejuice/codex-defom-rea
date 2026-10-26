# CP148 actual source-value test repair

The sentinel test now distinguishes a real 24-byte source copy from zero filling. Only `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` was modified; production Viewer is unchanged.

In `geometry_main_zero_sizing_copies_actual_source_once_into_owned_destination`, the final real sizer's after-lookup observation seeds the actual fresh step A allocation with three distinct exactly representable values `1.25, -2.5, 7.75`, while keeping destination sentinel values `11, 12, 13`. After the bounded copy, the test checks all three destination values, all source values, and byte equality of destination/expected, source/expected, and destination/source over `sizeof expectedSource` (three doubles, 24 bytes). Existing cached-reread assertions still verify destination21/22/23 is retained without repeating main sizing/copy/arrays/Stop. This adversarial test observation does not claim an original runtime capture.

Static preflight passed: delimiters, whitespace, Testkit outer macro arity, `<cstring>`, unique17 cases and single Testkit main. No build or test execution was performed. Focused target `m2m_mdt_geometry_a_producer`; CTest `m2m.mdt.geometry_a_producer`. Root owns rebuilt snapshot, focused and full validation.

Frozen release:
- Test SHA-256 `0e17210a2c19f4ef2ca22b4787496b430fea869fbfa381dcaab80d9570442c52`
- Unchanged production Viewer SHA-256 `154245702a04fdb3c16e01f9ea8a6eaa9aebe14de841d141fc231b295a23eada`

The previous implementation receipt remains a record of the earlier snapshot; this addendum supersedes its zero-source sentinel description and test hash. No headers, CMake, REA, GUI or evidence artifacts were changed. Source is released for root validation.
