#include "m2m/engine/report_document_slot.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Report document release prefix: ") + detail);
}
void requireQualification(const std::string& text)
{
    if (text.find_first_not_of(" \t\r\n") == std::string::npos)
        unavailable("source qualification is unavailable");
}
}

bool ReportDocumentSlot::observeInitialNull(std::string qualification)
{
    requireQualification(qualification);
    if (knowledge_ == ReportDocumentSlotKnowledge::KnownNull) return false;
    if (knowledge_ != ReportDocumentSlotKnowledge::Unknown)
        unavailable("initial slot observation cannot replace a nonnull value");
    (void)ensureDiagnosticScope();
    qualification_ = std::move(qualification);
    knowledge_ = ReportDocumentSlotKnowledge::KnownNull;
    return true;
}
bool ReportDocumentSlot::observeInitialNonNull(std::uintptr_t identity, std::string qualification)
{
    requireQualification(qualification);
    if (identity == 0) unavailable("nonnull slot observation requires a nonzero opaque identity");
    if (knowledge_ == ReportDocumentSlotKnowledge::KnownNonNull && documentIdentity_ == identity)
        return false;
    if (knowledge_ != ReportDocumentSlotKnowledge::Unknown)
        unavailable("initial slot observation cannot be rebound");
    (void)ensureDiagnosticScope();
    qualification_ = std::move(qualification);
    documentIdentity_ = identity;
    knowledge_ = ReportDocumentSlotKnowledge::KnownNonNull;
    return true;
}

ReportDocumentReleaseProgress::ReportDocumentReleaseProgress(const SubProject& sub,
    std::string sourceOccurrence, std::string qualification)
    : subIdentity_(reinterpret_cast<std::uintptr_t>(&sub)), qualification_(std::move(qualification))
{
    requireQualification(qualification_);
    if (sourceOccurrence != reportDocumentReleaseSourceOccurrence)
        unavailable("source occurrence is unavailable");
    // Binding is an explicit diagnostic use. Capture its lifetime now, so a
    // later attempt cannot silently bind a replacement at the same address.
    scope_ = sub.reportDocumentSlot.ensureDiagnosticScope();
}

ReportDocumentReleaseResult attemptReportDocumentReleasePrefix(const SubProject& sub,
    ReportDocumentReleaseProgress& progress)
{
    const auto& slot = sub.reportDocumentSlot;
    const auto recorded = progress.scope_.lock();
    if (progress.subIdentity_ != reinterpret_cast<std::uintptr_t>(&sub) || !recorded ||
        recorded.get() != slot.scope_.get() || recorded.owner_before(slot.scope_) ||
        slot.scope_.owner_before(recorded))
        unavailable("progress does not bind this live adapter SubProject slot scope");

    bool consumed = false;
    if (progress.stage_ == ReportDocumentReleaseStage::AwaitGuardRead) {
        if (slot.knowledge_ == ReportDocumentSlotKnowledge::Unknown)
            return {ReportDocumentReleaseStop::GuardValueUnavailable, false, 0,
                    "0x1400b5bc2:actual SubProject+0x30 guard value"};
        progress.observed_ = slot.knowledge_;
        progress.documentIdentity_ = slot.documentIdentity_;
        consumed = true;
        progress.stage_ = slot.knowledge_ == ReportDocumentSlotKnowledge::KnownNull
            ? ReportDocumentReleaseStage::NullGuardReturn
            : ReportDocumentReleaseStage::BeforeDocumentDeletingVirtual;
        // Nonnull's second pure getter sees this same immutable observation;
        // there is no callback, native document read or virtual invocation here.
    } else if (slot.knowledge_ != progress.observed_ || slot.documentIdentity_ != progress.documentIdentity_) {
        unavailable("slot observation changed after the consumed guard branch");
    }
    if (progress.stage_ == ReportDocumentReleaseStage::NullGuardReturn)
        return {ReportDocumentReleaseStop::NullGuardReturnReached, consumed, 0,
                "0x1400b5c26:null first-read return; no delete or guard clear"};
    return {ReportDocumentReleaseStop::DocumentDeletingVirtualUnavailable, consumed,
            progress.documentIdentity_, "0x1400b5bf6/0x1400b5c03:Report vtable/deleting virtual+0x18"};
}
} // namespace m2m::engine
