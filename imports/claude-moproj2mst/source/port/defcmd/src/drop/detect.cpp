// OBJPOS drop: contact detection of the moving body through the shared GENCTC 3D engine.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.5, §5.5.1; listing 0x1403704c9..0x140373650,
// 0x14005150d, 0x140049361 (decompiles out/p67/drop-solver/f/).
#include <defcmd/drop/contact.hpp>
#include <defcmd/drop/kernels.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/genctc3d_engine.hpp>

#include <defrt/ftn/fio.hpp>

#include <cmath>
#include <cstdio>

namespace def::cmd::drop {

namespace {

// @data 140706690  engine status flag passed to every engine call of 1403722fc; it is reset to 0
// only on the moving-body path, so it keeps its value between calls
i32 g_flag_140706690 = 0;

// RT(k, g) (1-based k) of RT group g: 200 doubles per group
inline double rt_at(const double* rt, i32 g, i32 k) { return rt[200 * (g - 1) + (k - 1)]; }
// IT(k, b) (1-based k) of body b: 20 ints per body
inline i32* it_of(i32* it, i32 b) { return it + 20 * (b - 1); }

} // namespace

// @src 1403722fc  engine calls of one detection: cube build for the fixed body (mode 1) and the
// moving body, then (mode 2, nb > 1) the drop pipeline 14005150d, the decision 14005190e per
// node and the result extraction 140373650
void fun_1403722fc(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* mode, i32* sw, i32* fid, double* nrm, double* xe, i32* flags,
                   double* vn, double* tolscale, i32* nb, i32* nb2, i32* flag)
{
    using namespace ctc3d;
    const i32 nbb = *nb2;
    const long long stride = nbb < 0 ? 0 : nbb; // PT pair stride
    i32 nvert = 4;
    i32 zero = 0;

    for (i32 b = 1; b <= *nb; ++b) {
        i32* itb = it_of(it, b);
        if (itb[0] == 0) continue;
        i32 obj = itb[8];
        const i32 t = ftn_nint(rt_at(rt, obj, 1));
        i32 first = itb[4];
        i32 cnt = (itb[5] - first) + 1;
        i32 inst = 0;
        i32 nface = 0;
        if (t == 3 && *mode != 1) {
            // 140372431..: only nface and the flag survive (inst and the IT(13/14) copies are
            // overwritten below)
            nface = (itb[7] - itb[6]) + 1;
            g_flag_140706690 = 0;
            inst = 3;
        }
        if (t > 1 && *mode != 1) {
            inst = 2;
            const i32 fface = ftn_nint(rt_at(rt, obj, 5));
            nface = ftn_nint(rt_at(rt, obj, 4));
            nvert = 4;
            if (*flag == 1) {
                zero = 0;
                fun_14004f56b(&obj, &inst, x + 3 * (first - 1), f + 4 * (fface - 1), &nvert, &first,
                              &cnt, &nface, &zero, &g_flag_140706690);
            } else {
                i32 a = 1, c = 2;
                i32 z = 0;
                fun_1400407e7(&obj, &inst, &a, &c, &cnt, &nface, &z, &g_flag_140706690);
            }
        }
        if (t < 3 && *mode != 2) {
            inst = 1;
            i32 nu = ftn_nint(rt_at(rt, obj, 6));
            const i32 fnode = ftn_nint(rt_at(rt, obj, 7));
            i32 nuf = ftn_nint(rt_at(rt, obj, 8));
            const i32 fface = ftn_nint(rt_at(rt, obj, 9));
            i32 base = 1;
            nvert = 4;
            zero = 0;
            // 140372880: the bases are XU + (first node - 1) doubles and FU + (first face - 1)
            // ints, as compiled (both offsets are 0 for the drop)
            fun_14004f56b(&obj, &inst, xu + (fnode - 1), fu + (fface - 1), &nvert, &base, &nu,
                          &nuf, &zero, &g_flag_140706690);
        }
    }

    if (*mode != 1 && nbb > 1) {
        i32 n1000 = 1000;
        fun_1403735bd(mp<i32>(0x14077191c), &n1000);
        i32 inst = 2;
        i32 one_a = 1;  // DAT_1407066c8
        i32 one_b = 1;  // DAT_1407066cc
        double tol0 = 0.0; // DAT_1407066d0
        dat_140778684[0] = *nb;
        for (i32 o = 1; o <= nbb; ++o) {
            dat_140778684[o] = 0;
            for (i32 i = 1; i <= 2; ++i) {
                dat_14076b6a8[i * 300 + o * 3] = 0;
                dat_14076e588[i * 300 + o * 3] = 0;
                dat_14076aa28[o + i * 100] = 0;
                dat_140771788[o + i * 100] = 0;
            }
        }
        for (i32 b = 1; b <= *nb; ++b) {
            i32* itb = it_of(it, b);
            i32 obj = itb[8];
            const i32 t = ftn_nint(rt_at(rt, obj, 1));
            dat_14077867c = 0;
            if (!(itb[1] > 0 && t == 3)) continue;
            const i32 last = itb[5];
            const i32 first0 = itb[4];
            inst = 2;
            const i32 nnb = (last - first0) + 1;
            const i32 nfb = ftn_nint(rt_at(rt, obj, 4));
            m<i32>(0x14076c008 + 12 * obj) = nnb;
            m<i32>(0x14076eee8 + 12 * obj) = nfb;
            for (i32 j = 1; j <= *nb; ++j) {
                i32* itj = it_of(it, j);
                const i32 o = itj[8];
                const double* pair = pt + 20 * (((long long)(o - 1) * stride + obj) - 1);
                if (ftn_nint(pair[0]) != 1) continue;
                ++dat_14077867c;
                const bool body3 = ftn_nint(rt_at(rt, o, 1)) == 3;
                m<i32>(0x140771918 + 4 * dat_14077867c) = o;
                if (body3) {
                    dat_140778684[o] = 2;
                    m<i32>(0x14076c008 + 12 * o) = (itj[5] - itj[4]) + 1;
                    m<i32>(0x14076eee8 + 12 * o) = ftn_nint(rt_at(rt, o, 4));
                } else {
                    dat_140778684[o] = 1;
                    m<i32>(0x14076ea38 + 12 * o) = ftn_nint(rt_at(rt, o, 8));
                    m<i32>(0x14076bb58 + 12 * o) = ftn_nint(rt_at(rt, o, 6));
                }
                if (ftn_nint(pair[1]) == 1) {
                    if (body3) dat_14076ad48[o] = 1;
                    else dat_14076abb8[o] = 1;
                }
            }
            if (dat_14077867c <= 0) continue;
            inst = 2;
            i32 firstm1 = first0 - 1; // DAT_1407066ec
            i32 ndof = 3;
            i32 z14 = 0;              // DAT_1407066f0
            i32 z11 = 0;              // DAT_1407066a8
            i32 first = first0;       // DAT_14070667c
            i32 cnt = nnb;            // DAT_140706680
            i32 nface = nfb;          // DAT_14070668c
            i32 lastv = last;         // DAT_1407066e8
            ctc3d::fun_14005150d(&obj, &inst, x + 3 * (first0 - 1), bc + 3 * (first0 - 1), &ndof,
                                 &first, &cnt, &nface, &one_b, &one_a, &z11, &g_flag_140706690,
                                 &tol0, &z14);
            const i32 base = first; // 140372b3c: read after the call
            i32 idx = first - firstm1;
            for (i32 n = first; n <= last; ++n, ++idx) {
                if (!(flags[n - 1] > 0)) continue;
                const double* v = vn + 3 * (n - 1);
                const double q = (v[2] * v[2]) + ((v[1] * v[1]) + (v[0] * v[0]));
                double tol = std::sqrt(q) * *tolscale; // DAT_140706708
                z11 = 0;
                ndof = 3;
                i32 node = idx; // DAT_1407066f8
                fun_14005190e(&obj, &inst, x + 3 * (base - 1), bc + 3 * (base - 1), &ndof, &node,
                              &one_b, &one_a, &z11, &g_flag_140706690, &tol);
            }
            if (*mode == 2) {
                fun_140373650(&obj, &first, &lastv, &inst, bc, sw, flags, nrm, fid, xe, x,
                              &dat_140778684[1], &firstm1);
            }
        }
    }
}

// @src 1403704c9  detection: back up X, insert/update and decide, restore X (node moves of the
// decision are undone), signed gaps 140370a26, count the contact nodes
void fun_1403704c9(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* tolscale, i32* nb, i32* nb2, i32* nn, i32* nodes, double* gap,
                   double* xb, double* w, double* kc, double* damp, double* p15, i32* nc,
                   i32* flag, i32* verbose)
{
    (void)w; (void)kc; (void)damp; (void)p15;
    const i32 n = *nn;
    i32 cnt = n * 3;
    fun_140375025(x, xb, &cnt);
    i32 mode = 1;
    if (*flag == 1) {
        mode = 1;
        fun_1403722fc(rt, pt, x, xu, it, f, fu, bc, &mode, sw, fid, nrm, xe, flags, vn, tolscale,
                      nb, nb2, flag);
    }
    mode = 2;
    fun_1403722fc(rt, pt, x, xu, it, f, fu, bc, &mode, sw, fid, nrm, xe, flags, vn, tolscale, nb,
                  nb2, flag);
    cnt = n * 3;
    fun_140375025(xb, x, &cnt);
    fun_140370a26(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, gap, nb, nb2, nodes);
    *nc = 0;
    for (i32 k = 1; k <= *nn; ++k) {
        const i32* c = bc + 3 * (k - 1);
        if (c[0] < 0 || c[1] < 0 || c[2] < 0) *nc = *nc + 1;
    }
    if (*verbose > 0) {
        // 140370a0c: list-directed WRITE ('Number of Contact points = ', nc); verbose is 0 from
        // OBJPOS, so this is unreachable there
        std::printf(" Number of Contact points = %12d\n", *nc);
    }
}

} // namespace def::cmd::drop

