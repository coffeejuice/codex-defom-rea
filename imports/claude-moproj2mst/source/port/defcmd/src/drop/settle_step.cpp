// OBJPOS drop: rigid update / step-size search of the settling solver (packet p67-3).
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.13 with the corrections of
// docs/re/commands/OBJPOS_drop_settle_notes.md. Control flow, comparisons (COMISD + JP/Jcc or
// SETNC/JA/JNC: every test is false for NaN) and FP order from the listing
// 0x14036d3fd..0x1403704c9 (out/p67/drop-solver/L_all.txt; constants from out/p673/d/L2.txt).
//
// Statics of the binary (0x140705fb0..0x140706250): all written before they are read in each
// call, so they are C++ locals, including h0 (1407061c0): wmax (140706180) starts at 0.0 and is
// only raised by ordered '>' tests, so it is never NaN and never negative, and one of the two
// branches that write h0 always runs. The trial array 140705fb0 (allocated and freed in the call)
// is a local vector. 1407061e4 (interrupt flag, never written) is in globals_d.hpp. Written-only
// statics are dropped: 140706170 (accept kind), 140706178, 140706108, 140706138, 140706250,
// 14070624c, 140706230 when mode > 0.
#include <defcmd/drop/globals_d.hpp>
#include <defcmd/drop/kernels.hpp>
#include <defcmd/drop/settle.hpp>

#include <cmath>
#include <cstdint>
#include <vector>

