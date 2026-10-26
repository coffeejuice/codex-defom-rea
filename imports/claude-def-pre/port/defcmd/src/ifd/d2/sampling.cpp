// defcmd/src/ifd/d2/sampling.cpp: Fortran parts of the 2D interference kernel around the gap
// kernels: context setters, the two samplers, overlap area, shoelace area, the area root finder
// and the loop rotation 14033f2bf. Spec: OBJPOS_interference.md 7.2-7.4, kernels spec 4.1, 4.2,
// 4.6. Orders and compares from the listings (out/p672/all_lst.txt).
#include "d2.hpp"

#include <bit>
#include <cstdint>
#include <cstdio>

#include <defrt/ftn/helpers.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ifd::d2 {

namespace {

using rt::sse::add;
using rt::sse::div;
using rt::sse::mul;
using rt::sse::sub;

double fab(double x) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffffull); }
double neg(double x) { return std::bit_cast<double>(std::bit_cast<std::uint64_t>(x) ^ 0x8000000000000000ull); }

// @data 140baec80  1e10 "no gap"
constexpr double kNoGap = 1e10;
// @data 140baec90  -1e-15 negative-gap cut
constexpr double kNegCut = -1e-15;
// @data 140baec98  (double)1e-6f zero-length line
constexpr double kZeroLen = 9.999999974752427e-07;
// @data 1405f6010  1e-6 tie tolerance of 14033f2bf
constexpr double kTie = 1e-6;

// Fortran Iw edit descriptor (right-justified, '*' fill when it does not fit)
void put_iw(std::FILE* f, std::int32_t w, std::int32_t v)
{
    char buf[32];
    const int n = std::snprintf(buf, sizeof buf, "%d", v);
    if (n > w) {
        for (std::int32_t i = 0; i < w; ++i) std::fputc('*', f);
    } else {
        std::fprintf(f, "%*s", w, buf);
    }
}

// The Fortran formatted WRITE of 140340273 (1404b5ef4) with format 1001 (str 1405f6190):
//   1001(4X,'WARNING: Zero length entity ',I4,' on object ',I3,     /,4X,'         Check geometry before continuing')
// on the unit at 140b847a4. That unit is never assigned (0); PathScale unit 0 is stderr
// [inferred, as GENCTC.md]. Reproduced as the two records it writes.
void write_zero_length(std::int32_t rec, std::int32_t obj)
{
    std::FILE* f = g_msg_unit == 6 ? stdout : stderr;
    std::fputs("    WARNING: Zero length entity ", f);
    put_iw(f, 4, rec);
    std::fputs(" on object ", f);
    put_iw(f, 3, obj);
    std::fputs("\n    " "         Check geometry before continuing\n", f);
}

} // namespace

void reset_state()
{
    g_skip_negative = 0;
    g_geoerr1_in = 0.0;
    g_geoerr2_in = 0.0;
    g_geoerr1 = 0.0;
    g_geoerr2 = 0.0;
    g_ext = 0.0;
    g_tol_override = 0.0;
    for (double& v : g_box) v = 0.0;
    g_msg_unit = 0;
}

// @src 1403460c2  store GEOERR 1/2 (model+0xdb0 / +0xdb8)
void fun_1403460c2(double* e1, double* e2)
{
    g_geoerr1_in = *e1;
    g_geoerr2_in = *e2;
}

// @src 1403460e7  ext and the GEOERR copies used by the kernels
void fun_1403460e7(double* e)
{
    g_ext = *e;
    g_geoerr1 = g_geoerr1_in;
    g_geoerr2 = g_geoerr2_in;
}

// @src 14033ffb8  skip negative gaps when flag == 0
void fun_14033ffb8(std::int32_t* flag)
{
    g_skip_negative = 0;
    if (*flag != 0) return;
    g_skip_negative = 1;
}

// @src 14034c209  box and ext = max(ymax - ymin, xmax - xmin)
void fun_14034c209(double* xmin, double* ymin, double* xmax, double* ymax)
{
    g_ext = sub(*ymax, *ymin);
    const double dx = sub(*xmax, *xmin);
    g_box[2] = *xmax;
    g_box[3] = *ymax;
    g_box[0] = *xmin;
    if (g_ext <= dx) g_ext = dx;
    g_box[1] = *ymin;
}

// @src 14033ffe0  mesh boundary sampler: g(i+1) += the gap of smallest magnitude (spec 4.1)
void fun_14033ffe0(std::int32_t* /*objnum*/, double* pts, std::int32_t* np, double* yref,
                   double* x0, double* x1, std::int32_t* n, double* dir, double* g)
{
    const std::int32_t nn = *n;
    double q[2] = {0.0, *yref}; // 140702300 / 140702308
    if (nn + 1 < 1) return;
    const double dx = div(sub(*x1, *x0), static_cast<double>(nn));
    const std::int32_t nseg = *np - 1;
    double zeros[4] = {0.0, 0.0, 0.0, 0.0}; // 1405f6030
    for (std::int32_t i = 0; i <= nn; ++i) {
        q[0] = add(mul(dx, static_cast<double>(i)), *x0);
        double best = kNoGap; // 140702318
        for (std::int32_t k = 1; k <= nseg; ++k) {
            double gap = best; // 140702328
            std::int32_t f = 0;
            fun_14030bba0(pts + 2 * (k - 1), zeros, q, dir, &gap, &f);
            if (g_skip_negative == 1 && kNegCut > gap) continue;
            if (fab(best) > fab(gap)) best = gap;
            if (best == 0.0) break;
        }
        g[i] = add(g[i], best);
    }
}

