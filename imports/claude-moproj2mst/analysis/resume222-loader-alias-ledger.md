# CP222 loader/native construction alias ledger

## Scope and conclusion

This bounded inventory covers the requested loader and construction family only. It identifies raw native step, simulation-control, and object pointers acquired or installed by these paths. It does not claim that the global alias barrier is closed. The proposed destination is the finite model-owned named-operation API described in `resume222-editor-closure-architecture.md`; `GuiModelAccess::withStep` is the current callback seam and is insufficient as a final boundary because callers can capture its pointer.

## Per-site ledger

| Source / site | Current raw acquisition or replacement | Escape/retention evidence | Required named API dependency |
|---|---|---|---|
| `src/engine/loader.cpp:183-186` `newModel` | Creates shared `GuiModel`, dereferences public `m->step` for `set_model_unit`. | Returns shared model; initializes native step via public owner field. | `createModel(dim, unit)` / owner-only initialization transaction; retain lazy/shared identity semantics. |
| `loader.cpp:201-348, 973` | `withOpStep` callbacks receive mutable `DB_STEP_DATA*`; callbacks call native import/write helpers. | Callback syntax permits capture; native reader/writer calls consume synchronously at these sites, but callback type itself proves no non-retention. | Named import/serialization operations taking values/keys and `FileEffects`; confine raw step to implementation adapter. |
| `loader.cpp:418-432, 452-464, 505-507` inherited/DB object loading | `get_or_create(m.step.get(), no)` constructs/retrieves `LLOBJ_DATA*`; source via `sc->object`; destination and source passed to native copy helpers. | Local aliases; current code does not store them beyond each operation, but mutable raw object pointers cross helper calls. | `loadInheritedObject`/`copyObject` operation by object key and source model identity, preserving order/failure prefix. |
| `loader.cpp:566, 688, 756-769, 795-829, 911-922, 1028-1054` model queries/setup | Reads `objectCount`, `object(no)`, `simCtl()`, and `step`; mutates `simCtl` fields and raw object fields. | Pointers locally consumed, including `sc` and object pointers; no stored callback capture evident in listed sites. | Scalar/object-summary queries and named setup/control mutation; no raw getter in public C++ surface. |
| `loader_l2.cpp:58-89, 101-120, 249-288, 316-348, 377-415` | `opSimCtl` returns raw sim-control pointer; slot helper returns raw object; callbacks receive step/control pointers and mutate controls/objects. | Lambdas capture model/operation and receive native pointer; scoped locals include `sc`, object aliases. | `applyHeatTransferData`, slot/object query and named control update; callbacks retained only as private synchronous implementation detail. |
| `loader_l3.cpp:77-139, 383-461, 494-516, 542-569, 678-737` | Reads public `m.step`; gets `simCtl`, `object`; writes control fields and invokes equipment/native helpers with raw step/object. | Local raw aliases; `prepare`/finalization flows span multiple helper calls. No persistent capture established in this file slice. | Named dimension/equipment/control/step-finalization commands, with multi-field changes in one admitted operation. |
| `shapeoptim_load.cpp:83-130, 201-245, 265-301` | Dimension goes through `withStep`; loops retrieve object pointers, `modelB8_->step` mutated; DB-created object destination/source pointers used in copy. | Raw object references local; modelB8 shared owner retained by class for later method use. This is model retention, not proof of native pointer retention. | `setDimension`, shape-optimization object-copy/build and step-flag commands by keys; preserve `modelB8_` owner identity privately. |
| `cogging_load.cpp:86-118, 183-203, 219-260` | `simCtl()` and `object()` pointers; keyword reader receives `t.step.get()`; equipment writer receives `*w->step` and raw object. | Local model/object/control pointers; lambdas registered at 285 capture only values/refs in shown registration, but capture lifetime needs call-contract proof. | Cogging load/serialize/model-setup commands; private synchronous adapter around native reader/writer. |
| `multiblow_load.cpp:57-61, 74-92, 119-135` | `simCtl()` raw pointer, object count, `object()` pointer; native control mutation. | Locals only in inspected functions. | `initializeMultiBlow`, `objectSummary`/lookup and named slot update. |
| `postload_views.cpp:108-172` | `equipmentBytes` takes `DB_STEP_DATA&` and `LLOBJ_DATA*`; caller obtains step, control, object from model. | References/pointers passed across a serialization helper; outputs bytes before return. | `writeEquipmentKey(ObjectKey, target, effects)` returning owned bytes/effect; do not expose native refs. |
| `db_load.cpp:379-431, 482-628` | `withStep` supplies mutable step to DB readers; model replacement/load path; post-load loops fetch raw object pointers and mutate/query. | Native DB calls are synchronous; object aliases remain local. `loadDbStep` yields model pointer from shared ownership. | `readDbStep` / `loadDbStep` as owner operations with preserved null/failure semantics; object updates via keys. |
| `class_slots.cpp:43-68, 90-120, 148-185` | `model()` returns raw `GuiModel*`; `simCtl()` and `object()` aliases mutate controls/objects; base creation calls `newModel`, initializes simCtl; `setDimension` uses `withStep`. | Model lookup pointers are borrowed from operation shared owner; callbacks are immediate lambdas. No retained native alias shown. | `operationModel` identity internal to owner; named inherited-slot/control/setup/dimension operations. Keep shared ownership and predecessor selection semantics. |

## Callback and replacement findings

The examined family has two distinct alias classes: direct interior aliases (`simCtl()`, `object()`, `step.get()` or public `step`) and callback-delivered aliases (`withOpStep`, `withPropsStep`, `GuiModelAccess::withStep`). The latter remain capturable despite current `void`/`bool` return constraints. The family also creates/reloads step-bearing models, so replacement/reset ownership must be closed alongside getters. No persistent native pointer stored in a callback was positively identified in this bounded pass; absence in this pass is not a whole-program claim.

Existing native consumers are predominantly synchronous, but named API migration must preserve each call's mutation order and partial-failure behavior. Native helper call synchrony does not itself establish that the helper does not retain its argument.

## Exact focused tests required for this family

1. Compile-time/API visibility: these files cannot access `GuiModel::step`, `simCtl()`, `object()`, or raw-returning forwarding helpers after migration; no replacement or `.get()` route remains.
2. Model creation/reset/reload: dimension and unit initialization, nullable-step behavior, shared-model identity, replacement failure prefix, and reset invalidation.
3. DB/KEY load equivalence: first/last step selection, successful and rejected reads, object creation/copy ordering, inherited-slot behavior, and simulation-control mutations.
4. L2/L3/ShapeOptim/Cogging/MultiBlow: representative valid and unsupported paths, preserving exact native field writes, object-number mapping, and error ordering.
5. Post-load/equipment serialization: generated bytes/effect order match existing fixtures; no native pointer enters `FileEffects` or survives the operation.
6. Borrow/lifecycle regression: attempted retained step/control/object identity is impossible through the API; reset/reload between lookup and subsequent command cannot use stale native storage.

## Ownership boundary

This file is the only artifact written for this assignment. Source ownership remains with the CP222 integration owner. The listed source files are dependency sites, not an edit authorization or a claim that the proposed API exists. No source/tests/build/executable/deployment/original/GUI/shared state was changed; no tests were run.
