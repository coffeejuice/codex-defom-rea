# CP136: property-table owner lifetime

## Conclusion

The original does not establish a weak/QPointer owner for `GuiModel+0x230`. It establishes an owning edge: `SfStepData` stores the `SfObjectManager*` at `+0x230`, and reset calls `0x140c902c0`, which invokes the object's virtual deleting destructor at `vtable+0x18` with delete flag `1`, then clears the slot. See `source/docs/re/moproj2mst/mdt/L0/spec_A.md:79-84,435-436`, `source/docs/re/moproj2mst/mdt/L0/spec_C.md:207-218`, and retained decompile `evidence/gui_mo/s1/mdt/L0/C/c7a.c`, `FUN_140c902c0`.

Thus a `GuiModelGuard` is a source-closed **adapter hardening** for the shared-state alias case, but is not a recovered native ownership behavior. The current adapter exposes `GuiModel::doeStepData` as `shared_ptr<StepDataState>`, whose `unique_ptr<PropertyTableState>` can survive the model if a caller retains that alias. `PropertyTableState` currently retains a raw `GuiModel*`; the existing guard's weak token can make `owner()` return null once the model's member lifetime ends (`engine.hpp`, `gui_model.cpp`). That prevents a stale identity from passing the finalizer's foreign-owner check after address reuse.

For the strict native-parity implementation, do not describe or justify the change as QPointer emulation, and do not make it a new behavioral contract. The retained native evidence instead says the manager is deleted by its model owner on reset. The weak guard is reasonable only as explicitly adapter-local protection for the extra public `shared_ptr` alias capability. It changes only that non-native detached-alias case; it does not alter the valid model-bound path or the C ABI. The `PropertyTableState` C++ layout still changes, so any C++ ABI compatibility assumption needs the normal integration review.

## Lifetime details and limits

- Native reset order is viewer clear, `+0x30` cleanup, DB step clear, delete `+0x230`, then set `+0x100` to zero (`spec_C.md:210-218`). This proves explicit reset deletion; the consulted material does not establish a Qt `QPointer` relationship for `SfObjectManager`.
- In the adapter, `GuiModel::reset()` calls `doeStepData->resetPropertyTable()` after clearing the step, which deletes the table even while a `StepDataState` alias remains. The proposed guard's special behavior is therefore full model destruction with an externally retained `doeStepData`, not ordinary reset (`src/engine/gui_model.cpp:195-208`; `include/m2m/doe_stepdata.hpp`).
- The lifetime token is declared before other model members and consequently expires after them are destroyed. Its comment explicitly excludes unrecovered reentrant destructor callbacks (`include/m2m/engine.hpp`; `src/engine/gui_model.cpp:36-46,158-169`). The guard does not extend model lifetime.
- The exact regression described in CP135—retain `shared_ptr<StepDataState>` across model scope exit, then ensure expired owner rejects finalization against a replacement model before effects—would validate this adapter safety property. It would not validate a native QPointer or a native detached table state.

## Decision for CP135

If the project accepts adapter-only lifetime hardening, the weak guard is the smallest source-closed cut and should be labeled accordingly. If this change is being presented strictly as recovered original semantics, leave the raw owner contract unchanged and keep detached aliases outside supported behavior. I found no native evidence that resolves that policy choice; no source, test, build, GUI or mutable REA state was changed or used.
