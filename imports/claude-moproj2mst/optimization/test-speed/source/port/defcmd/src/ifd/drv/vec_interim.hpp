// INTERIM copies of p67-1's small vector helpers (include/defcmd/vec3.hpp, namespace
// def::cmd::v3), used until that header is on this branch; then these forward to it. Not tagged
// @src on purpose: p67-1 owns the ports. Operation order as in the listings.
#pragma once

#include <cmath>

namespace def::cmd::ifd::iv {

// interim copy of 14023b3a0: ((a0*b0) + (a1*b1)) + (a2*b2)
inline double dot(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

// interim copy of 14023ace0: |a_i - b_i| < tol for i = 0, 1, 2 (each test only when the previous held)
inline bool near3(const double* a, const double* b, double tol)
{
    return std::fabs(a[0] - b[0]) < tol && std::fabs(a[1] - b[1]) < tol && std::fabs(a[2] - b[2]) < tol;
}

// interim copy of 14023b050: copy 3 doubles
inline void copy3(const double* src, double* dst)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

} // namespace def::cmd::ifd::iv
