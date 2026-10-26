# Mo slot ownership capsule implementation receipt (staged)

Status: source applied after root explicitly confirmed checkpoint48 frozen. Production cpp/test files were staged outside globs during validation, then restored after the freeze signal. No build or test was run by this worker.

## Owned files

- New production header: `source/apps/moproj2mst/include/m2m/engine/mo_model_slots.hpp`.
- New production source: `source/apps/moproj2mst/src/model/project_owned_storage.cpp`.
- New focused tests: `source/apps/moproj2mst/tests/mdt/test_mo_model_slots.cpp`.
- Applied existing-file diff retained for inspection: `analysis/staging/existing-slot-binding.patch.pending`, covering only `include/m2m/model.hpp`, `include/m2m/engine.hpp`, `src/engine/db_load.cpp` beneath the converter app.

No CMake, build, plan, model_manager or conversion_session edits belong to this lane. Existing interfaces/binding changes are now applied; conversion_session and build integration remain root-owned.

## Implemented storage contract

MoModelSlots owns the actual five existing manager representations. Project's borrowed aliases preserve existing field access and addresses; Project copy/move are explicitly deleted after the bounded full-tree audit. The same Mo DbHandle can adopt the Project capsule once; Base children, replacements and a second model identity refuse. Lazy projectDb adopts the same capsule on Mo paths; existing Base fixtures retain their narrower legacy adapter behavior.

A weak Project-storage marker remains live until all ordinary Project members are destroyed, preventing slot release while aliases or subproject destructors can still borrow them. Once Project storage is gone, explicit storage continuation deletes operations, objects, materials, equipment, lubricants in that order, rejecting repeated/out-of-order release. Getters reject released slots. Adapter fallback cleanup uses the same storage order without claiming native effects.

The exact registry owner is retained through late operation storage destruction. This is a lease of a session resource, not a sixth native Mo slot. Root must additionally retain the owner at session scope if required after model cleanup.

## Native qualifications retained

Reference: original DEF_GUI_MO_64 v11.0.2, SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. Static native order is 0440c0 -> 044bf0 (Project cleanup/delete, backlink null) -> 045000 (+19c8,+19d0,+19d8,+19e0,+19e8 deleting virtuals and null stores) -> cda520 base cleanup. The bounded ASM and hashes are recorded in resume46-close-manager-tail.md.

This implementation moves ownership/storage; it does not implement native constructor allocation provenance, Project+60 cleanup, real null-rebind, slot row/Props/Qt destructor payloads, base callbacks or accepted Close. Public Close remains guarded and untouched. Shared allocation lifetime is not native QObject lifetime. OpClass::current is a raw projection of a native QPointer and requires source-qualified invalidation at native operation teardown; this lane does not invent blanket reset timing.

Borrowed reference fields weaken const propagation through const Project&. This is documented in the proposed model header; capsule getters offer const overloads. Root must accept this internal compatibility choice or choose the wider accessor migration from resume48-close-slot-interface.md.

## Focused checks

The new test has six cases plus compile-time noncopy/nonmove assertions: stable aliases/defaults; null/reused Project capsule rejection; actual operation/DOM destruction and registry survival; fallback registry lifetime; same/foreign/Base model adoption; lazy Mo binding and legacy Base profile. These observe actual storage/object lifetimes where represented; they do not assert original native callback parity.

No build/test was run by this worker. Root owns configure/build and needs to add the lightweight source to standalone packet test targets as described in resume48-close-slot-interface.md. After integration, the focused command from converter subtree is:

```powershell
cmake --build build/candidate --target m2m_mdt_mo_model_slots
ctest --test-dir build/candidate -R '^m2m\.mdt\.mo_model_slots$' --output-on-failure
```

Use root's active converter build directory if candidate is not the selected build. Main/session/model-manager and affected standalone regression tests remain root-owned.

## Applied source hashes

- `include/m2m/engine/mo_model_slots.hpp`: `0486d2a65638576a3544d3cf7732a64e84d5f808bdd88202daf393deb463264c`
- `src/model/project_owned_storage.cpp`: `cb21a607853408ebaae8050037c468f61f51b230a1f96c6705497fd0c9165797`
- `include/m2m/model.hpp`: `00aacfaa9e4c0c6e2456b9682a60eff964a817ec36bb07442dd03201a398132d`
- `include/m2m/engine.hpp`: `c0849247fb275e299c306b2613b5dbb60daa46f1b6ddea26257ed5949d5fb415`
- `src/engine/db_load.cpp`: `f87d28001efdba3fc3fd286077a4ef83c7cb106d8c8495daa6eacbc9d5b35b75`
- `tests/mdt/test_mo_model_slots.cpp`: `02ed48db9a3598b0d84035789ef7be0feb647fe266b899edeb4ac94859f87ff6`

Root integration: call adoptMoModelSlots(p.modelSlotsOwner()) on the actual registered Mo child in session preparation/commit, retain p.registryOwner() at session lifetime, and link project_owned_storage.cpp into standalone test-double targets. No accepted-Close entry was enabled.
