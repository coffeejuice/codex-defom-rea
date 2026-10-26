#pragma once

#include <cstdint>
#include <optional>

namespace m2m::engine {

enum class CloseMainBusyDecision {
    NeedsCurrentIndex,
    NeedsMode270,
    Busy,
    NotBusy,
    NeedsMainPhase
};

enum class CloseMainBusyPhase {
    Unknown,
    SfMainWindowBaseConstruction,
    SfMoMainWindowDerived
};

// Pure decision projection of native01daf0, AFTER a caller has the return from
// the actual main+480 QStackedWidget::currentIndex read. Missing values suspend
// the projection at the first required read. Values supplied here establish no
// application/main/widget ownership, native read, or constructor completion.
// NotBusy is only this virtual predicate's result, never permission to Close.
// There is deliberately no cached prior result, Save status, or default state.
CloseMainBusyDecision evaluateCloseMainBusyDecision(
    std::optional<std::int32_t> currentIndex,
    std::optional<std::uint32_t> mode270 = std::nullopt) noexcept;

// Pure dynamic-phase projection. Base construction uses table14290ac88+300's
// constant-false ca44f0 without requiring either input. Derived uses01daf0's
// ordered reads above. Unknown phase remains unavailable, even with values.
// The phase is an explicit input, not authenticated native ownership/liveness.
// c99530 publishes this as current-main but installs no vtable: calling it alone
// cannot change phase. In particular a later current-main W cannot inherit a
// prior constructor-main M's phase. This helper stores neither identity/phase.
// All three arguments are required to preserve legacy one/two-argument calls.
CloseMainBusyDecision evaluateCloseMainBusyDecision(
    CloseMainBusyPhase,
    std::optional<std::int32_t> currentIndex,
    std::optional<std::uint32_t> mode270) noexcept;

} // namespace m2m::engine
