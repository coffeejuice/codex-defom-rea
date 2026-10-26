// IGES geometry build and tolerance (packet p8-03): entity records -> output points (112/126
// sampling, 124 transforms, arc mirror check), shortest distance, arc angle helpers.
// Spec: docs/re/commands/IGES_READER.md §4.5. FP order from the listings (out/p803/all_L.txt).
#include "iges_ftn.hpp"

#include <defcmd/iges.hpp>

#include <cmath>

#include <defrt/stdlib/libm.hpp>

namespace def::cmd::iges {

namespace {

inline std::int32_t& IW(std::int32_t* iw, std::int32_t i, std::int32_t f) { return iw[(i - 1) * 5 + (f - 1)]; }
inline std::int32_t IW(const std::int32_t* iw, std::int32_t i, std::int32_t f) { return iw[(i - 1) * 5 + (f - 1)]; }
// slot / point i (1-based, 3 doubles)
inline double* SL(double* rw, std::int32_t i) { return rw + (static_cast<std::int64_t>(i) - 1) * 3; }
inline const double* SL(const double* rw, std::int32_t i) { return rw + (static_cast<std::int64_t>(i) - 1) * 3; }

inline double fabs_bits(double x) { return std::fabs(x); }

constexpr double kTwoPiLit = 6.2831853071796;  // 140639d40 (0x401921FB54442D28)
constexpr double kF01 = 0.009999999776482582;  // 140bb2100 (double)0.01f
constexpr double kF1e9 = 9.999999717180685e-10; // 140bb2110 (double)1e-9f
constexpr double kF1e5 = 9.999999747378752e-06; // 140bb2350 (double)1e-5f

// composite-curve buffers of 14046519b (SAVE data)
struct CompState {
    double x[0x64] = {};          // 14072c960 X(1..)
    std::int32_t pairs[0xc8] = {}; // 14072cca0 (pt, rec) pairs
};
CompState g_comp;

} // namespace

// @src 140457196  build the output points (14046519b), shortest distance (1404648ff), tolerance
void fun_140457196(char*, std::int32_t*, std::int32_t* na, std::int32_t* nb, std::int32_t*, std::int32_t*,
                   std::int32_t*, std::int32_t*, std::int32_t*, const std::int32_t* nsamp, double* tol, double* rw,
                   std::int32_t* iw, std::int32_t*, std::int32_t*, std::int32_t*)
{
    FtnState& g = g_ftn;
    g.ac14 = 1;
    g.ac08 = 1;
    g.ac18 = *nb * 5 + 6;
    g.ac1c = *nb * 10 + 6;
    g.ac0c = *na * 3 + 1;
    fun_14046519b(rw, iw, rw + g.ac0c - 1, iw + g.ac18 - 1, na, nb, nsamp);
    fun_1404648ff(rw + g.ac0c - 1, na, iw + g.ac18 - 1, nb, &g.ac00);
    const double t = g.ac00 * kF01;
    *tol = t;
    if (t != 0.0) return;
    *tol = kF1e9;
}

// @src 14046519b  records -> output points RW2 and output records IW2; nA = points, nB = records
void fun_14046519b(double* rw, std::int32_t* iw, double* rw2, std::int32_t* iw2, std::int32_t* na,
                   std::int32_t* nb, const std::int32_t* nsamp)
{
    const std::int32_t nrec = *nb;
    static std::int32_t nsteps = 0; // 14072c928
    nsteps = 8;
    std::int32_t k = 0;   // 14072c92c
    std::int32_t cur = 1; // 14072c930
    for (std::int32_t i = 1; i <= nrec; ++i) {
        std::int32_t kind = IW(iw, i, 1); // 14072c938
        if (kind == 4 || kind == 3) continue;
        std::int32_t trf = IW(iw, i, 3);  // 14072c940
        const std::int32_t f = IW(iw, i, 4); // 14072c93c
        k = k + 1;
        IW(iw2, k, 1) = kind;
        IW(iw2, k, 2) = k;
        IW(iw2, k, 5) = i;
        IW(iw2, k, 3) = cur;
        std::int32_t last = cur + 1; // 14072c944
        IW(iw, i, 5) = k;
        if (kind == 1) last = cur + 2;
        std::int32_t npts; // 14072c948
        if (kind == 5) {
            last = cur;
            fun_140462e15(SL(rw, f), SL(rw, f + 1), &last, rw2, &nsteps);
            npts = (last - cur) + 1;
        } else if (kind == 6) {
            last = cur;
            fun_140463267(SL(rw, f), rw2, nsamp, &last);
            npts = (last - cur) + 1;
        } else {
            npts = 0;
            std::int32_t src = f - 1;
            for (std::int32_t p = cur; p <= last; ++p) { // 14072c94c
                npts = npts + 1;
                src = src + 1;
                for (std::int32_t c = 0; c < 3; ++c) SL(rw2, p)[c] = SL(rw, src)[c];
            }
        }
        const std::int32_t self = i;
        IW(iw2, k, 4) = last;
        fun_1404645b0(rw, rw2, iw, nb, &self, &kind, &cur, &last, &trf);
        if (kind == 1) fun_140467142(SL(rw, f), SL(rw2, cur));
        cur = npts + cur;
    }
    // composite curves (kind 3, IGES type -102)
    CompState& cs = g_comp;
    for (std::int32_t i = 1; i <= nrec; ++i) {
        std::int32_t saved = k;
        if (IW(iw, i, 1) == 3) {
            const std::int32_t f = IW(iw, i, 4);
            const std::int32_t k1 = k + 1;
            k = k1;
            IW(iw2, k1, 2) = cur;
            IW(iw2, k1, 1) = 3;
            static std::int32_t cc80 = 0, slots = 0; // 14072cc80, 14072c944
            const std::int32_t dir0 = 0;
            fun_140462631(cs.x, SL(rw, f), &cc80, &slots, &dir0);
            const std::int32_t nm = ftn_nint(cs.x[0]); // 14072cc84
            bool found_all = true;
            for (std::int32_t c = 1; c <= nm; ++c) { // 14072cc88
                const std::int32_t de = ftn_nint(cs.x[c]); // 14072cc8c
                std::int32_t r = 1;                        // 14072cc90
                if (*nb < 1) {
                    found_all = false;
                    break;
                }
                while (IW(iw, r, 2) != de) {
                    r = r + 1;
                    if (*nb < r) break;
                }
                if (*nb < r) {
                    found_all = false;
                    break;
                }
                const std::int32_t orec = IW(iw, r, 5); // 14072cfc0
                const std::int32_t a = IW(iw2, orec, 1);
                const std::int32_t m = -a < 0 ? a : -a;
                cs.pairs[(c - 1) * 2 + 1] = r;
                IW(iw2, orec, 1) = -m;
                cs.x[c] = static_cast<double>(orec);
            }
            if (found_all) {
                fun_140467e54(rw, iw, cs.pairs, &nm);
                std::int32_t cnt = nm + 1;
                const std::int32_t dir1 = 1;
                fun_140462631(cs.x, SL(rw2, cur), &cnt, &slots, &dir1);
                cur = cur + slots;
                const std::int32_t p1 = cs.pairs[0];
                const std::int32_t p2 = cs.pairs[(nm - 1) * 2];
                for (std::int32_t c = 0; c < 3; ++c) {
                    SL(rw2, cur)[c] = SL(rw, p1)[c];
                    SL(rw2, cur + 1)[c] = SL(rw, p2)[c];
                }
                IW(iw2, k1, 4) = cur + 1;
                IW(iw2, k1, 3) = cur;
                IW(iw2, k1, 5) = i;
                saved = k;
                cur = cur + 2;
            }
        }
        k = saved;
    }
    *na = cur - 1;
    *nb = k;
}

// @src 140462e15  IGES 112: nsteps points per segment, then the end point
void fun_140462e15(const double* blk, const double* t, std::int32_t* cur, double* out, const std::int32_t* nsteps)
{
    const std::int32_t n = ftn_nint(blk[0]); // 14072c494
    const std::int32_t q = (n + 1) / 3;
    std::int32_t p = *cur - 1;   // 14072c490
    std::int32_t ra = q + 3;     // 14072c498 row A of the segment
    const std::int32_t ns = *nsteps;
    auto coef = [&](std::int32_t row, std::int32_t c) { return blk[(row - 1) * 3 + c - 1]; };
    for (std::int32_t s = 1; s <= n; ++s) { // 14072c49c
        const double dt = (t[s] - t[s - 1]) / static_cast<double>(ns); // 14072c4a8
        for (std::int32_t j = 0; j < ns; ++j) { // 14072c4b0
            p = p + 1;
            const double u = static_cast<double>(j) * dt;
            const double u2 = u * u;
            const double u3 = u * u2;
            for (std::int32_t c = 1; c <= 3; ++c) {
                const double x3 = u * coef(ra + 1, c) + coef(ra, c);
                const double x2 = u2 * coef(ra + 2, c) + x3;
                SL(out, p)[c - 1] = u3 * coef(ra + 3, c) + x2;
            }
        }
        ra = ra + 4;
    }
    const double du = t[n] - t[n - 1]; // 14072c4b8: T(N+1) - T(N)
    const double du2 = du * du;
    const double du3 = du * du2;
    const std::int32_t pe = p + 1;
    for (std::int32_t c = 1; c <= 3; ++c) {
        const double x3 = coef(ra - 3, c) * du + coef(ra - 4, c);
        const double x2 = du2 * coef(ra - 2, c) + x3;
        SL(out, pe)[c - 1] = du3 * coef(ra - 1, c) + x2;
    }
    *cur = pe;
}

// @src 140463267  IGES 126 block -> 140463374 (K, M, positions of weights, control points, V0)
void fun_140463267(const double* blk, double* out, const std::int32_t* nsamp, std::int32_t* cur)
{
    static std::int32_t k = 0, m = 0, pw = 0, pc = 0, pv = 0; // 14072c4c0 c4 c8 cc d0
    static double v0 = 0.0, v1 = 0.0;                         // 14072c4d8, 14072c4e0
    pv = ftn_trunc(blk[5]);
    k = ftn_trunc(blk[0]);
    m = ftn_trunc(blk[1]);
    v0 = blk[pv - 1];
    pw = ftn_trunc(blk[3]);
    pc = ftn_trunc(blk[4]);
    v1 = blk[pv];
    fun_140463374(blk + pc - 1, blk + 6, blk + pw - 1, &k, &m, out, nsamp, cur, &v0, &v1);
}

// @src 140463374  rational B-spline sampling (see iges.hpp)
void fun_140463374(const double* ctrl, const double* knots, const double* weights, const std::int32_t* k,
                   const std::int32_t* m, double* out, const std::int32_t* nsamp, std::int32_t* nout,
                   const double* v0, const double* v1)
{
    // 14072c500: the basis table, (M+1)^2 doubles (64 in the original; larger M overruns it)
    static double nt[0x400];
    static double u = 0.0;     // 14072c4f0
    static std::int32_t j = 0; // 14072c4fc
    const std::int32_t n = *nsamp;
    const std::int32_t hi = (*m + 2 + *k) - 1; // 14072c4e8 - 1
    if (0 < n) {
        const double step = (*v1 - *v0) / static_cast<double>(n - 1);
        for (std::int32_t i = 0; i < n; ++i) { // 14072c4ec
            u = static_cast<double>(i) * step + *v0;
            for (std::int32_t jj = *m + 1; jj <= hi; ++jj) { // 14072c4f8
                if (knots[jj - 1] <= u && u <= knots[jj]) {
                    j = jj;
                    fun_1404635be(knots, &j, m, &u, nt);
                    fun_1404639eb(ctrl, weights, knots, &j, m, &u, nt, SL(out, *nout));
                    *nout = *nout + 1;
                    break;
                }
            }
        }
    }
    *nout = *nout - 1;
}

// @src 1404635be  basis table Nt(M+1, M+1) of the knot span j at u
void fun_1404635be(const double* t, const std::int32_t* j, const std::int32_t* m, const double* u, double* nt)
{
    const std::int32_t l = *m + 1;
    const std::int64_t ld = l < 0 ? 0 : l;
    auto N = [&](std::int32_t r, std::int32_t c) -> double& { return nt[(c - 1) * ld + r - 1]; };
    auto T = [&](std::int32_t i) { return t[i - 1]; };
    for (std::int32_t c = 1; c <= l; ++c)
        for (std::int32_t r = 1; r <= l; ++r) N(c, r) = 0.0;
    N(l, 1) = 1.0; // 140bb22c0
    const std::int32_t jj = *j;
    for (std::int32_t p = 1; p <= *m; ++p) { // 14072c708
        for (std::int32_t s = 0; s <= p; ++s) { // 14072c704 (14072c70c = s + 1)
            const double d1 = T(jj + p - s) - T(jj - s); // 14072c710
            double a;                                   // 14072c718
            if (d1 != 0.0) {
                a = ((*u - T(jj - s)) / d1) * N((*m - s) + 1, p);
            } else {
                a = 0.0;
            }
            const double d2 = T(jj + p + 1 - s) - T(jj + 1 - s); // 14072c720
            double b;                                           // 14072c728
            if (d2 != 0.0) {
                const std::int32_t r2 = (*m - s) + 2; // 14072c730
                if (l < r2) {
                    b = 0.0;
                } else {
                    b = ((T(p + 1 + jj - s) - *u) / d2) * N(r2, p);
                }
            } else {
                b = 0.0;
            }
            N(l - s, p + 1) = a + b;
        }
    }
}

// @src 1404639eb  rational point: sum of w*P*N over the span, divided by the sum of w*N
void fun_1404639eb(const double* ctrl, const double* w, const double*, const std::int32_t* j,
                   const std::int32_t* m, const double*, const double* nt, double* out)
{
    static double d[3]; // 14072c740
    const std::int32_t l = *m + 1;
    const std::int64_t ld = l < 0 ? 0 : l;
    for (std::int32_t c = 0; c < 3; ++c) {
        d[c] = 0.0;
        out[c] = 0.0;
    }
    std::int32_t cnt = 0; // 14072c760
    for (std::int32_t i = *j - *m; i <= *j; ++i) { // 14072c734
        cnt = cnt + 1;
        const double wi = w[i - 1];
        const double b = nt[(l - 1) * ld + cnt - 1];
        for (std::int32_t c = 0; c < 3; ++c) {
            out[c] = (wi * SL(ctrl, i)[c]) * b + out[c];
            d[c] = wi * b + d[c];
        }
    }
    for (std::int32_t c = 0; c < 3; ++c) out[c] = out[c] / d[c];
}

// @src 1404645b0  apply the 124 matrix the record points at (first other kind-4 record with
// IW(j,3) == trf) to the output points first..last
void fun_1404645b0(const double* rw, double* rw2, const std::int32_t* iw, const std::int32_t* nb,
                   const std::int32_t* self, const std::int32_t*, const std::int32_t* first,
                   const std::int32_t* last, const std::int32_t* trf)
{
    if (*trf == 0) return;
    for (std::int32_t j = 1; j <= *nb; ++j) { // 14072c85c
        if (*self != j && IW(iw, j, 1) == 4 && *trf == IW(iw, j, 3)) {
            const std::int32_t m = IW(iw, j, 4); // 14072c868
            std::int32_t p = *first;             // 14072c86c
            if (*last < p) return;
            do {
                fun_140464720(SL(rw, m), SL(rw, m + 3), SL(rw2, p));
                p = p + 1;
            } while (p <= *last);
            return;
        }
    }
}

// @src 140464720  p = R p + T (R = 3 slots, column by column)
void fun_140464720(const double* r, const double* t, double* p)
{
    static double tmp[3]; // 14072c880
    for (std::int32_t row = 1; row <= 3; ++row) {
        double s = 0.0;
        tmp[row - 1] = 0.0;
        for (std::int32_t c = 1; c <= 3; ++c) {
            s = s + r[(c * 3 + row) - 4] * p[c - 1];
            tmp[row - 1] = s;
        }
        tmp[row - 1] = s + t[row - 1];
    }
    for (std::int32_t row = 0; row < 3; ++row) p[row] = tmp[row];
}

// @src 1404648ff  shortest non-zero distance between consecutive points of a record (1.0E6 start)
void fun_1404648ff(const double* p, const std::int32_t*, const std::int32_t* iw2, const std::int32_t* nb,
                   double* dmin)
{
    static double d = 0.0; // 14072c8c0
    *dmin = 1000000.0;     // 140bb2330
    for (std::int32_t r = 1; r <= *nb; ++r) {
        if (IW(iw2, r, 1) == 4) continue;
        const std::int32_t last = IW(iw2, r, 4);
        for (std::int32_t a = IW(iw2, r, 3); a <= last - 1; ++a) {
            fun_1404681dc(nullptr, SL(p, a), SL(p, a + 1), &d);
            if (d != 0.0 && d < *dmin) *dmin = d;
        }
    }
}

// @src 140467142  mirror check: swap the arc's output start and end when the sweep changed
void fun_140467142(const double* src, double* out)
{
    static double a1 = 0.0, a2 = 0.0; // 14072d778, 14072d780
    fun_14046736c(src, src + 6, src + 3, &a1);
    if (a1 < 0.0) a1 = a1 + kTwoPiLit;
    fun_14046736c(out, out + 6, out + 3, &a2);
    if (a2 < 0.0) a2 = a2 + kTwoPiLit;
    if (fabs_bits(a1 - a2) <= kF1e5) return;
    const double s0 = out[0], s1 = out[1], s2 = out[2];
    out[2] = out[8];
    out[1] = out[7];
    out[0] = out[6];
    out[8] = s2;
    out[7] = s1;
    out[6] = s0;
}

// @src 1404681dc  d = |q - p| = sqrt((dx*dx + dy*dy) + dz*dz)
void fun_1404681dc(void*, const double* p, const double* q, double* d)
{
    static double v[3]; // 14072d8a0
    for (std::int32_t c = 0; c < 3; ++c) v[c] = q[c] - p[c];
    *d = std::sqrt((v[1] * v[1] + v[0] * v[0]) + v[2] * v[2]);
}

// @src 14046736c  signed angle from (a - c) to (b - c): atan2 of the normalised cross and dot
void fun_14046736c(const double* a, const double* b, const double* c, double* ang)
{
    const double ax = a[0] - c[0];
    const double ay = a[1] - c[1];
    const double r1 = std::sqrt(ay * ay + ax * ax);
    const double bx = b[0] - c[0];
    const double by = b[1] - c[1];
    const double r2 = std::sqrt(by * by + bx * bx);
    const double y = ((ax * by - bx * ay) / r1) / r2;
    const double x = ((ay * by + ax * bx) / r1) / r2;
    *ang = def::rt::libm::ps_atan2(y, x); // 1404b3878
}

// @src 1404672b2  rotate v by *ang (sincos 1404b3b30)
void fun_1404672b2(double* v, const double* ang)
{
    double s = 0.0, c = 0.0;
    def::rt::libm::ps_sincos(*ang, &s, &c);
    const double x = v[0];
    const double y = v[1];
    v[1] = s * x + c * y;
    v[0] = c * x - s * y;
}

void reset_geom_state() { g_comp = CompState{}; }

} // namespace def::cmd::iges
