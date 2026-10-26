#pragma once

#include "property_table.hpp"

#include <array>
#include <cstdint>

namespace deform::recovered {

// Selected owned state of18b500 at object2160. Names retain physical offsets
// until the corresponding scientific consumers have been recovered.
struct ObjectMotion {
    explicit ObjectMotion(int dimension);
    std::array<double, 3> direction_0x21b0{};
    std::int32_t mode_0x21c8 = 1;
    // Object offsets21d0,2248,2368,2400,2498,2510,25a8,2678,26f0.
    std::array<PropertyTable, 9> tables;
    std::int32_t field_0x22c0 = 0;
    double field_0x22c8 = 0.0;
    std::array<double, 3> values_0x22e8{};
    double field_0x2348 = 0.0;
    double field_0x2350 = 0.0;
    double field_0x2358 = 1.0;
    std::int32_t field_0x2360 = 0;
    std::array<double, 4> values_0x23e0{0.0, 1.0, 0.0, 0.0};
    std::array<double, 3> values_0x2478{};
    bool flag_0x2490 = false;
    std::array<double, 3> values_0x2588{};
    std::array<std::int32_t, 2> integers_0x25a0{};
    std::int32_t field_0x2620 = 1;
    std::int32_t field_0x2624 = 0;
    std::array<double, 9> values_0x2628{};
    std::int32_t field_0x2670 = 0;
    std::int32_t field_0x2674 = 0;
    std::array<double, 3> values_0x2768{};
    std::int32_t field_0x2780 = 0;
    std::int32_t field_0x2784 = 0;

    //18bc30/12f480 release table buffers without resetting metadata/scalars.
    void deactivate() noexcept;
};

} // namespace deform::recovered
