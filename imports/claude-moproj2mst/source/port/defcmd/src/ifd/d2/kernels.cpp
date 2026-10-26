// defcmd/src/ifd/d2/kernels.cpp: the 2D gap kernels of the interference positioning (Fortran,
// PathScale -O2): segment 14030bba0, arc 1403407d7 with its span / angle helpers and the arc
// contact refinement 140341100 / 14034151e. Spec: OBJPOS_interference_kernels.md 4.3-4.5.
// Operation order, operand order (first operand = SSE destination, def::rt::sse) and the NaN
// behaviour of every compare are taken from the listings (deform-rea-work/out/p672/all_lst.txt).
#include "d2.hpp"

#include <bit>
#include <cstdint>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ifd::d2 {

namespace {

using rt::sse::add;
using rt::sse::div;
using rt::sse::mul;
using rt::sse::sub;
using rt::sse::sqrt;

// fabs by and-mask, negation by sign-bit xor (NaN payloads kept)
double fab(double x) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffffull); }
double neg(double x) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) ^ 0x8000000000000000ull); }

// @data 1405f1eb8  1e-10 (segment tolerance tolE); 140bae858 the same value (min. length)
constexpr double kTolE = 1e-10;
constexpr double kMinLen = 1e-10;
// @data 1405f6238  pi as written in the Fortran source (1405f3be0, 1405f6298 the same)
constexpr double kPi = 3.1415926535898;
// @data 1405f3bd8  0.001 arc angle tolerance
constexpr double kAngTol = 0.001;
// @data 1405f6290  1e-4 (14034151e)
constexpr double kTol4 = 1e-4;

} // namespace

