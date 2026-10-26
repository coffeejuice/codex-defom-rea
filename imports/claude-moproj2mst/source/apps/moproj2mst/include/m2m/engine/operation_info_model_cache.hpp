// Native OPI+160 observation metadata; construction/delete effects guarded.
#pragma once

#include <string>

#include "m2m/engine/operation_info_guard.hpp"

namespace m2m {
struct Operation;
namespace engine {
class SfMoItemModelState; // no model allocation or populated model implementation here

enum class OperationInfoModelCacheKnowledge { Unknown, KnownNull };

class OperationInfoModelCache final {
public:
    // Operation owns one canonical instance exposed only as const. It remains
    // Unknown: no defaults event or current native target producer is bound.
    // Separately constructed instances are detached diagnostic observations.
    explicit OperationInfoModelCache(Operation&);
    OperationInfoModelCache(const OperationInfoModelCache&) = delete;
    OperationInfoModelCache& operator=(const OperationInfoModelCache&) = delete;
    OperationInfoModelCache(OperationInfoModelCache&&) = delete;
    OperationInfoModelCache& operator=(OperationInfoModelCache&&) = delete;

    OperationInfoModelCacheKnowledge knowledge() const noexcept { return knowledge_; }
    OperationInfoGuard identitySnapshot() const noexcept { return identity_; }
    const std::string& nullQualification() const noexcept { return nullQualification_; }

    // Native050550@05067b assigns OPI+160 null via cbc920. Caller must bind this
    // to the actual defaults event; a nonblank label is not proof it executed.
    // Detached diagnostics only; the canonical Operation-owned instance refuses
    // this receipt API even if a caller removes its const access qualification.
    // Repeated same-null commits preserve the first receipt and return false.
    bool commitNativeDefaultNull(Operation&, const std::string& sourceQualification);

    // Native054f40: create=false never allocates. Unknown refuses rather than
    // becoming null. create=true/KnownNull stops before unrecovered0c4890/Qt
    // construction. There is no virtual-delete or bypass-allocation API.
    SfMoItemModelState* lookup(Operation&, bool create) const;
private:
    void requireSameLiveOperation(const Operation&) const;
    OperationInfoGuard identity_; // metadata only; never retains OPI/model storage
    OperationInfoModelCacheKnowledge knowledge_ = OperationInfoModelCacheKnowledge::Unknown;
    std::string nullQualification_;
};
} // namespace engine
} // namespace m2m
