// Geometry kernel library, polygon / bilinear group (defrt/ftn/geoutl_poly.hpp). Ported from the
// PathScale listings: one C++ operation per SSE instruction where the order matters, the listing
// address of the branch in the comments. Comparisons keep the comisd sense: a test written
// `if (a < b)` is false for NaN, which is what the `jp` around the original branch does; where
// the original takes a branch on NaN the test is written negated (`!(a <= b)`).
// sqrt is the inline SQRTSD instruction (std::sqrt compiles to it); std::fabs is the and-mask
// DAT_140bb2638 and unary minus the xor with DAT_140bb2688 (-0.0).
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl_basic.hpp>
#include <defrt/ftn/geoutl_poly.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace def::rt::geoutl {

namespace {

// DAT_1406438d8: 1e-9 (0x3e112e0be826d695), .data, never written by these routines
const double kTol9 = std::bit_cast<double>(std::uint64_t{0x3e112e0be826d695});
// DAT_1406438e0..f8: corner s-coordinates (-1, 1, 1, -1)
const double kCornerS[4] = {-1.0, 1.0, 1.0, -1.0};
// DAT_140643900: INTEGER 50, bisection limit
const std::int32_t kBisectMax = 50;
// DAT_140643ba0: 1e-4 (0x3f1a36e2eb1c432d), Newton convergence
const double kNewtonTol = std::bit_cast<double>(std::uint64_t{0x3f1a36e2eb1c432d});
// DAT_140bb2640: REAL*4 1e-10 widened (0x3ddb7cdfe0000000)
const double kTiny10 = std::bit_cast<double>(std::uint64_t{0x3ddb7cdfe0000000});
// DAT_140bb2678: 1e-5 (0x3ee4f8b588e368f1)
const double kTol5 = std::bit_cast<double>(std::uint64_t{0x3ee4f8b588e368f1});
// DAT_140bb2680: 1e-6 (0x3eb0c6f7a0b5ed8d)
const double kTol6 = std::bit_cast<double>(std::uint64_t{0x3eb0c6f7a0b5ed8d});
// DAT_140bb26a0 / a8: -+1.001 as REAL*4 widened (0xbff00418a0000000 / 0x3ff00418a0000000)
const double kLo1001 = std::bit_cast<double>(std::uint64_t{0xbff00418a0000000});
const double kHi1001 = std::bit_cast<double>(std::uint64_t{0x3ff00418a0000000});
// DAT_140bb2830: REAL*4 0.001 widened (0x3f50624de0000000)
const double kStepMin = std::bit_cast<double>(std::uint64_t{0x3f50624de0000000});

// NEG/CMOVS: |i| with INT_MIN left as it is
std::int32_t iabs(std::int32_t v) {
    const std::int32_t n = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v));
    return n < 0 ? v : n;
}

// NaN-exact scalar ops (defrt/ftn/sse_ops.hpp): first argument = the destination register of
// the listing's addsd/subsd/mulsd/divsd, so a NaN result carries the original's sign/payload.
inline double fadd(double a, double b) { return sse::add(a, b); }
inline double fsub(double a, double b) { return sse::sub(a, b); }
inline double fmul(double a, double b) { return sse::mul(a, b); }
inline double fdiv(double a, double b) { return sse::div(a, b); }

} // namespace

void reset_poly_globals() {
    g_poly = PolySave{};
    g_bilin = BilinSave{};
    g_bilin_solve = BilinSolveSave{};
    g_bisect = BisectSave{};
    g_pointseg = PointSegSave{};
    g_newton = NewtonSave{};
    g_linesearch = LineSearchSave{};
    g_shape = ShapeSave{};
    g_pntlin = PntlinSave{};
}

// @src 1404d6974  bilinear interpolation out(3) = sum_k N_k(s,t) x(:,k)
void fun_1404d6974(const double* x, double* out, const double* s, const double* t) {
    ShapeSave& g = g_shape;
    const double sm = fsub(1.0, *s);
    const double sp = fadd(*s, 1.0);
    const double tp = fadd(*t, 1.0);
    const double tm = fsub(1.0, *t);
    g.d_430[2] = fmul(fmul(sp, tp), 0.25); // N3
    g.d_430[3] = fmul(fmul(tp, sm), 0.25); // N4
    g.d_430[0] = fmul(fmul(sm, tm), 0.25); // N1
    g.d_430[1] = fmul(0.25, fmul(sp, tm)); // N2
    for (g.i_450 = 1; g.i_450 <= 3; ++g.i_450) {
        const int i = g.i_450 - 1;
        double acc = 0.0;
        out[i] = 0.0;
        for (g.i_454 = 1; g.i_454 <= 4; ++g.i_454) {
            const int k = g.i_454 - 1;
            acc = fadd(acc, fmul(x[i + 3 * k], g.d_430[k]));
            out[i] = acc;
        }
    }
}

