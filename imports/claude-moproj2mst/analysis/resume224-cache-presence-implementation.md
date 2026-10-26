# Resume 224: Operation cache presence query implementation

Implemented the approved incremental leaf.

## Changes

- Added inline `Operation::hasEngineModel() const noexcept` in `source/apps/moproj2mst/include/m2m/model.hpp`. It returns the boolean state of the existing shared pointer without copying or exposing it.
- Migrated the Cogging Save cache-presence gate in `source/apps/moproj2mst/src/datafiles/mdt_write.cpp` to call the query. Branch order and surrounding condition are unchanged.
- Added absent/present assertions in `source/apps/moproj2mst/tests/doe/test_output_save_prefix.cpp` at the existing fixture states: existing owner, absent cache before lazy production, and cache present after production.

## Semantics

The public `engineModel` member remains available for staging. The query neither invokes lazy model creation nor acquires/releases a shared owner. It preserves null versus non-null behavior and cannot affect use counts or destruction timing. No cache assignment/reset, step access, C ABI, or cache1 behavior changed.

## Validation boundary

The grouped focused preflight reported C3861 for `CHECK_FALSE` at test lines 138 and 154. Inspection of `source/port/testkit/include/testkit/check.hpp` confirms the supported assertion macro is `CHECK(condition)`; both assertions were repaired to `CHECK(!condition)`. No build or CTest was run after this repair, as requested; root owns the rerun. The checkout contains unrelated concurrent edits; they were not modified by this repair.

