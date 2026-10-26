# Resume48 Project usage audit and slot interface draft

A noncopyable, nonmovable Project is compatible with every detected m2m::Project call site. Stable owner storage plus borrowed field aliases can preserve current reader/writer spellings, but out-of-line ownership code must also be linked into standalone packet tests. The alias option changes const propagation and needs explicit review before root adopts it.

This is a bounded full-tree lexical audit and interface design, not a compiled patch. No source edits, build, GUI or REA mutation occurred. Root's candidate48 source freeze remains intact. Native teardown ordering/qualifications remain those in `resume46-close-manager-tail.md` and `resume47-close-staged-teardown-design.md`; the interface below does not establish unknown native constructor/destructor payloads.

## Audited call sites

Searched all converter C/C++ header/source extensions (`h,hpp,cc,cxx,cpp,inl,ipp`): 409 files under `source/apps/moproj2mst`. A source-tree-wide include/qualified-type scan found 112 files including `m2m/model.hpp` or using `m2m::Project`, all inside the converter app subtree. The hpp/cpp Project-token scan produced 1,457 matches on 1,440 lines across 199 files; these include comments, XML strings and a distinct helper type. These counts describe coverage/output, not token savings.

Search families covered value construction/return/parameters, rvalue references, copy/move/assignment/swap, standard containers of Project, smart pointers/new/make_unique, type aliases, member pointers/offsetof/sizeof/aggregate checks, braced initialization, slot assignment/swap and address/reference escapes. Bounded returned candidates were inspected rather than dumping all Project uses. This is not an AST/preprocessor proof; macros, generated code or indirect generic instantiation remain compiler-validation obligations.

| Category | Findings |
| --- | --- |
| m2m::Project construction | Default stable stack instances throughout tests; `Project prj{}` at `tests/p11/test_xcheck.cpp:251`; temporary `m2m::Project()` in `tests/p01/test_accessors.cpp:31-32`; one `make_unique<Project>()` in `src/engine/conversion_session.cpp:192`. |
| m2m::Project copy/move/by-value API | No detected call site. No detected Project&&, Project standard value container, direct Project assignment/swap, nonempty aggregate initializer, pointer-to-slot-member or layout introspection. |
| Apparent Project return-by-value | `tests/p03/order_corpus.hpp:200,214` defines its own `p03test::Project` and returns it. Consumers in `test_subproject.cpp:86` and `test_order.cpp:203` explicitly use that helper type. Do not change it. |
| Stable pointers/backlinks | Operation::project, SubProject::project, DbHandle::moProject, ProjectScope/currentProject and editor bindings borrow the stable Project identity. Enabling movement would require repairing these; deleting move operations avoids introducing that work. |
| Slot reference/address escapes | `src/model/opinfo.cpp:270` returns `&op.project->objects`; `src/engine/db_load.cpp:451,579`, `src/engine/loader.cpp:632,664`, `src/mst/emit_a.cpp:197` bind manager references. Keep manager object addresses stable through Project lifetime. |
| Slot mutation | Existing readers/loaders clear/push/query actual containers (`opinfo.cpp:33` clears operations; readers append), so alias-to-original-container preserves these operations. No detected assignment/swap of the whole five Project slot objects. |
| Registry escape | Operation::opClass is raw. Registry owner must outlive Project and operation manager destruction. Default Project destructor must not end registry before slots. |

No outside-converter file was identified as a consumer of m2m::Project in the code scan. The public C ABI exposes paths/settings/status, not Project layout; preserving that ABI still requires root regression.

## Header boundary that avoids incomplete-type cycles

Suggested new internal header: `include/m2m/engine/mo_model_slots.hpp`. It may include `m2m/qt4.hpp` plus standard headers and forward-declare Operation, ItemManager and OpClass. It must NOT include model.hpp. Definitions live in a lightweight ownership translation unit that includes model.hpp and mst/writer.hpp for complete unique_ptr pointees.

