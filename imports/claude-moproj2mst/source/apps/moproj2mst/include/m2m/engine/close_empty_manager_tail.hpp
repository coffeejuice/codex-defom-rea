// Isolated empty manager clear/current-reset projection; not public Close.
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace m2m::engine {
class ConversionSession;
class ModelManager;
struct DbHandle;

inline constexpr const char* closeEmptyManagerSourceOccurrence =
    "0x140029a76:0x140c78300";

enum class CloseEmptyManagerTailStage {
    InspectPopulation,
    OwnedChildTeardownBlocked,
    ConnectionCleanupBlocked,
    BeforePostClearProjectLookup
};
enum class CloseEmptyManagerTailStop {
    OwnedChildTeardownUnavailable,
    ConnectionCleanupUnavailable,
    BeforePostClearProjectLookup
};
struct CloseEmptyManagerTailResult {
    CloseEmptyManagerTailStop stop;
    bool clearCommittedThisAttempt;
    std::size_t observedModelCount;
    const char* nextSourceSite;
};

// Binds this actual Session/manager, not supplied list/current values. A label
// is caller provenance, not proof that earlier accepted-Close effects ran.
// Raw child snapshots are compared only; they neither retain nor dereference
// child allocations. There is no API to bypass a nonempty-child boundary.
class CloseEmptyManagerTailProgress final {
public:
    CloseEmptyManagerTailProgress(const ConversionSession&, std::string sourceOccurrence,
                                  std::string qualification);
    CloseEmptyManagerTailProgress(const CloseEmptyManagerTailProgress&) = delete;
    CloseEmptyManagerTailProgress& operator=(const CloseEmptyManagerTailProgress&) = delete;
    CloseEmptyManagerTailProgress(CloseEmptyManagerTailProgress&&) = delete;
    CloseEmptyManagerTailProgress& operator=(CloseEmptyManagerTailProgress&&) = delete;

    CloseEmptyManagerTailStage stage() const noexcept { return stage_; }
    const std::string& qualification() const noexcept { return qualification_; }
    std::size_t observedModelCount() const noexcept { return models_.size(); }
    const DbHandle* blockedModelIdentity() const noexcept {
        return models_.empty() ? nullptr : models_.front();
    }
private:
    friend CloseEmptyManagerTailResult attemptCloseEmptyManagerTail(
        const ConversionSession&, CloseEmptyManagerTailProgress&);
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    std::string qualification_;
    CloseEmptyManagerTailStage stage_ = CloseEmptyManagerTailStage::InspectPopulation;
    std::vector<const DbHandle*> models_;
    std::size_t connections_ = 0;
};

// c78300's zero-child branch reaches list clear c7852b, then explicit current
// reset c7853e. Uses the real manager.clear() only with empty list/connections;
// no child destructor or connection entry is removed. Physical Qt container/
// QPointer effects, preceding busy/refresh phases and post-clear029a83 remain
// unimplemented. Does not release Project, dispatch callbacks or enable Close.
// Retry preserves the consumed population/guard; it cannot clear later arrivals.
CloseEmptyManagerTailResult attemptCloseEmptyManagerTail(
    const ConversionSession&, CloseEmptyManagerTailProgress&);
} // namespace m2m::engine