// @src 14030bba0  swept segment against a point along dir (spec 4.3, listing 14030bba0..c866)
void fun_14030bba0(double* seg, double* vel, double* q, double* dir, double* gap, std::int32_t* flag)
{
    // 1406ff2f8 = ext * GEOERR1 is stored but not read
    double tolg = g_tol_override; // 1406ff300
    if (g_tol_override == 0.0) tolg = mul(g_ext, g_geoerr2);
    *flag = 0;
    double ax = sub(vel[0], dir[0]);
    double bx = sub(vel[2], dir[0]);
    double ay = sub(vel[1], dir[1]);
    double by = sub(vel[3], dir[1]);
    const double la = sqrt(add(mul(ay, ay), mul(ax, ax)));
    const double lb = sqrt(add(mul(by, by), mul(bx, bx)));
    if (kTolE >= la && kTolE >= lb) return;
    if (!(kTolE < la)) {
        ay = by;
        ax = bx;
    }
    if (!(kTolE < lb)) {
        by = ay;
        bx = ax;
    }
    const double p1x = seg[0], p1y = seg[1], p2x = seg[2], p2y = seg[3];
    const double ex = mul(sub(p1x, p2x), kTolE);
    const double ey = mul(sub(p1y, p2y), kTolE);
    const double q2x = sub(p2x, ex);
    const double q1x = add(ex, p1x);
    const double q2y = sub(p2y, ey);
    const double q1y = add(ey, p1y);
    const double qx = q[0], qy = q[1];
    double t14 = mul(sub(q2y, q1y), ax);
    double t3 = mul(sub(q2x, q1x), ay);
    double t2 = mul(sub(qy, q1y), ax);
    double t15 = mul(sub(qx, q1x), ay);
    t14 = sub(t14, t3);
    t2 = sub(t2, t15);
    const double c1 = mul(t2, t14);
    double t4 = mul(sub(q1y, q2y), bx);
    double t11 = mul(sub(q1x, q2x), by);
    double t1 = mul(sub(qy, q2y), bx);
    double t10 = mul(sub(qx, q2x), by);
    t4 = sub(t4, t11);
    t1 = sub(t1, t10);
    const double c2 = mul(t1, t4);
    if (c1 < 0.0) return;
    if (0.0 > c2) return;
    // 14030bfb3: A and B are recomputed from vel - dir (the replacements above are dropped)
    ax = sub(vel[0], dir[0]);
    bx = sub(vel[2], dir[0]);
    ay = sub(vel[1], dir[1]);
    by = sub(vel[3], dir[1]);
    const double dpy = sub(qy, p1y);
    const double dpx = sub(qx, p1x);
    const double sx = sub(p2x, p1x);
    const double sy = sub(p2y, p1y);
    double b = sub(mul(sub(by, ay), dpx), mul(sub(bx, ax), dpy));
    const double lp = sqrt(add(mul(dpx, dpx), mul(dpy, dpy)));
    const double ls = sqrt(add(mul(sy, sy), mul(sx, sx)));
    const double a = sub(mul(bx, ay), mul(ax, by));
    b = sub(b, mul(ax, sy));
    b = add(b, mul(ay, sx));
    const double c = sub(mul(sy, dpx), mul(sx, dpy));
    double d0;
    if (kMinLen > ls) {
        d0 = lp;
    } else {
        d0 = div(c, ls);
    }
    if (neg(tolg) > d0) return;
    const double ad = fab(d0);
    if (!(kTolE < ad)) {
        // Q on the segment's line
        if (kTolE > fab(a) && kTolE > fab(b)) return;
        const double t = div(lp, ls);
        const double om = sub(1.0, t);
        const double vx = add(mul(t, bx), mul(om, ax));
        const double vy = add(mul(t, by), mul(om, ay));
        const double w = sub(mul(vy, dpx), mul(vx, dpy));
        if (!(w >= 0.0)) {
            *gap = 0.0;
            *flag = 1;
        }
        return;
    }
    if (!(kTolE < fab(a))) {
        // linear
        if (kTolE >= fab(b)) {
            *flag = -1;
            return;
        }
        if (d0 > 0.0) {
            const double t = neg(div(c, b));
            if (!(0.0 >= t)) {
                *flag = 1;
                *gap = t;
            }
        } else {
            if (ad > tolg) return;
            const double t = neg(div(c, b));
            if (!(t >= 0.0)) {
                *flag = 1;
                *gap = 0.0;
            }
        }
        return;
    }
    // quadratic (moving segment)
    const double disc = sub(mul(b, b), mul(mul(a, 4.0), c));
    if (disc < 0.0) {
        *flag = -1;
        return;
    }
    const double sq = sqrt(disc);
    const double r1 = mul(div(sub(sq, b), a), 0.5);
    const double r2 = mul(div(neg(add(sq, b)), a), 0.5);
    const double n1x = add(mul(r2, ax), p1x);
    const double n1y = add(mul(r2, ay), p1y);
    const double n2x = add(mul(r2, bx), p2x);
    const double n2y = add(mul(r2, by), p2y);
    double u14 = sub(n2x, n1x);
    double u3 = sub(n2y, n1y);
    double u13 = sub(qy, n1y);
    u14 = mul(u14, u14);
    double u2 = sub(qx, n1x);
    u3 = mul(u3, u3);
    double u1 = sub(qx, n2x);
    u14 = add(u14, u3);
    double u3b = sub(qy, n2y);
    u13 = mul(u13, u13);
    u2 = mul(u2, u2);
    u1 = mul(u1, u1);
    u2 = add(u2, u13);
    u3b = mul(u3b, u3b);
    const double l12 = sqrt(u14);
    const double l1 = sqrt(u2);
    u1 = add(u1, u3b);
    const double l2 = sqrt(u1);
    const double e = fab(sub(sub(l12, l1), l2));
    const double t = !(kTolE >= e) ? r1 : r2;
    if (d0 > 0.0) {
        if (0.0 > t) return;
        *flag = 1;
        *gap = t;
        return;
    }
    if (!(tolg > ad)) {
        *flag = -1;
        return;
    }
    if (t >= 0.0) return;
    *flag = 1;
    *gap = t;
}

