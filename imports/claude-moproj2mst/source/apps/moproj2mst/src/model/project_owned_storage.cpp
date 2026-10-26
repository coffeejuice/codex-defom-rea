// Storage ownership for native Mo+19c8..19e8 and main-window registry leases.
// Native destructor payloads/Qt effects and accepted Close remain guarded elsewhere.
#include "m2m/engine/mo_model_slots.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "project_feature_context_anchor.hpp"
#include "project_feature_storage.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Mo model slot storage: ") + detail);
}

template<class T> T& live(const std::unique_ptr<T>& value)
{
    if (!value) unavailable("access after slot storage release");
    return *value;
}
}

OperationClassRegistry::OperationClassRegistry() = default;
OperationClassRegistry::~OperationClassRegistry() = default;
OperationClassRegistry::Entries& OperationClassRegistry::entries() noexcept { return entries_; }
const OperationClassRegistry::Entries& OperationClassRegistry::entries() const noexcept { return entries_; }

MoModelSlots::MoModelSlots()
    : operations_(std::make_unique<OperationRows>()),
      objects_(std::make_unique<ItemManager>(ItemManager{ItemKind::Object, {}})),
      materials_(std::make_unique<ItemManager>(ItemManager{ItemKind::Material, {}})),
      equipment_(std::make_unique<ItemManager>(ItemManager{ItemKind::Equipment, {}})),
      lubricants_(std::make_unique<ItemManager>(ItemManager{ItemKind::Lubricant, {}})) {}

MoModelSlots::~MoModelSlots() { releaseAdapterStorage(); }

MoModelSlots::OperationRows& MoModelSlots::operations() { return live(operations_); }
const MoModelSlots::OperationRows& MoModelSlots::operations() const { return live(operations_); }
ItemManager& MoModelSlots::objects() { return live(objects_); }
const ItemManager& MoModelSlots::objects() const { return live(objects_); }
ItemManager& MoModelSlots::materials() { return live(materials_); }
const ItemManager& MoModelSlots::materials() const { return live(materials_); }
ItemManager& MoModelSlots::equipment() { return live(equipment_); }
const ItemManager& MoModelSlots::equipment() const { return live(equipment_); }
ItemManager& MoModelSlots::lubricants() { return live(lubricants_); }
const ItemManager& MoModelSlots::lubricants() const { return live(lubricants_); }

void MoModelSlots::bindProjectStorage(
    const Project& project, const std::shared_ptr<const ProjectStorageLifetime>& storage,
    const std::shared_ptr<OperationClassRegistry>& registry)
{
    if (!storage || !registry) unavailable("Project storage/registry owner is missing");
    if (projectIdentity_ || next_ != Slot::Operations)
        unavailable("capsule already belongs to a Project or has released storage");
    projectIdentity_ = &project;
    projectStorage_ = storage;
    registryLease_ = registry;
}

void MoModelSlots::bindModelIdentity(const DbHandle& model)
{
    if (!projectStorageAlive() || next_ != Slot::Operations)
        unavailable("model binding requires live unreleased Project storage");
    if (modelIdentity_ && modelIdentity_ != &model)
        unavailable("capsule already belongs to another model identity");
    modelIdentity_ = &model;
}

void MoModelSlots::releaseCompletedSlotStorage(Slot slot)
{
    if (projectStorageAlive()) unavailable("Project still borrows the manager slots");
    if (slot != next_ || slot == Slot::Done)
        unavailable("slot release is out of order or already completed");
    switch (slot) {
    case Slot::Operations: operations_.reset(); next_ = Slot::Objects; break;
    case Slot::Objects: objects_.reset(); next_ = Slot::Materials; break;
    case Slot::Materials: materials_.reset(); next_ = Slot::Equipment; break;
    case Slot::Equipment: equipment_.reset(); next_ = Slot::Lubricants; break;
    case Slot::Lubricants: lubricants_.reset(); next_ = Slot::Done; break;
    case Slot::Done: break; // rejected above
    }
}

