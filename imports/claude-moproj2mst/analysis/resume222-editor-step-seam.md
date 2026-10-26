# CP222 R300: editor step ownership seam

## Decision

There is no dependency-closed C++ ownership/API seam ready to implement now that closes a meaningful `GuiModel::step` alias class while preserving current behavior. Keep CP163's A-only publication and immediate `CoordinateSync` return, and keep cache1 refused. A local accessor or another callback wrapper would be a convention, not closure: callers can still use the public `unique_ptr`, retain mutable aliases obtained through `simCtl`/`object` access, or retain the shared model through `Operation::engineModel`.

This conclusion does not depend on the broader 41-file CP203 count alone. CP218's current `src/` inventory locates 25 production source/header files with direct `.step`/`->step` references and classifies their distinct roles: public convenience reads, lifecycle/load writers, editor mutations, serialization consumers, and viewer/geometry identity retention. Each category depends on storage and borrow semantics, so making just `objectCount()` or one editor caller use `withStep` leaves replacement and outstanding aliases able to bypass the guard. A callback can capture the raw pointer even when its result type is constrained to `void`/`bool`.

## Why a smaller slice is not safe

The apparent first seam is private storage plus guarded borrowing in `GuiModel` (`source/apps/moproj2mst/include/m2m/engine.hpp`, `src/engine/gui_model.cpp`, `include/m2m/engine/gui_model_access.hpp`). It is not dependency-closed by itself:

- The public `std::unique_ptr<DB_STEP_DATA> step` is both a read/write alias and a replacement/reset capability. Replacing it with a private field breaks direct callers at compile time; retaining it leaves the bypass open.
- `objectCount()` and `viewer.cpp` are only two consumers. The loader families construct and replace step state; `simcontrol300*` and Boolean lifecycle mutate it; Save families serialize it; viewer/geometry code retains identity across calls. Moving one family cannot establish the lifetime contract for the others.
- `withStep`/`withOpStep` do not enforce borrow lifetime. Fixing their callback signature alone cannot invalidate a pointer captured before a Stopping transition, and does not cover mutable `simCtl()`, `object()`, or `objectAt()` results.
- A Stopping gate must cover outstanding borrows and step replacement/reset, and the migration must preserve existing geometry admission and mutation/effect order. A local guard check has a race/bypass until all old routes are removed.

The public **m2m C ABI** can in principle remain stable: keep exported C declarations and layouts/behavior untouched and confine the new owner API to internal C++ implementation. But this does not make a partial C++ migration safe; the currently public C++ `GuiModel` member is itself an exposed source API, so an all-callers compile migration and compatibility review are still required. No evidence reviewed here establishes that external C++ consumers can be ignored.

## Exact prerequisite for a safe implementation

The minimum next R300 implementation is a coordinated ownership migration, not an isolated helper. Before it starts, define and review the owner contract for private step storage, owner-only replacement during load/reload/reset, non-escaping scoped access, and a shared pending-Stopping/geometry admission that accounts for already-active borrows. Then migrate and test the dependency set in this order:

1. `include/m2m/engine.hpp`, `src/engine/gui_model.cpp`, `include/m2m/engine/gui_model_access.hpp`, and `include/m2m/engine/engine_internal.hpp`: private storage and owner API; remove raw-pointer-return/capture escape routes; establish active-borrow and replacement rules.
2. Lifecycle writers: `src/engine/loader.cpp`, `loader_l2.cpp`, `loader_l3.cpp`, `shapeoptim_load.cpp`, `cogging_load.cpp`, and `postload_views.cpp`, plus construction/reset/destruction owners. Verify load/reload/failure-prefix behavior before migrating consumers.
3. Editor mutation family: `src/engine/simcontrol300.cpp`, `simcontrol300_lifecycle.cpp`, and `boolean_lifecycle.cpp`, followed by geometry/property/object/slot writers. Keep validation and associated writes inside one admitted operation.
4. Persistent aliases: viewer frames/evidence and `geometry_temp_prefix.cpp`; replace retained raw identity with an owner generation/lease or revalidation. Add reset/reload and stale-evidence lifetime coverage.
5. Read/serialization families: viewer/scene/DB reads and `save_current.cpp`, `save_l3.cpp`, `save_l6.cpp`, `ht_save.cpp`, `cogging_save.cpp`, `l5_slots.cpp`, and `l6_slots.cpp`; prove synchronous consumption and preserve output/effect order.
6. Close all remaining production, test, and supported external C++ callers; compile-time visibility should make direct member use fail. Only after this closure should `test_simcontrol300_stopping_cached.cpp` add the cached-stopping continuation, alongside API/borrow, lifecycle, viewer, DB/query, and Save regressions.

These files are a dependency map, not permission to edit them in this assignment. The closure proof must search direct member access, `.get()`/reset/assignment, forwarded pointers, callback captures, mutable object/control aliases, and retained owner handles across the whole app and tests. Existing tests need not be presumed sufficient; focused behavior/effect-order tests must be designed from the migrated call sites. Cache1 remains refused until both that closure and CP214's separate same-occurrence original Open-to-InitialFill witness are satisfied.

## CP203 / CP214 / CP218 reconciliation

CP203 remains controlling: public ownership and mutable aliases leave the guard bypassable; the exact native cache1 store is statically ordered after length inspection, but no same-occurrence public Open-to-InitialFill witness exists. CP214 correctly found no safe independent source increment and retains both the access-closure and original-witness gates. CP218 makes the migration actionable by mapping current direct-access files and migration classes; its 25-file source scan is narrower than CP203's 41-file application audit and is not a contradictory completeness claim. CP163's existing A-prefix behavior is preserved; its tests do not qualify cache1 continuation.

## Alternate ready work

For source work that can advance without this unresolved R300 ownership contract, take CP218's staged Open-directory decision integration: connect the already-reviewed pure collector in `src/model/operation_project_directories.hpp` to the actual `parseOperation` Open path in `src/model/operation_rescue.cpp`, retain ordered paths only in per-conversion `Ctx`, and do not apply filesystem writes or expose a public apply switch. Test effective-ID fallback, negative-ID no-op, multi-operation order/multiplicity, unchanged project tree, and non-consumption by `applyEffects`. This is a distinct, bounded source increment; it neither enables cache1 nor supplies original creator behavior.

## Scope

Converter-only static recommendation. No source, test, fixture, build, executable, deployment, GUI/app-control session, original process, mutable REA state, or shared recovery state was changed.