// @src 14032beef  signed angle from a to b: ATAN2F of the normalised cross and dot products
void fun_14032beef(double* a, double* b, double* ang)
{
    const double ax = a[0], ay = a[1], bx = b[0], by = b[1];
    const double la = sqrt(add(mul(ay, ay), mul(ax, ax)));
    const double lb = sqrt(add(mul(by, by), mul(bx, bx)));
    // @data 140baeb58  1e-10
    if (la > 1e-10 && lb > 1e-10) {
        const double y = div(div(sub(mul(ax, by), mul(ay, bx)), la), lb);
        const double x = div(div(add(mul(by, ay), mul(bx, ax)), la), lb);
        *ang = rt::libm::ps_atan2(y, x); // 1404b3878
        return;
    }
    *ang = 0.0;
}

// @src 14032bd2a  x lies on the arc: same radius within ext * GEOERR2, angle within the sweep
void fun_14032bd2a(double* s, double* c, double* phi, double* x, std::int32_t* in)
{
    *in = 0;
    double a[2] = {sub(s[0], c[0]), sub(s[1], c[1])};
    double bb[2] = {sub(x[0], c[0]), sub(x[1], c[1])};
    const double la = sqrt(add(mul(a[1], a[1]), mul(a[0], a[0])));
    const double lb = sqrt(add(mul(bb[1], bb[1]), mul(bb[0], bb[0])));
    if (fab(sub(lb, la)) > mul(g_ext, g_geoerr2)) return;
    double al = 0.0;
    fun_14032beef(a, bb, &al);
    double ph = *phi;
    if (0.0 > ph) {
        al = neg(al);
        ph = neg(ph);
    }
    const double lo = neg(kAngTol);
    if (al >= lo && al <= add(kAngTol, ph)) {
        *in = 1;
        return;
    }
    // @data 140baeb30  2.0
    const double a2 = add(mul(kPi, 2.0), al);
    if (a2 >= lo && add(ph, kAngTol) >= a2) *in = 1;
}

