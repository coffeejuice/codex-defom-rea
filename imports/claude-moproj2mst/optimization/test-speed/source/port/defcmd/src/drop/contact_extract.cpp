// OBJPOS drop: extraction of the engine's contact results per node (NI4 codes and polygons ->
// FID, NRM, XE) and the local coordinates of a node on its contact polygon.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.5.2; listing out/p673/b/L_b.txt
// (140373650..14037424f, 140374252..140374953, 140374954..140375024).
#include <defcmd/drop/contact.hpp>
#include <defrt/ftn/geoutl.hpp>
#include <defcmd/drop/globals_b.hpp>
#include <defcmd/genctc3d_engine.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>

#include <cmath>
#include <cstdint>

namespace def::cmd::drop {

namespace geo = def::rt::geoutl;

namespace {

// List-directed WRITE(*,*) (unit 6) through the PathScale runtime emulation of defrt (FUN_1404b5ef4)
void ftn_write_text(const char* t) { def::rt::ftn::fio::Write(6).a(t).end(); }
void ftn_write_text_int(const char* t, i32 v) { def::rt::ftn::fio::Write(6).a(t).i(v).end(); }

// part of @src 140374252 / 140374954 (inlined there): mode 1..6 zeroes normal components, then
// n = n * (1.0 / sqrt(s)) with s = (n1*n1) + 0.0; s = s + (n2*n2); s = (n3*n3) + s
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
    const double r = 1.0 / std::sqrt(s); // DAT_140baf930 = 1.0
    n[0] = n[0] * r;
    n[1] = n[1] * r;
    n[2] = n[2] * r;
}

} // namespace

// @src 140373650  result extraction (listing L_b.txt 140373650..14037424f)
void fun_140373650(i32* obj, i32* first, i32* last, i32* inst, i32* bc, i32* sw, i32* flags,
                   double* nrm, i32* fid, double* xe, double* x, i32* insttab, i32* off)
{
    for (i32 n = *first; n <= *last; ++n) { // DAT_140706714 (the bound is re-read every pass)
        const i32 kf = flags[n - 1];         // DAT_140706718
        if (!(0 < kf)) continue;
        i32 entity = n - *off;               // DAT_14070671c
        i32 field = 4;
        ctc3d::fun_140228830(obj, inst, &entity, &field, g_x650_ni4, &g_x650_ni4_cnt);
        double* xn = x + 3 * (n - 1);
        for (i32 s = 3; s >= 1; --s) {       // DAT_140706744
            const i32 c = g_x650_ni4[s - 1];
            if (!(c < 0)) continue;
            const long long sk = 6LL * (kf - 1) + 2LL * (s - 1); // SW(1,s,kf), XE(1,s,kf)
            if (0 < sw[sk]) {
                // 1403738d4: release slots whose code equals -SW(2,s,kf) (SW is never written in
                // the drop, so this is not reached there)
                const i32 code = sw[sk + 1]; // DAT_140706748
                const i32 mc = -code;
                for (i32 s2 = 3; s2 >= 1; --s2) { // DAT_14070674c
                    i32& b = bc[3LL * (n - 1) + (s2 - 1)];
                    if (mc != b) continue;
                    if (c != b) continue;
                    double* nr = nrm + 9LL * (kf - 1) + 3LL * (s2 - 1);
                    nr[0] = 0.0;
                    nr[1] = 0.0;
                    nr[2] = 0.0;
                    b = 0;
                    double* xe2 = xe + 6LL * (kf - 1) + 2LL * (s2 - 1);
                    xe2[0] = 0.0;
                    fid[3LL * (kf - 1) + (s2 - 1)] = 0;
                    xe2[1] = 0.0;
                }
                continue;
            }
            i32 cobj = -c;                    // DAT_140706750
            i32 poly = g_x650_ni4[3 + s - 1]; // DAT_140706754
            if (!(0 < poly)) {
                // 140373ba5: list-directed WRITEs (strings 140bafd60 .. 140bafe30)
                ftn_write_text(" Fatal Error in polygon search in CNTINF");
                ftn_write_text(" ");
                ftn_write_text(" Please report this to SFTC.");
                ftn_write_text(" ");
                ftn_write_text_int(" ...Object(Boundary)   = ", *obj);
                ftn_write_text_int(" ...Node number        = ", entity);
                ftn_write_text_int(" ...Contact die number = ", cobj);
                ftn_write_text(" ");
                continue;
            }
            i32 cinst = insttab[cobj - 1];    // DAT_140706758
            fid[3LL * (kf - 1) + (s - 1)] = poly;
            i32 f1 = 1;
            ctc3d::fun_140228540(&cobj, &cinst, &poly, &f1, g_x650_er1, &g_x650_er1_cnt);
            double* nr = nrm + 9LL * (kf - 1) + 3LL * (s - 1);
            nr[0] = -g_x650_er1[0];
            nr[1] = -g_x650_er1[1];
            nr[2] = -g_x650_er1[2];
            ctc3d::fun_14004f363(&cobj, &cinst, &poly, g_x650_xp, g_x650_conn, &g_x650_cnt);
            i32 zero = 0; // DAT_140706804
            const i32 c4 = g_x650_conn[3];
            // 140373b37: NEG + CMOVS (|INT_MIN| stays INT_MIN)
            const i32 a4 = c4 < 0 ? static_cast<i32>(0u - static_cast<std::uint32_t>(c4)) : c4;
            if (g_x650_conn[0] == a4) {
                fun_140374954(g_x650_xp, xn, &g_x650_xi, &g_x650_eta, &g_x650_bad, &zero);
            } else {
                fun_140374252(g_x650_xp, xn, &g_x650_xi, &g_x650_eta, &g_x650_bad, &zero);
            }
            if (!(0 < g_x650_bad)) {
                double* xe2 = xe + sk;
                xe2[0] = g_x650_xi;
                xe2[1] = g_x650_eta;
            }
        }
    }
}

