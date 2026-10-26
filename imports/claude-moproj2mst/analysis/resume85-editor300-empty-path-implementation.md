# CP85 null-array empty path source freeze

The existing advanced-page live predicate now admits an actual type7a/ndim1/
ncomp4 zero-point path only when all its arrays are null. It reads no sample
and does not allocate or evaluate a curve. Nonnull zero-count payloads remain
guarded, as do negative/overflow counts, other shapes and nonzero/unordered
samples. The scalar constant and positive-table branches remain intact.

Source qualification is the approved plan
`analysis/resume85-editor300-next.md`, SHA-256
`00459d49e7ccaa0912e5b6e6a905a04db268e134c375fa7915a4a0cf0e1e20f7`.
It traces both zero-iteration native loops and the 4*0 value-count result,
then the movement predicate's false return that skips `140874630`.

## Exact changes and regression coverage

`advancedTimeProfile()` now separates common table shape/count guards from
empty/positive pointer guards. Common checks retain type7a/ndim1/ncomp4,
nonnegative signed32-safe count, no u68 owner and zero/null inactive fields.
For n0=0, any nonnull axis0/values refuses; otherwise the actual object scan
continues immediately. For n0>0, both arrays remain required before the
unchanged axis-first and dependent-value loops. Fill and pre-store Apply
continue to use the SAME live GuiModel object and embedded DYNARY.

The header documents the empty/null and positive/safe-count profiles. No API,
layout, public session, palette/counter, runtime or path-evaluator changed.

Three new Release-active Testkit cases exercise:

- actual default arrays released while their positive counts still establish
  ownership, followed by n0=0: Fill and changed-time Apply succeed while all
  array pointers remain null and metadata, NaN scalar bits and position bits
  remain unchanged;
- successful Fill of an allocated positive path followed by release through
  the SAME embedded owner and a live transition to empty: Apply succeeds;
  later negative count or nonzero ndim0 scalar refuses before time stores,
  while restoring the empty-table profile ignores inactive scalar storage;
- zero count with both nonnull payloads, axis-only or values-only remains
  refused by Fill and Apply. The actual allocations/pointers and positive
  count are restored before ordinary cleanup; no owner is leaked by falsely
  converting it to borrowed metadata during destruction.

The existing zero-count refusal assertion was kept and clarified: it still
has nonnull one-point arrays and therefore exercises the guarded profile.
Existing negative/overflow, ncomp1, missing-positive-array, nonzero, NaN and
kind7 cases remain unchanged. Lifecycle's explicit nonzero refusal fixture
needs no amendment.

## Static preflight and frozen files

Compared these three files with CP84's archived source. Quoted includes resolve,
new macros are supported Testkit macros, delimiters balance, the test file has
one Testkit entry point, and no C assert or trailing whitespace is present.
Fixture release order was checked against `dynary_free_arrays`: release occurs
before setting a formerly positive count to zero, and nonnull-marker test
metadata is restored before destruction. No CMake/include change is needed.

Root target `m2m_mdt_simcontrol300_snapshot`, CTest
`m2m.mdt.simcontrol300_snapshot`, now has 88 cases (85+3). No build, executable
test, CTest, GUI/original execution or mutable REA was run by this worker.

Files relative to `source/apps/moproj2mst/` are released/frozen:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `f06a8a7ea6874cb00b327ff9cf8e6938199f939eac9f44def2dedb99b982ce4f` |
| `src/engine/simcontrol300.cpp` | `3ee26f182a1140d699d5035b00c7d587be4261f9e76d6569c055ef100c95ac5d` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `9f5d57e268c33b0af4f56c27f84cea7a3aaad35463e136441a2372400444bf8f` |

Root owns grouped build/validation/archive. Public editor300 remains behind
its earlier native widget/private-palette gate. Remaining path dependencies
include other type/component/dimension profiles and borrowed/extra-owner state;
actual nonzero copy/evaluation/rotation remains unrecovered and guarded.
