#include "m2m/engine/close_tree_notification_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/close_busy_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close Tree notifier prefix: ") + detail);
}
bool qualified(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") != std::string::npos;
}
template<class T> bool sameOwner(const std::shared_ptr<T>& actual,
                                const std::shared_ptr<T>& recorded)
{
    return actual && recorded && actual.get() == recorded.get() &&
        !actual.owner_before(recorded) && !recorded.owner_before(actual);
}
}

CloseTreeNotificationPrefixProgress::CloseTreeNotificationPrefixProgress(
    const ConversionSession& session, CloseBusyGlobalOwner& busy,
    std::string sourceOccurrence, std::string qualification)
    : sessionIdentity_(reinterpret_cast<std::uintptr_t>(&session)),
      qualification_(std::move(qualification))
{
    // BEFORE attempting validation: an unstarted prerequisite may not execute
    // its three stores as an implicit effect of constructing this progress.
    if (busy.stage() != CloseBusyPrefixStage::BeforeTreeEnableSelectionDispatch)
        unavailable("already-reached true busy boundary is required");
    const auto validated = attemptCloseBusyTruePrefix(session, busy);
    if (validated.stop != CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable ||
        validated.prefixCommittedThisAttempt)
        unavailable("true busy boundary was not already reached");
    if (sourceOccurrence != closeTreeNotificationSourceOccurrence || !qualified(qualification_))
        unavailable("direct-callee occurrence/qualification is unavailable");
    const auto manager = session.modelManagerOwner();
    const auto notifier = session.sessionNotificationStateOwner();
    if (!manager || manager->ownerSession() != &session || !notifier)
        unavailable("actual Session manager/notifier identity is unavailable");
    manager_ = manager;
    notifier_ = notifier;
}

CloseTreeNotificationPrefixResult attemptCloseTreeNotificationPrefix(
    const ConversionSession& session, CloseTreeNotificationPrefixProgress& progress)
{
    const auto manager = session.modelManagerOwner();
    const auto notifier = session.sessionNotificationStateOwner();
    if (progress.sessionIdentity_ != reinterpret_cast<std::uintptr_t>(&session) ||
        !sameOwner(manager, progress.manager_.lock()) ||
        !sameOwner(notifier, progress.notifier_.lock()) ||
        manager->ownerSession() != &session)
        unavailable("progress does not bind this live Session manager/notifier identity");

    bool read = false;
    bool stored = false;
    if (progress.stage_ == CloseTreeNotificationPrefixStage::AwaitSuppressionRead) {
        read = true;
        if (notifier->suppressed) { // native143d76700+141,2cf3f1
            progress.stage_ = CloseTreeNotificationPrefixStage::SuppressedCalleeReturn;
        } else {
            notifier->flag90 = false; //2cf408, BEFORE QString/map/snapshot guards
            stored = true;
            progress.stage_ = CloseTreeNotificationPrefixStage::BeforeKeyQStringConstruction;
        }
    }
    if (progress.stage_ == CloseTreeNotificationPrefixStage::SuppressedCalleeReturn)
        return {CloseTreeNotificationPrefixStop::SuppressedCalleeReturnReached, read, stored,
                "0x1402cf72e:suppressed callee epilogue; caller arguments remain unavailable"};
    return {CloseTreeNotificationPrefixStop::KeyQStringConstructionUnavailable, read, stored,
            "0x1402cf41f:QString(char const*) signal-key construction"};
}
} // namespace m2m::engine
