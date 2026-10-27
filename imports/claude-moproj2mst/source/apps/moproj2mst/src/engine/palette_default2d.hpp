// CP301 private conditional palette default; no live palette selection.
#pragma once

#include <array>

namespace m2m::engine {

// Original slot 12 when displayOption/dispEntityColor_12 settings lookup
// returns false. A successful lookup may provide a different QColor, and
// callback-time palette contents remain unqualified.
std::array<float, 4> conditionalDefaultPaletteSlot12() noexcept;

} // namespace m2m::engine
