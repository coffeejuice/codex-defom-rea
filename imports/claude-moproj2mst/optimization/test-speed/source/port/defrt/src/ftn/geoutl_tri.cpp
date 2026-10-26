// Triangle kernels of the geometry library (defrt/ftn/geoutl_tri.hpp). Operation order and
// comparison senses follow the listings (instruction addresses in the comments where the
// decompile differs); one C++ operation per SSE instruction.
#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/port.hpp>

#include <bit>
#include <cstdint>

namespace def::rt::geoutl {

namespace {

using sse::add;
using sse::mul;
inline double sqrtsd(double x) { return sse::sqrt(x); }

constexpr double kHalf = 0.5;                       // 140bb26b0 3fe0000000000000
constexpr double kOne = 1.0;                        // 140bb2658 3ff0000000000000
constexpr double kThree = 3.0;                      // 140bb2630 4008000000000000
constexpr double kAreaMin = 0x1.cd2b297d889bcp-54;  // 140bb28c8 3c9cd2b297d889bc (1e-16)
constexpr double kBarySum = 0x1.00001p+0;           // 140bb28d0 3ff0000100000000 (1.000001f)
constexpr double kBig4 = 0x1.388p+13;               // 140bb2780 40c3880000000000 (10000)
constexpr double kTHigh = 0x1.fff2e4p-1;            // 140bb2788 3fefff2e40000000 (0.9999f)
constexpr double kTLow = 0x1.a36e2ep-14;            // 140bb2790 3f1a36e2e0000000 (1e-4f)
constexpr double kTolScale = 0x1.47ae14p-7;         // 140bb2880 3f847ae140000000 (0.01f)
constexpr double kTolDefault = 0x1.47ae14p-6;       // 140bb2888 3f947ae140000000 (0.02f)
constexpr double kBig8 = 0x1.7d784p+26;             // 140bb2890 4197d78400000000 (1e8)
constexpr std::uint64_t kAbsMask = 0x7fffffffffffffffull;  // 140bb2638
constexpr std::uint64_t kSignBit = 0x8000000000000000ull;  // 140bb2688

} // namespace

void reset_globals_geoutl_tri() {
    g_proj_drv = ProjDrvState{};
    g_heron = HeronState{};
    g_area3 = Area3State{};
    g_bary = BaryState{};
    g_bary_work = BaryWork{{}, 0.0, {}, 0.0, {}, 0.0, {}, 0.0, 0x1.5555555555555p-2};
    g_proj_tri = ProjTriState{};
    g_bary_pt = BaryPtState{};
    g_tet_vol = TetVolState{};
    g_face_proj = FaceProjState{};
    g_orient = OrientState{};
    g_common_140b84720 = Common140b84720{};
}

// @src 1404e29fa  triangle area, Heron
void fun_1404e29fa(const double* x, double* area) {
    HeronState& g = g_heron;
    std::int32_t r11 = 2;
    g.i = 1;
    g.sum = 0.0;
    double sum;
    for (;;) {
        g.j = r11;
        if (r11 > 3) g.j = 1;
        g.k = 1;
        const std::int64_t ci = (static_cast<std::int64_t>(g.i) - 1) * 3;
        const std::int64_t cj = (static_cast<std::int64_t>(g.j) - 1) * 3;
        g.sumsq = 0.0;
        double acc;
        for (;;) {
            const std::int32_t k = g.k;
            double d = x[k - 1 + ci];
            d = d - x[k - 1 + cj];
            d = d * d;
            acc = add(d, g.sumsq); // 1404e2b2a addsd xmm0, [sumsq]
            g.k = k + 1;
            g.sumsq = acc;
            if (k + 1 > 3) break;
            double e = x[k + ci];
            e = e - x[k + cj];
            e = e * e;
            acc = add(acc, e); // 1404e2b61
            g.k = k + 2;
            g.sumsq = acc;
            if (k + 2 > 3) break;
        }
        if (0.0 > acc) g.sumsq = 0.0; // 1404e2b7a comisd 0, acc
        const double len = sqrtsd(g.sumsq);
        g.len[g.i - 1] = len;
        sum = add(len, g.sum); // 1404e2a6b
        g.i = g.i + 1;
        r11 = r11 + 1;
        g.sum = sum;
        if (g.i > 3) break;
    }
    // 1404e2b93: (s - L3) * ((s - L2) * ((s - L1) * s))  (fp-audit.md correction 5)
    const double s = mul(sum, kHalf);
    double a = s - g.len[0];
    double b = s - g.len[1];
    a = mul(a, s);
    double c = s - g.len[2];
    b = mul(b, a);
    c = mul(c, b);
    *area = c;
    if (0.0 > c) {
        *area = sqrtsd(0.0);
        return;
    }
    *area = sqrtsd(*area);
}

// @src 1404e2d44  planar barycentric coordinates by Heron area ratios, inside flag
void fun_1404e2d44(const double* x, const double* p, double* l1, double* l2, double* l3,
                   std::int32_t* iflag) {
    BaryState& g = g_bary;
    BaryWork& w = g_bary_work;
    g.i = 1;
    *iflag = 0;
    do {
        g.j = 1;
        const std::int64_t ci = (static_cast<std::int64_t>(g.i) - 1) * 3;
        for (;;) { // rows 1..2 only
            const std::int32_t j = g.j;
            g.j = j + 1;
            w.t0[j - 1 + ci] = x[j - 1 + ci];
            if (j + 1 > 2) break;
        }
        g.i = g.i + 1;
    } while (g.i <= 3);
    fun_1404e29fa(w.t0, &g.a0);
    if (kAreaMin > g.a0) *iflag = 1; // 1404e2e7c comisd 1e-16, a0
    g.i = 1;
    do {
        const std::int32_t i = g.i - 1;
        const double x1 = x[i], x2 = x[i + 3], x3 = x[i + 6];
        w.t3[i + 6] = x1;
        w.t3[i + 3] = x2;
        w.t2[i + 6] = x1;
        const double pi = p[i];
        w.t2[i + 3] = x3;
        w.t1[i + 6] = x3;
        w.t1[i + 3] = x2;
        w.t3[i] = pi;
        w.t2[i] = pi;
        g.i = g.i + 1;
        w.t1[i] = pi;
    } while (g.i <= 2);
    fun_1404e29fa(w.t1, &g.a1);
    fun_1404e29fa(w.t2, &g.a2);
    fun_1404e29fa(w.t3, &g.a3);
    const double a0 = g.a0;
    const double r1 = g.a1 / a0;
    const double r2 = g.a2 / a0;
    const double r3 = g.a3 / a0;
    double sum = add(r1, r2);
    sum = add(sum, r3);
    *l3 = r3;
    *l2 = r2;
    *l1 = r1;
    g.sum = sum;
    if (kBarySum > sum) return; // 1404e2fe7 comisd 1.000001f, sum: ja -> return
    *iflag = 1;
}

// @src 1404e34eb  point at barycentric (a, b, 1 - a - b) of X(n,3)
void fun_1404e34eb(const double* x, double* out, const double* a, const double* b,
                   const std::int32_t* n) {
    BaryPtState& g = g_bary_pt;
    const double wa = *a;
    double w3 = kOne - wa;
    const double wb = *b;
    g.i = 1;
    const std::int32_t nn = *n;
    w3 = w3 - wb;
    g.w[1] = wb;
    const std::int64_t ld = nn < 0 ? 0 : static_cast<std::int64_t>(nn);
    g.w[0] = wa;
    g.w[2] = w3;
    if (nn < 1) return;
    do {
        const std::int64_t i = g.i;
        double acc = 0.0;
        g.k = 1;
        out[i - 1] = 0.0;
        for (;;) {
            const std::int32_t k = g.k;
            double t = x[(static_cast<std::int64_t>(k) - 1) * ld + i - 1];
            t = mul(t, g.w[k - 1]);
            acc = add(acc, t);
            g.k = k + 1;
            out[i - 1] = acc;
            if (k + 1 > 3) break;
            double u = x[static_cast<std::int64_t>(k) * ld + i - 1];
            u = mul(u, g.w[k]);
            acc = add(acc, u);
            g.k = k + 2;
            out[i - 1] = acc;
            if (k + 2 > 3) break;
        }
        g.i = g.i + 1;
    } while (g.i <= nn);
}

// @src 1404e2c31  area of the triangle (p1, p2, p3)
void fun_1404e2c31(const double* p1, const double* p2, const double* p3, double* area) {
    Area3State& g = g_area3;
    g.i = 1;
    do {
        const std::int32_t i = g.i - 1;
        const double a = p1[i], b = p2[i], c = p3[i];
        g.x[i] = a;
        g.x[i + 3] = b;
        g.i = i + 2;
        g.x[i + 6] = c;
    } while (g.i <= 3);
    fun_1404e29fa(g.x, area);
}

// @src 1404ca2cb  signed tetrahedron volume
void fun_1404ca2cb(const double* x, double* vol) {
    TetVolState& g = g_tet_vol;
    fun_1404e29fa(x, &g.area);
    fun_1404d3d43(x, g.n);
    double t1 = x[9] - x[0];
    t1 = mul(t1, g.n[0]);
    double t2 = x[10] - x[1];
    t2 = mul(t2, g.n[1]);
    double t3 = x[11] - x[2];
    t3 = mul(t3, g.n[2]);
    t2 = add(t2, t1); // 1404ca351
    t3 = add(t3, t2); // 1404ca355
    g.h = t3;
    t3 = mul(t3, g.area);
    t3 = t3 / kThree;
    *vol = t3;
}

// @src 1404d37d7  2D orientation test of a polygon, optional reversal
void fun_1404d37d7(double* x, std::int32_t* nneg, const std::int32_t* ldim, const std::int32_t* n,
                   const std::int32_t* mode) {
    OrientState& g = g_orient;
    const std::int32_t ld32 = *ldim;
    *nneg = 0;
    g.last = 0;
    g.nloop = 4;
    const std::int64_t ld = ld32 < 0 ? 0 : static_cast<std::int64_t>(ld32);
    if (*n == 3) g.nloop = 1;
    g.i = 1;
    if (g.nloop >= 1) {
        std::int32_t esi = 2;
        std::int32_t ecx = 0;
        for (;;) {
            g.iprev = ecx;
            if (g.i == 1) g.iprev = *n;
            g.inext = esi;
            if (*n < esi) g.inext = 1;
            const double* xi = x + (static_cast<std::int64_t>(g.i) - 1) * ld;
            const double* xp = x + (static_cast<std::int64_t>(g.iprev) - 1) * ld;
            const double* xn = x + (static_cast<std::int64_t>(g.inext) - 1) * ld;
            const double x0 = xi[0];
            double dx1 = x0 - xp[0];
            const double y0 = xi[1];
            g.dx1 = dx1;
            const double dy1 = y0 - xp[1];
            double dx2 = xn[0];
            double dy2 = xn[1];
            dx2 = dx2 - x0;
            dy2 = dy2 - y0;
            double l1 = dy1 * dy1;
            const double dx1sq = dx1 * dx1;
            double dx2sq = dx2 * dx2;
            l1 = add(l1, dx1sq);
            double l2 = dy2 * dy2;
            l1 = sqrtsd(l1);
            l2 = add(l2, dx2sq);
            l2 = sqrtsd(l2);
            double cross = mul(dx1, dy2);
            const double t = mul(dx2, dy1);
            cross = cross - t;
            g.dy1 = dy1;
            g.dy2 = dy2;
            g.dx2 = dx2;
            g.cross = cross;
            g.l1 = l1;
            g.l2 = l2;
            if (l1 == 0.0 || l2 == 0.0) { // 1404d39ca / 1404d3a0a (NaN goes on)
                g.last = g.i;
            } else {
                double c = cross / l1;
                c = c / l2;
                g.cross = c;
                if (0.0 > c) {
                    const std::int32_t cnt = *nneg + 1;
                    g.last = g.i;
                    *nneg = cnt;
                }
            }
            const std::int32_t inext = g.i + 1;
            g.i = inext;
            esi = esi + 1;
            ecx = ecx + 1;
            if (inext > g.nloop) break;
        }
    }
    const std::int32_t m = *mode;
    if (m == 0) return;
    if (g.last == 0) {
        if (m != -1) return;
    } else if (*mode != 1) {
        return;
    }
    g.jsrc = *n;
    g.i = 1;
    if (*n < 1) {
        g.i = 1;
        return;
    }
    const bool rows = ld32 >= 1;
    if (ld32 > 3 || *n > 4) DEF_NOT_PORTED(0x1404d37d7); // T(3,4) of the original would overflow
    do {
        g.k = 1;
        if (rows) {
            const std::int64_t ts = (static_cast<std::int64_t>(g.i) - 1) * 3;
            const std::int64_t xs = (static_cast<std::int64_t>(g.jsrc) - 1) * ld;
            do {
                const std::int32_t k = g.k;
                g.k = k + 1;
                g.t[k - 1 + ts] = x[k - 1 + xs];
            } while (g.k <= ld32);
        }
        g.jsrc = g.jsrc - 1;
        g.i = g.i + 1;
    } while (*n >= g.i);
    g.i = 1;
    do {
        g.k = 1;
        if (rows) {
            const std::int64_t ts = (static_cast<std::int64_t>(g.i) - 1) * 3;
            const std::int64_t xs = (static_cast<std::int64_t>(g.i) - 1) * ld;
            do {
                const std::int32_t k = g.k;
                g.k = k + 1;
                x[k - 1 + xs] = g.t[k - 1 + ts];
            } while (g.k <= ld32);
        }
        g.i = g.i + 1;
    } while (*n >= g.i);
}

// @src 1404d3094  project a point onto a triangle (foot of the normal or nearest edge point)
void fun_1404d3094(const double* x, const double* p, double* q, double* dist, std::int32_t* iflag) {
    FaceProjState& g = g_face_proj;
    g.one = 1;
    g.three = 3;
    *iflag = 0;
    fun_1404d3d43(x, g.n);
    if (g_tri_normal_len == 0.0) { // 1404d312c: jp / jnz go on
        *iflag = 10000;
        return;
    }
    fun_1404cf5c0(g.n, g.r);
    g.i = 1;
    do {
        const std::int32_t i = g.i - 1;
        double acc = 0.0;
        q[i] = 0.0;
        g.j = 1;
        g.c[i] = 0.0;
        for (;;) { // c(i) = sum_j X(i,j), accumulator first (addsd xmm0, [x])
            const std::int32_t j = g.j;
            acc = add(acc, x[(j - 1) * 3 + i]);
            g.j = j + 1;
            g.c[i] = acc;
            if (j + 1 > 3) break;
            acc = add(acc, x[j * 3 + i]);
            g.j = j + 2;
            g.c[i] = acc;
            if (j + 2 > 3) break;
            acc = add(acc, x[(j + 1) * 3 + i]);
            g.j = j + 3;
            g.c[i] = acc;
            if (j + 3 > 3) break;
        }
        g.i = g.i + 1;
    } while (g.i <= 3);
    g.c[2] = g.c[2] / kThree;
    g.c[1] = g.c[1] / kThree;
    g.c[0] = g.c[0] / kThree;
    std::int32_t l1 = 3;
    fun_1404d0939(x, g.c, g.r, g.x, &g.one, &l1);
    l1 = 1;
    fun_1404d0939(p, g.c, g.r, g.p, &g.one, &l1);
    std::int32_t l2 = 1;
    l1 = 3;
    fun_1404d37d7(g.x, &g.nneg, &g.three, &l1, &l2);
    std::int32_t l3 = 0;
    g.dmin = kBig4;
    l1 = 3;
    l2 = 1;
    fun_1404cc289(g.x, g.p, g.nearest, &g.dmin, &g.edge, &l1, &g.inout, &l2, &g.a9, &l3, &g.three);
    if (g.inout <= 0) {
        const double h = g.p[2];
        l1 = 1;
        g.p[2] = 0.0;
        l2 = 2;
        *dist = h;
        fun_1404ded0c(g.p, q, g.c, g.r, &l1, &l2);
        return;
    }
    std::int32_t r12 = 2;
    std::int32_t r13 = -1;
    g.i = 1;
    g.dmin = kBig4;
    for (;;) {
        g.j = r12;
        if (g.i == 3) g.j = 1;
        fun_1404d6b13(x + (static_cast<std::int64_t>(g.i) - 1) * 3,
                      x + (static_cast<std::int64_t>(g.j) - 1) * 3, p, g.q, &g.t, dist);
        const double d = *dist;
        if (g.dmin > d) { // 1404d35a7 comisd dmin, d
            g.dmin = d;
            g.k = 1;
            do {
                const std::int32_t k = g.k;
                g.k = k + 1;
                q[k - 1] = g.q[k - 1];
            } while (g.k <= 3);
            const double t = g.t;
            g.t_best = t;
            *iflag = r13;
            if (t >= kTHigh) *iflag = g.j;   // 1404d3671: jp / jc skip
            if (kTLow >= g.t) *iflag = g.i;  // 1404d369c: jp / jc skip
        }
        g.i = g.i + 1;
        r13 = r13 - 1;
        r12 = r12 + 1;
        if (g.i > 3) break;
    }
    const double dmin = g.dmin;
    *dist = dmin;
    if (0.0 > g.p[2]) { // 1404d36f8
        *dist = std::bit_cast<double>(kSignBit ^ (kAbsMask & std::bit_cast<std::uint64_t>(dmin)));
    }
}

// @src 1404e2914  project onto a triangle, then barycentric coordinates of the projection
void fun_1404e2914(const double* x, const double* p, double* xi, double* eta, double* q,
                   double* dist, std::int32_t* iflag) {
    ProjDrvState& g = g_proj_drv;
    fun_1404d3094(x, p, q, &g.dist, iflag);
    if (*iflag == 10000) {
        *dist = kBig8;
        return;
    }
    fun_1404e305a(x, q, xi, eta, &g.zeta, &g.flag);
    *dist = g.dist;
}

// @src 1404e305a  barycentric coordinates of a point in a triangle's own frame
void fun_1404e305a(const double* x, const double* p, double* l1, double* l2, double* l3,
                   std::int32_t* iflag) {
    ProjTriState& g = g_proj_tri;
    *iflag = 0;
    fun_1404d3d43(x, g.n);
    fun_1404cf5c0(g.n, g.r);
    std::int32_t a = 3;
    fun_1404e34eb(x, g.c, &g_bary_work.third, &g_bary_work.third, &a);
    std::int32_t b = 1;
    a = 1;
    fun_1404ded0c(p, g.p, g.c, g.r, &a, &b);
    a = 3;
    b = 1;
    fun_1404ded0c(x, g.x, g.c, g.r, &a, &b);
    g.tol = g_common_140b84720.u08;
    if (g_common_140b84720.u08 == 0.0) g.tol = mul(g_common_140b84720.u10, kTolScale);
    if (g.tol == 0.0) g.tol = kTolDefault;
    std::int32_t zero = 0;
    g.dmin = kBig8;
    b = 1;
    std::int32_t ld = 3;
    a = 3;
    fun_1404cc289(g.x, g.p, g.nearest, &g.dmin, &g.edge, &a, &g.inout, &b, &g.a9, &zero, &ld);
    if (g.inout > 0 && g.dmin > g.tol) { // 1404e32dd comisd dmin, tol
        *iflag = 1;
        return;
    }
    fun_1404e2d44(g.x, g.p, l1, l2, l3, iflag);
    if (*iflag > 0) return;
    a = 3;
    fun_1404e34eb(g.x, g.nearest, l1, l2, &a);
}

} // namespace def::rt::geoutl
