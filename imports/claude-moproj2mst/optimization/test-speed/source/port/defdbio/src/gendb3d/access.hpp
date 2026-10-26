// gendb3d/access.hpp: raw-offset access for p4-05.
//
// LLOBJ_DATA's sub-objects (0x8a0 node fields, 0x9e0 element fields, 0xbe0 boundary records,
// 0x1ea0.. BC/movement sets) are opaque byte slots in the P1 layout header, and their classes are
// owned by other packets. The writers read them by the binary's offsets, which keeps every line
// comparable with the decompile: fld<int32_t>(obj, 0x880) is *(int *)(obj + 0x880).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::g3d {

template <class T> inline T& fld(void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(base) + off);
}
template <class T> inline const T& fld(const void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const char*>(base) + off);
}
inline char* at(void* base, std::ptrdiff_t off) { return static_cast<char*>(base) + off; }

// *(obj + 15000): the object's DB_STEP_DATA
inline model::DB_STEP_DATA* owner_of(const model::LLOBJ_DATA* obj) { return obj->owner; }

// Region start / width through the index tables (DAT_1406e81fc.., DAT_1406e82d0..); the argument
// is the global's address, so the call site reads like the decompile.
inline std::int32_t mar_ix(std::uint64_t dat) { return model::g_mar_index[(dat - 0x1406e81fcull) / 4]; }
inline std::int32_t a_ix(std::uint64_t dat) { return model::g_a_index[(dat - 0x1406e82d0ull) / 4]; }

} // namespace def::dbio::g3d
