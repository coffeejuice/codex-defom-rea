// Small helpers shared by the kw-g57 sources (not public).
#pragma once

#include <cstdint>

namespace def::key::g57::detail {

// (int)d as compiled: cvttsd2si, which gives INT_MIN (0x80000000) for NaN and out-of-range values.
inline std::int32_t trunc_i32(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) return INT32_MIN;
    return static_cast<std::int32_t>(d);
}

} // namespace def::key::g57::detail
