// OBJPOS drop: equilibrium test of the settling solver (packet p67-3).
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.12 with the corrections of
// docs/re/commands/OBJPOS_drop_settle_notes.md. Control flow, comparisons (COMISD + JP/Jcc, so
// every test is false for NaN) and FP order from the listing 0x140369c19..0x14036d3fd
// (out/p67/drop-solver/L_all.txt; constants from out/p673/d/L2.txt).
//
// Statics of the binary (0x140705bc8..0x140705fa0): all written before they are read in each
// call (C++ locals here) except the frame T 140705be0 and the vector 140705d40 (globals_d.hpp).
// Several statics are written and never read (140705d90, 140705d98, 140705e08, 140705e0c,
// 140705cc0 as a GAP copy); they are dropped. Copies of 6 doubles (140705d00 -> 140705d40 ->
// 140705c90, 140705da0 added to / subtracted from 140705f30) move 3 slots that are never read
// (0x140705d18.., 0x140705d58.., 0x140705ca8.., 0x140705db8.., 0x140705f48..): only the first
// 3 doubles are kept.
#include <defrt/ftn/geoutl.hpp>
#include <defcmd/drop/globals_d.hpp>
#include <defcmd/drop/kernels.hpp>
#include <defcmd/drop/settle.hpp>

#include <cmath>

