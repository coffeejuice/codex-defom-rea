# CP134 independent source review: exact-frame geometry retirement

## Result

No blocking source or test defect found in the frozen CP134 slice. Retirement is guarded by the exact frame identity and shared ownership identity, happens after Stop inside the same synchronous `withStep` callback, and requires the currently borrowed step to still belong to the model. The first coordinate getter is not replayed on success. Earlier refusals continue to leave the barrier installed and keep the model unavailable to later `withStep` calls.

This is static source/test review only. Root owns compilation and focused runtime validation; no build or test was run here.

## Frozen identity

The implementation and test match the CP134 receipt:

| Artifact | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `c4897abb365516e9e3056c028b40ac26be4ea3df56c04e0aa0aa70fa4a7b1f61` |
| `tests/mdt/test_viewer_box.cpp` | `888f7e0b80e4a3c57c0316f4bfb46f4567a956239c88b3c658c583459198808b` |
| `analysis/resume134-geometry-frame-retirement.md` | `8e07cf5387c600d0559e63b6fac4957ce699fd0d1814313df9da6a9983976555` |

## Source review

- `completeInitialCoordinateRead` is called exactly once, immediately after node timer Stop and `NodeSkipLoopCommitted`, from inside the initial `GuiModelAccess::withStep` callback. `withStep` gives this callback the current raw `model.step.get()` and does not pin or lock that pointer, so doing the comparison and reset there is the appropriate scope.
- The helper requires both `installed.get() == frame.get()` and equivalent `shared_ptr` ownership (`owner_before` in both directions). It also requires the live model guard to match this model, a non-null borrowed step identical to current `model.step`, the exact completed stage, successful query, low signed post-version, and the qualified id/flag/which/dimension tuple. It then checks the borrowed step's current version equals the frame's post-query version before marking `CoordinateReturned` and resetting the model's barrier owner.
- The helper does not dereference `frame->step`; static search shows that saved pointer is only assigned when the frame is created. Local `frame` ownership remains through the callback and following code, so resetting the model's shared owner cannot destroy it mid-callback.
- A failure before or during Stop returns before `completeInitialCoordinateRead`; `coordinateReturned` remains false, the pending status is returned, and the installed barrier remains. If the exact completion predicate fails after Stop, the helper returns false without clearing; the caller likewise returns pending. The high-record and null-companion branches retain their pending exits.
- On success, the local completion flag is checked after callback return. Only then does the code set `coordinatePrefixCompleted`, so the already-completed first getter is skipped once and existing connectivity/readiness logic follows. The builder's later second getter remains intact. No reader, outer sync, query, timer or first COW path is replayed by this branch.

## Test review and limits

The new outcome path in `checkGeometryDirectoryOutcome` treats only a low-record case with nonnegative arrays timer and successful node-skip loop as completed. It checks scene acceptance, availability of a fresh `withStep`, timer labels/counts, one query/version update, pointer/step state, and preserved optional coordinate/connectivity pointers. Repeated refresh checks exact timer snapshots and expects the builder's additional getter round only when connectivity is present. The added later-builder-guard case specifically verifies reader/timer completion survives a separate scene refusal without reinstalling the old barrier or replaying the initial query.

The broader existing node-skip matrix still covers foreign/null owner, nonzero +0x87c, assignment-state order, invalid MAR selector and stride boundaries; those cases retain parked expectations. Coverage is fixture-based and does not establish concurrent mutation safety, asynchronous continuation, or original runtime parity. The exact-frame helper is private and its bad-frame predicates are not directly fault-injected in this slice; the tests exercise the reachable valid and incomplete callback paths instead.

## Validation boundary

I found no weakening of public admission or a route that clears the barrier on an incomplete profile. This review did not modify source/tests, run a build, launch/control the original app, or mutate REA state.
