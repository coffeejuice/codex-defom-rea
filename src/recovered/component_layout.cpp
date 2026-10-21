#include "component_layout.hpp"

#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {
std::int32_t checked(std::int64_t value) {
    if (value < std::numeric_limits<std::int32_t>::min() ||
        value > std::numeric_limits<std::int32_t>::max())
        throw std::overflow_error("Component layout arithmetic exceeds int32");
    return static_cast<std::int32_t>(value);
}
std::int32_t add(std::int32_t a, std::int32_t b) {
    return checked(std::int64_t{a} + b);
}
std::int32_t multiply(std::int32_t a, std::int32_t b) {
    return checked(std::int64_t{a} * b);
}
std::array<std::int32_t, 79> generate_layout(const ComponentLayoutInputs& in,
                                           bool refresh) {
    // Evidence: analysis/startup-0x1401d09c0.json,
    // ev_77968f14dcb6435fff0e9d95a018214f389629fbb600ce664abc73f959fb230f;
    // refresh: analysis/root-layout-0x1401d0320.json,
    // ev_d57d0d56722651d21c5770e910854bfcb471bf40c332fe736e5ed2f3c591ec8e.
    // 158e90 returns root+0x1c; 0015e0/0015f0 test that field for2/3.
    // See root-layout-0x140158e90.json, root-layout-0x1400015f0.json
    // and mesh-0x1400015e0.json.
    std::array<std::int32_t, 79> result;
    result.fill(1);
    // Use original byte offsets so each assignment can be checked directly.
    const auto at = [&result](std::size_t offset) -> std::int32_t& {
        return result[(offset - 0x15f8) / 4];
    };
    const auto dimension = in.dimension;
    if (refresh) {
        if (dimension == 3) at(0x16b0) = 0;
    } else if (dimension == 2) {
        at(0x168c) = 4;
        at(0x16f0) = 2;
    } else {
        at(0x168c) = 0;
        at(0x16f0) = 3;
        at(0x16b0) = 0;
    }
    at(0x15f8) = dimension;
    at(0x15fc) = dimension;
    at(0x1600) = dimension;
    at(0x1604) = dimension;
    at(0x1608) = dimension;
    at(0x160c) = dimension;
    at(0x1610) = dimension;
    at(0x1640) = dimension;
    at(0x1644) = dimension;
    at(0x165c) = dimension;
    at(0x1658) = in.feature24.enabled ? 5 : 0;
    at(0x1690) = multiply(dimension, 2);
    at(0x1660) = multiply(dimension, 2);
    at(0x1694) = multiply(dimension, 2);
    std::int32_t local_20 = 0;
    for (const auto flag : in.root_0x86c_to_0x87c)
        local_20 = add(local_20, flag);
    const auto local_18 = local_20 < 2 ? 1 : local_20;
    local_20 = local_18;
    const auto local_1c = add(in.root_0x880, in.root_0x884);
    at(0x1698) = add(local_1c, multiply(multiply(local_18, dimension), 2));
    at(0x16dc) = multiply(dimension, 2);
    at(0x16b4) = 2;
    if (!in.feature49.enabled) {
        at(0x16bc) = 0;
        at(0x16c0) = 0;
        at(0x16c4) = 0;
        at(0x16c8) = 0;
    } else {
        at(0x16bc) = in.root_0x1320;
        at(0x16c0) = in.root_0x1320;
        at(0x16c4) = in.root_0x13f4;
        at(0x16c8) = in.root_0x13f4;
    }
    at(0x16cc) = in.feature53.enabled ? in.feature53.width : 0;
    at(0x166c) = in.feature29.enabled ? in.feature29.width : 0;
    at(0x16d0) = in.feature54.enabled ? in.feature54.width : 0;
    at(0x16d8) = 2;
    at(0x16e0) = 2;
    at(0x16e4) = dimension == 3 ? 3 : 2;
    at(0x16d4) = in.feature55.enabled ? in.feature55.width : 0;
    if (!refresh && dimension == 3)
        at(0x16b0) = in.feature46.enabled ? 9 : 0;
    at(0x163c) = in.root_0x3398;
    at(0x16e8) = in.root_0x33a8;
    if (refresh) {
        if (dimension == 2) {
            at(0x168c) = 4;
            at(0x16f0) = dimension;
        } else if (dimension == 3) {
            at(0x168c) = 0;
            at(0x16b0) = in.feature46.enabled ? 9 : 0;
            at(0x16f0) = dimension;
        }
    }
    if (dimension == 2 && in.root_0x850 == 3) {
        at(0x1600) = 3;
        at(0x1604) = 3;
        at(0x1608) = 3;
        at(0x1640) = 3;
        at(0x1644) = 3;
        at(0x1690) = 6;
        at(0x1660) = 6;
        at(0x1694) = 6;
        at(0x1698) = add(multiply(local_20, 6), local_1c);
        at(0x16dc) = 6;
    }
    if (in.root_0x888 == 0 || in.root_0x888 == 3) at(0x1668) = 0;
    else if (refresh) at(0x1668) = in.root_0x33c8;
    if (in.root_0x88c == 0 || in.root_0x88c == 3) at(0x1664) = 0;
    if (in.root_0x890 == 0 || in.root_0x890 == 3) at(0x1660) = 0;
    if (refresh) {
        at(0x16ac) = in.root_0x33d8;
        at(0x1680) = in.root_0x11c4;
        at(0x16f4) = in.root_0x123c;
        at(0x1684) = in.root_0x12b4;
    }
    return result;
}
} // namespace

std::array<std::int32_t, 79> initialize_layout(const ComponentLayoutInputs& in) {
    return generate_layout(in, false);
}

std::array<std::int32_t, 79> refresh_layout(const ComponentLayoutInputs& in) {
    return generate_layout(in, true);
}

} // namespace deform::recovered