namespace def::cmd::drop {

namespace {

using def::rt::geoutl::fun_1404cf5c0;
using def::rt::geoutl::fun_1404ded0c;

constexpr double k_best0 = 2.0;                   // DAT_140baf938
constexpr double k_one = 1.0;                     // DAT_140baf930
constexpr double k_side = 1.0e-14;                // DAT_140bafc68
constexpr double k_side_neg = -1.0e-14;           // DAT_140bafc70
constexpr double k_cos179 = -0.9998476951563913;  // DAT_140bafca8
constexpr double k_big = 1.0e10;                  // DAT_140bafc50
constexpr double k_two = 2.0;                     // DAT_140baf938
constexpr double k_mom = 1.0e-5;                  // DAT_140bafcd0
constexpr double k_cos91 = -0.01745240643728351;  // DAT_140bafcd8
constexpr double k_shift = 10.0;                  // DAT_140bafcf0

// part of @src 140369c19: u = unit(T(r - 0) with component 3 cleared). ded0c with origin
// 140705c30 (zeroed before each block), mode 1; then u(3) = 0 (0x140705ca0 / 0x140705d10),
// l = sqrt(dot(u, u)) and, if l > 0, three divisions.
void dir_in_plane(double* r, double* u)
{
    double o[3] = {0.0, 0.0, 0.0};
    i32 np = 1;
    i32 mode = 1;
    fun_1404ded0c(r, u, o, g_t_140705be0, &np, &mode);
    u[2] = 0.0;
    i32 three = 3;
    double l = 0.0;
    fun_140367f0b(u, u, &l, &three);
    l = std::sqrt(l);
    if (l > 0.0) {
        u[0] = u[0] / l;
        u[1] = u[1] / l;
        u[2] = u[2] / l;
    }
}

// part of @src 140369c19: r = X(:,n) - c
void rel_to_c(const double* x, i32 n, double* r)
{
    const double* p = x + 3 * (n - 1);
    r[0] = p[0] - g_body.c[0];
    r[1] = p[1] - g_body.c[1];
    r[2] = p[2] - g_body.c[2];
}

// part of @src 140369c19: mirrored direction of node n in the plane K(:,1) (nk = 1 blocks):
// p = X(:,n); 140365d0c(p, K(1:3,1), K(4:6,1)); p = p - c; u = dir_in_plane(p)
void mirrored_dir(const double* x, i32 n, double* kp, double* kn, double* u)
{
    const double* xn = x + 3 * (n - 1);
    double p[3] = {xn[0], xn[1], xn[2]};
    fun_140365d0c(p, kp, kn);
    p[0] = p[0] - g_body.c[0];
    p[1] = p[1] - g_body.c[1];
    p[2] = p[2] - g_body.c[2];
    dir_in_plane(p, u);
}

// part of @src 140369c19: one candidate of the "most opposite" search
void search_step(double* ref, double* u, double* best)
{
    i32 three = 3;
    double q = 0.0;
    fun_140367f0b(ref, u, &q, &three);
    if (q < *best) {
        *best = q;
        g_d40_140705d40[0] = u[0];
        g_d40_140705d40[1] = u[1];
        g_d40_140705d40[2] = u[2];
    }
}

// part of @src 140369c19: one node of the side test (cross output into 140705d40)
void side_step(double* ref, double* u, i32* lft, i32* rgt, double* cl, double* cr)
{
    fun_140366881(ref, u, g_d40_140705d40);
    i32 three = 3;
    double q = 0.0;
    fun_140367f0b(ref, u, &q, &three);
    if (g_d40_140705d40[2] > k_side) {
        *lft = 1;
        if (q < *cl) *cl = q;
    }
    if (g_d40_140705d40[2] < k_side_neg) {
        *rgt = 1;
        if (q < *cr) *cr = q;
    }
}

// part of @src 140369c19: sign counters of the moment M' (|M'_a| > 1e-5)
void count_signs(const double* m, i32* cnt6)
{
    for (int a = 0; a < 3; ++a) {
        if (std::fabs(m[a]) > k_mom) {
            if (m[a] > 0.0) cnt6[2 * a] = cnt6[2 * a] + 1;
            if (m[a] < 0.0) cnt6[2 * a + 1] = cnt6[2 * a + 1] + 1;
        }
    }
}

} // namespace

// @src 140369c19  equilibrium test (chkstop)
void fun_140369c19(double* /*rt*/, double* /*pt*/, double* x, double* /*xu*/, i32* /*it*/,
                   i32* /*f*/, i32* /*fu*/, i32* bc, i32* /*sw*/, i32* /*fid*/, double* nrm,
                   double* /*xe*/, i32* /*flags*/, double* /*vn*/, i32* /*nb*/, i32* /*nb2*/,
                   i32* nn, i32* /*nodes*/, double* gap, double* /*xb*/, double* d,
                   double* /*w*/, double* /*kc*/, double* /*damp*/, double* p15, i32* stop,
                   i32* nk, double* k, i32* l840, i32* shift, i32* verbose)
{
    // 140705bc8 = contact count, list into l840 (140704840)
    i32 cnt = 0;
    *stop = 0;
    *shift = 0;
    for (i32 n = 1; n <= *nn; ++n) {
        const i32* b = bc + 3 * (n - 1);
        if (b[0] < 0 || b[1] < 0 || b[2] < 0) {
            cnt = cnt + 1;
            l840[cnt - 1] = n;
        }
    }
    if (cnt == 0) return;
    if (cnt <= 2) {
        if (*nk == 0) return;
        if (*nk == 1 && cnt <= 1) return; // 0x14036cc66
    }
    if (*nk >= 2 && cnt >= 1) { // 0x140369d69
        *stop = 1;
        return;
    }

    if (*nk == 0 && cnt >= 3) { // 0x14036b570
        fun_1404cf5c0(d, g_t_140705be0);
        double r[3];
        double ref[3]; // 140705c90
        double u[3];   // 140705d00
        rel_to_c(x, l840[0], r);
        dir_in_plane(r, ref);
        double best = k_best0; // 140705cc8
        for (i32 i = 2; i <= cnt; ++i) {
            rel_to_c(x, l840[i - 1], r);
            dir_in_plane(r, u);
            search_step(ref, u, &best);
        }
        ref[0] = g_d40_140705d40[0];
        ref[1] = g_d40_140705d40[1];
        ref[2] = g_d40_140705d40[2];
        i32 lft = 0, rgt = 0;           // 140705d70, 140705d74
        double cl = k_one, cr = k_one;  // 140705d78, 140705d80
        for (i32 i = 1; i <= cnt; ++i) {
            rel_to_c(x, l840[i - 1], r);
            dir_in_plane(r, u);
            side_step(ref, u, &lft, &rgt, &cl, &cr);
        }
        if (*verbose > 0) {
            // verbose-only, unreachable from OBJPOS: 0x14036b89e 'MAX COSINE1' cl, 'MAX COSINE2' cr
        }
        if (lft == 1 && rgt == 1) {
            *stop = 1;
            return;
        }
        if (lft == 0 && rgt == 0) return;
        if (lft == 0 || rgt == 0) {
            if (lft == 1 && cl > k_cos179) return;
            if (rgt == 1 && cr > k_cos179) return;
        }
    }

    if (*nk == 1 && cnt >= 2) { // 0x14036b24b
        double kp[3] = {k[0], k[1], k[2]}; // 140705da0 = K(1:3,1)
        double kn[3] = {k[3], k[4], k[5]}; // 140705dd0 = K(4:6,1)
        fun_1404cf5c0(d, g_t_140705be0);
        double r[3];
        double ref[3];
        double u[3];
        rel_to_c(x, l840[0], r);
        dir_in_plane(r, ref);
        double best = k_best0;
        for (i32 i = 2; i <= cnt; ++i) { // direct nodes
            rel_to_c(x, l840[i - 1], r);
            dir_in_plane(r, u);
            search_step(ref, u, &best);
        }
        for (i32 i = 1; i <= cnt; ++i) { // mirror images
            mirrored_dir(x, l840[i - 1], kp, kn, u);
            search_step(ref, u, &best);
        }
        ref[0] = g_d40_140705d40[0];
        ref[1] = g_d40_140705d40[1];
        ref[2] = g_d40_140705d40[2];
        i32 lft = 0, rgt = 0;
        double cl = k_one, cr = k_one;
        for (i32 i = 1; i <= cnt; ++i) {
            rel_to_c(x, l840[i - 1], r);
            dir_in_plane(r, u);
            side_step(ref, u, &lft, &rgt, &cl, &cr);
        }
        for (i32 i = 1; i <= cnt; ++i) {
            mirrored_dir(x, l840[i - 1], kp, kn, u);
            side_step(ref, u, &lft, &rgt, &cl, &cr);
        }
        if (*verbose > 0) {
            // verbose-only, unreachable from OBJPOS: 'MAX COSINE1' / 'MAX COSINE2' (nk = 1 block)
        }
        if (lft == 1 && rgt == 1) {
            *stop = 1;
            return;
        }
        if (lft == 0 && rgt == 0) return;
        if (lft == 0 || rgt == 0) {
            if (lft == 1 && cl > k_cos179) return;
            if (rgt == 1 && cr > k_cos179) return;
        }
    }

    // moment test (0x140369dd4)
    double gmin = k_big; // 140705e00, strict <, list order (unrolled by 8 in the binary)
    for (i32 i = 1; i <= cnt; ++i) {
        const double g = gap[l840[i - 1] - 1];
        if (g < gmin) gmin = g;
    }
    const double lim = *p15 * k_two; // 140705d90
    if (gmin < 0.0 && std::fabs(gmin) > lim) return;

    i32 cnt6[6] = {0, 0, 0, 0, 0, 0}; // 140705e10..e24: +x, -x, +y, -y, +z, -z
    i32 sup = 0;                      // 140705e28
    double fm[3] = {0.0, 0.0, 0.0};   // 140705e30
    fun_1404cf5c0(d, g_t_140705be0);
    double o[3] = {0.0, 0.0, 0.0}; // 140705c30
    for (i32 i = 1; i <= cnt; ++i) {
        const i32 n = l840[i - 1];
        const i32* b = bc + 3 * (n - 1);
        // slot = last negative code (0x14036a6e0..0x14036a74e); none -> next node
        i32 s = 0;
        if (b[0] < 0) s = 1;
        if (b[1] < 0) s = 2;
        if (b[2] < 0) s = 3;
        if (s == 0) continue;
        const double* xn = x + 3 * (n - 1);
        const double* nv = nrm + 9 * (n - 1) + 3 * (s - 1);
        double rr[3] = {xn[0] - g_body.c[0], xn[1] - g_body.c[1], xn[2] - g_body.c[2]}; // ed0
        double fc[3] = {-nv[0], -nv[1], -nv[2]};                                          // f00
        double mom[3];                                                                     // f30
        fun_140366881(rr, fc, mom);
        double mt[3]; // f60
        i32 np = 1, mode = 1;
        fun_1404ded0c(mom, mt, o, g_t_140705be0, &np, &mode);
        count_signs(mt, cnt6);
        if (*nk == 0) {
            i32 three = 3;
            double q = 0.0; // 140705f98
            fun_140367f0b(fc, d, &q, &three);
            if (q <= k_cos91) sup = sup + 1;
            const double dc = static_cast<double>(cnt);
            fm[0] = (fc[0] / dc) + fm[0];
            fm[1] = (fc[1] / dc) + fm[1];
            fm[2] = (fc[2] / dc) + fm[2];
        }
        if (*nk == 1) {
            double kp[3] = {k[0], k[1], k[2]};
            double kn[3] = {k[3], k[4], k[5]};
            mom[0] = mom[0] + kp[0];
            mom[1] = mom[1] + kp[1];
            mom[2] = mom[2] + kp[2];
            fun_140365bfc(mom, kp, kn);
            mom[0] = mom[0] - kp[0];
            mom[1] = mom[1] - kp[1];
            mom[2] = mom[2] - kp[2];
            np = 1;
            mode = 1;
            fun_1404ded0c(mom, mt, o, g_t_140705be0, &np, &mode);
            count_signs(mt, cnt6);
        }
    }

    if (*nk == 0 && sup == 0) { // 0x14036cc8b -> 0x14036a355: no contact supports the weight
        for (int a = 0; a < 3; ++a) fm[a] = ((d[a] + fm[a]) * *p15) * k_shift;
        for (int a = 0; a < 3; ++a) g_body.c[a] = fm[a] + g_body.c[a];
        for (i32 n = 1; n <= *nn; ++n) {
            double* xn = x + 3 * (n - 1);
            for (int a = 0; a < 3; ++a) xn[a] = xn[a] + fm[a];
        }
        for (int j = 0; j < 3; ++j)
            for (int a = 0; a < 3; ++a) g_body.a[j][a] = fm[a] + g_body.a[j][a];
        for (int a = 0; a < 3; ++a) g_body.ct[a] = fm[a] + g_body.ct[a];
        *shift = 1;
        return;
    }

    if (*verbose > 0) {
        // verbose-only, unreachable from OBJPOS: 0x14036af1d 'chkstop - EQM' and the six counters
    }
    const i32 px = cnt6[0], mx = cnt6[1], py = cnt6[2], my = cnt6[3];
    if ((px != 0 || mx < 1) && (px < 1 || mx != 0) && (py != 0 || my < 1) &&
        (py < 1 || my != 0)) {
        i32 two = 0; // 140705fa0
        if (px > 0 && mx > 0) two = 1;
        if (py > 0 && my > 0) two = two + 1;
        if (two > 1) *stop = 1;
    }
}

} // namespace def::cmd::drop
