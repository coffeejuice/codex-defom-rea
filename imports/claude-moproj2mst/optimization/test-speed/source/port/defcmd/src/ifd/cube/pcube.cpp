// pcube.cpp: polygon / unit-cube intersection as linked into DEF_PRE (the structure of Green &
// Hatch, "Fast polygon-cube intersection testing", Graphics Gems V), transcribed from the
// decompile and the listing operation by operation (out/p672/all_lst.txt). The cube is
// [-0.5, 0.5]^3. Comparisons follow the comisd/ucomisd + branch pairs of the listing; where a
// NaN takes a different branch than the decompile suggests, the listing wins (noted below).
#include <defcmd/ifd/cube_lib.hpp>

namespace def::cmd::cube {

// @src 140454110  unnormalised polygon normal of v(3, nv): sum over k = 2..nv-1 of
// (v_{k-1} - v_0) x (v_k - v_0), each component written as the listing computes it
void fun_140454110(double* n, std::int32_t nv, const double* v)
{
    n[0] = 0.0;
    n[1] = 0.0;
    n[2] = 0.0;
    double ax = v[3] - v[0];
    double ay = v[4] - v[1];
    double az = v[5] - v[2];
    for (std::int32_t k = 2; k <= nv - 1; ++k) {
        const double bx = v[k * 3] - v[0];
        const double by = v[k * 3 + 1] - v[1];
        const double bz = v[k * 3 + 2] - v[2];
        const double cx = ay * bz + az * (-by);
        const double cy = -(ax * bz + az * (-bx));
        const double cz = ax * by + ay * (-bx);
        n[0] = n[0] + cx;
        n[1] = n[1] + cy;
        n[2] = n[2] + cz;
        ax = bx;
        ay = by;
        az = bz;
    }
}

// @src 1404531b0  face planes: bit 1/2 for x > 0.5 / x < -0.5, 4/8 for y, 0x10/0x20 for z,
// only planes whose bits are in mask (DAT_14051f240, DAT_14056aed0)
std::uint32_t fun_1404531b0(const double* p, std::uint32_t mask)
{
    std::uint32_t r = 0;
    if ((mask & 3) != 0) {
        const double x = p[0];
        if ((mask & 1) != 0 && x > 0.5) {
            r = r | 1;
        } else if ((mask & 2) != 0 && -0.5 > x) {
            r = r | 2;
        }
    }
    if ((mask & 0xc) != 0) {
        const double y = p[1];
        if ((mask & 4) != 0 && y > 0.5) {
            r = r | 4;
        } else if ((mask & 8) != 0 && -0.5 > y) {
            r = r | 8;
        }
    }
    if ((mask & 0x30) != 0) {
        const double z = p[2];
        if ((mask & 0x10) != 0 && z > 0.5) {
            r = r | 0x10;
        } else if ((mask & 0x20) != 0 && -0.5 > z) {
            r = r | 0x20;
        }
    }
    return r;
}

namespace {
// part of @src 140453300 / 1404535e0: one pair of bevel planes (value > lim / value < -lim)
// bits is a pair (pos = lower bit, neg = upper bit); comisd v, lim / comisd nlim, v + jbe:
// a NaN sets neither bit
std::uint32_t bevel(std::uint32_t mask, std::uint32_t bits, double v, double lim, double nlim)
{
    if ((mask & bits) == 0) {
        return 0;
    }
    const std::uint32_t pos = bits & (0u - bits); // lowest set bit
    const std::uint32_t neg = bits & ~pos;
    if ((mask & pos) != 0 && v > lim) {
        return pos;
    }
    if ((mask & neg) != 0 && nlim > v) {
        return neg;
    }
    return 0;
}
} // namespace

// @src 140453300  edge bevels: x+y, x-y, x+z, x-z, y+z, y-z against 1.0 / -1.0 (DAT_140509c90,
// DAT_140522fc8), bit pairs 3, 0xc, 0x30, 0xc0, 0x300, 0xc00
std::uint32_t fun_140453300(const double* p, std::uint32_t mask)
{
    std::uint32_t r = 0;
    r |= bevel(mask, 0x3, p[0] + p[1], 1.0, -1.0);
    r |= bevel(mask, 0xc, p[0] - p[1], 1.0, -1.0);
    r |= bevel(mask, 0x30, p[0] + p[2], 1.0, -1.0);
    r |= bevel(mask, 0xc0, p[0] - p[2], 1.0, -1.0);
    r |= bevel(mask, 0x300, p[1] + p[2], 1.0, -1.0);
    r |= bevel(mask, 0xc00, p[1] - p[2], 1.0, -1.0);
    return r;
}

// @src 1404535e0  corner bevels: (x+y)+z, (x+y)-z, (x-y)+z, (x-y)-z against 1.5 / -1.5
// (DAT_14051f1d0, DAT_14056aed8), bit pairs 3, 0xc, 0x30, 0xc0
std::uint32_t fun_1404535e0(const double* p, std::uint32_t mask)
{
    std::uint32_t r = 0;
    if ((mask & 3) != 0) {
        r |= bevel(mask, 0x3, (p[0] + p[1]) + p[2], 1.5, -1.5);
    }
    if ((mask & 0xc) != 0) {
        r |= bevel(mask, 0xc, (p[0] + p[1]) - p[2], 1.5, -1.5);
    }
    if ((mask & 0x30) != 0) {
        r |= bevel(mask, 0x30, (p[0] - p[1]) + p[2], 1.5, -1.5);
    }
    if ((mask & 0xc0) != 0) {
        r |= bevel(mask, 0xc0, (p[0] - p[1]) - p[2], 1.5, -1.5);
    }
    return r;
}

// @src 140453010  trivial vertex tests: 1 if a vertex is inside the cube, 0 if every vertex is
// outside one face, edge bevel or corner bevel plane, else -1
std::int32_t fun_140453010(std::int32_t nv, const double* v, std::int32_t vout)
{
    std::uint32_t cum = 0xffffffffu;
    std::int32_t i;
    if (vout == 0) {
        for (i = 0; i < nv; ++i) {
            const std::uint32_t c = fun_1404531b0(v + i * 3, 0xffffffffu);
            if (c == 0) {
                return 1;
            }
            cum = cum & c;
        }
    } else {
        i = 0;
        while (i < nv && (cum = fun_1404531b0(v + i * 3, cum)) != 0) {
            ++i;
        }
    }
    if (cum != 0) {
        return 0;
    }
    cum = 0xffffffffu;
    i = 0;
    while (i < nv && (cum = fun_140453300(v + i * 3, cum)) != 0) {
        ++i;
    }
    if (cum != 0) {
        return 0;
    }
    cum = 0xffffffffu;
    i = 0;
    while (i < nv && (cum = fun_1404535e0(v + i * 3, cum)) != 0) {
        ++i;
    }
    if (cum != 0) {
        return 0;
    }
    return -1;
}

// @src 140453c40  segment v0-v1 against the cube: 1 if it intersects
std::int32_t fun_140453c40(const double* v0, const double* v1)
{
    double e[3];
    e[0] = v1[0] - v0[0];
    e[1] = v1[1] - v0[1];
    e[2] = v1[2] - v0[2];
    std::int32_t s[3];
    for (int i = 0; i < 3; ++i) {
        s[i] = (0.0 > e[i]) ? -1 : 1; // comisd 0, e; jbe -> 1 (NaN: 1)
    }
    for (int i = 0; i < 3; ++i) {
        if (v0[i] * static_cast<double>(s[i]) > 0.5) {
            return 0;
        }
        if (-0.5 > v1[i] * static_cast<double>(s[i])) {
            return 0;
        }
    }
    for (int i = 0; i < 3; ++i) {
        const int j = (i + 1) % 3;
        const int k = (i + 2) % 3;
        const double cross = e[k] * v0[j] - e[j] * v0[k];
        const double lim = 0.5 * (e[k] * static_cast<double>(s[j]) + e[j] * static_cast<double>(s[k]));
        if (cross * cross > lim * lim) {
            return 0;
        }
    }
    return 1;
}

// @src 140453850  winding number of point p in polygon v(3, nv), projected along the dominant
// axis of n
std::int32_t fun_140453850(std::int32_t nv, const double* v, const double* n, const double* p)
{
    double a[3];
    for (int i = 0; i < 3; ++i) {
        a[i] = (0.0 > n[i]) ? -n[i] : n[i]; // comisd 0, n; jbe -> n unchanged (NaN too)
    }
    std::int32_t zaxis;
    if (a[0] > a[2]) { // comisd a0, a2; jbe
        zaxis = (a[0] > a[1]) ? 0 : 1;
    } else {
        zaxis = ((a[1] > a[2]) ? 0 : 1) + 1;
    }
    std::int32_t xaxis;
    std::int32_t yaxis;
    if (0.0 > n[zaxis]) { // comisd 0, n[z]; jbe. The decompile's "0 <= n" differs for NaN:
        xaxis = (zaxis + 2) % 3; // the listing takes the other branch, as here
        yaxis = (zaxis + 1) % 3;
    } else {
        xaxis = (zaxis + 1) % 3;
        yaxis = (zaxis + 2) % 3;
    }
    std::int32_t wn = 0;
    for (std::int32_t i = 0; i < nv; ++i) {
        const double* v0 = v + i * 3;
        const double* v1 = v + ((i + 1) % nv) * 3;
        const std::int32_t a1 = (v1[xaxis] > p[xaxis]) ? 1 : 0;
        const std::int32_t a0 = (v0[xaxis] > p[xaxis]) ? 1 : 0;
        const std::int32_t xdir = a1 - a0;
        if (xdir != 0) {
            const std::int32_t b1 = (v1[yaxis] > p[yaxis]) ? 1 : 0;
            const std::int32_t b0 = (v0[yaxis] > p[yaxis]) ? 1 : 0;
            if (b1 - b0 != 0) {
                const double lhs = (static_cast<double>(xdir) * (p[xaxis] - v0[xaxis])) * (v1[yaxis] - v0[yaxis]);
                const double rhs = (static_cast<double>(xdir) * (p[yaxis] - v0[yaxis])) * (v1[xaxis] - v0[xaxis]);
                if (rhs >= lhs) { // comisd rhs, lhs; jc skips (NaN skips)
                    wn = wn + xdir;
                }
            } else if (p[yaxis] >= v0[yaxis]) { // comisd p, v0; jc skips
                wn = wn + xdir;
            }
        }
    }
    return wn;
}

// @src 140453e80  edges against the cube, then the polygon's plane against the cube diagonal
// nearest the normal: the winding number of the crossing point (only exactly 1 counts later)
std::int32_t fun_140453e80(std::int32_t nv, const double* v, const double* n, std::int32_t eout)
{
    if (eout == 0) {
        for (std::int32_t i = 0; i < nv; ++i) {
            if (fun_140453c40(v + i * 3, v + ((i + 1) % nv) * 3) != 0) {
                return 1;
            }
        }
    }
    // ucomisd + jp/jnz: a NaN component is "not zero"
    if (n[0] == 0.0 && n[1] == 0.0 && n[2] == 0.0) { // DAT_14051bed8
        return 0;
    }
    std::int32_t s[3];
    for (int i = 0; i < 3; ++i) {
        s[i] = (0.0 > n[i]) ? -1 : 1; // comisd 0, n; jbe -> 1. The decompile's "0 <= n" differs for NaN.
    }
    const double t = (n[0] * v[0] + n[1] * v[1] + n[2] * v[2]) /
                     (n[0] * static_cast<double>(s[0]) + n[1] * static_cast<double>(s[1]) +
                      n[2] * static_cast<double>(s[2]));
    if (!((t - -0.5) * (t - 0.5) <= 0.0)) { // DAT_14056aed0, DAT_14051f240; comisd 0, prod; jnc
        return 0;
    }
    double q[3];
    q[0] = t * static_cast<double>(s[0]);
    q[1] = t * static_cast<double>(s[1]);
    q[2] = t * static_cast<double>(s[2]);
    return fun_140453850(nv, v, n, q);
}

// @src 1404537f0  polygon intersects cube: trivial vertex tests, then 140453e80 when undecided
std::int32_t fun_1404537f0(std::int32_t nv, const double* v, const double* n, std::int32_t vout,
                           std::int32_t eout)
{
    const std::int32_t r = fun_140453010(nv, v, vout);
    if (r == -1) {
        return fun_140453e80(nv, v, n, eout);
    }
    return r;
}

} // namespace def::cmd::cube