// @src 1404d064d  point-segment (2D)
void fun_1404d064d(double* p, const double* a, const double* b, double* len, double* dist,
                   double* t, const std::int32_t* mode) {
    PointSegSave& g = g_pointseg;
    const double ay = a[1], bx = b[0], by = b[1], ax = a[0];
    double ux = fsub(bx, ax);
    double uy = fsub(by, ay);
    const double l = std::sqrt(fadd(fmul(uy, uy), fmul(ux, ux)));
    ux = fdiv(ux, l);
    uy = fdiv(uy, l);
    const double px = p[0], py = p[1];
    const double wx = fsub(px, ax);
    const double wy = fsub(py, ay);
    const double proj = fadd(fmul(uy, wy), fmul(ux, wx));
    const double tt = fdiv(proj, l);
    const double dn = fsub(fmul(wx, uy), fmul(ux, wy));
    g.d_4a8 = wy;
    *len = l;
    g.d_4a0 = wx;
    g.d_490 = ux;
    g.d_498 = uy;
    g.d_4b0 = dn;
    g.d_4b8 = proj;
    *t = tt;
    if (proj >= 0.0 && l >= proj) { // 1404d073f / 1404d0773: NaN goes outside
        g.i_4c0 = 0;
        *dist = dn;
        if (*mode == 1) return;
        p[1] = fadd(fmul(uy, proj), ay);
        p[0] = fadd(fmul(proj, ux), ax);
        return;
    }
    const double ex = fsub(px, bx);
    const double ey = fsub(py, by);
    g.i_4c0 = 1;
    const bool pos = dn >= 0.0; // 1404d083d SETNC: false for NaN
    const double db = std::sqrt(fadd(fmul(ey, ey), fmul(ex, ex)));
    const double da = std::sqrt(fadd(fmul(g.d_4a8, g.d_4a8), fmul(g.d_4a0, g.d_4a0)));
    g.d_4c8 = db;
    const double adb = std::fabs(db);
    const double ada = std::fabs(da);
    if (ada > adb) { // 1404d0865
        g.i_4c0 = 2;
        *dist = pos ? adb : -adb;
        if (*mode == 1) return;
        p[1] = by;
        p[0] = bx;
        return;
    }
    *dist = pos ? ada : -ada;
    if (*mode == 1) return;
    p[1] = ay;
    p[0] = ax;
}

