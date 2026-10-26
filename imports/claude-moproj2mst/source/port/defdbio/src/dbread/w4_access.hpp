// dbread/w4_access.hpp: raw-offset access for helper W4 of p-dbread. Objects whose members at the
// offsets the readers use are opaque in the layout headers are read as in the decompile:
// fi(obj, 0x880) is *(int *)(obj + 0x880).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/step/db_step.hpp>

namespace def::dbio::dbread::w4d {

template <class T> inline T& f(void* p, std::size_t off) { return model::field<T>(p, off); }
inline std::int32_t& fi(void* p, std::size_t off) { return model::field<std::int32_t>(p, off); }
inline double& fd(void* p, std::size_t off) { return model::field<double>(p, off); }
inline char* at(void* p, std::size_t off) { return static_cast<char*>(p) + off; }

// DAT_1406e81fc.. (MAR slots) and DAT_1406e82d0.. (A slots), addressed by the global's address
inline std::int32_t* mix_ptr(std::uint64_t dat) { return &model::g_mar_index[(dat - 0x1406e81fcull) / 4]; }
inline std::int32_t* aix_ptr(std::uint64_t dat) { return &model::g_a_index[(dat - 0x1406e82d0ull) / 4]; }
inline std::int32_t mix(std::uint64_t dat) { return *mix_ptr(dat); }
inline std::int32_t aix(std::uint64_t dat) { return *aix_ptr(dat); }

} // namespace def::dbio::dbread::w4d
