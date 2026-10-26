# CP138: pure retained-image Win32 error mapper

Implemented private `mapRetainedFeatureWin32Error(uint32_t) noexcept` in three NEW files. It preserves all45 ordered initial-image rows, first-match key6→9 despite later6→22, and exact unsigned32 fallback arithmetic: unmatched19..36→13,188..202→8,else22. The caller must supply the explicit error word. No LastError/FLS/PTD/errno read/write, allocation or Close integration exists in production code.

The controlling source contract is `analysis/resume138-feature-error-map-next.md` SHA-256 `6eea1fc004796fc8c534f69b6966835fcc2078914a1c4bb117ed3a0ab3eac7e5`. Original DEF_GUI_MO_64.exe v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; native mapper1414740ec..141474133 SHA `f44e86936fa3b398df977c4dbe968eff2c5fec42fd3c07a7dc4adc02d61bce17`; table143b5e160..143b5e2c8 SHA `dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe`, ranges end-exclusive.

The original table is writable.data. This implementation explicitly models its retained initial bytes, not current original-module state or a live errno owner. Native's45-row loop excludes the adjacent fallback errno word. The source-qualified first-match loop and uint32 modular subtraction preserve these details without host errno constants or a modern mapping policy.

Only the new private hpp/cpp/test and this receipt were edited. No existing runtime/shared/model/Project/Session/CMake/plan edits, compilation/tests, GUI/original execution or REA database work. Root owns execution and source-stable freeze.

## Focused tests and static preflight

Four Release-active Testkit cases cover all44 unique source-table keys; duplicate priority; error0 and fallback boundary transitions18/19/36/37 and187/188/202/203; table-specific206/1816; full uint32 high/wrap cases; noexcept; and an explicit host-only errno purity probe that restores entry state. That probe neither binds native PTD nor treats the supplied test value as observed LastError. Production code includes no OS/CRT error accessor.

Static preflight passed include resolution, Testkit main/supported macros, no C assert/REQUIRE_EQ, exact ordered45-row packed-table SHA match, duplicate order, unsigned fallback order, source-derived expected-vector consistency, unique library helper names and `git diff --check`. These are source/data checks, not C++ test execution. No build/tests ran here. Existing source/test globs supply the new target without CMake edits:

```text
cmake --build <build> --config Release --target m2m_model_feature_win32_error_map
ctest --test-dir <build> -R '^m2m\.model\.feature_win32_error_map$' --output-on-failure
```

## Frozen receipt

Paths relative to source/apps/moproj2mst/:

| NEW file | SHA-256 |
| --- | --- |
|src/model/feature_win32_error_map.hpp|`ec689dd8845c8def3bbb6e3abdcabd663cb4506f83842f596013e067f17b0f92`|
|src/model/feature_win32_error_map.cpp|`128c9e07d02e589fc1f14d0e7de0a6a1656c9b120ed408135cc034f921f71cd5`|
|tests/model/test_feature_win32_error_map.cpp|`c9ea0e7c83446932d2afd47efa00a62081885bca49d9f0616a91c65ad2f4e5d0`|

All three files are frozen and released to root. Live FLS index/callback/lock/PTD ownership, captured same-thread error continuity, actual failed-free completion, context lifetime and public Close remain guarded. This is a static implementation receipt, not a tested archive or runtime observation. No reconstructed executable/deployment was inspected or changed.
