#include "geometry_temp_prefix.hpp"
#include "gui_model_access.hpp"
#include <defmodel/layout/db_step_data.hpp>
#include <mutex>
#include <new>

namespace m2m::engine::detail {
namespace {
std::mutex preparationMutex;
// Created only by explicit preparation. Observation of ordinary models allocates nothing.
std::shared_ptr<GeometryTempPrefixBinding>* retainedBinding = nullptr;
}
struct GeometryTempPrefixBinding::Storage {
    mutable std::mutex mutex;
    GuiModelGuard model;
    DB_STEP_DATA* step = nullptr;
    DbHandle* db = nullptr;
    const void* reader = nullptr;
    std::shared_ptr<def::rt::ModuleHeapRole> role;
    std::shared_ptr<def::rt::TemporaryZeroCall> call;
    GeometryTempPrefixReport progress;
};
GeometryTempPrefixBinding::GeometryTempPrefixBinding() : storage_(std::make_unique<Storage>()) {}
GeometryTempPrefixBinding::~GeometryTempPrefixBinding() = default;

GeometryTempPrepareResult prepareGeometryTempPrefix(GuiModel& model, def::rt::HeapPrefixMode mode)
{
    DB_STEP_DATA* step = nullptr;
    try { GuiModelAccess::withStep(model, [&](DB_STEP_DATA* current) { step = current; }); }
    catch (const Error& error) {
        if (error.code != M2M_E_UNSUPPORTED) throw;
        return {GeometryTempPrepareStatus::ParkedModel, {}};
    }
    if (!step || !model.db || !model.db->reader || step->db != model.db->reader.get())
        return {GeometryTempPrepareStatus::ModelUnavailable, {}};
    std::lock_guard preparationLock(preparationMutex);
    if (retainedBinding) {
        const auto& binding = *retainedBinding;
        std::lock_guard lock(binding->storage_->mutex);
        if (binding->storage_->model.get() != &model || binding->storage_->step != step ||
            binding->storage_->db != model.db || binding->storage_->reader != step->db)
            return {GeometryTempPrepareStatus::ForeignModel, {}};
        if (binding->storage_->progress.mode != mode) return {GeometryTempPrepareStatus::ModeMismatch, {}};
        return {GeometryTempPrepareStatus::AlreadyPrepared, binding};
    }
    try {
        auto binding = std::shared_ptr<GeometryTempPrefixBinding>(new GeometryTempPrefixBinding);
        // Every app/lower retention allocation completes before HeapCreate.
        auto role = def::rt::ModuleHeapRole::prepareCurrentModule();
        auto* const retention = new std::shared_ptr<GeometryTempPrefixBinding>(binding);
        auto& s = *binding->storage_;
        s.model.reset(&model); s.step = step; s.db = model.db; s.reader = step->db; s.role = role;
        s.progress.mode = mode;
        s.progress.modelToken = reinterpret_cast<std::uintptr_t>(&model);
        s.progress.stepToken = reinterpret_cast<std::uintptr_t>(step);
        retainedBinding = retention; // Sticky even on producer/preparation failure.
        s.progress.producer = role->attemptProducer();
        const auto prepared = role->prepareTemporaryZero(role, mode);
        if (prepared.status != def::rt::Temp0PrepareStatus::Prepared) {
            s.progress.preparation = prepared.status == def::rt::Temp0PrepareStatus::CapacityUnavailable ?
                GeometryTempPrepareStatus::CapacityUnavailable : GeometryTempPrepareStatus::ProducerUnavailable;
        } else {
            s.call = prepared.call; s.progress.preparation = GeometryTempPrepareStatus::Prepared;
            s.progress.temporary = s.call->report();
            s.progress.scalar = s.call->unpublishedScalarReport();
        }
        return {s.progress.preparation, binding};
    } catch (const std::bad_alloc&) {
        return {GeometryTempPrepareStatus::HostPreparationUnavailable, {}};
    }
}
std::shared_ptr<GeometryTempPrefixBinding> preparedGeometryTempPrefix(const GuiModel& model) noexcept
{
    std::lock_guard preparationLock(preparationMutex);
    if (!retainedBinding) return {};
    const auto& binding = *retainedBinding;
    std::lock_guard lock(binding->storage_->mutex);
    const auto& s = *binding->storage_;
    if (s.model.get() != &model || s.step != model.step.get() || s.db != model.db ||
        !s.step || s.reader != s.step->db) return {};
    return binding;
}
GeometryTempPrefixReport GeometryTempPrefixBinding::attempt(const GeometryTempFrameEvidence& evidence,
    def::rt::CurrentScalarProof proof, const void* context) noexcept
{
    std::lock_guard lock(storage_->mutex);
    auto& s = *storage_; auto& r = s.progress;
    if (r.sealed) return r; // Before following any saved app address.
    auto* const model = s.model.get();
    if (!model || model != evidence.model || s.step != evidence.step || model->step.get() != s.step ||
        s.db != model->db || s.reader != s.step->db || !s.call ||
        !s.role->matchesRetainedLease(s.role) || !evidence.frame || !evidence.object || !evidence.registry ||
        !evidence.receiver || !evidence.body || !evidence.slot || evidence.ordinal < 0) return r;
    if (r.mode == def::rt::HeapPrefixMode::UnpublishedScalar &&
        (!evidence.preTempDataObservedNull || !proof || !context)) return r;
    r.sealed = true;
    r.preTempDataObservedNull = evidence.preTempDataObservedNull;
    r.frameToken = reinterpret_cast<std::uintptr_t>(evidence.frame);
    r.objectToken = reinterpret_cast<std::uintptr_t>(evidence.object);
    r.registryToken = reinterpret_cast<std::uintptr_t>(evidence.registry);
    r.receiverToken = reinterpret_cast<std::uintptr_t>(evidence.receiver);
    r.bodyToken = reinterpret_cast<std::uintptr_t>(evidence.body);
    r.slotToken = reinterpret_cast<std::uintptr_t>(evidence.slot); r.ordinal = evidence.ordinal;
    if (r.mode == def::rt::HeapPrefixMode::UnpublishedScalar) {
        r.scalar = s.call->attemptUnpublishedScalar(s.role, proof, context);
        r.temporary = s.call->report(); // Historical raw-only receipt stays unchanged.
    } else r.temporary = s.call->attempt(s.role);
    return r;
}
GeometryTempPrefixReport GeometryTempPrefixBinding::report() const noexcept
{
    std::lock_guard lock(storage_->mutex);
    auto report = storage_->progress;
    report.modelAlive = storage_->model.get() != nullptr;
    if (storage_->call) report.temporary = storage_->call->report();
    if (storage_->call) report.scalar = storage_->call->unpublishedScalarReport();
    return report;
}
} // namespace m2m::engine::detail