// @src 1404cc289  point in polygon (nearest vertex / edge, inside flag)
void fun_1404cc289(const double* xy, const double* p, double* q, double* dist,
                   std::int32_t* iedge, const std::int32_t* n, std::int32_t* inside,
                   const std::int32_t* closed, const std::int32_t* idx,
                   const std::int32_t* useidx, const std::int32_t* ld) {
    PolySave& g = g_poly;
    const std::int32_t nv = *n;
    const std::int64_t ldv = *ld;
    const std::int64_t lds = ldv >= 0 ? ldv : 0;
    auto col = [&](std::int32_t v) { return xy + (static_cast<std::int64_t>(v) - 1) * lds; };

    *inside = 1;
    *iedge = 0;
    g.i_dd8 = nv;
    g.i_ddc = 1;
    g.i_de0 = 1;
    if (*useidx == 1) g.i_de0 = idx[0]; // 1404ccc3f
    const double px = p[0], py = p[1];
    {
        const double* v = col(g.i_de0);
        const double dx = fsub(v[0], px);
        const double dy = fsub(v[1], py);
        g.i_df0 = 2;
        g.d_de8 = std::sqrt(fadd(fmul(dy, dy), fmul(dx, dx)));
    }
    if (nv >= 2) { // 1404ccb86: nearest vertex
        for (;;) {
            g.i_de0 = g.i_df0;
            if (*useidx != 0) g.i_de0 = idx[g.i_df0 - 1];
            const double* v = col(g.i_de0);
            const double dx = fsub(v[0], px);
            const double dy = fsub(v[1], py);
            const double d = std::sqrt(fadd(fmul(dy, dy), fmul(dx, dx)));
            g.d_df8 = d;
            if (g.d_de8 > d) { // 1404ccc06
                g.d_de8 = d;
                g.i_ddc = g.i_df0;
            }
            g.i_df0 = g.i_df0 + 1;
            if (g.i_df0 > nv) break;
        }
    }
    // 1404cc3ac: nearest edge
    if (*closed != 1) g.i_dd8 = nv - 1;
    g.i_df0 = 1;
    if (g.i_dd8 < 1) return;
    std::int32_t iend = 2;
    bool copy_p = true;
    for (;;) {
        if (copy_p) { // 1404cc3f5
            g.d_e00[0] = p[0];
            g.d_e00[1] = p[1];
        }
        const std::int32_t ib = g.i_df0; // 1404cc400
        g.i_e20 = ib;
        g.i_e24 = iend;
        if (iend > nv) g.i_e24 = 1;
        if (*useidx == 0) {
            g.i_e28 = ib;
            g.i_e2c = g.i_e24;
        } else {
            g.i_e28 = idx[ib - 1];
            g.i_e2c = idx[g.i_e24 - 1];
        }
        const double* va = col(g.i_e28);
        const double* vb = col(g.i_e2c);
        const double ex = fsub(va[0], vb[0]);
        const double ey = fsub(va[1], vb[1]);
        const double el = std::sqrt(fadd(fmul(ey, ey), fmul(ex, ex)));
        g.d_e30 = el;
        if (el < kTiny10) { // 1404cc506: degenerate edge skipped
            g.i_df0 = ib + 1;
            ++iend;
            copy_p = false;
            if (g.i_dd8 >= g.i_df0) continue;
            break;
        }
        const std::int32_t two = 2;
        fun_1404d064d(g.d_e00, va, vb, &g.d_e38, &g.d_e40, &g.d_e48, &two);
        const double ad = std::fabs(g.d_e40);
        if (!(ad > std::fabs(*dist))) { // 1404cc716: NaN accepted
            g.d_e58 = g.d_e48;
            g.d_e50 = g.d_e40;
            q[0] = g.d_e00[0];
            q[1] = g.d_e00[1];
            *iedge = ib;
            *dist = ad;
        }
        g.i_df0 = ib + 1;
        ++iend;
        copy_p = true;
        if (g.i_dd8 >= g.i_df0) continue;
        break;
    }
    // 1404cc538
    if (*iedge == 0) return;
    const double dd = std::fabs(fsub(*dist, g.d_de8));
    g.d_e30 = dd;
    if (kTiny10 > dd) *iedge = g.i_ddc;
    if (*dist == 0.0) { // 1404cc5a8
        *inside = 0;
        *iedge = iabs(*iedge);
        return;
    }
    if (g.d_de8 > std::fabs(g.d_e50) && dd > kTiny10) { // 1404cc5dc / 1404cc5f6
        *inside = 1;
        if (0.0 > g.d_e50) *inside = -1;
        *iedge = iabs(*iedge);
        return;
    }
    // 1404cc7ad: nearest point is a vertex; sign from the two adjacent edges
    const std::int32_t iv = iabs(g.i_ddc);
    g.i_e60 = iv - 1;
    *iedge = iv;
    if (iv == 1) g.i_e60 = nv;
    g.i_e64 = iv + 1;
    if (g.i_e64 > nv) g.i_e64 = 1;
    if (*useidx == 0) {
        g.i_e68 = g.i_e60;
        g.i_e6c = iv;
        g.i_e70 = g.i_e64;
    } else {
        const std::int32_t c = idx[iv - 1];
        g.i_e68 = idx[g.i_e60 - 1];
        g.i_e6c = c;
        g.i_e70 = idx[g.i_e64 - 1];
    }
    const double* vprev = col(g.i_e68);
    const double* vcur = col(g.i_e6c);
    g.d_e00[1] = p[1];
    g.d_e00[0] = p[0];
    *inside = 1;
    const std::int32_t one = 1;
    fun_1404d064d(g.d_e00, vprev, vcur, &g.d_e78, &g.d_e80, &g.d_e88, &one);
    const double* vnext = col(g.i_e70);
    fun_1404d064d(g.d_e00, vcur, vnext, &g.d_e90, &g.d_e98, &g.d_ea0, &one);
    if (0.0 > g.d_e80 && 0.0 > g.d_e98) *inside = -1; // 1404cc97e
    if (g.d_e80 > 0.0 && g.d_e98 > 0.0) return;        // 1404cc9bb
    if (*inside == -1) return;
    g.d_ed0[2] = 0.0; // 1404cca1e
    g.d_ef0[2] = 0.0;
    g.d_eb0[2] = 0.0;
    for (g.i_df0 = 1; g.i_df0 <= 2; ++g.i_df0) {
        const int i = g.i_df0 - 1;
        const double c = vcur[i];
        const double nx = fsub(vnext[i], vcur[i]);
        const double pv = fsub(vprev[i], c);
        g.d_ef0[i] = 0.0;
        g.d_eb0[i] = nx;
        g.d_ed0[i] = pv;
    }
    fun_1404cce5e(g.d_eb0, g.d_ed0, g.d_ef0, &g.d_f10);
    if (g.d_ef0[2] > 0.0) return; // 1404ccad7
    *inside = -1;
}

