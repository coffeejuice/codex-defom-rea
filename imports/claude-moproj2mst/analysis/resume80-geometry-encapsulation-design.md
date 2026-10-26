# CP80 staged GuiModel encapsulation design

## Objective and ABI boundary

The geometry barrier is model-owned and its `GuiModel` member functions now reject parked work. Raw state access still exists in `engine.hpp`, so the guarantee currently applies to guarded operation entrypoints, not every path that can reach the model's native step, object, reader, or display state.

The CMake target defines the shared `moproj2mst.dll` ABI in `src/dll/abi.cpp`; exported `M2M_API` functions are C functions and do not expose `GuiModel` or `DB_STEP_DATA`. `m2m_core` is linked statically into that DLL and the CLI. Making `GuiModel` data members private or adding non-exported access helpers does not change its object layout if member order/types stay fixed, nor the C ABI. It will change C++ source access and may affect undocumented C++ consumers of `engine.hpp`; no exported C++ `GuiModel` ABI is visible in the inspected target configuration.

`GuiModel` itself is not a native `DB_STEP_DATA`; preserve the native `DB_STEP_DATA` layout and all C exports. As a low-risk first stage, retain the exact `GuiModel` member order/types and change only access control plus internal callsites.

## Direct state surface and known bypasses

A broad source scan found 454 references matching `GuiModel` step/DB/viewer/table/object access across 40 files. This is a syntactic inventory, not a count of writes. A narrower engine scan found 138 step-related references; it includes reads, so it also is not a write count. The callsites are spread across loaders, L5/L6 callbacks, operations, save code, simulation control, DOE and viewer code. This is too broad for a single mechanical substitution.

| Surface | Representative current access | Why entry guards alone do not close it |
|---|---|---|
| Raw model step | Public `std::unique_ptr<DB_STEP_DATA> step` in `engine.hpp` | Callers can mutate the real step directly or retain `step.get()` before parking. |
| Object capability | `object()` / `objectAt()` gate then return mutable `LLOBJ_DATA*` (`gui_model.cpp:210,217`) | A returned pointer retained before parking remains usable afterward. |
| Sim-control capability | `simCtl()` gates then returns mutable `Cls_1401e7b10*` (`gui_model.cpp:179`) | Previously returned pointer is not revoked. |
| DB reader capability | `modelDbReader(const GuiModel&)` returns mutable reader pointer (`db_load.cpp:247`) | It can be used to call mutable DB read/cache routes while a model cursor is parked. |
| Direct DB read | `readDbEntry(..., false)` writes `step->u1b58` and enters native DB read before `viewerRebuild` (`db_load.cpp:389-409`) | No entry gate; `resetViewer=true` happens to stop earlier at `viewerClear`, but false bypasses it. |
| Object removal | `deleteObject` calls `step_delete_object` before its conditional `pdie`/`setPdie` calls (`cogging_save.cpp:253-267`) | A non-PDIE deletion does not reach the gate; a PDIE deletion may throw after deleting. |
| Display acceptance | `displayShow` updates main model/display links; `displayRefreshScene` calls it before `fit` reaches guarded refresh (`viewer_camera.cpp:401-417`) | A public display call can leave links changed even though the later geometry refresh refuses. Loader slot `showModel` points to `displayShow` (`class_slots.cpp:176`). |
| Dimension callback | `g_loaderSlots.setDimension` writes directly through `m.step` (`class_slots.cpp:181-183`) | Callback bypasses guarded model methods. |
| Other direct writers | `scene.cpp:35-41` stores operation/model values; `boolean_lifecycle.cpp:128,136` writes `u1b4c`; `l5_slots.cpp:370,478,1072,1209` writes `u1b4c`; `l6_slots.cpp:414-416` copies material/phase state; `loader.cpp:919` stores current step number. | These are internal callback/frontier writes through the public field and require classification/entry guards if the contract is a model-wide park. |

