// 2D arc contact refinement of the OBJPOS interference arc kernel (OBJPOS_interference_kernels.md
// 4.5), packet p67-6. Transcribed from the listings (deform-rea-work out/p676/asm.txt); operation
// order as compiled. sqrt is SQRTSD (std::sqrt), fabs / negation are the ANDPD / XORPD sign masks
// (DAT_140baec70 / DAT_140baec78). Fortran ASIN (1404b3d50 = 1404b3ba4 with k = 0, i.e. asin, not
// acos), SIN (1404b3ab0) and SINCOS (1404b3b30) come from the PathScale runtime port.
#include <defcmd/arc_refine.hpp>

#include <bit>
#include <cmath>

#include <defmodel/topo/ftn_geom.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ftn {

namespace {

constexpr double kPi = 3.1415926535898;   // 1405f6298 (0x400921fb54442d28, not exact pi)
constexpr double kTwo = 2.0;              // 140baecb0
constexpr double kHalf = 0.5;             // 140baec88
constexpr double kOne = 1.0;              // 140baec30
constexpr double kMinusOne = -1.0;        // 140baec38
constexpr double kAngTol = 1e-4;          // 1405f6290
static_assert(std::bit_cast<std::uint64_t>(kPi) == 0x400921fb54442d28ull);
static_assert(std::bit_cast<std::uint64_t>(kAngTol) == 0x3f1a36e2eb1c432dull);

// A(row, col) of the 2 x 12 table, 1-based; flat(k) is the k-th double from 140702540
double& a1(Save14034151e& sv, std::int32_t col) { return sv.a[2 * (col - 1)]; }
double& a2(Save14034151e& sv, std::int32_t col) { return sv.a[2 * (col - 1) + 1]; }
double& flat(Save14034151e& sv, std::int32_t k) { return sv.a[k - 1]; }

} // namespace

void reset_arc_refine_save()
{
    g_save_140341100 = {};
    g_save_14034151e = {};
}

// @src 140341100  arc refinement driver: frame of the ray, ordered end points, 14034151e
void fun_140341100(const double* s, double* e, const double* c, const double* phi,
                   const double* vel, const double* q, const double* dir, const double* r,
                   double* g, std::int32_t* res)
{
    auto& sv = g_save_140341100;
    const double dx = dir[0] - vel[0];
    sv.d[0] = dx;
    const double dy = dir[1] - vel[1];
    const double len = std::sqrt(dy * dy + dx * dx);  // 14034114b: (dy*dy) + (dx*dx)
    e[0] = s[0] - c[0];
    e[1] = s[1] - c[1];
    sv.d[1] = dy;
    sv.len = len;
    const double rad = std::sqrt(e[1] * e[1] + e[0] * e[0]);  // 140341195
    const double uy = dy / len;
    const double ux = dx / len;
    sv.u[1] = uy;
    sv.u[0] = ux;
    sv.rad = rad;
    fun_1404d71d7(e, phi);
    e[0] = e[0] + c[0];  // 140341272
    e[1] = e[1] + c[1];
    const double qx = q[0];
    const double qy = q[1];
    sv.flag = 1;
    const double sx = s[0] - qx;
    const double ey = e[1] - qy;
    const double ex = e[0] - qx;
    const double sy = s[1] - qy;
    const double cy = c[1] - qy;
    const double cx = c[0] - qx;
    const double s_par = sy * uy + sx * ux;   // 14034131f
    const double s_perp = sy * ux - sx * uy;  // 140341332
    const double e_par = ey * uy + ex * ux;   // 140341345
    const double e_perp = ux * ey - uy * ex;  // 140341372
    const double c_perp = cy * ux - cx * uy;  // 14034135e
    const double c_par = cy * uy + cx * ux;   // 140341377
    const double ph = *phi;
    sv.p1[0] = s_par;
    sv.p1[1] = s_perp;
    sv.phi = ph;
    sv.p2[0] = e_par;
    sv.p2[1] = e_perp;
    sv.cf[1] = c_perp;
    sv.cf[0] = c_par;
    if (s_par > e_par) {  // 140341368: COMISD, JP/JBE skip
        sv.p2[1] = s_perp;
        sv.p2[0] = s_par;
        sv.p1[1] = e_perp;
        sv.p1[0] = e_par;
        sv.swp[1] = s_perp;
        sv.swp[0] = s_par;
        sv.flag = 2;
        sv.phi = -ph;
    }
    fun_14034151e(sv.p1, sv.p2, sv.cf, &sv.rad, r, &sv.phi, g, &sv.len, &sv.status);
    const std::int32_t st = sv.status;
    if (st == 0) {
        *res = -1;
        return;
    }
    if (st >= 0) {
        *res = 0;
        return;
    }
    *res = 1;
    if (sv.flag == 1) {
        *res = 2;
    }
}

