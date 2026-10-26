# CP131 prepared Type3 row value projections

## Change

Extended the dormant private Type3 scalar-state value helper with two explicit prepared-value append operations. They operate only on values already converted/formatted elsewhere and keep the parser's `NUM_CON` / `REGIONS` key outcomes at `RowBodyNotImplemented`.

- `appendType3NumConPreparedRow` takes arguments in the observed native method-call order: field 1, classifier result, field 3 double, prepared QString, field 2, field 4. It stores the corresponding final row values at offsets `+0x00`, `+0x04`, `+0x08`, `+0x0c`, `+0x18`, and `+0x20`, in encounter order, and returns vector `size - 1`.
- `appendType3RegionPreparedRow` stores `{value00=0, value08=value}` and returns vector `size - 1`, matching the observed `edx=0` and QString argument.

The pure value vectors do not model Qt allocation/refcount, native stack temporary construction/destruction, error unwinding, or maximum-capacity failure. The caller is required to keep row counts in the positive `int32_t` range, matching the observed native `size()`/return type domain. Prepared records do not infer meaning for native offsets.

## Files

- `src/ops/type3_optimizer_scalar_state.hpp/.cpp`: added record projections and append functions; scalar line parsing and `ENDDATA` `Terminate` behavior unchanged.
- `tests/doe/test_type3_optimizer_scalar_prefix.cpp`: two new cases check six-input mapping, encounter order, region constant zero, and size-minus-one results. Existing scalar tests and the `NUM_CON`/`REGIONS` dispatcher guard remain.

## Evidence boundary

This slice is supported by original PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, `analysis/resume127-type3-opt-parser.asm`, `evidence/gui_mo/s1/vtables.tsv`, and the bounded CP131 windows documented in `analysis/resume131-type3-next.md`. The `NUM_CON` parser call supplies field-1 int, classifier int, field-3 double, formatted QString, field-2 int, field-4 int; method `0x14091e110` builds the final row fields and appends through `0x14091fb60` into `+0x18`, then returns `size - 1`. `REGIONS` parser supplies integer zero and a QString reference; method `0x14091f1a0` forms the `{int, QString}` temporary and appends through `0x14091fce0` into `+0x20`, then returns `size - 1`. The parser ignores both results.

The QRegExp at `0x143dd9a28`, its initializer/pattern/options, `.OPT` file-to-token conversion, `NUM_CON` string formatting, derivation of the `REGIONS` QString, row-specific malformed input behavior, native Qt allocation/cleanup, and public Type3/ShapeOptim behavior remain excluded. No parser path or public guard is changed.

## Static preflight

The existing test source-relative include resolves to the private header. The test retains its `m2m-doe` label, Testkit entry point, identifier-form `TEST_CASE` macros, and no C `assert`. CMake recursive source and test globs cover these files, so no CMake edit is needed. Static path/macro/whitespace checks passed. No build or tests were run; root owns validation.
