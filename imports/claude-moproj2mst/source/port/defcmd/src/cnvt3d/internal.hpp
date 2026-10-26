// Shared helpers of the p9-01 sources (not public).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace def::cmd::cnvt3d::detail {

// field of type T at byte offset off (the model classes keep these members as raw bytes)
template <class T>
inline T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}
template <class T>
inline const T& at(const void* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(p) + off);
}
inline std::byte* add(void* p, std::size_t off) { return static_cast<std::byte*>(p) + off; }
inline const std::byte* add(const void* p, std::size_t off) { return static_cast<const std::byte*>(p) + off; }

} // namespace def::cmd::cnvt3d::detail
