// OBJPOS drop: initial drop of the moving body along d (rays from its nodes to the fixed
// surface, else from the fixed nodes back along -d) and the rigid translation by dmin + p15/2.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.6; listing 0x140368889..0x140369c18.
#include <defcmd/drop/solver.hpp>
#include <defcmd/genctc3d_engine.hpp>
#include <defcmd/genctc3d_ray.hpp>

#include <cmath>

namespace def::cmd::drop {

namespace {
inline double rt_at(const double* rt, i32 g, i32 k) { return rt[200 * (g - 1) + (k - 1)]; }
} // namespace

// @src 140368889  initial drop
void fun_140368889(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nodes, i32* ins, i32* redrop, double* d, double* p15)
{
    (void)f; (void)bc; (void)sw; (void)fid; (void)nrm; (void)xe; (void)flags; (void)vn;
    const long long stride = *nb2 < 0 ? 0 : *nb2;
    for (i32 rb = 1; rb <= *nb; ++rb) {
        i32* itb = it + 20 * (rb - 1);
        if (!(0 < itb[1])) continue;
        i32 grp_first = itb[8];  // DAT_140705a74
        const i32 grp_last = itb[9];
        double dmin = 1e10;      // DAT_140705a80 (DAT_140bafc50)
        double L = 0.0;          // DAT_140705ae0
        i32 hits = 0;            // DAT_140705ae8
        i32 partner = 0;         // DAT_140705a98
        i32 last_partner = 0;    // DAT_140705aac
        // local_98: last argument of the ray query; the forward loop sets it to the group number
        // before each NR1 lookup, the reverse loop passes whatever it holds then (uninitialised
        // stack in the original if the forward loop made no lookup; 0 here)
        i32 local98 = 0;
        double dd[3];            // DAT_140705ac0
        for (i32 g = grp_first; g <= grp_last; ++g) {
            const i32 n_first = ftn_int(rt_at(rt, g, 3));
            const i32 n_last = ftn_int((rt_at(rt, g, 2) - rt_at(rt, g, 3)) + 1.0);
            for (i32 j = 1; j <= *nb; ++j) {
                partner = it[20 * (j - 1) + 8];
                const double* pair = pt + 20 * (((long long)(partner - 1) * stride + g) - 1);
                if (ftn_nint(pair[0]) != 1) continue;
                i32 nu = ftn_nint(rt_at(rt, partner, 6));
                i32 nuf = ftn_nint(rt_at(rt, partner, 8));
                const i32 fnode = ftn_nint(rt_at(rt, partner, 7));
                const i32 fface = ftn_nint(rt_at(rt, partner, 9));
                last_partner = partner;
                if (0 < *ins) {
                    // 1403689f0..: insert the partner's surface (not reached from OBJPOS, ins = 0)
                    i32 inst = 1, nvert = 4, zero = 0, fl = 0;
                    local98 = 1;
                    ctc3d::fun_14004f56b(&partner, &inst, xu + 3 * (fnode - 1), fu + 4 * (fface - 1),
                                         &nvert, &local98, &nu, &nuf, &zero, &fl);
                }
                dd[0] = d[0];
                dd[1] = d[1];
                dd[2] = d[2];
                hits = 0;
                L = 1e5; // DAT_140bafc60
                for (i32 n = n_first; n <= n_last; ++n) {
                    const i32 node = nodes[n - 1];
                    double p[3] = {x[3 * (node - 1)], x[3 * (node - 1) + 1], x[3 * (node - 1) + 2]};
                    i32 inst = 2, field = 1, ent = n;
                    local98 = g;
                    ctc3d::fun_140228b10(&local98, &inst, &ent, &field, g_nr1_buf, &g_nr1_cnt);
                    // 1403697ce: (nr3*d3) + ((nr2*d2) + (nr1*d1))
                    const double dot = (g_nr1_buf[2] * dd[2]) + ((g_nr1_buf[1] * dd[1]) + (g_nr1_buf[0] * dd[0]));
                    if (0.0 <= dot) {
                        double q[3], frac, uv[2];
                        i32 face;
                        i32 inst1 = 1;
                        ctc3d::fun_14001f54f(&partner, &inst1, p, dd, q, &L, &frac, uv, &face, &local98);
                        if (0 < face && 0.0 <= frac) {
                            if (L < dmin) dmin = L;
                            ++hits;
                        }
                        L = dmin;
                    }
                }
            }
        }
        *redrop = 0;
        if (hits == 0) {
            *redrop = 1;
            const i32 n_first = ftn_int(rt_at(rt, last_partner, 7));
            const i32 n_last = ftn_int((rt_at(rt, last_partner, 6) - rt_at(rt, last_partner, 7)) + 1.0);
            dd[0] = -d[0];
            dd[1] = -d[1];
            dd[2] = -d[2];
            i32 hits2 = 0; // DAT_140705ba8
            L = 1e5;
            if (n_last < n_first) return;
            for (i32 n = n_first; n <= n_last; ++n) {
                double p[3] = {xu[3 * (n - 1)], xu[3 * (n - 1) + 1], xu[3 * (n - 1) + 2]};
                i32 inst = 1, field = 1, ent = n;
                ctc3d::fun_140228b10(&last_partner, &inst, &ent, &field, g_nr1_buf, &g_nr1_cnt);
                const double dot = (g_nr1_buf[2] * dd[2]) + ((g_nr1_buf[1] * dd[1]) + (g_nr1_buf[0] * dd[0]));
                if (0.0 <= dot) {
                    double q[3], frac, uv[2];
                    i32 face;
                    i32 inst2 = 2;
                    ctc3d::fun_14001f54f(&grp_first, &inst2, p, dd, q, &L, &frac, uv, &face, &local98);
                    if (0 < face && 0.0 <= frac) {
                        if (L < dmin) dmin = L;
                        ++hits2;
                    }
                    L = dmin;
                }
            }
            if (hits2 == 0) return;
        }
        const double half = *p15 * 0.5; // DAT_140705bb0 (DAT_140baf918)
        if (0.0 < dmin) {
            // 14036897a: sqrt((d3*d3) + ((d2*d2) + (d1*d1))); s = dmin + p15/2; delta = (d/l)*s
            const double l = std::sqrt((d[2] * d[2]) + ((d[1] * d[1]) + (d[0] * d[0])));
            const double s = dmin + half;
            double delta[3];
            for (int k = 0; k < 3; ++k) delta[k] = (d[k] / l) * s;
            for (i32 n = itb[4]; n <= itb[5]; ++n)
                for (int k = 0; k < 3; ++k) x[3 * (n - 1) + k] = x[3 * (n - 1) + k] + delta[k];
            for (int k = 0; k < 3; ++k) g_body.c[k] = delta[k] + g_body.c[k];
            for (int k = 0; k < 3; ++k) g_body.ct[k] = delta[k] + g_body.ct[k];
            for (int a = 0; a < 3; ++a)
                for (int k = 0; k < 3; ++k) g_body.a[a][k] = delta[k] + g_body.a[a][k];
        }
    }
}

} // namespace def::cmd::drop