// @src 1403407d7  arc kernel (spec 4.4, listing 1403407d7..1403410ff)
void fun_1403407d7(double* s, double* c, double* phi, double* q, double* dir, double* gap,
                   double* tola, std::int32_t* flag)
{
    // 1407023a8 = ext * GEOERR2 and 1407023b8 / 1407023f8 (sign of phi, (Lp - R) * sign) are
    // stored but not read.
    double g1 = *gap; // 140702398
    double g2 = *gap; // 1407023a0
    const double cx = c[0], cy = c[1];
    const double dpx = sub(q[0], cx);
    const double dpy = sub(q[1], cy);
    const double lp = sqrt(add(mul(dpy, dpy), mul(dpx, dpx)));
    const double ld = sqrt(add(mul(dir[1], dir[1]), mul(dir[0], dir[0])));
    const double sxc = sub(s[0], cx);
    const double syc = sub(s[1], cy);
    const double r = sqrt(add(mul(syc, syc), mul(sxc, sxc)));
    double ux = div(dpx, lp);  // 140702400
    double uy = div(dpy, lp);  // 140702408
    const double dhx = div(dir[0], ld); // 1407023d0
    const double dhy = div(dir[1], ld); // 1407023d8
    // @data 140baec88  0.5
    double half_pi = mul(kPi, 0.5);
    double n[2] = {dhx, dhy}; // 140702410
    rt::geoutl::fun_1404d71d7(n, &half_pi);
    double qd = add(mul(n[1], dpy), mul(n[0], dpx)); // 140702420
    if (fab(qd) > r) return;
    if (!(qd > 0.0)) {
        qd = neg(qd);
        n[0] = neg(n[0]);
        n[1] = neg(n[1]);
    }
    const double fx = add(mul(n[0], qd), cx);
    const double fy = add(mul(qd, n[1]), cy);
    const double h = sqrt(sub(mul(r, r), mul(qd, qd)));
    const double hx = mul(h, dhx);
    const double hy = mul(h, dhy);
    double x[2] = {add(hx, fx), add(hy, fy)}; // 140702450
    std::int32_t in = 0;                      // 140702460
    fun_14032bd2a(s, c, phi, x, &in);
    if (in != 0) {
        const double ex = sub(x[0], q[0]);
        const double ey = sub(x[1], q[1]);
        const double l = sqrt(add(mul(ey, ey), mul(ex, ex)));
        const double sd = add(mul(ey, dhy), mul(ex, dhx));
        const double v = fab(div(l, ld));
        g1 = !(sd < 0.0) ? v : neg(v);
    }
    x[1] = sub(fy, hy);
    x[0] = sub(fx, hx);
    fun_14032bd2a(s, c, phi, x, &in);
    if (in != 0) {
        const double ex = sub(x[0], q[0]);
        const double ey = sub(x[1], q[1]);
        const double l = sqrt(add(mul(ey, ey), mul(ex, ex)));
        const double sd = add(mul(ey, dhy), mul(ex, dhx));
        const double v = fab(div(l, ld));
        g2 = !(sd < 0.0) ? v : neg(v);
    }
    double g = g1; // 140702478
    if (fab(g1) > fab(g2)) g = g2;
    double ag = fab(g);
    // @data 140baeca0  1e-7
    if (!(ag > 1e-7)) {
        if (*phi > 0.0) {
            ux = neg(ux);
            uy = neg(uy);
        }
        const double dot = add(mul(dhy, uy), mul(dhx, ux)); // 140702480
        // @data 140baeca8  1e-5
        if (1e-5 >= dot) return;
        double tmp[2] = {0.0, 0.0};   // 1407023e0 / 1407023e8
        double zeros[2] = {0.0, 0.0}; // 1405f6240
        g = *gap;
        std::int32_t f2 = 0;          // 140702488
        fun_140341100(s, tmp, c, phi, zeros, q, dir, tola, &g, &f2);
        ag = fab(g);
    }
    if (fab(*gap) > ag) {
        *gap = g;
        *flag = 1;
    }
}

// @src 140341100  arc contact refinement driver: end point E, S / E / C in the frame of dir
// relative to Q, ordered by the coordinate along dir, then 14034151e
void fun_140341100(double* s, double* tmp, double* c, double* phi, double* vel, double* q,
                   double* dir, double* tola, double* gap, std::int32_t* flag)
{
    const double dx = sub(dir[0], vel[0]); // 140702490
    const double dy = sub(dir[1], vel[1]); // 140702498
    double len = sqrt(add(mul(dy, dy), mul(dx, dx))); // 1407024a0
    tmp[0] = sub(s[0], c[0]);
    tmp[1] = sub(s[1], c[1]);
    double r = sqrt(add(mul(tmp[1], tmp[1]), mul(tmp[0], tmp[0]))); // 1407024a8
    const double ey = div(dy, len);
    const double ex = div(dx, len);
    rt::geoutl::fun_1404d71d7(tmp, phi);
    tmp[0] = add(tmp[0], c[0]);
    tmp[1] = add(tmp[1], c[1]);
    std::int32_t order = 1; // 1407024f0
    const double qx = q[0], qy = q[1];
    const double sqx = sub(s[0], qx);
    const double eqy = sub(tmp[1], qy);
    const double eqx = sub(tmp[0], qx);
    const double sqy = sub(s[1], qy);
    const double cqy = sub(c[1], qy);
    const double cqx = sub(c[0], qx);
    double p1[2]; // 1407024d0
    double p2[2]; // 1407024e0
    double cc[2]; // 1407024c0
    p1[0] = add(mul(sqy, ey), mul(sqx, ex));
    p1[1] = sub(mul(sqy, ex), mul(sqx, ey));
    p2[0] = add(mul(eqy, ey), mul(eqx, ex));
    cc[1] = sub(mul(cqy, ex), mul(cqx, ey));
    p2[1] = sub(mul(ex, eqy), mul(ey, eqx));
    cc[0] = add(mul(cqy, ey), mul(cqx, ex));
    double ph = *phi; // 1407024f8
    if (p1[0] > p2[0]) {
        const double s0 = p1[0], s1 = p1[1];
        p1[0] = p2[0];
        p1[1] = p2[1];
        p2[0] = s0;
        p2[1] = s1;
        order = 2;
        ph = neg(ph);
    }
    std::int32_t st = 0; // 140702510
    fun_14034151e(p1, p2, cc, &r, tola, &ph, gap, &len, &st);
    if (st == 0) {
        *flag = -1;
    } else if (st > 0) {
        *flag = 0;
    } else {
        *flag = order != 1 ? 1 : 2;
    }
}

