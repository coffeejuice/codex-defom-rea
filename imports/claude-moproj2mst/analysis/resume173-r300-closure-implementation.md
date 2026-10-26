# CP173 R300 first-cut implementation: internal `opStep` closure

## Outcome and scope

The four exclusively owned engine files now replace the C++ internal `opStep(Project&, Operation&) -> DB_STEP_DATA*` return with `withOpStep`, a synchronous callback that receives the same nullable pointer and can return only `void` or `bool`. This implements the first source cut specified by `analysis/resume173-r300-barrier-review.md`; compileability remains unverified because this assignment excluded builds. It closes the named `opStep` raw-return route only. It does not install the R300 Stopping barrier, publish cache1, privatize `GuiModel::step`, or close other raw aliases.

`withOpStep` calls the existing `opModel` lookup at each former acquisition point, passes `model->step.get()` or null directly to the callback, adds no geometry refusal, and does not return the pointer or a reference. No callback stores its step argument or returns an escaping closure. The callback return concept restricts the result to `void` or `bool`.

## Exact migration

| File | Migrated behavior |
| --- | --- |
| `engine_internal.hpp` | Removed raw-return declaration and added the constrained `withOpStep` template. |
| `loader.cpp` | Removed raw-return definition. `inheritSimCtrl`, `loadSimCtrl`, `loadInterObject`, both MeshSettings readers, and `loadObjProperties` retain their null-false paths and original call order within boolean callbacks. `slotMaterialsFromMesh` retains its unchecked dereference after the mesh predicate, so a missing model still has the prior behavior. In `loadMaterials`, item ID/path mutation still precedes step acquisition. The native writer acquires immediately in its selected branch; the scratch writer acquires inside `captureWrite` at its prior point. Each selected writer makes one acquisition. |
| `loader_l2.cpp` | Furnace prologue retains nullable-step behavior, setting `tnow3` only for a nonnull step. |
| `db_load.cpp` | `syncMaterialItem` retains the nullable material lookup after item lookup. `syncFromModel` retains its initial `opModel`, missing-model return, material-manager lookup, then second model lookup for the step. Its complete ordered material/object/equipment/diagram sequence remains inside one synchronous callback, including nested `syncMaterialItem` calls and scratch writes. |

The nested `captureWrite` callbacks execute synchronously before their enclosing step callback returns; they do not retain the borrowed pointer. `syncFromModel` continues to hold its pointer across nested calls, matching the former local raw pointer's lifetime. These are code-review observations, not a general alias-safety proof.

## Static check and source identity

A scoped search of all C++ source and headers under `source/apps/moproj2mst/src` found **zero** declarations/calls matching `\bopStep\s*\(`. There is one `withOpStep` definition and twelve calls: nine in `loader.cpp`, one in `loader_l2.cpp`, and two in `db_load.cpp`. The six-export `include/moproj2mst.h` remains SHA-256 `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`, with `M2M_API_VERSION 1`. The untouched CP163 Stopping source `simcontrol300.cpp` remains SHA-256 `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`.

Post-cut SHA-256 values:

| File under `source/apps/moproj2mst/src/engine` | SHA-256 |
| --- | --- |
| `engine_internal.hpp` | `f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d` |
| `loader.cpp` | `c205ebd5404366ade24209ae52c0baf732aad8e9ff88b841e46f0f5227a2dcbd` |
| `loader_l2.cpp` | `1a6961f6b11d9e3536a52ecdf6b0cd237119d95f637f26b19b47883340d7cabe` |
| `db_load.cpp` | `acfe30074e85dde62f39c24bb36dbbe7d7a02d8902342983c97aca7939407b7c` |

No build, CTest, fixture comparison, freeze, GUI session, or live mutable REA operation was run under this source-only assignment. No test file was owned or edited. A test owner may use a non-connector test file after coordinating exclusive ownership; this implementation makes no test-path claim. Root still needs the focused targets named in the CP173 review and a source-stable full suite/freeze after integration.

## Remaining raw consumers and risks

Direct `GuiModel::step` access remains across `engine` and `ops`, including `loader.cpp`'s `newModel`/reader path, `boolean_lifecycle.cpp`, `l5_slots.cpp`, `l6_slots.cpp`, `ht_save.cpp`, and `heattreat_htwz.cpp`; `GuiModelAccess::withStep` also still exposes a nullable pointer inside its own callback. Other C++ internals still return raw model/SimControl/object pointers (`opModel`, `opSimCtl`, `slotObject`, `addObjectNo`, `GuiModel::simCtl/object/objectAt`), and the first cut does not gate previously obtained aliases or pre-effect Save/DB operations. The staged privatization and whole-operation Stopping gate remain separate work. Concurrent or nested reentry behavior remains unqualified. A source-bound compile and focused tests are required before asserting runtime parity; this static cut alone does not establish the barrier or reader-tail behavior.

Exclusive ownership of the four edited source files is released for root integration after this report.
