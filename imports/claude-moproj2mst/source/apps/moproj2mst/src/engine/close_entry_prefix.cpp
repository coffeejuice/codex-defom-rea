#include "m2m/engine/close_entry_prefix.hpp"

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_dialog_answer.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
bool qualified(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") != std::string::npos;
}

CloseEntryPrefixResult busyUnavailable(const char* site)
{
    return {CloseEntryPrefixStop::BusyObservationUnavailable, 0, std::nullopt, site};
}

CloseEntryPrefixResult warningUnavailable()
{
    // Public0299a0 passed show=true; busy does not immediately return false.
    return {CloseEntryPrefixStop::BusyWarningEffectsUnavailable, 0, std::nullopt,
            "0x1402ca982:0x1402caf10 busy-warning effects"};
}
}

CloseEntryPrefixResult attemptCloseEntryPrefix(
    const ConversionSession& session, const CloseEntryBusyObservations& busy)
{
    const auto actualManager = session.modelManagerOwner();
    if (!actualManager || actualManager->ownerSession() != &session || busy.sessionOwner != &session)
        throw Error(M2M_E_UNSUPPORTED, "Close entry prefix lacks the actual session/manager owner binding");
    const auto& manager = *actualManager;
    if (busy.sourceOccurrence != closeEntrySourceOccurrence || !busy.globalFlag9 ||
        !qualified(busy.globalFlag9->qualification))
        return busyUnavailable("0x1402ca937:global+9 observation");

    // Native2ca920 short-circuits the main-window query for nonzero global+9.
    if (busy.globalFlag9->value != 0) return warningUnavailable();
    if (!busy.globalMainWindowPresent || !qualified(busy.globalMainWindowPresent->qualification))
        return busyUnavailable("0x1402ca84e:global main-window lookup");
    if (busy.globalMainWindowPresent->value) {
        if (!busy.mainBusy300 || !qualified(busy.mainBusy300->qualification))
            return busyUnavailable("0x1402ca878:main-window virtual+0x300");
        if (busy.mainBusy300->value) return warningUnavailable();
    }
    // A qualified null global main-window makes2ca840 return false without+300.

    std::size_t allowed = 0;
    const auto count = manager.size();
    for (std::size_t ordinal = 0; ordinal < count; ++ordinal) {
        if (manager.size() != count)
            return {CloseEntryPrefixStop::ModelPopulationUnavailable, allowed, ordinal,
                    "0x140c7a100:manager child population"};
        const auto* model = manager.modelAt(static_cast<int>(ordinal));
        if (!model)
            return {CloseEntryPrefixStop::ModelPopulationUnavailable, allowed, ordinal,
                    "0x140c7a100:manager child identity"};
        if (model->kind != DbHandle::Kind::Mo)
            return {CloseEntryPrefixStop::ModelVirtualCanCloseUnavailable, allowed, ordinal,
                    "0x140c7a100:child virtual+0x68"};
        if (model->moProject)
            return {CloseEntryPrefixStop::MoProjectCanCloseUnavailable, allowed, ordinal,
                    "0x140044143:application process-type/conditional question route"};

        close_dialog::CloseInput input;
        input.hasProjectModel = model->moProject != nullptr; // actual native owner field
        // Null Project returns before process-type/modal/schedule queries.
        const auto decision = close_dialog::resolve(input, {}, {});
        if (decision != close_dialog::Outcome::CloseAllowed)
            throw Error(M2M_E_UNSUPPORTED, "Close null-Project can-close leaf changed unexpectedly");
        ++allowed;
    }
    // The next native call writes+9 and runs further busy/notification effects.
    // Refuse at its entry, rather than publishing a partial bool assignment.
    return {CloseEntryPrefixStop::BeforeBusyTransitionEffects, allowed, std::nullopt,
            "0x140029a54:0x1402ca9f0(true)"};
}
} // namespace m2m::engine
