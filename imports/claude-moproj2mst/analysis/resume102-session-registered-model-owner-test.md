# CP102 — registered Session model-owner destruction test seam

## Recommendation

A focused test can observe the candidate ownership chain without claiming native Close: a Session registers a Mo `DbHandle` in its Session-owned `ModelManager`; the model adopts the Project's single `MoModelSlots`; explicit Project storage release leaves the registered model and slots alive; destroying the Session then destroys the manager-owned model and lets the slots owner expire. Use weak references only. This verifies candidate C++ lifetime boundaries, not native Qt deletion, accepted Close, native list clearing, or exact destructor callback timing.

## Source-derived destruction order

1. `ConversionSession::prepareProjectModel` creates the Mo `DbHandle`, binds the Project's `modelSlotsOwner`, sets its DB path, appends it to `modelManager_`, registers two signal connection projections, then assigns `p.dataDb`. Project-construction commit later binds `moProject` to that same registered child.
2. `ConversionSession::~ConversionSession` calls `releaseProjectStorage()` in its destructor body. That function saves a local shared model lease, resets Project scope and `project_`, then clears the model's `moProject` backlink and returns; the local lease ends on return. The Session's `modelManager_` is still alive.
3. After the destructor body, C++ destroys Session members in reverse declaration order. `modelManager_` is destroyed after later-declared `project_` and its other members. `ModelManager::~ModelManager` calls `clear()`, which resets each manager-owned model, then clears its list/current/connections.
4. `DbHandle::~DbHandle` explicitly resets its `moModelSlots_` lease. Once Project's anchor is already gone and there are no outside strong leases, the final `MoModelSlots` destructor runs and releases its adapter slot storage. Project and child share one capsule; no second model/storage copy is made.

Evidence is current source: `src/engine/conversion_session.cpp` (`prepareProjectModel`, destructor, `releaseProjectStorage`); `include/m2m/engine/conversion_session.hpp` (member declaration order); `src/engine/model_manager.cpp` (`~ModelManager`, `clear`); `src/engine/db_load.cpp` (`adoptMoModelSlots`, `~DbHandle`); and `src/model/project_owned_storage.cpp` (`Project` and `MoModelSlots` ownership/destructors).

## Smallest test

A new Release-active case in `tests/mdt/test_conversion_session.cpp` (or one uniquely named MDT file if exclusive ownership requires it) can:

- Construct a fresh Session, call `prepareProjectModel`, `commitProjectModelConstruction`, and `commitProjectModelSelection` so the child is actually registered/selected in the candidate manager.
- Capture `weak_ptr`s to `ModelManager`, `DbHandle`, and `MoModelSlots`; do not retain any extra `shared_ptr` beyond immediate assertions.
- Verify manager membership/current identity and that `model->moModelSlotsOwner()` equals the Project's `modelSlotsOwner()`.
- Call `releaseProjectAfterCloseEffects()` only as the existing explicit adapter storage-release boundary, with no diagram binding pending. Assert `liveProject()==nullptr`, `moProject==nullptr`, while manager/model/slot weak references remain live. Comment explicitly that the call tests storage ownership only and does not simulate accepted Close or assert callbacks occurred.
- Exit Session scope, then require all three weak references expired. This confirms Project-first release and later manager-owned child/capsule release for the candidate.

Existing tests cover pieces but not the end-to-end Session registration chain: `test_mo_model_slots.cpp::mo_actual_model_adopts_one_project_capsule_and_keeps_it_after_project_release` tests a standalone model lease; `test_model_manager.cpp::model_manager_destructor_releases_sole_owned_children_in_encounter_order` tests manager child release with custom child deleters. A new Session test would connect those owner identities using ordinary weak references. It cannot determine an exact sub-object destructor timestamp or prove `MoModelSlots::~MoModelSlots` ran after `DbHandle::~DbHandle` from expiration alone; those are established by source order. Do not add production callbacks solely to instrument this seam.

## Boundary

The safe test claim is: “candidate Session teardown releases Project storage before its retained manager child; that child's lease keeps the same MoModelSlots alive until manager teardown.” It must not be described as native Close, native QObject signal disconnection, or proof of native manager/QPointer deletion. No original runtime evidence is added by this test.

## Scope

Read-only source and test inventory review. No source/test edits, build, GUI, original execution, or mutable REA action was performed.
