// 2D GENCTC Fortran kernels: point-segment, point-arc, rotation, nearest segment, rigid master,
// element check (GENCTC.md 4.3, 4.4, 4.6). Operation order from the listings (out/p676/asm.txt
// in deform-rea-work); sqrt is SQRTSD (correctly rounded, std::sqrt), fabs/negation are the
// sign-mask ANDPD/XORPD of the original.
#include <defcmd/ctc2d_ftn.hpp>
#include <defcmd/ctc2d_msg.hpp>
#include <defcmd/ctc2d_save.hpp>

#include <cmath>
#include <cstring>

#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ftn {

void reset_ctc2d_save()
{
    g_save_1404345c0 = {};
    g_save_14043556b = {};
    g_arc_where_14070d834 = 0;
}

// @src 1404345c0  point to segment
void fun_1404345c0(double* p, const double* a, const double* b, double* len, double* d,
                   double* s, const std::int32_t* mode)
{
    auto& sv = g_save_1404345c0;
    const double ay = a[1];
    const double bx = b[0];
    const double by = b[1];
    const double ax = a[0];
    double dx = bx - ax;
    double dy = by - ay;
    const double l = std::sqrt(dy * dy + dx * dx);  // 1404345ff: (dy*dy) + (dx*dx)
    const double px0 = p[0];
    const double py0 = p[1];
    *s = 0.0;
    sv.u[0] = dx;
    const double px = px0 - ax;
    sv.u[1] = dy;
    const double py = py0 - ay;
    *len = l;
    if (1e-10 > l) {  // 14043462e COMISD 1e-10, L; JP/JBE: a NaN length takes the normal path
        // degenerate segment (14043467d)
        *len = 0.0;
        sv.p[1] = py;
        sv.where = 1;
        sv.p[0] = px;
        *d = std::sqrt(px * px + py * py);  // 140434685: (px*px) + (py*py)
        if (*mode == 2) {
            p[1] = ay;
            p[0] = ax;
        }
        return;
    }
    // 1404346e7
    const double ux = sv.u[0] / l;
    const double uy = sv.u[1] / l;
    const double t = uy * py + ux * px;  // 140434722: (uy*py) + (ux*px)
    *s = t / l;
    const double h = uy * px - ux * py;  // 140434765
    sv.p[0] = px;
    sv.p[1] = py;
    sv.u[0] = ux;
    sv.u[1] = uy;
    sv.h = h;
    sv.s = t;
    if (!(t < 0.0) && !std::isnan(t) && !(l < t)) {
        // inside (1404347ad)
        sv.where = 0;
        *d = h;
        if (*mode == 1) return;
        p[1] = uy * t + ay;  // 1404347d3: (uy*t) + ay
        p[0] = t * ux + ax;  // 1404347d8: (t*ux) + ax
        return;
    }
    // outside (140434804)
    const double qx = px0 - bx;
    const double qy = py0 - by;
    const double da = std::fabs(std::sqrt(px * px + py * py));  // 140434832: (px*px) + (py*py)
    const double dbr = std::sqrt(qy * qy + qx * qx);            // 140434837: (qy*qy) + (qx*qx)
    const double db = std::fabs(dbr);
    const bool pos = !(h < 0.0) && !std::isnan(h);  // 14043485d SETNC after COMISD h, 0
    sv.where = 1;
    sv.db = dbr;
    if (da > db) {  // 140434885 JP/JBE not taken: ordered and da > db
        sv.where = 2;
        *d = pos ? db : -db;
        if (*mode == 1) return;
        p[1] = by;
        p[0] = bx;
        return;
    }
    *d = pos ? da : -da;
    if (*mode == 1) return;
    p[1] = ay;
    p[0] = ax;
}

// @src 14043556b  rotate v by *ang
void fun_14043556b(double* v, const double* ang)
{
    double s = 0.0;
    double c = 0.0;
    def::rt::libm::ps_sincos(*ang, &s, &c);  // 1404b3b30
    const double y = v[1];
    const double x = v[0];
    const double sy = s * y;
    const double cy = c * y;
    const double sx = s * x;
    const double cx = c * x;
    g_save_14043556b.c = c;
    g_save_14043556b.s = s;
    g_save_14043556b.y = y;
    g_save_14043556b.x = x;
    v[1] = sx + cy;  // 1404355e3
    v[0] = cx - sy;  // 1404355f5
}

} // namespace def::cmd::ftn
