#include "feature_win32_error_map.hpp"

#include <array>

namespace m2m::detail {
namespace {
struct RetainedFeatureErrorRow { std::uint32_t error; std::int32_t code; };
// Exact retained order. In particular the second6:22 row must not replace6:9.
constexpr std::array<RetainedFeatureErrorRow, 45> retainedFeatureErrorRows{{
    {1,22},{2,2},{3,2},{4,24},{5,13},{6,9},{7,12},{8,12},{9,12},{10,7},{11,8},{12,22},{13,22},
    {15,2},{16,13},{17,18},{18,2},{33,13},{53,2},{65,13},{67,2},{80,17},{82,13},{83,13},
    {87,22},{89,11},{108,13},{109,32},{112,28},{114,9},{6,22},{128,10},{129,10},{130,9},
    {131,22},{132,13},{145,41},{158,13},{161,2},{164,11},{167,13},{183,17},{206,2},{215,11},{1816,12}
}};
}

std::int32_t mapRetainedFeatureWin32Error(std::uint32_t error) noexcept
{
    for (const auto& row : retainedFeatureErrorRows)
        if (error == row.error) return row.code; //14740fc/147412b: first match wins.
    // Native LEA/ADD operate on32-bit registers, then compare unsigned.
    const std::uint32_t accessOffset = error - std::uint32_t{0x13};
    if (accessOffset <= std::uint32_t{0x11}) return 13;
    const std::uint32_t executableOffset = error - std::uint32_t{0xbc};
    return executableOffset <= std::uint32_t{0x0e} ? 8 : 22;
}
} // namespace m2m::detail