namespace def::cmd::ctc3d {

// @src 140049361  constraint rows (14004946f) for every node 1..*n
void fun_140049361(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* n, i32* p7, i32* p8,
                   i32* p9)
{
    g_node_14066fbc0 = 1;
    if (0 < *n) {
        do {
            i32 node = g_node_14066fbc0; // DAT_14066fbc4
            fun_14004946f(x + 3 * (g_node_14066fbc0 - 1), obj, inst, bc, ndof, &node, p7, p8, p9);
            g_node_14066fbc0 = g_node_14066fbc0 + 1;
        } while (g_node_14066fbc0 <= *n);
    }
}

// @src 14005150d  drop variant of the GENCTC 3D pipeline: stages 1-2, constraint rows, then
// (unless NO_DIEGEO.DAT exists in the working directory) stages 3-7
void fun_14005150d(i32* obj, i32* inst, double* x, i32* bc, i32* ndof, i32* first, i32* nn,
                   i32* nf, i32* p9, i32* p10, i32* p11, i32* flag, double* tol, i32* p14)
{
    (void)first; (void)p9; (void)p10;
    fun_14005c3a7(obj, inst, nf, bc, ndof);
    fun_140052f49(obj, inst, nn, bc, ndof);
    fun_140049361(x, obj, inst, bc, ndof, nn, &dat_140676710, p11, flag);
    if (g_no_diegeo == -1) return;
    if (g_no_diegeo == 0) {
        // 14005158e: OPEN(91, FILE='NO_DIEGEO.DAT', STATUS='OLD', ERR=...) then CLOSE(91). The
        // ERR branch (file absent) sets 1; a successful open sets -1 and returns. [inferred: an
        // existing readable file is what makes STATUS='OLD' succeed]
        if (rt::ftn::fio::open_old(91, "NO_DIEGEO.DAT")) {
            rt::ftn::fio::close(91);
            g_no_diegeo = -1;
            return;
        }
        g_no_diegeo = 1;
    }
    fun_140029d47(obj, inst, nn, nf);
    fun_14002cc15(obj, inst, nf, bc, ndof);
    dat_140676718 = 0;
    fun_14003b304(x, obj, inst, nn, nf, bc, ndof, &dat_140676718, tol, p11, p14);
    fun_140088e99();
    i32 z = 0;
    fun_14003fadc(obj, inst, nn, nf, &z);
}

} // namespace def::cmd::ctc3d
