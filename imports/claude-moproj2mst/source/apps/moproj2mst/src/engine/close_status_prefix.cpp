#include "m2m/engine/close_status_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close status prefix: ") + detail);
}
bool qualified(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") != std::string::npos;
}
}

CloseStatusPrefixProgress::CloseStatusPrefixProgress(const ConversionSession& session,
    std::string sourceOccurrence, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()),
      qualification_(std::move(qualification))
{
    const auto manager = manager_.lock();
    if (!manager || manager->ownerSession() != &session)
        unavailable("actual Session manager owner binding is unavailable");
    if (sourceOccurrence != closeStatusSourceOccurrence || !qualified(qualification_))
        unavailable("fresh status entry occurrence/qualification is unavailable");
}

CloseStatusPrefixResult attemptCloseStatusPrefix(const ConversionSession& session,
    CloseStatusPrefixProgress& progress, const CloseStatusMainObservation& observation)
{
    const auto actualManager = session.modelManagerOwner();
    const auto recordedManager = progress.manager_.lock();
    if (progress.session_ != &session || !actualManager || !recordedManager ||
        actualManager != recordedManager || actualManager->ownerSession() != &session)
        unavailable("progress does not bind this live Session manager identity");

    bool committed = false;
    if (progress.stage_ == CloseStatusPrefixStage::AwaitMainLookup) {
        if (observation.sessionOwner != &session ||
            observation.sourceOccurrence != closeStatusSourceOccurrence ||
            !observation.globalMainPresent || !qualified(observation.qualification))
            return {CloseStatusPrefixStop::MainLookupUnavailable, false, std::nullopt,
                    "0x1402d0bef:fresh global main-window lookup"};
        progress.mainPresent_ = *observation.globalMainPresent;
        progress.stage_ = *progress.mainPresent_
            ? CloseStatusPrefixStage::NonNullStatusBarGuard
            : CloseStatusPrefixStage::NullSkipBeforeCallerQStringRelease;
        committed = true;
    }
    if (progress.stage_ == CloseStatusPrefixStage::NonNullStatusBarGuard)
        return {CloseStatusPrefixStop::NonNullStatusBarEffectsUnavailable, committed,
                progress.mainPresent_, "0x1402d0c08:QMainWindow::statusBar effects"};

    // 2d0bff/2d0c01 null branch skips both status bar and ShowMsgInArea, returns
    // via2d0c6b. Caller still owns the QString and every public return effect.
    return {CloseStatusPrefixStop::NullMainSkipBeforeCallerQStringRelease, committed,
            progress.mainPresent_, "0x140029cea:caller QString release/public-return boundary"};
}
} // namespace m2m::engine
