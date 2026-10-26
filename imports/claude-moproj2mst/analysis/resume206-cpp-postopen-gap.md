# CP206 converter C++ postOpen gap

## Scope and evidence boundary

Read-only source audit of the current converter. CP205's original v11.0.2 B98 capture locates the crash during `postOpen`'s direct `2signalRefreshMainView()` delivery, before the outer Open returns. CP206's frozen CP201 `--write-save` candidate succeeds and writes its MST/project artifacts, but that is not a paired original Open/Save/Close comparison. This audit does not assign a C++ semantic type to native `O`, `H`, or `P`, and makes no implementation or runtime claim.

## Public Open representation

The process-level `M2M_FLAG_WRITE_SAVE` route in `source/apps/moproj2mst/src/core/convert.cpp` creates an `engine::ConversionSession`, gets its `Project&`, then runs the source-labeled Open stages: `prepareProjectModel(db)`, `initProject(...)`, `commitProjectModelConstruction()`, `openProject(p,file,ctx)`, and `commitProjectModelSelection()` after accepted-project admission. `activateSubProject(p,0,ctx)` follows. This is a synchronous sequence of calls and guarded commits, not a public Open/postOpen event object or replayable event queue.

`openProject` is declared in `include/m2m/model.hpp` and implemented in `src/moproj/read.cpp`; it represents project file parsing and associated model/database reads. The Open wrapper then explicitly performs model selection and subproject activation. There is no named `postOpen()` counterpart in this converter path, and no `RefreshMainView` or `2signalRefreshMainView()` dispatch among these stages.

## Nearest current state and owner proxies

- `ConversionSession` in `include/m2m/engine/conversion_session.hpp` owns the candidate `Project`, model-manager owner and session-scoped engine state. `liveProject()` and `modelManagerOwner()` expose these adapter owners.
- `prepareProjectModel` creates/registers a Mo `DbHandle` in the session `ModelManager` and assigns it to `Project::dataDb`. `commitProjectModelConstruction` checks that exact registered child and installs its `moProject` backlink. `commitProjectModelSelection` resolves the actual child identity to its list ordinal and selects it. These are the closest current representations of native model registration/ownership and Open's selected-project step.
- `DiagramLifecycleEvents` in `include/m2m/engine/lifecycle_events.hpp` records logical receiver IDs and delayed model-update timers for diagram `setModel` behavior. It supports `modelAttached` and later due-timer delivery, but not the synchronous `2signalRefreshMainView()` notification or arbitrary Qt direct receiver invocation.
- The converter's `Project` is an application-domain model, while the captured native `O/H/P` are raw object/field-derived values in one original callback path. Existing source evidence does not establish that any of the above owners is the same semantic object as `O`, `H`, or `P`. In particular, `Project::dataDb` and the session `ModelManager` cannot be equated to the captured `H+0x38` owner from current evidence.

## Narrowest missing architectural transition

The demonstrated representational gap is between successful Project model binding/selection and the original postOpen synchronous notification/callback boundary. The converter has a committed Project-to-model relationship and selected ordinal, but no explicit postOpen phase that dispatches the refresh notification against a modeled receiver set and makes callback-visible state available before Open completion. Thus candidate success can pass through a state where original execution entered a callback and faulted.

That locates the narrowest missing architecture, but does **not** yet establish a valid fix or the desired failure contract. CP205 leaves unresolved why `[O+0x30]+0x38` was zero at callback use: producer returned null, state was later cleared/replaced, or the chain changed. Until that producer/field transition is traced and its input-domain relation to the public m2m Open profile is established, do not alias the null native owner to `Project::dataDb`/`ModelManager`, manufacture an event semantic, or decide to normalize the original failure into successful conversion.

## Source locations consulted

- `source/apps/moproj2mst/src/core/convert.cpp`: session construction and Open-stage order (around lines 195–250).
- `source/apps/moproj2mst/src/engine/conversion_session.cpp`: `prepareProjectModel`, `commitProjectModelConstruction`, `commitProjectModelSelection`, and diagram binding commits (around lines 278–340).
- `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp`: owner accessors and guarded Open commits.
- `source/apps/moproj2mst/include/m2m/engine/lifecycle_events.hpp` and `src/engine/lifecycle_events.cpp`: timer-based diagram lifecycle representation.
- `source/apps/moproj2mst/src/moproj/read.cpp`: project XML/model read path.
