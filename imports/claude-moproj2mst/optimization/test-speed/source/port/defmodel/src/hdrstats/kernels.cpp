// Numeric kernels of packet p4-07: principal values (2D C++, 3D Fortran), Jacobi eigenvalues,
// polygon and surface area. Declarations: defmodel/hdrstats/object_stats.hpp.
//
// Every expression follows the disassembly (the Fortran routines 14032d1b0 and 1403f19xx are
// PathScale -O2: several sums are grouped differently from Ghidra's decompile; the grouping used
// here is the compiled one and each such place says so). Constants are the binary's bit patterns.
#include <defmodel/hdrstats/object_stats.hpp>

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::model::p407 {

namespace {

constexpr double bits(std::uint64_t u) { return std::bit_cast<double>(u); }

// 14032d1b0's constants (Fortran data, 0x140baeb90 ..)
constexpr double k_half = bits(0x3fe0000000000000);     // 140baeb90 0.5
constexpr double k_quarter = bits(0x3fd0000000000000);  // 140baeb98 0.25
constexpr double k_tiny = bits(0x3eb0c6f7a0000000);     // 140baeba8 1e-6 (REAL*4 rounded)
constexpr double k_shear_tol = bits(0x3f50624de0000000); // 140baebb8 0.001 (REAL*4 rounded)
constexpr double k_two = bits(0x4000000000000000);      // 140baebc0 2.0
constexpr double k_halfpi_f = bits(0x3ff921fb00000000); // 140baebc8 pi/2 (REAL*4 digits)
constexpr double k_angle_tol = bits(0x3ee4f8b580000000); // 140baebd0 1e-5 (REAL*4 rounded)
constexpr double k_three = bits(0x4008000000000000);    // 140baebd8 3.0

// 1403f1980 .. 1403f1ec3 (0x140bb11f0 .., 0x1406208b0 ..)
constexpr double k3_half = bits(0x3fe0000000000000);    // 140bb11f0
constexpr double k3_six = bits(0x4018000000000000);     // 140bb1200
constexpr double k3_w[6] = {
    bits(0x3fe5555555555573), bits(0x3fe5555555555573), bits(0x3fe5555555555573), // 1406208b0..c0
    bits(0x3ff5555555555554), bits(0x3ff5555555555554), bits(0x3ff5555555555554), // 1406208c8..d8
};
constexpr double k3_two = bits(0x4000000000000000);     // 140bb1218
constexpr double k3_three = bits(0x4008000000000000);   // 140bb1220
constexpr double k3_nine = bits(0x4022000000000000);    // 140bb1228
constexpr double k3_27 = bits(0x403b000000000000);      // 140bb1230
constexpr double k3_54 = bits(0x404b000000000000);      // 140bb1238
constexpr double k3_one = bits(0x3ff0000000000000);     // 140bb1240
constexpr double k3_mone = bits(0xbff0000000000000);    // 140bb1248
constexpr double k3_eps = bits(0x3eb0c6f7a0b5ed8d);     // 140bb1250 1e-6
constexpr double k3_mtwo = bits(0xc000000000000000);    // 140bb1258
constexpr double k3_lo = bits(0xc197d78400000000);      // 140bb1260 -1e8
constexpr double k3_hi = bits(0x4197d78400000000);      // 140bb1268 1e8
constexpr double k3_pi = bits(0x400921fb54442d28);      // 1406208e0 3.1415926535898

// C++ constants (.rdata)
constexpr double k_zero = 0.0;                          // 14051bed8
constexpr double k_one = bits(0x3ff0000000000000);      // 140509c90
constexpr double k_halfc = bits(0x3fe0000000000000);    // 14051f240
constexpr double k_jac_thresh = bits(0x3fc999999999999a); // 140531778 0.2
constexpr double k_jac_100 = bits(0x4059000000000000);  // 140524cd8 100.0

// MAXSD / MINSD: the first operand when it compares greater / smaller, else the second (also when
// either is NaN)
double maxsd(double a, double b) { return a > b ? a : b; }
double minsd(double a, double b) { return a < b ? a : b; }

// fabs as ANDPD with 0x7fff...
double andabs(double x) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffffull); }

} // namespace

