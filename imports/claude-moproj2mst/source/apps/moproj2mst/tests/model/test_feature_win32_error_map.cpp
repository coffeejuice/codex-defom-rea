// ctest-labels: m2m-model feature-context retained-error-map
#include <testkit/check.hpp>
#include "../../src/model/feature_win32_error_map.hpp"

#include <array>
#include <cerrno>
#include <cstdint>
#include <limits>

using m2m::detail::mapRetainedFeatureWin32Error;
namespace {
struct Golden { std::uint32_t input; std::int32_t output; };
}

TEST_CASE(retained_error_map_matches_all_unique_original_table_keys)
{
    // Golden first-match outputs from retained143b5e160..143b5e2c8. This is
    // source-image data, not a host OS/errno or original runtime observation.
    constexpr std::array<Golden, 44> original{{
        {1,22},{2,2},{3,2},{4,24},{5,13},{6,9},{7,12},{8,12},{9,12},{10,7},{11,8},{12,22},{13,22},
        {15,2},{16,13},{17,18},{18,2},{33,13},{53,2},{65,13},{67,2},{80,17},{82,13},{83,13},
        {87,22},{89,11},{108,13},{109,32},{112,28},{114,9},{128,10},{129,10},{130,9},
        {131,22},{132,13},{145,41},{158,13},{161,2},{164,11},{167,13},{183,17},{206,2},{215,11},{1816,12}
    }};
    for (const auto& value : original) CHECK_EQ(mapRetainedFeatureWin32Error(value.input), value.output);
}

TEST_CASE(retained_error_map_uses_first_duplicate_and_exact_unsigned_fallback_boundaries)
{
    CHECK_EQ(mapRetainedFeatureWin32Error(6), 9); // Later6:22 is deliberately ignored.
    constexpr std::array<Golden, 14> edges{{
        {0,22},{14,22},{18,2},{19,13},{20,13},{32,13},{36,13},{37,22},
        {187,22},{188,8},{189,8},{202,8},{203,22},{206,2}
    }};
    for (const auto& value : edges) CHECK_EQ(mapRetainedFeatureWin32Error(value.input), value.output);
    // Zero must not be matched against the adjacent errno fallback word as a46th row.
    CHECK_EQ(mapRetainedFeatureWin32Error(0), 22);
}

TEST_CASE(retained_error_map_wraps_full_uint32_input_without_signed_range_aliases)
{
    constexpr std::array<std::uint32_t, 7> high{{
        0x7fffffffu,0x80000000u,0xffffff44u,0xffffffedu,0xfffffffeu,0xffffffffu,0x10000u
    }};
    for (const auto input : high) CHECK_EQ(mapRetainedFeatureWin32Error(input), 22);
    CHECK_EQ(mapRetainedFeatureWin32Error(std::numeric_limits<std::uint32_t>::max()), 22);
}

TEST_CASE(retained_error_map_has_no_runtime_owner_or_host_errno_effect)
{
    // Host-only purity probe; no native FLS/PTD/errno owner is asserted.
    const int entry = errno;
    errno = 123;
    const auto result = mapRetainedFeatureWin32Error(1816);
    const int after = errno;
    errno = entry;
    CHECK_EQ(result, 12);
    CHECK_EQ(after, 123);
    static_assert(noexcept(mapRetainedFeatureWin32Error(std::uint32_t{0})));
}

int main() { return testkit::run_all(); }
