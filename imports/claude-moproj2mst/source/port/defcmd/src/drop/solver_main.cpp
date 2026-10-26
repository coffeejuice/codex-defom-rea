// OBJPOS drop: main routine of the settling solver.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §4; listing 0x14035b861..0x140362db8
// (out/p67/drop-solver/L_all.txt), FP traces out/p67/drop-solver/sym_b861.txt.
//
// Not ported (no effect on results): the empty stubs 14001cd20 / 14001cd30 / 14001cd40 (3-byte
// RET) and their counter DAT_140704cd4; the verbose > 0 list-directed prints and the verbose == 2
// WKP2.GEO dump (OBJPOS always passes verbose = 0; marked below).
#include <defcmd/drop/contact.hpp>
#include <defcmd/drop/kernels.hpp>
#include <defcmd/drop/settle.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/genctc3d_engine.hpp>
#include <defrt/port.hpp>

#include <cmath>
#include <cstring>

namespace def::cmd::drop {

namespace {

// @data 140704af0  A6(6,6) mass matrix of the step (column-major)
double g_a6[36];
// @data 140704c10  F6(6) forces, then the solution (accelerations)
double g_f6[6];
// @data 140704c40  acceleration of the previous step
double g_acc_prev[6];
// @data 140704c70  Sk(3,3) skew matrix of the angular momentum (written, never read)
double g_sk[9];
// @data 140704cc0  iteration limit
i32 g_itmax = 0;
// @data 140704cd8  simulated time (written only)
double g_time = 0.0;
// @data 140704d2c  nodes per face (4 or 3); kept when IT(13) is neither 8 nor 4
i32 g_npf = 0;
// @data 140704da8  "no velocity or no contact" flag (written only)
i32 g_flag_da8 = 0;
// @data 140704db0, @data 140704dd0  v and w passed to the step search
double g_v_db0[3];
double g_w_dd0[3];
// @data 140704e00  error flag of the 6x6 solve
i32 g_err = 0;
// @data 140704e04
i32 g_e04 = 0;
// @data 140704ea0  centre of mass after the step (written, never read)
double g_c_ea0[3];

} // namespace

// @src 14035b861  settling solver (explicit rigid-body dynamics with penalty contact)
void fun_14035b861(double* x, double* rt, double* xu, i32* it, i32* f, i32* fu, i32* nodes,
                   i32* nb, i32* nb2, i32* nn, i32* nf2, i32* nf, i32* nn2, i32* nuf, i32* nu,
                   double* pt, double* nrm, double* xe, i32* bc, i32* fid, i32* flags, i32* sw,
                   double* d, i32* nk, double* k, i32* nv, double* v, double* g, double* p15,
                   i32* itmax, i32* verbose, double* t, double* r, i32* status)
{
    (void)nf2; (void)nf; (void)nuf; (void)nu;
    DropArrays& A = g_arr;

    // 4.1 initialisation (14035b861..14035d8f0)
    std::memset(g_a6, 0, sizeof g_a6);
    std::memset(g_f6, 0, sizeof g_f6);
    std::memset(g_acc_prev, 0, sizeof g_acc_prev);
    std::memset(g_sk, 0, sizeof g_sk);
    for (double& a : g_body.ang) a = 0.0;
    g_itmax = *itmax;
    g_time = 0.0;
    g_it = 0;
    g_dt = 0.001; // DAT_140baf9b0
    for (int i = 0; i < 3; ++i) g_body.v[i] = 0.0;
    for (int i = 0; i < 3; ++i) g_body.w[i] = 0.0;
    for (int i = 0; i < 6; ++i) g_body.acc[i] = 0.0;

    i32 nfree = 0, nfixed = 0; // DAT_140704ce4, DAT_140704ce8
    for (i32 b = 1; b <= *nb; ++b) {
        const i32 fl = it[20 * (b - 1) + 1];
        if (0 < fl) ++nfree;
        if (fl == 0) ++nfixed;
    }
    if (nfixed == 0) return;
    g_ins = 0;
    if (nfree == 0) return;
    if (1 < nfree) return;

    i32 first = 0, nnb = 0, ngroups = 0; // DAT_140704cf8, DAT_140704d00, DAT_140704d0c
    i32 nnode = 0, fface = 0, nface = 0; // DAT_140704d20, DAT_140704d24, DAT_140704d28
    for (i32 b = 1; b <= *nb; ++b) {
        const i32* itb = it + 20 * (b - 1);
        if (!(0 < itb[1])) continue;
        first = itb[4];
        nnb = (itb[5] - itb[4]) + 1;
        ngroups = (itb[7] - itb[6]) + 1;
        for (i32 gi = itb[8]; gi <= itb[9]; ++gi) {
            const double* R = rt + 200 * (gi - 1);
            nnode = ftn_int(R[1]);
            fface = ftn_int(R[4]);
            nface = ftn_int(R[3]);
        }
        A.fv.assign(3 * (nface < 0 ? 0 : nface), 0.0);
        A.vn.assign(3 * (nnode < 0 ? 0 : nnode), 0.0);
        A.vnp.assign(3 * (nnode < 0 ? 0 : nnode), 0.0);
        A.l840.assign(nnode < 0 ? 0 : nnode, 0);
        A.l890.assign(nnode < 0 ? 0 : nnode, 0);
        A.l8e0.assign(nnode < 0 ? 0 : nnode, 0);
        if (itb[12] == 8) g_npf = 4;
        if (itb[12] == 4) g_npf = 3;
        g_tol_b84728 = 0.01;
        g_tol_b84720 = 1e-4;
        fun_1403750cf(x, f, &g_npf, &ngroups, &g_len_b84730);
    }
    fun_140365e0b();
    A.fn.assign(6 * (*nn2 < 0 ? 0 : *nn2), 0.0);
    A.xb.assign(3 * (*nn < 0 ? 0 : *nn), 0.0);
    A.gap.assign(*nn < 0 ? 0 : *nn, 0.0);

    fun_140362db9(x, f, &fface, &nface, &g_npf, A.fv.data(), nk, k);
    for (int i = 0; i < 3; ++i) g_vel6[i] = g_body.v[i];
    for (int i = 0; i < 3; ++i) g_vel6[3 + i] = g_body.w[i];
    i32 six = 6;
    fun_140367f0b(g_vel6, g_vel6, &g_vnorm, &six);
    g_vnorm = std::sqrt(g_vnorm);
    double* xb0 = x + 3 * (first - 1); // X(:, IT(5)) of the free body
    fun_1403668de(xb0, A.vn.data(), &nnb);
    g_weight = g_body.mass * 9800.0; // DAT_140baf9c8
    for (int i = 0; i < 3; ++i) g_body.c0[i] = g_body.c[i];
    for (int i = 0; i < 3; ++i) g_body.ct[i] = g_body.c[i];
    for (int a = 0; a < 3; ++a)
        for (int i = 0; i < 3; ++i) g_body.a[a][i] = 0.0;
    for (int i = 0; i < 3; ++i) g_body.pad_78[i] = 0.0; // 14035c0a0: the zeroing loop runs 4 rows
    g_body.a[2][2] = 1.0;
    g_body.a[0][0] = 1.0;
    g_body.a[1][1] = 1.0;
    for (int a = 0; a < 3; ++a)
        for (int i = 0; i < 3; ++i) g_body.a[a][i] = g_body.c0[i] + g_body.a[a][i];
    // verbose > 0: 'Iteration = ' 1 (14035c1b1), unreachable from OBJPOS

    i32 ncubes = 10; // DAT_140704d70
    ctc3d::fun_140225640(&ncubes, verbose);
    g_it = 5;

    double* vn = A.vn.data();
    double* gap = A.gap.data();
    double* xb = A.xb.data();
    i32 flag1 = 1;
    bool finish = false;

    // 4.2 first detection and initial drop
    fun_1403704c9(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &g_dt, nb, nb2, nn,
                  nodes, gap, xb, &g_weight, &g_kc, &g_damp, p15, &g_nc, &flag1, verbose);
    if (g_nc == 0) {
        g_redrop = 0;
        fun_140368889(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, nb, nb2, nodes,
                      &g_ins, &g_redrop, d, p15);
        if (g_redrop == 1) finish = true;
        else fun_140362db9(x, f, &fface, &nface, &g_npf, A.fv.data(), nk, k);
    }
    if (!finish) {
        fun_1403704c9(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &g_dt, nb, nb2,
                      nn, nodes, gap, xb, &g_weight, &g_kc, &g_damp, p15, &g_nc, &g_ins, verbose);
        fun_140367fe2(&g_k, &g_damp, &g_weight, p15);
        if (g_nc == 0) g_kc = g_k;
        else g_kc = g_k / static_cast<double>(g_nc);
        fun_140369c19(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, nb, nb2, nn,
                      nodes, gap, xb, d, &g_weight, &g_kc, &g_damp, p15, &g_stop, nk, k,
                      A.l840.data(), &g_shift, verbose);
        if (g_stop == 1) finish = true;
    }

    // 4.3 iteration
    while (!finish) {
        g_it = g_it + 1;
        // verbose > 0: 'Iteration = ' it+1 (14035e1a0), unreachable from OBJPOS
        std::memset(g_a6, 0, sizeof g_a6);
        std::memset(g_f6, 0, sizeof g_f6);
        std::memset(g_sk, 0, sizeof g_sk);
        const double h = g_dt; // dVar6 (14035e2b0)
        g_h = g_dt;
        for (size_t i = 0; i < A.vn.size(); ++i) A.vnp[i] = A.vn[i];
        g_flag_da8 = (g_vnorm < 1.000000013351432e-10) ? 1 : 0; // DAT_140baf9e0 (float 1e-10)
        if (g_nc == 0) g_flag_da8 = 1;
        if (1 < g_it) {
            for (int i = 0; i < 6; ++i) g_acc_prev[i] = g_body.acc[i];
        }
        for (int i = 0; i < 3; ++i) g_v_db0[i] = g_body.v[i];
        for (int i = 0; i < 3; ++i) g_w_dd0[i] = g_body.w[i];
        // A6(1:3,1:3) = M (cd88), A6(4:6,4:6) = I (cd40); column-major 6x6
        for (int j = 0; j < 3; ++j)
            for (int i = 0; i < 3; ++i) g_a6[6 * j + i] = g_body.massm[3 * j + i];
        for (int j = 0; j < 3; ++j)
            for (int i = 0; i < 3; ++i) g_a6[6 * (j + 3) + (i + 3)] = g_body.inertia[3 * j + i];
        for (int i = 0; i < 3; ++i) g_f6[i] = d[i] * g_weight;
        std::memset(g_sk, 0, sizeof g_sk);
        // gyroscopic moment (14035f932..14035fb1d); I(i,j) = inertia[(i-1) + 3(j-1)]
        const double* I = g_body.inertia;
        const double wx = g_body.w[0], wy = g_body.w[1], wz = g_body.w[2];
        const double L3 = (wz * I[8]) + ((wy * I[5]) + (wx * I[2]));
        const double L2 = (wz * I[7]) + ((wy * I[4]) + (wx * I[1]));
        const double L1 = (wz * I[6]) + ((wy * I[3]) + (wx * I[0]));
        g_sk[1] = -L3; // DAT_140704c78
        g_sk[2] = L2;  // DAT_140704c80
        g_sk[3] = L3;  // DAT_140704c88
        g_sk[5] = -L1; // DAT_140704c98 (written, not read)
        g_sk[6] = -L2; // DAT_140704ca0
        g_sk[7] = L1;  // DAT_140704ca8
        g_f6[3] = (g_f6[3] - (wy * L3)) + (L2 * wz);
        g_f6[4] = ((wx * L3) + g_f6[4]) - (wz * L1);
        g_f6[5] = (g_f6[5] - (wx * L2)) + (wy * L1);
        if (0 < g_nc) {
            fun_140371339(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, nb, nb2,
                          nodes, g_f6, &g_weight, d, &g_kc, &g_damp, p15, A.fn.data());
        }
        g_nc = 0;
        for (i32 n = 1; n <= *nn; ++n) {
            const i32* c = bc + 3 * (n - 1);
            if (c[0] < 0 || c[1] < 0 || c[2] < 0) ++g_nc;
        }
        if (g_nc == 0) g_kc = g_k;
        else g_kc = g_k / static_cast<double>(g_nc);
        fun_140366ac3(g_a6, g_f6, &six, &g_err);
        if (g_err == 1) {
            // 140362d97: arrays are not freed in the original (leaked); status stays 1
            g_e04 = 1;
            ctc3d::fun_140225820(verbose);
            return;
        }
        for (int i = 0; i < 6; ++i) g_body.acc[i] = g_f6[i];
        g_time = h + g_time;
        if (g_nc < 1) {
            if (g_nc == 0) {
                if (g_it < 2) {
                    for (int i = 0; i < 6; ++i) g_vel6[i] = (h * g_f6[i]) + g_vel6[i];
                } else {
                    // 1403622dd: (((accPrev*0.5) + (acc*0.5)) * h) + V
                    for (int i = 0; i < 6; ++i)
                        g_vel6[i] = (((g_acc_prev[i] * 0.5) + (g_f6[i] * 0.5)) * h) + g_vel6[i];
                }
            }
        } else {
            for (int i = 0; i < 6; ++i) g_vel6[i] = h * g_f6[i];
        }
        fun_140367f0b(g_vel6, g_vel6, &g_vnorm, &six);
        g_vnorm = std::sqrt(g_vnorm);
        for (int i = 0; i < 3; ++i) g_body.v[i] = g_vel6[i];
        for (int i = 0; i < 3; ++i) g_body.w[i] = g_vel6[3 + i];

        // allowed motions (§5.11), listing 140360063..1403610f7
        i32 three = 3;
        double q = 0.0; // DAT_140704e50
        double n1[3], n2[3];
        if (*nk == 1 || *nk == 2) {
            for (int i = 0; i < 3; ++i) n1[i] = k[3 + i];
            fun_140367f0b(n1, g_body.v, &q, &three);
            for (int i = 0; i < 3; ++i) g_body.v[i] = g_body.v[i] - (n1[i] * q);
            fun_140367f0b(n1, g_body.w, &q, &three);
            for (int i = 0; i < 3; ++i) g_body.w[i] = n1[i] * q;
            if (*nk == 2) {
                for (int i = 0; i < 3; ++i) n2[i] = k[9 + i];
                fun_140367f0b(n2, g_body.v, &q, &three);
                for (int i = 0; i < 3; ++i) g_body.v[i] = g_body.v[i] - (n2[i] * q);
                for (int i = 0; i < 3; ++i) g_body.w[i] = 0.0;
            }
        }
        if (*nk == 3) {
            for (int i = 0; i < 3; ++i) n1[i] = k[15 + i];
            fun_140367f0b(n1, g_body.v, &q, &three);
            for (int i = 0; i < 3; ++i) g_body.v[i] = n1[i] * q;
            for (int i = 0; i < 3; ++i) g_body.w[i] = 0.0;
        }
        if (0 < *nv) {
            fun_140367f0b(v, g_body.w, &q, &three);
            for (int i = 0; i < 3; ++i) n1[i] = v[i] * q;
            fun_140367f0b(v + 3, g_body.w, &q, &three);
            for (int i = 0; i < 3; ++i) n2[i] = v[3 + i] * q;
            for (int i = 0; i < 3; ++i) g_body.w[i] = n1[i] + n2[i];
        }
        fun_1403668de(xb0, vn, &nnb);
        // verbose > 0: ' VELOC(1:3)' VN(:,1) (140360619), unreachable from OBJPOS
        // 14036063d: only node 1 is checked
        if (std::fabs(vn[0]) > 1e100 || std::fabs(vn[1]) > 1e100 || std::fabs(vn[2]) > 1e100) {
            g_stop = 1;
            break;
        }
        if (g_stop == 1) break;
        g_redrop = 0;
        fun_14036d3fd(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &g_h, nb, nb2, nn,
                      nodes, &g_ins, gap, xb, d, &g_weight, &g_kc, &g_damp, p15, g_c_ea0, g_v_db0,
                      g_w_dd0, nk, k, A.l840.data(), A.l890.data(), A.l8e0.data(), nv, v, g,
                      &g_redrop, verbose);
        if (g_redrop == 1) break;
        for (int i = 0; i < 3; ++i) g_c_ea0[i] = g_body.c[i];
        fun_140362db9(x, f, &fface, &nface, &g_npf, A.fv.data(), nk, k);
        fun_1403704c9(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &g_h, nb, nb2, nn,
                      nodes, gap, xb, &g_weight, &g_kc, &g_damp, p15, &g_nc, &g_ins, verbose);
        // verbose == 2: WKP2.GEO dump (140361925..1403620e7), unreachable from OBJPOS
        fun_140369c19(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, nb, nb2, nn,
                      nodes, gap, xb, d, &g_weight, &g_kc, &g_damp, p15, &g_stop, nk, k,
                      A.l840.data(), &g_shift, verbose);
        if (g_shift == 1) {
            fun_1403704c9(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &g_h, nb,
                          nb2, nn, nodes, gap, xb, &g_weight, &g_kc, &g_damp, p15, &g_nc, &g_ins,
                          verbose);
        }
        if (g_stop == 1 || g_itmax <= g_it) break;
        if (g_interrupt == 1) break; // DAT_140704ec4, set only by the empty stub 14001cd40
    }

    // 4.4 FINISH (1403606eb)
    // verbose == 2: WKP2.GEO dump, unreachable from OBJPOS
    fun_140362db9(x, f, &fface, &nface, &g_npf, A.fv.data(), nk, k);
    // verbose > 0: 'positioning finished', mass, centre, contact points (140360817..), unreachable
    double tt[3];
    tt[0] = g_body.ct[0] - g_body.c0[0];
    tt[1] = g_body.ct[1] - g_body.c0[1];
    tt[2] = g_body.ct[2] - g_body.c0[2];
    for (int a = 0; a < 3; ++a)
        for (int i = 0; i < 3; ++i) g_body.a[a][i] = g_body.a[a][i] - g_body.ct[i];
    i32 three = 3;
    for (int a = 0; a < 3; ++a) {
        fun_140367f0b(g_body.a[a], g_body.a[a], &g_vnorm, &three);
        g_vnorm = std::sqrt(g_vnorm);
        for (int i = 0; i < 3; ++i) g_body.a[a][i] = g_body.a[a][i] / g_vnorm;
    }
    // verbose > 0: 'linear transform', 'rotational matrix', 'rotation center' prints, unreachable
    t[0] = tt[0];
    t[1] = tt[1];
    t[2] = tt[2];
    for (int a = 0; a < 3; ++a)
        for (int i = 0; i < 3; ++i) r[3 * a + i] = g_body.a[a][i];
    for (int i = 0; i < 3; ++i) r[9 + i] = g_body.ct[i];
    *status = 0;
    A.fv.clear();
    A.vn.clear();
    A.vnp.clear();
    A.l840.clear();
    A.l890.clear();
    A.l8e0.clear();
    A.fn.clear();
    A.xb.clear();
    A.gap.clear();
    ctc3d::fun_140225820(verbose);
}

} // namespace def::cmd::drop