void MoModelSlots::releaseAdapterStorage() noexcept
{
    // Ordinary adapter cleanup does not replay native destructor callbacks.
    // Explicit order mirrors the separately bound native slot sequence.
    operations_.reset();
    objects_.reset();
    materials_.reset();
    equipment_.reset();
    lubricants_.reset();
    next_ = Slot::Done;
}
} // namespace m2m::engine

namespace m2m::detail {
class FeatureEpochBundle final {
public:
    engine::ProjectStorageLifetime marker;
};

namespace {
struct FeatureEpochDelete {
    FeatureEpochBundle* bundle;
    void operator()(const engine::ProjectStorageLifetime*) const noexcept
    {
        delete bundle; // Host storage only; no native feature/heap cleanup.
    }
};
}

std::shared_ptr<const engine::ProjectStorageLifetime> makeFeatureEpoch()
{
    auto* bundle = new FeatureEpochBundle;
    // If control-block construction throws, shared_ptr invokes this deleter.
    // There is no second independently armed owner of the bundle.
    return {&bundle->marker, FeatureEpochDelete{bundle}};
}

FeatureEpochBundle* featureEpoch(
    const std::shared_ptr<const engine::ProjectStorageLifetime>& epoch) noexcept
{
    if (!epoch) return nullptr;
    const auto* owner = std::get_deleter<FeatureEpochDelete>(epoch);
    if (!owner || !owner->bundle || epoch.get() != &owner->bundle->marker) return nullptr;
    return owner->bundle;
}

bool FeatureContextAccess::hasTypedEpoch(const Project::FeatureContextAnchor& anchor) noexcept
{
    if (!anchor.sourceOwnerAlive()) return false;
    return featureEpoch(anchor.storage_.lock()) != nullptr;
}
} // namespace m2m::detail

