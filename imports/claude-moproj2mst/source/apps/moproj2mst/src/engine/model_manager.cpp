#include "m2m/engine/model_manager.hpp"

#include <limits>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/doe_slice.hpp"

namespace m2m::engine {
ModelManager::ModelManager(ConversionSession* owner) : owner_(nullptr)
{
    // All C++ list/current storage exists before the meaningful owner store.
    // The initializer's private placeholder is not a native null-owner event.
    owner_ = owner;
    constructorPrefix_.ownerToken_ = reinterpret_cast<std::uintptr_t>(owner);
    constructorPrefix_.ownerAssigned_ = true;

    // Native15d170 initialized the fresh guard to null. c782b0/cbc920 then
    // compares null with null and skips changeGuard with NO pointer write.
    // Compare default weak ownership, not expired()/currentModel(), which can
    // conceal nondefault expired or nonmember state. Never clear/reset here.
    const std::weak_ptr<DbHandle> defaultCurrent;
    if (current_.owner_before(defaultCurrent) || defaultCurrent.owner_before(current_))
        throw Error(M2M_E_UNSUPPORTED, "Model manager constructor requires the fresh default-null current guard");
    constructorPrefix_.equalitySkip_ = true;

    // Adapter process-retention binding is separate from that source prefix.
    if (owner) slicePlaneHeap_ = owner->slicePlaneHeapOwner();
}

ModelManager::~ModelManager() { clear(); }

DbHandle* ModelManager::modelAt(int ordinal) const noexcept
{
    if (ordinal < 0 || static_cast<std::size_t>(ordinal) >= models_.size()) return nullptr;
    return models_[static_cast<std::size_t>(ordinal)].get();
}

DbHandle* ModelManager::modelWithId(std::int32_t id) const noexcept
{
    if (id < 1) return nullptr; //c78ee0 explicitly rejects nonpositive IDs
    for (const auto& model : models_)
        if (model->modelId == id) return model.get();
    return nullptr;
}

DbHandle* ModelManager::currentModel() const noexcept
{
    const auto current = current_.lock();
    if (!current) return nullptr;
    for (const auto& model : models_)
        if (model.get() == current.get()) return current.get();
    return nullptr;
}

std::int32_t ModelManager::currentModelId() const noexcept
{
    const auto* model = currentModel();
    return model ? model->modelId : -1;
}

bool ModelManager::selectModel(DbHandle* selected) noexcept
{
    for (const auto& model : models_) {
        if (model.get() != selected) continue;
        current_ = model;
        return true;
    }
    return false;
}

bool ModelManager::selectOrdinal(int ordinal) noexcept
{
    auto* model = modelAt(ordinal);
    return model && selectModel(model);
}

std::int32_t ModelManager::nextModelId() const
{
    std::int32_t id = 1;
    while (modelWithId(id)) {
        if (id == std::numeric_limits<std::int32_t>::max())
            throw Error(M2M_E_UNSUPPORTED, "Model manager native ID increment overflow remains unsupported");
        ++id;
    }
    return id;
}

std::int32_t ModelManager::appendCreatedModel(std::shared_ptr<DbHandle> model)
{
    if (!model)
        throw Error(M2M_E_UNSUPPORTED, "Model manager native null construction dereference remains unsupported");
    for (const auto& owned : models_)
        if (owned.get() == model.get())
            throw Error(M2M_E_UNSUPPORTED, "Model manager append requires a fresh native factory child");
    const auto id = nextModelId();
    // Bind adapter allocation retention to this actual session. The native
    // plane list itself remains nonowning; no plane is allocated by append.
    if (owner_) {
        const auto heap = slicePlaneHeap_.lock();
        if (!heap) throw Error(M2M_E_UNSUPPORTED, "Model manager original process heap has expired");
        model->slicePlanes->bindHeap(heap);
    }
    model->modelId = id; //cdd740 precedes QList append; preserve partial effect on append failure
    models_.push_back(std::move(model));
    return id; //current is deliberately unchanged
}

void ModelManager::registerModelSignals(const std::shared_ptr<DbHandle>& model)
{
    if (!model || !owner_) throw Error(M2M_E_UNSUPPORTED, "Model connection sender or actual receiver is unavailable");
    bool member = false;
    for (const auto& owned : models_) if (owned == model) member = true;
    if (!member) throw Error(M2M_E_UNSUPPORTED, "Model connections require the actual appended child");
    connections_.push_back({model, "2signalModelChanged(SfStepData *)", "1slotModelChanged(SfStepData *)", 3, owner_});
    connections_.push_back({model, "2signalModelUpdated()", "1slotModelUpdated()", 3, owner_});
}

void ModelManager::emitModelSignal(DbHandle& sender, const std::string& signal, GuiModel*)
{
    for (const auto& connection : connections_)
        if (connection.signal == signal && connection.sender.lock().get() == &sender)
            throw Error(M2M_E_UNSUPPORTED, "Model signal receiver/type3 dispatch remains unrecovered: " + signal);
}

void ModelManager::clear() noexcept
{
    for (auto& model : models_) model.reset();
    models_.clear();
    current_.reset();
    connections_.clear(); // QObject disconnection projection; no receiver invoked
}

Project* currentProjectForManager(ModelManager& manager)
{
    auto* model = manager.currentModel();
    if (!model || model->kind != DbHandle::Kind::Mo) return nullptr;
    auto* project = model->moProject;
    if (!project)
        throw Error(M2M_E_UNSUPPORTED, "Current Mo model requires unrecovered lazy Project reconstruction");
    if (project->dataDb.get() != model)
        throw Error(M2M_E_UNSUPPORTED, "Current Mo Project backlink has a different model owner");
    return project;
}

std::shared_ptr<DbHandle> currentSubProjectDbOwner(ModelManager& manager)
{
    auto* project = currentProjectForManager(manager);
    if (!project) return {};
    auto* sub = activeSubProject(*project);
    if (!sub || !sub->project) return {};
    if (sub->project != project)
        throw Error(M2M_E_UNSUPPORTED, "Current subproject belongs to a different Project");
    if (sub->type != 4) return project->dataDb;
    if (!sub->ownDb)
        throw Error(M2M_E_UNSUPPORTED, "Current type4 subproject requires native Base model construction and path effects");
    return sub->ownDb;
}
} // namespace m2m::engine
