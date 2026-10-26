// Small helpers shared by the kw-g3 sources (not public).
#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace def::key::g3::detail {

// A 64-bit model field that holds a double (layout headers type unknown 8-byte fields as u64).
inline double& dbl(std::uint64_t& u) { return *reinterpret_cast<double*>(&u); }
inline double dbl(const std::uint64_t& u) { return *reinterpret_cast<const double*>(&u); }

// (int)d as compiled: cvttsd2si, which gives INT_MIN (0x80000000) for NaN and out-of-range values.
inline std::int32_t trunc_i32(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) return INT32_MIN;
    return static_cast<std::int32_t>(d);
}

// The original dereferences a null pointer at `addr` here (an access violation). The port stops
// with a message instead of undefined behaviour.
[[noreturn]] inline void null_deref(std::uint64_t addr)
{
    std::fprintf(stderr, "def_pre port: the original dereferences NULL at 0x%llx\n",
                 static_cast<unsigned long long>(addr));
    std::abort();
}

} // namespace def::key::g3::detail
