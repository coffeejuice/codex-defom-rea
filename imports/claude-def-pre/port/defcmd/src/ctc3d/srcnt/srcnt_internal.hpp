// Shared helpers of the SRCNT group (packet p67-7, group "srcnt"): image access shorthands,
// Fortran integer/float idioms as the listings compute them, and the DIE_00k.GEO record
// writers (split out of 14003cd08 / 14002b89c so that their bytes can be tested).
#pragma once

#include <defcmd/ctc3d/common.hpp>

#include <emmintrin.h>

#include <bit>
#include <cstdint>

namespace def::cmd::ctc3d::srcnt_detail {

using u64 = std::uint64_t;

inline i32& I(u64 a) { return m<i32>(a); }
inline double& D(u64 a) { return m<double>(a); }
inline i32* IP(u64 a) { return mp<i32>(a); }
inline double* DP(u64 a) { return mp<double>(a); }

// Fortran ABS on INTEGER as compiled (NEG + CMOVS): ABS(-2147483648) stays -2147483648.
inline i32 iabs(i32 v)
{
    i32 n = static_cast<i32>(0u - static_cast<std::uint32_t>(v));
    return n < 0 ? v : n;
}
// -x with wrap-around (NEG)
inline i32 ineg(i32 v) { return static_cast<i32>(0u - static_cast<std::uint32_t>(v)); }

// ABS of a REAL*8: ANDPD with 0x7fff...ffff (DAT_140bad200)
inline double dabs(double v) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(v) & 0x7fffffffffffffffull); }
// -x: XORPD with -0.0 (DAT_140bad268)
inline double dneg(double v) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(v) ^ 0x8000000000000000ull); }

// INT(x): CVTTSD2SI (out of range and NaN give 0x80000000)
inline i32 cvtt(double v) { return _mm_cvttsd_si32(_mm_set_sd(v)); }

// i-th column (1-based) of a (3, *) REAL*8 array at address a
inline double* col3(u64 a, i32 i) { return DP(a + 24 * static_cast<std::int64_t>(i - 1)); }

// ---- DIE_00k.GEO (14003cd08 / 14002b89c) ---------------------------------------------------

// part of @src 14003cd08: WRITE(name(7:7),'(I1)') k (k < 10) or WRITE(name(6:7),'(I2)') k
// (k < 100) into the DIE_000.GEO buffer s_DIE_000_GEO_1405c68c0; k >= 100 leaves it unchanged.
void die_name_set(i32 k);
// part of @src 14003cd08: after the file is written, WRITE(name(6:6),'(I1)') 0 when k > 9.
void die_name_reset(i32 k);
// The 11 bytes of the buffer.
const char* die_name();

// part of @src 14002b89c: the three record kinds on unit 81.
void geo_write_count(i32 n);                               // 1000 FORMAT(I8)
void geo_write_point(i32 i, const double* xyz);            // 1001 FORMAT(I6,3F16.7)
void geo_write_poly(i32 j, i32 n1, i32 n2, i32 n3, i32 n4); // 1002 FORMAT(5I8)

} // namespace def::cmd::ctc3d::srcnt_detail
