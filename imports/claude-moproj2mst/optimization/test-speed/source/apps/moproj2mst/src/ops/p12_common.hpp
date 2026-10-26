// Small helpers of the P12 writers (Boolean, Convert2Dto3D, CopyMirror). Owner P12.
#pragma once

#include <cstdint>

#include "m2m/model.hpp"

namespace m2m::mst::p12 {

// @src 0x140065270  ICB flag set: flag[n-1] = v. The original has no range check and writes
// outside the 100 flags for an object number outside 1..100; the port drops such a write.
inline void icbSetFlag(ContactBlock& c, int n, std::uint8_t v)
{
    if (n < 1 || n > static_cast<int>(c.flag.size())) return;
    c.flag[static_cast<size_t>(n - 1)] = v;
}

// @src 0x1400ab7e0  ICB gen byte set
inline void icbSetGen(ContactBlock& c, std::uint8_t v)
{
    c.gen = v;
}

// @src 0x140050b10  CURSIM third value: always 0
inline int cursimThird(const Operation&)
{
    return 0;
}

}  // namespace m2m::mst::p12
