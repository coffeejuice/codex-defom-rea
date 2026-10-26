// defdbio/src/gendb/raw.hpp: field access at the binary's offsets for the model objects whose
// layout is kept as byte slots (packet p4-02a).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::dbio::gendb::raw {

template <class T>
inline T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(base) + off);
}

template <class T>
inline const T& at(const void* base, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const char*>(base) + off);
}

inline void* ptr(void* base, std::size_t off) { return static_cast<char*>(base) + off; }

} // namespace def::dbio::gendb::raw
