// IGES pass 2 (packet p8-03): end-point matching, chain trace, segment table and the DEF_IGS.OUT
// writer. Spec: docs/re/commands/IGES_READER.md §5, §7. FP order from the listings.
#include "iges_ftn.hpp"

#include <cmath>

#include <defrt/ftn/fio.hpp>

namespace def::cmd::iges {

namespace fio = def::rt::ftn::fio;

namespace {

inline std::int32_t& IW(std::int32_t* iw, std::int32_t i, std::int32_t f) { return iw[(i - 1) * 5 + (f - 1)]; }
inline std::int32_t IW(const std::int32_t* iw, std::int32_t i, std::int32_t f) { return iw[(i - 1) * 5 + (f - 1)]; }
inline double* SL(double* rw, std::int32_t i) { return rw + (static_cast<std::int64_t>(i) - 1) * 3; }
inline const double* SL(const double* rw, std::int32_t i) { return rw + (static_cast<std::int64_t>(i) - 1) * 3; }
// pair arrays (2 ints per entry, 1-based): first = point, second = record
inline std::int32_t& P1(std::int32_t* a, std::int32_t i) { return a[(i - 1) * 2]; }
inline std::int32_t& P2(std::int32_t* a, std::int32_t i) { return a[(i - 1) * 2 + 1]; }
inline std::int32_t P1(const std::int32_t* a, std::int32_t i) { return a[(i - 1) * 2]; }
inline std::int32_t P2(const std::int32_t* a, std::int32_t i) { return a[(i - 1) * 2 + 1]; }
// table row k (1-based, 6 doubles)
inline double* ROW(double* t, std::int32_t k) { return t + (static_cast<std::int64_t>(k) - 1) * 6; }

// IDIV as the original executes it (a zero divisor raises the integer-divide exception)
inline std::int32_t idiv(std::int32_t a, std::int32_t b)
{
    volatile std::int32_t vb = b;
    return a / vb;
}

constexpr double kTwoPiLit = 6.2831853071796;   // 140639680 / 140639c28
constexpr double kNeg1e10 = -1.000000013351432e-10; // 140bb2358 / 140bb2360 (double)-1e-10f
constexpr double kRadDeg = 57.295779513082195;  // 140639c30

// -|x|, and -1e-10f for 0 (the trace's visited mark)
inline double negmark(double x)
{
    const bool z = x == 0.0;
    const double r = -std::fabs(x);
    return z ? kNeg1e10 : r;
}

} // namespace

// @src 140457312  pass 2 driver: (rebuild,) match, trace, segment table (flag 1) or DEF_IGS.OUT
void fun_140457312(std::int32_t* na, std::int32_t* nb, double* tol, double* rw, std::int32_t* iw,
                   double* table, const std::int32_t* rwsize, const std::int32_t* iwsize,
                   const std::int32_t* mode, const std::int32_t* flag, std::int32_t* st)
{
    FtnState& g = g_ftn;
    *st = 0;
    if (*tol <= 0.0) *tol = 9.999999717180685e-10; // 140bb2110
    if (*mode != 1) {
        g.ac08 = 1;
        g.ac14 = 1;
        g.ac0c = *na * 3 + 1;
        g.ac18 = *nb * 5 + 6;
        fun_140458e6f(nb, table, rw + g.ac0c - 1, iw + g.ac18 - 1, na);
    }
    g.ac10 = g.ac0c + *na * 3;
    g.ac1c = g.ac18 + *nb * 5;
    fun_140464abc(rw + g.ac0c - 1, iw + g.ac18 - 1, nb, na, iw + g.ac1c - 1, tol, rw + g.ac10 - 1);
    const std::int32_t n2 = *nb * 2;
    g.dc4 = g.ac1c + *na * 2;
    g.dc8 = n2 + g.dc4;
    g.dcc = n2 + g.dc8;
    g.dd0 = n2 + g.dcc;
    g.dd4 = n2 + g.dd0;
    if (*rwsize < g.dd4 || *iwsize < g.ac10) {
        *st = 1;
        return;
    }
    std::int32_t* s = iw + g.dc8 - 1;
    std::int32_t* starts = iw + g.dc4 - 1;
    fun_140467473(rw + g.ac0c - 1, iw + g.ac18 - 1, tol, na, nb, iw + g.ac1c - 1, &g.ac00, rw + g.ac10 - 1,
                  starts, &g.dd8, s, iw + g.dcc - 1, iw + g.dd0 - 1, iw + g.dd4 - 1, flag, st);
    if (*flag == 1) {
        g.ddc = *nb;
        const std::int32_t one = 1;
        fun_140459149(&one, table, rw + g.ac0c - 1, iw + g.ac18 - 1, s, &g.dd8, starts, &g.ddc);
        *nb = g.ddc;
        return;
    }
    static std::int32_t unit = 0; // 140721de0
    unit = 7;
    if (!fio::open_unknown(unit, "DEF_IGS.OUT")) { // OPEN(7,'DEF_IGS.OUT',STATUS='UNKNOWN',ERR=)
        *st = 2;
        return;
    }
    static std::int32_t de4 = 0; // 140721de4
    de4 = 1;
    fun_1404587d1(rw + g.ac0c - 1, iw + g.ac18 - 1, s, &g.dd8, starts, &unit);
    fio::close(unit);
}

// @src 140458e6f  rebuild points and records from a segment table (lines: 2 points, arcs: start,
// centre, end = centre + rotated start vector)
void fun_140458e6f(const std::int32_t* nb, const double* table, double* p, std::int32_t* iw2, std::int32_t* np)
{
    static double v[2] = {}; // 140722030
    static double a = 0.0;   // 140722040
    std::int32_t pnt = 1;    // first point of the record
    std::int32_t n = 0;      // 140722020
    std::int32_t r = 1;      // 140722024
    std::int32_t i3 = 3, i2 = 2;
    while (0 < *nb && r <= *nb) {
        const double* row = table + (static_cast<std::int64_t>(r) - 1) * 6;
        const std::int32_t type = ftn_trunc(row[0]); // 140722028
        if (type != 2) {
            i3 = i3 + 2;
            IW(iw2, r, 2) = r;
            IW(iw2, r, 4) = i2;
            i2 = i2 + 2;
            IW(iw2, r, 3) = pnt;
            SL(p, pnt)[0] = row[1];
            n = n + 2;
            IW(iw2, r, 1) = 2;
            r = r + 1;
            SL(p, pnt)[1] = row[2];
            pnt = pnt + 2;
            SL(p, n)[0] = row[3];
            SL(p, n)[1] = row[4];
            continue;
        }
        const double cx = row[3];
        const double cy = row[4];
        a = row[5];
        const double x1 = row[1];
        const double y1 = row[2];
        SL(p, pnt)[0] = x1;
        v[0] = x1 - cx;
        IW(iw2, r, 2) = r;
        SL(p, pnt)[1] = y1;
        v[1] = y1 - cy;
        IW(iw2, r, 3) = pnt;
        SL(p, i2)[0] = cx;
        IW(iw2, r, 4) = i3;
        SL(p, i2)[1] = cy;
        IW(iw2, r, 1) = 1;
        fun_1404672b2(v, &a);
        n = n + 3;
        SL(p, n)[0] = cx + v[0];
        i3 = i3 + 3;
        i2 = i2 + 3;
        pnt = pnt + 3;
        r = r + 1;
        SL(p, n)[1] = cy + v[1];
    }
    *np = n;
}

// @src 140464abc  match record ends within tol; move matched pairs to their x/y midpoint
void fun_140464abc(double* p, const std::int32_t* iw2, const std::int32_t* nb, const std::int32_t* np,
                   std::int32_t* l, const double* tol, double* d)
{
    FtnState& g = g_ftn;
    static double dd = 0.0; // 14072c908
    auto D = [&](std::int32_t i) -> double& { return d[static_cast<std::int64_t>(i) - 1]; };
    for (std::int32_t i = 1; i <= *np; ++i) {
        P2(l, i) = 0;
        P1(l, i) = 0;
        D(i) = *tol;
    }
    for (std::int32_t r = 1; r <= *nb; ++r) { // 14072c8c8
        if (IW(iw2, r, 1) < 0) continue;
        const std::int32_t f = IW(iw2, r, 3);
        const std::int32_t st = IW(iw2, r, 4) - f;
        const std::int32_t ne = idiv(st * 2, st);
        std::int32_t e = f; // 14072c8dc
        for (std::int32_t m = 0; m < ne; ++m, e = e + st) {
            double best = D(e); // 14072c8e0
            P2(l, e) = r;
            if (best == 0.0) continue;
            bool stop = false;
            for (std::int32_t q = 1; q <= *nb && !stop; ++q) { // 14072c8e8
                if (r == q) continue;
                if (IW(iw2, q, 1) < 0) continue;
                const std::int32_t fq = IW(iw2, q, 3);
                const std::int32_t st2 = IW(iw2, q, 4) - fq;
                const std::int32_t ne2 = idiv(st2 * 2, st2);
                std::int32_t gg = fq; // 14072c8fc
                for (std::int32_t it = 0; it < ne2; ++it, gg = gg + st2) {
                    if (e == gg) {
                        D(e) = 0.0;
                        P2(l, e) = r;
                        P1(l, e) = gg;
                        P1(l, gg) = e;
                        P2(l, gg) = q;
                        D(gg) = 0.0;
                        stop = true;
                        break;
                    }
                    if (D(gg) == 0.0) continue;
                    fun_1404681dc(nullptr, SL(p, e), SL(p, gg), &dd);
                    if (dd < best) {
                        g.c910 = q;
                        best = dd;
                        P1(l, e) = gg;
                        P2(l, e) = r;
                        if (dd == 0.0) {
                            stop = true;
                            break;
                        }
                    }
                }
                if (stop) break;
                if (best < D(e)) {
                    const std::int32_t h = P1(l, e);
                    D(e) = best;
                    D(h) = best;
                    P1(l, h) = e;
                    P2(l, h) = g.c910;
                }
            }
        }
    }
    static double mx = 0.0, my = 0.0; // 14072c918, 14072c920
    for (std::int32_t i = 1; i <= *np; ++i) {
        const std::int32_t h = P1(l, i);
        if (0 < h) {
            P1(l, h) = -P1(l, h);
            mx = (SL(p, i)[0] + SL(p, h)[0]) * 0.5; // 140bb2338
            my = (SL(p, i)[1] + SL(p, h)[1]) * 0.5;
            SL(p, i)[0] = mx;
            SL(p, h)[0] = mx;
            SL(p, i)[1] = my;
            D(i) = 0.0;
            D(h) = 0.0;
            SL(p, h)[1] = my;
        }
    }
    for (std::int32_t i = 1; i <= *np; ++i) {
        const std::int32_t v = P1(l, i);
        P1(l, i) = -v < 0 ? v : -v;
    }
}

// @src 140467473  trace one chain from the first unvisited point; a second chain (flag 1) gives
// st 3 with every record as its own loop
void fun_140467473(double* p, const std::int32_t* iw2, const double* tol, const std::int32_t* np,
                   const std::int32_t* nb, const std::int32_t* l, double*, double* d, std::int32_t* starts,
                   std::int32_t* nloops, std::int32_t* s, std::int32_t* a, std::int32_t* b, std::int32_t* m,
                   const std::int32_t* flag, std::int32_t* st)
{
    auto D = [&](std::int32_t i) -> double& { return d[static_cast<std::int64_t>(i) - 1]; };
    std::int32_t next = 1; // 14072d7e0 next S entry
    *nloops = 0;
    starts[0] = 1;
    for (std::int32_t r = 1; r <= *nb; ++r) {
        P1(b, r) = 0;
        P2(b, r) = 0;
        P2(a, r) = 0;
        P1(a, r) = 0;
    }
    std::int32_t lp = 1; // loop number + 1
    for (std::int32_t p0 = 1; p0 <= *np; ++p0) { // 14072d7e8
        double dv = D(p0);
        if (!(0.0 <= dv)) continue;
        const std::int32_t was = *nloops;
        lp = lp + 1;
        *nloops = was + 1;
        if (1 < was + 1 && *flag == 1) {
            *st = 3;
            for (std::int32_t r = 1; r <= *nb; ++r) {
                P2(s, r) = r;
                starts[r - 1] = r;
                starts[r] = r + 1;
                P1(s, r) = IW(iw2, r, 3);
            }
            *nloops = *nb;
            return;
        }
        const std::int32_t r0 = P2(l, p0); // 14072d7ec
        std::int32_t na = 1;               // 14072d7f0
        std::int32_t nbk = 0;              // 14072d7f4
        std::int32_t c = IW(iw2, r0, 3);   // 14072d804
        const std::int32_t last0 = IW(iw2, r0, 4);
        P2(a, 1) = r0;
        P1(a, 1) = p0;
        if (p0 == c) c = last0;
        std::int32_t pp = p0;
        for (;;) {
            double dold;    // 14072d808
            double dg;      // 14072d818
            std::int32_t gpt; // 14072d810
            for (;;) {
                for (;;) {
                    dold = dv;
                    if (dv < 0.0 && c == 0) goto done;
                    if (0.0 <= dv) break;
                    pp = c;
                    c = 0;
                    dv = D(pp);
                }
                dv = negmark(dv);
                D(pp) = dv;
                if (*tol < dold) continue;
                gpt = P1(l, pp);
                dg = D(gpt);
                if (dold != dg) continue;
                break;
            }
            D(gpt) = negmark(dg);
            const std::int32_t rg = P2(l, gpt); // 14072d820
            if (r0 == rg) break;
            if (c == 0) {
                nbk = nbk + 1;
                P1(b, nbk) = gpt;
                P2(b, nbk) = rg;
            } else {
                na = na + 1;
                P2(a, na) = rg;
                P1(a, na) = gpt;
            }
            const std::int32_t fq = IW(iw2, rg, 3);
            const std::int32_t lq = IW(iw2, rg, 4);
            std::int32_t nx = lq; // 14072d830
            if (lq == gpt) nx = fq;
            pp = nx;
            dv = D(nx);
        }
    done:
        std::int32_t j = 0; // 14072d838
        for (std::int32_t i = nbk; i >= 1; --i) {
            j = j + 1;
            P1(m, j) = P1(b, i);
            P2(m, j) = P2(b, i);
        }
        for (std::int32_t i = 1; i <= na; ++i) {
            j = j + 1;
            P1(m, j) = P1(a, i);
            P2(m, j) = P2(a, i);
        }
        std::int32_t tot = na + nbk; // 14072d83c
        const std::int32_t end = nbk + na + next;
        std::int32_t mi = 1;
        for (std::int32_t k = next; k <= end - 1; ++k, ++mi) {
            const std::int32_t rec = P2(m, mi);
            const std::int32_t lr = IW(iw2, rec, 4);
            P1(s, k) = P1(m, mi);
            P2(s, k) = rec;
            for (std::int32_t q = IW(iw2, rec, 3); q <= lr; ++q) D(q) = negmark(D(q));
        }
        if (1 < tot) fun_140467e54(p, iw2, &s[(next - 1) * 2], &tot);
        next = end;
        starts[lp - 1] = end;
    }
}

// @src 140467e54  start point of each entry of a chain from the distances to the next record
void fun_140467e54(const double* p, const std::int32_t* iw2, std::int32_t* s, const std::int32_t* n)
{
    FtnState& g = g_ftn;
    static double d1 = 0.0, d2 = 0.0, d3 = 0.0, d4 = 0.0; // 14072d878.. d890
    const std::int32_t cnt = *n;
    for (std::int32_t i = 1; i <= cnt - 1; ++i) { // 14072d850
        const std::int32_t rj = P2(s, i + 1);
        const std::int32_t ri = P2(s, i);
        g.d864 = IW(iw2, rj, 3);
        g.d858 = IW(iw2, ri, 3);
        g.d868 = IW(iw2, rj, 4);
        g.d85c = IW(iw2, ri, 4);
        g.d86c = 1;
        fun_1404681dc(nullptr, SL(p, g.d858), SL(p, g.d864), &d1);
        fun_1404681dc(nullptr, SL(p, g.d85c), SL(p, g.d864), &d2);
        if (d2 < d1) {
            d1 = d2;
            g.d86c = 2;
        }
        fun_1404681dc(nullptr, SL(p, g.d858), SL(p, g.d868), &d3);
        if (d3 < d1) {
            d1 = d3;
            g.d86c = 3;
        }
        fun_1404681dc(nullptr, SL(p, g.d85c), SL(p, g.d868), &d4);
        if (d4 < d1) {
            d1 = d3; // as in the original (14046807c stores the third distance)
            g.d86c = 4;
        }
        if (g.d86c == 1 || g.d86c == 3) {
            P1(s, i) = g.d85c;
        } else {
            P1(s, i) = g.d858;
        }
    }
    if (g.d86c != 1 && g.d86c != 2) {
        P1(s, cnt) = g.d868;
        return;
    }
    P1(s, cnt) = g.d864;
}

// @src 140459149  segment table from the loops: arcs [2, start, centre, sweep], polylines [1, p, q, 0]
void fun_140459149(const std::int32_t*, double* table, const double* p, const std::int32_t* iw2,
                   const std::int32_t* s, const std::int32_t* nloops, const std::int32_t* starts,
                   std::int32_t* nseg)
{
    static std::int32_t k = 0;   // 140722048
    static std::int32_t rev = 0; // 140722070
    static std::int32_t n = 0;   // 140722074
    k = 0;
    for (std::int32_t lp = 1; lp <= *nloops; ++lp) { // 14072204c
        const std::int32_t a = starts[lp - 1];
        const std::int32_t e = starts[lp] - 1;
        for (std::int32_t i = a; i <= e; ++i) { // 140722058
            k = k + 1;
            const std::int32_t rec = P2(s, i);
            const std::int32_t pt = P1(s, i);
            const std::int32_t f = IW(iw2, rec, 3);
            const std::int32_t last = IW(iw2, rec, 4);
            const std::int32_t kind = IW(iw2, rec, 1);
            if (kind < 0) continue;
            rev = f != pt ? 1 : 0;
            n = (last - f) + 1;
            if (kind == 1) fun_1404594a3(&k, table, SL(p, f), &rev);
            if (kind == 2) {
                const std::int32_t two = 2;
                fun_140459607(&k, table, SL(p, f), &rev, &two);
            }
            if (kind == 5 || kind == 6) fun_140459607(&k, table, SL(p, f), &rev, &n);
            if (kind == 3) {
                const std::int32_t comp = IW(iw2, rec, 2); // 140722078
                fun_14045979e(table, p, SL(p, comp), &k, &rev, iw2);
            }
        }
    }
    *nseg = k;
}

// @src 1404594a3  arc row k: [2, start, centre, sweep]; reversed: start = third point, -sweep
void fun_1404594a3(std::int32_t* k, double* table, const double* p, const std::int32_t* rev)
{
    static double a = 0.0, out = 0.0; // 140722080, 140722088
    static std::int32_t idx = 0;      // 140722090
    fun_14046736c(p, p + 6, p + 3, &a);
    if (a < 0.0) a = a + kTwoPiLit;
    idx = 1;
    out = a;
    if (*rev == 1) {
        out = -a; // XOR with the sign bit (140bb2210)
        idx = 3;
    }
    double* row = ROW(table, *k);
    row[5] = out;
    row[4] = p[4];
    row[3] = p[3];
    row[2] = SL(p, idx)[1];
    row[1] = SL(p, idx)[0];
    row[0] = 2.0; // 140bb2218
}

// @src 140459607  polyline rows from k: [1, p(i), p(i+-1), 0]; *k = the last row written
void fun_140459607(std::int32_t* k, double* table, const double* p, const std::int32_t* rev, const std::int32_t* n)
{
    FtnState& g = g_ftn;
    std::int32_t i0 = 0;   // 140722094
    std::int32_t step = 1; // 140722098
    if (*rev == 1) {
        i0 = *n + 1;
        step = -1;
    }
    const std::int32_t cnt = *n;
    std::int32_t row = *k;
    std::int32_t ib = step + i0;
    for (std::int32_t j = 1; j <= cnt - 1; ++j) { // 14072209c
        ib = ib + step;
        i0 = i0 + step;
        g.r0a4 = row;
        double* t = ROW(table, row);
        t[5] = 0.0;
        t[0] = 1.0; // 140bb2220
        t[4] = SL(p, ib)[1];
        t[3] = SL(p, ib)[0];
        t[2] = SL(p, i0)[1];
        t[1] = SL(p, i0)[0];
        row = row + 1;
    }
    *k = g.r0a4;
}

// @src 14045979e  composite curve (kind 3) rows; reached only for IGES type -102 records
void fun_14045979e(double* table, const double* p, const double* comp, std::int32_t* k,
                   const std::int32_t* rev, const std::int32_t* iw2)
{
    FtnState& g = g_ftn;
    static double y[0xc8] = {};        // 1407220b0 Y(1..)
    static std::int32_t pairs[0x190] = {}; // 1407223e0 (pt, rec)
    static std::int32_t nslots = 0;    // 1407223d0
    const std::int32_t nm = ftn_nint(comp[0]); // 1407220a8
    std::int32_t cnt[2] = {nm + 1, 0};
    fun_140462631(y, const_cast<double*>(comp), &cnt[0], &nslots, &cnt[1]);
    for (std::int32_t c = 1; c <= nm; ++c) {
        P1(pairs, c) = 0;
        P2(pairs, c) = ftn_nint(y[c]);
    }
    fun_140467e54(p, iw2, pairs, &nm);
    *k = 0;
    std::int32_t step = 1; // 140722700
    if (*rev == 1) {
        step = -1;
        *k = nm + 1;
    }
    for (std::int32_t c = 1; c <= nm; ++c) { // 140722704
        const std::int32_t idx = *k + step;
        const std::int32_t rec = P2(pairs, idx);
        g.r708 = g.r708 + 1;
        const std::int32_t last = IW(iw2, rec, 4);
        const std::int32_t f = IW(iw2, rec, 3);
        const std::int32_t kind = IW(iw2, rec, 1);
        std::int32_t n = (last - f) + 1; // 140722720
        *k = idx;
        if (kind == 1) {
            fun_1404594a3(&g.r708, table, SL(p, f), rev);
        } else if (kind == 2) {
            const std::int32_t two = 2;
            fun_140459607(&g.r708, table, SL(p, f), rev, &two);
        } else if (kind == 5) {
            fun_140459607(&g.r708, table, SL(p, f), rev, &n);
        }
    }
}

// @src 1404587d1  DEF_IGS.OUT: loop count, then per loop "(I5,I5)" loop and segment count and the rows
void fun_1404587d1(const double* p, const std::int32_t* iw2, const std::int32_t* s, const std::int32_t* nloops,
                   const std::int32_t* starts, const std::int32_t* unit)
{
    fio::Write(*unit).i(*nloops).end();
    const std::int32_t nl = *nloops;
    static std::int32_t lp = 0, cnt = 0, row = 0, rev = 0, n = 0; // 140721fe8, ff4, 140722018, 2010, 2014
    for (lp = 1; lp <= nl; ++lp) {
        const std::int32_t a = starts[lp - 1];
        const std::int32_t e = starts[lp] - 1;
        cnt = 0;
        for (std::int32_t i = a; i <= e; ++i) {
            const std::int32_t pt = P1(s, i);
            const std::int32_t rec = P2(s, i);
            const std::int32_t f = IW(iw2, rec, 3);
            const std::int32_t kind = IW(iw2, rec, 1);
            const std::int32_t last = IW(iw2, rec, 4);
            std::int32_t c = cnt + 1;
            if (-1 < kind) {
                rev = f != pt ? 1 : 0;
                n = last - f;
                if (kind == 5 || kind == 6) c = cnt + n;
            }
            cnt = c;
        }
        if (0 < cnt) fio::Write(*unit, "(I5,I5)").i(lp).i(cnt).end();
        row = 0;
        for (std::int32_t i = a; i <= e; ++i) {
            row = row + 1;
            const std::int32_t rec = P2(s, i);
            const std::int32_t pt = P1(s, i);
            const std::int32_t f = IW(iw2, rec, 3);
            const std::int32_t last = IW(iw2, rec, 4);
            const std::int32_t kind = IW(iw2, rec, 1);
            if (kind < 0) continue;
            rev = f != pt ? 1 : 0;
            n = (last - f) + 1;
            const std::int32_t two = 2;
            if (kind == 1) fun_1404668e4(const_cast<double*>(SL(p, f)), &two, &row, &rev, unit);
            if (kind == 2) fun_1404665f3(SL(p, f), &row, &two, &rev, unit);
            if (kind == 5 || kind == 6) fun_1404665f3(SL(p, f), &row, &n, &rev, unit);
            if (kind == 3) {
                const std::int32_t comp = IW(iw2, rec, 2); // 14072201c
                fun_140465bb1(p, SL(p, comp), &row, &rev, iw2);
            }
        }
    }
}

// @src 1404665f3  polyline rows "(I5,' 1 ',5(F16.9,1X))": row, p(i), p(i+-1), 0.0
void fun_1404665f3(const double* p, std::int32_t* k, const std::int32_t* n, const std::int32_t* rev,
                   const std::int32_t* unit)
{
    FtnState& g = g_ftn;
    std::int32_t i0 = 0;   // 14072d688
    std::int32_t step = 1; // 14072d68c
    const double zero = 0.0; // 14072d680
    if (*rev == 1) {
        i0 = *n + 1;
        step = -1;
    }
    const std::int32_t cnt = *n;
    std::int32_t row = *k;
    std::int32_t ib = step + i0;
    for (std::int32_t j = 1; j <= cnt - 1; ++j) { // 14072d690
        ib = ib + step;
        i0 = i0 + step;
        g.d698 = row;
        fio::Write(*unit, "(I5,' 1 ',5(F16.9,1X))")
            .i(g.d698)
            .d(SL(p, i0)[0])
            .d(SL(p, i0)[1])
            .d(SL(p, ib)[0])
            .d(SL(p, ib)[1])
            .d(zero)
            .end();
        row = row + 1;
    }
    *k = g.d698;
}

// @src 1404668e4  arc row "(I5,' 2 ',5(F16.9,1X))": row, start, centre, sweep in degrees
// (mode 1: exchange the first two points instead)
void fun_1404668e4(double* p, const std::int32_t* mode, const std::int32_t* k, const std::int32_t* rev,
                   const std::int32_t* unit)
{
    if (*mode == 1) {
        double t1[3], t2[3], t3[3];
        for (std::int32_t c = 0; c < 3; ++c) {
            t1[c] = p[c];
            t2[c] = p[3 + c];
            t3[c] = p[6 + c];
        }
        for (std::int32_t c = 0; c < 3; ++c) {
            p[c] = t2[c];
            p[3 + c] = t1[c];
            p[6 + c] = t3[c];
        }
        return;
    }
    static double a = 0.0, deg = 0.0; // 14072d6f8, 14072d700
    static std::int32_t idx = 0;      // 14072d708
    fun_14046736c(p, p + 6, p + 3, &a);
    if (a < 0.0) a = a + kTwoPiLit;
    deg = a * kRadDeg;
    idx = 1;
    if (*rev == 1) {
        deg = -deg; // XOR with the sign bit (140bb2340)
        idx = 3;
    }
    fio::Write(*unit, "(I5,' 2 ',5(F16.9,1X))")
        .i(*k)
        .d(SL(p, idx)[0])
        .d(SL(p, idx)[1])
        .d(p[3])
        .d(p[4])
        .d(deg)
        .end();
}

// @src 140465bb1  composite curve rows of DEF_IGS.OUT (kind 3 only)
void fun_140465bb1(const double* p, const double* comp, std::int32_t* k, const std::int32_t* rev,
                   const std::int32_t* iw2)
{
    FtnState& g = g_ftn;
    static double y[0xc8] = {};           // 14072cfd0
    static std::int32_t pairs[0x190] = {}; // 14072d300
    static std::int32_t nslots = 0;       // 14072d2f0
    const std::int32_t nm = ftn_nint(comp[0]); // 14072cfcc
    std::int32_t cnt[2] = {nm + 1, 0};
    fun_140462631(y, const_cast<double*>(comp), &cnt[0], &nslots, &cnt[1]);
    for (std::int32_t c = 1; c <= nm; ++c) {
        P1(pairs, c) = 0;
        P2(pairs, c) = ftn_nint(y[c]);
    }
    fun_140467e54(p, iw2, pairs, &nm);
    *k = 0;
    std::int32_t step = 1; // 14072d620
    if (*rev == 1) {
        step = -1;
        *k = nm + 1;
    }
    for (std::int32_t c = 1; c <= nm; ++c) { // 14072d624
        const std::int32_t idx = *k + step;
        const std::int32_t rec = P2(pairs, idx);
        g.d628 = g.d628 + 1;
        const std::int32_t last = IW(iw2, rec, 4);
        const std::int32_t f = IW(iw2, rec, 3);
        const std::int32_t kind = IW(iw2, rec, 1);
        std::int32_t n = (last - f) + 1; // 14072d640
        *k = idx;
        const std::int32_t two = 2;
        if (kind == 1) {
            fun_1404668e4(const_cast<double*>(SL(p, f)), &two, &g.d628, rev, &g.d644);
        } else if (kind == 2) {
            fun_1404665f3(SL(p, f), &g.d628, &two, rev, &g.d644);
        } else if (kind == 5) {
            fun_1404665f3(SL(p, f), &g.d628, &n, rev, &g.d644);
        }
    }
}

void reset_ftn_state() { g_ftn = FtnState{}; }

} // namespace def::cmd::iges
