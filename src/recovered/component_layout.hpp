#pragma once

#include <array>
#include <cstdint>

namespace deform::recovered {

// Result of one original FUN_1401d64f0 query. For queries with an output
// pointer, width is local_24 AFTER that call, including any retained value.
// Query implementation and the meaning of these IDs are separate concerns.
struct FeatureQueryResult {
    bool enabled = false;
    std::int32_t width = 0;
};

struct ComponentLayoutInputs {
    std::int32_t dimension = 0; // Original param_2, without normalization.
    FeatureQueryResult feature24, feature49, feature53, feature29;
    FeatureQueryResult feature54, feature55, feature46;
    std::array<std::int32_t, 5> root_0x86c_to_0x87c{};
    std::int32_t root_0x880 = 0;
    std::int32_t root_0x884 = 0;
    std::int32_t root_0x1320 = 0;
    std::int32_t root_0x13f4 = 0;
    std::int32_t root_0x3398 = 0;
    std::int32_t root_0x33a8 = 0;
    std::int32_t root_0x850 = 0;
    std::int32_t root_0x888 = 0;
    std::int32_t root_0x88c = 0;
    std::int32_t root_0x890 = 0;
    // Refresh-only inputs; ignored by initialize_layout.
    std::int32_t root_0x33c8 = 0;
    std::int32_t root_0x33d8 = 0;
    std::int32_t root_0x11c4 = 0;
    std::int32_t root_0x123c = 0;
    std::int32_t root_0x12b4 = 0;
};

// Internal helper 0x1401d09c0; output slot i is root+0x15f8+4*i.
// Reproduces ordered writes, not a universal dimensional constant profile.
// Throws std::overflow_error if any original arithmetic intermediate would
// exceed signed int32, including intermediates overwritten later.
[[nodiscard]] std::array<std::int32_t, 79>
initialize_layout(const ComponentLayoutInputs& input);

// Internal refresh helper 0x1401d0320. Starts again with 79 ones; it does
// not modify a previous layout. dimension supplies the root dimension getter.
[[nodiscard]] std::array<std::int32_t, 79>
refresh_layout(const ComponentLayoutInputs& input);

} // namespace deform::recovered
