// defrt/rdata.hpp: read-only double constants of DEF_PRE_64.exe's .rdata that several packets
// use. Each address has one owner here (fold_status --check: one @data per address); a packet
// names its local constant after its meaning and initialises it from these. Bits read from the
// binary.
#pragma once

#include <bit>
#include <cstdint>

namespace def::rt::rdata {

// @data 140509b50  1e30 (upper bound / initial box minimum)
inline constexpr double d_140509b50 = std::bit_cast<double>(std::uint64_t{0x46293e5939a08ceaULL});
// @data 140509c90  1.0 (one)
inline constexpr double d_140509c90 = std::bit_cast<double>(std::uint64_t{0x3ff0000000000000ULL});
// @data 140509fb8  2.0 (two)
inline constexpr double d_140509fb8 = std::bit_cast<double>(std::uint64_t{0x4000000000000000ULL});
// @data 14051bed8  0.0 (zero)
inline constexpr double d_14051bed8 = std::bit_cast<double>(std::uint64_t{0x0000000000000000ULL});
// @data 14051f1b8  1e-6 (point / duplicate-point tolerance)
inline constexpr double d_14051f1b8 = std::bit_cast<double>(std::uint64_t{0x3eb0c6f7a0b5ed8dULL});
// @data 14051f1f0  20.0 (room temperature, SI)
inline constexpr double d_14051f1f0 = std::bit_cast<double>(std::uint64_t{0x4034000000000000ULL});
// @data 14051f1f8  68.0 (room temperature, English)
inline constexpr double d_14051f1f8 = std::bit_cast<double>(std::uint64_t{0x4051000000000000ULL});
// @data 14051f228  180.0 (180)
inline constexpr double d_14051f228 = std::bit_cast<double>(std::uint64_t{0x4066800000000000ULL});
// @data 14051f260  1e-12 (degenerate segment / snap tolerance)
inline constexpr double d_14051f260 = std::bit_cast<double>(std::uint64_t{0x3d719799812dea11ULL});
// @data 140522f48  5.0 (five)
inline constexpr double d_140522f48 = std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL});
// @data 14051bee8  10.0 (ten)
inline constexpr double d_14051bee8 = std::bit_cast<double>(std::uint64_t{0x4024000000000000ULL});
// @data 14051f1c8  3.0 (three)
inline constexpr double d_14051f1c8 = std::bit_cast<double>(std::uint64_t{0x4008000000000000ULL});
// @data 14051f1e8  0.001 (1e-3)
inline constexpr double d_14051f1e8 = std::bit_cast<double>(std::uint64_t{0x3f50624dd2f1a9fcULL});
// @data 14051f258  0.01 (1e-2)
inline constexpr double d_14051f258 = std::bit_cast<double>(std::uint64_t{0x3f847ae147ae147bULL});
// @data 140522f38  7.0 (seven)
inline constexpr double d_140522f38 = std::bit_cast<double>(std::uint64_t{0x401c000000000000ULL});
// @data 140522f40  8.0 (eight)
inline constexpr double d_140522f40 = std::bit_cast<double>(std::uint64_t{0x4020000000000000ULL});
// @data 140522fc8  -1.0 (minus one)
inline constexpr double d_140522fc8 = std::bit_cast<double>(std::uint64_t{0xbff0000000000000ULL});
// @data 14051f210  360.0 (360)
inline constexpr double d_14051f210 = std::bit_cast<double>(std::uint64_t{0x4076800000000000ULL});
// @data 140524cd8  100.0 (hundred)
inline constexpr double d_140524cd8 = std::bit_cast<double>(std::uint64_t{0x4059000000000000ULL});
// @data 14052c598  1e29 (start of a minimum search)
inline constexpr double d_14052c598 = std::bit_cast<double>(std::uint64_t{0x45f431e0fae6d721ULL});
// @data 140522fd0  0.017453292519943295 (pi/180)
inline constexpr double d_140522fd0 = std::bit_cast<double>(std::uint64_t{0x3f91df46a2529d39ULL});
// @data 140522fd8  57.29577951308232 (180/pi)
inline constexpr double d_140522fd8 = std::bit_cast<double>(std::uint64_t{0x404ca5dc1a63c1f8ULL});

} // namespace def::rt::rdata
