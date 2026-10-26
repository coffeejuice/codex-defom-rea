// defcmd/src/ifd/d2/driver.cpp: the 2D interference driver 1401a0c70 and kernel 1401a1370 of
// OBJPOS modes 2/12/102/112 (spec OBJPOS_interference.md 7.1, 7.2; decompile and listing in
// deform-rea-work/out/p672). Both are MSVC C++.
#include "d2.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defcmd/ifd/ifd2d.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ifd {

namespace d2 {

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

double* alloc_doubles(std::int64_t n) { return static_cast<double*>(std::malloc(static_cast<std::size_t>(n) * 8)); }

} // namespace

// @src 1401a1370  2D kernel (IF 7.2): S = target, T = mover, d = direction, tol = area percent,
// n = 1000 samples, flag 1 = back T off behind S first. Returns 0 with out = displacement of T
// (in the original frame), 1 on failure. The original leaks its buffers on the early returns;
// the port frees them (not observable).
std::int32_t fun_1401a1370(DB_STEP_DATA* m, LLOBJ_DATA* s, LLOBJ_DATA* t, double* d, double tol,
                           std::int32_t n, double* out, std::int32_t flag)
{
    std::int32_t num_s = at<std::int32_t>(s, 8); // local_328
    std::int32_t num_t = at<std::int32_t>(t, 8); // local_300
    std::int32_t kind_s = -1, kind_t = -1;       // local_318, local_3a0
    std::int32_t cnt_s = -1, cnt_t = -1;         // local_38c[1], local_3b0
    double area_s = 0.0, area_t = 0.0;           // local_2e8, local_348
    double xs[2] = {0.0, 0.0}, ys[2] = {0.0, 0.0}; // local_250/248, local_380/378
    double xt[2] = {0.0, 0.0}, yt[2] = {0.0, 0.0}; // local_340/338, local_238/230
    double* g = alloc_doubles(static_cast<std::int64_t>(n) + 1); // local_270
    std::unique_ptr<OBJ_GEO2D> curve_s(new OBJ_GEO2D());          // local_278
    std::unique_ptr<OBJ_GEO2D> curve_t(new OBJ_GEO2D());          // puVar7
    double* buf_s = nullptr; // local_2d0
    double* xy_s = nullptr;  // local_258
    double* buf_t = nullptr; // local_240
    double* xy_t = nullptr;  // local_3a8
    auto release = [&]() {
        std::free(g);
        std::free(xy_t);
        std::free(buf_t);
        std::free(xy_s);
        std::free(buf_s);
    };
    std::int32_t loop = -1;
    if (fun_1401ffa90(s, &loop)) {
        void* rec = &at<std::byte>(s, 0xbe0 + static_cast<std::ptrdiff_t>(loop - 1) * 0x30);
        buf_s = alloc_doubles(static_cast<std::int64_t>(at<std::int32_t>(rec, 0) * 2));
        const std::int32_t nn = at<std::int32_t>(s, 0x880);
        xy_s = alloc_doubles(static_cast<std::int64_t>(nn * 2));
        std::memcpy(xy_s, fun_1401808d0(&at<std::byte>(s, 0x8a0)), static_cast<std::size_t>(nn * 2) * 8);
    }
    loop = -1;
    if (fun_1401ffa90(t, &loop)) {
        void* rec = &at<std::byte>(t, 0xbe0 + static_cast<std::ptrdiff_t>(loop - 1) * 0x30);
        buf_t = alloc_doubles(static_cast<std::int64_t>(at<std::int32_t>(rec, 0) * 2));
        const std::int32_t nn = at<std::int32_t>(t, 0x880);
        xy_t = alloc_doubles(static_cast<std::int64_t>(nn * 2));
        std::memcpy(xy_t, fun_1401808d0(&at<std::byte>(t, 0x8a0)), static_cast<std::size_t>(nn * 2) * 8);
    }
    const double dx = d[0], dy = d[1];
    // rdata 140509c90  1.0, 140509fb8  2.0
    double theta = 2.0 * rt::libm::crt_atan(1.0); // 1404865ac
    const double l = std::sqrt(dx * dx + dy * dy);
    theta = theta - rt::libm::crt_atan2(dy / l, dx / l); // 140485d6c
    fun_1401a28c0(m, s, curve_s.get(), xy_s, buf_s, 1, &kind_s, &theta, &cnt_s, xs, ys, &area_s);
    fun_1401a28c0(m, t, curve_t.get(), xy_t, buf_t, 2, &kind_t, &theta, &cnt_t, xt, yt, &area_t);
    double ref;
    if (kind_s == 2 && kind_t == 2) {
        ref = (area_t <= area_s) ? area_t : area_s;
    } else if (kind_s == 2) {
        ref = area_s;
    } else if (kind_t == 2) {
        ref = area_t;
    } else {
        const double a = std::fabs(xs[1] - xs[0]);
        const double b = std::fabs(ys[1] - ys[0]);
        ref = a * b;
        // rdata 14051bed8  0.0, 14051bee8  10.0
        if (a * b == 0.0) ref = 10.0;
    }
    // rdata 140524cd8  100.0
    double atol = (ref * tol) / 100.0; // local_2f0
    double yref_s;     // local_298
    double yref_t = 0; // local_2f8
    double s0;         // local_350
    if (flag == 1) {
        yref_s = ys[0];
        yref_t = ys[0];
        s0 = ys[0] - yt[1];
        double shift[2] = {0.0, s0}; // local_310
        if (kind_t == 1) {
            double* p = curve_t->points;
            for (std::int32_t r = 0; r < cnt_t; ++r) {
                for (std::int32_t j = 1; j < 5; ++j) p[r * 6 + j] = p[r * 6 + j] + shift[(j + 1) & 1];
            }
        } else {
            for (std::int32_t r = 0; r < cnt_t; ++r) {
                for (std::int32_t c = 0; c < 2; ++c) xy_t[c + r * 2] = xy_t[c + r * 2] + shift[c];
            }
        }
    } else {
        yref_s = yt[1];
        s0 = 0.0;
    }
    double x0 = (xs[0] <= xt[0]) ? xt[0] : xs[0]; // local_368
    double x1 = (xt[1] <= xs[1]) ? xt[1] : xs[1]; // local_330
    if (!(x0 < x1)) {
        release();
        return 1;
    }
    yref_t = yref_s;
    // rdata 14051f260  1e-12
    if (!(1e-12 <= std::fabs(x1 - x0))) {
        release();
        return 1;
    }
    std::memset(g, 0, static_cast<std::size_t>(static_cast<std::int64_t>(n) + 1) * 8);
    fun_1403460c2(&at<double>(m, 0xdb0), &at<double>(m, 0xdb8));
    double ext;
    if (std::fabs(xs[1] - xs[0]) <= std::fabs(ys[1] - ys[0])) {
        ext = std::fabs(ys[1] - ys[0]);
    } else {
        ext = std::fabs(xs[1] - xs[0]);
    }
    fun_1403460e7(&ext);
    std::int32_t err = 0; // local_390
    double dir[2] = {0.0, 1.0}; // local_2c0 / 2b8
    fun_14033ffb8(&flag);
    if (kind_s == 1) {
        fun_140340273(&num_s, curve_s->points, &cnt_s, &yref_s, &x0, &x1, &n, dir, g, &err);
    } else {
        fun_14033ffe0(&num_s, xy_s, &cnt_s, &yref_s, &x0, &x1, &n, dir, g);
    }
    if (!(err < 1)) {
        release();
        return 1;
    }
    err = 0;
    dir[0] = 0.0;
    dir[1] = -1.0; // 140522fc8
    if (kind_t == 1) {
        fun_140340273(&num_t, curve_t->points, &cnt_t, &yref_t, &x0, &x1, &n, dir, g, &err);
    } else {
        fun_14033ffe0(&num_t, xy_t, &cnt_t, &yref_t, &x0, &x1, &n, dir, g);
    }
    if (!(err < 1)) {
        release();
        return 1;
    }
    if (flag == 0) {
        const double dd = yref_t - yref_s; // local_210 (0.0: both references are equal)
        for (std::int32_t i = 0; i < n + 1; ++i) g[i] = g[i] - dd;
    }
    double dxs = (x1 - x0) / static_cast<double>(n); // local_288
    double rng = (ys[1] - ys[0] <= yt[1] - yt[0]) ? yt[1] - yt[0] : ys[1] - ys[0];
    // @data 14051bef8  0.1, 14051f258  0.01
    double step = rng * 0.1; // local_2b0
    if (step < 0.01) step = 0.01;
    double sroot = 0.0;   // local_290
    std::int32_t e2 = 0;  // local_268
    fun_14033f8d4(&step, &atol, g, &n, &dxs, &sroot, &e2);
    if (e2 == 1) {
        release();
        return 1;
    }
    const double zero = 0.0; // local_320
    s0 = s0 + sroot;
    const double c = rt::libm::crt_cos(theta); // 140bb3490
    const double sn = rt::libm::crt_sin(theta); // 140bb3000
    out[0] = zero * c + s0 * sn;
    out[1] = -zero * sn + s0 * c; // sign-bit xor of 0.0 (14051bec0)
    release();
    return 0;
}

} // namespace d2