// @src 1404cd2e1  bilinear analytic solve
void fun_1404cd2e1(double* a1, double* a2, double* a3, double* a4, double* b1, double* b2,
                   double* b3, double* b4, double* r, std::int32_t* nroot, std::int32_t* ierr) {
    BilinSolveSave& g = g_bilin_solve;
    double* const e = g.d_000;
    double* const rt = g.d_020;
    double q;
    *nroot = 1;
    {
        const double v = *a1;
        if (std::fabs(v) > kTol9) { // 1404cd318
            const double x3 = fdiv(*a2, v);
            const double x2 = fdiv(*a3, v);
            const double x1 = fdiv(*a4, v);
            *a1 = 1.0;
            *a2 = x3;
            *a3 = x2;
            *a4 = x1;
        }
    }
    {
        const double v = *b1;
        if (std::fabs(v) >= kTol9) { // 1404cd388 (note >=)
            *b1 = 1.0;
            const double x2 = fdiv(*b3, v);
            const double x1 = fdiv(*b4, v);
            const double x3 = fdiv(*b2, v);
            *b3 = x2;
            *b4 = x1;
            *b2 = x3;
        }
    }
    if (*b1 == *a1) { // 1404cd3e4: case 0, subtract the rows
        g.i_ff8 = 0;
        e[0] = fsub(*b2, *a2);
        e[1] = fsub(*b3, *a3);
        e[2] = fsub(*b4, *a4);
        if (!(std::fabs(e[0]) <= kTol9)) goto L_cdcfd;
        q = fdiv(e[2], e[1]);
        rt[1] = -q;
        goto L_eta;
    }
    if (!(std::fabs(*a1) <= kTol9)) goto L_cd5ee;
    // case 1: row 1 is linear (1404cd4da)
    {
        const double B1 = *a2, C1 = *a3, D1 = *a4;
        e[2] = D1;
        e[1] = C1;
        g.i_ff8 = 1;
        e[0] = B1;
        if (!(std::fabs(B1) <= kTol9)) {
            if (!(std::fabs(C1) <= kTol9)) { // 1404cde75: quadratic from row 2
                g.i_040 = 0;
                const double x8 = fdiv(fmul(B1, *b1), C1);
                const double x1a = fdiv(fmul(*b1, D1), C1);
                const double x2 = fdiv(fmul(B1, *b3), C1);
                const double x4 = fdiv(fmul(*b3, D1), C1);
                g.d_048 = -x8;
                const double x1 = fadd(fsub(x1a, *b2), x2);
                g.d_058 = fsub(*b4, x4);
                g.d_050 = -x1;
                goto L_9eb;
            }
            // 1404cdd9a: s from row 1, t from row 2
            q = fdiv(D1, B1);
            rt[0] = -q;
            g.i_040 = 1;
            g.d_090 = fsub(*b3, fmul(q, *b1));
            if (g.d_090 == 0.0) g.d_090 = kTol9;
            rt[1] = -fdiv(fsub(*b4, fmul(q, *b2)), g.d_090);
            goto L_accept;
        }
        // 1404cd53b: t from row 1, s from row 2
        q = fdiv(D1, C1);
        rt[1] = -q;
        g.i_040 = 1;
        g.d_090 = fsub(*b2, fmul(q, *b1));
        if (g.d_090 == 0.0) g.d_090 = kTol9;
        rt[0] = -fdiv(fsub(*b4, fmul(q, *b3)), g.d_090);
        goto L_accept;
    }
L_cd5ee: // case 2: row 2 is linear
    {
        const double x4 = *b2, x3 = *b4, x0 = *b3;
        e[1] = x0;
        g.i_ff8 = 2;
        e[2] = x3;
        e[0] = x4;
        if (!(std::fabs(x4) <= kTol9)) {
            if (!(std::fabs(x0) <= kTol9)) goto L_quad;
            q = fdiv(x3, x4);
            rt[0] = -q;
            goto L_xi;
        }
        q = fdiv(x3, x0);
        rt[1] = -q;
        goto L_eta;
    }
L_cdcfd:
    if (!(std::fabs(e[1]) <= kTol9)) goto L_quad;
    q = fdiv(e[2], e[0]);
    rt[0] = -q;
    goto L_xi;
L_eta: // 1404cd695: t = -q given, s from row 1
    g.i_040 = 1;
    g.d_090 = fsub(*a2, fmul(q, *a1));
    if (g.d_090 == 0.0) g.d_090 = kTol9;
    rt[0] = -fdiv(fsub(*a4, fmul(q, *a3)), g.d_090);
    goto L_accept;
L_xi: // 1404cd8c3: s = -q given, t from row 1
    g.i_040 = 1;
    g.d_090 = fsub(*a3, fmul(q, *a1));
    if (g.d_090 == 0.0) g.d_090 = kTol9;
    rt[1] = -fdiv(fsub(*a4, fmul(q, *a2)), g.d_090);
    goto L_accept;
L_quad: // 1404cd93e: quadratic from row 1 and E
    {
        g.i_040 = 0;
        const double x7 = fdiv(fmul(*a1, e[0]), e[1]);
        g.d_048 = -x7;
        const double x4 = fdiv(fmul(e[0], *a3), e[1]);
        const double x2 = fdiv(fmul(*a1, e[2]), e[1]);
        const double x3 = fdiv(fmul(*a3, e[2]), e[1]);
        const double x2b = fadd(fsub(x2, *a2), x4);
        g.d_058 = fsub(*a4, x3);
        g.d_050 = -x2b;
    }
L_9eb:
    if (std::fabs(g.d_048) <= kTol9) { // 1404cda13: linear in s
        g.i_040 = 1;
        const double x2 = fdiv(g.d_058, g.d_050);
        rt[0] = -x2;
        rt[1] = -fdiv(fsub(e[2], fmul(x2, e[0])), e[1]);
        goto L_accept;
    }
    {
        const double disc = fsub(fmul(g.d_050, g.d_050), fmul(fmul(g.d_048, 4.0), g.d_058)); // 1404cdab2
        g.d_060 = disc;
        if (0.0 > disc) {
            *ierr = 1;
            return;
        }
        const double sq = std::sqrt(disc);
        g.d_068 = sq;
        const double a2x = fmul(g.d_048, 2.0);
        const double s1 = fdiv(fsub(sq, g.d_050), a2x);
        const double s2 = fdiv(-fadd(g.d_050, sq), a2x);
        g.d_078 = s2;
        g.d_070 = s1;
        const double t2 = -fdiv(fadd(fmul(e[0], s2), e[2]), e[1]);
        const double t1 = -fdiv(fadd(fmul(s1, e[0]), e[2]), e[1]);
        g.d_088 = t2;
        g.d_080 = t1;
        if (s1 >= kLo1001 && kHi1001 >= s1 && t1 >= kLo1001 && kHi1001 >= t1) { // 1404cdbe0
            rt[0] = s1;
            g.i_040 = 1;
            rt[1] = t1;
        }
        if (s2 >= kLo1001 && kHi1001 >= s2 && t2 >= kLo1001 && kHi1001 >= t2) { // 1404cdc6a
            const std::int32_t m = g.i_040 + 1;
            rt[2 * (m - 1) + 1] = t2;
            g.i_040 = m;
            rt[2 * (m - 1)] = s2;
        }
    }
L_accept: // 1404cd70b
    {
        const std::int32_t m = g.i_040;
        if (m == 0) {
            *ierr = 1;
            return;
        }
        g.i_098 = 0;
        g.i_09c = 1;
        if (m < 1) {
            *ierr = 1;
            return;
        }
        // The bounds test and the clamps read root m (the last one) for every j (listing
        // 1404cd743..1404cdff9); the value stored is root j.
        const double* rm = rt + 2 * (m - 1);
        const bool out_hi = rm[0] > kHi1001; // SETA: false for NaN
        const bool out_lo = kLo1001 > rm[0];
        for (;;) {
            bool accept = false;
            if (!out_hi && !out_lo) {
                const double t = rm[1];
                if (!(t > kHi1001)) {          // 1404cd7d8 JA
                    accept = !(kLo1001 > t); // 1404cd7ec: JP or JBE accept
                }
            }
            if (accept) {
                g.i_098 = g.i_098 + 1;
                const std::int32_t nn = g.i_098;
                const std::int32_t j = g.i_09c;
                for (g.i_0a0 = 1; g.i_0a0 <= 2; ++g.i_0a0) {
                    const int i = g.i_0a0 - 1;
                    const double v = rm[i];
                    double* dst = r + i + 2 * (nn - 1);
                    *dst = rt[i + 2 * (j - 1)];
                    if (v > 1.0) *dst = 1.0;
                    if (-1.0 > v) *dst = -1.0;
                }
            }
            g.i_09c = g.i_09c + 1;
            if (g.i_09c > g.i_040) break;
        }
        if (g.i_098 == 0) {
            *ierr = 1;
            return;
        }
        *ierr = 0;
        *nroot = g.i_098;
    }
}

