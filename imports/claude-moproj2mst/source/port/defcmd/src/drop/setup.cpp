// OBJPOS drop: set-up of the settling solver (body tables, direction and constraint checks).
// Spec: docs/re/commands/OBJPOS_drop.md §5, OBJPOS_drop_solver.md §2.1, §3; listing
// 0x140359a70..0x14035b860.
#include <defcmd/drop/kernels.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/genctc3d_engine.hpp>

#include <cmath>
#include <cstdio>

namespace def::cmd::drop {

namespace {
// @data 1407047b0  nb copy; @data 1407047b8  nn copy; @data 1407047bc  nf copy
i32 g_nb_7b0 = 0, g_nn_7b8 = 0, g_nf_7bc = 0;
// @data 1407047c8  direction length; @data 1407047d8  constraint test value
double g_len_7c8 = 0.0, g_c_7d8 = 0.0;
} // namespace

// @src 140359a70  drop solver set-up (nb must be 2: body 1 moving node set, body 2 rigid surface)
void fun_140359a70(i32* nb, i32* nn, i32* nf, double* x, i32* f, i32* nu, i32* nuf, double* xu,
                   i32* fu, double* dir, i32* nk, double* k, double* v, double* g, double* p15,
                   i32* itmax, i32* verbose, double* t, double* r, i32* status)
{
    DropArrays& A = g_arr;
    ctc3d::m<i32>(0x14075bc80) = 0; // engine partition count / index (GENCTC_3d_search �3.2)
    ctc3d::m<i32>(0x14075bc84) = 0;
    ctc3d::m<i32>(0x14075bc88) = 0;
    *status = 1;
    if (*nb != 2) return;
    if (*g <= 0.0) *g = 0.5;          // DAT_140baf918
    if (*p15 <= 0.0) *p15 = 0.002;    // DAT_140baf920
    g_nb_7b0 = *nb;
    const long long nbb = *nb < 0 ? 0 : *nb;
    A.it.assign(20 * nbb, 0);
    A.rt.assign(200 * nbb, 0.0);
    A.pt.assign(20 * nbb * nbb, 0.0);
    const long long n = *nn < 0 ? 0 : *nn;
    A.xe.assign(6 * n, 0.0);
    A.nrm.assign(9 * n, 0.0);
    A.nodes.assign(n, 0);
    A.flags.assign(n, 0);
    A.bc.assign(3 * n, 0);
    A.fid.assign(3 * n, 0);
    A.sw.assign(6 * n, 0);
    for (i32 i = 1; i <= *nn; ++i) A.nodes[i - 1] = i;
    i32* IT = A.it.data();
    double* RT = A.rt.data();
    double* PT = A.pt.data();
    // body 1: the moving node set with faces
    IT[9] = 1;
    IT[8] = 1;
    IT[4] = 1;
    IT[1] = 1;
    IT[0] = 1;
    IT[5] = *nn;
    // 14035ab0c: the element type is 4 (3-node faces) when F(4,1) == F(1,1), else 8 (the first
    // test F(4,1) == 0 -> 4 is overwritten)
    if (f[3] == 0) IT[12] = 4;
    if (f[3] == f[0]) IT[12] = 4;
    else IT[12] = 8;
    RT[4] = 1.0;
    RT[0] = 3.0; // DAT_140baf928
    RT[2] = 1.0;
    RT[1] = static_cast<double>(*nn);
    IT[13] = 1;
    RT[3] = static_cast<double>(*nf);
    PT[20 * nbb] = 1.0;  // pair (j = 2, body 1): body 2 is a partner of body 1
    for (i32 i = 1; i <= *nn; ++i) A.flags[i - 1] = i;
    PT[20] = 2.0;        // DAT_140baf938
    // 14035ae4b: sqrt((d3*d3) + ((d2*d2) + (d1*d1)))
    g_len_7c8 = std::sqrt((dir[2] * dir[2]) + ((dir[1] * dir[1]) + (dir[0] * dir[0])));
    g_nf_7bc = *nf;
    g_nn_7b8 = *nn;
    // body 2: the rigid surface
    RT[208] = 1.0;
    RT[206] = 1.0;
    RT[200] = 1.0;
    IT[29] = 2;
    IT[28] = 2;
    RT[207] = static_cast<double>(*nuf);
    IT[21] = 0;
    RT[205] = static_cast<double>(*nu);
    IT[20] = 2;
    if (!(g_len_7c8 > 0.0)) {
        // 14035afa8: zero length -> no solve (status 1); NaN falls through undivided
        if (g_len_7c8 == 0.0) goto done;
    } else {
        for (int i = 0; i < 3; ++i) dir[i] = dir[i] / g_len_7c8;
    }
    {
        bool fail = false;
        i32 three = 3;
        if (*nk == 1 || *nk == 2) {
            fun_140367f0b(k + 3, dir, &g_c_7d8, &three);
            g_c_7d8 = std::fabs(g_c_7d8);
            if (g_c_7d8 <= 1e-8) { // DAT_140baf940
                if (*nk == 2) {
                    fun_140367f0b(k + 9, dir, &g_c_7d8, &three);
                    g_c_7d8 = std::fabs(g_c_7d8);
                    if (1e-8 < g_c_7d8) fail = true;
                }
            } else {
                fail = true;
            }
        }
        if (!fail && *nk == 3) {
            fun_140367f0b(k + 15, dir, &g_c_7d8, &three);
            g_c_7d8 = std::fabs(std::fabs(g_c_7d8) + -1.0); // DAT_140baf950
            if (1e-8 < g_c_7d8) fail = true;
        }
        if (fail) {
            if (0 < *verbose) {
                // 14035b5af: list-directed WRITE, unreachable from OBJPOS (verbose 0)
                std::printf(" moving direction and symmetry plane normal vector\n");
            }
            goto done;
        }
        g_nv = 0;
        fun_140367f0b(v, v, &g_c_7d8, &three);
        if (0.0 < g_c_7d8) {
            g_nv = 1;
            for (int i = 0; i < 3; ++i) v[i] = v[i] / g_c_7d8;
        }
        fun_140367f0b(v + 3, v + 3, &g_c_7d8, &three);
        if (0.0 < g_c_7d8) {
            g_nv = g_nv + 1;
            for (int i = 0; i < 3; ++i) v[3 + i] = v[3 + i] / g_c_7d8;
        }
        fun_14035b861(x, RT, xu, IT, f, fu, A.nodes.data(), nb, &g_nb_7b0, &g_nn_7b8, &g_nf_7bc,
                      nf, nn, nuf, nu, PT, A.nrm.data(), A.xe.data(), A.bc.data(), A.fid.data(),
                      A.flags.data(), A.sw.data(), dir, nk, k, &g_nv, v, g, p15, itmax, verbose, t,
                      r, status);
    }
done:
    A.rt.clear();
    A.pt.clear();
    A.nrm.clear();
    A.xe.clear();
    A.it.clear();
    A.bc.clear();
    A.fid.clear();
    A.nodes.clear();
    A.flags.clear();
    A.sw.clear();
}

} // namespace def::cmd::drop