namespace m2m {
namespace {
template<class T> std::shared_ptr<T> checked(std::shared_ptr<T> value)
{
    if (!value) throw Error(M2M_E_UNSUPPORTED, "Project manager/registry storage owner is missing");
    return value;
}
}

Project::Project()
    : Project(std::make_shared<engine::MoModelSlots>(),
              std::make_shared<engine::OperationClassRegistry>()) {}

Project::Project(std::shared_ptr<engine::MoModelSlots> slots,
                 std::shared_ptr<engine::OperationClassRegistry> registryOwner)
    : registryAnchor_(checked(std::move(registryOwner))),
      slotsAnchor_(checked(std::move(slots))),
      slotBorrow_(detail::makeFeatureEpoch()),
      operations(slotsAnchor_->operations()), objects(slotsAnchor_->objects()),
      materials(slotsAnchor_->materials()), equipment(slotsAnchor_->equipment()),
      lubricants(slotsAnchor_->lubricants()), registry(registryAnchor_->entries())
{
    slotsAnchor_->bindProjectStorage(*this, slotBorrow_, registryAnchor_);
    // Shell metadata only. Native089917 is later, after the actual prepared Mo.
    featureContext_ = std::shared_ptr<FeatureContextAnchor>(new FeatureContextAnchor(*this, slotBorrow_, slotsAnchor_));
}

Project::~Project()
{
    // Invalidate escaped metadata before any member destruction; the slot epoch
    // still expires at its existing later boundary. No native feature cleanup.
    if (featureContext_) featureContext_->projectScopeActive_ = false;
}

Project::FeatureContextAnchor::FeatureContextAnchor(
    const Project& project, const std::shared_ptr<const engine::ProjectStorageLifetime>& storage,
    const std::shared_ptr<engine::MoModelSlots>& slots)
    : project_(&project), storage_(storage), slots_(slots)
{}

bool Project::FeatureContextAnchor::sharesStorageEpoch(const FeatureContextAnchor& other) const noexcept
{
    return !storage_.owner_before(other.storage_) && !other.storage_.owner_before(storage_);
}

bool Project::preparedFeatureContextModel(bool beforeBacklink) const noexcept
{
    if (!dataDb || dataDb->kind != engine::DbHandle::Kind::Mo || dataDb->modelId <= 0 || !dataDb->pathSet ||
        !slotsAnchor_->projectStorageAlive() || slotsAnchor_->projectIdentity() != this ||
        slotsAnchor_->modelIdentity() != dataDb.get()) return false;
    const auto& slots = dataDb->moModelSlotsOwner();
    if (!slots || slots.get() != slotsAnchor_.get() || slots.owner_before(slotsAnchor_) ||
        slotsAnchor_.owner_before(slots)) return false;
    return beforeBacklink ? dataDb->moProject == nullptr :
        (dataDb->moProject == nullptr || dataDb->moProject == this);
}

void Project::beginFeatureContextConstructorAttempt() noexcept
{
    if (!featureContext_ || !featureContext_->sourceOwnerAlive()) return;
    auto& context = *featureContext_;
    if (context.stage_ == FeatureContextStage::PassiveShell) {
        if (!preparedFeatureContextModel(true)) return; // legacy unprepared fixtures are storage-only
        context.model_ = dataDb;
        context.modelIdentity_ = dataDb.get();
        context.adapterF10AtAttempt_ = f10;
        context.backlinkWasNull_ = true; // actual read above, before adapter completion commit
        context.stage_ = FeatureContextStage::ProxyAllocationPending;
        return; //12f107b: unknown allocator/handler profile; no8-byte proxy allocation or other native store
    }
    const auto bound = context.model_.lock();
    const auto slots = context.slots_.lock();
    if (!preparedFeatureContextModel(false) || !bound || bound.get() != dataDb.get() ||
        bound.owner_before(dataDb) || dataDb.owner_before(bound) || !slots ||
        slots.get() != slotsAnchor_.get() || slots.owner_before(slotsAnchor_) || slotsAnchor_.owner_before(slots))
        context.bindingChanged_ = true; // never rebind/restart a consumed constructor occurrence
}

Project::FeatureContextAttemptReport Project::featureContextAttemptReport() const noexcept
{
    if (!featureContext_ || !featureContext_->sourceOwnerAlive())
        return {FeatureContextStage::PassiveShell, FeatureContextStop::ProjectStorageUnavailable, nullptr, nullptr};
    const auto& context = *featureContext_;
    if (context.stage_ == FeatureContextStage::PassiveShell)
        return {context.stage_, preparedFeatureContextModel(true) ? FeatureContextStop::ConstructorPhaseNotEntered :
                FeatureContextStop::PreparedMoUnavailable, nullptr, nullptr};
    const auto model = context.model_.lock();
    const auto slots = context.slots_.lock();
    const bool sameBinding = !context.bindingChanged_ && context.project_ == this && preparedFeatureContextModel(false) &&
        model && model.get() == dataDb.get() && !model.owner_before(dataDb) && !dataDb.owner_before(model) &&
        slots && slots.get() == slotsAnchor_.get() && !slots.owner_before(slotsAnchor_) && !slotsAnchor_.owner_before(slots);
    return {context.stage_, sameBinding ? FeatureContextStop::HeapProfileUnavailable : FeatureContextStop::OwnerBindingChanged,
            "0x140089917:0x1412f10d0", "0x1412f107b:8-byte proxy allocation"};
}

const std::shared_ptr<engine::MoModelSlots>& Project::modelSlotsOwner() const noexcept { return slotsAnchor_; }
const std::shared_ptr<engine::OperationClassRegistry>& Project::registryOwner() const noexcept { return registryAnchor_; }
std::shared_ptr<const Project::FeatureContextAnchor> Project::featureContextAnchor() const noexcept { return featureContext_; }
} // namespace m2m