// @src 1404ce05b  bilinear bisection fallback
void fun_1404ce05b(const double* x, const double* p, double* st, std::int32_t* ierr) {
    BisectSave& g = g_bisect;
    g.i_0b0 = 0;
    g.d_0a8 = kTol6;
    g.d_0c0 = 0.0;
    g.d_0b8 = 0.0;
    double sx = 0.0, sy = 0.0;
    for (g.i_0c8 = 1; g.i_0c8 <= 4; ++g.i_0c8) { // 1404ce0d9, unrolled by 2
        const int k = g.i_0c8 - 1;
        if ((k & 1) == 0) { // 1404ce0e9: load x(.,k) then add the sum
            sx = fadd(x[3 * k], sx);
            sy = fadd(x[3 * k + 1], sy);
        } else {            // 1404ce133: add x(.,k) to the sum
            sx = fadd(sx, x[3 * k]);
            sy = fadd(sy, x[3 * k + 1]);
        }
        g.d_0b8 = sx;
        g.d_0c0 = sy;
    }
    const double cx = fsub(x[0], fmul(sx, 0.25));
    g.d_0d0 = cx;
    const double cy = fsub(x[1], fmul(sy, 0.25));
    const double ex = fsub(x[3], x[0]);
    const double ey = fsub(x[4], x[1]);
    g.d_0d8 = cy;
    const double cr0 = fsub(fmul(cx, ey), fmul(cy, ex));
    g.d_0e0 = ey;
    g.i_0f0 = 1;
    g.d_0e8 = ex;
    if (0.0 > cr0) g.i_0f0 = -1; // 1404ce1cc
    for (g.i_0c8 = 1; g.i_0c8 <= 4; ++g.i_0c8)
        for (g.i_0f4 = 1; g.i_0f4 <= 3; ++g.i_0f4) {
            const int i = g.i_0f4 - 1, k = g.i_0c8 - 1;
            g.d_100[i + 3 * k] = x[i + 3 * k];
        }
    const double p1 = p[0];
    const double p2 = p[1];
    g.d_168 = 1.0;
    g.d_160 = -1.0;
    for (;;) {
        const double tm = fmul(fadd(g.d_160, g.d_168), 0.5); // 1404ce46f
        g.d_170 = tm;
        for (g.i_0c8 = 1; g.i_0c8 <= 2; ++g.i_0c8) {
            const int c = g.i_0c8 - 1;
            const double sc = kCornerS[c];
            for (g.i_0f4 = 1; g.i_0f4 <= 3; ++g.i_0f4) {
                const int i = g.i_0f4 - 1;
                g.d_178 = 0.0;
                double acc = 0.0;
                for (g.i_180 = 1; g.i_180 <= 4; ++g.i_180) {
                    const int k = g.i_180 - 1;
                    acc = fadd(fmul(fadd(fmul(sc, kCornerS[k]), 1.0), g.d_100[i + 3 * k]), acc);
                    g.d_178 = acc;
                }
                g.d_190[i + 3 * c] = fmul(acc, 0.25);
            }
        }
        double* m = g.d_190; // m(i,c) = m[i + 3c]
        if (g.i_0f0 == 1) {  // 1404ce965
            const double dx = fsub(m[3], m[0]);
            const double dy = fsub(m[4], m[1]);
            g.d_1c8 = m[1];
            g.d_1c0 = m[0];
            g.d_0e8 = dx;
            g.d_0e0 = dy;
        } else { // 1404ce5f9
            const double dy = fsub(m[1], m[4]);
            const double dx = fsub(m[0], m[3]);
            g.d_1c8 = m[4];
            g.d_1c0 = m[3];
            g.d_0e0 = dy;
            g.d_0e8 = dx;
        }
        g.d_1d0 = libm::ps_atan2(g.d_0e0, g.d_0e8); // 1404b3878
        double sn, cs;
        libm::ps_sincos(g.d_1d0, &sn, &cs);         // 1404b3b30
        g.d_1e0 = cs;
        g.d_1d8 = sn;
        const double qx = fsub(p1, g.d_1c0);
        const double qy = fsub(p2, g.d_1c8);
        const double cr = fsub(fmul(cs, qy), fmul(sn, qx));
        const double acr = std::fabs(cr);
        g.d_1e8 = cr;
        g.d_1f0 = acr;
        if (kTol6 >= acr) { // 1404ce72b: on the mid line
            *ierr = 0;
            st[1] = tm;
            const double pr = fadd(fmul(sn, qy), fmul(cs, qx));
            const double ln = fadd(fmul(g.d_0e0, sn), fmul(cs, g.d_0e8));
            g.d_200 = pr;
            g.d_1f8 = ln;
            if (pr > ln) g.d_200 = ln;
            if (0.0 > g.d_200) g.d_200 = 0.0;
            st[0] = fmul(fadd(fdiv(fmul(g.d_200, 2.0), ln), -1.0), static_cast<double>(g.i_0f0));
            return;
        }
        if (!(0.0 > g.d_1e8)) { // 1404ce34c: NaN keeps the upper half
            g.d_160 = tm;
            for (g.i_0c8 = 1; g.i_0c8 <= 2; ++g.i_0c8)
                for (g.i_0f4 = 1; g.i_0f4 <= 3; ++g.i_0f4) {
                    const int i = g.i_0f4 - 1, c = g.i_0c8 - 1;
                    g.d_100[i + 3 * c] = m[i + 3 * c];
                }
        } else { // 1404ce85f
            g.d_168 = tm;
            for (g.i_0f4 = 1; g.i_0f4 <= 3; ++g.i_0f4) {
                const int i = g.i_0f4 - 1;
                const double m1 = m[i];
                const double m2 = m[i + 3];
                g.d_100[i + 9] = m1;
                g.d_100[i + 6] = m2;
            }
        }
        g.i_0b0 = g.i_0b0 + 1;
        if (g.i_0b0 >= kBisectMax) {
            *ierr = 1;
            return;
        }
    }
}

