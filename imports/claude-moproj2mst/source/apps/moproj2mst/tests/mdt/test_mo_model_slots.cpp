// ctest-labels: m2m-mdt mo-model-slots
#include <testkit/check.hpp>

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/mo_model_slots.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

using namespace m2m;
using Slots = engine::MoModelSlots;
namespace {
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}

struct LifetimeProps final : Props {
    explicit LifetimeProps(std::function<void()> callback) : callback_(std::move(callback)) {}
    ~LifetimeProps() override { callback_(); }
private:
    std::function<void()> callback_;
};

void releaseAll(Slots& slots)
{
    for (const auto slot : {Slots::Slot::Operations, Slots::Slot::Objects, Slots::Slot::Materials,
                            Slots::Slot::Equipment, Slots::Slot::Lubricants})
        slots.releaseCompletedSlotStorage(slot);
}
}

static_assert(!std::is_copy_constructible_v<Project>);
static_assert(!std::is_copy_assignable_v<Project>);
static_assert(!std::is_move_constructible_v<Project>);
static_assert(!std::is_move_assignable_v<Project>);

TEST_CASE(mo_slot_aliases_are_stable_same_storage_and_preserve_defaults)
{
    Project project{};
    const auto slots = project.modelSlotsOwner();
    REQUIRE(slots);
    CHECK(&project.operations == &slots->operations());
    CHECK(&project.objects == &slots->objects());
    CHECK(&project.materials == &slots->materials());
    CHECK(&project.equipment == &slots->equipment());
    CHECK(&project.lubricants == &slots->lubricants());
    CHECK(&project.registry == &project.registryOwner()->entries());
    CHECK(project.objects.kind == ItemKind::Object);
    CHECK(project.materials.kind == ItemKind::Material);
    CHECK(project.equipment.kind == ItemKind::Equipment);
    CHECK(project.lubricants.kind == ItemKind::Lubricant);
    CHECK(slots->projectStorageAlive());
    CHECK(slots->projectIdentity() == &project);
    project.operations.push_back(std::make_unique<Operation>());
    CHECK(slots->operations().size() == 1);
    CHECK(refuses([&] { slots->releaseCompletedSlotStorage(Slots::Slot::Operations); }, "still borrows"));
    CHECK(slots->nextStorageBoundary() == Slots::Slot::Operations);
}

TEST_CASE(mo_slots_reject_null_owners_and_second_project_without_rebinding)
{
    auto registry = std::make_shared<engine::OperationClassRegistry>();
    CHECK(refuses([&] { Project invalid({}, registry); }, "storage owner is missing"));
    auto slots = std::make_shared<Slots>();
    CHECK(refuses([&] { Project invalid(slots, {}); }, "storage owner is missing"));
    auto first = std::make_unique<Project>(slots, registry);
    const auto* identity = first.get();
    CHECK(refuses([&] { Project second(slots, registry); }, "already belongs"));
    CHECK(slots->projectIdentity() == identity);
    CHECK(slots->projectStorageAlive());
    first.reset();
    CHECK(!slots->projectStorageAlive());
    CHECK(refuses([&] { Project replacement(slots, registry); }, "already belongs"));
}

