// defcmd/src/objpos/internal.hpp: raw-offset access to the object parts the OBJPOS transforms
// touch. Most sub-objects of LLOBJ_DATA are byte slots in the layout headers, so the transforms
// read their fields at the binary's offsets (as kw-g2obj does).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::cmd::objpos::detail {

template <class T, class C>
inline T& at(C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}
template <class T, class C>
inline const T& at(const C* base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const unsigned char*>(base) + off);
}
template <class C>
inline unsigned char* ptr(C* base, std::size_t off)
{
    return reinterpret_cast<unsigned char*>(base) + off;
}

// 1400015e0 / 1400015f0 on the object's owning model (obj+15000 = +0x3a98); the binary reads the
// model's dimension, so a NULL owner crashes there as in the original.
inline bool owner_is_2d(const model::LLOBJ_DATA* o) { return o->owner->dimension == 2; }
inline bool owner_is_3d(const model::LLOBJ_DATA* o) { return o->owner->dimension == 3; }

} // namespace def::cmd::objpos::detail

