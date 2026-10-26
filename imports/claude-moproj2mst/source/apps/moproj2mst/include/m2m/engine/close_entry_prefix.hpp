// Actual-owner Close entry prefix only. No modal or accepted cleanup execution.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

namespace m2m::engine {
class ConversionSession;

inline constexpr const char* closeEntrySourceOccurrence = "0x1400299cb:normal-close-entry";

struct CloseEntryByteObservation {
    std::uint8_t value;
    std::string qualification;
};
struct CloseEntryBoolObservation {
    bool value;
    std::string qualification;
};

// Observations must describe this attempt and the SAME session's native global
// state/window. Labels identify supplied evidence; they do not prove it true.
// mainBusy300 is the actual main-window virtual return, not MainView state,
// engineMutex, MoInputMode, process type, or a default false startup assumption.
struct CloseEntryBusyObservations {
    const ConversionSession* sessionOwner = nullptr;
    std::string sourceOccurrence;
    std::optional<CloseEntryByteObservation> globalFlag9;
    std::optional<CloseEntryBoolObservation> globalMainWindowPresent;
    std::optional<CloseEntryBoolObservation> mainBusy300;
};

enum class CloseEntryPrefixStop {
    BusyObservationUnavailable,
    BusyWarningEffectsUnavailable,
    ModelPopulationUnavailable,
    ModelVirtualCanCloseUnavailable,
    MoProjectCanCloseUnavailable,
    BeforeBusyTransitionEffects
};

struct CloseEntryPrefixResult {
    CloseEntryPrefixStop stop;
    std::size_t allowedMoChildren = 0;
    std::optional<std::size_t> pendingModelOrdinal;
    const char* nextSourceSite;
};

// Obtains the actual manager from this live Session. Reads real children/kind and
// moProject; never accepts a caller-supplied hasProject boolean. Only null Mo
// Project uses close_dialog::resolve's immediate-allow leaf. All-allowed stops
// BEFORE native2ca9f0(true), which has effects beyond assigning busy+9.
// No outcome means terminal Close success/cancellation; no state is mutated.
CloseEntryPrefixResult attemptCloseEntryPrefix(
    const ConversionSession&, const CloseEntryBusyObservations&);

} // namespace m2m::engine
