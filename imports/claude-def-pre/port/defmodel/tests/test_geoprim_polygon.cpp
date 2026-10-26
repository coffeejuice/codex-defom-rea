// GEOPRIM polygon library and shell faces (packet p9-04): segment crossing 140308340, the
// simple-profile test 1402e7f40, the min-area triangulation 1402e24f0 (solver 0x140420750..),
// the shell face generator 1402bc980 and the normal test 1402bc720. Expected values are worked
// out by hand from the listing (CNVT3D_SWEEP.md 10.5 / 10.6); the kernels other packets own
// (14023e670, 14023b850, 14023a380, 140240d80) are given here as written in the listing.
// ctest-labels: p9-04 geoprim
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/geoprim/extrude.hpp>
#include <defmodel/geoprim/polygon.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

using namespace def::model::geoprim;

namespace {

// 14023a4b0 cross, 14023a1b0 add, 1402399b0 / 140239a30 length, 14023e670 area (DAT_14051f240 0.5)
void cross(const double* p, const double* q, double* r)
{
    r[0] = p[1] * q[2] - p[2] * q[1];
    r[1] = p[2] * q[0] - p[0] * q[2];
    r[2] = p[0] * q[1] - p[1] * q[0];
}
double area_listing(std::int32_t n, const double* pts)
{
    double acc[3] = {0.0, 0.0, 0.0};
    for (std::int32_t i = 0; i < n; ++i) {
        const double* p = pts + i * 3;
        const double* q = (i == n - 1) ? pts : pts + (i + 1) * 3;
        double c[3];
        cross(p, q, c);
        acc[0] = acc[0] + c[0];
        acc[1] = acc[1] + c[1];
        acc[2] = acc[2] + c[2];
    }
    return 0.5 * std::sqrt(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
}
// 14023b850
double dist2_listing(const double* a, const double* b)
{
    return (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]);
}
// 14023a380
double dot_listing(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
// 140240d80 with 140239fb0 (sub) and 140239b40 (unit, zero below 1e-12)
void normal_listing(double* out, const double* p, std::int32_t n)
{
    double acc[3] = {0.0, 0.0, 0.0};
    double prev[3] = {p[3] - p[0], p[4] - p[1], p[5] - p[2]};
    for (std::int32_t k = 2; k <= n - 1; ++k) {
        double cur[3] = {p[k * 3] - p[0], p[k * 3 + 1] - p[1], p[k * 3 + 2] - p[2]};
        double c[3];
        cross(prev, cur, c);
        acc[0] = acc[0] + c[0];
        acc[1] = acc[1] + c[1];
        acc[2] = acc[2] + c[2];
        std::memcpy(prev, cur, sizeof prev);
    }
    const double len = std::sqrt(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
    if (std::fabs(len) <= 1e-12) {
        out[0] = out[1] = out[2] = 0.0;
    } else {
        out[0] = acc[0] / len;
        out[1] = acc[1] / len;
        out[2] = acc[2] / len;
    }
}

void install_kernels()
{
    p904_ext().polygon_area = area_listing;
    p904_ext().dist2 = dist2_listing;
    p904_ext().dot = dot_listing;
    p904_ext().poly_normal = normal_listing;
}

// profile with 1-based points
struct Profile {
    std::vector<double> buf;
    ProfileArray p{};
    explicit Profile(const std::vector<double>& xy)
    {
        buf.assign(2, 0.0);
        buf.insert(buf.end(), xy.begin(), xy.end());
        p.n = static_cast<std::int32_t>(xy.size() / 2);
        p.pts = buf.data();
        p.u10 = 0;
    }
};

} // namespace

TEST_CASE(line_crossing_of_the_diagonals_of_a_square)
{
    double x = -1, y = -1;
    // a1 = 2, b1 = -2, c1 = 0; a2 = -2, b2 = -2, c2 = 4; det = -8; x = -8/-8, y = -8/-8
    CHECK_EQ(fun_140308340(0, 0, 2, 2, 0, 2, 2, 0, &x, &y), 1);
    CHECK_BITS(x, 1.0);
    CHECK_BITS(y, 1.0);
    // parallel: both lines pass the side tests with r = 0 only when collinear
    CHECK_EQ(fun_140308340(0, 0, 1, 0, 2, 0, 3, 0, &x, &y), 2);
    // disjoint: (0,0)-(1,0) and (0,1)-(1,2): r3 * r4 > 0
    CHECK_EQ(fun_140308340(0, 0, 1, 0, 0, 1, 1, 2, &x, &y), 0);
    const double s1[6] = {0, 0, 0, 1, 0, 0}, s2[6] = {5, 5, 0, 6, 6, 0};
    double out[2] = {9, 9};
    CHECK_EQ(fun_140308120(s1, s2, out), 0);
    CHECK_BITS(out[0], 0.0);
    CHECK_BITS(out[1], 0.0);
}

TEST_CASE(box_test_uses_the_unsorted_end_points)
{
    const double p[2] = {0.5, 1.0};
    const double up[4] = {-1, 1, 2, 1}, down[4] = {2, 1, -1, 1};
    CHECK(fun_140309960(p, up));
    CHECK(!fun_140309960(p, down));
}

TEST_CASE(simple_profile_test_checks_only_the_closing_edge)
{
    Profile sq({0, 0, 1, 0, 1, 1, 0, 1});
    CHECK(fun_1402e7f40(&sq.p));
    CHECK_EQ(int(sq.p.u10), 1);
    // a bow tie passes: its closing edge (0,0)-(0,1) meets the other edges only at p1 / pn
    Profile bow({0, 0, 1, 1, 1, 0, 0, 1});
    CHECK(fun_1402e7f40(&bow.p));
    // closing edge (0,0)-(1,2) crosses (-1,1)-(2,1) at (0.5, 1), inside both boxes
    Profile bad({0, 0, -1, 1, 2, 1, 1, 2});
    CHECK(!fun_1402e7f40(&bad.p));
    CHECK_EQ(int(bad.p.u10), 0);
    // the same crossing with the edge written (2,1)-(-1,1): its box holds nothing -> passes
    Profile quirk({0, 0, 2, 1, -1, 1, 1, 2});
    CHECK(fun_1402e7f40(&quirk.p));
    // cached: a set byte returns true at once
    bad.p.u10 = 1;
    CHECK(fun_1402e7f40(&bad.p));
    Profile two({0, 0, 1, 0});
    CHECK(!fun_1402e7f40(&two.p));
}

TEST_CASE(min_area_triangulation_of_a_triangle_and_a_square)
{
    install_kernels();
    {
        // n = 3: closing pair (0, 1); emit(1, 0) with best (1 + 1) % 3 = 2 -> (v1, v2, v0)
        const double pts[8] = {0, 0, 0, 0, 1, 0, 0, 1};
        std::int32_t t[12] = {};
        CHECK_EQ(fun_1402e24f0(nullptr, 3, pts, t), 1);
        CHECK_EQ(t[0], 2);
        CHECK_EQ(t[1], 3);
        CHECK_EQ(t[2], 1);
    }
    {
        // square: every chord costs (180, 1.0); first strict minimum: closing (0, 1),
        // entry(1, 0).best = 2, entry(2, 0).best = 3 -> (2, 3, 1), (3, 4, 1)
        const double pts[10] = {0, 0, 0, 0, 1, 0, 1, 1, 0, 1};
        std::int32_t t[16] = {};
        CHECK_EQ(fun_1402e24f0(nullptr, 4, pts, t), 2);
        const std::int32_t want[6] = {2, 3, 1, 3, 4, 1};
        for (int i = 0; i < 6; ++i) CHECK_EQ(t[i], want[i]);
    }
}

TEST_CASE(min_area_triangulation_avoids_outside_chords)
{
    install_kernels();
    // L shape of area 3: any chord outside the polygon adds area, so the minimum is 3
    const double xy[] = {0, 0, 2, 0, 2, 1, 1, 1, 1, 2, 0, 2};
    std::vector<double> pts(2, 0.0);
    pts.insert(pts.end(), std::begin(xy), std::end(xy));
    std::vector<std::int32_t> ids = {1, 2, 3, 4, 5, 6};
    std::vector<double> c = {0, 0, 0};
    for (int i = 1; i <= 6; ++i) {
        c.push_back(pts[i * 2]);
        c.push_back(pts[i * 2 + 1]);
        c.push_back(0.0);
    }
    Cls_140420750 s;
    CHECK(s.run(6, ids.data(), c.data()));
    CHECK_EQ(s.result().size(), std::size_t{12});
    CHECK_BITS(s.area(), 3.0);
}

TEST_CASE(large_polygons_are_split_at_the_shortest_diagonal)
{
    install_kernels();
    // 400-gon on a circle squeezed in y: the shortest chord pair is found and both parts are
    // solved; n - 2 triangles in all, each vertex id used
    const int n = 400;
    std::vector<double> c = {0, 0, 0};
    std::vector<std::int32_t> ids;
    for (int i = 0; i < n; ++i) {
        const double a = 6.283185307179586 * i / n;
        c.push_back(std::cos(a));
        c.push_back(0.25 * std::sin(a));
        c.push_back(0.0);
        ids.push_back(i + 1);
    }
    Cls_140420750 s;
    CHECK(s.run(n, ids.data(), c.data()));
    CHECK_EQ(s.result().size(), std::size_t(3 * (n - 2)));
}

namespace {
struct MeshFixture {
    std::unique_ptr<DEF_GEOPRIM> prim{new DEF_GEOPRIM()};
    std::vector<std::int32_t> conn = {1, 2, 5, 4, 2, 3, 6, 5};
    std::vector<std::int32_t> blist = {1, 2, 3, 6, 5, 4, 1};
    static std::vector<std::int32_t>* s_conn;
    static std::int32_t* conn_hook(def::model::OBJ_MESH2D*) { return s_conn->data(); }
    MeshFixture()
    {
        std::int32_t np = 6, ne = 2;
        std::memcpy(reinterpret_cast<char*>(prim.get()) + 0x8 + 0x12c, &np, 4);
        std::memcpy(reinterpret_cast<char*>(prim.get()) + 0x148 + 0xe8, &ne, 4);
        s_conn = &conn;
        ext().mesh_conn = conn_hook;
        std::int32_t one = 1, n = static_cast<std::int32_t>(blist.size());
        prim->int_lists.resize(&one);
        prim->int_lists.assign(&one, &n, blist.data());
    }
};
std::vector<std::int32_t>* MeshFixture::s_conn = nullptr;
} // namespace

TEST_CASE(shell_faces_of_a_two_quad_mesh_in_two_sections)
{
    MeshFixture f;
    std::int32_t q[16 * 4] = {};
    const std::int32_t ns = 2;
    const char flip = 0, closed = 0;
    CHECK_EQ(fun_1402bc980(f.prim.get(), &ns, nullptr, q, &flip, &closed), 16);
    CHECK_EQ(f.prim->int_lists.items[0].n, 6);   // closed list shortened in place
    const std::int32_t want[16][4] = {
        {1, 2, 5, 4}, {2, 3, 6, 5},                                          // bottom
        {1, 7, 8, 2}, {2, 8, 9, 3}, {3, 9, 10, 6}, {6, 10, 11, 5}, {5, 11, 12, 4}, {4, 12, 7, 1},
        {7, 13, 14, 8}, {8, 14, 15, 9}, {9, 15, 18, 10}, {10, 18, 17, 11}, {11, 17, 16, 12},
        {12, 16, 13, 7},                                                     // s = ns
        {17, 14, 13, 16}, {18, 15, 14, 17}};                                 // top, a/c swapped
    for (int i = 0; i < 16; ++i)
        for (int k = 0; k < 4; ++k) CHECK_EQ(q[i * 4 + k], want[i][k]);
}

TEST_CASE(shell_faces_flip_and_single_section)
{
    MeshFixture f;
    std::int32_t q[10 * 4] = {};
    const std::int32_t ns = 1;
    const char flip = 1, closed = 0;
    CHECK_EQ(fun_1402bc980(f.prim.get(), &ns, nullptr, q, &flip, &closed), 10);
    CHECK_EQ(q[0], 5);   // bottom (1,2,5,4) swapped
    CHECK_EQ(q[2], 1);
    // ns == 1 strip j0: (1, 1+6, 2+6, 2) swapped -> (8, 7, 1, 2)
    CHECK_EQ(q[8], 8);
    CHECK_EQ(q[9], 7);
    CHECK_EQ(q[10], 1);
    CHECK_EQ(q[11], 2);
    // top: base 6, not swapped when flip
    CHECK_EQ(q[8 * 4], 7);
    CHECK_EQ(q[8 * 4 + 2], 11);
}

TEST_CASE(normal_test_of_the_first_element)
{
    install_kernels();
    MeshFixture f;
    const double ccw[18] = {0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 0, 2, 1, 0};
    CHECK(fun_1402bc720(f.prim.get(), ccw));
    f.conn = {1, 4, 5, 2, 2, 5, 6, 3};
    CHECK(!fun_1402bc720(f.prim.get(), ccw));
}

TEST_CASE(closed_int_list_and_list_resize)
{
    IntArray a;
    CHECK(!fun_1402e91d0(&a));
    const std::int32_t n = 3, v[3] = {4, 5, 4};
    a.assign(&n, v);
    CHECK(fun_1402e91d0(&a));
    IntArrayList l;
    l.init();
    const std::int32_t two = 2, k0 = 0, k3 = 3, k2 = 2, len = 5;
    l.resize(&two);
    fun_1402e9470(&l, &k0, &len);
    fun_1402e9470(&l, &k3, &len);
    CHECK_EQ(l.items[0].n, 0);
    fun_1402e9470(&l, &k2, &len);
    CHECK_EQ(l.items[1].n, 5);
    l.clear();
}

int main() { return testkit::run_all(); }