TEST_CASE(mo_project_destruction_preserves_slots_and_registry_until_ordered_storage_release)
{
    auto project = std::make_unique<Project>();
    auto slots = project->modelSlotsOwner();
    std::weak_ptr<engine::OperationClassRegistry> registry = project->registryOwner();
    project->dom = std::make_shared<xml::Document>();
    std::weak_ptr<xml::Document> document = project->dom;
    bool operationDestroyed = false, registryAliveDuringOperation = false;
    auto operation = std::make_unique<Operation>();
    operation->props = std::make_shared<LifetimeProps>([&] {
        operationDestroyed = true;
        registryAliveDuringOperation = !registry.expired();
    });
    project->operations.push_back(std::move(operation));
    auto* objectIdentity = &project->objects;
    project.reset();
    CHECK(document.expired());
    CHECK(!slots->projectStorageAlive());
    CHECK(!operationDestroyed);
    CHECK(!registry.expired());
    CHECK(&slots->objects() == objectIdentity);
    CHECK(refuses([&] { slots->releaseCompletedSlotStorage(Slots::Slot::Objects); }, "out of order"));
    slots->releaseCompletedSlotStorage(Slots::Slot::Operations);
    CHECK(operationDestroyed);
    CHECK(registryAliveDuringOperation);
    CHECK(slots->nextStorageBoundary() == Slots::Slot::Objects);
    CHECK(refuses([&] { slots->operations(); }, "access after"));
    CHECK(refuses([&] { slots->releaseCompletedSlotStorage(Slots::Slot::Operations); }, "out of order"));
    slots->releaseCompletedSlotStorage(Slots::Slot::Objects);
    CHECK(refuses([&] { slots->objects(); }, "access after"));
    slots->releaseCompletedSlotStorage(Slots::Slot::Materials);
    CHECK(refuses([&] { slots->materials(); }, "access after"));
    slots->releaseCompletedSlotStorage(Slots::Slot::Equipment);
    CHECK(refuses([&] { slots->equipment(); }, "access after"));
    slots->releaseCompletedSlotStorage(Slots::Slot::Lubricants);
    CHECK(refuses([&] { slots->lubricants(); }, "access after"));
    CHECK(slots->nextStorageBoundary() == Slots::Slot::Done);
    CHECK(refuses([&] { slots->releaseCompletedSlotStorage(Slots::Slot::Done); }, "out of order"));
    slots.reset();
    CHECK(registry.expired());
}

TEST_CASE(mo_slot_adapter_fallback_destroys_operation_while_registry_is_alive)
{
    bool operationDestroyed = false, registryAlive = false;
    std::weak_ptr<engine::OperationClassRegistry> registry;
    {
        Project project;
        registry = project.registryOwner();
        auto operation = std::make_unique<Operation>();
        operation->props = std::make_shared<LifetimeProps>([&] {
            operationDestroyed = true;
            registryAlive = !registry.expired();
        });
        project.operations.push_back(std::move(operation));
    }
    CHECK(operationDestroyed);
    CHECK(registryAlive);
    CHECK(registry.expired());
}

TEST_CASE(mo_actual_model_adopts_one_project_capsule_and_keeps_it_after_project_release)
{
    auto project = std::make_unique<Project>();
    auto model = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    model->adoptMoModelSlots(project->modelSlotsOwner());
    model->adoptMoModelSlots(project->modelSlotsOwner()); // same binding is idempotent
    CHECK(model->moModelSlotsOwner() == project->modelSlotsOwner());
    CHECK(model->moModelSlotsOwner()->modelIdentity() == model.get());
    auto foreign = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    CHECK(refuses([&] { foreign->adoptMoModelSlots(project->modelSlotsOwner()); }, "another model"));
    auto base = std::make_shared<engine::DbHandle>();
    CHECK(refuses([&] { base->adoptMoModelSlots(project->modelSlotsOwner()); }, "Mo model"));
    auto other = std::make_unique<Project>();
    CHECK(refuses([&] { model->adoptMoModelSlots(other->modelSlotsOwner()); }, "different capsule"));
    std::weak_ptr<Slots> retained = project->modelSlotsOwner();
    project.reset();
    CHECK(!retained.expired());
    CHECK(!model->moModelSlotsOwner()->projectStorageAlive());
    releaseAll(*model->moModelSlotsOwner());
    model.reset();
    CHECK(retained.expired());
}

TEST_CASE(mo_lazy_project_db_adopts_same_storage_and_preserves_legacy_base_profile)
{
    Project project;
    auto* model = engine::projectDb(project);
    REQUIRE(model);
    CHECK(model->kind == engine::DbHandle::Kind::Mo);
    CHECK(model->moModelSlotsOwner() == project.modelSlotsOwner());
    CHECK(engine::projectDb(project) == model);
    Project legacy;
    legacy.dataDb = std::make_shared<engine::DbHandle>();
    CHECK(engine::projectDb(legacy) == legacy.dataDb.get());
    CHECK(!legacy.dataDb->moModelSlotsOwner()); // legacy adapter is not native Mo ownership
}

int main() { return testkit::run_all(); }
