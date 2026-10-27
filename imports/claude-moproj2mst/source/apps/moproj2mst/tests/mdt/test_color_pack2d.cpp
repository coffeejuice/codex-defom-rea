// CP297 recovered-static-contract checks; no original runtime parity claim.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include "../../src/engine/color_pack2d.hpp"

#include <array>
#include <cstdint>

using m2m::engine::packColor2D;

TEST_CASE(color_pack2d_independent_channel_order) {
    CHECK_EQ(packColor2D({1.0f, 0.0f, 0.0f}, 0.0f), 0x000000ffu);
    CHECK_EQ(packColor2D({0.0f, 1.0f, 0.0f}, 0.0f), 0x0000ff00u);
    CHECK_EQ(packColor2D({0.0f, 0.0f, 1.0f}, 0.0f), 0x00ff0000u);
    CHECK_EQ(packColor2D({0.0f, 0.0f, 0.0f}, 1.0f), 0xff000000u);
}

TEST_CASE(color_pack2d_endpoints_and_scalar_words) {
    const std::array<float, 3> rgb{1.0f, 0.5f, 0.0f};
    CHECK_EQ(packColor2D({0.0f, 0.0f, 0.0f}, 0.0f), 0u);
    CHECK_EQ(packColor2D({1.0f, 1.0f, 1.0f}, 1.0f), 0xffffffffu);
    CHECK_EQ(packColor2D(rgb, 0.99f), 0xfc007fffu);
    CHECK_EQ(packColor2D(rgb, 0.0f), 0x00007fffu);
}

TEST_CASE(color_pack2d_truncation_thresholds) {
    const std::array<float, 3> zero{0.0f, 0.0f, 0.0f};
    CHECK_EQ(packColor2D(zero, 0.5f), 0x7f000000u);
    CHECK_EQ(packColor2D(zero, 0.499f), 0x7f000000u);
    CHECK_EQ(packColor2D(zero, 0.502f), 0x80000000u);
    CHECK_EQ(packColor2D({0.502f, 0.5f, 0.499f}, 0.0f), 0x007f7f80u);
}

int main() { return testkit::run_all(); }
