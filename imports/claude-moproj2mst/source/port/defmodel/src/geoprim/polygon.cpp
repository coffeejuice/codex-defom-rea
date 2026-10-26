// 2D polygon helpers of the GEOPRIM extrude (packet p9-04). Spec: CNVT3D_SWEEP.md 10.5, 10.6.
// Decompiles and listings: deform-rea-work/out/p904/all.c, all.asm.
#include <defmodel/geoprim/polygon.hpp>

#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/geoprim/extrude.hpp>

#include <defmodel/geo_kw/curve_model.hpp>

#include <cmath>

namespace def::model::geoprim {

namespace {

// 140484558: CRT fabs (sign bit cleared for finite values; the NaN/inf paths of _sptype are
// not reached by these callers)
double crt_fabs(double x) { return std::fabs(x); }

constexpr double kEps12 = 1e-12;   // DAT_14051f260
constexpr double kEps6 = 1e-6;     // DAT_14051f1b8
constexpr double k180 = 180.0;     // DAT_14051f228
constexpr double k1e29 = 1e29;     // DAT_14052c598

} // namespace

// @src 140308340  line crossing
std::int32_t fun_140308340(double x1, double y1, double x2, double y2, double x3, double y3,
                           double x4, double y4, double* x, double* y)
{
    const double a1 = y2 - y1;                 // 14030835f
    const double b1 = x1 - x2;
    const double c1 = x2 * y1 - x1 * y2;
    const double r3 = a1 * x3 + b1 * y3 + c1;
    const double r4 = a1 * x4 + b1 * y4 + c1;
    // 140308419: UCOMISD ==0 (JP skips on NaN), then COMISD r3*r4, 0; JC: below or unordered
    if (!(r3 == 0.0 || r4 == 0.0 || !(r3 * r4 >= 0.0))) return 0;
    const double a2 = y4 - y3;                 // 14030845a
    const double b2 = x3 - x4;
    const double c2 = x4 * y3 - x3 * y4;
    const double r1 = a2 * x1 + b2 * y1 + c2;
    const double r2 = a2 * x2 + b2 * y2 + c2;
    if (!(r1 == 0.0 || r2 == 0.0 || !(r1 * r2 >= 0.0))) return 0;
    const double det = a1 * b2 - a2 * b1;      // 140308555
    // 14030858e: COMISD 1e-12, |det|; JBE -> compute (also unordered)
    if (kEps12 > crt_fabs(det)) return 2;
    *x = (b1 * c2 - b2 * c1) / det;
    *y = (a2 * c1 - a1 * c2) / det;
    return 1;
}

// @src 140308120  segment form
std::int32_t fun_140308120(const double* s1, const double* s2, double* out)
{
    double x, y;
    std::int32_t r = fun_140308340(s1[0], s1[1], s1[3], s1[4], s2[0], s2[1], s2[3], s2[4], &x, &y);
    if (r == 0 || r == 2) {
        out[0] = 0.0;
        out[1] = 0.0;
    } else {
        out[0] = x;
        out[1] = y;
        r = 1;
    }
    return r;
}

// @src 140309960  box test
bool fun_140309960(const double* p, const double* b)
{
    if (p[0] < b[0] || b[2] < p[0] || p[1] < b[1] || b[3] < p[1]) return false;
    return true;
}

// @src 1402e7f40  simple-profile test (closing edge only)
// (140308730, the 2D -> 24-byte point copy, is defmodel/statevar/geo2d_kernels' copy_vec; inlined)
bool fun_1402e7f40(ProfileArray* p)
{
    if (p->u10 != 0) return true;
    const std::int32_t n = p->n;
    if (n < 3 || p->pts == nullptr) {
        p->u10 = 0;
        return false;
    }
    const double* P = p->pts;
    const double* A = P + 2;                 // point 1
    const double* Z = P + 2 * n;             // point n
    const double closing[6] = {A[0], A[1], 0.0, Z[0], Z[1], 0.0};
    for (std::int32_t i = 1; i < n; ++i) {
        const double* S = P + 2 * i;
        const double* T = P + 2 * (i + 1);
        const double edge[6] = {S[0], S[1], 0.0, T[0], T[1], 0.0};
        double c[2];
        const std::int32_t r = fun_140308120(closing, edge, c);
        const double box1[4] = {A[0], A[1], Z[0], Z[1]};
        const bool in1 = fun_140309960(c, box1);
        const double box2[4] = {S[0], S[1], T[0], T[1]};
        const bool in2 = fun_140309960(c, box2);
        if (r == 1 && in1 && in2) {
            const bool ok = (i == n - 1) ? geo_kw::points_near(c, P + 2 * n, kEps6)    // 140308d60
                                      : geo_kw::points_near(c, P + 2, kEps6);
            if (!ok) {
                p->u10 = 0;
                return false;
            }
        } else if (r == 2) {
            const double lo1 = (Z[0] <= A[0]) ? Z[0] : A[0];
            const double hi1 = (A[0] <= Z[0]) ? Z[0] : A[0];
            const double lo2 = (T[0] <= S[0]) ? T[0] : S[0];
            const double hi2 = (S[0] <= T[0]) ? T[0] : S[0];
            if (lo1 < hi2 && lo2 < hi1) {
                p->u10 = 0;
                return false;
            }
        }
    }
    p->u10 = 1;
    return true;
}

// ---- min-area triangulation -------------------------------------------------------------------

// @src 140421810  combine
TriCost fun_140421810(const TriCost& x, const TriCost& y)
{
    TriCost r;
    r.a = (x.a < y.a || x.a == y.a) ? y.a : x.a;
    r.b = x.b + y.b;
    return r;
}

// @src 1404218a0  strict lexicographic less
bool fun_1404218a0(const TriCost& x, const TriCost& y)
{
    if (x.a < y.a || x.a == y.a) {
        if (y.a < x.a || y.a == x.a) {
            if (x.b < y.b || x.b == y.b) {
                if (y.b < x.b || y.b == x.b) return false;
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

// @src 140420750  ctor
// @src 140421970  table body ctor (operator new 0x20, 140421ed0)
// @src 140421ed0  vector body ctor
// @src 140421b70  part list ctor (operator new 0x18, 140422360)
// @src 140422360  list body ctor
// @src 1404221c0  list clear / detach (140422980 when sole owner)
Cls_140420750::Cls_140420750() = default;

// @src 140420840  reset
// @src 140421b40  table clear (140421f80)
void Cls_140420750::reset_part()
{
    k_ = -1;
    n_ = 0;
    v_ = nullptr;
    table_.clear();
}

// @src 140421920
std::int32_t Cls_140420750::chord(std::int32_t i, std::int32_t j) const
{
    std::int32_t d = (j - i) - 1;
    if (d < 0) d = d + n_;
    return d;
}

// @src 140421120
// @src 140421a00  table element pointer (140421e30 detach, 140421e00 data)
TriEntry& Cls_140420750::entry(std::int32_t i, std::int32_t j)
{
    const std::int32_t d = chord(i, j);
    return table_[static_cast<std::size_t>(d * n_ + i)];
}

// @src 140421060  table init
// @src 140421b00  reserve (1404222d0, 140422780)
// @src 140421a40  push_back (grow 1404222d0 to size + 1 + size / 2)
void Cls_140420750::init_table()
{
    k_ = n_ - 2;
    table_.clear();
    const std::int32_t total = (k_ + 1) * n_;
    table_.reserve(total > 0 ? static_cast<std::size_t>(total) : 0);
    for (std::int32_t i = 0; i < (k_ + 1) * n_; ++i) {
        TriEntry e;
        e.best = -1;
        e.cost.a = k180;
        e.cost.b = k1e29;
        table_.push_back(e);
    }
}

// @src 140421210  |area| of the triangle (14023e670 on the 3 points, then fabs)
double Cls_140420750::tri_area(std::int32_t i, std::int32_t j, std::int32_t k) const
{
    const std::int32_t id[3] = {v_[i], v_[j], v_[k]};
    double pts[9];
    for (std::int32_t m = 0; m < 3; ++m) {
        pts[m * 3] = coords_[id[m] * 3];
        pts[m * 3 + 1] = coords_[id[m] * 3 + 1];
        pts[m * 3 + 2] = coords_[id[m] * 3 + 2];
    }
    const double a = p904_ext().polygon_area(3, pts);
    return crt_fabs(a);
}

// @src 1404211a0  (180, |area|)
TriCost Cls_140420750::tri_cost(std::int32_t i, std::int32_t j, std::int32_t k) const
{
    const double a = tri_area(i, j, k);
    TriCost c;
    c.a = k180;
    c.b = a;
    return c;
}

// @src 140420a30  dynamic programme on one part
// (callee 1400030b0)  vector size (std::vector here)
// (callee 140005100)  vector data (std::vector here)
bool Cls_140420750::solve(const std::vector<std::int32_t>& poly)
{
    reset_part();
    n_ = static_cast<std::int32_t>(poly.size());
    v_ = poly.data();
    init_table();
    for (std::int32_t i = 0; i < n_; ++i) {
        std::int32_t j = (i + 1) % n_;
        TriEntry& e0 = entry(i, j);
        e0.cost.b = 0.0;   // +0x10
        e0.cost.a = 0.0;   // +0x08
        e0.best = -1;
        j = (i + 2) % n_;
        std::int32_t m = (i + 1) % n_;
        TriEntry& e1 = entry(i, j);
        e1.cost = tri_cost(i, m, j);
        e1.best = m;
    }
    if (1 < k_) {
        for (std::int32_t len = 2; len <= k_; ++len) {
            for (std::int32_t i = 0; i < n_; ++i) {
                const std::int32_t j = (i + 1 + len) % n_;
                std::int32_t m = (i + 1) % n_;
                TriEntry& e = entry(i, j);
                for (; m != j; m = (m + 1) % n_) {
                    const TriEntry& l = entry(i, m);
                    const TriEntry& r = entry(m, j);
                    const TriCost lr = fun_140421810(l.cost, r.cost);
                    const TriCost c = fun_140421810(lr, tri_cost(i, m, j));
                    if (fun_1404218a0(c, e.cost)) {
                        e.cost = c;
                        e.best = m;
                    }
                }
            }
        }
    }
    TriCost best;
    best.b = k1e29;  // local_108
    best.a = k180;   // local_110
    std::int32_t f = 0, g = 0;  // uninitialised in the binary; always set for n >= 3
    for (std::int32_t i = 0; i < n_; ++i) {
        for (std::int32_t j = (i + 1) % n_; j != i; j = (j + 1) % n_) {
            const TriEntry& a = entry(i, j);
            const TriEntry& b = entry(j, i);
            const TriCost c = fun_140421810(a.cost, b.cost);
            if (fun_1404218a0(c, best)) {
                best = c;
                f = i;
                g = j;
            }
        }
    }
    emit(f, g);
    emit(g, f);
    return true;
}

// @src 1404213c0  emit, pre-order
// (callee 140220810)  int vector push_back (std::vector here)
void Cls_140420750::emit(std::int32_t i, std::int32_t j)
{
    if (0 < chord(i, j)) {
        std::int32_t m = entry(i, j).best;
        const std::int32_t ids[3] = {v_[i], v_[m], v_[j]};
        const double a = tri_area(i, m, j);
        area_ = area_ + a;
        for (std::int32_t t = 0; t < 3; ++t) tris_.push_back(ids[t]);
        emit(i, m);
        emit(m, j);
    }
}

// @src 140421510  split
// (callee 14023b850)  squared distance (through P904Ext::dist2)
// @src 140421ca0  part list push_back (1404224e0 node insert)
void Cls_140420750::split(std::vector<std::int32_t>& poly)
{
    const std::int32_t m = static_cast<std::int32_t>(poly.size());
    if (m < 200) {
        std::vector<std::int32_t> copy;
        for (std::int32_t i = 0; i < m; ++i) copy.push_back(poly[static_cast<std::size_t>(i)]);
        parts_.push_back(copy);
        return;
    }
    double best = k1e29;
    std::int32_t a = 0, b = 0;  // uninitialised in the binary
    for (std::int32_t k = m / 2; 2 < k; --k) {
        for (std::int32_t i = 0; i < k; ++i) {
            const std::int32_t p = poly[static_cast<std::size_t>(i)];
            const std::int32_t q = poly[static_cast<std::size_t>((i + k) % m)];
            const double d = p904_ext().dist2(coords_ + p * 3, coords_ + q * 3);
            if (d < best) {
                a = i;
                b = (i + k) % m;
                best = d;
            }
        }
    }
    std::vector<std::int32_t> pa, pb;
    pa.push_back(poly[static_cast<std::size_t>(a)]);
    std::int32_t j = (a + 1) % m;
    bool first = true;
    while (j != a) {
        if (!first) pb.push_back(poly[static_cast<std::size_t>(j)]);
        else pa.push_back(poly[static_cast<std::size_t>(j)]);
        if (j == b && first) first = false;
        else j = (j + 1) % m;
    }
    pb.push_back(poly[static_cast<std::size_t>(a)]);
    poly.clear();   // 14019a7d0
    split(pa);
    split(pb);
}

// @src 140420890  run
// (callee 14019a7d0)  int vector clear (std::vector here)
// @src 140421c00  list begin, 140421c40 end, 140421d50 value, 140421d70 next (std::list here)
bool Cls_140420750::run(std::int32_t n, const std::int32_t* v, const double* coords)
{
    std::vector<std::int32_t> poly;
    for (std::int32_t i = 0; i < n; ++i) poly.push_back(v[i]);
    n_ = n;
    coords_ = coords;
    area_ = 0.0;
    bool ok = false;
    tris_.clear();
    parts_.clear();
    split(poly);
    for (const std::vector<std::int32_t>& part : parts_) ok = solve(part);
    return ok;
}

} // namespace def::model::geoprim
