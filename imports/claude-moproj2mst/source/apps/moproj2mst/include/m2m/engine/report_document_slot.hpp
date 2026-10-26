// Actual SubProject slot projection; no Report allocation/destructor/Close replay.
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace m2m {
struct SubProject;
namespace engine {
class ReportDocumentReleaseProgress;
struct ReportDocumentReleaseResult;
enum class ReportDocumentSlotKnowledge { Unknown, KnownNull, KnownNonNull };

// C++ metadata/lifetime only. Empty weak metadata is never native null. Initial
// observations are explicit, immutable except idempotent same-value repeats,
// and their labels do not authenticate original execution or QObject lifetime.
class ReportDocumentSlot final {
public:
    // Ordinary SubProject construction adds no allocation/failure point. Scope
    // is acquired only by explicit diagnostic observation/progress binding.
    // Inline defaults also preserve standalone packet linkage.
    ReportDocumentSlot() noexcept = default;
    ~ReportDocumentSlot() = default;
    ReportDocumentSlot(const ReportDocumentSlot&) = delete;
    ReportDocumentSlot& operator=(const ReportDocumentSlot&) = delete;
    ReportDocumentSlot(ReportDocumentSlot&&) = delete;
    ReportDocumentSlot& operator=(ReportDocumentSlot&&) = delete;
    ReportDocumentSlotKnowledge knowledge() const noexcept { return knowledge_; }
    std::uintptr_t documentIdentityToken() const noexcept { return documentIdentity_; }
    const std::string& qualification() const noexcept { return qualification_; }
    bool hasDiagnosticScope() const noexcept { return static_cast<bool>(scope_); }
    bool observeInitialNull(std::string qualification);
    bool observeInitialNonNull(std::uintptr_t opaqueDocumentIdentity, std::string qualification);
private:
    friend class ReportDocumentReleaseProgress;
    friend ReportDocumentReleaseResult attemptReportDocumentReleasePrefix(
        const SubProject&, ReportDocumentReleaseProgress&);
    struct Scope final {}; // adapter member scope, not a Report/QObject allocation
    const std::shared_ptr<Scope>& ensureDiagnosticScope() const {
        if (!scope_) scope_ = std::make_shared<Scope>();
        return scope_;
    }
    mutable std::shared_ptr<Scope> scope_;
    ReportDocumentSlotKnowledge knowledge_ = ReportDocumentSlotKnowledge::Unknown;
    std::uintptr_t documentIdentity_ = 0;
    std::string qualification_;
};

inline constexpr const char* reportDocumentReleaseSourceOccurrence =
    "0x1400b5bb0:SubProject Report release prefix";
enum class ReportDocumentReleaseStage { AwaitGuardRead, NullGuardReturn, BeforeDocumentDeletingVirtual };
enum class ReportDocumentReleaseStop {
    GuardValueUnavailable, NullGuardReturnReached, DocumentDeletingVirtualUnavailable
};
struct ReportDocumentReleaseResult {
    ReportDocumentReleaseStop stop;
    bool guardReadThisAttempt;
    std::uintptr_t observedDocumentIdentity;
    const char* nextSourceSite;
    static constexpr bool documentDeleteExecuted = false;
    static constexpr bool guardAssignmentExecuted = false;
    static constexpr bool nativeSubProjectCleanupComplete = false;
    static constexpr bool completeForPublicClose = false;
};

class ReportDocumentReleaseProgress final {
public:
    ReportDocumentReleaseProgress(const SubProject&, std::string sourceOccurrence,
                                  std::string qualification);
    ReportDocumentReleaseProgress(const ReportDocumentReleaseProgress&) = delete;
    ReportDocumentReleaseProgress& operator=(const ReportDocumentReleaseProgress&) = delete;
    ReportDocumentReleaseProgress(ReportDocumentReleaseProgress&&) = delete;
    ReportDocumentReleaseProgress& operator=(ReportDocumentReleaseProgress&&) = delete;
    ReportDocumentReleaseStage stage() const noexcept { return stage_; }
    ReportDocumentSlotKnowledge consumedKnowledge() const noexcept { return observed_; }
    std::uintptr_t observedDocumentIdentityToken() const noexcept { return documentIdentity_; }
private:
    friend ReportDocumentReleaseResult attemptReportDocumentReleasePrefix(
        const SubProject&, ReportDocumentReleaseProgress&);
    std::uintptr_t subIdentity_;
    std::weak_ptr<ReportDocumentSlot::Scope> scope_;
    std::string qualification_;
    ReportDocumentReleaseStage stage_ = ReportDocumentReleaseStage::AwaitGuardRead;
    ReportDocumentSlotKnowledge observed_ = ReportDocumentSlotKnowledge::Unknown;
    std::uintptr_t documentIdentity_ = 0;
};

// Gets this actual SubProject's member internally. First known-null read skips
// directly to0b5c26: NO delete and NO cbc920/guard clear. Nonnull preserves its
// opaque identity and stops before vtable/deleting-virtual effects. No document
// pointer is formed, lifetime inferred, constructor replayed or Close enabled.
ReportDocumentReleaseResult attemptReportDocumentReleasePrefix(
    const SubProject&, ReportDocumentReleaseProgress&);
} // namespace engine
} // namespace m2m
