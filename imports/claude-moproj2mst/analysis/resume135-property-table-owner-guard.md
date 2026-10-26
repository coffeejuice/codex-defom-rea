# CP135 PropertyTableState owner-lifetime cut

## Finding

No later evidence/source closes populated property-cell semantics; the GUI item witness from CP131 is still required before implementing check-state reads/writes. A smaller, source-closed private ownership cut is available independent of that GUI path: make `PropertyTableState`'s nonowning `GuiModel` identity a `GuiModelGuard` rather than a raw pointer. `GuiModel::doeStepData` is a public `shared_ptr<StepDataState>`, so an external alias can keep the `StepDataState` and its uniquely owned `PropertyTableState` alive after `GuiModel` destruction. Today `PropertyTableState::owner()` returns the stale raw pointer, and finalization's owner check compares it with the supplied model.

This is an adapter lifetime hardening cut only. It does not change native Qt item behavior, admit populated cells, alter current-object refresh order, or release any public guard.

## Current owner chain and exact risk

- `GuiModel` owns its normal Doe step-data reference through public `std::shared_ptr<doe::StepDataState> doeStepData` (`include/m2m/engine.hpp`, field comment: lazy imported property/state tables).
- `StepDataState` owns `std::unique_ptr<PropertyTableState> propertyTable` (`include/m2m/doe_stepdata.hpp`). `readStepData` lazily allocates the table for the actual model and passes it to `finalizeStepDataPropertyTable` (`src/doe/stepdata.cpp`, after Object child import).
- `PropertyTableState` stores `engine::GuiModel* const owner_` and its inline `owner()` returns that raw pointer (`include/m2m/doe_property_table.hpp`). `finalizeStepDataPropertyTable` checks `table.owner() != &model` before snapshot, current-object refresh, or cell queries (`src/engine/viewer.cpp`, current finalizer).
- A caller can retain a copy of `model.doeStepData`; after the model's full destruction, that shared alias still retains `StepDataState` and its `PropertyTableState`. The owner pointer no longer designates a live object. If a replacement model later occupies the same address, raw pointer equality could make this stale table appear to belong to it; even without address reuse, public `owner()` exposes a dead address as though live.
- `GuiModelGuard` already provides the intended nonowning identity semantics: it holds a weak reference to the model lifetime token and `get()` returns null after the `GuiModel` lifetime ends (`include/m2m/engine.hpp`, `src/engine/gui_model.cpp`). It does not keep the model alive. The lifetime token is declared before model fields and destroyed after them, so it expires after full member destruction.

The claim is limited to the candidate's explicitly shared `doeStepData` ownership. It does not assert a native Qt ownership analogue for aliases retained by external adapter code.

## Smallest CP136 source/test ownership

If root assigns this cut, reserve only:

- `source/apps/moproj2mst/include/m2m/doe_property_table.hpp`: store an `engine::GuiModelGuard`, initialize it from the owner, and have `owner()` return `ownerGuard_.get()`. `engine.hpp` forward-declares `doe::StepDataState` and does not include `doe_property_table.hpp`, so including the engine declaration in this header does not create the current StepData include cycle; root should still run the compile/test gate after assignment.
- `source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp`: add one regression that retains a `shared_ptr<StepDataState>` past a scoped model, confirms the retained table remains allocated but `owner()==nullptr`, then calls `finalizeStepDataPropertyTable` with a live replacement model and confirms early foreign-owner refusal with no viewer, query-prefix or count effect.

No `viewer.cpp`, `GuiModelAccess`, native layout, C ABI, CMake, or Qt table interface change is required. The public owner accessor keeps its pointer result type; it simply returns null when the original model has expired. The `PropertyTableState` C++ adapter layout changes from one raw pointer to a weak guard object, so validate repository-level C++ ABI assumptions during root's normal test/ABI review; no native `SfObjectManager`/GuiModel layout changes are implied.

The test must not call `GuiModel::reset()` before destruction, because reset intentionally deletes the `+0x230` table and would fail to exercise the shared-alias lifetime case. A plain scope exit with the retained `StepDataState` copy isolates the exact owner edge. Do not force allocator address reuse; `owner()==nullptr` and rejection against a new model are sufficient.

## Remaining GUI-dependent boundary

After owner liveness is hardened, `itemIsAbsent` still reports only constructor-empty/negative-index absence and refuses for an otherwise populated cell (`src/doe/property_table.cpp`). No code or test without GUI item evidence can establish the actual table generation, eligible row-to-View mapping, item type/role/check-state/text, or whether inspecting/changing a populated cell signals/re-enters callbacks. Keep this refusal. The owner guard does not promote the existing no-item model into a complete Qt property-table implementation.

## Evidence and limits

Reviewed current `engine.hpp`, `gui_model.cpp`, `doe_stepdata.hpp`, `doe_property_table.hpp`, `stepdata.cpp`, `viewer.cpp`, `property_table.cpp`, and focused property-table tests. No source edits, build, GUI operation, original execution or mutable REA state was used. This report supersedes only CP131's statement that no source-closed local ownership cut remains; its populated-cell witness requirement is unchanged.
