#pragma once

#include <defrt/module_heap_role.hpp>
#include "m2m/engine.hpp"

namespace m2m::engine::detail {
class GeometryTempPrefixBinding;
// Only the current Viewer GeometrySyncAccess can seal these observations.
class GeometryTempFrameEvidence final {
    friend struct ::m2m::engine::GeometrySyncAccess;
    friend class GeometryTempPrefixBinding;
    GeometryTempFrameEvidence() = default;
    GuiModel* model = nullptr;
    DB_STEP_DATA* step = nullptr;
    const void* frame = nullptr;
    const void* object = nullptr;
    const void* registry = nullptr;
    const void* receiver = nullptr;
    const void* body = nullptr;
    const void* slot = nullptr;
    std::int32_t ordinal = -1;
    bool preTempDataObservedNull = false;
};
enum class GeometryTempPrepareStatus { Prepared, AlreadyPrepared, ForeignModel, ParkedModel, ModelUnavailable, ProducerUnavailable, CapacityUnavailable, HostPreparationUnavailable, ModeMismatch };
struct GeometryTempPrefixReport {
    GeometryTempPrepareStatus preparation = GeometryTempPrepareStatus::ModelUnavailable;
    def::rt::HeapPrefixMode mode = def::rt::HeapPrefixMode::RawTemporaryOnly;
    bool sealed = false, modelAlive = false, preTempDataObservedNull = false;
    std::uintptr_t modelToken = 0, stepToken = 0, frameToken = 0, objectToken = 0;
    std::uintptr_t registryToken = 0, receiverToken = 0, bodyToken = 0, slotToken = 0;
    std::int32_t ordinal = -1;
    def::rt::HeapRoleProducerReport producer;
    def::rt::Temp0Report temporary;
    def::rt::UnpublishedScalarReport scalar;
};
struct GeometryTempPrepareResult {
    GeometryTempPrepareStatus status = GeometryTempPrepareStatus::ModelUnavailable;
    std::shared_ptr<GeometryTempPrefixBinding> binding;
};
// Private explicit candidate experiment. No production/ABI/default conversion hook.
GeometryTempPrepareResult prepareGeometryTempPrefix(GuiModel& model,
    def::rt::HeapPrefixMode mode = def::rt::HeapPrefixMode::RawTemporaryOnly);
std::shared_ptr<GeometryTempPrefixBinding> preparedGeometryTempPrefix(const GuiModel& model) noexcept;
class GeometryTempPrefixBinding final {
public:
    ~GeometryTempPrefixBinding(); // Host metadata only, never free T or destroy heap.
    GeometryTempPrefixReport report() const noexcept;
private:
    friend struct ::m2m::engine::GeometrySyncAccess;
    friend GeometryTempPrepareResult prepareGeometryTempPrefix(GuiModel&, def::rt::HeapPrefixMode);
    friend std::shared_ptr<GeometryTempPrefixBinding> preparedGeometryTempPrefix(const GuiModel&) noexcept;
    GeometryTempPrefixBinding();
    GeometryTempPrefixReport attempt(const GeometryTempFrameEvidence&,
        def::rt::CurrentScalarProof proof = nullptr, const void* currentProofContext = nullptr) noexcept;
    struct Storage;
    std::unique_ptr<Storage> storage_;
};
} // namespace m2m::engine::detail