```cpp
#pragma once
#include <map>
#include <memory>
#include <vector>
#include "m2m/qt4.hpp"

namespace m2m {
struct Operation;
struct ItemManager;
namespace mst { class OpClass; }
namespace engine {
class StagedMoChildTeardown;

class MoModelSlots final {
public:
    using OperationRows = std::vector<std::unique_ptr<Operation>>;
    enum class Slot { Operations, Objects, Materials, Equipment, Lubricants, Done };

    MoModelSlots();                    // adapter storage defaults only
    ~MoModelSlots();                   // out of line; adapter fallback cleanup
    MoModelSlots(const MoModelSlots&) = delete;
    MoModelSlots& operator=(const MoModelSlots&) = delete;
    MoModelSlots(MoModelSlots&&) = delete;
    MoModelSlots& operator=(MoModelSlots&&) = delete;

    OperationRows& operations();
    const OperationRows& operations() const;
    ItemManager& objects();
    const ItemManager& objects() const;
    ItemManager& materials();
    const ItemManager& materials() const;
    ItemManager& equipment();
    const ItemManager& equipment() const;
    ItemManager& lubricants();
    const ItemManager& lubricants() const;
    Slot nextStorageBoundary() const noexcept { return next_; }

private:
    friend class StagedMoChildTeardown;
    // Called only after the corresponding qualified native effect completes.
    // Validate exact next slot before resetting; never silently skip a slot.
    void releaseStorageAtCompletedNativeBoundary(Slot);
    void releaseAdapterStorage() noexcept; // no normal-Close claim

    std::unique_ptr<OperationRows> operations_;
    std::unique_ptr<ItemManager> objects_, materials_, equipment_, lubricants_;
    Slot next_ = Slot::Operations;      // adapter continuation, not native field
};

class OperationClassRegistry final {
public:
    using Entries = std::map<qt::String, std::unique_ptr<mst::OpClass>>;
    OperationClassRegistry();
    ~OperationClassRegistry();        // needs complete OpClass, out of line
    OperationClassRegistry(const OperationClassRegistry&) = delete;
    OperationClassRegistry& operator=(const OperationClassRegistry&) = delete;
    Entries& entries() noexcept;
    const Entries& entries() const noexcept;
private:
    Entries entries_;
};
} // namespace engine
} // namespace m2m
```

Out-of-line storage getters must refuse a destroyed/null slot; a diagnostic shared_ptr must not resurrect it. The storage-release method and logical stage do not supply native effects. Root's effect driver first completes known destructor payload, then calls the private storage release. Unknown payloads retain a guard before deletion. Capsule construction must not certify native lazy manager instantiation or null/non-null field provenance.

`MoModelSlots::~MoModelSlots()` must explicitly release remaining storage in operation/object/material/equipment/lubricant order for fallback cleanup. Relying on member destruction would reverse the order again. Exact intra-container row destruction remains qualified separately; resetting an OperationRows vector is not proof of original QStandardItemModel row/Qt effects.

## Project declaration change

Use out-of-line constructors/destructor in the lightweight ownership translation unit. Place anchors first, with registry anchor BEFORE slots anchor: reverse destruction then drops the slots anchor before the registry anchor, after all ordinary Project members. Include the slot header (which contains only forward pointee declarations) from model.hpp.

```cpp
struct Project {
private:
    std::shared_ptr<engine::OperationClassRegistry> registryAnchor_;
    std::shared_ptr<engine::MoModelSlots> slotsAnchor_;
public:
    Project();
    Project(std::shared_ptr<engine::MoModelSlots>,
            std::shared_ptr<engine::OperationClassRegistry>);
    ~Project();
    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;
    Project(Project&&) = delete;
    Project& operator=(Project&&) = delete;

    const std::shared_ptr<engine::MoModelSlots>& modelSlotsOwner() const noexcept;
    const std::shared_ptr<engine::OperationClassRegistry>& registryOwner() const noexcept;

    // Existing ordinary fields remain, with these six ownership replacements:
    engine::MoModelSlots::OperationRows& operations;
    ItemManager& objects;
    ItemManager& materials;
    ItemManager& equipment;
    ItemManager& lubricants;
    engine::OperationClassRegistry::Entries& registry;
    // ... other existing Project members ...
};
```

The explicit constructor validates nonnull anchors before dereferencing them in alias initializers. Implement validation in an initializer helper returning the checked shared_ptr, not in the constructor body after references have already been bound. Initialize aliases using the stored anchors in declaration order. Default constructor delegates using two fresh owners. `~Project() = default` is defined out of line with complete member types. Copy/move are explicitly deleted, matching the audited stable-identity use; default `{}` and temporary `Project()` construction continue working.

Do not rebind anchors when `prepareProjectModel()` runs. Bind the Project's existing capsule to the actual new Mo DbHandle; lazy projectDb adopts the same capsule. Session stores the exact registry owner before Project release. Base/type4 models reject Mo capsule adoption. Storage binding and native constructor completion remain distinct checks.

