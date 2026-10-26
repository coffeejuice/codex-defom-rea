# CP129 Type3 optimizer scalar prefix implementation

## Result

Implemented the private, already-tokenized scalar prefix described in `analysis/resume128-type3-scalar-implementation-plan.md`. The helper models the eight qualified scalar defaults, applies the five resolved scalar-key effects in encounter order, and returns explicit `Ignored` or `RowBodyNotImplemented` outcomes for skipped/unknown keys and unported row paths. It does not tokenize `.OPT` text, construct the native optimizer, or integrate Type3 loading.

## Files and contract

- `src/ops/type3_optimizer_scalar_state.hpp/.cpp`: private `m2m::mst::detail::Type3OptimizerScalarState` with fields named by native offsets and `applyType3OptimizerScalarLine` over `std::vector<String>`.
- `tests/doe/test_type3_optimizer_scalar_prefix.cpp`: seven focused Testkit cases cover constructor values, scalar mappings, `TYP_OBJ`, encounter-order replacement, `DB_PURGE` missing/1/0 mapping, skipped and unknown keys, guarded `NUM_CON`/`REGIONS`, empty input, and short scalar lines with no partial `TYP_OBJ` mutation.

Recognized scalar keys call `qt::toInt`/`qt::toDouble` without a validity output, matching the null-ok shape seen in the native parser. Numeric fidelity is qualified to the existing conversion seam and supported ASCII test values; arbitrary locale behavior is not asserted. A DB_PURGE row without a value explicitly writes false, as the retained parser evidence specifies. ENDDATA is treated as unchanged/ignored by this value helper; parser-loop termination remains outside its scope.

## Static preflight

- Confirmed the test's `../../src/ops/type3_optimizer_scalar_state.hpp` include resolves from `tests/doe/` to the added private header.
- Confirmed the test includes `<testkit/check.hpp>`, has first-line `// ctest-labels: m2m-doe`, uses `TEST_CASE`/`CHECK` macros, and ends in `testkit::run_all()`; no C `assert` is present.
- Confirmed `src/*.cpp` recursive CMake discovery covers the implementation and `tests/<packet>/test_*.cpp` discovery covers the test; no CMake edit is required.
- `git diff --check` passed for the three implementation files and this receipt. No build or test executable was run, per task scope.

## Evidence boundary

The constructor defaults and key-to-offset mapping derive from the CP128 static PE receipt and original hash recorded in `analysis/resume128-type3-constructor-parser.md`. This implementation is a dormant scalar value model only. It does not represent the QString at +0x28, the shared-data members at +0x18/+0x20, the parser's QFile/QTextStream behavior, QRegExp splitting, `NUM_CON`/`REGIONS` row conversion or ownership, parser return behavior, populated `listF0`, ShapeOptim/GEO effects, or public Type3 support. Public Type3 and ShapeOptim guards remain untouched.

## CP129 preflight repair

The first grouped preflight found two compile-contract issues before the root rebuild: `String` is declared as `m2m::qt::String`, and this repository's `TEST_CASE` macro takes an identifier rather than a quoted label. The private API/implementation/test now use `qt::String` and seven identifier-named test cases. Scope and behavior are unchanged. The root owns the rebuild; this repair pass ran static checks only.
