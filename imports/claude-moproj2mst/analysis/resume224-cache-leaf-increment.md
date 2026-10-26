# Resume 224: incremental Operation cache consumer

## Recommended leaf

A safe first batch is an additive, read-only presence query on `Operation`, then migrate exactly one production consumer: the Cogging Save cache gate in `src/datafiles/mdt_write.cpp:584`.

Add `bool hasEngineModel() const noexcept { return static_cast<bool>(engineModel); }` beside Operation's other named cache/identity accessors in `include/m2m/model.hpp`. Change the gate from `!cur->engineModel` to `!cur->hasEngineModel()`. Leave the public data member in place for all other callers. This is a real API consumer change and compiles independently without a tree-wide migration or test-fixture churn.

## Semantics and lifetime

The method observes the same shared_ptr object and returns its nullness only. It does not call lazy `opModel`, construct or retain a GuiModel, copy a shared owner, expose a reference, or change cache lifetime. In `mdt_write.cpp`, the check remains in the same branch and evaluation order, after Cogging props/model checks. Thus null cache still takes the existing path and non-null cache still blocks that action. There is no new weak/strong owner, address comparison, or native step access.

This is intentionally a read-only *query* step toward privatization, not a claim that the cache is encapsulated. Because the underlying field remains public, replacement and mutation routes remain available elsewhere.

## Exact edit ownership

- Header/API: `source/apps/moproj2mst/include/m2m/model.hpp` — add one inline const presence query adjacent to Operation accessors.
- Production consumer: `source/apps/moproj2mst/src/datafiles/mdt_write.cpp` — replace only the direct presence expression at line 584.
- Focused regression target: `source/apps/moproj2mst/tests/doe/test_output_save_prefix.cpp` — existing cases assert absent-cache and present-cache Save gate behavior. No new test is needed for an accessor that is exactly shared_ptr boolean conversion; if a test is desired, assert `hasEngineModel()` for the existing absent/present fixture states in that file.

No other source or test ownership is required in this batch. In particular, do not touch `operation_info_cleanup.cpp` or `conversion_session.cpp`: they need owner identity/control-block evidence and cannot safely be migrated to a boolean query.

## Why this preserves behavior

The source expression `!cur->engineModel` is contextual conversion to bool followed by logical negation. The proposed expression returns that same conversion as a bool and negates it at the same call site. It does not alter branch order, side effects, exceptions, or lifetime. Since no owner copy is made, use_count and last-owner destruction timing are unchanged. The existing Save prefix tests already cover the absent/present gate's surrounding effects; source-level equivalence is direct.

## Boundary

This leaf establishes a migration pattern only for presence checks. The larger privacy change still needs owner-aware APIs for cleanup/session/lifecycle consumers, controlled assignment/reset for loader and fixtures, and a separately staged GuiModel step closure. No source/tests/build were changed and no tests were run for this design note.

