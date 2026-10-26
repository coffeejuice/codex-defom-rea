// Shared helpers of the group "surface" (packet p67-7): image accessors for the SAVE locals of
// the PathScale Fortran routines and the integer / floating-point idioms the listings use.
#pragma once

#include <defcmd/ctc3d/common.hpp>

#include <defrt/ftn/sse_ops.hpp>

#include <cstdint>

namespace def::cmd::ctc3d::surf {

// SAVE / COMMON variable at binary address a.
inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline double& D(std::uint64_t a) { return m<double>(a); }
inline i32* IP(std::uint64_t a) { return mp<i32>(a); }
inline double* DP(std::uint64_t a) { return mp<double>(a); }

// Fortran ABS(INTEGER): `t = -x; if (t < 0) t = x` (NEG / CMOVS), INT_MIN stays INT_MIN.
inline i32 iabs(i32 v)
{
    const i32 n = static_cast<i32>(0u - static_cast<std::uint32_t>(v));
    return n < 0 ? v : n;
}

// Fortran integer division / MOD (truncate toward zero), as IDIV.
inline i32 idiv(i32 a, i32 b) { return a / b; }
inline i32 imod(i32 a, i32 b) { return a % b; }

// ((a1*b1 + a2*b2) + a3*b3) in the listings' register order: t1 = a2*b2 + a1*b1 (dest a2*b2),
// then a3*b3 + t1 (dest a3*b3).
inline double dot3(const double* a, const double* b)
{
    namespace s = def::rt::sse;
    return s::add(s::mul(a[2], b[2]), s::add(s::mul(a[1], b[1]), s::mul(a[0], b[0])));
}

// Unit for the engine's messages: the INTEGER at 14077a7bc (1 = NEWEQ.MSG).
inline int msg_unit() { return dat_14077a7bc; }

// Fortran STOP (blank stop code) as the PathScale runtime does it (FUN_1404b5acc): " STOP " is
// printed only when bit 0x20 of the runtime flags (14072fba0) is set, which the port treats as
// unset, so nothing is printed; then the units are closed and the process exits with status 0.
[[noreturn]] void ftn_stop();

} // namespace def::cmd::ctc3d::surf