// @src 1402204f0  (float)fabs((double)x) through the CRT fabs 140484558
float fabs_float_b(float x) { return static_cast<float>(std::fabs(static_cast<double>(x))); }

// @src 1402204d0  (forwards to 1402204f0)
float fabs_float(float x) { return fabs_float_b(x); }

// @src 14032d1b0  2D principal values. The SAVE variables 0x140701bb0 .. 0x140701bd0 are written
// and read inside one call only; locals here.
void principal_2d(const std::int32_t* kind, const double* s, double* eff, double* pmax,
                  double* p1, double* p2, double* angle)
{
    const double sxy = s[3];
    double t = sxy;                                    // 140701bb0
    if (*kind != 1 && *kind != 2) t = sxy * k_half;
    const double sxx = s[0];
    const double syy = s[1];
    const double sum = sxx + syy;                      // xmm8
    double d = sxx - syy;                              // 140701bb8
    const double c = sum * k_half;                     // 140701bc0
    const double r = std::sqrt(d * d * k_quarter + t * t);
    *p1 = c + r;
    std::int32_t swapped = 0;                          // 140701bc8
    *p2 = c - r;
    if (!(syy < sxx)) swapped = 1;                     // COMISD syy, sxx; JP/JC skip
    if (d == 0.0) d = k_tiny;
    const double szz = s[2];
    if (*kind == 0 || *kind == 1) {
        *pmax = maxsd(maxsd(*p2, *p1), szz);
    } else {
        *pmax = minsd(minsd(*p2, *p1), szz);
    }
    if (andabs(t) > k_shear_tol) {
        const double a = def::rt::libm::ps_atan((t * k_two) / d) * k_half;
        *angle = a;
        if (swapped == 1) *angle = a + k_halfpi_f;
    } else if (*kind == 0 || *kind == 1) {
        if (k_angle_tol > andabs(*pmax - sxx)) *angle = 0.0;
        if (k_angle_tol > andabs(*pmax - syy)) *angle = k_halfpi_f;
    } else {
        if (k_angle_tol > andabs(*pmax - sxx)) *angle = k_halfpi_f;
        if (k_angle_tol > andabs(*pmax - syy)) *angle = 0.0;
    }
    const double sxy2 = sxy * sxy;
    if (*kind == 1) {
        // 14032d51b: mean (sum + szz) / 3, then szz'^2 + (syy'^2 + sxx'^2) (Ghidra groups the
        // squares left to right) and 3 * (sxy * sxy)
        const double m = (sum + szz) / k_three;        // 140701bd0
        const double dy = syy - m;
        const double dx = sxx - m;
        const double dz = szz - m;
        double q = dy * dy + dx * dx;
        q = dz * dz + q;
        q = q * k_three;
        q = q * k_half;
        *eff = std::sqrt(q + k_three * sxy2);
        return;
    }
    // 14032d3fa: (sxy^2 * 0.5) + (szz^2 + (syy^2 + sxx^2))
    const double q = szz * szz + (syy * syy + sxx * sxx);
    *eff = std::sqrt(((sxy2 * k_half + q) * k_two) / k_three);
}