// @src 140374252  local coordinates on a quad (listing L_b.txt 140374252..140374953)
void fun_140374252(double* xp, double* p, double* xi, double* eta, i32* bad, i32* mode)
{
    double nq[3]; // DAT_140706820
    geo::fun_1404d3d43(xp, nq);
    zero_and_normalize(nq, *mode);
    double tf[9]; // DAT_140706850
    geo::fun_1404cf5c0(nq, tf);
    double q0[3]; // DAT_1407068a0
    double z0 = 0.0, z1 = 0.0;
    geo::fun_1404d6974(xp, q0, &z0, &z1);
    double pp[3]; // DAT_1407068c0
    i32 one = 1, one_b = 1;
    geo::fun_1404ded0c(p, pp, q0, tf, &one, &one_b);
    double v[12]; // DAT_1407068e0: V'(3,4)
    i32 four = 4;
    geo::fun_1404ded0c(xp, v, q0, tf, &four, &one_b);
    double dmin = 1e8; // DAT_140706940 (DAT_140bafe40)
    double nearq[3];   // DAT_140706950
    i32 edge = 0;      // DAT_140706970
    i32 idx[8] = {};   // DAT_140706974
    i32 nv = 4, p8 = 1, p10 = 0, p11 = 3;
    geo::fun_1404cc289(v, pp, nearq, &dmin, &edge, &nv, bad, &p8, idx, &p10, &p11);
    double tol = g_tol_b84720; // DAT_140706978
    if (tol == 0.0) tol = g_len_b84730 * 1e-5; // DAT_140bafcd0
    if (!(*bad < 1 || (*bad == 1 && tol >= dmin))) return;
    if (*bad == 1) *bad = 0;
    double xek[2]; // DAT_140706980
    i32 kk = 0;    // DAT_1407069a0
    i32 err = 0;   // DAT_1407069a4
    geo::fun_1404ccf58(v, pp, xek, &kk, &err);
    if (err == 0) {
        *eta = xek[1];
        *xi = xek[0];
    } else {
        // 1403746fc: node 4 of V' := node 1 (also for a quad), then the nearest edge
        v[11] = v[2];
        v[9] = v[0];
        v[10] = v[1];
        geo::fun_1404d41f8(v, pp, xi, eta, q0, &dmin);
    }
    geo::fun_1404d6974(v, nearq, xi, eta);
    double tol2 = g_tol_b84728;
    const double e = std::fabs(nearq[2] - pp[2]); // DAT_1407069a8
    if (tol2 == 0.0) tol2 = g_len_b84730 * 0.5; // DAT_140baf918
    if (e > tol2) *bad = 1;
}

// @src 140374954  local coordinates on a triangle (listing L_b.txt 140374954..140375024)
void fun_140374954(double* xp, double* p, double* xi, double* eta, i32* bad, i32* mode)
{
    double nq[3]; // DAT_1407069b0
    geo::fun_1404d3d43(xp, nq);
    zero_and_normalize(nq, *mode);
    double q0[3]; // DAT_1407069e0
    double third = 0.3333333333333333; // DAT_140605680
    double third_b = third;
    i32 three = 3;
    geo::fun_1404e34eb(xp, q0, &third, &third_b, &three);
    double tf[9]; // DAT_140706a00
    geo::fun_1404cf5c0(nq, tf);
    double pp[3]; // DAT_140706a50
    i32 one = 1, one_b = 1;
    geo::fun_1404ded0c(p, pp, q0, tf, &one, &one_b);
    double* v = g_x954_v; // DAT_140706a70
    i32 three_b = 3;
    geo::fun_1404ded0c(xp, v, q0, tf, &three_b, &one_b);
    double dmin = 1e8; // DAT_140706ad0
    double nearq[3];   // DAT_140706ae0
    i32 edge = 0;      // DAT_140706b00
    i32 idx[8] = {};   // DAT_140706b04
    i32 nv = 3, p8 = 1, p10 = 0, p11 = 3;
    geo::fun_1404cc289(v, pp, nearq, &dmin, &edge, &nv, bad, &p8, idx, &p10, &p11);
    double tol = g_tol_b84720; // DAT_140706b08
    if (tol == 0.0) tol = g_len_b84730 * 1e-5;
    if (!(*bad < 1 || (*bad == 1 && tol >= dmin))) return;
    if (*bad == 1) *bad = 0;
    double b3 = 0.0; // DAT_140706b10
    i32 bad2 = 0;    // DAT_140706b18
    geo::fun_1404e2d44(v, pp, xi, eta, &b3, &bad2);
    if (bad2 != 0) {
        v[10] = v[1];
        v[9] = v[0];
        v[11] = v[2];
        geo::fun_1404d41f8(v, pp, xi, eta, q0, &dmin);
    }
    i32 three_c = 3;
    geo::fun_1404e34eb(v, nearq, xi, eta, &three_c);
    double tol2 = g_tol_b84728;
    const double e = std::fabs(nearq[2] - pp[2]); // DAT_140706b20
    if (tol2 == 0.0) tol2 = g_len_b84730 * 0.5;
    if (e > tol2) *bad = 1;
}

} // namespace def::cmd::drop