// @src 1404ccf58  bilinear inverse map driver
void fun_1404ccf58(const double* x, const double* p, double* st, std::int32_t* nroot,
                   std::int32_t* ierr) {
    BilinSave& g = g_bilin;
    *ierr = 0;
    {
        const double x1 = x[0], x2 = x[3], x3 = x[6], x4 = x[9];
        const double sp = fadd(x2, x1);
        const double dm = fsub(x1, x2);
        const double c_t = fmul(fsub(fsub(sp, x3), x4), -0.25);
        const double c0 = fmul(fadd(fadd(sp, x3), x4), 0.25);
        const double c_st = fmul(fsub(fadd(x3, dm), x4), 0.25);
        const double c_s = fmul(fadd(fsub(dm, x3), x4), -0.25);
        g.d_f70[2] = c_t;
        g.d_f70[0] = c_st;
        g.d_f70[1] = c_s;
        g.d_f70[3] = fsub(c0, p[0]);
    }
    {
        const double y1 = x[1], y2 = x[4], y3 = x[7], y4 = x[10];
        const double sp = fadd(y2, y1);
        const double dm = fsub(y1, y2);
        const double c0 = fmul(fadd(fadd(sp, y3), y4), 0.25);
        const double c_t = fmul(fsub(fsub(sp, y3), y4), -0.25);
        const double c_st = fmul(0.25, fsub(fadd(dm, y3), y4));
        const double c_s = fmul(-0.25, fadd(fsub(dm, y3), y4));
        g.d_f90[2] = c_t;
        g.d_f90[0] = c_st;
        g.d_f90[1] = c_s;
        g.i_fb0 = 0;
        g.d_f90[3] = fsub(c0, p[1]);
    }
    if (std::fabs(g.d_f70[0]) > kTol5 && std::fabs(g.d_f90[0]) > kTol5) g.i_fb0 = 1; // 1404cd104
    fun_1404cd2e1(&g.d_f70[0], &g.d_f70[1], &g.d_f70[2], &g.d_f70[3], &g.d_f90[0], &g.d_f90[1],
                  &g.d_f90[2], &g.d_f90[3], st, nroot, ierr);
    if (*ierr != 1) {
        if (g.i_fb0 == 1) return;
        fun_1404d6974(x, g.d_fc0, &st[0], &st[1]);
        const double dx = fsub(p[0], g.d_fc0[0]);
        g.d_fe0 = dx;
        const double dy = fsub(p[1], g.d_fc0[1]);
        const double r2 = fadd(fmul(dy, dy), fmul(dx, dx));
        g.d_fe8 = dy;
        g.d_ff0 = r2;
        if (kTol6 > r2 && *nroot == 1) return; // 1404cd26d
    }
    *nroot = 1; // 1404cd297
    fun_1404ce05b(x, p, st, ierr);
}

