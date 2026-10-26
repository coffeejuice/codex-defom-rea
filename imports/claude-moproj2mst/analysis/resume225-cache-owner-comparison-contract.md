# CP225: Operation cache owner comparison contract

## Recommendation

The narrow next consumer is the owner-equivalence clause in
`source/apps/moproj2mst/src/model/operation_info_cleanup.cpp`, inside
`attemptOperationInfoEngineCacheCleanup` at `EngineModelVirtualDeleteBlocked`.
It is a real identity check that can move behind a named `Operation` query
without changing the `shared_ptr`/`weak_ptr` lifetime relationship or the
surrounding branch order. This is an incremental API leaf; it does not require
privatizing the cache or migrating the broad direct-read tree first.

## Proposed API and exact semantics

Add a const, non-owning query on `Operation`, conceptually:

```cpp
bool sameEngineModelOwner(const std::weak_ptr<engine::GuiModel>& expected) const noexcept;
```

Its implementation should compare `engineModel` with `expected` using
`owner_before` in both directions:

```cpp
return !engineModel.owner_before(expected) && !expected.owner_before(engineModel);
```

This means *control-block equivalence*, not pointee-address equality. Do not
accept a `shared_ptr` by value, return an owner, or lock the weak pointer. Those
alternatives would introduce copies or transient strong ownership. The current
progress field is specifically `std::weak_ptr<GuiModel>`; retaining that weak
field and its assignment from the cache preserves its existing non-owning
lifetime behavior.

At the call site, retain the existing ordered guard:

```cpp
if (!cache || token != progress.observedEngineCacheToken_ ||
    !operation.sameEngineModelOwner(progress.observedEngineCache_))
    unavailable(...);
```

Keep the cache read as a reference, the explicit `uintptr_t` token check, the
short-circuit order, the same refusal, and the success outcome. The token
check establishes the stored raw address identity; the named query establishes
the same control block. They are complementary. Do not reorder or combine them:
in particular, the owner query must not run when the cache is null or the
address token already differs.

At `EngineModelBoundary`, preserve the present behavior exactly: a non-null
cache is assigned into the weak observation and the raw-address token is
recorded before advancing to the blocked virtual-delete stage; a null cache
advances directly to `ObjectCleanupBoundary` with no reset or deletion. This
leaf should change only the repeated identity comparison, not that boundary.

## Focused test requirements

Extend the existing `tests/mdt/test_operation_info_cleanup.cpp` coverage for
the engine-cache blocked stage. Preserve or add cases showing:

1. The original cache owner remains present at the same address and control
   block: the query accepts and the existing blocked outcome is returned.
2. The same pointee address is installed under a different control block: the
   query rejects, even though the stored address token matches.
3. A different pointee address with the original control block (an aliasing
   `shared_ptr`, if safely constructible in the fixture): the token check
   rejects before owner comparison can change the result.
4. The cache is null: the null guard rejects before the identity query.
5. The original owner is released and the progress weak pointer expires: the
   query remains non-owning; a replacement owner does not pass merely because
   an address is reused. Keep assertions on weak expiration/use counts only if
   the existing fixture already has a safe way to establish them.

The same-owner case should continue to reach the existing virtual-delete
unavailable outcome; this recommendation does not claim the deletion itself
is implemented. Tests should also retain the existing initial-null path and
its direct transition to `ObjectCleanupBoundary`.

## Why other visible reads are less suitable

`conversion_session.cpp::beginEditor300Factory` checks only
`op.engineModel.get() == &model`, alongside project/model-manager identity. It
does not currently establish control-block identity. Replacing it with a
control-block owner query would strengthen behavior for same-address,
different-owner aliases, so that consumer is not a semantics-preserving next
step unless a source contract first proves owner identity is intended there.

The other cleanup-stage `if (operation.engineModel)` reads are presence gates,
not owner-identity comparisons. They can use the CP224 `hasEngineModel()`
query in a separate mechanical migration, but they do not exercise the named
owner-identity API proposed here. Loader creation, weak guards, and consumers
that retain model ownership have different lifetime/access requirements and
should not be swept into this leaf.

## Evidence limits

This recommendation is based on the CP223 operation-cache contract, CP224
presence implementation and independent review, the current
`conversion_session.cpp`, `operation_info_cleanup.cpp`,
`operation_info_cleanup.hpp`, and `model.hpp`. The cleanup progress declaration
confirms `observedEngineCache_` is weak; the implementation explicitly checks
both address token and symmetric `owner_before` equivalence. No source, tests,
build, runtime, GUI, or REA state was changed or exercised for this report.
This is a migration contract, not proof that a future patch compiles or passes
tests. Public cache mutability and the remaining direct-reference migration
inventory stay open, as does GuiModel step closure.
