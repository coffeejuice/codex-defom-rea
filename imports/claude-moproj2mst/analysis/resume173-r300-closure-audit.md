# CP173 R300 parked Stopping barrier: raw-borrow closure audit

## Decision

A parked guard on `GuiModel` is **not sufficient while `GuiModel::step` remains a public `unique_ptr`**. Direct readers/writers can bypass `requireGeometrySyncAvailable()` and `GuiModelAccess::withStep`; `opStep` also returns an unscoped raw `DB_STEP_DATA*`. A barrier could protect a closed set of named top-level routes, but the current architecture does not establish that all supported consumers enter through those routes or that previously acquired aliases cannot be used.

This is a source-closure blocker, not a reason to weaken the cache guard. The smallest route that preserves the existing C ABI is to make the C++ `GuiModel::step` member private, preserve its storage/layout and existing C ABI, and convert internal direct accesses to a guarded synchronous borrow. Convert `opStep` to a callback/borrow helper rather than returning the raw pointer. Keep `Operation::engineModel` as the existing shared ownership/identity field; guard model-consuming operations and stop direct model dereferences that do not route through the gate. This changes internal C++ source access but need not change the converter's public C ABI. No evidence here establishes an external C++ binary-layout contract for `GuiModel`.

The audit is read-only. No source, tests, shared state, build, executable, GUI/app-control session, live REA or mutable analysis DB was changed or run. CP172's barrier plan is `analysis/resume172-r300-barrier-plan.md`, SHA-256 `b48334a4d53bf555a01f782240e8d6c5f4ed025caaa65cd4c473841d4cee1034`. Native and original qualifications remain as stated in CP171/172; no original runtime/public path claim is added.

## Inventory and owner map

The following are current source line numbers from the audited subtree. Ranges below are compact exact line sets, grouped by file; direct `step` tokens that are clearly **not** the `GuiModel::step` member are called out afterward. The inventory came from `rg` over `source/apps/moproj2mst/{src,include}` for `.step`/`->step`, then separate searches for `engineModel` and `opStep`.

### Direct model-step reads, aliases and writes

| File | Direct access sites | Owner / closure consequence |
| --- | --- | --- |
| `src/engine/boolean_lifecycle.cpp` | 26, 122, 128, 136 | Actual Boolean model cache and direct step writes; gate before dereference and use scoped borrow. |
| `src/engine/cogging_load.cpp` | 114, 260 | Temporary model and actual `w` model; the latter must enter the gate before KEY/equipment access. |
| `src/engine/cogging_save.cpp` | 344, 524 | Actual model step used for save/router values; gate before any derived read or output effect. |
| `src/engine/geometry_temp_prefix.cpp` | 40, 52, 79-80, 90-91 | Retained geometry token checks compare/read model step. Check unified model gate before dereference; do not let this geometry-specific record authorize Stopping. |
| `src/engine/gui_model_access.hpp` | 26 | Central synchronous step borrow. Extend this choke point; current method already checks the geometry-only guard before returning the borrow to its callback. |
| `src/engine/ht_save.cpp` | 90-91, 252-254, 291, 334, 512-513, 549-550, 682 | HEAT model save/property readers/writers; gate their public entry paths and convert raw use to scoped access. |
| `src/engine/l5_slots.cpp` | 213, 270, 276, 410-412, 478-479, 966, 970, 1042, 1046, 1119, 1176, 1209 | Loader/saver slot paths include direct mutable step use and nested SimControl step. All operation callbacks that can hit a parked model must refuse before their first field effect. |
| `src/engine/l6_slots.cpp` | 87, 89-90, 116, 385, 387, 407, 414-416, 445 | Dimension tests, property walks, dimension mutation, model-copy and slot save. Requires gate at entry and scoped borrow; mutation cannot run through a parked step. |
| `src/engine/loader.cpp` | 186, 200, 461, 504, 919 | Model construction/unit init, `opStep` implementation, object-copy destinations and DB-step load metadata. `opStep` is an exported-internal raw escape today. |
| `src/engine/loader_l2.cpp` | 105, 311 | Heat-treatment loader step and `opStep` consumer. |
| `src/engine/loader_l3.cpp` | 85, 92, 139, 414, 456, 494-495 | Heat-transfer/forming loader model reads, copies and writes; guard class-load entry before touching the actual step. |
| `src/engine/postload_views.cpp` | 129, 152 | Post-load view/equipment reads on actual model; gate before adding viewer or file effects. |
| `src/engine/props_sync.cpp` | 50 | Actual model-to-properties step binding; this is a raw accessor and must be guarded/scoped. |
| `src/ops/heattreat_htwz.cpp` | 67-68, 265-266, 496-497 | HeatTreat model dimension/step reads and SimControl writes outside the engine folder; gate at callback entry. |
| `src/engine/save_current.cpp` | 120, 342, 399, 534, 584, 587, 642-643, 730, 735 | Save-current helpers, KEY writes and object/material serialization. A Save gate must run before output effects; helpers also need their own guarded entry where callable directly. |
| `src/engine/save_l3.cpp` | 91-92, 145-146, 255 | L3 property and slot saves read actual step. Gate before property copy or output effects. |
| `src/engine/save_l6.cpp` | 229-230 | L6 save reads actual step; gate before save work. |
| `src/engine/scene.cpp` | 31-41 | Named model initialization reads/writes primary and secondary model step; guard both input/output models before first write. |
| `src/engine/shapeoptim_load.cpp` | 230, 239 | Inherited geometry source and cached model direct step access; gate before copy/store. |
| `src/engine/simcontrol300.cpp` | 375, 393, 470-472, 663, 679, 1078, 1130, 1203, 1251, 1318, 1340, 1495, 1566, 1592-1593, 1628 | Stopping coordinate getter and its geometry checks plus StepSize, meshing, Advanced and recovery routes. Gate public route starts before direct reads. Apply functions that consume the model must also reject. |
| `src/engine/simcontrol300_lifecycle.cpp` | 98, 214, 315 | Initial-fill construction, backup owner and Apply. Gate before page/state/backup effects. |
| `src/engine/viewer.cpp` | 466, 486, 505-506, 553, 1147-1148, 1165, 1169, 1176, 1251, 1257-1258, 1304, 1310-1311, 1317, 1819, 1837 | Geometry readiness, viewer model access, callbacks, frame producers and temporary-prefix operations. Ensure all viewer entry paths use a combined model availability check; their geometry barrier does not subsume Stopping. |

