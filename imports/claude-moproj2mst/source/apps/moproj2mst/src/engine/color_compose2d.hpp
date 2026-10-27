// CP298 private, supplied-provider composition. No palette or GUI binding.
#pragma once

#include <array>
#include <cstdint>
#include <functional>

namespace m2m::engine {

class CurveItemState;

struct SuppliedColorProviders2D {
    // Each call supplies a fresh RGB triple from the selected palette slot.
    std::function<std::array<float, 3>()> paletteRgb;
    std::function<double()> redF;
    std::function<double()> greenF;
    std::function<double()> blueF;
};

// For ordinary finite [0,1] values in the normal masked FP environment.
// Provider side effects are caller-owned. Both words are prepared before the
// owner's existing raw-word writer is called; exceptions are not rolled back.
void composeSuppliedRenderColor2D(CurveItemState& owner, std::uint8_t mode,
    const SuppliedColorProviders2D& providers);

} // namespace m2m::engine