// @src 1404d6647  line search of 1404d5a1d
void fun_1404d6647(const double* x, const double* p, const double* s, const double* t,
                   const double* ds, const double* dt, double* step) {
    LineSearchSave& g = g_linesearch;
    g.d_3a0 = *t;
    g.d_398 = *s;
    *step = 1.0;
    fun_1404d6974(x, g.d_3b0, &g.d_398, &g.d_3a0);
    fun_1404d68dd(g.d_3b0, p, g.d_3d0, &g.d_3f0);
    for (;;) {
        const double hs = fmul(*ds, *step); // 1404d6764
        g.d_3f8 = fadd(fmul(hs, 0.5), *s);
        const double ht = fmul(*step, *dt);
        g.d_400 = fadd(fmul(ht, 0.5), *t);
        fun_1404d6974(x, g.d_3b0, &g.d_3f8, &g.d_400);
        fun_1404d68dd(g.d_3b0, p, g.d_3d0, &g.d_408);
        g.d_410 = fadd(hs, *s);
        g.d_418 = fadd(ht, *t);
        fun_1404d6974(x, g.d_3b0, &g.d_410, &g.d_418);
        fun_1404d68dd(g.d_3b0, p, g.d_3d0, &g.d_420);
        if (g.d_408 > g.d_420 && g.d_3f0 > g.d_420) return; // 1404d6881
        const double h = fmul(*step, 0.5);                 // 1404d66f1
        *step = h;
        if (!(g.d_408 > g.d_3f0)) return; // 1404d6727
        if (kStepMin > h) return;         // 1404d6745: NaN continues
    }
}

// @src 1404d6b13  PNTLIN: point to segment (3D)
void fun_1404d6b13(const double* a, const double* b, const double* p, double* proj, double* t,
                   double* dist) {
    PntlinSave& g = g_pntlin;
    const double a1 = a[0], a2 = a[1], a3 = a[2];
    const double p1 = p[0], p2 = p[1], p3 = p[2];
    const double dx = fsub(b[0], a1);
    const double dy = fsub(b[1], a2);
    const double dz = fsub(b[2], a3);
    const double l2 = fadd(fmul(dz, dz), fadd(fmul(dy, dy), fmul(dx, dx)));
    g.d_480[0] = fsub(p1, a1);
    g.d_480[1] = fsub(p2, a2);
    g.d_460[2] = dz;
    g.d_460[0] = dx;
    g.d_460[1] = dy;
    g.d_480[2] = fsub(p3, a3);
    if (l2 == 0.0) { // 1404d6be0
        // 1404d6dae: WRITE(6,1001) a(1), a(2), a(3) with
        // 1001 FORMAT(' ERROR IN PNTLIN ',/,3F20.7) (str 140643c30), Fortran runtime 1404b5ef4
        def::rt::ftn::fio::Write(6, "(' ERROR IN PNTLIN ',/,3F20.7)").d(a[0]).d(a[1]).d(a[2]).end();
        const std::int32_t code = -1;
        fun_1404edb2f(&code);
    }
    // 1404d6c2b
    const double x4 = fmul(g.d_460[0], g.d_480[0]);
    const double x2 = fmul(g.d_480[1], g.d_460[1]);
    const double x1 = fmul(g.d_480[2], g.d_460[2]);
    *t = fdiv(fadd(x1, fadd(x2, x4)), l2);
    if (0.0 > *t) *t = 0.0;
    if (*t > 1.0) *t = 1.0;
    const double tt = *t;
    const double q1 = fadd(fmul(tt, g.d_460[0]), a1);
    proj[0] = q1;
    const double q2 = fadd(fmul(tt, g.d_460[1]), a2);
    proj[1] = q2;
    const double q3 = fadd(fmul(tt, g.d_460[2]), a3);
    const double ey = fsub(q2, p2);
    const double ex = fsub(q1, p1);
    const double ez = fsub(q3, p3);
    const double d = std::sqrt(fadd(fmul(ez, ez), fadd(fmul(ey, ey), fmul(ex, ex))));
    g.d_480[0] = ex;
    g.d_480[1] = ey;
    proj[2] = q3;
    g.d_480[2] = ez;
    *dist = d;
}

