#include "m2m/engine/operation_info_model_cache.hpp"

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailableItemModelCache(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("OperationInfo item-model cache: ") + detail);
}
}

OperationInfoModelCache::OperationInfoModelCache(Operation& operation)
    : identity_(operation.operationInfoIdentity().guard())
{
    requireSameLiveOperation(operation);
}

void OperationInfoModelCache::requireSameLiveOperation(const Operation& operation) const
{
    if (identity_.get() != &operation || operation.operationInfoIdentity().get() != &operation)
        unavailableItemModelCache("cache does not bind this live OPI identity");
}

bool OperationInfoModelCache::commitNativeDefaultNull(
    Operation& operation, const std::string& sourceQualification)
{
    requireSameLiveOperation(operation);
    if (this == &operation.operationInfoModelCache())
        unavailableItemModelCache("canonical slot has no source-owned default-null producer");
    if (sourceQualification.find_first_not_of(" \t\r\n") == std::string::npos)
        unavailableItemModelCache("native default-null event requires a source qualification");
    if (knowledge_ == OperationInfoModelCacheKnowledge::KnownNull) return false;
    // Preserve Unknown if receipt allocation fails; no model/OPI mutation here.
    nullQualification_ = sourceQualification;
    knowledge_ = OperationInfoModelCacheKnowledge::KnownNull;
    return true;
}

SfMoItemModelState* OperationInfoModelCache::lookup(Operation& operation, bool create) const
{
    requireSameLiveOperation(operation);
    if (knowledge_ == OperationInfoModelCacheKnowledge::Unknown)
        unavailableItemModelCache("OPI+160 guard result is unavailable");
    if (create)
        unavailableItemModelCache("native0c4890 SfMoItemModel construction/Qt effects are unavailable");
    return nullptr; //054f40(false): qualified current guard null, no allocation
}
} // namespace m2m::engine