// @src 1403f1ec3  roots of the cubic x^3 + a x^2 + b x + c. The SAVE variables 0x14070aff8 ..
// 0x14070b080 are locals except the root indices 14070b084 / 14070b088, which the original keeps
// between calls (globals_p4_07.hpp).
void cubic_roots(const double* a, const double* b, const double* c, double* rmax, double* rmid,
                 double* rmin, std::int32_t* fail)
{
    const double a0 = *a;
    const double b0 = *b;
    const double aa = a0 * *a;                         // 14070aff8
    const double aaa = a0 * aa;                        // 14070b000
    *rmax = 0.0;
    *rmid = 0.0;
    *fail = 0;
    *rmin = 0.0;
    const double q = (aa - b0 * k3_three) / k3_nine;   // 14070b008
    const double r = ((aaa * k3_two - (a0 * k3_nine) * b0) + *c * k3_27) / k3_54; // 14070b010
    if (!(q > 0.0)) {
        *fail = 1;
        return;
    }
    const double q3 = (q * q) * q;                     // 14070b020
    double t = r / std::sqrt(q3);                      // 14070b028
    const double third_turn = (k3_pi * k3_two) / k3_three; // 14070b018
    const double at = andabs(t);                       // 14070b030
    if (at > k3_one) {
        if (at + k3_mone > k3_eps) {
            *fail = 1;
            return;
        }
        t = k3_one;
    }
    const double theta = def::rt::libm::ps_acos(t);    // 14070b040
    const double th3 = theta / k3_three;               // 14070b048
    const double sq = std::sqrt(q);
    const double a3 = *a / k3_three;                   // 14070b000
    const double m = sq * k3_mtwo;                     // 14070b050
    double x[3];                                       // 14070b060 ..
    x[0] = m * def::rt::libm::ps_cos(th3) - a3;
    x[1] = m * def::rt::libm::ps_cos(third_turn + th3) - a3;
    x[2] = def::rt::libm::ps_cos(th3 - third_turn) * m - a3;
    *rmax = k3_lo;
    *rmin = k3_hi;
    for (std::int32_t k = 1; k <= 3; k = k + 1) {
        const double v = x[k - 1];
        if (v > *rmax) {
            *rmax = v;
            g_root_imax = k;
        }
        if (*rmin > v) {
            *rmin = v;
            g_root_imin = k;
        }
    }
    for (std::int32_t k = 1; k <= 3; k = k + 1) {
        if (k == g_root_imax || k == g_root_imin) continue;
        *rmid = x[k - 1];
        return;
    }
}

// @src 1403f1caa  invariants, then 1403f1ec3. Loop counter 14070afc4 and the invariants
// 14070afc8 .. 14070aff0 are SAVE locals.
void principal_3d_roots(double* s, double* pmax, double* pmid, double* pmin, std::int32_t* fail)
{
    for (std::int32_t k = 1; k <= 6; k = k + 1) {
        if (andabs(s[k - 1]) > 0.0) {
            const double s0 = s[0], s1 = s[1], s2 = s[2], s3 = s[3], s4 = s[4], s5 = s[5];
            const double a3 = s3 * s3;                 // 14070afc8
            const double a4 = s4 * s4;                 // 14070afd0
            const double a5 = s5 * s5;                 // 14070afd8
            // I2 = s2*s0 + (s2*s1 + s0*s1) - ((a3 + a4) + a5)  (compiled grouping)
            const double i2 = (s2 * s0 + (s2 * s1 + s0 * s1)) - ((a3 + a4) + a5);
            const double i1 = -((s0 + s1) + s2);       // XORPD with -0.0
            // I3 = s2*a3 + ((s0*a4 - ((s0*s1)*s2 + ((s3*2)*s4)*s5)) + s1*a5)
            double t = s0 * a4 - ((s0 * s1) * s2 + ((s3 * k3_two) * s4) * s5);
            t = t + s1 * a5;
            const double i3 = s2 * a3 + t;
            cubic_roots(&i1, &i2, &i3, pmax, pmid, pmin, fail);
            return;
        }
    }
    *pmax = 0.0;
    *pmin = 0.0;
    *pmid = 0.0;
    *fail = 0;
}

// @src 1403f1980  3D effective value and principal values. The copy of s (14070af90 ..) and the
// failure flag 14070afc0 are SAVE locals.
void principal_3d(const std::int32_t* kind, const double* s, double* eff, double* pmax,
                  double* pmid, double* pmin)
{
    double g[6] = {s[0], s[1], s[2], s[3], s[4], s[5]};
    if (*kind == 0) {
        g[3] = s[3] * k3_half;
        g[4] = s[4] * k3_half;
        g[5] = k3_half * s[5];
    }
    std::int32_t fail = 0;
    principal_3d_roots(g, pmax, pmid, pmin, &fail);
    if (fail == 1) *pmax = 0.0;
    if (*kind == 1) {
        // 1403f1bcc: (g5^2 + (g4^2 + g3^2)) * 6 + ((g2-g0)^2 + ((g1-g2)^2 + (g0-g1)^2)), * 0.5
        const double d01 = g[0] - g[1];
        const double d20 = g[2] - g[0];
        const double d12 = g[1] - g[2];
        double sh = g[4] * g[4] + g[3] * g[3];
        sh = g[5] * g[5] + sh;
        double di = d12 * d12 + d01 * d01;
        di = d20 * d20 + di;
        *eff = std::sqrt((sh * k3_six + di) * k3_half);
        return;
    }
    // 1403f1a75: t5 + (t4 + (t3 + (t2 + (t1 + t0)))), tk = (wk * gk) * gk
    double acc = (k3_w[1] * g[1]) * g[1] + (k3_w[0] * g[0]) * g[0];
    acc = (k3_w[2] * g[2]) * g[2] + acc;
    acc = (k3_w[3] * g[3]) * g[3] + acc;
    acc = (k3_w[4] * g[4]) * g[4] + acc;
    acc = (k3_w[5] * g[5]) * g[5] + acc;
    *eff = std::sqrt(acc);
}

