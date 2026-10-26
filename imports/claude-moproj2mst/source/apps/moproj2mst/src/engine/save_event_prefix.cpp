#include "m2m/engine/save_event_prefix.hpp"

#include <stdexcept>

namespace m2m::engine {
namespace {
constexpr const char* saveEntryPollSite = "0x140098129";
}

SaveEntryEventPrefix::Result SaveEntryEventPrefix::run(
    unsigned flags, bool dispatcherPresent,
    const std::optional<SaveEntryDueObservation>& observation,
    const DispatchDue& dispatch)
{
    if (attempted_)
        throw std::runtime_error("Save-entry event prefix may run only once per Save call");
    if (flags != 0)
        throw std::runtime_error("Save-entry event prefix is source-bound to processEvents(flags=0)");
    if (!dispatcherPresent) {
        if (observation)
            throw std::runtime_error("Save-entry observation supplied without a thread dispatcher");
        attempted_ = true;
        return Result::DispatcherAbsent;
    }
    if (!observation || observation->sourceSite != saveEntryPollSite ||
        observation->occurrence != 0 || observation->qualification.empty() || !dispatch)
        throw std::runtime_error("Save-entry due-event observation is unavailable or mismatched");

    // The pump has now been entered. If a callback fails, do not replay this
    // original one-shot dispatch boundary on a retry.
    attempted_ = true;
    dispatch(observation->dueTimerIds);
    return Result::Polled;
}

} // namespace m2m::engine
