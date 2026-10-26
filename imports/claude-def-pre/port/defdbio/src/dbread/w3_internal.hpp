// dbread/w3_internal.hpp: helpers shared by the W3 sources of packet p-dbread.
#pragma once

#include <emmintrin.h>

#include <cstddef>
#include <cstdint>

namespace def::dbio::dbread::w3 {

// (int)d as compiled: cvttsd2si (INT32_MIN for NaN and out-of-range values).
inline std::int32_t cvtt(double x) { return _mm_cvttsd_si32(_mm_set_sd(x)); }

// *(T*)(p + off), for offsets of records the layout headers do not name (0xe0 cache records,
// 0xcd0 object blocks).
template <class T>
inline T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}

// Remap maps of FUN_1401cd1f0 (w3_remap_tables.cpp): kRemapBlock[(addr - 0x1405d5f00) / 4 + k]
// is word k of the map at `addr`, also past its end as the original reads it.
inline constexpr std::uint64_t kRemapBase = 0x1405d5f00;
extern const std::int32_t kRemapBlock[1664];
inline std::int32_t remap(std::uint64_t map, std::int32_t k)
{
    return kRemapBlock[(map - kRemapBase) / 4 + static_cast<std::ptrdiff_t>(k)];
}

} // namespace def::dbio::dbread::w3
