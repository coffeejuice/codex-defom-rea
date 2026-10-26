# CP225: Operation cache owner comparison implementation

## Change

Added `Operation::sameEngineModelOwner(const std::weak_ptr<engine::GuiModel>&)`
as a `const noexcept` query in `source/apps/moproj2mst/include/m2m/model.hpp`.
It uses symmetric `owner_before` comparison against the current
`engineModel`. It takes the expected owner by weak reference, returns only a
boolean, and does not copy/lock either owner or acquire a new strong reference.

Migrated the control-block equivalence portion of
`attemptOperationInfoEngineCacheCleanup` in
`source/apps/moproj2mst/src/model/operation_info_cleanup.cpp`. The guard still
checks null first, then the stored raw-address token, then owner equivalence;
short-circuit order, refusal text, stage, and outcome are unchanged. The
existing weak progress observation and initial-null transition remain intact.
No cache privacy, cache assignment/reset, or cache1 behavior changed.

Extended `tests/mdt/test_operation_info_cleanup.cpp` using its existing safe
same-address/different-control-block fixture. Coverage now checks that the
query accepts the original owner and rejects the no-op-deleter replacement,
that the blocked retry retains the established refusal, and that repeated
cleanup does not change the cache's `use_count`. Two additional edge cases use
safe owner fixtures: two `GuiModel` objects aliasing one `shared_ptr` control
block (different address rejected before owner comparison), and an externally
managed stack model whose original no-op-deleter control block expires before
a new no-op-deleter block is installed at the same address (address token
matches, expired owner does not). No fixture deleter destroys the stack model.
Existing cleanup tests retain coverage of the null-cache skip and
cache-appearance rejection. Testkit macros used are the supported `REQUIRE`,
`CHECK`, and `CHECK_EQ`.

## Validation

- The first grouped build invocation had incorrectly rooted source-preflight
  paths under `apps/moproj2mst/`, so its source preflight failed with
  `missing_file`; no compilation or CTest ran in that attempt.
- The corrected `admin/build_candidate.py --then-full` invocation compiled
  `m2m_mdt_operation_info_cleanup` successfully (exit code 0). Its focused
  `m2m.mdt.operation_info_cleanup` CTest passed 1/1.
- The grouped script then proceeded into its full-project build (698 build
  steps). That was outside the requested focused validation; it was
  interrupted at 163/698. The script therefore exited 1 and provides no full
  build or full-suite validation. The source hashes logged during the focused
  compile and CTest matched. After interruption, a process check found no
  `cmake`, `ninja`, or `cl` process remaining.
- After the two additional fixture cases were added, the read-only static
  preflight passed for `tests/mdt/test_operation_info_cleanup.cpp` with no
  issues. No build or CTest was run for those additions; root owns the grouped
  CP225 validation.

Root owns the combined CP225 build and full validation. No original, GUI, REA,
or shared runtime state was touched.