The public `viewer`, `db`, table, and viewer-mirror members are also mutable. Test code intentionally swaps `viewer` to demonstrate that the barrier survives it. That is compatible with the current viewer-entry guard, but it is not a frozen-model contract.

## Staged migration

### Stage 1: guard named operation frontiers

Use the existing private `requireGeometrySyncAvailable()` as the common check. Add it at the top of every public or callback entry that can mutate the parked step, its geometry objects, reader state used by the frame, or display association. Do not place the only check after an earlier model write.

Immediate frontiers evidenced above:

- `readDbStep`, `readDbEntry`, and `modelDbReader` access/mutation route.
- `deleteObject`, loader `setDimension`, and the project `scene` writes.
- `displayShow` and `displayRefreshScene` before changing display links.
- Boolean/L5/L6 model writers and any future callback that has a `GuiModel&`/pointer and uses raw fields.

This stage keeps all current field syntax and object layout. It closes known operation paths but cannot revoke a raw pointer acquired before the barrier. Add a focused test for each frontier before expanding the allowlist.

### Stage 2: centralize direct member access

Make the mutable model members private without changing their order or types. Keep the current C ABI untouched. Move internal reads/writes to a single non-exported `GuiModelAccess`/`GuiModelMutation` layer, declared in an internal header and friended by `GuiModel`; all other internal functions use named read/write operations. The gate should be part of acquiring a mutation capability, not a caller convention. Read-only code should receive const views where possible.

The migration should proceed by dependency groups already present in the scan: DB/readers, object/loader callbacks, viewer/display, operation writers, and save/read-only code. An `rg` allowlist per stage can ensure raw `m.step`, `m.viewer`, etc. disappear from the converted files without imposing an unrelated whole-repository rewrite.

Do not provide conversion operators that hand out an untracked raw pointer from the access layer. For calls that need a native pointer synchronously, use a guarded callback (`withStep`, `withObject`, or an operation-specific method) so the validity check occurs immediately before the call. A closure can still capture a pointer; code review and the raw-access allowlist remain necessary. If strict runtime revocation of already-extracted pointers is required, an ordinary C++ raw pointer cannot supply it; callers must stop receiving raw pointers or use a resolvable handle/proxy at each access.

### Stage 3: replace long-lived mutable aliases

Migrate saved `DB_STEP_DATA*`, `LLOBJ_DATA*`, sim-control pointers, and reader pointers to short-lived handles that resolve against the owning model/generation on each use. For object references, key by the actual model plus object identity/number and verify it still resolves to the same live object. Do not infer a step or object generation from an address alone. Native functions that retain pointers asynchronously need separate lifetime qualification before conversion.

This is the stage needed to close the pre-held-pointer escape; changing a method guard cannot do it. Preserve synchronous native call order while resolving immediately before each call, and refuse when the owner or object has changed.

## Test gates

1. **Entry-frontier test:** reuse the pending directory fixture; call `readDbEntry` with `resetViewer=false`, `deleteObject`, loader `setDimension`, and `displayRefreshScene`. Each must refuse before any DB_STEP_DATA bytes, object count, display links, or table state changes. First implement the smallest case for `readDbEntry`, currently the clearest direct bypass.
2. **Access-layer test:** acquire a supported step/object lease before parking, park the model, then require lease resolution to fail before exposing a pointer. This validates the new capability contract; it cannot validate writes through legacy raw aliases.
3. **Compile-time/source gate:** after field privatization, compile all targets and tests and require a zero-hit raw-member scan outside explicitly allowed `GuiModelAccess` and adapter implementation files. This protects newly added callsites from reopening the bypass.
4. **ABI check:** compare `moproj2mst.dll` exports and public `moproj2mst.h` declarations before/after. Verify native `DB_STEP_DATA` size/offset assertions remain unchanged. The `GuiModel` C++ layout can remain byte-for-byte unchanged by retaining members/order/types.

No source changes, builds, tests, GUI actions, or mutable REA actions were made for this design review. The exact inventory is bounded to source matches and the identified entrypoints; it does not claim every transitive application callback has been callgraph-audited.
