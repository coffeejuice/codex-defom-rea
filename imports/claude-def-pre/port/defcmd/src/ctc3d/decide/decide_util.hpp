// Private helpers of the decide group (packet p67-7): access to the static image by address,
// integer and dot-product helpers with the binary's operation order.
#pragma once

#include <defcmd/ctc3d/common.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <bit>
#include <cstdint>

namespace def::cmd::ctc3d::decide_detail {

using u64 = std::uint64_t;
using i64 = std::int64_t;

// INTEGER*4 / REAL*8 static at binary address a (Fortran SAVE locals, COMMON members).
inline i32& I4(u64 a) { return m<i32>(a); }
inline double& R8(u64 a) { return m<double>(a); }
inline i32* PI(u64 a) { return mp<i32>(a); }
inline double* PD(u64 a) { return mp<double>(a); }

// NEG + CMOVS: |v| with the two's complement wrap of INT_MIN.
inline i32 iabs(i32 v) { return v < 0 ? static_cast<i32>(0u - static_cast<std::uint32_t>(v)) : v; }
// NEG: -v with wrap.
inline i32 ineg(i32 v) { return static_cast<i32>(0u - static_cast<std::uint32_t>(v)); }

// ANDPD with the abs mask / XORPD with the sign mask.
inline double fabs_b(double v)
{
    return std::bit_cast<double>(std::bit_cast<std::uint64_t>(v) & 0x7fffffffffffffffull);
}
inline double fneg_b(double v)
{
    return std::bit_cast<double>(std::bit_cast<std::uint64_t>(v) ^ 0x8000000000000000ull);
}

// 3-term dot product as every listing of the group computes it: v(3)*w(3) +
// (v(2)*w(2) + v(1)*w(1)), the products with v as the MULSD destination, the sums with the later
// term as the ADDSD destination.
inline double dot_l(const double* v, const double* w)
{
    using namespace def::rt;
    const double x = sse::mul(v[0], w[0]);
    const double y = sse::mul(v[1], w[1]);
    const double z = sse::mul(v[2], w[2]);
    return sse::add(z, sse::add(y, x));
}

// Element (r) of the BCCDEF row of node n: bcc + 4*((n-1)*ld + r-1), ld = max(ndof, 0).
inline i64 ld_of(const i32* ndof) { return *ndof < 0 ? 0 : static_cast<i64>(*ndof); }
inline i32* bc_row(i32* bc, i64 ld, i32 n) { return bc + (static_cast<i64>(n) - 1) * ld; }

// Cube entity (instance) of object k: DAT_140778684[k].
inline i32& inst_of(i32 k) { return m<i32>(0x140778684ull + 4ull * static_cast<u64>(static_cast<i64>(k))); }

// Number of polygons of (obj, inst): DAT_14076e588[obj*3 + inst*300].
inline i32& npoly_of(i32 obj, i32 inst)
{
    return m<i32>(0x14076e588ull + 4ull * static_cast<u64>(static_cast<i64>(obj) * 3 + static_cast<i64>(inst) * 300));
}

// Message unit DAT_14077a7bc (1 = NEWEQ.MSG).
inline int msg_unit() { return dat_14077a7bc; }

} // namespace def::cmd::ctc3d::decide_detail
