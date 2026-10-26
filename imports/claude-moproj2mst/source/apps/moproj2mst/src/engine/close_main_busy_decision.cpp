#include "m2m/engine/close_main_busy_decision.hpp"

namespace m2m::engine {

CloseMainBusyDecision evaluateCloseMainBusyDecision(
    std::optional<std::int32_t> currentIndex,
    std::optional<std::uint32_t> mode270) noexcept
{
    // 01db05's Qt getter return is compared with exactly2 at01db0b. The native
    // first read cannot be skipped just because a later mode value is known.
    if (!currentIndex) return CloseMainBusyDecision::NeedsCurrentIndex;
    if (*currentIndex == 2) return CloseMainBusyDecision::Busy;

    // Only the non2 path calls327c0 at01db15;327ce compares DWORDmain+270 with1.
    if (!mode270) return CloseMainBusyDecision::NeedsMode270;
    return *mode270 == 1 ? CloseMainBusyDecision::Busy : CloseMainBusyDecision::NotBusy;
}

CloseMainBusyDecision evaluateCloseMainBusyDecision(
    CloseMainBusyPhase phase,
    std::optional<std::int32_t> currentIndex,
    std::optional<std::uint32_t> mode270) noexcept
{
    switch (phase) {
    case CloseMainBusyPhase::SfMainWindowBaseConstruction:
        // ca44f0: saved-this, xor al,al, ret. No widget/field read occurs.
        return CloseMainBusyDecision::NotBusy;
    case CloseMainBusyPhase::SfMoMainWindowDerived:
        return evaluateCloseMainBusyDecision(currentIndex, mode270);
    case CloseMainBusyPhase::Unknown:
        return CloseMainBusyDecision::NeedsMainPhase;
    }
    return CloseMainBusyDecision::NeedsMainPhase;
}

} // namespace m2m::engine
