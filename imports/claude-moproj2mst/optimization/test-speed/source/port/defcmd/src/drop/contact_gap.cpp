// OBJPOS drop: contact point and signed gap per contact node, separation release.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.5.3; listing out/p673/b/L_b.txt
// (140370a26..140371338).
#include <defcmd/drop/contact.hpp>
#include <defcmd/drop/kernels.hpp>

#include "contact_internal.hpp"

#include <cmath>

namespace def::cmd::drop {

// @src 140370a26  signed gaps and separation release (listing L_b.txt 140370a26..140371338)
void fun_140370a26(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* gap, i32* nb, i32* nb2, i32* nodes)
{
    (void)pt; (void)f; (void)sw; (void)flags; (void)vn; (void)nb2;
    for (i32 b = 1; b <= *nb; ++b) { // DAT_140706268
        const i32* itb = it + 20LL * (b - 1);
        if (!(0 < itb[1])) continue;
        const i32 g1 = itb[8]; // DAT_14070626c
        const i32 g2 = itb[9]; // DAT_140706270
        for (i32 g = g1; g <= g2; ++g) { // DAT_140706274
            const double* rg = rt + 200LL * (g - 1);
            const i32 n1 = ftn_int(rg[2]);                    // DAT_140706278
            const i32 n2 = ftn_int((rg[1] - rg[2]) + 1.0);    // DAT_14070627c
            for (i32 i = n1; i <= n2; ++i) { // DAT_140706280
                const i32 n = nodes[i - 1]; // DAT_140706284
                i32* c = bc + 3LL * (n - 1);
                const i32 s = drop_detail::last_negative_slot(c); // DAT_140706288
                if (s == 0) continue;
                double p[3]; // DAT_140706290
                p[0] = x[3LL * (n - 1)];
                p[1] = x[3LL * (n - 1) + 1];
                p[2] = x[3LL * (n - 1) + 2];
                double nv[3]; // DAT_1407062b0
                const double* nr = nrm + 9LL * (i - 1) + 3LL * (s - 1);
                nv[0] = nr[0];
                nv[1] = nr[1];
                nv[2] = nr[2];
                const i32 face = fid[3LL * (i - 1) + (s - 1)]; // DAT_1407062d0
                const double* xes = xe + 6LL * (i - 1) + 2LL * (s - 1);
                const double xi = xes[0];  // DAT_1407062d8
                const double eta = xes[1]; // DAT_1407062e0
                // DAT_1407062e8 = -BC(s,n) (written, not read)
                double q[3]; // DAT_140706380
                drop_detail::face_point(xu, fu, face, xi, eta, q);
                double dv[3]; // DAT_1407063a0
                dv[0] = q[0] - p[0];
                dv[1] = q[1] - p[1];
                dv[2] = q[2] - p[2];
                double dl = 0.0; // DAT_1407063c0
                i32 three = 3;
                fun_140367f0b(dv, dv, &dl, &three);
                dl = std::sqrt(dl);
                double dn = 0.0; // DAT_1407063c8
                i32 three_b = 3;
                fun_140367f0b(dv, nv, &dn, &three_b);
                if (dn > 0.0 && dl > 1e-5) c[s - 1] = 0; // DAT_140bafcd0
                gap[n - 1] = -dl;
                if (dn > 0.0) gap[n - 1] = dl;
            }
        }
    }
}

} // namespace def::cmd::drop
