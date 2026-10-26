# CP129 Type3 optimizer scalar implementation plan

## Engineering purpose

The CP128 evidence supports a private value-level scalar prefix for the Type3 optimizer. It does not support a complete `.OPT` reader or a live `ShapeOptim` owner. This plan pins the smallest implementation seam to already decoded scalar fields, ending before the unknown static `QRegExp` delimiter and before `NUM_CON` / `REGIONS` row bodies.

## Source basis

`analysis/resume128-type3-constructor-parser.md` records original PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC` and the bounded CP127 dumpbin evidence. Constructor `0x140917ff0` and initializer `0x1409180b0` establish these scalar defaults:

| Field | Default | Evidence role |
|---|---:|---|
| `+0x0c` | `1` | derived initializer after temporary zero |
| `+0x10` | `1` | base/derived initialization |
| `+0x30` | `0x3fd` | derived initializer |
| `+0x34` | `-1` | derived initializer |
| `+0x38` | `200` | derived initializer |
| `+0x3c` | `0` | derived initializer |
| `+0x40` | `0.001` | bits `0x3f50624dd2f1a9fc` |
| `+0x48` | `false` | derived initializer |

The constructor also default-constructs and clears a QString at `+0x28`; that value is deliberately outside this scalar helper because the requested seam is limited to scalar state. `+0x18` and `+0x20` have nontrivial shared-data/reference-counted initialization and cleanup and are not represented.

The parser at `0x140908630` handles these scalar key effects after successful QFile open, in input order:

- `OPR_NO`: token 1 `toInt`, set `+0x38`.
- `KRG_MAX`: token 1 `toInt`, set `+0x0c`.
- `TYP_OBJ`: tokens 1, 2, 3 `toInt`, set `+0x30`, `+0x10`, `+0x34` in that order.
- `EPS_OBJ`: token 1 `toDouble`, set `+0x40`.
- `DB_PURGE`: if token 1 exists, set `+0x48` to (`toInt(token1) != 1`); if missing, set false.

`DEF_OPT`, `TYP_OPT`, `GRD_TYP`, and `DOE_PAR` are recognized but skipped. `ENDDATA` terminates the parser. `NUM_CON` appends to the member at `+0x18`; `REGIONS` appends to `+0x20`. Their row schemas/ownership are unclosed. The static split regex at `0x143dd9a28` has no recovered initializer/pattern, so this helper must accept already-tokenized fields and must not implement line splitting.

## Proposed private seam

Add only:

- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp`
- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.cpp`
- `source/apps/moproj2mst/tests/doe/test_type3_optimizer_scalar_prefix.cpp`

Keep the type under `m2m::mst::detail`. Model only the scalar values, named by their recovered byte offsets to avoid inventing higher-level semantics:

```cpp
struct Type3OptimizerScalarState {
    std::int32_t value0c = 1;
    std::int32_t value10 = 1;
    std::int32_t value30 = 0x3fd;
    std::int32_t value34 = -1;
    std::int32_t value38 = 200;
    std::int32_t value3c = 0;
    double value40 = 0.001;
    bool value48 = false;
};

enum class Type3ScalarLineResult { Applied, Ignored, RowBodyNotImplemented };
Type3ScalarLineResult applyType3OptimizerScalarLine(
    Type3OptimizerScalarState& state,
    const std::vector<String>& fields);
```

The function takes fields produced elsewhere; field zero is the key. It is a value operation only: no file access, QFile/QTextStream emulation, QObject/ShapeOptim allocation, shared references, append behavior, diagnostics, or public import admission.

Recognized scalar keys with enough fields update their mapped values and return `Applied`. A `TYP_OBJ` line converts all three values before committing the three stores, so this bounded reconstruction helper does not invent a partially converted state when the tokenized input is insufficient. Missing token(s) for a recognized scalar line return `RowBodyNotImplemented` without mutation. This is an explicit bounded-input admission rule, not a claim about original malformed-input behavior. `NUM_CON` and `REGIONS` return `RowBodyNotImplemented` without mutation. Skipped keys and unknown keys return `Ignored` unchanged. `ENDDATA` remains the file-loop controller's concern and is not presented as a scalar assignment.

For supported ASCII numeric spellings, use existing `qt::toInt` and `qt::toDouble` (`src/qt4/number.cpp`) with null `ok` pointer, consistent with the observed parser's ignored conversion status. Record the abstraction boundary: native QLocale/global state equivalence for arbitrary strings is not established by the CP128 evidence. Do not claim locale-independent parity.

## Test contract

The new test should be standalone in the existing packet test convention: first-line `// ctest-labels: m2m-doe`, `#include <testkit/check.hpp>`, include the private header by the source-relative path `../../src/ops/type3_optimizer_scalar_state.hpp`, and end with `return testkit::run_all();`. Use `CHECK`/`CHECK_EQ` and no C `assert`.

Cover:

1. All eight constructor defaults.
2. One-field-at-a-time mapping for `OPR_NO`, `KRG_MAX`, `EPS_OBJ`, and `DB_PURGE`.
3. `TYP_OBJ` mapping to the three exact members.
4. Encounter-order overwrite for repeated keys.
5. `DB_PURGE` missing token => false, token 1 => false, other supported ASCII integer => true.
6. Skipped keys and an unknown key leave the state unchanged.
7. `NUM_CON` / `REGIONS` return `RowBodyNotImplemented` and leave scalar state unchanged.
8. Undersized recognized scalar lines are rejected without partial mutation, especially undersized `TYP_OBJ`.

These tests establish only the helper's explicit tokenized-value contract. They do not test `.OPT` tokenization, QRegExp behavior, file-open failures, parser return value, row parsing/append/cleanup, native locale initialization, or Type3 public Save/Open effects.

## Integration/build effects

No shared/public header is required. Do not modify `include/m2m/shapeoptim.hpp`, `src/ops/shapeoptim.hpp`, `src/engine/shapeoptim_load.cpp`, `src/model/subproject.cpp`, `src/doe/*`, or the public type-3/geometry guards. The helper remains dormant and does not construct the SUB+0x28 object.

CMake auto-discovers `src/*.cpp` in `m2m_core` via `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`. It auto-discovers `tests/<packet>/test_*.cpp` and links normal packet tests with `m2m_core` and testkit. Therefore these three files require no CMake edits. At implementation, resolve the private quoted include from the test directory as described above and run the mandated static include/Testkit preflight; do not build as part of this design-only handoff.

## Stop boundary and remaining unknowns

Stop before QRegExp tokenization: its global initializer and split pattern are unavailable. Stop before `NUM_CON` and `REGIONS`: member layout/ownership, row conversion order, allocation failure, and destructor behavior are not closed. The parser's successful-open `vtable +0x160` write and always-false observed return also remain outside this scalar helper. No claim of native optimizer allocation/refcount parity, full `.OPT` parsing, populated `listF0`, active ShapeOptim GEO import, or public Type3 support follows from this plan.
