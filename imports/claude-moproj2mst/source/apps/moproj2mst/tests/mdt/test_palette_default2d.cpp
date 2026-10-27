// CP301 conditional false-settings-arm default only.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include "../../src/engine/palette_default2d.hpp"
#include "../../src/engine/color_pack2d.hpp"

#include <array>
#include <bit>
#include <cstdint>

using namespace m2m::engine;

TEST_CASE(palette_default2d_exact_conditional_bits) {
    const auto value = conditionalDefaultPaletteSlot12();
    for (int i = 0; i != 3; ++i)
        CHECK_EQ(std::bit_cast<std::uint32_t>(value[i]), 0x3f05e354u);
    CHECK_EQ(std::bit_cast<std::uint32_t>(value[3]), 0x3f733333u);
}

TEST_CASE(palette_default2d_supplied_rgb_packs_independently) {
    const auto value = conditionalDefaultPaletteSlot12();
    const std::array<float, 3> rgb{value[0], value[1], value[2]};
    // 0x3f05e354 * 255 rounds to 133.365..., then truncates to 0x85.
    CHECK_EQ(packColor2D(rgb, 0.99f), 0xfc858585u);
    CHECK_EQ(packColor2D(rgb, 0.0f), 0x00858585u);
}

int main() { return testkit::run_all(); }