Other `.step` hits are not `GuiModel::step`: `src/model/htexpress_props.cpp:59` and `src/ops/heattransfer_props.cpp:18` write property-owned `HeatTransferStepData`; `src/engine/loader_l3.cpp:465`, `src/engine/loader_l2.cpp:105`, and `src/engine/save_l3.cpp:279` include property-owned `pr.step`; `src/engine/l5_slots.cpp:276,970,1046` also access nested `SimControl::step`. These still need entry gating if their owning callback can consume a parked model, even when the token itself is not `GuiModel::step`.

### `opStep` return and consumers

`src/engine/engine_internal.hpp:57` declares `DB_STEP_DATA* opStep(Project&, Operation&)`; `src/engine/loader.cpp:197-201` implements it by calling `opModel` then returning `m->step.get()`.

| Consumer | Sites | Use |
| --- | --- | --- |
| `src/engine/loader.cpp` | 208, 229, 253 | `inheritSimCtrl`, `loadSimCtrl`, `loadInterObject`; synchronous loads/mutations. |
| `src/engine/loader.cpp` | 306, 308 | `loadMaterials` write-back after material ID change; includes native or captured KEY writer. |
| `src/engine/loader.cpp` | 317, 328, 346 | Own/inherited mesh settings and object properties imports. |
| `src/engine/loader.cpp` | 970 | `slotMaterialsFromMesh`; synchronous material-list read. |
| `src/engine/loader_l2.cpp` | 311 | Furnace load-prologue SimControl update. |
| `src/engine/db_load.cpp` | 482, 609 | DB-load metadata mutation and `syncFromModel` material/object/key serialization. |

These consumers appear synchronous in the inspected call sites, but a guard on `opStep` only prevents acquisition after the barrier; it cannot invalidate a raw pointer previously returned. Converting the helper to `withOperationStep(p, op, callback)` with void/bool return, and auditing callbacks for capture/escape, closes this helper's post-return alias path. `loadMaterials` currently calls `opStep` twice inside the writer path; capture one scoped borrow for that synchronous operation rather than preserving duplicate unscoped acquisitions.

### `Operation::engineModel` uses

`src/engine/loader.cpp:174-178` is `opModel`'s lazy cache/create/return path and is a central candidate to refuse when the returned model is parked. Other users:

- `src/engine/boolean_lifecycle.cpp:26,35,46,119,122,128,136`: some identity/lifetime binding, but also direct `engineModel->step` read and step writes. These are real bypasses and need unified guard/scoped access.
- `src/engine/conversion_session.cpp:254`, `src/engine/simcontrol300_lifecycle.cpp:286`, `src/engine/simcontrol300.cpp:1590,1622-1623,1628`, `src/engine/simcontrol300_owner.cpp:50`: mostly owner identity and shared-owner checks; identity comparisons alone do not expose step bytes. Their model-consuming entrypoints still need a gate before any step read/write.
- `src/model/operation_info_cleanup.cpp:78,136,190,241,291`: cache presence/identity cleanup census. Inspect cleanup behavior before allowing a parked model to be detached or destroyed; a strong `engineModel` reference is a lifetime owner, not permission to read its step.
- `src/datafiles/mdt_write.cpp:584`: cache-presence decision only, but it occurs in a data-file writer and must be behind the Save/model-state gate if it can run for the active operation.
- `include/m2m/model.hpp:237` exposes the shared pointer in the C++ `Operation` struct. Keep ownership and identity semantics; do not replace it with a new public C handle for this task.

