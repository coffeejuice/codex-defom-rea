# CP227 loader leaf design

## Recommendation

Migrate one bounded loader leaf: `loader.cpp`'s `newModel` unit initialization (`source/apps/moproj2mst/src/engine/loader.cpp`, around CP222 ledger lines 183–186) to a named `GuiModel::createModel(dim, unit)` owner operation, conditional on CP226 landing that factory contract or the integration owner supplying it. This is a small direct construction consumer with no callback escape: it creates the shared model, performs `set_model_unit` on its owned step, then returns the model. It is independent of CP226's dimension command and its owned files (`engine.hpp`, `gui_model.cpp`, `class_slots.cpp`, `l6_slots.cpp`, and tests).

The leaf must preserve the exact sequence and failures: model construction/allocation first; unit setter next; return only after setter completes. Do not translate this into reset, initialize GUI defaults, or alter shared model identity. The current public `m->step` access is the raw alias to remove. If the factory's behavior/exception ordering is not established by source, treat it as a dependency and do not implement a speculative wrapper.

## Files and verification

Production ownership: `source/apps/moproj2mst/src/engine/loader.cpp` only, after the owner API is available. Tests: existing loader-focused target `m2m_mdt_loader` / registered test name to confirm from configured metadata; add the narrow `newModel` unit-initialization assertions there only if a suitable fixture already exists. Otherwise request a dedicated test file from integration owner, avoiding CP226-owned test files. Verify factory failure is not swallowed, successful model's unit matches the prior native setter outcome, and the returned shared owner is the same initialized model. Keep public C ABI untouched; this is internal C++ only.

Evidence: `analysis/resume222-loader-alias-ledger.md` identifies `loader.cpp:183-186` as direct public-step construction initialization and calls for `createModel(dim, unit)` / owner-only initialization while retaining lazy/shared identity. `analysis/resume226-dimension-owner-command-contract.md` covers only dimension mutation and explicitly does not provide a general factory; therefore the named factory remains the precise prerequisite. CP222's other loader callback and object/control cases span import, I/O, ordering, and failure behavior and are not equally small/independent candidates for this leaf.

## Ownership and limits

Exclusive source ownership is `loader.cpp`; no overlap with CP226. Keep C ABI headers and adapters unchanged. This design writes no production or test source and claims no tests run. No build, shared state, original, GUI, or REA state was changed.
