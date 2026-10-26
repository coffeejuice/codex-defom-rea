// Element tensor rotation (STRESS, STNCMP, YLDS, MATAXI) for the OBJPOS rotate.
// Spec: docs/re/commands/OBJPOS_geo_kernels.md section 1 (operation order from the listings).
#include <defcmd/objpos_parts.hpp>
#include <defmodel/obj_scale.hpp>

#include <cmath>
#include <cstdint>
#include <cstdlib>

#include <defmodel/members/ftn_geom.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/stdlib/libm.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using namespace detail;

// @data 140701810  SAVE block of 140325884 (frame T at 0x140701840, R/Q at 0x1407018a0, W at
// 0x140701910). Kept here because the kernel is the only user; the values persist between calls
// as in the original (only the element buffer's staleness for kind != 1, 2 could show it).
struct TensorKernelSave {
    double t[9];   // T(i,j) at (i-1) + 3*(j-1)
    double q[9];   // R, then Q = T^T R T in place
    double w[9];
    double s[6];   // element buffer
};
inline TensorKernelSave g_tensor_3d{};

namespace {

inline double& T(double* m, int i, int j) { return m[(i - 1) + 3 * (j - 1)]; }

} // namespace

// @src 140325884  rotate n symmetric tensors [xx, yy, zz, xy, yz, zx] by *a degrees about the
// axis D (frame threshold 1e-12, angle (a*pi)/180, Fortran SIN/COS); kind 2 (strain) halves the
// shear terms before and doubles them after; flag 1 negates the sine
void rotate_tensors_3d(double* s, const std::int32_t* n, const std::int32_t* kind, const double* d,
                       const double* a, const std::int32_t* flag)
{
    TensorKernelSave& g = g_tensor_3d;
    double* t = g.t;
    const double xa = d[0], xb = d[1], xc = d[2];
    const double r1 = std::sqrt(xa * xa + xc * xc);
    const double r2 = std::sqrt((xb * xb + xa * xa) + xc * xc);
    const double sx = r1 / r2;
    const double tx = xb / r2;
    if (!(r1 >= 1e-12)) {  // DAT_140baea68; NaN takes this branch
        T(t, 1, 1) = 1.0;
        T(t, 2, 1) = 0.0;
        T(t, 3, 1) = 0.0;
        T(t, 1, 2) = 0.0;
        T(t, 2, 2) = sx;
        T(t, 3, 2) = tx;
        T(t, 1, 3) = 0.0;
        T(t, 2, 3) = -tx;
        T(t, 3, 3) = sx;
    } else {
        const double ca = xa / r1;
        const double cc = xc / r1;
        T(t, 1, 1) = cc;
        T(t, 2, 1) = -(ca * tx);
        T(t, 3, 1) = ca * sx;
        T(t, 1, 2) = 0.0;
        T(t, 2, 2) = sx;
        T(t, 3, 2) = tx;
        T(t, 1, 3) = -ca;
        T(t, 2, 3) = -(cc * tx);
        T(t, 3, 3) = cc * sx;
    }
    const double rad = (*a * 3.141592653589793) / 180.0;  // DAT_1405f3140, DAT_140baea78
    double sn = 0.0;
    double cs = 0.0;
    rt::libm::ps_sincos(rad, &sn, &cs);  // 1404b3b30
    const double sp = (*flag == 1) ? -sn : sn;
    double* r = g.q;
    T(r, 1, 1) = cs;
    T(r, 2, 1) = -sp;
    T(r, 3, 1) = 0.0;
    T(r, 1, 2) = sp;
    T(r, 2, 2) = cs;
    T(r, 3, 2) = 0.0;
    T(r, 1, 3) = 0.0;
    T(r, 2, 3) = 0.0;
    T(r, 3, 3) = 1.0;
    double* w = g.w;
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            T(w, i, j) = ((0.0 + T(r, i, 1) * T(t, 1, j)) + T(r, i, 2) * T(t, 2, j)) + T(r, i, 3) * T(t, 3, j);
        }
    }
    double* q = g.q;
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            T(q, i, j) = ((0.0 + T(t, 1, i) * T(w, 1, j)) + T(t, 2, i) * T(w, 2, j)) + T(t, 3, i) * T(w, 3, j);
        }
    }
    for (std::int32_t e = 0; e < *n; ++e) {
        double* x = s + 6 * static_cast<std::ptrdiff_t>(e);
        if (*kind == 1 || *kind == 2) {
            for (int k = 0; k < 6; ++k) g.s[k] = x[k];
            if (*kind == 2) {
                g.s[3] = g.s[3] * 0.5;  // DAT_140baea80
                g.s[4] = g.s[4] * 0.5;
                g.s[5] = g.s[5] * 0.5;
            }
            double sm[9];  // symmetric S(i,j)
            T(sm, 1, 1) = g.s[0];
            T(sm, 2, 2) = g.s[1];
            T(sm, 3, 3) = g.s[2];
            T(sm, 1, 2) = g.s[3];
            T(sm, 2, 1) = g.s[3];
            T(sm, 2, 3) = g.s[4];
            T(sm, 3, 2) = g.s[4];
            T(sm, 1, 3) = g.s[5];
            T(sm, 3, 1) = g.s[5];
            double v[9];
            for (int i = 1; i <= 3; ++i) {
                for (int j = 1; j <= 3; ++j) {
                    T(v, i, j) = (T(q, i, 1) * T(sm, 1, j) + T(q, i, 2) * T(sm, 2, j)) + T(q, i, 3) * T(sm, 3, j);
                }
            }
            auto sp2 = [&](int i, int l) {
                return (T(q, l, 1) * T(v, i, 1) + T(q, l, 2) * T(v, i, 2)) + T(q, l, 3) * T(v, i, 3);
            };
            g.s[0] = sp2(1, 1);
            g.s[1] = sp2(2, 2);
            g.s[2] = sp2(3, 3);
            g.s[3] = sp2(2, 1);
            g.s[4] = sp2(3, 2);
            g.s[5] = sp2(3, 1);
            if (*kind == 2) {
                g.s[3] = g.s[3] * 2.0;  // DAT_140baea88
                g.s[4] = g.s[4] * 2.0;
                g.s[5] = g.s[5] * 2.0;
            }
        }
        for (int k = 0; k < 6; ++k) x[k] = g.s[k];  // other kinds store the stale buffer
    }
}

