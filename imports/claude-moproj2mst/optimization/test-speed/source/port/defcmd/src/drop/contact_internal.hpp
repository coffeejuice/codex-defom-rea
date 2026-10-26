// Shared inline parts of the drop contact routines 140370a26 and 140371339 (the binary repeats
// the same code in both). Internal to src/drop.
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

namespace drop_detail {

// part of @src 140370a26 / 140371339 (the same inline code in both): last negative slot of
// BC(:,n) (3 beats 2 beats 1), 0 when none
inline i32 last_negative_slot(const i32* c)
{
    i32 s = 0;
    if (c[0] < 0) s = 1;
    if (c[1] < 0) s = 2;
    if (c[2] < 0) s = 3;
    return s;
}

// part of @src 140370a26 / 140371339: contact point Q on face f of the fixed body at (xi, eta);
// shape functions as in the listing (140371299.., 140370fb3..), Q from +0.0, Q_c = (N_k*X_c) + Q_c
inline void face_point(const double* xu, const i32* fu, i32 f, double xi, double eta, double* q)
{
    const i32* fn = fu + 4LL * (f - 1);
    i32 nv = 4;
    if (fn[0] == fn[3]) nv = 3;
    if (fn[3] == 0) nv = 3;
    double xv[12]; // DAT_140706300 (140370a26) / DAT_1407064a0 (140371339)
    for (i32 k = 1; k <= nv; ++k) {
        const double* src = xu + 3LL * (fn[k - 1] - 1);
        xv[3 * (k - 1)] = src[0];
        xv[3 * (k - 1) + 1] = src[1];
        xv[3 * (k - 1) + 2] = src[2];
    }
    double sh[4]; // DAT_140706360 / DAT_140706500
    if (nv == 4) {
        const double a = (1.0 - xi) * 0.25; // DAT_140bafc10
        const double ep = eta + 1.0;
        const double em = 1.0 - eta;
        const double xp = xi + 1.0;
        sh[3] = ep * a;
        sh[0] = a * em;
        const double b = xp * 0.25;
        sh[2] = ep * b;
        sh[1] = b * em;
    } else {
        sh[2] = (1.0 - xi) - eta;
        sh[0] = xi;
        sh[1] = eta;
    }
    q[0] = 0.0;
    q[1] = 0.0;
    q[2] = 0.0;
    for (i32 k = 1; k <= nv; ++k) {
        const double w = sh[k - 1];
        q[0] = (w * xv[3 * (k - 1)]) + q[0];
        q[1] = (w * xv[3 * (k - 1) + 1]) + q[1];
        q[2] = (w * xv[3 * (k - 1) + 2]) + q[2];
    }
}

} // namespace drop_detail

} // namespace def::cmd::drop
