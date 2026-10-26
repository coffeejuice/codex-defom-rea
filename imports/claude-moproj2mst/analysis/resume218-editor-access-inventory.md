# CP218 R300: `GuiModel::step` access-closure inventory

## Purpose and decision

This note turns CP203/CP214's “41 files / no safe cache1 increment” finding into an ordered migration inventory. It does not reopen whether CP163's A-only prefix is valid: keep its immediate `CoordinateSync` return after A publication. A cache1 publication remains unsafe until every direct member access, mutable alias, and earlier-acquired pointer is closed.

A current bounded scan of `source/apps/moproj2mst` for C++ `.step` / `->step` references finds **25 production source/header files** and another **43 test files** (68 total). CP203's 41-file application count is retained as its broader audit result; the differing count here is a narrower `src/` search, not evidence that prior coverage vanished. A regex count is a locator, not a proof of completeness: aliases, member forwarding, template helpers, and externally compiled C++ users still need semantic review.

## Concrete production sites and migration classes

The examples below are direct source locations from the current tree. Each class requires a different replacement contract; simple textual substitution to `withStep` is not enough.

| Access class | Concrete sites | Migration requirement |
|---|---|---|
| Public storage / public convenience reads | `include/m2m/engine.hpp` declares public `std::unique_ptr<DB_STEP_DATA> step`; `src/engine/gui_model.cpp` has `objectCount()` read the member directly (CP203); `src/engine/viewer.cpp:553` reads dimension through `m.step`. | Make storage private; replace public convenience reads with guarded value snapshots or a model-owned operation. Audit external C++ callers and aggregate/init assumptions before changing the type. |
| Lifecycle and loader writers | `src/engine/loader.cpp:186,464,507,922` mutates unit/object/step fields and passes `step.get()` into object construction; other loader paths in `loader_l2.cpp`, `loader_l3.cpp`, `shapeoptim_load.cpp`, `cogging_load.cpp` and `postload_views.cpp`. | Preserve construction, load/reload, failure-prefix, and finalization order. Storage replacement needs a narrowly privileged owner API; construction pointers must not outlive the call. |
| Simulation/editor writers and readers | `src/engine/simcontrol300.cpp:663,679,1078,1130,1203,1251,1318,1340,1495,1566,1592-1628`; `src/engine/simcontrol300_lifecycle.cpp:98,214,315`; `src/engine/boolean_lifecycle.cpp:122-136`. | Separate immutable scalar snapshots from mutation commands. Multi-field mutations need one borrow covering validation and writes, with the same model-owned Stopping/geometry admission. |
| Save and serialization consumers | `src/engine/save_current.cpp:356,436,543,586-595,748,756`; `save_l3.cpp`, `save_l6.cpp`, `ht_save.cpp`, `cogging_save.cpp`, `l5_slots.cpp`, `l6_slots.cpp`. | Preserve writer ordering and callback/file effects; ensure borrowed data is consumed synchronously and not stored in deferred callbacks or closures. |
| Viewer / geometry / identity aliases | `src/engine/viewer.cpp:466,486,505,1147-1176,1251-1258,1304-1317` stores and compares raw step identity in frames/evidence; `src/engine/geometry_temp_prefix.cpp:52,79-91` records and later validates a step pointer; `src/engine/engine_internal.hpp:72`, `gui_model_access.hpp:22-27`, and `props_sync.hpp:36` pass raw pointers to callbacks. | Replace lifetime identity with an unforgeable lease/token or owner generation; do not keep `DB_STEP_DATA*` in frames/evidence across calls. Callback APIs must prevent capture/return escape or remain private to tightly audited synchronous owners. |

Additional direct-reference production files in the current scan (requiring the same per-site read/write/escape classification) are `src/model/htexpress_props.cpp`, `src/engine/scene.cpp`, `src/engine/cogging_save.cpp`, `src/engine/l6_slots.cpp`, `src/engine/cogging_load.cpp`, `src/engine/l5_slots.cpp`, `src/engine/ht_save.cpp`, `src/engine/save_l3.cpp`, `src/engine/save_l6.cpp`, `src/ops/heattreat_htwz.cpp`, and `src/ops/heattransfer_props.cpp`. The remaining production scan files are listed above in their relevant class. These names are a file-level queue, not a claim that all accesses within them have been audited.

### Escape and bypass sites that block a guard

