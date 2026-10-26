# Resume 223: Operation shared native-step cache privatization contract

## Scope

Read-only design note for the next integration step. Evidence: CP222 property and loader alias ledgers, save alias ledger, editor closure architecture, and current source. No code or tests changed.

## Current contract and required semantics

`Operation::engineModel` is presently a public `shared_ptr<engine::GuiModel>` at `include/m2m/model.hpp:237`. It combines three distinct concerns: an optional/lazy operation model cache, shared ownership (multiple consumers may retain the same model), and a mutable public replacement/reset point. Privatizing the cache must preserve the first two while making replacement an owner-controlled transition.

The narrow compile-safe target is a private `engineModel_` plus a small Operation API implemented inline or in a source file where complete types are available:

- `engineModelOwner()` (or equivalent) provides a temporary shared owner only to trusted infrastructure that must retain lifetime; ordinary production consumers should migrate toward guarded model operations/opaque handle.
- `hasEngineModel()` and `sameEngineModelOwner(shared_ptr...)` support null checks and control-block identity checks without public mutation.
- `installEngineModel(shared_ptr<GuiModel>)` and `clearEngineModel()` are restricted transition methods; do not expose a mutable reference to the cache.
- Preserve lazy `opModel(Project&, Operation&)`: absent cache remains absent until the same original call path asks for a model; null-model and no-step remain distinct and valid states. Do not make construction eager or collapse null-step to null-model.
- Identity checks must retain both address and owner identity where current behavior does so. Weak guards stay weak/non-owning; an observation may hold a strong owner only where existing staged cleanup explicitly does.

A minimal first commit can privatize the field and add named accessors, then migrate direct reads/writes in a single compiling slice. It cannot close native step aliases by itself; it is an ownership/API staging change.

## Compile-safe sequence

1. Add private cache storage and narrowly named accessors/transitions on Operation. Keep Operation noncopyable/nonmovable, so its embedded identity/cache invariants remain intact. Avoid returning `shared_ptr&`.
2. Migrate source direct field sites together: `engine_internal.hpp::withOpStep`, `conversion_session.cpp`, `operation_info_cleanup.cpp`, `boolean_lifecycle.cpp`, `simcontrol300_lifecycle.cpp`, `simcontrol300_owner.cpp`, `simcontrol300.cpp`, `loader.cpp`, and `mdt_write.cpp`. Use owner comparison APIs where an exact shared-owner check is required. Keep staged cleanup's captured owner and `owner_before` equivalence check intact.
3. Migrate test fixture setup and assertions from direct assignment/reset/field inspection to explicit test-facing setup/owner observations. Primary test files are listed below; the boolean-editor tests intentionally replace and restore owners to exercise mismatch paths, so preserve the ability to simulate replacement through a controlled transition seam.
4. Only after all direct `engineModel` field references compile away, begin the separate GuiModel step-closure migration. Do not mix cache encapsulation with changing native-step semantics in one broad patch.

## Actual direct call/test inventory

Production references found by symbol search:

- `src/engine/engine_internal.hpp` — `withOpStep` resolves `opModel`, then reads `model->step`; this is an important step-alias seam, not just a cache query.
- `src/engine/conversion_session.cpp` — checks that the passed GuiModel is exactly the operation cache owner.
- `src/model/operation_info_cleanup.cpp` — captures cache owner at the EngineModelBoundary and checks stable null/non-null state across continuation stages.
- `src/engine/boolean_lifecycle.cpp` — checks cache and step presence, retains a weak model owner, compares owner identity, and later checks cache presence.
- `src/engine/simcontrol300_lifecycle.cpp`, `simcontrol300_owner.cpp`, `simcontrol300.cpp` — owner/lifecycle and sim-control reads; inspect exact call lines during implementation.
- `src/engine/loader.cpp` — loader cache creation/assignment and model resolution.
- `src/datafiles/mdt_write.cpp` — Cogging save gate checks cache absence/presence.

Test references are broadly distributed; explicit high-value owner/cache tests include:

- `tests/mdt/test_operation_info_cleanup.cpp` — staged null/present cache behavior, owner replacement, same-address/different-control-block cases, weak operation guard expiry.
- `tests/mdt/test_operation_info_model_cache.cpp` — cache knowledge semantics.
- `tests/mdt/test_conversion_session.cpp` — exact operation/model binding and rejection.
- `tests/mdt/test_boolean_editor.cpp` and `test_boolean_postload_creation.cpp` — cache replacement, nulling, and stable model checks.
- `tests/mdt/test_boolean_serial_gate.cpp` — weak frame behavior with expiry/replacement.
- `tests/mdt/test_loader_l2.cpp`, `test_db_inheritance.cpp`, `test_l7_load.cpp` — loader-created shared owner identity.
- `tests/doe/test_output_save_prefix.cpp` — cache absent/present Save gate and model retention.
- `tests/s/test_save_io.cpp`, `tests/s/test_distortion_contacts.cpp`, `tests/mdt/test_registered_session_geometry_lifetime.cpp`, `tests/mdt/test_simcontrol300_*.cpp`, editor setup/grid/base tests — fixture owners and native model setup requiring mechanical migration.

The complete current reference-file set can be regenerated with `rg -l "engineModel" source/apps/moproj2mst/{src,include,tests}`; this list is intentionally more complete than a hand-curated list of behavioral tests.

## Exact GuiModel step-closure dependency

Privatizing Operation's shared cache does not privatize `GuiModel::step`. The editor-closure architecture explicitly requires `step_` private, preserving one `unique_ptr<DB_STEP_DATA>` aggregate and its native interior backlinks, with no public raw getters or generic pointer callback escape. Current `GuiModel` still declares public `std::unique_ptr<DB_STEP_DATA> step` and legacy raw APIs (`simCtl`, `object`, `objectAt`, `objectWrap`, `appendObject`, plus `GuiModelAccess::withStep`). `engine_internal.hpp::withOpStep` passes `model->step.get()` to a callback constrained only to void/bool; capture remains possible. The editor architecture identifies a concrete captured-pointer escape in `prepareGeometryTempPrefix`. Consequently cache privacy is only the outer ownership layer: step closure still requires whole-tree conversion to named model-owned operations/keys, generation invalidation at destructive loads and reset, and preserving weak model guards and retained strong owners.

The exact linkage is: callers often acquire `shared_ptr<GuiModel>` from Operation and then use its public step/interior aliases. Removing cache mutation from general code prevents replacement races but does not prevent native step aliasing through a retained model owner. The final contract must allow retaining a model handle while denying access to `DB_STEP_DATA*` or its interior pointers.

## Limits

This is a static design inventory, not a claim of complete repository-wide reference enumeration beyond `rg -l`, test passage, or proof that callbacks cannot retain pointers. No tests/build were run.

