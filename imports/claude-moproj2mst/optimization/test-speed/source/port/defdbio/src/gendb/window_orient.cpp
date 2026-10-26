// Orientation of polygon windows (packet p4-02a): 140265bf0 and the 3-vector helpers it uses.
// The vectors are 24-byte values returned through a hidden first argument in the binary; the
// port keeps that shape (out pointer returned).
#include <defdbio/gendb/obj_counts.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defrt/rdata.hpp>

#include "raw.hpp"

namespace def::dbio::gendb {

using raw::at;

namespace {

ObjCountExt& ext() { return obj_count_ext(); }

// rdata 14051f260  1e-12
constexpr double kTiny = def::rt::rdata::d_14051f260;

// MSVC CRT fabs (140484558); the port calls std::fabs (CRT, not ported).
double crt_fabs(double x) { return std::fabs(x); }

// 14025c930: LLWINDEF_DATA+0x898 == 2 (a 2D window)
bool window_is_2d(const void* win) { return at<std::int32_t>(win, 0x898) == 2; }

} // namespace

// @src 140239700  (x, y, z)
double* vec3_make(double* out, double x, double y, double z)
{
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return out;
}

// @src 1402399b0  x*x + y*y + z*z (left to right)
double vec3_dot_self(const double* v) { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }

// @src 140239a30  |v|: sqrt of 1402399b0 (MSVC CRT sqrt 140484644, correctly rounded like
// std::sqrt; not ported)
double vec3_norm(const double* v) { return std::sqrt(vec3_dot_self(v)); }

// @src 140239b40  v / |v| (140239a30), or 0 when |v| <= 1e-12
double* vec3_normalized(double* out, const double* v)
{
    double r[3] = {v[0], v[1], v[2]};
    const double len = ext().vec3_length(v);
    if (kTiny < crt_fabs(len)) {
        r[0] = r[0] / len;
        r[1] = r[1] / len;
        r[2] = r[2] / len;
    } else {
        r[2] = 0.0;
        r[1] = 0.0;
        r[0] = 0.0;
    }
    std::memcpy(out, r, 0x18);
    return out;
}

// @src 140239fb0  a - b
double* vec3_sub(double* out, const double* a, const double* b)
{
    const double r[3] = {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
    std::memcpy(out, r, 0x18);
    return out;
}

// @src 14023a1b0  a + b
double* vec3_add(double* out, const double* a, const double* b)
{
    const double r[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
    std::memcpy(out, r, 0x18);
    return out;
}

// @src 14023a4b0  a x b (each component as a1*b2 - a2*b1, the order of the SUBSD operands)
double* vec3_cross(double* out, const double* a, const double* b)
{
    const double r[3] = {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
    std::memcpy(out, r, 0x18);
    return out;
}

// @src 140240d80  unit normal of the polygon pts[0..n-1]: the sum over i = 2 .. n-1 of
// (p[i-1] - p0) x (p[i] - p0), accumulated as sum + term, normalised (140239b40)
double* polygon_normal(double* out, const double* pts, std::int32_t n)
{
    double sum[3];
    vec3_make(sum, 0.0, 0.0, 0.0);
    double e[3];
    vec3_sub(e, pts + 3, pts);
    for (std::int32_t i = 2; i <= n - 1; ++i) {
        double d[3];
        vec3_sub(d, pts + static_cast<std::int64_t>(i) * 3, pts);
        double c[3];
        vec3_cross(c, e, d);
        vec3_add(sum, sum, c);
        vec3_make(e, d[0], d[1], d[2]);
    }
    double r[3];
    vec3_normalized(r, sum);
    std::memcpy(out, r, 0x18);
    return out;
}

// @src 140265bf0  1 when the window's polygon (+0x178 points at +0x180) is counter-clockwise:
// 2D its signed area (140309550) >= 0, 3D every component of its normal (140240d80, components
// below 1e-12 in magnitude taken as 0) >= 0. Otherwise, when *reverse is set, the point order is
// reversed in place (copy through 140257150); returns 0. No points: 0.
std::uint8_t orient_window(void* win, const std::uint8_t* reverse)
{
    double area = 0.0;
    double nrm[3] = {0.0, 0.0, 0.0};
    const std::int32_t n = at<std::int32_t>(win, 0x178);
    double*& pts = at<double*>(win, 0x180);
    if (!(0 < n))
        return 0;
    if (!window_is_2d(win)) {
        // operator new[](n * 0x18) (thunk 1404797d8; the size saturates on overflow)
        auto* tmp = static_cast<double*>(::operator new(static_cast<std::size_t>(static_cast<std::int64_t>(n)) * 0x18));
        for (std::int32_t i = 0; i < n; ++i) {
            tmp[i * 3] = pts[i * 3];
            tmp[i * 3 + 1] = pts[i * 3 + 1];
            tmp[i * 3 + 2] = pts[i * 3 + 2];
        }
        double r[3];
        polygon_normal(r, tmp, n);
        std::memcpy(nrm, r, 0x18);
        std::free(tmp);  // the binary frees with free()
        if (crt_fabs(nrm[0]) < kTiny)
            nrm[0] = 0.0;
        if (crt_fabs(nrm[1]) < kTiny)
            nrm[1] = 0.0;
        if (crt_fabs(nrm[2]) < kTiny)
            nrm[2] = 0.0;
    } else {
        area = model::geo2d::polygon_area(n, pts);
    }
    // DAT_14051bed8 = 0.0
    if (!window_is_2d(win)) {
        if (0.0 <= nrm[0] && 0.0 <= nrm[1] && 0.0 <= nrm[2])
            return 1;
    } else if (0.0 <= area) {
        return 1;
    }
    if (*reverse != 0) {
        if (!window_is_2d(win)) {
            double* rev = ext().alloc_doubles(n * 3);
            for (std::int32_t i = 0; i < n; ++i)
                std::memcpy(rev + static_cast<std::int64_t>(((n - 1) - i) * 3), pts + static_cast<std::int64_t>(i * 3), 0x18);
            std::memcpy(pts, rev, static_cast<std::size_t>(static_cast<std::int64_t>(n * 3)) << 3);
            if (rev != nullptr)
                std::free(rev);
        } else {
            double* rev = ext().alloc_doubles(n << 1);
            for (std::int32_t i = 0; i < n; ++i)
                std::memcpy(rev + static_cast<std::int64_t>(((n - 1) - i) * 2), pts + static_cast<std::int64_t>(i << 1), 0x10);
            std::memcpy(pts, rev, static_cast<std::size_t>(static_cast<std::int64_t>(n << 1)) << 3);
            if (rev != nullptr)
                std::free(rev);
        }
    }
    return 0;
}

} // namespace def::dbio::gendb