// @src 14032e585  2D: n tensors [rr, zz, hoop, rz] rotated by *a degrees ((a*pi)/180, Fortran
// SIN/COS, flag 1 negates the sine); kind 2 halves rz before and doubles it after; hoop unchanged
void rotate_tensors_2d(double* s, const std::int32_t* n, const std::int32_t* kind, const double* a,
                       const std::int32_t* flag)
{
    const double rad = (*a * 3.141592653589793) / 180.0;  // DAT_1405f4350, DAT_140baebf8
    double sn = 0.0;
    double cs = 0.0;
    rt::libm::ps_sincos(rad, &sn, &cs);
    const double sp = (*flag == 1) ? -sn : sn;  // DAT_140baec00
    const double cc = cs * cs;
    const double ss = sp * sp;
    const double sc = sp * cs;
    const double dd = cc - ss;
    for (std::int32_t e = 0; e < *n; ++e) {
        double* x = s + 4 * static_cast<std::ptrdiff_t>(e);
        const double e0 = x[0];
        const double e1 = x[1];
        const double s3 = (*kind == 2) ? x[3] * 0.5 : x[3];  // DAT_140baeb90
        const double h = (s3 * 2.0) * sc;                    // DAT_140baebc0
        x[0] = ((e1 * ss) + (e0 * cc)) + h;
        x[1] = ((e1 * cc) + (e0 * ss)) - h;
        x[3] = ((e1 * sc) - (e0 * sc)) + (s3 * dd);
        if (*kind == 2) x[3] = x[3] * 2.0;
    }
}

// 140182980: ported in defmodel/obj_scale.hpp (scale::element_set_dims)
const std::int32_t* elem_widths(const unsigned char* es)
{
    return model::scale::element_set_dims(const_cast<unsigned char*>(es));
}