namespace def::cmd::drop {

namespace {

constexpr double k_sin2_1deg = 3.0458649045213493e-4; // DAT_140bafcf8
constexpr double k_minus_one = -1.0;                  // DAT_140baf950
constexpr double k_axis_tol = 1.0e-10;                // DAT_140bafc18
constexpr double k_deg = 0.017453292519943295;        // DAT_140bafca0
constexpr double k_r_full = 130.0;                    // DAT_140bafd10
constexpr double k_r_empty = 90.0;                    // DAT_140bafd18
constexpr double k_r_override = 50.0;                 // DAT_140bafd28
constexpr double k_bis = 0.0010000000474974513;       // DAT_140bafd20 = (double)0.001f
constexpr double k_half = 0.5;                        // DAT_140baf918
constexpr double k_big = 1.0e10;                      // DAT_140bafc50

// part of @src 14036d3fd: l = sqrt(dot(a, a)); if l > 0: a_k = a_k / l
void unit3(double* a)
{
    i32 three = 3;
    double l = 0.0;
    fun_140367f0b(a, a, &l, &three);
    l = std::sqrt(l);
    if (l > 0.0) {
        a[0] = a[0] / l;
        a[1] = a[1] / l;
        a[2] = a[2] / l;
    }
}

// part of @src 14036d3fd: contact node list (any BC(:,n) < 0) of nodes 1..nn into lst
i32 contact_list(const i32* bc, i32 nn, i32* lst)
{
    i32 c = 0;
    for (i32 n = 1; n <= nn; ++n) {
        const i32* b = bc + 3 * (n - 1);
        if (b[0] < 0 || b[1] < 0 || b[2] < 0) {
            c = c + 1;
            lst[c - 1] = n;
        }
    }
    return c;
}

// part of @src 14036d3fd: rotation about the axis: a = w * h; q = dot(ax, a); out = |q| * ax
void axis_rotation(double* ax, double h, double* out)
{
    double a[3] = {g_body.w[0] * h, g_body.w[1] * h, g_body.w[2] * h}; // 1407061a0
    i32 three = 3;
    double q = 0.0;
    fun_140367f0b(ax, a, &q, &three);
    const double aq = std::fabs(q);
    out[0] = aq * ax[0];
    out[1] = aq * ax[1];
    out[2] = aq * ax[2];
}

} // namespace

// @src 14036d3fd  rigid update with step-size search (also calls 14001cd40, an empty stub: not
// ported, its flag 1407061e4 stays 0)
void fun_14036d3fd(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* h, i32* nb, i32* nb2, i32* nn, i32* nodes, i32* ins, double* gap,
                   double* xb, double* d, double* w, double* kc, double* damp, double* p15,
                   double* /*ea0*/, double* /*db0*/, double* /*dd0*/, i32* nk, double* k,
                   i32* l840, i32* l890, i32* l8e0, i32* nv, double* v, double* g, i32* redrop,
                   i32* verbose)
{
    i32 cnt = 0; // 140706010
    if (*nn >= 1) cnt = contact_list(bc, *nn, l840);
    if (cnt == 0) { // 0x14036d4e3: lost all contacts, drop again
        fun_140368889(rt, pt, x, xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, nb, nb2, nodes,
                      ins, redrop, d, p15);
        return;
    }

    i32 three = 3;
    i32 mode = 0;    // 140706024
    double p1[3]{};  // 140706030
    double ax[3]{};  // 140706070
    double r[3];     // 140706050
    double q = 0.0;  // 140706090
    bool to_free = false; // jump to 0x14036e3f3 (skip the constraint block)
    const double* x1 = x + 3 * (l840[0] - 1);
    if (cnt == 1) {
        mode = 1;
        for (int a = 0; a < 3; ++a) p1[a] = x1[a];
        for (int a = 0; a < 3; ++a) r[a] = g_body.c[a] - x1[a];
        fun_140366881(r, d, ax);
        unit3(ax);
    } else if (cnt == 2) {
        mode = 2;
        const double* x2 = x + 3 * (l840[1] - 1);
        for (int a = 0; a < 3; ++a) p1[a] = x1[a];
        for (int a = 0; a < 3; ++a) r[a] = x1[a];
        double p2[3] = {x2[0], x2[1], x2[2]}; // 1407060a0
        for (int a = 0; a < 3; ++a) ax[a] = p2[a] - r[a];
        unit3(ax);
        for (int a = 0; a < 3; ++a) r[a] = g_body.c[a] - x1[a];
        double cr[3]; // 1407060c0
        fun_140366881(r, d, cr);
        fun_140367f0b(ax, cr, &q, &three);
        if (q <= 0.0)
            for (int a = 0; a < 3; ++a) ax[a] = -ax[a];
    } else { // cnt >= 3 (cnt < 3 would go to 0x14036e3f3: dead)
        mode = cnt;
        const double* x2 = x + 3 * (l840[1] - 1);
        for (int a = 0; a < 3; ++a) p1[a] = x1[a];
        for (int a = 0; a < 3; ++a) r[a] = x1[a];
        double p2[3] = {x2[0], x2[1], x2[2]};
        for (int a = 0; a < 3; ++a) ax[a] = p2[a] - r[a];
        unit3(ax);
        for (i32 i = 3; i <= cnt; ++i) {
            const double* xi = x + 3 * (l840[i - 1] - 1);
            double e[3] = {xi[0] - r[0], xi[1] - r[1], xi[2] - r[2]}; // 1407060c0
            unit3(e);
            double ce[3]; // 1407060e0
            fun_140366881(ax, e, ce);
            double qq = 0.0; // 140706100
            fun_140367f0b(ce, ce, &qq, &three);
            if (std::fabs(qq) > k_sin2_1deg) { // not collinear: free motion
                mode = 0;
                to_free = true;
                break;
            }
        }
        if (!to_free) {
            for (int a = 0; a < 3; ++a) r[a] = g_body.c[a] - x1[a];
            double cr[3];
            fun_140366881(r, d, cr);
            fun_140367f0b(ax, cr, &q, &three);
            if (q <= 0.0)
                for (int a = 0; a < 3; ++a) ax[a] = -ax[a];
            if (cnt < 1) to_free = true; // 0x14036e356 (dead)
        }
    }
    if (!to_free && *nk > 0) { // 0x14036e35e: n = K(4:6,1), also for nk = 3
        double kn[3] = {k[3], k[4], k[5]}; // 140706110
        if (mode == 1) {
            double qk = 0.0; // 140706130
            fun_140367f0b(kn, ax, &qk, &three);
            for (int a = 0; a < 3; ++a) ax[a] = kn[a] * qk;
        } else if (mode > 1) {
            double qk = 0.0;
            fun_140367f0b(kn, ax, &qk, &three);
            qk = std::fabs(std::fabs(qk) + k_minus_one);
            if (qk > k_axis_tol) mode = 0;
        }
    }

    // 0x14036e3f3: mode-9 vectors
    if (*nv > 0 && mode > 0) {
        double q1 = 0.0; // 140706140
        fun_140367f0b(v, ax, &q1, &three);
        double t1[3] = {v[0] * q1, v[1] * q1, v[2] * q1}; // 140706110
        double q2 = 0.0;
        fun_140367f0b(v + 3, ax, &q2, &three);
        double t2[3] = {v[3] * q2, v[4] * q2, v[5] * q2}; // 140706150
        for (int a = 0; a < 3; ++a) ax[a] = t1[a] + t2[a];
        unit3(ax);
    }

    // 0x14036e6c2: trial array 140705fb0 (3 * max(nn, 0) doubles)
    const i32 nnv = *nn < 0 ? 0 : *nn;
    std::vector<double> xt(static_cast<std::size_t>(nnv) * 3u);

    // step size from the angular velocity
    double wr[3] = {g_body.w[0], g_body.w[1], g_body.w[2]}; // 140706188
    double wmax = 0.0;                                       // 140706180
    if (mode > 0) {
        double a[3] = {g_body.w[0], g_body.w[1], g_body.w[2]}; // 1407061a0
        double qw = 0.0;
        fun_140367f0b(ax, a, &qw, &three);
        const double aq = std::fabs(qw);
        for (int i = 0; i < 3; ++i) a[i] = aq * ax[i];
        for (int i = 0; i < 3; ++i) wr[i] = a[i];
    }
    if (std::fabs(wr[0]) > 0.0) wmax = std::fabs(wr[0]);
    if (std::fabs(wr[1]) > wmax) wmax = std::fabs(wr[1]);
    if (std::fabs(wr[2]) > wmax) wmax = std::fabs(wr[2]);
    double h0 = 0.0; // 1407061c0 (always written: see the file comment)
    if (wmax > 0.0) {
        h0 = (*g * k_deg) / wmax;
    } else if (wmax == 0.0) {
        h0 = *h;
        g_body.w[0] = 0.0;
        g_body.w[1] = 0.0;
        g_body.w[2] = 0.0;
        mode = 0;
    }

    i32 ntry = 0;     // 1407061e0
    double hi = 0.0;  // 1407061d0 upper bracket
    double hc = h0;   // 1407061c8 current step
    double lo = 0.0;  // 1407061d8 lower bracket
    double s = 0.0;   // 140706230 accepted step (mode 0)
    double rr = 0.0;  // 140706220 rotation ratio of the last trial
    i32 cntn = 0;     // 14070621c trial contact count
    bool axis_accept = false;

    for (;;) { // 0x14036ebc5
        if (g_intr_1407061e4 == 1) return; // 14001cd40 never sets it (dead)
        ntry = ntry + 1;
        if (*verbose > 0) {
            // verbose-only, unreachable from OBJPOS: 0x14036ec0e 'ITR' line (format 140bafd00)
            // with ntry, hc, hi, lo
        }
        i32 n3 = *nn * 3;
        fun_140375025(x, xt.data(), &n3);
        i32 track0 = 0;
        if (mode == 0) { // 0x14036fcd9
            double o[3];  // 1407061f0
            double a[3];  // 1407061a0
            for (int i = 0; i < 3; ++i) o[i] = (g_body.v[i] * hc) + g_body.c[i];
            for (int i = 0; i < 3; ++i) a[i] = g_body.w[i] * hc;
            fun_140367448(xt.data(), g_body.c, o, a, nn, &track0);
        } else {
            double o[3] = {p1[0], p1[1], p1[2]};
            double a[3];
            axis_rotation(ax, hc, a);
            fun_140367448(xt.data(), o, o, a, nn, &track0);
        }
        double tol0 = 0.0; // 140706210
        i32 nct = 0;       // 140706218
        fun_1403704c9(rt, pt, xt.data(), xu, it, f, fu, bc, sw, fid, nrm, xe, flags, vn, &tol0,
                      nb, nb2, nn, nodes, gap, xb, w, kc, damp, p15, &nct, ins, verbose);

        const double hs = hc; // XMM4
        rr = (*g * hs) / h0;
        const bool full = rr >= k_r_full; // SETNC
        cntn = 0;
        if (*nn >= 1) cntn = contact_list(bc, *nn, l890);

        bool accept = false;
        if (full) { // 0x14036f2ff
            s = hs;
            accept = true;
        } else if (cntn == 0) { // 0x14036f1d8
            if (rr >= k_r_empty) {
                s = hs;
                accept = true;
            } else {
                lo = hs;
                if (hs > hi) {
                    hc = hs + h0;
                } else { // 0x140370000
                    hc = (hs + hi) * k_half;
                    if (std::fabs(hi - hs) < (h0 * k_bis)) {
                        s = hc * k_half;
                        accept = true;
                    }
                }
            }
        } else {
            i32 nfr = 0; // 140706228
            if (cntn > 0) {
                for (i32 i = 1; i <= cntn; ++i) {
                    const i32 node = l890[i - 1];
                    i32 found = 0; // 140706244
                    for (i32 j = 1; j <= cnt; ++j)
                        if (l840[j - 1] == node) found = 1;
                    if (found == 0) {
                        nfr = nfr + 1;
                        l8e0[nfr - 1] = node;
                    }
                }
            }
            if (nfr != 0) {
                if (nfr < 1) { // 0x1403703cd (dead)
                    s = hs * k_half;
                    accept = true;
                } else {
                    double gm = k_big; // 140706238, strict <, list order
                    for (i32 i = 1; i <= nfr; ++i) {
                        const double gv = gap[l8e0[i - 1] - 1];
                        if (gv < gm) gm = gv;
                    }
                    if (gm > 0.0) { // 0x14037035f
                        s = hs * k_half;
                        accept = true;
                    } else if (gm >= -(*p15)) { // 0x140370396
                        s = hs * k_half;
                        accept = true;
                    } else { // 0x14036fc18: bisect towards lo
                        hc = (hs + lo) * k_half;
                        hi = hs;
                        if (std::fabs(hs - lo) < (h0 * k_bis)) {
                            s = hc * k_half;
                            accept = true;
                        }
                    }
                }
            } else { // 0x14036ff1c: no fresh contact
                lo = hs;
                if (hs > hi) {
                    hc = hs + h0;
                } else { // 0x14036ff61
                    hc = (hs + hi) * k_half;
                    if (std::fabs(hi - hs) < (h0 * k_bis)) {
                        s = hc * k_half;
                        accept = true;
                    }
                }
            }
        }
        if (accept) {
            axis_accept = (mode != 0);
            break;
        }
    }

    i32 track1 = 1;
    if (!axis_accept) { // 0x1403700d5
        const i32 five_cnt =
            static_cast<i32>(static_cast<std::uint32_t>(cnt) * 5u); // LEA EDI,[RBP+RBP*4]
        if (cntn > five_cnt) s = hc;
        if (rr >= k_r_override) s = hc;
        double o[3]; // 1407061f0
        for (int i = 0; i < 3; ++i) o[i] = (g_body.v[i] * s) + g_body.c[i];
        for (int i = 0; i < 3; ++i) g_body.ang[i] = g_body.w[i] * s;
        fun_140367448(x, g_body.c, o, g_body.ang, nn, &track1);
    } else { // 0x14036f31f: rotation about the axis through P1 with the current hc
        double o[3] = {p1[0], p1[1], p1[2]};
        axis_rotation(ax, hc, g_body.ang);
        fun_140367448(x, o, o, g_body.ang, nn, &track1);
    }
    // 0x14036f537: deallocate 140705fb0 (xt goes out of scope)
}

} // namespace def::cmd::drop
