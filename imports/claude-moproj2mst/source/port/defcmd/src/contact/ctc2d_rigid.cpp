// 2D GENCTC Fortran kernels, part 2: angle helpers, arc kernel, nearest segment, rigid master and
// element check (GENCTC.md 4.3, 4.4, 4.6). Operation order from the listings (deform-rea-work
// out/p676/asm.txt). Comparisons follow the COMISD + JP/Jcc pairs, so NaN operands take the same
// branch as in the original.
#include <defcmd/ctc2d_ftn.hpp>
#include <defcmd/ctc2d_msg.hpp>
#include <defcmd/ctc2d_save.hpp>

#include <cmath>

#include <defrt/stdlib/libm.hpp>

namespace def::cmd::ftn {

// @src 140435625  angle from a to b
void fun_140435625(const double* a, const double* b, double* out)
{
    const double ax = a[0];
    const double ay = a[1];
    const double bx = b[0];
    const double by = b[1];
    const double la = std::sqrt(ay * ay + ax * ax); // 140435661: (ay*ay) + (ax*ax)
    const double lb = std::sqrt(by * by + bx * bx); // 140435665
    if (la > 1e-10 && lb > 1e-10) {                 // 14043567f / 1404356b8 (140bb1548)
        const double cosv = ((ay * by + ax * bx) / la) / lb; // 1404356e1
        const double sinv = ((by * ax - bx * ay) / la) / lb; // 1404356e5
        *out = def::rt::libm::ps_atan2(sinv, cosv);          // 1404b3878 (xmm0, xmm1)
        return;
    }
    *out = 0.0;
}

// @src 140435caa  x within the sweep of the arc (start a, centre c, sweep *ang)?
void fun_140435caa(const double* a, const double* c, const double* ang, const double* x,
                   std::int32_t* in)
{
    *in = 0;
    const double v1[2] = {a[0] - c[0], a[1] - c[1]}; // 14070d850
    const double v2[2] = {x[0] - c[0], x[1] - c[1]}; // 14070d860
    double alpha = 0.0;                              // 14070d870
    fun_140435625(v1, v2, &alpha);
    double sw = *ang;                                // 14070d878
    if (0.0 > sw) {
        sw = -sw;
        alpha = -alpha;
    }
    const double tol = 1e-6; // 14062c6a8
    const double ntol = -tol;
    if (!(alpha < ntol) && !std::isnan(alpha)) { // 140435db8 JP/JC -> second test
        const double hi = tol + sw;
        if (alpha <= hi) {                       // 140435dd2 JP -> second test, JBE -> inside
            *in = 1;
            return;
        }
    }
    const double a2 = 3.1415926535898 * 2.0 + alpha; // 14062c6b0 * 140bb14f0, then + alpha
    if (a2 < ntol || std::isnan(a2)) return;
    const double hi = tol + sw;
    if (hi < a2 || std::isnan(hi)) return;
    *in = 1;
}

// @src 14043574f  point to arc
void fun_14043574f(double* p, const double* a, const double* c, const double* ang, double* d,
                   double* ang_out, const std::int32_t* mode)
{
    const double ry = a[1] - c[1];
    const double rx = a[0] - c[0];
    const double cy = c[1];
    const double cx = c[0];
    double e[2] = {rx, ry};       // 14070d7e0: becomes the end point
    const double r[2] = {rx, ry}; // 14070d7d0
    fun_14043556b(e, ang);
    e[0] = cx + e[0];             // 140435818
    e[1] = cy + e[1];             // 14043583a
    const double q[2] = {p[0] - cx, p[1] - cy};             // 14070d800
    const double rq = std::sqrt(q[1] * q[1] + q[0] * q[0]); // 140435885
    const double rr = std::sqrt(rx * rx + ry * ry);         // 14043588a
    const double one = std::fabs(1.0);                      // |14062c698|
    const double sign = (std::isnan(*ang) || !(*ang < 0.0)) ? one : -one; // CMPNLTSD
    *ang_out = *ang;
    *d = 1e10;                         // 140bb1570
    const double h = (rq - rr) * sign; // 14070d818
    fun_140435625(r, q, ang_out);      // 14043597b: RDX = &q, R8 still = ang_out
    const double fx = (q[0] / rq) * rr;
    const double fy = (q[1] / rq) * rr;
    const double f[2] = {fx + cx, fy + cy}; // 14070d820
    std::int32_t in = 0;                    // 14070d830
    fun_140435caa(a, c, ang, f, &in);
    if (in == 1) {
        g_arc_where_14070d834 = 0;
        *d = h;
        if (*mode == 1) return;
        p[0] = fx + c[0];
        p[1] = fy + c[1];
        return;
    }
    const double ay = a[1];
    const double ax = a[0];
    const double dax = p[0] - ax;
    const double day = p[1] - ay;
    const double da = std::sqrt(day * day + dax * dax); // 140435a5e
    const double dex = p[0] - e[0];
    const double dey = p[1] - e[1];
    const double de = std::sqrt(dey * dey + dex * dex); // 140435abb
    const double big = 1e10;                            // 140bb1578
    bool take_end = false;
    if (big > std::fabs(da)) {
        g_arc_where_14070d834 = 1;
        *d = da;
        if (std::fabs(da) > std::fabs(de)) { // 140435afd JA
            take_end = true;
        } else {
            if (*mode == 1) return;
            p[1] = ay;
            p[0] = ax;
            return;
        }
    } else if (big > std::fabs(de)) {
        take_end = true;
    }
    if (take_end) {
        g_arc_where_14070d834 = 2;
        *d = de;
        if (*mode == 1) return;
        p[1] = e[1];
        p[0] = e[0];
        return;
    }
    // 140435b98: both end points 1e10 or farther away: the point the previous call chose
    if (*mode == 1) return;
    if (g_arc_where_14070d834 == 0) {
        p[0] = fx + c[0];
        p[1] = fy + c[1];
    } else if (g_arc_where_14070d834 == 1) {
        p[1] = ay;
        p[0] = ax;
    } else {
        p[1] = e[1];
        p[0] = e[0];
    }
}

// @src 140436a9e  nearest segment
void fun_140436a9e(const std::int32_t* nseg, double* seg, double* p, const double* tol,
                   double* d, std::int32_t* iseg, std::int32_t* st, std::int32_t* p8)
{
    // SAVE locals 14070d940..14070d980 are all written before they are read
    std::int32_t ibest = 0;   // 14070d944
    std::int32_t kmode = 1;   // 14070d948
    double len = 0.0;         // 14070d950
    double dj = 0.0;          // 14070d958
    double s = 0.0;           // 14070d960
    double ang_out = 0.0;     // 14070d968
    const double big = 1e11;  // 140bb1580
    *p8 = 0;
    *d = big;
    auto rec = [&](std::int32_t j) { return seg + 6 * (static_cast<std::int64_t>(j) - 1); };
    if (*nseg < 1) {
        *iseg = 0;
        if (big > *tol) {
            *st = 1;
            return;
        }
        // 140436b4e: with *tol >= 1e11 and no segment the original goes on with record 0 (the
        // 48 bytes before the array). Not reachable from GENCTC, whose tolerances are lengths.
        *st = 2;
        return;
    }
    for (std::int32_t j = 1; j <= *nseg; ++j) { // 14070d94c
        double* r = rec(j);
        double type = r[0];
        if (type == 1.0) { // 14062c700
            fun_1404345c0(p, r + 1, r + 3, &len, &dj, &s, &kmode);
            type = r[0];
        }
        if (type == 2.0) { // 14062c708
            fun_14043574f(p, r + 1, r + 3, r + 5, &dj, &ang_out, &kmode);
        }
        const double a = std::fabs(dj);
        const double b = std::fabs(*d);
        const double diff = a - b; // 14070d970
        const bool far = !(1e-10 >= std::fabs(diff)); // 140436cdd JP/JC -> 140436d03
        if (!far && dj < 0.0) continue;               // 140436cfd
        if (b < a || std::isnan(b) || std::isnan(a)) continue; // 140436d03 JP/JC
        double sign = 1.0;                       // 14070d978 = 140bb14e8
        const double gap = std::fabs(b - a);     // 14070d980
        if (j == ibest + 1 && gap <= 1e-10) {    // 140436d66, 140436d80
            if (0.0 > *d && 0.0 > dj) sign = -1.0; // 140bb1530
            dj = a * sign;                       // 140436bda
        }
        ibest = j;
        *d = dj;
    }
    *iseg = ibest;
    if (*d > *tol) { // 140436dfc JA
        *st = 1;
        return;
    }
    kmode = 2;
    *st = 2;
    double* r = rec(ibest);
    const double type = r[0];
    if (type == 1.0) {
        fun_1404345c0(p, r + 1, r + 3, &len, &dj, &s, &kmode);
    } else if (type == 2.0) {
        fun_14043574f(p, r + 1, r + 3, r + 5, &dj, &ang_out, &kmode);
    }
}

// @src 140350f12  rigid master
void fun_140350f12(double* xy, const std::int32_t* n, const std::int32_t* nodes,
                   const std::int32_t* nseg, double* seg, const std::int32_t* /*p6*/,
                   const std::int32_t* /*p7*/, const std::int32_t* mobj, std::int32_t* bc3,
                   const double* tol, std::int32_t* changed, const std::int32_t* do_codes,
                   std::int32_t* fail)
{
    if (*n < 1) return;
    const double lim = std::fabs(*tol * 100.0);  // 140baf860, abs by the mask 140baf848
    for (std::int32_t k = 1; k <= *n; ++k) {     // 140703750
        const std::int32_t node = nodes[k - 1];  // 140703754
        double tol_copy = *tol;                  // 140703758
        double* pt = xy + 2 * (static_cast<std::int64_t>(node) - 1);
        const double x0 = pt[0];
        const double y0 = pt[1];
        double d = 0.0;        // 140703780
        std::int32_t iseg = 0; // 140703788
        std::int32_t st = 0;   // 14070378c
        std::int32_t p8 = 0;   // 140703790
        fun_140436a9e(nseg, seg, pt, &tol_copy, &d, &iseg, &st, &p8);
        double moved = 0.0;    // 140703798
        if (st == 2) {
            const double dx = pt[0] - x0;          // 140703760
            const double dy = pt[1] - y0;          // 140703768
            moved = std::sqrt(dy * dy + dx * dx);  // 140351334
        }
        if (moved > lim) { // 140351148 JA
            ctc2d_warn_1004(node, *mobj, iseg, d);
            pt[0] = x0;
            pt[1] = y0;
            *fail = 1;
            continue;
        }
        if (*tol * -100.0 > d) { // 140baf868; 140351176 JP/JBE -> code update
            ctc2d_warn_1002(node, *mobj, iseg, d);
            pt[0] = x0;
            pt[1] = y0;
            *fail = 1;
            continue;
        }
        if (st != 2) continue;
        *changed = 1;
        if (*do_codes == 0) continue;
        std::int32_t* c = bc3 + 3 * (static_cast<std::int64_t>(node) - 1);
        const std::int32_t kc = -*mobj;
        if (kc == c[0] || kc == c[1]) continue;
        if (c[1] == 0) {
            c[1] = kc;
        } else if (c[1] < 1) {
            if (c[0] < 1) c[0] = c[1];
            c[1] = kc;
        } else if (c[0] < 1) {
            c[0] = kc;
        } else {
            ctc2d_warn_1001(node, c[0], c[1]);
        }
    }
}

// @src 14032e34e  element check
void fun_14032e34e(const std::int32_t* e, const std::int32_t* conn, const double* xy,
                   std::int32_t* ok, std::int32_t* p5)
{
    const std::int32_t* ce = conn + 4 * (static_cast<std::int64_t>(*e) - 1);
    auto x = [&](int i) { return xy[2 * (static_cast<std::int64_t>(ce[i - 1]) - 1)]; };
    auto y = [&](int i) { return xy[2 * (static_cast<std::int64_t>(ce[i - 1]) - 1) + 1]; };
    const double t1 = (y(1) - y(2)) * (x(3) - x(4));
    const double t2 = (y(3) - y(4)) * (x(1) - x(2));
    const double t3 = (y(2) - y(3)) * (x(1) - x(4));
    const double t4 = (y(1) - y(4)) * (x(2) - x(3));
    const double t5 = (y(1) - y(3)) * (x(2) - x(4));
    const double t6 = (y(2) - y(4)) * (x(1) - x(3));
    *p5 = 0;
    const double cc = (t1 - t2) * -0.57735026918963; // 14032e52d, 140baebe0
    const double bb = t6 - t5;                       // 14032e531
    const double aa = (t4 - t3) * 0.57735026918963;  // 14032e540, 140baebe8
    const double v = (aa + (bb + cc)) * 0.125;       // 14032e544, 14032e548, 140baebf0
    *ok = (v > 0.0) ? 1 : 0;
}

} // namespace def::cmd::ftn