// @src 1401a0c70  2D driver (IF 7.1): for b then a, a rigid object whose curve 0 is of type 2
// gets its curves copied and its zero-length segments removed (140306a70) for the kernel; the
// curves are restored afterwards when anything was removed. a = mover, b = target.
bool fun_1401a0c70(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* b, double* dir,
                   double* tol, double* dist, double* disp, char flag)
{
    using model::OBJ_GEO2D;
    namespace gk = model::geo_kw;
    if (b == nullptr || a == nullptr) return false;
    const std::int32_t fl = flag != 0 ? 1 : 0; // local_d0
    std::unique_ptr<OBJ_GEO2D[]> copy_b;  // local_f8
    std::unique_ptr<OBJ_GEO2D[]> copy_a;  // local_c0
    bool changed_b = false, changed_a = false; // local_f0, local_e8
    auto save = [&](model::LLOBJ_DATA* o, std::unique_ptr<OBJ_GEO2D[]>& copy, bool& changed) {
        if (d2::at<std::int32_t>(o, 0xc) == 0 && 0 < gk::curve_count(*o) &&
            gk::curve(*o, 0)->curve_type == 2) {
            const std::int32_t k = gk::curve_count(*o);
            copy.reset(new OBJ_GEO2D[static_cast<std::size_t>(k)]);
            for (std::int32_t i = 0; i < gk::curve_count(*o); ++i) copy[i].copy_from(gk::curve(*o, i)); // vtable +0x20
            changed = d2::fun_140306a70(o);
        }
    };
    save(b, copy_b, changed_b);
    save(a, copy_a, changed_a);
    double r[2] = {0.0, 0.0}; // local_e0 / local_d8
    const std::int32_t rc = d2::fun_1401a1370(m, b, a, dir, *tol, 1000, r, fl);
    if (changed_b && copy_b) {
        for (std::int32_t i = 0; i < gk::curve_count(*b); ++i) d2::fun_14019a620(gk::curve(*b, i), &copy_b[i]);
    }
    if (changed_a && copy_a) {
        for (std::int32_t i = 0; i < gk::curve_count(*a); ++i) d2::fun_14019a620(gk::curve(*a, i), &copy_a[i]);
    }
    copy_b.reset();
    copy_a.reset();
    if (rc != 0) {
        *dist = 0.0;
        return false;
    }
    *dist = std::sqrt(r[0] * r[0] + r[1] * r[1]); // 140484644
    if (disp != nullptr) std::memcpy(disp, r, 0x10);
    // @data 140522f50  1e9
    if (*dist < 1e9) return true;
    *dist = 0.0;
    return false;
}

} // namespace def::cmd::ifd
