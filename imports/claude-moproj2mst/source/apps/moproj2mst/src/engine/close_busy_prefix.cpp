#include "m2m/engine/close_busy_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close busy true prefix: ") + detail);
}

bool qualified(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") != std::string::npos;
}
}

CloseBusyGlobalOwner::CloseBusyGlobalOwner(const ConversionSession& session,
    CloseBusyNativeFields fields, std::string sourceOccurrence, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()), fields_(fields),
      qualification_(std::move(qualification))
{
    const auto manager = manager_.lock();
    if (!manager || manager->ownerSession() != &session)
        unavailable("actual Session manager owner binding is unavailable");
    if (sourceOccurrence != closeBusyTrueSourceOccurrence || !qualified(qualification_))
        unavailable("true entry occurrence/qualification is unavailable");
}

CloseBusyPrefixResult attemptCloseBusyTruePrefix(
    const ConversionSession& session, CloseBusyGlobalOwner& owner)
{
    const auto actualManager = session.modelManagerOwner();
    const auto recordedManager = owner.manager_.lock();
    if (owner.session_ != &session || !actualManager || !recordedManager ||
        recordedManager != actualManager || actualManager->ownerSession() != &session)
        unavailable("state does not bind this live Session manager identity");

    bool committed = false;
    if (owner.stage_ == CloseBusyPrefixStage::BeforeTrueStore) {
        owner.fields_.flag9 = 1; // 2caa12; true skips counter reset at2caa21
        // 2ca920(show=false) short-circuits on this exact nonzero stored byte.
        // It has no window query or modal delivery on this true invocation.
        owner.fields_.fieldA = 0; // 2caa4f ->2cb543
        owner.fields_.fieldB = 0; // 2caa5e ->2cb583
        owner.stage_ = CloseBusyPrefixStage::BeforeTreeEnableSelectionDispatch;
        committed = true;
    }
    // No population query, suppression test, notifier+90 reset, map insertion,
    // argument construction or tree callback has executed at this boundary.
    return {CloseBusyPrefixStop::TreeEnableSelectionDispatchUnavailable, committed,
            false, "0x1402caa63:before2cf3c0 TreeEnableSelection(false) arguments"};
}
} // namespace m2m::engine
