// OBJPOS drop: contact forces and moments of the contact nodes, rescaled to the weight.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.8; listing out/p673/b/L_b.txt
// (140371339..1403722fb).
#include <defcmd/drop/contact.hpp>
#include <defcmd/drop/kernels.hpp>

#include "contact_internal.hpp"

#include <cmath>

namespace def::cmd::drop {

// @src 140371339  contact forces (listing L_b.txt 140371339..1403722fb)
void fun_140371339(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nodes, double* f6, double* w, double* d, double* kc,
                   double* damp, double* p15, double* fn)
{
    (void)pt; (void)f; (void)sw; (void)flags; (void)nb2; (void)damp; (void)p15;
    double sum[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // DAT_1407063d0
    for (i32 b = 1; b <= *nb; ++b) { // DAT_140706400
        const i32* itb = it + 20LL * (b - 1);
        if (!(0 < itb[1])) continue;
        const i32 g1 = itb[8]; // DAT_140706404
        const i32 g2 = itb[9]; // DAT_140706408
        for (i32 g = g1; g <= g2; ++g) { // DAT_14070640c
            const double* rg = rt + 200LL * (g - 1);
            const i32 n1 = ftn_int(rg[2]);                 // DAT_140706410
            const i32 n2 = ftn_int((rg[1] - rg[2]) + 1.0); // DAT_140706414
            for (i32 i = n1; i <= n2; ++i) { // DAT_140706418
                const i32 n = nodes[i - 1]; // DAT_14070641c
                const i32* c = bc + 3LL * (n - 1);
                const i32 s = drop_detail::last_negative_slot(c); // DAT_140706420
                if (s == 0) continue;
                double p[3]; // DAT_140706430
                p[0] = x[3LL * (n - 1)];
                p[1] = x[3LL * (n - 1) + 1];
                p[2] = x[3LL * (n - 1) + 2];
                double nv[3]; // DAT_140706450
                const double* nr = nrm + 9LL * (i - 1) + 3LL * (s - 1);
                nv[0] = nr[0];
                nv[1] = nr[1];
                nv[2] = nr[2];
                const i32 face = fid[3LL * (i - 1) + (s - 1)]; // DAT_140706470
                const double* xes = xe + 6LL * (i - 1) + 2LL * (s - 1);
                const double eta = xes[1]; // DAT_140706480
                const double xi = xes[0];  // DAT_140706478
                // DAT_140706488 = -BC(s,n) (written, not read)
                double q[3]; // DAT_140706520
                drop_detail::face_point(xu, fu, face, xi, eta, q);
                double dv[3]; // DAT_140706540
                dv[0] = q[0] - p[0];
                dv[1] = q[1] - p[1];
                dv[2] = q[2] - p[2];
                double dl = 0.0; // DAT_140706560 (|D|, not used further)
                i32 three = 3;
                fun_140367f0b(dv, dv, &dl, &three);
                dl = std::sqrt(dl);
                double u[3]; // DAT_140706570 = VN(:,n)
                u[0] = vn[3LL * (n - 1)];
                u[1] = vn[3LL * (n - 1) + 1];
                u[2] = vn[3LL * (n - 1) + 2];
                double un = 0.0; // DAT_140706590
                i32 three_b = 3;
                fun_140367f0b(nv, u, &un, &three_b);
                double ut[3]; // DAT_1407065a0
                ut[0] = u[0] - (nv[0] * un);
                ut[1] = u[1] - (nv[1] * un);
                ut[2] = u[2] - (nv[2] * un);
                double lt = 0.0; // DAT_1407065c0
                i32 three_c = 3;
                fun_140367f0b(ut, ut, &lt, &three_c);
                lt = std::sqrt(lt);
                double th[3] = {ut[0], ut[1], ut[2]}; // DAT_1407065d0
                if (lt > 0.0) {
                    th[0] = ut[0] / lt;
                    th[1] = ut[1] / lt;
                    th[2] = ut[2] / lt;
                }
                double fm = *kc * 0.1; // DAT_1407065f0, DAT_140bafd50
                if (0.0 >= fm) fm = *kc * 0.001; // DAT_140baf9b0
                // DAT_1407065f8 = 0.0: friction coefficient
                double* fni = fn + 6LL * (i - 1); // FN(1:6,i) (DAT_140704930)
                fni[0] = -(nv[0] * fm);
                fni[1] = -(nv[1] * fm);
                fni[2] = -(nv[2] * fm);
                const double mu = 0.0 * fm;
                fni[0] = fni[0] - (mu * th[0]);
                fni[1] = fni[1] - (mu * th[1]);
                fni[2] = fni[2] - (mu * th[2]);
                double r[3]; // DAT_140706600 = X(:,n) - c
                r[0] = p[0] - g_body.c[0];
                r[1] = p[1] - g_body.c[1];
                r[2] = p[2] - g_body.c[2];
                double fv[3] = {fni[0], fni[1], fni[2]}; // DAT_140706620
                double mv[3];                             // DAT_140706640
                fun_140366881(r, fv, mv);
                fni[3] = mv[0];
                fni[4] = mv[1];
                fni[5] = mv[2];
                for (int j = 0; j < 6; ++j) sum[j] = fni[j] + sum[j];
            }
        }
    }
    double a = 0.0; // DAT_140706660
    i32 three = 3;
    fun_140367f0b(sum, d, &a, &three);
    a = std::fabs(a);
    double sc = 1.0; // DAT_140706668
    if (a > 0.0) sc = *w / a;
    for (int j = 0; j < 6; ++j) f6[j] = (sum[j] * sc) + f6[j];
}

} // namespace def::cmd::drop
