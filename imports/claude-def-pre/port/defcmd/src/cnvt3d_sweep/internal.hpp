// Internal helpers of packet p9-03a (cnvt3d_sweep).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <type_traits>

#include "ext.hpp"

namespace def::cmd::cnvt3d_sweep::detail {

// Field at a byte offset (the layouts of LLOBJ_DATA / DB_STEP_DATA / _MeshGenBase are mostly
// byte blobs; offsets from the decompile).
template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(const_cast<std::remove_const_t<B>*>(base)) + off);
}
template <class B>
void* ptr(B* base, std::size_t off)
{
    return reinterpret_cast<std::byte*>(const_cast<std::remove_const_t<B>*>(base)) + off;
}

inline Ext& E() { return seam(); }

} // namespace def::cmd::cnvt3d_sweep::detail

namespace def::cmd::cnvt3d_sweep {
}

namespace def::cmd::cnvt3d_sweep::detail {

} // namespace def::cmd::cnvt3d_sweep::detail