// @src 140340273  die curve sampler: g(i+1) += the smallest signed gap (spec 4.2)
void fun_140340273(std::int32_t* objnum, double* recs, std::int32_t* nrec, double* yref, double* x0,
                   double* x1, std::int32_t* n, double* dir, double* g, std::int32_t* err)
{
    double tola = g_tol_override; // 140702338
    *err = 0;
    if (g_tol_override == 0.0) tola = mul(g_ext, g_geoerr2);
    double q[2] = {0.0, *yref}; // 140702340 / 140702348
    const double dx = div(sub(*x1, *x0), static_cast<double>(*n)); // 140702350
    if (*nrec >= 1) {
        for (std::int32_t k = 1; k <= *nrec; ++k) {
            double* r = recs + 6 * (k - 1);
            if (static_cast<std::int32_t>(r[0]) != 1) continue;
            const double a = sub(r[1], r[3]);
            const double b = sub(r[2], r[4]);
            const double l = rt::sse::sqrt(add(mul(a, a), mul(b, b)));
            if (kZeroLen > l) {
                write_zero_length(k, *objnum);
                *err = 1;
            }
        }
        if (*err == 1) return;
    }
    const std::int32_t nn = *n;
    double zeros[4] = {0.0, 0.0, 0.0, 0.0}; // 1405f6210
    for (std::int32_t i = 0; i <= nn; ++i) {
        q[0] = add(mul(static_cast<double>(i), dx), *x0);
        double best = kNoGap; // 140702380
        for (std::int32_t k = 1; k <= *nrec; ++k) {
            double* r = recs + 6 * (k - 1);
            double gap = best; // 140702388
            std::int32_t f = 0;
            if (static_cast<std::int32_t>(r[0]) == 1) {
                fun_14030bba0(r + 1, zeros, q, dir, &gap, &f);
            } else {
                fun_1403407d7(r + 1, r + 3, r + 5, q, dir, &gap, &tola, &f);
            }
            if (g_skip_negative == 1 && kNegCut > gap) continue;
            if (best > gap) best = gap;
            if (best == 0.0) break;
        }
        g[i] = add(g[i], best);
    }
}

// @src 14033fe8e  A(s) = sum ((max(0, s-g_i) + max(0, s-g_i+1)) * dx) * 0.5 + acc
double fun_14033fe8e(std::int32_t* n, double* dx, double* g, double* s)
{
    double acc = 0.0;
    for (std::int32_t i = 1; i <= *n; ++i) {
        double m1 = sub(*s, g[i - 1]); // 1407022f0
        double m2 = sub(*s, g[i]);     // 1407022f8
        if (0.0 > m1) m1 = 0.0;
        if (0.0 > m2) m2 = 0.0;
        acc = add(mul(mul(add(m1, m2), *dx), 0.5), acc);
    }
    return acc;
}

// @src 14033fdb8  shoelace: S = ((x_k y_k-1) + S) - x_k-1 y_k ...; returns S * 0.5
double fun_14033fdb8(std::int32_t* n, double* p)
{
    double s = 0.0;
    for (std::int32_t k = 2; k <= *n; ++k) {
        s = sub(add(mul(p[2 * (k - 1)], p[2 * (k - 2) + 1]), s), mul(p[2 * (k - 2)], p[2 * (k - 1) + 1]));
    }
    const std::int32_t m = *n;
    s = sub(add(mul(p[2 * (m - 1) + 1], p[0]), s), mul(p[2 * (m - 1)], p[1]));
    return mul(s, 0.5);
}

