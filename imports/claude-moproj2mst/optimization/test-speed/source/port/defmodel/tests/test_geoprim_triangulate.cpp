// ctest-labels: p9-05 geoprim
// Tier-1 tests of the cap triangulator 14041f830 (trapezoidation, monotone pieces, greedy
// triangulation). Expected values are geometric facts that hold for any correct triangulation of
// a simple counter-clockwise polygon: n-2 triangles, every triangle counter-clockwise (or
// degenerate), triangle areas summing to the polygon area, every edge of the polygon used once.
// The binary seeds the segment insertion order with srand(time(NULL)) (14045c530): the output
// (order, rotation and even the choice of diagonals) depends on the second the run starts. The
// test checks validity for several seeds and that one seed always gives the same result.
#include <defmodel/geoprim/triangulate.hpp>
#include <defrt/clock.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <map>
#include <utility>
#include <vector>

using namespace def::model::geoprim;

namespace {

struct Poly {
    std::vector<double> pts;   // 1-based: pts[0], pts[1] unused
    int n() const { return (int)(pts.size() / 2) - 1; }
};

Poly make(std::initializer_list<std::pair<double, double>> p)
{
    Poly r;
    r.pts = {0.0, 0.0};
    for (auto& q : p) {
        r.pts.push_back(q.first);
        r.pts.push_back(q.second);
    }
    return r;
}

double tri_area2(const Poly& p, int a, int b, int c)
{
    const double ax = p.pts[2 * a], ay = p.pts[2 * a + 1];
    const double bx = p.pts[2 * b], by = p.pts[2 * b + 1];
    const double cx = p.pts[2 * c], cy = p.pts[2 * c + 1];
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

double poly_area2(const Poly& p)
{
    double s = 0;
    const int n = p.n();
    for (int i = 1; i <= n; ++i) {
        int j = i % n + 1;
        s += p.pts[2 * i] * p.pts[2 * j + 1] - p.pts[2 * j] * p.pts[2 * i + 1];
    }
    return s;
}

std::vector<int> run(const Poly& p, std::int64_t seed_time)
{
    def::rt::pin_clock(seed_time);
    int n = p.n();
    std::vector<int> tri(3 * (size_t)(n + 4), -7);
    int nt = fun_14041f830(1, &n, p.pts.data(), tri.data());
    def::rt::pin_clock(-1);
    tri.resize(3 * (size_t)nt);
    return tri;
}

void check_valid(const Poly& p, const std::vector<int>& tri)
{
    const int n = p.n();
    CHECK_EQ((int)tri.size(), 3 * (n - 2));
    double sum = 0;
    std::map<std::pair<int, int>, int> edges;
    for (size_t k = 0; k + 2 < tri.size(); k += 3) {
        const int a = tri[k], b = tri[k + 1], c = tri[k + 2];
        CHECK(a >= 1 && a <= n && b >= 1 && b <= n && c >= 1 && c <= n);
        const double t = tri_area2(p, a, b, c);
        CHECK(t >= -1e-9);
        sum += std::fabs(t);
        edges[{a, b}]++;
        edges[{b, c}]++;
        edges[{c, a}]++;
    }
    CHECK(std::fabs(sum - poly_area2(p)) <= 1e-9 * std::fabs(poly_area2(p)));
    for (int i = 1; i <= n; ++i) {   // each boundary edge appears once, in polygon direction
        const int j = i % n + 1;
        const int cnt = edges[std::make_pair(i, j)];
        CHECK_EQ(cnt, 1);
    }
}

void check_all_seeds(const Poly& p)
{
    const auto ref = run(p, 1700000000);
    check_valid(p, ref);
    for (std::int64_t s : {0LL, 1LL, 12345LL, 1761040000LL, 1761043601LL, 2000000000LL}) {
        const auto r = run(p, s);
        check_valid(p, r);
        CHECK(run(p, s) == r);
    }
}

} // namespace

TEST_CASE(crt_rand_sequence)
{
    // MSVC CRT: holdrand = holdrand * 214013 + 2531011, (holdrand >> 16) & 0x7fff; seed 1
    // gives 41, 18467, 6334 (well-known first values of rand() without srand).
    tri_detail::crt_srand(1);
    CHECK_EQ(tri_detail::crt_rand(), 41);
    CHECK_EQ(tri_detail::crt_rand(), 18467);
    CHECK_EQ(tri_detail::crt_rand(), 6334);
}

TEST_CASE(triangle)
{
    Poly p = make({{0, 0}, {10, 0}, {0, 10}});
    check_all_seeds(p);
}

TEST_CASE(square)
{
    Poly p = make({{0, 0}, {10, 0}, {10, 10}, {0, 10}});
    check_all_seeds(p);
}

TEST_CASE(die_profile)
{
    // a closed die section r = 0..60, z = 0..40 with a step and an inclined flank, CCW
    Poly p = make({{0, 0}, {60, 0}, {60, 15}, {45, 15}, {35, 40}, {0, 40}});
    check_all_seeds(p);
}

TEST_CASE(nonconvex_comb)
{
    Poly p = make({{0, 0}, {50, 0}, {50, 30}, {40, 30}, {40, 10}, {30, 10}, {30, 30}, {20, 30},
                   {20, 10}, {10, 10}, {10, 30}, {0, 30}});
    check_all_seeds(p);
}

TEST_CASE(ring_section_with_arc)
{
    Poly p;
    p.pts = {0.0, 0.0};
    // outer arc of radius 30 from 0 to 90 degrees around (10, 0), then back along the axes
    for (int k = 0; k <= 12; ++k) {
        const double a = 3.141592653589793 * 0.5 * k / 12.0;
        p.pts.push_back(10.0 + 30.0 * std::cos(a));
        p.pts.push_back(30.0 * std::sin(a));
    }
    p.pts.push_back(0.0);
    p.pts.push_back(30.0);
    p.pts.push_back(0.0);
    p.pts.push_back(0.0);
    check_all_seeds(p);
}

int main() { return testkit::run_all(); }
