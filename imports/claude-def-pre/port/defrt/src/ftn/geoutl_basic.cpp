// Geometry kernel library, basic group (defrt/ftn/geoutl_basic.hpp). The floating-point operation
// order follows the PathScale listings (instruction addresses in the comments where it is not a
// left-to-right reading). + and * go through def::rt::sse (sse_ops.hpp) in the listing's
// dest, src order so that NaN sign/payload match too; sqrt is the SQRTSD instruction.
#include <defrt/ftn/geoutl_basic.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>

#include <bit>
#include <cmath>
#include <cstdint>

namespace def::rt::geoutl {

namespace {

using sse::add;
using sse::div;
using sse::mul;
using sse::sub;

// DAT_140bb2668: REAL*4 1e-16 widened to double (0x3c9cd2b2a0000000)
const double kCrossTiny = std::bit_cast<double>(std::uint64_t{0x3c9cd2b2a0000000});
// DAT_140643930: 1e-9 (0x3e112e0be826d695)
const double kDirTiny = std::bit_cast<double>(std::uint64_t{0x3e112e0be826d695});
// DAT_140bb2678: 1e-5 (0x3ee4f8b588e368f1)
const double kSameTol = std::bit_cast<double>(std::uint64_t{0x3ee4f8b588e368f1});

} // namespace

void reset_globals_basic() {
    g_cross_inv = 0.0;
    g_quad_normal = QuadNormalState{};
    g_tri_normal = TriNormalState{};
    g_tri_normal_len = 0.0;
    g_dir_matrix = DirMatrixState{};
    g_rot_frame = RotFrameState{};
    g_rot2d = Rot2dState{};
    g_same_point = SamePointState{};
    g_xform = XformState{};
}

// @src 1404cce5e  c = a x b, normalised, |c| out
void fun_1404cce5e(const double* a, const double* b, double* c, double* len) {
    // Memory order kept for callers that pass c aliased with a or b: a(1), b(1) are read again
    // after c(1) is stored (1404ccea0, 1404ccea4), the other loads come before it.
    const double b2 = b[2];
    const double a1 = a[1];
    const double b1 = b[1];
    const double a2 = a[2];
    const double c0 = sub(mul(a1, b2), mul(b1, a2));
    const double b0 = b[0];
    const double a0 = a[0];
    c[0] = c0;
    const double c1 = sub(mul(a2, b[0]), mul(b2, a[0]));
    const double c2 = sub(mul(a0, b1), mul(b0, a1));
    const double l = sse::sqrt(add(mul(c2, c2), add(mul(c1, c1), mul(c0, c0)))); // 1404ccec4..edc
    c[2] = c2;
    c[1] = c1;
    *len = l;
    if (kCrossTiny >= l) { // comisd tiny, l; jp/jc: unordered normalises
        *len = 0.0;
        return;
    }
    const double inv = div(1.0, *len); // DAT_140bb2658 / [len]
    g_cross_inv = inv;
    c[2] = mul(c2, inv);
    c[1] = mul(c1, inv);
    c[0] = mul(c0, inv);
}

// @src 1404d68dd  unit vector from p to q, length out
void fun_1404d68dd(const double* p, const double* q, double* u, double* len) {
    // Memory order kept for u aliased with p or q: q(k) is loaded before u(k-1) is stored, p(k)
    // after it.
    const double u0 = q[0] - p[0];
    const double q1 = q[1];
    u[0] = u0;
    const double u1 = q1 - p[1];
    const double q2 = q[2];
    u[1] = u1;
    const double u2 = q2 - p[2];
    const double l2 = add(mul(u2, u2), add(mul(u1, u1), mul(u0, u0))); // 1404d6910..1404d6924
    u[2] = u2;
    *len = l2;
    if (l2 == 0.0) { // comisd l2, 0; jp/jz: NaN goes on
        return;
    }
    const double l = sse::sqrt(*len);
    *len = l;
    u[2] = div(u2, l);
    u[1] = div(u1, l);
    u[0] = div(u0, l);
}

// @src 1404cf5c0  rotation matrix of the direction v
void fun_1404cf5c0(const double* v, double* m) {
    DirMatrixState& g = g_dir_matrix;
    const double x = v[0];
    const double y = v[1];
    g.zero_vector = 0;
    double z;
    if (x == 0.0) {
        z = v[2];
        if (y == 0.0 && z == 0.0) {
            g.zero_vector = 1;
            return;
        }
    } else {
        z = v[2];
    }
    const double xx = mul(x, x);
    const double zz = mul(z, z);
    const double rxz = sse::sqrt(add(xx, zz));          // 1404cf63e, 1404cf647
    const double r = sse::sqrt(add(add(mul(y, y), xx), zz)); // 1404cf639, 1404cf643, 1404cf64c
    const double c = div(rxz, r);
    const double s = div(y, r);
    g.y = y;
    g.z = z;
    g.x = x;
    g.rxz = rxz;
    g.r = r;
    g.c = c;
    g.s = s;
    if (!(rxz >= kDirTiny)) { // comisd rxz, 1e-9; jp/jc: unordered takes this branch too
        m[8] = c;
        m[5] = s;
        m[4] = c;
        m[2] = 0.0;
        m[1] = 0.0;
        m[6] = 0.0;
        m[3] = 0.0;
        m[0] = 1.0; // DAT_140bb2658
        m[7] = -s;  // xorpd with DAT_140bb2688 (sign bit)
        return;
    }
    const double ux = div(x, rxz);
    const double uz = div(z, rxz);
    m[4] = c;
    m[3] = 0.0;
    m[5] = s;
    g.ux = ux;
    m[0] = uz;
    g.uz = uz;
    m[6] = -ux;
    m[8] = mul(uz, c);
    m[2] = mul(c, ux);
    m[7] = -mul(uz, s);
    m[1] = -mul(s, ux);
}

// @src 1404ded0c  b(:,i) = R (a(:,i) - c) for mode < 2, else R^T a(:,i) + c
void fun_1404ded0c(const double* a, double* b, const double* c, const double* r,
                   const std::int32_t* n, const std::int32_t* mode) {
    XformState& g = g_xform;
    // 1-based column-major accessors
    auto A = [a](std::int64_t k, std::int64_t i) { return a[(k - 1) + 3 * (i - 1)]; };
    auto B = [b](std::int64_t k, std::int64_t i) -> double& { return b[(k - 1) + 3 * (i - 1)]; };
    auto R = [r](std::int64_t k, std::int64_t l) { return r[(k - 1) + 3 * (l - 1)]; };
    if (*mode < 2) {
        g.i = 1;
        if (1 <= *n) {
            do {
                g.j = 1;
                do {
                    double s = 0.0;
                    g.k = 1;
                    B(g.j, g.i) = 0.0;
                    do {
                        s = add(s, mul(sub(A(g.k, g.i), c[g.k - 1]), R(g.j, g.k)));
                        g.k = g.k + 1;
                        B(g.j, g.i) = s;
                    } while (g.k <= 3);
                    g.j = g.j + 1;
                } while (g.j <= 3);
                g.i = g.i + 1;
            } while (g.i <= *n);
        }
    } else {
        g.i = 1;
        if (1 <= *n) {
            do {
                g.j = 1;
                do {
                    double s = 0.0;
                    g.k = 1;
                    B(g.j, g.i) = 0.0;
                    do {
                        s = add(s, mul(A(g.k, g.i), R(g.k, g.j)));
                        g.k = g.k + 1;
                        B(g.j, g.i) = s;
                    } while (g.k <= 3);
                    B(g.j, g.i) = add(s, c[g.j - 1]);
                    g.j = g.j + 1;
                } while (g.j <= 3);
                g.i = g.i + 1;
            } while (g.i <= *n);
        }
    }
}

// @src 1404d71d7  rotate p about the origin by *a
void fun_1404d71d7(double* p, const double* a) {
    Rot2dState& g = g_rot2d;
    double s;
    double c;
    libm::ps_sincos(*a, &s, &c); // 1404b3b30(a, &s, &c)
    const double y = p[1];
    const double x = p[0];
    const double t1 = mul(s, y);
    const double t2 = mul(c, y);
    double ny = mul(s, x);
    double nx = mul(c, x);
    g.cos_a = c;
    ny = add(ny, t2);
    g.sin_a = s;
    nx = sub(nx, t1);
    g.y = y;
    g.x = x;
    p[1] = ny;
    p[0] = nx;
}

// @src 1404d0939  b(:,i) = R (a(:,i) - c), b(3,i) negated unless *flag > 0
void fun_1404d0939(const double* a, const double* c, const double* r, double* b,
                   const std::int32_t* flag, const std::int32_t* n) {
    RotFrameState& g = g_rot_frame;
    g.i = 1;
    if (*n < 1) {
        return;
    }
    do {
        const std::int64_t col = 3 * static_cast<std::int64_t>(g.i - 1);
        g.j = 1;
        do {
            g.d[g.j - 1] = a[col + (g.j - 1)] - c[g.j - 1];
            g.j = g.j + 1;
        } while (g.j <= 3);
        g.k = 1;
        do {
            double s = 0.0;
            g.j = 1;
            g.w[g.k - 1] = 0.0;
            do {
                s = add(s, mul(r[(g.k - 1) + 3 * static_cast<std::int64_t>(g.j - 1)], g.d[g.j - 1]));
                g.j = g.j + 1;
                g.w[g.k - 1] = s;
            } while (g.j <= 3);
            g.k = g.k + 1;
        } while (g.k <= 3);
        g.sign = -1.0; // DAT_140bb2650
        if (*flag > 0) {
            g.sign = 1.0; // DAT_140bb2658
        }
        g.j = 1;
        do {
            b[col + (g.j - 1)] = g.w[g.j - 1];
            g.j = g.j + 1;
        } while (g.j <= 3);
        b[col + 2] = mul(b[col + 2], g.sign);
        g.i = g.i + 1;
    } while (g.i <= *n);
}

// @src 1404cccfb  unit normal of the bilinear quad x(3,4) at (xi, eta)
void fun_1404cccfb(const double* x, const double* xi, const double* eta, double* n) {
    QuadNormalState& g = g_quad_normal;
    g.i = 1;
    const double e = *eta;
    const double em = add(e, -1.0); // DAT_140bb2650
    const double me = sub(1.0, e);
    const double ep = add(e, 1.0);
    const double s = *xi;
    const double sm = add(s, -1.0);
    const double sp = add(s, 1.0);
    const double ms = sub(1.0, s);
    do {
        const std::int64_t k = g.i - 1;
        const double x1 = x[k];
        const double x2 = x[k + 3];
        const double x3 = x[k + 6];
        const double x4 = x[k + 9];
        // 1404ccd92..1404ccdf4: (((1-eta) x2 + (eta-1) x1) + (eta+1) x3 - (eta+1) x4) / 4
        double d1 = add(mul(me, x2), mul(em, x1));
        d1 = add(mul(ep, x3), d1);
        d1 = sub(d1, mul(ep, x4));
        // 1404ccdbb..1404cce0d: ((xi-1) x1 - (xi+1) x2 + (xi+1) x3) + (1-xi) x4, / 4
        double d2 = sub(mul(x1, sm), mul(x2, sp));
        d2 = add(d2, mul(x3, sp));
        d2 = add(mul(x4, ms), d2);
        g.dxi[k] = mul(d1, 0.25); // DAT_140bb2660
        g.deta[k] = mul(d2, 0.25);
        g.i = g.i + 1;
    } while (g.i <= 3);
    fun_1404cce5e(g.dxi, g.deta, n, &g.len);
}

// @src 1404d3d43  unit normal of the triangle x(3,3)
void fun_1404d3d43(const double* x, double* n) {
    TriNormalState& g = g_tri_normal;
    const double e1x = x[3] - x[0];
    const double e2z = x[8] - x[2];
    g.e1[0] = e1x;
    const double e1y = x[4] - x[1];
    const double e2y = x[7] - x[1];
    g.e1[1] = e1y;
    const double e1z = x[5] - x[2];
    const double e2x = x[6] - x[0];
    g.e2[2] = e2z;
    g.e2[1] = e2y;
    g.e2[0] = e2x;
    g.e1[2] = e1z;
    fun_1404cce5e(g.e1, g.e2, n, &g_tri_normal_len);
}

// @src 1404e33b7  *same = 1 when a and b agree within 1e-5 per component
void fun_1404e33b7(const double* a, const double* b, std::int32_t* same) {
    SamePointState& g = g_same_point;
    *same = 1;
    const double d0 = std::fabs(a[0] - b[0]); // andpd with DAT_140bb2638
    g.d[0] = d0;
    const double d1 = std::fabs(a[1] - b[1]);
    g.d[1] = d1;
    const double d2 = std::fabs(a[2] - b[2]);
    g.d[2] = d2;
    if (d0 > kSameTol || d1 > kSameTol || d2 > kSameTol) { // ja / ja / jp-jbe: NaN = equal
        *same = 0;
    }
}

// @src 1404edb2f  bare RET
void fun_1404edb2f(const std::int32_t* /*code*/) {}

} // namespace def::rt::geoutl