// @src 1404d5a1d  Newton projection (s,t) on a quadratic patch, line search 1404d6647
// Residuals: R1 = c1(1) s^2 t + c1(2) s^2 + c1(3) s t + c1(4) t + c1(5) s + c1(6),
//            R2 = c2(1) s t^2 + c2(2) t^2 + c2(3) s t + c2(4) t + c2(5) s + c2(6).
void fun_1404d5a1d(const double* c1, const double* c2, double* s, double* t, const double* x,
                   const double* p, std::int32_t* ierr) {
    NewtonSave& g = g_newton;
    g.d_298 = 0.0;
    g.i_280 = 0;
    g.d_290 = 0.0;
    g.d_288 = 1.0;
    *ierr = 0;
    for (;;) {
        // 1404d5aaf
        const double S = fadd(fmul(g.d_288, g.d_290), *s);
        *s = S;
        const double T = fadd(fmul(g.d_288, g.d_298), *t);
        g.d_288 = 1.0;
        *t = T;
        // Jacobian (1404d5afb..1404d5c11)
        const double j11 = fadd(fadd(fmul(fmul(T, c2[0]), T), fmul(T, c2[2])), c2[4]); // 2a0
        const double j12 = fadd(fadd(fmul(c2[2], S),
                                     fadd(fmul(fmul(c2[1], 2.0), T), fmul(fmul(fmul(c2[0], 2.0), S), T))),
                                c2[3]); // 2b0
        const double j22 = fadd(fadd(fmul(c1[2], S), fmul(fmul(c1[0], S), S)), c1[3]); // 2b8
        const double j21 = fadd(fadd(fadd(fmul(fmul(fmul(c1[0], 2.0), S), T), fmul(fmul(2.0, c1[1]), S)),
                                     fmul(T, c1[2])),
                                c1[4]); // 2a8
        g.d_2a0 = j11;
        g.d_2b0 = j12;
        g.d_2b8 = j22;
        g.d_2a8 = j21;
        const double det = fsub(fmul(j11, j22), fmul(j12, j21));
        g.d_2c0 = det;
        if (det == 0.0) { // 1404d5c1a
            const std::int32_t code = -1;
            fun_1404edb2f(&code);
        }
        // 1404d5c52: residuals at the stored (s, t)
        const double s0 = *s;
        const double t0 = *t;
        const double i11 = fdiv(j11, det);
        const double i21 = fdiv(j21, det);
        const double i12 = fdiv(j12, det);
        const double i22 = fdiv(j22, det);
        const double r2 = fadd(
            fadd(fmul(c2[4], s0),
                 fadd(fmul(c2[3], t0),
                      fadd(fmul(fmul(T, c2[2]), s0),
                           fadd(fmul(fmul(fmul(c2[0], s0), t0), t0), fmul(fmul(c2[1], t0), t0))))),
            c2[5]);
        const double r1 = fadd(
            fadd(fmul(c1[4], s0),
                 fadd(fmul(c1[3], t0),
                      fadd(fmul(fmul(T, c1[2]), s0),
                           fadd(fmul(fmul(fmul(c1[0], S), S), t0), fmul(fmul(c1[1], s0), s0))))),
            c1[5]);
        g.d_2d0 = i22;
        g.d_2e0 = -i12;
        g.d_2d8 = -i21;
        g.d_2e8 = i11;
        const double m2 = -r2;
        const double m1 = -r1;
        g.d_2f0 = m2;
        g.d_2f8 = m1;
        g.d_290 = fsub(fmul(i22, m2), fmul(i12, m1));
        g.d_298 = fsub(fmul(i11, m1), fmul(i21, m2));
        fun_1404d6647(x, p, s, t, &g.d_290, &g.d_298, &g.d_288);
        const double nrm = std::sqrt(fadd(fmul(g.d_298, g.d_298), fmul(g.d_290, g.d_290)));
        g.i_280 = g.i_280 + 1;
        g.d_300 = nrm;
        if (kNewtonTol > nrm) return; // 1404d5ed4
        if (g.i_280 > 30 || kStepMin > g.d_288) break;
    }
    *ierr = 1;
}

} // namespace def::rt::geoutl