// @src 14034151e  ray / arc contact in angle space (listing 14034151e..1403426fd). The candidate
// table T(2,12) (140702540) holds an angle in row 1 and a 0/1 mark in row 2; the compaction loop
// of the original addresses it as T(j,1) / T(j,2) (flat elements j-1 and j+1), reproduced here.
void fun_14034151e(double* p1, double* /*p2*/, double* cc, double* r, double* tola, double* phi,
                   double* gap, double* len, std::int32_t* status)
{
    double t[24]; // T(2,12) column-major, t[2(k-1)] = T(1,k), t[2(k-1)+1] = T(2,k)
    auto t1 = [&t](std::int32_t k) -> double& { return t[2 * (k - 1)]; };
    auto t2 = [&t](std::int32_t k) -> double& { return t[2 * (k - 1) + 1]; };
    const double ta = *tola;
    std::int32_t mode = 1; // 140702520
    *status = 0;
    if (*r == ta) mode = 2;
    const double cy = cc[1];
    if (cy == 0.0) {
        const double sc = cc[0];
        if (ta >= *r) {
            *status = 1;
            *gap = div(sc, *len);
            return;
        }
        const double s1 = p1[0];
        if (sc > s1 && p1[1] == 0.0) {
            *status = 1;
            *gap = div(add(ta, s1), *len);
            return;
        }
        if (s1 > sc && p1[1] == 0.0) {
            *status = 1;
            *gap = div(sub(s1, ta), *len);
            return;
        }
    }
    for (double& v : t) v = 0.0;
    const double two_pi = mul(kPi, 2.0); // 140baecb0 = 2.0
    if (mode != 2) {
        double x = div(cy, sub(ta, *r)); // 140702600
        const double ax = fab(x);
        const double d = fab(sub(1.0, ax));
        if (kTol4 > d || 1.0 >= ax) {
            if (-1.0 > x) x = -1.0;
            if (x > 1.0) x = 1.0;
            const double a = rt::libm::ps_asin(x); // 1404b3d50
            t1(1) = a;
            t1(2) = sub(kPi, a);
            if (0.0 > t1(1)) t1(1) = add(mul(kPi, 2.0), t1(1));
            *status = 2;
        }
    }
    if (cy != 0.0 || cy != cy) {
        double x = div(cy, neg(add(ta, *r)));
        const double ax = fab(x);
        const double d = fab(sub(1.0, ax));
        if (kTol4 > d || 1.0 >= ax) {
            if (-1.0 > x) x = -1.0;
            if (x > 1.0) x = 1.0;
            const std::int32_t k = *status;
            const double a = rt::libm::ps_asin(x);
            t1(k + 1) = a;
            t1(k + 2) = sub(kPi, a);
            if (0.0 > t1(k + 1)) t1(k + 1) = add(mul(kPi, 2.0), t1(k + 1));
            *status = k + 2;
        }
        if (*status == 0) return;
    }
    std::int32_t one = 1, one2 = 1, ld = 2;
    rt::geoutl::fun_1404d93ae(t, &one, status, &one2, &ld);
    const std::int32_t n = *status;
    for (std::int32_t i = 1; i <= n; ++i) {
        t1(n + i) = sub(t1(i), two_pi);
        t2(n + i) = t2(i);
    }
    for (std::int32_t i = 1; i <= n; ++i) {
        t1(2 * n + i) = add(two_pi, t1(i));
        t2(2 * n + i) = t2(i);
    }
    std::int32_t m = n * 3; // 140702610
    one = 1;
    one2 = 1;
    ld = 2;
    rt::geoutl::fun_1404d93ae(t, &one, &m, &one2, &ld);
    std::int32_t np = m; // 140702630
    if (m - 1 >= 1) {
        for (std::int32_t i = 1; i <= m - 1; ++i) {
            const double mid = mul(add(t1(i), t1(i + 1)), 0.5); // 140702618
            const double v = add(div(cy, rt::libm::ps_sin(mid)), *r); // 140702620
            if (ta > fab(v)) t2(i) = 1.0;
        }
        for (std::int32_t i = 1; i <= m - 1; ++i) {
            if (i >= np) break;
            if (t2(i) == 1.0 && t2(i + 1) == 1.0) {
                np = np - 1;
                if (np >= i + 1) {
                    std::int32_t j = i + 1;
                    std::int32_t s = i + 2;
                    do {
                        t[j - 1] = t[s - 1]; // flat T(j,1) = T(j+1,1)
                        t[j + 1] = t[s + 1]; // flat T(j,2) = T(j+1,2)
                        ++j;
                        ++s;
                    } while (j <= np);
                }
            }
        }
    }
    const double sc = cc[0];
    double a1[2] = {1.0, 0.0};          // 140702650
    double b1[2] = {neg(sc), neg(cy)};  // 140702640
    double ang_c = 0.0;                 // 140702660
    fun_14032beef(a1, b1, &ang_c);
    if (0.0 > ang_c) ang_c = add(mul(kPi, 2.0), ang_c);
    a1[0] = 1.0;
    a1[1] = 0.0;
    b1[0] = sub(p1[0], sc);
    b1[1] = sub(p1[1], cy);
    double ang_s = 0.0; // 140702668
    fun_14032beef(a1, b1, &ang_s);
    if (0.0 > ang_s) ang_s = add(mul(kPi, 2.0), ang_s);
    (void)add(div(cy, rt::libm::ps_sin(ang_s)), *r); // 140702620, not read
    std::int32_t col = 0;
    bool found = false;
    for (std::int32_t i = 1; i <= np; ++i) {
        if (fab(sub(t1(i), ang_c)) <= kTol4) { // 140702670; NaN: not found
            col = i;
            found = true;
            break;
        }
    }
    double ang;
    if (!found && np - 1 < 1) {
        // single interval
        if (t2(1) == 0.0) {
            *status = 0;
            return;
        }
        std::int32_t k = 2;
        if (0.0 > *phi) k = 1;
        const double v = add(*phi, ang_s);
        if (v > t1(1) && t1(2) > v) {
            ang = v;
            *status = -1;
        } else {
            ang = t1(k);
            *status = 1;
        }
    } else {
        if (!found) {
            col = np;
            for (std::int32_t i = 1; i <= np - 1; ++i) {
                if (ang_c > t1(i) && t1(i + 1) > ang_c) {
                    col = i;
                    break;
                }
            }
        }
        if (t2(col) == 0.0) {
            *status = 0;
            return;
        }
        std::int32_t k = col + 1; // 140702678
        if (0.0 > *phi) k = col;
        const double v = add(*phi, ang_s); // 140702680
        if (v > t1(col) && t1(col + 1) > v) {
            ang = v;
            *status = -1;
        } else {
            ang = t1(k);
            *status = 1;
        }
    }
    double sn = 0.0, cs = 0.0;
    rt::libm::ps_sincos(ang, &sn, &cs); // 1404b3b30
    const double v = fab(div(cy, sn));  // 140702690
    const double g = add(mul(v, cs), sc); // 140702528
    *gap = div(g, *len);
    if (0.0 > *gap) *status = 0;
}

} // namespace def::cmd::ifd::d2
