#include "palette_default2d.hpp"

#include <bit>
#include <cstdint>

namespace m2m::engine {

std::array<float, 4> conditionalDefaultPaletteSlot12() noexcept {
    constexpr float rgb = std::bit_cast<float>(std::uint32_t{0x3f05e354});
    constexpr float fourth = std::bit_cast<float>(std::uint32_t{0x3f733333});
    return {rgb, rgb, rgb, fourth};
}

} // namespace m2m::engine
