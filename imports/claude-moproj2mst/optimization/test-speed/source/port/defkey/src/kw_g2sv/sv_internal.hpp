// Internal helpers of packet kw-g2sv: raw field access at the offsets the decompile uses.
//
// The object's state-variable arrays live in sub-objects that defmodel keeps opaque or names by
// slot (LLOBJ_DATA +0x8a0 / +0x9e0 / +0xae8), and several model fields have no member yet, so the
// readers and writers address them by offset, exactly as the binary does.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::key::g2sv::detail {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

template <class T, class B>
inline T& at(B& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}
template <class T, class B>
inline const T& at(const B& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&base) + off);
}

// obj+0x880 nodes, obj+0x884 elements, obj+8 object number, obj+0x3f24 output number
inline std::int32_t& nodes(LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x880); }
inline std::int32_t& elems(LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x884); }
inline std::int32_t& obj_no(LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x8); }
inline std::int32_t& out_no(LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x3f24); }
inline double*& darr(LLOBJ_DATA& o, std::size_t off) { return at<double*>(o, off); }
inline std::int32_t*& iarr(LLOBJ_DATA& o, std::size_t off) { return at<std::int32_t*>(o, off); }

// The model's component table (+0x15f8, 0x4f ints): ncomp[code].
inline std::int32_t& ncomp(DB_STEP_DATA& m, int code) { return m.u15f8[code]; }
inline std::int32_t& mfield(DB_STEP_DATA& m, std::size_t off) { return at<std::int32_t>(m, off); }

inline bool is_2d(const DB_STEP_DATA& m) { return m.dimension == 2; }  // 1400015e0
inline bool is_3d(const DB_STEP_DATA& m) { return m.dimension == 3; }  // 1400015f0

// (int)d as compiled: cvttsd2si (INT_MIN for NaN and out-of-range values).
inline std::int32_t trunc_i32(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) return INT32_MIN;
    return static_cast<std::int32_t>(d);
}

} // namespace def::key::g2sv::detail