// @src 14023efb0  Jacobi (Numerical Recipes "jacobi" in C, float matrix, double sums)
std::int32_t jacobi3(float* a, float* d, float* v)
{
    constexpr std::int32_t n = 3;
    double b[3];
    double z[3];
    std::int32_t nrot = 0;
    for (std::int32_t ip = 0; ip < n; ip = ip + 1) {
        for (std::int32_t iq = 0; iq < n; iq = iq + 1) v[ip * 3 + iq] = 0.0f;
        v[ip * 3 + ip] = 1.0f;                         // 14053176c
    }
    for (std::int32_t ip = 0; ip < n; ip = ip + 1) {
        d[ip] = a[ip * 3 + ip];
        b[ip] = static_cast<double>(d[ip]);
        z[ip] = 0.0;
    }
    for (std::int32_t i = 1; i <= 50; i = i + 1) {
        double sm = 0.0;
        for (std::int32_t ip = 0; ip < n - 1; ip = ip + 1) {
            for (std::int32_t iq = ip + 1; iq < n; iq = iq + 1)
                sm = sm + std::fabs(static_cast<double>(a[ip * 3 + iq]));
        }
        if (sm == k_zero) return nrot;
        double tresh;
        if (i < 4) {
            tresh = (k_jac_thresh * sm) / static_cast<double>(n * n);
        } else {
            tresh = 0.0;
        }
        for (std::int32_t ip = 0; ip < n - 1; ip = ip + 1) {
            for (std::int32_t iq = ip + 1; iq < n; iq = iq + 1) {
                const double g = k_jac_100 * std::fabs(static_cast<double>(a[ip * 3 + iq]));
                if (i > 4 && std::fabs(static_cast<double>(d[ip])) + g == std::fabs(static_cast<double>(d[ip])) &&
                    std::fabs(static_cast<double>(d[iq])) + g == std::fabs(static_cast<double>(d[iq]))) {
                    a[ip * 3 + iq] = 0.0f;
                } else if (std::fabs(static_cast<double>(a[ip * 3 + iq])) > tresh) {
                    double h = static_cast<double>(d[iq] - d[ip]);
                    double t;
                    if (std::fabs(h) + g == std::fabs(h)) {
                        t = static_cast<double>(a[ip * 3 + iq]) / h;
                    } else {
                        const double theta = (k_halfc * h) / static_cast<double>(a[ip * 3 + iq]);
                        t = k_one / (std::fabs(theta) + std::sqrt(k_one + theta * theta));
                        if (0.0 > theta) t = -t;
                    }
                    const double c = k_one / std::sqrt(k_one + t * t);
                    const double s = t * c;
                    const double tau = s / (k_one + c);
                    h = t * static_cast<double>(a[ip * 3 + iq]);
                    z[ip] = z[ip] - h;
                    z[iq] = z[iq] + h;
                    d[ip] = static_cast<float>(static_cast<double>(d[ip]) - h);
                    d[iq] = static_cast<float>(static_cast<double>(d[iq]) + h);
                    a[ip * 3 + iq] = 0.0f;
                    auto rotate = [&](float* m, std::int32_t i1, std::int32_t j1, std::int32_t k1,
                                      std::int32_t l1) {
                        const double gg = static_cast<double>(m[i1 * 3 + j1]);
                        const double hh = static_cast<double>(m[k1 * 3 + l1]);
                        m[i1 * 3 + j1] = static_cast<float>(gg - s * (hh + gg * tau));
                        m[k1 * 3 + l1] = static_cast<float>(hh + s * (gg - hh * tau));
                    };
                    for (std::int32_t j = 0; j <= ip - 1; j = j + 1) rotate(a, j, ip, j, iq);
                    for (std::int32_t j = ip + 1; j <= iq - 1; j = j + 1) rotate(a, ip, j, j, iq);
                    for (std::int32_t j = iq + 1; j < n; j = j + 1) rotate(a, ip, j, iq, j);
                    for (std::int32_t j = 0; j < n; j = j + 1) rotate(v, j, ip, j, iq);
                    nrot = nrot + 1;
                }
            }
        }
        for (std::int32_t ip = 0; ip < n; ip = ip + 1) {
            b[ip] = b[ip] + z[ip];
            d[ip] = static_cast<float>(b[ip]);
            z[ip] = 0.0;
        }
    }
    return nrot;
}