- `GuiModelAccess::withStep` and `withOpStep` pass mutable raw pointers. A callback can capture one; the helper's void/bool return constraint does not prevent capture. Existing aliases acquired before a pending state remain usable.
- `Operation::engineModel` exposes shared model ownership. A caller can retain the model and later access the public member.
- Mutable `simCtl()`, `object()`, and `objectAt()` results create parallel alias paths. The geometry admission around those methods does not close an alias retained before admission changes.
- Viewer frame/evidence records retain step identity past the borrow; geometry temporary evidence also ties a raw step to a model and reader. These are not merely transient field reads.
- Public `unique_ptr` permits reset/replacement as well as read/write, so a private field plus read-only accessor alone is insufficient.

## Smallest safe closure slice and dependency order

There is no safe one-file or one-caller slice that admits cache1. The smallest *useful staged migration* is one API seam plus one dependency-closed call-site family at a time, while the barrier remains closed throughout. A staged commit is acceptable only if the public member remains and cache1 stays refused until the final audit.

1. **Define the ownership contract first.** In `engine.hpp` / `gui_model.cpp`, specify private step storage, owner-only replacement during construction/load/reset, value-based read access, and a scoped borrow whose type/lifetime makes escape difficult to express. Define how the common Stopping/geometry gate tracks active borrows and serializes pending-state publication. Decide how `simCtl` / object aliases and `Operation::engineModel` are covered. Add API-level unit tests for nested/nullable access, mutation, replacement rejection while borrowed, pending-gate behavior, and compile-time non-return/non-copy constraints if the language/API permits. Do not migrate callers or enable cache1 until this contract is reviewed.

2. **Migrate storage writers and lifecycle owners.** Move constructor, loader, reload, reset and destruction flows first (`gui_model.cpp`, `loader*.cpp`, `postload_views.cpp`, shape-optimization and Cogging load paths). These establish exactly who may replace the step and when. Add focused load/reload/reset lifetime tests before moving consumers.

3. **Migrate synchronous mutation owners.** Move `simcontrol300*`, Boolean lifecycle, object/slot/geometry helpers, heat-transfer property writers and property sync. Keep each admission check and its associated writes under one owner operation; test Stopping/geometry pending rejection and unchanged field/effect order. This is the first slice that can exercise the proposed barrier but still must leave cache1 refused.

4. **Remove persistent aliases and deferred escapes.** Convert viewer frames, geometry evidence, temporary prefixes, and any stored callback captures to generation/lease identities or owner-mediated revalidation with no raw pointer retained between calls. Add lifetime tests across reset/reload, pending transition, nested calls, and stale frame/evidence reuse.

5. **Migrate read-only consumers and Save.** Move scene/viewer snapshot reads, DB helpers, and Save/serialization families. Preserve output bytes, writer order, error prefix, and synchronous consumption. Test existing Save and reader families, including null/missing-step paths and reentrant callbacks.

6. **Close external and test callers, then prove closure.** Search all app production + tests + public C++ consumers for direct storage, `.get()`/`reset`/assignment, forwarded raw pointers, callback captures, `objectCount`, simCtl/object aliases, and retained owner handles. Use compile-time visibility failures to make future direct member use fail. Focused API/lifecycle/alias tests precede focused editor300 cached-stopping tests and DB/query/viewer/Save regressions; only then run the required source-stable full suite. Keep CP163's cache1 refusal until the closure review and same-occurrence original witness both pass.

This dependency order is driven by replacement ownership first, then mutations and retained aliases, then readers/writers. Migrating read-only callsites first would leave reset/replacement and stale aliases able to invalidate the supposed guard.

## CP203 / CP214 reconciliation

CP203 remains the controlling static audit: public `unique_ptr`, mutable pointer accessors, `objectCount()`, shared model ownership, callback escape, earlier aliases, 41 source files, and the exact native cache1 store after length inspection. CP214 correctly concludes no independently safe source increment remains and requires full access closure plus one same-occurrence original path witness. This inventory adds current file-level access classes, representative line anchors, a narrower reproducible count (25 production source/header files under `src/`), and an ordered migration/test plan. It does **not** relax either CP214 gate or claim that callback wrappers already provide closure.

## Scope

Static read-only inventory only. No source or test edits, builds, test execution, GUI/app-control, original process, REA mutable session, or shared state changes were made.
