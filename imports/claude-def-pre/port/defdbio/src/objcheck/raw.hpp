// defdbio/src/objcheck/raw.hpp: member access by binary offset for packet p4-03.
//
// The object checks read many LLOBJ_DATA / DB_STEP_DATA / Material members that the layout
// headers (owned by other packets) still keep as opaque byte slots. Reading them through these
// helpers keeps the offsets visible next to the decompile; when an owner names a member the call
// site can switch to it.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::dbio::objcheck::raw {

template <class T>
inline T& at(void* p, std::int64_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T>
inline const T& at(const void* p, std::int64_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline std::int32_t& i32(void* p, std::int64_t off) { return at<std::int32_t>(p, off); }
inline std::int32_t i32(const void* p, std::int64_t off) { return at<std::int32_t>(p, off); }
inline double& f64(void* p, std::int64_t off) { return at<double>(p, off); }
inline double f64(const void* p, std::int64_t off) { return at<double>(p, off); }
inline std::uint8_t& u8(void* p, std::int64_t off) { return at<std::uint8_t>(p, off); }
inline std::uint8_t u8(const void* p, std::int64_t off) { return at<std::uint8_t>(p, off); }
inline void*& ptr(void* p, std::int64_t off) { return at<void*>(p, off); }
inline void* ptr(const void* p, std::int64_t off) { return at<void*>(p, off); }
inline char* add(void* p, std::int64_t off) { return static_cast<char*>(p) + off; }

// LLOBJ_DATA+0x3a98 (15000): the owning DB_STEP_DATA.
inline void* owner(const void* obj) { return ptr(obj, 0x3a98); }
// DB_STEP_DATA+0x750: simulation control.
inline void* sim(void* step) { return add(step, 0x750); }

// 1400015e0 / 1400015f0 (p1-01, inlined everywhere): DB_STEP_DATA's dimension word (+0x1c).
inline bool is2d(const void* step) { return i32(step, 0x1c) == 2; }
inline bool is3d(const void* step) { return i32(step, 0x1c) == 3; }

} // namespace def::dbio::objcheck::raw
