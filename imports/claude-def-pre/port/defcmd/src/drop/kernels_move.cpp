// Drop solver kernels (packet p67-3): contact stiffness and the rigid move.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.7, §5.13 (move). FP order from the listing
// (out/p67/drop-solver/L_all.txt). The statics of both routines (0x140705720..0x1407058f8,
// 0x140705918..0x140705930) are written before they are read in every call: locals here.
#include <defcmd/drop/kernels.hpp>
#include <defrt/stdlib/libm.hpp>

#include <cmath>
#include <cstdint>

namespace def::cmd::drop {

namespace {
constexpr double kPiApprox = 3.1415926535898;       // DAT_140bafc20 (not pi)
constexpr double kTwo52 = 4503599627370496.0;       // DAT_140bafc28
constexpr double kTenthF = 0.10000000149011612;     // DAT_140bafc40 = (double)0.1f
} // namespace

// @src 140367fe2  k = ((((e + ((m*0.5)*|v|)*|v|) / p15) / p15) + W) * 5.0 with the rotational
// energy e = ((Lw3*wz) + ((Lw2*wy) + (Lw1*wx))) * 0.5, Lw = I w (140367df1);
// damp = ((p15*0.1f)*k)/|v| if |v| > 0, sqrt(k*m)*0.1f if |v| == 0, unchanged if |v| is NaN
void fun_140367fe2(double* k, double* damp, double* w, double* p15)
{
    const double* v = g_body.v;
    const double vabs = std::sqrt((v[2] * v[2]) + ((v[1] * v[1]) + (v[0] * v[0]))); // 140368084
    double lw[3];
    i32 three = 3;
    fun_140367df1(g_body.inertia, g_body.w, lw, &three);
    const double* om = g_body.w;
    const double e = ((lw[2] * om[2]) + ((lw[1] * om[1]) + (lw[0] * om[0]))) * 0.5;
    const double ke = ((g_body.mass * 0.5) * vabs) * vabs;
    const double p = *p15;
    const double kk = ((((e + ke) / p) / p) + *w) * 5.0; // DAT_140bafc38
    *k = kk;
    if (vabs > 0.0) { // 140368153: JP/JA
        *damp = ((p * kTenthF) * kk) / vabs;
        return;
    }
    if (vabs == 0.0) // 140368171: JP/JNZ return
        *damp = std::sqrt(*k * g_body.mass) * kTenthF;
}

// @src 140367448  rigid move of n points: ang reduced in place modulo 3.1415926535898,
// R = (Rz Ry) Rx (140367c86), X(:,j) = to + R (X(:,j) - from) (140367df1);
// track > 0 also moves A1..A3 and CT of g_body
void fun_140367448(double* x, double* from, double* to, double* ang, i32* n, i32* track)
{
    for (int k = 0; k < 3; ++k) {
        const double q = ang[k] / kPiApprox;
        // 1403674ce: CMPNLTSD selects q itself when !(|q| < 2^52) (also for NaN), else the
        // 64-bit truncation (CVTTSD2SI RCX)
        const double t = !(std::fabs(q) < kTwo52) ? q : static_cast<double>(static_cast<std::int64_t>(q));
        ang[k] = ang[k] - (t * kPiApprox);
    }
    double s = 0.0;
    double c = 0.0;
    double rx[9] = {};
    rx[0] = 1.0;
    def::rt::libm::ps_sincos(ang[0], &s, &c);
    rx[5] = s;
    rx[7] = -s;
    rx[8] = c;
    rx[4] = c;
    double ry[9] = {};
    ry[4] = 1.0;
    def::rt::libm::ps_sincos(ang[1], &s, &c);
    ry[6] = s;
    ry[2] = -s;
    ry[8] = c;
    ry[0] = c;
    double rz[9] = {};
    rz[8] = 1.0;
    def::rt::libm::ps_sincos(ang[2], &s, &c);
    rz[1] = s;
    rz[3] = -s;
    rz[4] = c;
    rz[0] = c;
    i32 three = 3;
    double m[9];
    fun_140367c86(rz, ry, m, &three);
    three = 3;
    double r[9];
    fun_140367c86(m, rx, r, &three);

    auto move_point = [&](double* p) {
        double d[3];
        for (int i = 0; i < 3; ++i) d[i] = p[i] - from[i];
        double y[3];
        i32 n3 = 3;
        fun_140367df1(r, d, y, &n3);
        for (int i = 0; i < 3; ++i) p[i] = to[i] + y[i];
    };
    for (i32 j = 1; j <= *n; ++j)
        move_point(x + 3 * static_cast<std::ptrdiff_t>(j - 1));
    if (*track > 0) {
        for (int k = 0; k < 3; ++k)
            move_point(g_body.a[k]);
        move_point(g_body.ct);
    }
}

} // namespace def::cmd::drop
