// dbread/w1_access.hpp: raw member access for the W1 part of the DBREAD port (packet p-dbread).
// Most sub-objects this subtree fills are opaque byte slots in their owners' layout headers
// (LLOBJ_DATA s1ea0 / s2160 / s2d10 / s2e68, the window's friction block, Cls_140258580's body), so
// the code writes them at the binary's offsets, exactly as the decompile does. Internal header.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <defmodel/layout/dynary.hpp>

namespace def::dbio::dbread::w1 {

template <class T>
inline T& at(void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}
template <class T>
inline const T& at(const void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(base) + off);
}
inline std::byte* ptr(void* base, std::ptrdiff_t off) { return static_cast<std::byte*>(base) + off; }
inline model::DYNARY* dyn(void* base, std::ptrdiff_t off) { return reinterpret_cast<model::DYNARY*>(ptr(base, off)); }

// 8-byte copy (the original moves the double through a GPR or MOVSD; bits are kept)
inline void copy8(void* dst, std::ptrdiff_t doff, const void* src, std::ptrdiff_t soff)
{
    std::memcpy(static_cast<std::byte*>(dst) + doff, static_cast<const std::byte*>(src) + soff, 8);
}

// 140484ab8: CRT abs (wraps INT_MIN like the original's NEG)
inline std::int32_t crt_abs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

// FUN_14047903c() + 0x30: stdout of the VS2010 CRT
std::FILE* out_stream();

} // namespace def::dbio::dbread::w1
