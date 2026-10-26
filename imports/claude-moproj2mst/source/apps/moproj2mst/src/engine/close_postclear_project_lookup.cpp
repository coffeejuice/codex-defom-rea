#include "m2m/engine/close_postclear_project_lookup.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close post-clear Project leaf: ") + detail);
}
bool qualified(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") != std::string::npos;
}
bool sameManager(const std::shared_ptr<ModelManager>& left,
                 const std::shared_ptr<ModelManager>& right)
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

ClosePostClearProjectProgress::ClosePostClearProjectProgress(const ConversionSession& session,
    CloseEmptyManagerTailProgress& clear, std::string sourceOccurrence, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()),
      qualification_(std::move(qualification))
{
    if (sourceOccurrence != closePostClearProjectSourceOccurrence || !qualified(qualification_))
        unavailable("entry occurrence/qualification is unavailable");
    if (clear.stage() != CloseEmptyManagerTailStage::BeforePostClearProjectLookup)
        unavailable("completed empty-manager boundary is required");
    // The stage check is deliberately BEFORE this call: an unstarted progress
    // cannot be advanced implicitly to manufacture this prerequisite.
    const auto validated = attemptCloseEmptyManagerTail(session, clear);
    if (validated.stop != CloseEmptyManagerTailStop::BeforePostClearProjectLookup ||
        validated.clearCommittedThisAttempt)
        unavailable("empty-manager boundary was not already complete");
}

ClosePostClearProjectResult attemptClosePostClearProjectLookup(
    const ConversionSession& session, ClosePostClearProjectProgress& progress)
{
    const auto manager = session.modelManagerOwner();
    if (progress.session_ != &session || !sameManager(manager, progress.manager_.lock()) ||
        manager->ownerSession() != &session)
        unavailable("progress does not bind this live Session manager identity");
    if (manager->size() != 0 || !manager->connections().empty())
        unavailable("manager population changed after empty-clear admission");

    bool committed = false;
    if (progress.stage_ == ClosePostClearProjectStage::AwaitNullCurrentLeaf) {
        // 04578e ->c78ad0;045798/79e ->0457a0 returns null before the child
        // cast and044a90 lazy constructor. Do not substitute adapter Project.
        if (manager->currentModel() != nullptr)
            unavailable("actual current model is not the admitted null leaf");
        progress.stage_ = ClosePostClearProjectStage::NullProjectLeafReached;
        committed = true;
    }
    return {ClosePostClearProjectStop::NativeWrapperAndMainWindowEffectsUnavailable,
            committed, nullptr,
            "0x1400133c6:caller main+0x50/cast binding; 0x140029a93:0x14002a0e0 effects"};
}
} // namespace m2m::engine