// @src 14034151e  ray/arc contact in angle space
void fun_14034151e(const double* p1, const double* /*p2: not read*/, const double* cf,
                   const double* rad, const double* r, const double* phi, double* g,
                   const double* len, std::int32_t* st)
{
    using def::rt::libm::ps_asin;
    auto& sv = g_save_14034151e;
    const double rr = *r;
    sv.mode = 1;
    sv.r = rr;
    *st = 0;
    if (*rad == rr) {
        sv.mode = 2;
    }
    const double cy = cf[1];
    if (cy == 0.0) {  // 14034228a: the centre lies on the ray line
        const double cx = cf[0];
        if (rr >= *rad) {  // 14034229e: COMISD r, R; JNC
            sv.gnum = cx;
            *st = 1;
            *g = cx / *len;
            return;
        }
        const double x0 = p1[0];
        if (cx > x0 && p1[1] == 0.0) {  // 1403422c7: p1 on the ray, before the centre
            sv.gnum = rr + x0;
            *st = 1;
            *g = sv.gnum / *len;
            return;
        }
        if (x0 > cx && p1[1] == 0.0) {  // 1403422e4: p1 on the ray, beyond the centre
            sv.gnum = x0 - rr;
            *st = 1;
            *g = sv.gnum / *len;
            return;
        }
    }

    // 1403415cf: clear A(2,12)
    for (sv.i = 1; sv.i <= 12; ++sv.i) {
        for (sv.k = 1; sv.k <= 2; ++sv.k) {
            sv.a[2 * (sv.i - 1) + (sv.k - 1)] = 0.0;
        }
    }

    if (sv.mode != 2) {  // 140341691: sin(alpha) = cy / (r - R)
        const double x = cy / (rr - *rad);
        const double ax = std::fabs(x);
        const double d = std::fabs(kOne - ax);
        sv.a[24] = x;
        sv.a[25] = d;
        if (kAngTol > d || kOne >= ax) {
            if (kMinusOne > sv.a[24]) {
                sv.a[24] = kMinusOne;
            }
            if (sv.a[24] > kOne) {
                sv.a[24] = kOne;
            }
            const double t = ps_asin(sv.a[24]);
            a1(sv, 1) = t;
            a1(sv, 2) = kPi - t;
            if (0.0 > a1(sv, 1)) {
                a1(sv, 1) = kPi * kTwo + a1(sv, 1);
            }
            *st = 2;
        }
    }
    if (!(cy == 0.0)) {  // 1403417f6: sin(alpha) = cy / -(r + R)
        const double x = cy / -(rr + *rad);
        const double ax = std::fabs(x);
        const double d = std::fabs(kOne - ax);
        sv.a[24] = x;
        sv.a[25] = d;
        if (kAngTol > d || kOne >= ax) {
            if (kMinusOne > sv.a[24]) {
                sv.a[24] = kMinusOne;
            }
            if (sv.a[24] > kOne) {
                sv.a[24] = kOne;
            }
            const std::int32_t n = *st;
            const double t = ps_asin(sv.a[24]);
            a1(sv, n + 1) = t;
            a1(sv, n + 2) = kPi - t;
            if (0.0 > a1(sv, n + 1)) {
                a1(sv, n + 1) = kPi * kTwo + a1(sv, n + 1);
            }
            *st = n + 2;
        }
        if (*st == 0) {
            return;
        }
    }

    // 1403419a8: sort by angle, append the -2 pi and +2 pi copies, sort again
    {
        std::int32_t key = 1;
        std::int32_t asc = 1;
        std::int32_t nrow = 2;
        fun_1404d93ae(sv.a, &key, st, &asc, &nrow);
    }
    sv.i = 1;
    if (*st >= 1) {
        const double twopi = kPi * kTwo;  // 140341a2a
        const std::int32_t n = *st;
        for (sv.i = 1; sv.i <= *st; ++sv.i) {
            a1(sv, n + sv.i) = a1(sv, sv.i) - twopi;
            a2(sv, n + sv.i) = a2(sv, sv.i);
        }
        for (sv.i = 1; sv.i <= *st; ++sv.i) {
            a1(sv, 2 * n + sv.i) = twopi + a1(sv, sv.i);
            a2(sv, 2 * n + sv.i) = a2(sv, sv.i);
        }
    }
    sv.n3 = *st * 3;
    {
        std::int32_t key = 1;
        std::int32_t asc = 1;
        std::int32_t nrow = 2;
        fun_1404d93ae(sv.a, &key, &sv.n3, &asc, &nrow);
    }

    // 140341bef: mark the intervals whose mid angle puts the ray line within r of the circle,
    // then merge runs of marked intervals
    sv.i = 1;
    const std::int32_t last = sv.n3 - 1;
    if (last < 1) {  // 140341e84
        sv.n = sv.n3;
        sv.one = kOne;
    } else {
        for (sv.i = 1; sv.i <= last; ++sv.i) {
            sv.mid = (a1(sv, sv.i) + a1(sv, sv.i + 1)) * kHalf;  // 140341c73
            const double sn = def::rt::libm::ps_sin(sv.mid);
            sv.v = cy / sn + *rad;  // 140341c9e: (cy / sin) + R
            if (rr > std::fabs(sv.v)) {
                a2(sv, sv.i) = kOne;
            }
        }
        sv.n = sv.n3;
        sv.one = kOne;
        sv.i = 1;
        while (sv.i < sv.n) {  // 140341df6
            if (a2(sv, sv.i) == kOne && a2(sv, sv.i + 1) == kOne) {
                const std::int32_t m = sv.n - 1;
                sv.n = m;
                sv.k = sv.i + 1;
                if (m >= sv.i + 1) {
                    // 140341d3c: the shift indexes the table as A(k,1), A(k,2) with a leading
                    // dimension of 2, i.e. flat elements k and k + 2, not columns (as compiled)
                    std::int32_t j = sv.i + 2;
                    for (;;) {
                        const std::int32_t k = sv.k;
                        flat(sv, k) = flat(sv, j);
                        sv.k = k + 1;
                        flat(sv, k + 2) = flat(sv, j + 2);
                        ++j;
                        if (sv.k > sv.n) {
                            break;
                        }
                    }
                }
            }
            ++sv.i;
            if (sv.i > last) {
                break;
            }
        }
    }
    sv.n3 = sv.n;  // 140341f05

    // 140341ea9: th0 = direction of Q from the centre, th1 = direction of p1, both in [0, 2 pi)
    const double cx = cf[0];
    sv.ex[1] = 0.0;
    sv.ex[0] = kOne;
    sv.vec[1] = -cy;
    sv.vec[0] = -cx;
    fun_14032beef(sv.ex, sv.vec, &sv.th0);
    if (0.0 > sv.th0) {
        sv.th0 = kPi * kTwo + sv.th0;
    }
    sv.ex[1] = 0.0;
    sv.ex[0] = kOne;
    sv.vec[0] = p1[0] - cx;
    sv.vec[1] = p1[1] - cy;
    fun_14032beef(sv.ex, sv.vec, &sv.th1);
    if (0.0 > sv.th1) {
        sv.th1 = kPi * kTwo + sv.th1;
    }
    sv.v = cy / def::rt::libm::ps_sin(sv.th1) + *rad;  // 14034205b (not used further)

    // 140342077: find the table entry at th0 (within 1e-4) or the interval holding it
    bool found = false;
    sv.i = 1;
    if (sv.n >= 1) {
        for (sv.i = 1; sv.i <= sv.n; ++sv.i) {
            sv.dd = std::fabs(a1(sv, sv.i) - sv.th0);
            if (sv.dd <= kAngTol) {
                found = true;
                break;
            }
        }
    }
    if (!found) {  // 1403420a2
        sv.i = 1;
        const std::int32_t last2 = sv.n - 1;
        if (last2 >= 1) {
            for (;;) {
                if (sv.th0 > a1(sv, sv.i) && a1(sv, sv.i + 1) > sv.th0) {
                    break;
                }
                ++sv.i;
                if (sv.i > last2) {
                    break;  // 140342154: falls through with i = n
                }
            }
            found = true;
        }
    }

    double val;
    if (found) {  // 1403421fa
        if (a2(sv, sv.i) == 0.0) {
            *st = 0;
            return;
        }
        sv.idx = sv.i + 1;
        if (0.0 > *phi) {
            sv.idx = sv.i;
        }
        val = *phi + sv.th1;  // 1403423d2
        sv.val = val;
        if (val > a1(sv, sv.i) && a1(sv, sv.i + 1) > val) {
            sv.chosen = val;
            *st = -1;
        } else {
            sv.chosen = a1(sv, sv.idx);
            sv.val = val;
            *st = 1;
        }
    } else {  // 140342440: fewer than two table entries
        if (a2(sv, 1) == 0.0) {
            *st = 0;
            return;
        }
        sv.idx = 2;
        if (0.0 > *phi) {
            sv.idx = 1;
        }
        val = *phi + sv.th1;
        sv.val = val;
        if (val > a1(sv, 1) && a1(sv, 2) > val) {
            sv.chosen = val;
            *st = -1;
        } else {
            sv.chosen = a1(sv, sv.idx);
            sv.val = val;
            *st = 1;
        }
    }

    // 14034252e: point of the ray line at the chosen angle
    double sn;
    double cs;
    def::rt::libm::ps_sincos(sv.chosen, &sn, &cs);
    const double h = std::fabs(cy / sn);
    sv.h = h;
    sv.vec[0] = cs;
    sv.gnum = h * cs + cx;  // 14034257e: (h * cos) + cx
    *g = sv.gnum / *len;
    if (0.0 > *g) {
        *st = 0;
    }
}

// Stand-in for 1404d71d7 (rotate, ported by p1-11 in defmodel/topo/ftn_geom.cpp; the shared
// def::rt::geoutl::fun_1404d71d7 is not declared yet): forwards to that port.
void fun_1404d71d7(double* v, const double* ang) { def::model::topo::rotate_2d(v, ang); }

} // namespace def::cmd::ftn