## Can closure preserve the ABI?

Yes for the documented converter C ABI: `GuiModel` and `Operation` are internal C++ source structures, and no public C function signature needs to change. Making `GuiModel::step` private preserves its member storage and object layout; replacing internal raw reads with scoped methods is a compile-time/source migration. `opStep` is declared in the internal `engine_internal.hpp`, not the C ABI; replacing it with a callback helper also preserves external C signatures.

This does **not** preserve source compatibility for any external C++ consumer that directly accesses `GuiModel::step`; no such supported external consumer or binary layout promise was found in the inspected headers. If the project intends that C++ access to be public ABI, the audit cannot certify closure without a compatibility adapter that still prevents alias escape. Merely keeping `step` public and adding more `requireAvailable` calls does not close the surface.

## Minimal patch order

1. **Close the type seam first.** Move `GuiModel::step` to private storage without changing its declaration type/order/layout; add the model-owned Stopping barrier plus one private `requireModelUseAvailable()` check that composes with the existing geometry guard. Make the existing `GuiModelAccess` and a narrow Stopping installer the only friends. Keep barrier payload nonowning and install no-throw; don't touch native step layout/C ABI.
2. **Replace borrowed-pointer APIs.** Change `GuiModelAccess::withStep` to call the composed gate before borrowing. Replace internal `opStep` with `withOperationStep` (or equivalent) so no helper returns `DB_STEP_DATA*`; preserve callback return constraints and review callback captures. Gate `opModel` before returning an existing cached model, while retaining lazy creation behavior for an unparked model.
3. **Migrate every direct access owner in the inventory.** Convert each to a scoped borrow or a guarded `GuiModel` method. Gate the top-level callback before any project/page/property/file effects where an operation can consume the model. This includes Boolean, H/T save/load, L5/L6 slots, cogging, scene/post-load, props sync, ShapeOptim, viewer, Stopping lifecycle, KEY/save helpers and DB sync. Keep owner-only `engineModel` comparisons where harmless, but guard cleanup paths that could detach the actual pending model.
4. **Close aggregate routes.** Add availability checks at `fillStopping3DInto`/`applyStopping3D`, initial editor construction/apply, `loadDbStep`/reset and raw step access, display selection/refresh, Save-current/project before effects, and all loader callbacks before first direct write. `reset()` must refuse while parked; model destruction remains the only retirement path. No `Operation`/project/save path may clear or replace the owner behind the barrier.
5. **Prove the search is closed.** Re-run scoped `rg` for direct `GuiModel::step` access and `opStep(`. Remaining hits must be either inside the narrow access implementation, a documented test fixture, or proven non-model `.step` owners; no returned DB_STEP_DATA pointer may be cached outside a synchronous callback. Review `engineModel` dereferences separately. This is a source audit gate, not an ABI claim.
6. **Add focused barrier tests and then build.** Test the cache0 length-zero fixture's barrier stage, every API family above, sequential retry refusal before cache-hit return, reset/reload/save/display refusal before effects, destruction/step-owned A cleanup, and unchanged CP161/163 negatives. Only after independent source review and a clean borrow audit should root run focused Stopping + snapshot/lifecycle tests and the source-stable suite/freeze.

## Evidence limits and conclusion

The audit found a concrete bypass surface and a path to close it without changing the C ABI, but it did not execute tests or prove every caller/alias. Static native evidence supports a conservative parked barrier after cache1. It does not support resuming the same call or public Open/Save equivalence. The original same-occurrence reader tail remains necessary for native fidelity claims, and the current root GUI-release gate still forbids collecting a new runtime witness. Until the C++ raw borrow is private/scoped and the aggregate entrypoints are gated, the zero-cache implementation remains blocked at CP163's after-A refusal.

## Inspected source binding

The audit is for current source in this subtree. Principal interface hashes at the prior read-only design checkpoint: `engine.hpp` `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f`; `gui_model.cpp` `26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45`; `gui_model_access.hpp` `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`; `engine_internal.hpp` `d2ed7167152588552c4023a10d0f01e32fbbc677a42058e2e9cf834971ee67d8`; `simcontrol300.cpp` `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`. No file listed here was modified.