// @src 14033f8d4  overlap-area root: bracket by steps, then Illinois false position (IF 7.3)
void fun_14033f8d4(double* step, double* atol, double* g, std::int32_t* n, double* dx, double* s,
                   std::int32_t* err)
{
    std::int32_t ld = 1, cnt = *n + 1, col = 1, init = 1;
    *err = 0;
    double gmin = 0.0, gmax = 0.0; // 140702288 / 140702290
    rt::ftn::column_min_max(g, &ld, &cnt, &col, &gmin, &gmax, &init);
    if (gmin >= kNoGap) {
        *err = 1;
        return;
    }
    double alo = 0.0;  // 1407022a0
    double lo = gmin;  // 140702298
    *s = add(*step, gmin);
    double a = 0.0;    // 1407022a8
    for (;;) {
        a = fun_14033fe8e(n, dx, g, s);
        if (*atol == a) return;
        if (!(*atol > a)) break;
        alo = a;
        lo = *s;
        *s = add(*s, *step);
    }
    double hi = *s;      // 1407022b0
    double ahi = a;      // 1407022b8
    double side = -1.0;  // 1407022c0
    std::int32_t iter = 0; // 1407022c8
    for (;;) {
        ++iter;
        if (iter > 100) {
            *err = 1;
            return;
        }
        const double f = div(sub(ahi, *atol), sub(ahi, alo)); // 1407022d0
        const double w = sub(hi, lo);
        *s = sub(hi, mul(f, w));
        a = fun_14033fe8e(n, dx, g, s);
        // @data 1405f6018  1e-3, 1405f6020  1e-5
        if (1e-3 >= fab(div(sub(*atol, a), *atol))) return;
        if (1e-5 >= fab(w)) return;
        const double t = *atol;
        const double slo = !(sub(alo, t) < 0.0) ? 1.0 : -1.0;
        const double sg = !(sub(a, t) < 0.0) ? 1.0 : -1.0; // 1407022d8
        if (sg == slo) {
            alo = a;
            lo = *s;
            if (side == sg) ahi = mul(add(t, ahi), 0.5);
        } else {
            ahi = a;
            hi = *s;
            if (side == sg) alo = mul(add(t, alo), 0.5);
        }
        side = sg;
    }
}

// @src 14033f2bf  rotate the loop nodes nodes(range(1)..range(2)-1) of xy by theta into out,
// track the x extremes (side-dependent tie rule) and y range, area = -(shoelace), then copy the
// points from the extreme node (side 1: x-min to x-max, side 2: x-max to x-min) back into xy
void fun_14033f2bf(std::int32_t* side, double* xy, double* out, std::int32_t* range,
                   std::int32_t* nodes, std::int32_t* count, double* area, double* xmm, double* ymm,
                   double* theta)
{
    double sn = 0.0, cs = 0.0;
    rt::libm::ps_sincos(*theta, &sn, &cs); // 1404b3b30
    // @data 140baec50/58/60/68  1e11, 1e11, -1e11, -1e11
    double xmin = 1e11, xmax = -1e11, ymin = 1e11, ymax = -1e11;
    ymm[0] = 1e11;
    std::int32_t imin = 1; // 140702258
    ymm[1] = -1e11;
    std::int32_t imax = 1; // 14070225c
    xmm[0] = 1e11;
    xmm[1] = -1e11;
    const std::int32_t last = range[1] - 1;
    *count = 0;
    for (std::int32_t k = range[0]; k <= last; ++k) {
        const std::int32_t node = nodes[k - 1];
        const std::int32_t c = *count + 1;
        *count = c;
        const double x = xy[2 * (node - 1)];
        const double y = xy[2 * (node - 1) + 1];
        const double xr = sub(mul(cs, x), mul(sn, y));
        const double yr = add(mul(x, sn), mul(y, cs));
        out[2 * (c - 1)] = xr;
        out[2 * (c - 1) + 1] = yr;
        double d = sub(xmin, xr);                  // 140702268
        double dy = sub(out[2 * (imin - 1) + 1], yr); // 140702270
        if (d > kTie) {
            imin = c;
            xmin = xr;
            xmm[0] = xr;
        } else if (kTie > fab(d) &&
                   ((*side == 1 && dy > kTie) || (*side == 2 && neg(dy) > kTie))) {
            imin = *count;
            xmin = xr;
            xmm[0] = xr;
        }
        d = sub(xr, xmax);
        dy = sub(out[2 * (imax - 1) + 1], yr);
        if (d > kTie) {
            imax = *count;
            xmm[1] = xr;
            xmax = xr;
        } else if (kTie > fab(d) &&
                   ((*side == 1 && dy > kTie) || (*side == 2 && neg(dy) > kTie))) {
            imax = *count;
            xmax = xr;
            xmm[1] = xr;
        }
        if (ymin > yr) {
            ymin = yr;
            ymm[0] = yr;
        }
        if (yr > ymax) {
            ymax = yr;
            ymm[1] = yr;
        }
    }
    *area = neg(fun_14033fdb8(count, out));
    if (*side == 2) {
        const std::int32_t t = imin;
        imin = imax;
        imax = t;
    }
    std::int32_t m = 0;      // 14070227c
    std::int32_t cur = imin; // 140702280
    for (;;) {
        for (;;) {
            ++m;
            xy[2 * (m - 1)] = out[2 * (cur - 1)];
            xy[2 * (m - 1) + 1] = out[2 * (cur - 1) + 1];
            if (imax == cur) {
                *count = m;
                return;
            }
            cur = cur + 1;
            if (*count < cur) break;
            if (imin == cur) {
                *count = m;
                return;
            }
        }
        cur = 1;
        if (imin == 1) break;
    }
    *count = m;
}

} // namespace def::cmd::ifd::d2