The alias interface is compile-safe C++20 in declaration design, but was NOT compiled in this lane. It introduces a deliberate internal constness hazard: a reference member remains a mutable reference through const Project&. Existing const readers still compile, but the old type-level prohibition on mutation weakens. If root requires preserving the old const contract, migrate callsites to `operations()`/`objects()` const-overloaded accessors instead of reference fields. That is a wider mechanical patch; do not silently claim reference aliases preserve const semantics. No pointer/reference rebind or copying can repair this language rule.

## Linkage hazard and exact ownership set

Main CMake uses `GLOB_RECURSE src/*.cpp`, so a new ownership source is picked up by m2m_core after configure. Nine ctest-standalone tests bypass m2m_core and will otherwise get unresolved Project/capsule constructor/destructor symbols:

- `tests/p08/test_emit_a.cpp`
- `tests/p09/test_emit_b.cpp`, `tests/p09/test_keywords.cpp`
- `tests/p10/test_base_order.cpp`
- `tests/p12/test_p12_writers.cpp`
- `tests/p13/test_p13_writers.cpp`, `tests/p13/test_p13_mdwz.cpp`
- `tests/p15/test_tree.cpp`, `tests/p15/test_express.cpp`

The main CMake standalone branch (`CMakeLists.txt:130-147`) can append the lightweight ownership source once to standalone targets (and deduplicate sources), avoiding nine per-test metadata edits. Keep it independent of engine startup, native effect drivers or defmodel routines so test-double targets do not pull the full core. The separate `tests/p08/CMakeLists.txt` and `tests/p09/CMakeLists.txt` also require that source in their executable definitions; they do not use the main test loop. Do not solve linkage by linking m2m_core into test-double targets, which risks duplicate seams and changes their isolation.

Root's exact proposed exclusive write set:

1. `include/m2m/engine/mo_model_slots.hpp` NEW.
2. `src/model/project_owned_storage.cpp` NEW: capsule/registry storage definitions plus Project constructors/destructor/accessors, all lifetime-only and lightweight.
3. `include/m2m/model.hpp`: owner anchors and aliases (or explicit accessor migration chosen by root).
4. `include/m2m/engine.hpp`, `src/engine/db_load.cpp`: same-capsule Mo child binding and lazy adoption, no duplicate model.
5. `include/m2m/engine/conversion_session.hpp`, `src/engine/conversion_session.cpp`: registry retention and staged root ownership coordination.
6. `include/m2m/engine/model_manager.hpp`, `src/engine/model_manager.cpp`: only if adding the separate staged child-clear seam. Existing ordinary clear stays nonthrowing adapter cleanup.
7. `CMakeLists.txt`, `tests/p08/CMakeLists.txt`, `tests/p09/CMakeLists.txt`: lightweight ownership source linkage.
8. `tests/mdt/test_mo_model_slots.cpp` NEW; `test_conversion_session.cpp`, `test_model_manager.cpp` only for integration expectations. Existing storage-only expectations remain unchanged.

`src/model/project.cpp` need not gain constructors/destructor: putting them in the new storage source avoids dragging initProject/native model routines into standalone packet tests. `src/mst/emit_*`, ordinary operation writers/readers and packet test files need no source edits under alias choice. An accessor choice expands ownership set and must be separately frozen.

## Migration checks before accepting the patch

- Build all affected standalone targets, ordinary model/session tests and both standalone packet configurations when maintained. Include-only compile checks for model.hpp and mo_model_slots.hpp should verify no OpClass/Operation incomplete-type destructor leak.
- Check `Project()` temporary/default `{}` construction, and compile-time checks that Project is neither copyable nor movable. Do not add move support unless an actual required callsite emerges.
- Verify alias addresses equal capsule getters before/after DbHandle binding; p.operations mutation remains the same rows; foreign/session/base capsule adoption refuses without mutation.
- Verify registry survives Project while operation slots remain; Project/subprojects expire before slot deletion; slots delete in ascending order; external shared capsule observers cannot access destroyed slots.
- Keep source-/native-lifetime status separate from shared_ptr allocation liveness. Slot storage tests cannot establish native row/Props/Qt destructor payloads or accepted Close completion.
- Preserve existing exception adapter cleanup, shared Display/session survivors, stable raw backlinks and C ABI behavior; no self-cycle from moving Project ownership into DbHandle while Project.dataDb is shared.

All source is untouched in this lane. Root must serialize this interface migration after candidate48 freeze and maintain the preceding normal-Close dependency guards.
