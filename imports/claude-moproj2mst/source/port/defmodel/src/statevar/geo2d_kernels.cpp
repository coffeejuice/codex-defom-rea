// 2D geometry helpers and Fortran curve kernels (p1-09); see defmodel/statevar/geo2d_kernels.hpp.
// The Fortran kernels follow the compiled (PathScale -O2) instruction order; constants are the
// binary's (.rodata 140baeb08.., .data 1405f3a70..).
#include <defmodel/statevar/geo2d_kernels.hpp>

#include <defmodel/globals_p1_09.hpp>
#include <defmodel/port.hpp>
#include <defmodel/topo/ftn_geom.hpp>
#include <defrt/stdlib/libm.hpp>

#include <cmath>
#include <cstdio>

namespace def::model::geo2d {

namespace {

constexpr double k_deg_to_rad = 0.017453292519943295;   // 140522fd0
constexpr double k_half = 0.5;                           // 14051f240
constexpr double k_45 = 45.0;                            // 140baeb08
constexpr double k_quarter_pi = 0.7853981633974483;      // 140baeb10
constexpr double k_one = 1.0;                            // 140baeb18
constexpr double k_three = 3.0;                          // 140baeb28
constexpr double k_two = 2.0;                            // 140baeb30
constexpr double k_half_b = 0.5;                         // 140baeb40
constexpr double k_tol_default = 1e-06;                  // 140baeb50
constexpr double k_cross_min = 1e-10;                    // 140baeb58
constexpr double k_two_52 = 4503599627370496.0;          // 140baeb78 (NINT: no fraction above)
// .data literals of the Fortran objects
constexpr double k_arc_type = 2.0;                       // 1405f3a70
constexpr double k_pi = 3.1415926535898;                 // 1405f3a78 (not the double nearest pi)
constexpr double k_cross_max = 9.999999974752427e-07;    // 1405f3ba8 (REAL*4 1e-6 widened)
constexpr double k_line_type = 1.0;                      // 1405f3bb0
constexpr double k_arc_type_b = 2.0;                     // 1405f3bb8

// The binary's libm (p1-01, defrt/stdlib/libm.hpp): Fortran runtime sincos 1404b3b30, tan
// 1404b3df0, atan2 1404b3878, acos 1404b3d58.
using def::rt::libm::ps_acos;
using def::rt::libm::ps_atan2;
using def::rt::libm::ps_sincos;
using def::rt::libm::ps_tan;

// 1404d71d7 (p1-11): rotate v by *angle (radians)
void rotate_vec(double* v, const double* angle) { topo::rotate_2d(v, angle); }

// CVTTSD2SI: truncation, 0x80000000 when out of range
std::int32_t cvttsd2si(double v)
{
    if (!(v > -2147483649.0 && v < 2147483648.0)) return INT32_MIN;
    return static_cast<std::int32_t>(v);
}

// Fortran NINT as compiled in 14032b34b: v + copysign(0.5, v) below 2^52, then truncate
std::int32_t ftn_nint(double v)
{
    const double h = !(v < 0.0) ? 0.5 : -0.5;   // 140baeb60 / 140baeb68
    const double r = (std::fabs(v) >= k_two_52 || std::isnan(v)) ? v : h + v;
    return cvttsd2si(r);
}

// point record k (1-based) and its fields
double* P(double* pts, std::int32_t k) { return pts + (static_cast<std::int64_t>(k) - 1) * 3; }
const double* P(const double* pts, std::int32_t k) { return pts + (static_cast<std::int64_t>(k) - 1) * 3; }
double* E(double* ents, std::int32_t k) { return ents + (static_cast<std::int64_t>(k) - 1) * 6; }
const double* E(const double* ents, std::int32_t k) { return ents + (static_cast<std::int64_t>(k) - 1) * 6; }

void tolerances_defaults()
{
    Common140b84720& c = g_common_140b84720;
    if (c.u08 == 0.0) c.u08 = k_tol_default;
    if (c.u00 == 0.0) c.u00 = k_tol_default;
    if (c.u10 == 0.0) c.u10 = k_one;
}

} // namespace

// @src 1403077e0  (b - a) rotated by deg degrees; MSVC CRT cos (140bb3490) and sin (140bb3000)
void rotate_difference_deg(const double* a, const double* b, double* out, double deg)
{
    const double ang = deg * k_deg_to_rad;
    double d[2];
    sub_vec(d, b, a);   // 1403089b0(tmp, b, a)
    const double x_cos = d[0] * def::rt::libm::crt_cos(ang); // 140bb3490
    out[0] = x_cos - d[1] * def::rt::libm::crt_sin(ang);   // 140bb3000
    const double x_sin = d[0] * def::rt::libm::crt_sin(ang);
    out[1] = x_sin + d[1] * def::rt::libm::crt_cos(ang);
}

// @src 140308730
void copy_vec(double* dst, const double* src)
{
    dst[0] = src[0];
    dst[1] = src[1];
}

// @src 1403089b0
void sub_vec(double* dst, const double* b, const double* a)
{
    const double x = b[0] - a[0];
    const double y = b[1] - a[1];
    dst[0] = x;
    dst[1] = y;
}

// @src 140308a20
void add_vec(double* dst, const double* a, const double* b)
{
    const double x = a[0] + b[0];
    const double y = a[1] + b[1];
    dst[0] = x;
    dst[1] = y;
}

// @src 140309550  sum of 0.5 * (x_i * y_j - y_i * x_j), j = i + 1 (0 for the last point)
double polygon_area(std::int32_t n, const double* xy)
{
    double area = 0.0;
    for (std::int32_t i = 0; i < n; ++i) {
        const double* p = xy + static_cast<std::int64_t>(i * 2);
        const double* q = xy;
        if (i != n - 1) q = xy + static_cast<std::int64_t>(i * 2 + 2);
        const double t = p[0] * q[1] - p[1] * q[0];
        area = area + k_half * t;
    }
    return area;
}

// @src 14032ab90  rotate p about c by *deg degrees
void rotate_point_deg(double* p, const double* deg, const double* c)
{
    const double cy = c[1];
    const double cx = c[0];
    const double dx = p[0] - cx;
    const double dy = p[1] - cy;
    const double d = *deg;
    p[0] = dx;
    p[1] = dy;
    double s;
    double co;
    ps_sincos((d / k_45) * k_quarter_pi, &s, &co);
    p[0] = (dx * co - dy * s) + cx;
    p[1] = (dy * co + dx * s) + cy;
}

// @src 14032ac75  entities -> points
void entities_to_points(std::int32_t* npts, double* pts, const std::int32_t* nent, const double* ents)
{
    std::int32_t i = 1;    // entity, 1-based (local_294)
    std::int32_t k = 0;    // points written (local_298)
    if (0 < *nent) {
        do {
            const double* e = E(ents, i);
            const double type = e[0];
            if (type == k_line_type) {
                double* a = P(pts, k + 1);
                double* b = P(pts, k + 2);
                a[0] = e[1];
                a[1] = e[2];
                a[2] = 0.0;
                b[0] = e[3];
                b[1] = e[4];
                b[2] = 0.0;
                k = k + 2;
            } else if (k_arc_type == type) {
                const double sweep = e[5];
                double sx = e[1];   // current piece start (0x278 / 0x280)
                double sy = e[2];
                const double cx = e[3];
                const double cy = e[4];
                const double limit = (k_pi / k_three) * k_two;
                std::int32_t pieces = 1;
                double d;
                for (;;) {
                    d = sweep / static_cast<double>(pieces);
                    if (!(limit < std::fabs(d))) break;   // COMISD: NaN takes another piece
                    if (pieces + 1 > 3) {
                        // 14032b272: WRITE(6, 1030) i; then the stop routine
                        // str 1405f3b50  1030(/,' ENTITY ',I5,' HAS TOO LARGE SWEEPING ANGLE.')
                        if (i >= -9999 && i <= 99999)
                            std::fprintf(stdout, "\n ENTITY %5d HAS TOO LARGE SWEEPING ANGLE.\n", i);
                        else
                            std::fprintf(stdout, "\n ENTITY ***** HAS TOO LARGE SWEEPING ANGLE.\n");
                        std::fflush(stdout);
                        pieces = pieces + 1;
                        if (g_stop_1403c4504) g_stop_1403c4504();
                        else def::rt::not_ported(0x1403c4504, __FILE__, __LINE__);
                        break;   // 14032b346: continues at 14032af62 if the stop returns
                    }
                    pieces = pieces + 1;
                }
                // 14032af62
                double rx = e[1] - cx;   // radius vector (0x288 / 0x290)
                double ry = e[2] - cy;
                const double r = std::sqrt(ry * ry + rx * rx);
                double t[2] = {rx / r, ry / r};   // unit radius, turned into the tangent (0x1b0)
                double step = d;                  // 0x38
                const double quarter = std::fabs(k_pi * k_half_b);
                double turn = !(step < 0.0) ? quarter : -quarter;   // CMPNLTSD: NaN counts as >= 0
                rotate_vec(t, &turn);
                const double h = std::fabs(ps_tan(step * k_half_b) * r);
                const std::int32_t n = pieces;
                if (0 < n) {
                    std::int32_t a = k + 1;   // EBX
                    std::int32_t b = k + 2;   // EBP
                    std::int32_t piece = 1;
                    do {
                        double v[2] = {rx, ry};
                        rotate_vec(v, &step);
                        double* ps = P(pts, a);
                        ps[2] = 0.0;
                        ps[1] = cy + ry;
                        ps[0] = cx + rx;
                        double* pc = P(pts, b);
                        pc[0] = h * t[0] + sx;
                        pc[2] = r;
                        pc[1] = h * t[1] + sy;
                        const double ex = cx + v[0];
                        k = k + 3;
                        double* pe = P(pts, k);
                        pe[0] = ex;
                        const double ey = cy + v[1];
                        rx = v[0];
                        ry = v[1];
                        a = a + 3;
                        b = b + 3;
                        pe[2] = 0.0;
                        pe[1] = ey;
                        rotate_vec(t, &step);
                        sx = ex;
                        sy = ey;
                        piece = piece + 1;
                    } while (piece <= n);
                }
            }
            i = i + 1;
        } while (i <= *nent);
    }
    const std::int32_t before = k;
    std::int32_t merged = 0;   // local_e8, unused afterwards
    merge_close_points(&k, pts, &merged);
    *npts = k;
    for (std::int32_t j = k + 1; j <= before; ++j) {
        double* p = P(pts, j);
        p[0] = 0.0;
        p[1] = 0.0;
        p[2] = 0.0;
    }
}

// @src 14032b34b  points -> entities
void points_to_entities(const std::int32_t* npts, const double* pts, std::int32_t* nent, double* ents)
{
    tolerances_defaults();
    const Common140b84720& g = g_common_140b84720;
    double tol_r = g.u10 * g.u08;     // 0x30 (local_158)
    double tol_len = g.u10 * g.u00;   // 0x38 (local_150)
    if (k_tol_default < tol_r) tol_r = k_tol_default;
    if (k_tol_default < tol_len) tol_len = k_tol_default;

    // XMM5 / XMM6: the corner being processed (the original loads P2 before testing npts)
    double cur_x = *npts >= 2 ? P(pts, 2)[0] : 0.0;
    double cur_y = *npts >= 2 ? P(pts, 2)[1] : 0.0;
    std::int32_t i = 2;            // 0x44
    std::int32_t k = 0;            // 0x40 entities written
    double s_x = P(pts, 1)[0];     // 0x138 / 0x140 segment start
    double s_y = P(pts, 1)[1];
    double q_x = s_x;              // 0x128 / 0x130 previous corner
    double q_y = s_y;
    // corner state kept across iterations on the stack
    double e_x = 0.0, e_y = 0.0;   // 0x50 / 0x58 segment end
    double n_x = 0.0, n_y = 0.0;   // 0x60 / 0x68 next point
    double r = 0.0;                // 0x70 corner radius
    double t_x = 0.0, t_y = 0.0;   // 0x80 / 0x88 next segment start
    double c_x = 0.0, c_y = 0.0;   // 0x90 / 0x98 fillet centre
    double sweep = 0.0;            // 0xa0 fillet angle
    std::int32_t next = 3;         // EBP

    if (*npts >= 2) {
        for (;;) {
            // 14032b6b0
            e_x = cur_x;
            e_y = cur_y;
            bool skip = false;
            if (*npts != i) {
                n_x = P(pts, next)[0];
                n_y = P(pts, next)[1];
                r = P(pts, i)[2];
                bool fillet = false;
                if (tol_r < r) {
                    // 14032b855
                    double x1 = cur_x - q_x;
                    double y1 = cur_y - q_y;
                    double x2 = n_x - cur_x;
                    double y2 = n_y - cur_y;
                    const double l1 = std::sqrt(y1 * y1 + x1 * x1);
                    const double l2 = std::sqrt(x2 * x2 + y2 * y2);
                    y1 = y1 / l1;
                    x1 = x1 / l1;
                    y2 = y2 / l2;
                    x2 = x2 / l2;
                    const double cross = y2 * x1 - x2 * y1;
                    const double ac = std::fabs(cross);
                    if (ac < k_cross_max && k_cross_min < ac) {
                        skip = true;   // 14032b942: on to the next point, corner unchanged
                    } else if (k_cross_min < ac) {
                        // 14032b9a1: fillet tangent to both segments
                        const double ang = ps_acos(y2 * y1 + x2 * x1);
                        const bool pos = cross >= 0.0;   // SETNC
                        const double ar = std::fabs(r);
                        const double sr = pos ? ar : -ar;
                        double cxv = (x2 - x1) * sr;
                        double cyv = (y2 - y1) * sr;
                        cxv = cxv / cross;
                        cyv = cyv / cross;
                        const double one = std::fabs(k_line_type);
                        const double sone = pos ? one : -one;
                        const double sgr = sone * r;
                        const double aang = std::fabs(ang);
                        sweep = pos ? aang : -aang;
                        c_x = cxv + cur_x;
                        c_y = cyv + cur_y;
                        t_x = y2 * sgr + c_x;
                        e_x = y1 * sgr + c_x;
                        e_y = c_y - x1 * sgr;
                        t_y = c_y - x2 * sgr;
                        fillet = true;
                    }
                }
                if (!skip && !fillet) {
                    // 14032b714 / 14032b95f: sharp corner
                    e_x = cur_x;
                    e_y = cur_y;
                    t_x = cur_x;
                    t_y = cur_y;
                    sweep = 0.0;
                    c_x = cur_x;
                    c_y = cur_y;
                }
            }
            if (!skip) {
                // 14032b751: the straight segment S -> E, or merge it into the previous entity
                const double dx = e_x - s_x;
                const double dy = e_y - s_y;
                const double len = std::sqrt(dy * dy + dx * dx);
                if (len > tol_len * g.u10) {
                    k = k + 1;
                    double* en = E(ents, k);
                    en[0] = k_line_type;
                    en[1] = s_x;
                    en[2] = s_y;
                    en[3] = e_x;
                    en[4] = e_y;
                } else if (0 < k) {
                    double* en = E(ents, k);
                    std::int32_t type = ftn_nint(en[0]);
                    double e_pt[2] = {e_x, e_y};
                    trim_entity_end(en + 1, en + 3, e_pt, en + 5, &type);
                    e_x = e_pt[0];
                    e_y = e_pt[1];
                }
                // 14032b5c9
                if (*npts != i) {
                    if (r > tol_r) {
                        k = k + 1;
                        double* en = E(ents, k);
                        en[0] = k_arc_type_b;
                        en[1] = e_x;
                        en[2] = e_y;
                        en[3] = c_x;
                        en[4] = c_y;
                        en[5] = sweep;
                    }
                    // 14032b656
                    s_x = t_x;
                    s_y = t_y;
                    q_x = cur_x;
                    q_y = cur_y;
                    cur_x = n_x;
                    cur_y = n_y;
                }
            }
            // 14032b698
            i = i + 1;
            next = next + 1;
            if (*npts < i) break;
        }
    }
    *nent = k;
}

// @src 14032bba1  move the end of an entity to e
void trim_entity_end(const double* p1, double* p2, double* e, double* angle, const std::int32_t* type)
{
    if (*type == 1) {
        const double y = e[1];
        p2[0] = e[0];
        p2[1] = y;
    }
    if (*type == 2) {
        const double cx = p2[0];
        const double cy = p2[1];
        double v[2] = {p1[0] - cx, p1[1] - cy};
        const double l1 = std::sqrt(v[1] * v[1] + v[0] * v[0]);
        double wx = e[0];
        double wy = e[1];
        *angle = 0.0;
        wx = wx - cx;
        wy = wy - cy;
        if (l1 == 0.0) return;   // JNZ / JP: NaN goes on
        const double l2 = std::sqrt(wy * wy + wx * wx);
        if (l2 == 0.0) return;
        const double y = ((wy * v[0] - wx * v[1]) / l1) / l2;
        const double x = ((wy * v[1] + wx * v[0]) / l1) / l2;
        *angle = ps_atan2(y, x);
        rotate_vec(v, angle);
        e[0] = cx + v[0];
        e[1] = cy + v[1];
    }
}

// @src 14032c4a0  merge close points
void merge_close_points(std::int32_t* npts, double* pts, std::int32_t* merged)
{
    tolerances_defaults();
    const Common140b84720& g = g_common_140b84720;
    dat_1405f3c00 = g.u10 * g.u08;
    dat_1405f3c08 = g.u10 * g.u00;
    if (k_tol_default < dat_1405f3c00) dat_1405f3c00 = k_tol_default;
    if (k_tol_default < dat_1405f3c08) dat_1405f3c08 = k_tol_default;
    *merged = 0;
    if (2 < *npts) {
        std::int32_t last = *npts - 1;   // R13D
        std::int32_t i = 1;
        while (i + 1 < *npts) {
            const double* a = P(pts, i + 1);
            const double* b = P(pts, i);
            const double dx = a[0] - b[0];
            const double dy = a[1] - b[1];
            if (std::sqrt(dy * dy + dx * dx) < dat_1405f3c00) {
                *merged = 1;
                for (std::int32_t j = i + 1; j <= last; ++j) {
                    double* d = P(pts, j);
                    const double* s = P(pts, j + 1);
                    d[0] = s[0];
                    d[1] = s[1];
                    d[2] = s[2];
                }
                last = last - 1;
                *npts = *npts - 1;
            } else {
                i = i + 1;
            }
        }
    }
}

} // namespace def::model::geo2d