// @src 14023fc50  (Numerical Recipes "eigsrt", descending, float)
void eigsrt3(float* d, float* v)
{
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        std::int32_t k = i;
        float p = d[i];
        for (std::int32_t j = i + 1; j < 3; j = j + 1) {
            if (p <= d[j]) {
                k = j;
                p = d[j];
            }
        }
        if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                const float tmp = v[j * 3 + i];
                v[j * 3 + i] = v[j * 3 + k];
                v[j * 3 + k] = tmp;
            }
        }
    }
}

// @src 14023e670  area of a polygon of n points: 0.5 * |sum p_i x p_(i+1)|
double polygon_area(std::int32_t n, const Vec3* pts)
{
    Vec3 tmp;
    Vec3 acc = *ext::fun_140239700(&tmp, 0.0, 0.0, 0.0);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        const Vec3 p = pts[i];
        const Vec3 q = (i == n - 1) ? pts[0] : pts[i + 1];
        Vec3 cr;
        cr = *ext::fun_14023a4b0(&tmp, &p, &q);
        Vec3 sum;
        sum = *ext::fun_14023a1b0(&tmp, &acc, &cr);
        acc = sum;
    }
    return k_halfc * ext::fun_140239a30(&acc);
}

// @src 1401638f0  surface area (3D objects only; negative sums give 0)
double obj_surface_area(LLOBJ_DATA* obj, std::int32_t which)
{
    double area = 0.0;
    if (!def::rt::dimension_is_3(obj->owner)) return 0.0;
    std::int32_t nface;
    const std::int32_t* faces;
    double* xyz;
    if (which == 0) {
        if (!ext::fun_140160d90(obj)) return 0.0;
        const std::int32_t zero = 0;
        void* geo = ext::fun_140198620(obj, &zero);
        nface = static_cast<OBJ_GEO3D*>(geo)->elem_count();   // 140003500
        faces = ext::fun_140005510(geo);
        xyz = ext::fun_140006410(geo);
    } else {
        if (which != 1) return 0.0;
        if (obj->u87c[0] == 0) return 0.0;
        // LLOBJ_DATA+0xbe0 (Cls_14017e740): count at +0x10, 4 node numbers per face at +0x18
        nface = *reinterpret_cast<const std::int32_t*>(obj->sbe0[0] + 0x10);
        faces = *reinterpret_cast<std::int32_t* const*>(obj->sbe0[0] + 0x18);
        xyz = ext::fun_1401808d0(obj->s8a0);
    }
    for (std::int32_t f = 0; f < nface; f = f + 1) {
        const std::int32_t* face = faces + static_cast<std::ptrdiff_t>(f * 4);
        Vec3 pts[4];
        for (std::int32_t k = 0; k < 4; k = k + 1) {
            const std::int32_t node = std::abs(face[k]);       // 140484ab8
            const double* c = xyz + static_cast<std::ptrdiff_t>((node - 1) * 3);
            Vec3 tmp;
            pts[k] = *ext::fun_140239700(&tmp, c[0], c[1], c[2]);
        }
        area = area + polygon_area(4, pts);
    }
    if (area < 0.0) area = 0.0;
    return area;
}

} // namespace def::model::p407
