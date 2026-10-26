// GENCTC 3D engine: ray query 14001f54f and its ray-triangle / ray-quad kernels with their
// fall-backs. Specs: docs/re/commands/OBJPOS_drop_solver.md §5.6, GENCTC_3d_search.md §10.2,
// OBJPOS_drop_ray.md. Operation order from the listings (out/p67/drop-solver/L_contact.txt,
// L_64f58.txt, out/p673/L_extra.txt, out/p673/b/L_b.txt), not from the decompile.
#include <defrt/ftn/geoutl.hpp>
#include <defcmd/genctc3d_engine.hpp>
#include <defcmd/genctc3d_ray.hpp>
#include <defrt/ftn/geoutl.hpp>

#include <cmath>
#include <cstdint>
#include <vector>

namespace def::cmd::ctc3d {

namespace geo = def::rt::geoutl;

namespace {

// DAT_140bad130 = 1e-10f, DAT_140bad1f8 = 1e-4f (single-precision literals widened to double)
constexpr double kTiny10f = 1.000000013351432e-10;
constexpr double kPad4f = 9.999999747378752e-05;
// DAT_140bad3c0 = 1e-6f
constexpr double kTiny6f = 9.999999974752427e-07;
// DAT_140bad800 = 0.9f
constexpr double kNineTenthsF = 0.8999999761581421;

// part of @src 14007ff21 / 14007f8c4 (inlined there): mode 1..3 zeroes component mode, 4 zeroes
// 1 and 2, 5 zeroes 2 and 3, 6 zeroes 1 and 3; then n = n * (1.0 / sqrt(sum)), the sum built
// as s = (n1*n1) + 0.0; s = s + (n2*n2); s = (n3*n3) + s (listing 14007ffe2..140080159)
void zero_and_normalize(double* n, i32 mode)
{
    if (!(0 < mode && mode < 7)) return;
    if (mode < 4) {
        n[mode - 1] = 0.0;
    } else if (mode == 4) {
        n[1] = 0.0;
        n[0] = 0.0;
    } else if (mode == 5) {
        n[2] = 0.0;
        n[1] = 0.0;
    } else {
        n[0] = 0.0;
        n[2] = 0.0;
    }
    double s = 0.0;
    s = (n[0] * n[0]) + s;
    s = s + (n[1] * n[1]);
    s = (n[2] * n[2]) + s;
    const double r = 1.0 / std::sqrt(s);
    n[0] = n[0] * r;
    n[1] = n[1] * r;
    n[2] = n[2] * r;
}

} // namespace

// @src 14001f54f  ray query against the polygons of one cube (listing L_contact.txt)
void fun_14001f54f(i32* obj, i32* inst, double* p, double* d, double* q, double* len,
                   double* frac, double* uv, i32* face, i32* p10)
{
    (void)p10; // [RSP+0x128] is never read
    *face = 0;
    double pts[6];     // DAT_1406493b0: P, E = P + d*L
    double lsq = 0.0;  // DAT_140649390
    double dd = 0.0;   // DAT_140649398
    for (int k = 0; k < 3; ++k) {
        const double dk = d[k];
        const double e = dk * *len; // DAT_1406493e0
        pts[k] = p[k];
        lsq = (e * e) + lsq;
        pts[3 + k] = p[k] + e;
        dd = (dk * dk) + dd;
    }
    double dh[3] = {d[0], d[1], d[2]}; // DAT_1406493f0
    if (dd > kTiny10f) {
        const double r = 1.0 / std::sqrt(dd);
        dh[0] = r * d[0];
        dh[1] = r * d[1];
        dh[2] = r * d[2];
    }
    const double l = std::sqrt(lsq);
    double pad = l * kPad4f; // DAT_140649410
    double box[6];           // DAT_140649420
    i32 two = 2;
    i32 idx[2] = {1, 2}; // DAT_1405c1800
    geo::fun_1404d1b1c(pts, box, &pad, &two, idx);
    i32 cnt = 0; // DAT_140649450
    i32 z1 = 0, z2 = 0;
    fun_140228c60(obj, inst, box, &cnt, &z1, &z2);
    if (cnt == 0) return;
    // Fortran allocatable list DAT_140649340 (max(cnt, 0) ints), freed at the end
    std::vector<i32> list(static_cast<std::size_t>(cnt > 0 ? cnt : 0) + 1);
    i32 z3 = 0;
    fun_14022a5c0(obj, inst, list.data(), reinterpret_cast<std::uint32_t*>(&cnt), &z3);
    i32 prev = 0; // DAT_140649458
    *frac = 1.0;
    for (i32 i = 1; i <= cnt; ++i) {
        i32 poly = list[static_cast<std::size_t>(i - 1)]; // DAT_14064945c
        if (prev == poly) continue;
        prev = poly;
        i32 f2 = 2;
        fun_140228260(obj, inst, &poly, &f2, &g_ray_ei2, &g_ray_ei2_cnt);
        if (g_ray_ei2 != 1) continue;
        fun_14004f363(obj, inst, &poly, g_ray_xp, g_ray_conn, &g_ray_cnt);
        i32 f1 = 1;
        fun_140228540(obj, inst, &poly, &f1, g_ray_er1, &g_ray_er1_cnt);
        const double* n = g_ray_er1;
        const double dn = ((dh[1] * n[1]) + (dh[0] * n[0])) + (dh[2] * n[2]); // DAT_140649528
        if (dn >= 0.0) continue; // JNC: a NaN goes on
        const double nn = ((n[1] * n[1]) + (n[0] * n[0])) + (n[2] * n[2]); // DAT_140649530
        i32 bad = (kTiny10f > nn) ? 1 : 0; // DAT_140649538
        i32 mode = 1;                      // DAT_14064953c
        double din = l;                    // DAT_140649540
        // DAT_140649548 = node count, or 3 for a triangle (written, not read)
        if (g_ray_conn[0] == g_ray_conn[3] || g_ray_conn[3] == 0 || g_ray_cnt == 3) {
            fun_1400647a0(p, dh, g_ray_er1, g_ray_xp, &din, &g_ray_s, &g_ray_t, g_ray_q,
                          &g_ray_hit, &mode, &bad);
        } else {
            fun_140064f58(p, dh, g_ray_er1, g_ray_xp, &din, &g_ray_s, &g_ray_t, g_ray_q,
                          &g_ray_hit, &mode, &bad);
        }
        if (g_ray_hit == 1) {
            const double fr = din / l; // DAT_140649588
            if (std::fabs(*frac) > std::fabs(fr)) {
                *frac = fr;
                q[2] = g_ray_q[2];
                q[1] = g_ray_q[1];
                q[0] = g_ray_q[0];
                uv[1] = g_ray_t;
                *face = poly;
                uv[0] = g_ray_s;
            }
        }
    }
    if (0 < *face) {
        if (*frac > 1.0) *face = 0;
        *len = *len * *frac;
    }
}

// @src 140063ab3  ray-plane intersection (listing L_extra.txt 140063ab3..140063c38)
void fun_140063ab3(double* p, double* dir, double* n, double* x1, double* q, double* d, i32* ok)
{
    const double den = ((dir[1] * n[1]) + (dir[0] * n[0])) + (dir[2] * n[2]); // DAT_1406a1610
    *ok = 0;
    if (1e-11 >= std::fabs(den)) return; // DAT_140bade80; a NaN goes on
    *ok = 1;
    const double a = -(((n[1] * x1[1]) + (n[0] * x1[0])) + (n[2] * x1[2])); // DAT_1406a1618
    const double b = ((n[1] * p[1]) + (n[0] * p[0])) + (n[2] * p[2]);        // DAT_1406a1620
    const double tt = (a + b) / den;
    *d = -tt;
    q[2] = p[2] - (dir[2] * tt);
    q[1] = p[1] - (dir[1] * tt);
    q[0] = p[0] - (dir[0] * tt);
}

// @src 1400647a0  ray-triangle (listing L_extra.txt 1400647a0..140064f53)
void fun_1400647a0(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode, i32* bad)
{
    if (*bad == 1) {
        fun_140065ef8(p, dir, n, x, d, s, t, q, hit, bad);
        return;
    }
    double qq[3];  // DAT_1406a18b0
    double dist;   // DAT_1406a18d0
    i32 ok;        // DAT_1406a18d8
    fun_140063ab3(p, dir, n, x, qq, &dist, &ok);
    const double dn = ((dir[1] * n[1]) + (dir[0] * n[0])) + (dir[2] * n[2]); // DAT_1406a18e0
    *hit = 0;
    if (static_cast<float>(ok) == 0.0f) { // CVTSI2SS + COMISS
        *bad = 0;
        fun_140065ef8(p, dir, n, x, d, s, t, q, hit, bad);
        return;
    }
    if (*mode == 1) {
        if (dn > 0.0) return;
        if (std::fabs(dist) > std::fabs(*d)) return;
    }
    if (*mode == 2) {
        if (std::fabs(dist) > std::fabs(*d)) return;
    }
    // dominant normal axis m (DAT_1406a18e8), the other two j, k (DAT_1406a18ec / f0)
    int j = 2, k = 3;
    double mx = std::fabs(n[0]); // DAT_1406a18f8
    const double ay = std::fabs(n[1]);
    if (ay > mx) {
        j = 3;
        k = 1;
        mx = ay;
    }
    const double az = std::fabs(n[2]);
    if (az > mx) {
        j = 1;
        k = 2;
    }
    const double xj = x[j - 1];
    const double a1 = qq[j - 1] - xj;     // DAT_1406a1900
    const double c1 = x[6 + j - 1] - xj;  // DAT_1406a1920
    const double b1 = x[3 + j - 1] - xj;  // DAT_1406a1910
    const double xk = x[k - 1];
    const double b2 = x[3 + k - 1] - xk;  // DAT_1406a1918
    const double c2 = x[6 + k - 1] - xk;  // DAT_1406a1928
    const double a2 = qq[k - 1] - xk;     // DAT_1406a1908
    double u = -10.0;                     // DAT_1406a1930 (DAT_140badde0)
    double w;                             // DAT_1406a1948
    if (std::fabs(b1) > 1e-11) {
        w = ((b1 * a2) - (a1 * b2)) / ((b1 * c2) - (c1 * b2));
        if (w >= -1e-5 && 1.00001 >= w) u = (a1 - (c1 * w)) / b1;
    } else {
        w = a1 / c1;
        if (w >= -1e-5 && 1.00001 >= w) u = (a2 - (w * c2)) / b2;
    }
    const double sum = u + w; // DAT_1406a1950
    if (u >= -1e-5 && w >= -1e-5 && 1.00001 >= sum) {
        if (0.0 >= u) u = 0.0;
        if (u >= 1.0) u = 1.0;
        if (0.0 >= w) w = 0.0;
        if (w >= 1.0) w = 1.0;
        q[0] = qq[0];
        q[1] = qq[1];
        *hit = 1;
        q[2] = qq[2];
        *d = dist;
        *t = u;
        *s = (1.0 - u) - w;
        return;
    }
    *hit = 0;
}

// @src 140064f58  ray-quad (listing L_64f58.txt)
void fun_140064f58(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode, i32* bad)
{
    // DAT_1406a1958 = 1e-6 (written, not read)
    *hit = 0;
    if (*bad == 1) {
        fun_1400653e5(p, dir, n, x, d, s, t, q, hit, mode);
        return;
    }
    double nq[3]; // DAT_1406a1960
    double tf[9]; // DAT_1406a1980
    geo::fun_1404d3d43(x, nq);
    geo::fun_1404cf5c0(nq, tf);
    double y4[3]; // DAT_1406a19d0
    i32 one = 1, one_b = 1;
    geo::fun_1404ded0c(x + 9, y4, x, tf, &one, &one_b);
    // DAT_1406a19f0 = planar flag (written, not read)
    if (!(1e-6 > std::fabs(y4[2]))) { // DAT_140bad888; a NaN takes the non-planar path
        fun_1400653e5(p, dir, n, x, d, s, t, q, hit, mode);
        return;
    }
    double qq[3]; // DAT_1406a1a00
    double dist;  // DAT_1406a1a20
    i32 ok;       // DAT_1406a1a28
    fun_140063ab3(p, dir, n, x, qq, &dist, &ok);
    if (ok == 0) {
        fun_1400653e5(p, dir, n, x, d, s, t, q, hit, mode);
        return;
    }
    if (*mode == 1) {
        const double dn = ((dir[1] * n[1]) + (dir[0] * n[0])) + (dir[2] * n[2]); // DAT_1406a1a30
        if (dn > 0.0) return;
        if (std::fabs(dist) > std::fabs(*d)) return;
    } else if (*mode == 2) {
        if (std::fabs(dist) > std::fabs(*d)) return;
    }
    i32 zero = 0;
    fun_14007ff21(x, qq, s, t, &g_quad_in, &zero);
    if (0 < g_quad_in) return;
    q[0] = qq[0];
    *hit = 1;
    *d = dist;
    q[1] = qq[1];
    q[2] = qq[2];
}

// @src 14007ff21  point in quad (listing L_extra.txt 14007ff21..1400805aa)
void fun_14007ff21(double* x, double* p, double* s, double* t, i32* in, i32* mode)
{
    // DAT_1406d2f20 = 1e-6 (written, not read)
    double nq[3]; // DAT_1406d2f30
    geo::fun_1404d3d43(x, nq);
    zero_and_normalize(nq, *mode);
    double tf[9]; // DAT_1406d2f60
    geo::fun_1404cf5c0(nq, tf);
    double q0[3]; // DAT_1406d2fb0
    double z0 = 0.0, z1 = 0.0;
    geo::fun_1404d6974(x, q0, &z0, &z1);
    double pp[3]; // DAT_1406d2fd0
    i32 one = 1, one_b = 1;
    geo::fun_1404ded0c(p, pp, q0, tf, &one, &one_b);
    double v[12]; // DAT_1406d2ff0: V'(3,4)
    i32 four = 4;
    geo::fun_1404ded0c(x, v, q0, tf, &four, &one_b);
    double dmin = 1e8; // DAT_1406d3050 (DAT_140bad808)
    double nearq[3];   // DAT_1406d3060
    i32 edge = 0;      // DAT_1406d3080
    i32 idx[8] = {};   // DAT_1406d3084
    i32 nv = 4, p8 = 1, p10 = 0, p11 = 3;
    geo::fun_1404cc289(v, pp, nearq, &dmin, &edge, &nv, in, &p8, idx, &p10, &p11);
    if (!(*in < 1 || (*in == 1 && 1e-6 >= dmin))) return;
    if (*in == 1) *in = 0;
    double xe[2]; // DAT_1406d3090
    i32 kk = 0;   // DAT_1406d30b0
    i32 err = 0;  // DAT_1406d30b4
    geo::fun_1404ccf58(v, pp, xe, &kk, &err);
    if (err == 0) {
        *t = xe[1];
        *s = xe[0];
    } else {
        // 1400803a9: node 4 of V' := node 1, then the nearest edge
        v[11] = v[2];
        v[9] = v[0];
        v[10] = v[1];
        geo::fun_1404d41f8(v, pp, s, t, q0, &dmin);
    }
    geo::fun_1404d6974(v, nearq, s, t);
    const double e = std::fabs(nearq[2] - pp[2]); // DAT_1406d30b8
    if (e > 1e-6) *in = 1;
}

// @src 14007f8c4  point in triangle (listing L_b.txt 14007f8c4..)
void fun_14007f8c4(double* x, double* p, double* s, double* t, i32* in, i32* mode)
{
    // DAT_1406d2da0 = 1e-6 (written, not read)
    double nq[3]; // DAT_1406d2db0
    geo::fun_1404d3d43(x, nq);
    zero_and_normalize(nq, *mode);
    double q0[3]; // DAT_1406d2de0
    double third = 0.3333333333333333; // DAT_1405c8850
    double third_b = third;
    i32 three = 3;
    geo::fun_1404e34eb(x, q0, &third, &third_b, &three);
    double tf[9]; // DAT_1406d2e00
    geo::fun_1404cf5c0(nq, tf);
    double pp[3]; // DAT_1406d2e50
    i32 one = 1, one_b = 1;
    geo::fun_1404ded0c(p, pp, q0, tf, &one, &one_b);
    double* v = g_f8c4_v; // DAT_1406d2e70
    i32 three_b = 3;
    geo::fun_1404ded0c(x, v, q0, tf, &three_b, &one_b);
    double dmin = 1e8; // DAT_1406d2ed0
    double nearq[3];   // DAT_1406d2ee0
    i32 edge = 0;      // DAT_1406d2f00
    i32 idx[8] = {};   // DAT_1406d2f04
    i32 nv = 3, p8 = 1, p10 = 0, p11 = 3;
    geo::fun_1404cc289(v, pp, nearq, &dmin, &edge, &nv, in, &p8, idx, &p10, &p11);
    if (!(*in < 1 || (*in == 1 && 1e-6 >= dmin))) return;
    if (*in == 1) *in = 0;
    double b3 = 0.0; // DAT_1406d2f08
    i32 bad = 0;     // DAT_1406d2f10
    geo::fun_1404e2d44(v, pp, s, t, &b3, &bad);
    if (bad != 0) {
        v[10] = v[1];
        v[9] = v[0];
        v[11] = v[2];
        geo::fun_1404d41f8(v, pp, s, t, q0, &dmin);
    }
    i32 three_c = 3;
    geo::fun_1404e34eb(v, nearq, s, t, &three_c);
    const double e = std::fabs(nearq[2] - pp[2]); // DAT_1406d2f18
    if (e > 1e-6) *in = 1;
}

// @src 1400653e5  non-planar quad: fan of four triangles around the centroid (listing L_extra.txt
// 1400653e5..140065b88)
void fun_1400653e5(double* p, double* dir, double* n, double* x, double* d, double* s, double* t,
                   double* q, i32* hit, i32* mode)
{
    (void)n; // R8 is never read
    i32 k_tri = 0; // DAT_1406a1a3c: last edge with a plane hit inside its triangle
    i32 k_fb = 0;  // DAT_1406a1a40: last edge accepted by the in-plane fall-back
    double tri[9]; // DAT_1406a1a50: X(:,k), X(:,k2), centroid C
    tri[6] = (((x[0] + x[3]) + x[6]) + x[9]) * 0.25; // DAT_140bada38
    tri[7] = (((x[1] + x[4]) + x[7]) + x[10]) * 0.25;
    tri[8] = 0.25 * (((x[2] + x[5]) + x[8]) + x[11]);
    double best = 1e10; // DAT_1406a1aa0 (DAT_140bad6e8)
    i32 kb = 2;
    for (i32 k = 1; k <= 4; ++k, ++kb) { // DAT_1406a1aa8
        const i32 k2 = kb > 4 ? 1 : kb; // DAT_1406a1aac
        tri[0] = x[3 * k - 3];
        tri[1] = x[3 * k - 2];
        tri[2] = x[3 * k - 1];
        tri[3] = x[3 * k2 - 3];
        tri[4] = x[3 * k2 - 2];
        tri[5] = x[3 * k2 - 1];
        double nt[3]; // DAT_1406a1ab0
        geo::fun_1404d3d43(tri, nt);
        const double sa = (std::fabs(nt[1]) + std::fabs(nt[0])) + std::fabs(nt[2]); // DAT_1406a1ad8
        i32 flag = 0; // DAT_1406a1ad0
        bool fallback = true;
        if (sa >= kNineTenthsF) {
            flag = 1;
            double dist; // DAT_1406a1b00
            i32 ok;      // DAT_1406a1b08
            fun_140063ab3(p, dir, nt, tri, g_nq_q, &dist, &ok);
            if (ok != 0) {
                fallback = false;
                // DAT_1406a1b20 = 1 / 0 (written, not read)
                bool test = true;
                if (0 < *mode) {
                    const double dn =
                        ((dir[1] * nt[1]) + (dir[0] * nt[0])) + (dir[2] * nt[2]); // DAT_1406a1b28
                    if (*mode == 1) {
                        if (dn > 0.0 || std::fabs(dist) > std::fabs(*d)) test = false;
                    } else if (*mode == 2) {
                        if (std::fabs(dist) > std::fabs(*d)) test = false;
                    }
                }
                if (test) {
                    i32 zero = 0;
                    fun_14007f8c4(tri, g_nq_q, s, t, &g_nq_in, &zero);
                    if (!(0 < g_nq_in)) {
                        k_tri = k;
                        *hit = 1;
                        if (best > dist) {
                            best = dist;
                            q[0] = g_nq_q[0];
                            q[1] = g_nq_q[1];
                            q[2] = g_nq_q[2];
                        }
                    }
                }
            }
        }
        if (fallback) {
            fun_140065ef8(p, dir, nt, tri, &g_nq_dist, s, t, g_nq_q, hit, &flag);
            const double r3 = (1.0 - *s) - *t; // DAT_1406a1b18
            if (1e-6 > std::fabs(r3) && best > g_nq_dist && *hit == 1) {
                best = g_nq_dist;
                q[0] = g_nq_q[0];
                q[1] = g_nq_q[1];
                k_fb = k;
                q[2] = g_nq_q[2];
            }
        }
    }
    if (1e9 > best && (0 < k_tri || 0 < k_fb)) { // DAT_140badea0
        i32 zero = 0;
        fun_14007ff21(x, q, s, t, &g_nq_in, &zero);
        if (!(0 < g_nq_in)) *d = best;
    }
    if (g_nq_in == 1) *hit = 0;
}

// @src 140065ef8  in-plane ray against the three edges of triangle x (listing L_b.txt)
void fun_140065ef8(double* p, double* dir, double* n, double* x, double* dist, double* s,
                   double* t, double* q, i32* hit, i32* flag)
{
    double tf[9]; // DAT_1406a1cc0
    if (*flag == 1) {
        i32 found = 0; // DAT_1406a1c68
        i32 i = 1;     // DAT_1406a1c64
        i32 jb = 2;
        double e[3];   // DAT_1406a1c70
        double len;    // DAT_1406a1c90
        double c[3];   // DAT_1406a1ca0
        do {
            const i32 j = jb > 3 ? 1 : jb; // DAT_1406a1c6c
            geo::fun_1404d68dd(x + 3 * (j - 1), x + 3 * (i - 1), e, &len);
            if (len > kTiny6f) {
                // 14006603b: R9 still holds &len, so the cross product's length overwrites it
                geo::fun_1404cce5e(e, dir, c, &len);
                if (len > kTiny6f) {
                    geo::fun_1404cf5c0(c, tf);
                    found = 1;
                }
            }
            ++i;
            ++jb;
        } while (i < 4 && found == 0);
        if (found == 0) return;
    } else {
        geo::fun_1404cf5c0(n, tf);
    }
    double y[12]; // DAT_1406a1d10: the three nodes in the frame, then x, y of node 1 again
    i32 one = 1, three = 3;
    geo::fun_1404d0939(x, x, tf, y, &one, &three);
    y[9] = y[0];  // DAT_1406a1d58
    y[10] = y[1]; // DAT_1406a1d60
    double r[6];  // DAT_1406a1d70: P, P + dir
    for (int k = 0; k < 3; ++k) {
        r[k] = p[k];
        r[3 + k] = dir[k] + p[k];
    }
    double z[6]; // DAT_1406a1da0
    i32 one_b = 1, two = 2;
    geo::fun_1404d0939(r, x, tf, z, &one_b, &two);
    const double h = std::fabs(z[2] - y[2]); // DAT_1406a1dd0
    *hit = 0;
    if (h > kTiny6f) return;
    i32 m = 0;          // DAT_1406a1de0
    double best = 1e5;  // DAT_1406a1dd8 (DAT_140bad3c8)
    double out[9];      // DAT_1406a1df0, e08, e20
    i32 ok = 0;         // DAT_1406a1e70
    i32 in1 = 0, in2 = 0, in3 = 0; // DAT_1406a1e74 / 78 / 7c
    i32 n3 = 3;
    fun_140066698(z, y, out, &g_ef8_par[0], &n3, &ok);
    fun_140066902(&g_ef8_par[0], &g_ef8_par[1], &in1);
    if (in1 != 0 && ok != 0) {
        m = 1;
        best = g_ef8_par[0];
    }
    i32 n3b = 3;
    fun_140066698(z, y + 3, out + 3, &g_ef8_par[2], &n3b, &ok);
    fun_140066902(&g_ef8_par[2], &g_ef8_par[3], &in2);
    if (ok != 0 && in2 != 0 && std::fabs(best) > std::fabs(g_ef8_par[2])) {
        best = g_ef8_par[2];
        m = 2;
    }
    i32 n3c = 3;
    fun_140066698(z, y + 6, out + 6, &g_ef8_par[4], &n3c, &ok);
    fun_140066902(&g_ef8_par[4], &g_ef8_par[5], &in3);
    if (ok != 0 && in3 != 0 && std::fabs(best) > std::fabs(g_ef8_par[4])) {
        best = g_ef8_par[4];
        m = 3;
    }
    if (m == 0) return;
    // natural coordinates of the triangle's corners (DAT_1405c80e0, stride 3 doubles):
    // V1 = (1, 0), V2 = (0, 1), V3 = (0, 0)
    static constexpr double kVx[3] = {1.0, 0.0, 0.0};
    static constexpr double kVy[3] = {0.0, 1.0, 0.0};
    const double a = g_ef8_par[2 * (m - 1)];
    *hit = 1;
    q[0] = (dir[0] * a) + p[0];
    q[1] = (dir[1] * a) + p[1];
    const i32 m2 = (m + 1) > 3 ? 1 : (m + 1); // DAT_1406a1e80
    q[2] = (a * dir[2]) + p[2];
    const double b = g_ef8_par[2 * (m - 1) + 1];
    const double omb = 1.0 - b;
    *s = (omb * kVx[m - 1]) + (kVx[m2 - 1] * b);
    *t = (omb * kVy[m - 1]) + (b * kVy[m2 - 1]);
    fun_14007a47d(p, q, dist);
}

// @src 140066698  2D intersection of the lines a(:,1)-a(:,2) and b(:,1)-b(:,2) (listing L_b.txt)
void fun_140066698(double* a, double* b, double* out, double* par, i32* n, i32* ok)
{
    const long long nn = *n;
    const long long st = nn >= 0 ? nn : 0; // column stride
    *ok = 0;
    // statics DAT_1406a1e90..1406a1ed0 in this order; out(i) reads the first *n of them
    double v[9];
    v[0] = a[st] - a[0];          // a1  e90
    v[2] = b[0] - b[st];          // b1  ea0
    v[1] = a[st + 1] - a[1];      // a2  e98
    v[3] = b[1] - b[st + 1];      // b2  ea8
    v[4] = (v[3] * v[0]) - (v[2] * v[1]); // det eb0
    if (1e-9 >= std::fabs(v[4])) return;   // DAT_140badea8; a NaN goes on
    v[5] = b[0] - a[0];           // c1  eb8
    v[6] = b[1] - a[1];           // c2  ec0
    v[7] = ((v[5] * v[3]) - (v[6] * v[2])) / v[4]; // r  ec8
    v[8] = ((v[6] * v[0]) - (v[5] * v[1])) / v[4]; // q  ed0
    par[0] = v[7];
    par[1] = v[8];
    *ok = 1;
    for (i32 i = 1; i <= *n && i <= 9; ++i) out[i - 1] = (v[7] * v[i - 1]) + a[i - 1];
}

// @src 140066902  snap to [0, 1] within 1e-8 and test (listing L_b.txt)
void fun_140066902(double* a, double* b, i32* ok)
{
    *ok = 0;
    if (0.0 > *a && *a > -1e-8) {            // DAT_140badeb0
        *a = 0.0;
    } else if (*a > 1.0 && 1.00000001 > *a) { // DAT_140badeb8
        *a = 1.0;
    }
    if (0.0 > *b && *b > -1e-8) {
        *b = 0.0;
    } else if (1.00000001 > *b && *b > 1.0) {
        *b = 1.0;
    }
    if (*a >= 0.0 && 1.0 >= *a && *b >= 0.0 && 1.0 >= *b) *ok = 1;
}

// @src 14007a47d  distance (listing L_b.txt)
void fun_14007a47d(double* a, double* b, double* dist)
{
    const double e1 = b[0] - a[0];
    const double e2 = b[1] - a[1];
    const double e3 = b[2] - a[2];
    const double sq = ((e2 * e2) + (e1 * e1)) + (e3 * e3);
    *dist = sq;
    if (!(sq == 0.0)) *dist = std::sqrt(sq);
}

} // namespace def::cmd::ctc3d