// @src 140184ac0  element tensors of the element set (obj+0x9e0) for a rotation by angle (deg)
// @src 14019a480  (inlined here) element set dimension test: +0xf8 == 2
void elems_rotate(unsigned char* es, const double* origin, const double* dir, double angle)
{
    std::int32_t flag = 1;  // local_5c
    std::int32_t m = 1;     // local_70
    double a = angle;
    const bool is3 = at<std::int32_t>(es, 0xf8) == 3;  // 14019a450
    const bool is2 = at<std::int32_t>(es, 0xf8) == 2;  // 14019a480
    if (origin == nullptr || elem_widths(es) == nullptr || !(!is3 || dir != nullptr)) return;
    const unsigned char* wt = reinterpret_cast<const unsigned char*>(elem_widths(es));
    const std::int32_t geotyp = at<std::int32_t>(at<unsigned char*>(es, 0xf0), 0x850);  // 1401d6d90
    std::int32_t* n = &at<std::int32_t>(es, 0xe8);
    if (at<double*>(es, 0x20) != nullptr) {  // STRESS
        const std::int32_t w = at<std::int32_t>(wt, 0x98);
        if (!(is2 && w != 4)) {
            std::int32_t kind = 1;
            if (!is2) rotate_tensors_3d(at<double*>(es, 0x20), n, &kind, dir, &a, &flag);
            else rotate_tensors_2d(at<double*>(es, 0x20), n, &kind, &a, &flag);
        }
    }
    if (at<double*>(es, 0x30) != nullptr) {  // STNCMP
        const std::int32_t w = at<std::int32_t>(wt, 0xa0);
        if (!(is2 && w != 4 && geotyp == 3)) {
            double* buf = nullptr;
            std::int32_t rem = w % 4;
            std::int32_t bw = 4;
            std::int32_t blocks = w / 4;
            if (is3) {
                rem = w % 6;
                bw = 6;
                blocks = w / 6;
            }
            std::int32_t used = bw * blocks;
            std::int32_t kind = 2;
            double* arr = at<double*>(es, 0x30);
            if (!is2) {
                m = *n * blocks;
                if (rem < 1) {
                    rotate_tensors_3d(arr, &m, &kind, dir, &a, &flag);
                } else {
                    used = bw * blocks;
                    buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(used * *n), 8));
                    for (std::int32_t e = 0; e < *n; ++e)
                        for (std::int32_t k = 0; k < used; ++k) buf[e * used + k] = arr[e * w + k];
                    rotate_tensors_3d(buf, &m, &kind, dir, &a, &flag);
                    for (std::int32_t e = 0; e < *n; ++e)
                        for (std::int32_t k = 0; k < used; ++k) arr[e * w + k] = buf[e * used + k];
                }
            } else if (w == 4) {
                rotate_tensors_2d(arr, n, &kind, &a, &flag);
            } else {
                for (std::int32_t b = 0; b < blocks; ++b) {
                    if (buf != nullptr) {
                        std::free(buf);
                        buf = nullptr;
                    }
                    buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(used * *n), 8));
                    for (std::int32_t e = 0; e < *n; ++e)
                        for (std::int32_t k = 0; k < bw; ++k) buf[e * bw + k] = arr[b * bw + k + e * w];
                    rotate_tensors_2d(buf, n, &kind, &a, &flag);
                    for (std::int32_t e = 0; e < *n; ++e)
                        for (std::int32_t k = 0; k < bw; ++k) arr[b * bw + k + e * w] = buf[e * bw + k];
                }
            }
            if (buf != nullptr) std::free(buf);
        }
    }
    if (at<double*>(es, 0xb8) != nullptr) {  // YLDS
        const std::int32_t w = at<std::int32_t>(wt, 0xe4);
        if (!(is2 && w != 4)) {
            std::int32_t kind = 1;
            if (!is2) rotate_tensors_3d(at<double*>(es, 0xb8), n, &kind, dir, &a, &flag);
            else rotate_tensors_2d(at<double*>(es, 0xb8), n, &kind, &a, &flag);
        }
    }
    if (at<double*>(es, 0x60) != nullptr) {  // MATAXI
        double* ax = at<double*>(es, 0x60);
        if (!is2) {
            if (is3) {
                double z0[3] = {0.0, 0.0, 0.0};
                model::fun_140325691(z0, origin, dir, &a);
                if (at<std::int32_t>(wt, 0xb8) == 9) {
                    for (std::int32_t e = 0; e < *n; ++e) {
                        for (int k = 0; k < 9; k += 3) {
                            double* v = ax + (e * 9 + k);
                            model::fun_140325691(v, origin, dir, &a);
                            rt::ftn::vec3_sub(v, z0, v);  // 14023b1f0
                        }
                    }
                }
            }
        } else {
            for (std::int32_t e = 0; e < *n; ++e) ax[e] = ax[e] + a;
        }
    }
}

} // namespace def::cmd::objpos
