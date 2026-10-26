// ctest-labels: p67-7 ctc3d
// Tier-1 tests of MDSPS2_REC FUN_14006798b and its helpers (packet p67-7, group "search") on a
// hand-made surface: flat plates at z = 0 (normal +z) in a real type-3 cube of group "cube"
// (140225ff0), attributes set with the cube's Init getters. Expected values are worked out by hand
// from the geometry.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <testkit/check.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <vector>

using namespace def::cmd::ctc3d;

// ---- surface on a real type-3 cube (group "cube"), attributes written with the Init getters ----
namespace {

struct Plate {
    std::vector<double> xyz;            // points (the cube aliases this array)
    std::vector<i32> faces;             // 4 per polygon (n4 = n1 for a triangle)
    std::vector<std::array<i32, 4>> pl; // same, for the attribute set-up
    i32 np = 0;
    i32 nf = 0;
};
Plate g;
constexpr i32 kObj = 2;

double& X(i32 k, int c) { return g.xyz[static_cast<std::size_t>(3 * (k - 1) + c)]; }

void set_ei(i32 e, i32 f, std::vector<i32> v)
{
    i32 obj = kObj, inst = 1, n = static_cast<i32>(v.size());
    fun_1402280d0(&obj, &inst, &e, &f, v.data(), &n);
}
void set_er(i32 e, i32 f, std::vector<double> v)
{
    i32 obj = kObj, inst = 1, n = static_cast<i32>(v.size());
    fun_1402283b0(&obj, &inst, &e, &f, v.data(), &n);
}
void set_ni(i32 e, i32 f, std::vector<i32> v)
{
    i32 obj = kObj, inst = 1, n = static_cast<i32>(v.size());
    fun_140228690(&obj, &inst, &e, &f, v.data(), &n);
}
void set_nr(i32 e, i32 f, std::vector<double> v)
{
    i32 obj = kObj, inst = 1, n = static_cast<i32>(v.size());
    fun_140228980(&obj, &inst, &e, &f, v.data(), &n);
}

// builds the cube, then EI1 (neighbour across edge k by the reversed half-edge), EI2 = 1, ER1 =
// +z, ER2 = area, ER3 = pi at an edge with a neighbour else 0, NI1, NI3 = 1, NR1 = (0,0,1,conv),
// NR2 = 1
void finish(double conv = 0.0)
{
    i32 n3 = 3, vb = 0;
    fun_140225820(&vb);
    fun_140225640(&n3, &vb);
    i32 obj = kObj, inst = 1, type = 3, nv4 = 4, base = 1;
    fun_140225ff0(&obj, &inst, &type, &g.np, g.xyz.data(), &g.nf, &nv4, g.faces.data(), &base, &vb);
    std::vector<std::vector<i32>> ni1(static_cast<std::size_t>(g.np) + 1);
    for (i32 p = 1; p <= g.nf; ++p) {
        const auto& c = g.pl[static_cast<std::size_t>(p - 1)];
        const int nv = (c[0] == c[3]) ? 3 : 4;
        std::vector<i32> e1(static_cast<std::size_t>(2 * nv), 0);
        std::vector<double> e3(static_cast<std::size_t>(nv), 0.0);
        for (int k = 0; k < nv; ++k) {
            const i32 a = c[static_cast<std::size_t>(k)], b = c[static_cast<std::size_t>((k + 1) % nv)];
            for (i32 q = 1; q <= g.nf; ++q) {
                if (q == p) continue;
                const auto& d = g.pl[static_cast<std::size_t>(q - 1)];
                const int mv = (d[0] == d[3]) ? 3 : 4;
                for (int j = 0; j < mv; ++j) {
                    if (d[static_cast<std::size_t>(j)] == b && d[static_cast<std::size_t>((j + 1) % mv)] == a) {
                        e1[static_cast<std::size_t>(k)] = q;
                        e3[static_cast<std::size_t>(k)] = 3.141592653589793;
                    }
                }
            }
        }
        double A = 0.0;
        const double* x1 = &X(c[0], 0);
        for (int k = 1; k + 1 < nv; ++k) {
            const double* a = &X(c[static_cast<std::size_t>(k)], 0);
            const double* b = &X(c[static_cast<std::size_t>(k + 1)], 0);
            A += 0.5 * std::fabs((a[0] - x1[0]) * (b[1] - x1[1]) - (a[1] - x1[1]) * (b[0] - x1[0]));
        }
        set_ei(p, 1, e1);
        set_ei(p, 2, {1});
        set_er(p, 1, {0.0, 0.0, 1.0});
        set_er(p, 2, {A});
        set_er(p, 3, e3);
        for (int k = 0; k < nv; ++k) ni1[static_cast<std::size_t>(c[static_cast<std::size_t>(k)])].push_back(p);
    }
    for (i32 k = 1; k <= g.np; ++k) {
        set_ni(k, 1, ni1[static_cast<std::size_t>(k)]);
        set_ni(k, 3, {1});
        set_nr(k, 1, {0.0, 0.0, 1.0, conv});
        set_nr(k, 2, {1.0});
    }
}

// n x n quads (or 2n^2 triangles) on [0, n] x [0, n], z = 0, counter-clockwise from above
void plate(int n, bool triangles)
{
    g = Plate{};
    for (int j = 0; j <= n; ++j)
        for (int i = 0; i <= n; ++i) {
            g.xyz.push_back(i);
            g.xyz.push_back(j);
            g.xyz.push_back(0.0);
        }
    g.np = (n + 1) * (n + 1);
    auto id = [n](int i, int j) { return static_cast<i32>(j * (n + 1) + i + 1); };
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i) {
            if (!triangles) {
                g.pl.push_back({id(i, j), id(i + 1, j), id(i + 1, j + 1), id(i, j + 1)});
            } else {
                g.pl.push_back({id(i, j), id(i + 1, j), id(i + 1, j + 1), id(i, j)});
                g.pl.push_back({id(i, j), id(i + 1, j + 1), id(i, j + 1), id(i, j)});
            }
        }
    for (const auto& c : g.pl) g.faces.insert(g.faces.end(), c.begin(), c.end());
    g.nf = static_cast<i32>(g.pl.size());
}

} // namespace

namespace {

struct Res {
    double st[2] = {-7, -7}, d = 5.0, q[3] = {-7, -7, -7};
    i32 poly = -7, status = -7;
};

Res search(double x, double y, double z, i32 mode = 0, i32 erfield = 3)
{
    Res r;
    i32 obj = kObj, inst = 1, npoly = g.nf, dbg = 0;
    double p[3] = {x, y, z}, dirn[4] = {0, 0, 0, 0};
    fun_14006798b(&obj, &inst, &erfield, &npoly, p, r.st, &r.poly, &r.d, &r.status, r.q, dirn, &mode, &dbg);
    return r;
}

bool near(double a, double b) { return std::fabs(a - b) <= 1e-12; }

} // namespace

TEST_CASE(mdsps2_quad_plate_above_and_below)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish();
    Res r = search(0.7, 0.6, 0.3);
    CHECK_EQ(r.status, 1);
    CHECK_EQ(r.poly, 1);
    CHECK(near(r.d, 0.3));
    CHECK(near(r.q[0], 0.7));
    CHECK(near(r.q[1], 0.6));
    CHECK(near(r.q[2], 0.0));
    CHECK(near(r.st[0], 0.4)); // s = 2x - 1 on [0, 1]
    CHECK(near(r.st[1], 0.2));
    Res b = search(1.6, 1.3, -0.25);
    CHECK_EQ(b.status, 1);
    CHECK_EQ(b.poly, 4);
    CHECK(near(b.d, -0.25));
}

TEST_CASE(mdsps2_triangle_plate)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(2, true);
    finish();
    // (0.7, 0.2) lies in the lower triangle of the first square (polygon 1: (0,0) (1,0) (1,1))
    Res r = search(0.7, 0.2, 0.4);
    CHECK_EQ(r.status, 1);
    CHECK_EQ(r.poly, 1);
    CHECK(near(r.d, 0.4));
    CHECK(near(r.q[0], 0.7));
    CHECK(near(r.q[1], 0.2));
    // area coordinates: weight of vertex 1 and 2 (q = s*X1 + t*X2 + (1-s-t)*X3)
    CHECK(near(r.st[0], 0.3));
    CHECK(near(r.st[1], 0.5));
    Res b = search(0.2, 0.7, -0.1);
    CHECK_EQ(b.poly, 2);
    CHECK(near(b.d, -0.1));
}

TEST_CASE(mdsps2_outside_boundary_edge)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish();
    // beyond the x = 2 boundary edge of polygon 2: foot on the edge, no neighbour -> code 0,
    // distance |p - foot| with the sign of (p - q).n (positive, above the plane)
    Res r = search(2.3, 0.5, 0.4);
    CHECK_EQ(r.status, 1);
    CHECK_EQ(r.poly, 2);
    CHECK(near(r.q[0], 2.0));
    CHECK(near(r.q[1], 0.5));
    CHECK(near(r.d, 0.5));
}

TEST_CASE(mdsps2_skips_disabled_polygon)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish();
    set_ei(1, 2, {0}); // polygon 1 disabled
    Res r = search(0.7, 0.6, 0.3);
    CHECK_EQ(r.status, 1);
    CHECK(r.poly != 1);
    // mode > 0 accepts every polygon
    Res a = search(0.7, 0.6, 0.3, 1);
    CHECK_EQ(a.poly, 1);
}

TEST_CASE(mdsps2_no_polygon_found)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(1, false);
    finish();
    set_ei(1, 2, {0});
    Res r = search(0.5, 0.5, 0.2);
    CHECK_EQ(r.status, -1);
    CHECK_EQ(r.poly, 0);
    CHECK(r.d >= 0.0);
}

TEST_CASE(reorder_14006e938)
{
    reset_image();
    plate(2, false);
    finish();
    set_ei(1, 2, {-3});
    set_ei(2, 2, {0});
    set_ei(3, 2, {1});
    set_ei(4, 2, {-1000});
    i32 obj = kObj, inst = 1, n = 4, k = 3;
    i32 list[4] = {1, 2, 3, 4};
    fun_14006e938(&obj, &inst, list, &n, &k);
    CHECK_EQ(list[0], 1);
    CHECK_EQ(list[1], 3);
    CHECK_EQ(list[2], 2);
    CHECK_EQ(list[3], 4);
    i32 list2[4] = {1, 2, 3, 4};
    k = 0;
    fun_14006e938(&obj, &inst, list2, &n, &k);
    CHECK_EQ(list2[0], 3);
    CHECK_EQ(list2[1], 1);
    CHECK_EQ(list2[2], 2);
    CHECK_EQ(list2[3], 4);
}

TEST_CASE(polygon_coords_14004f363_and_box_14006790a)
{
    reset_image();
    plate(1, true);
    finish();
    i32 obj = kObj, inst = 1, poly = 1, conn[4], cnt = 0;
    double x[12] = {};
    fun_14004f363(&obj, &inst, &poly, x, conn, &cnt);
    CHECK_EQ(cnt, 4); // n4 = n1 > 0 counts
    CHECK_EQ(conn[1], 2);
    CHECK(x[3] == 1.0 && x[4] == 0.0);
    double box[6] = {5, 5, 5, 6, 6, 6};
    i32 empty = -1;
    fun_14006790a(&obj, &inst, box, &empty);
    CHECK_EQ(empty, 1);
    double box2[6] = {0.2, 0.2, -1, 0.3, 0.3, 1};
    fun_14006790a(&obj, &inst, box2, &empty);
    CHECK_EQ(empty, 0);
}

TEST_CASE(mdsps2_vertex_foot_sign)
{
    // beyond the corner (0, 0): foot at vertex 1 of polygon 1 (code -1), sign from NR1 of node 1
    const double dd = std::sqrt((0.09 + 0.16) + 0.04);
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish(0.0); // undecided vertex: u = unit(p - q) against the node normal
    Res r = search(-0.3, -0.4, 0.2);
    CHECK_EQ(r.poly, 1);
    CHECK(near(r.d, dd));
    // below the plane: u.n <= 0 -> vertex re-search 14006de2d: the ray to the centroid of
    // polygon 1 (largest at node 1) meets nothing else, so the sign comes from polygon 1: inside
    Res b = search(-0.3, -0.4, -0.2);
    CHECK_EQ(b.poly, 1);
    CHECK(near(b.d, -dd));
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish(-1.0); // concave node: always inside
    Res c = search(-0.3, -0.4, 0.2);
    CHECK(near(c.d, -dd));
}

TEST_CASE(mdsps2_edge_foot_dihedral)
{
    // above the shared edge x = 1 of polygons 1 and 2: polygon 1 wins the tie (strict <), foot on
    // its edge 2; the sign follows ER3 of that edge
    struct C {
        double e;
        double d;
    };
    const C cases[] = {{1.0, -0.3}, {5.0, 0.3}, {0.05, 0.3}, {3.141592653589793, 0.3}, {3.2, 0.3}};
    for (const C& c : cases) {
        reset_image();
        dat_14077a7bc = 6;
        plate(2, false);
        finish();
        set_er(1, 3, {0.0, c.e, 3.141592653589793, 0.0});
        Res r = search(1.0, 0.5, 0.3);
        CHECK_EQ(r.poly, 1);
        CHECK(near(r.d, c.d));
    }
}

TEST_CASE(mdsps2_direction_filter_dropped_after_first_pass)
{
    reset_image();
    dat_14077a7bc = 6;
    plate(2, false);
    finish();
    Res r;
    i32 obj = kObj, inst = 1, npoly = 4, dbg = 0, mode = 0, erf = 3;
    double p[3] = {0.7, 0.6, 0.3}, dirn[4] = {0.0, 0.0, 1.0, 0.5}; // every normal fails c <= lim
    fun_14006798b(&obj, &inst, &erf, &npoly, p, r.st, &r.poly, &r.d, &r.status, r.q, dirn, &mode, &dbg);
    CHECK_EQ(r.poly, 1);
    CHECK(near(r.d, 0.3));
}

TEST_CASE(neighbour_projection_14006fb9b)
{
    reset_image();
    double xq[12] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0}, nrm[3] = {0, 0, 1};
    double p[3] = {0.25, 0.5, -0.2}, s = -7, t = -7, q[3], din = 9.0, d = 0.0;
    i32 kind = 0, trimode = 0, flag = -7, inside = -7;
    fun_14006fb9b(xq, nrm, &kind, p, &s, &t, q, &din, &d, &trimode, &flag, &inside);
    CHECK(near(d, -0.2));
    CHECK(near(s, -0.5));
    CHECK(near(t, 0.0));
    CHECK_EQ(inside, 0);
    CHECK_EQ(trimode, 0);
    double xt[9] = {0, 0, 0, 1, 0, 0, 0, 1, 0};
    double p2[3] = {0.2, 0.2, 0.3};
    kind = 1;
    din = -5.0;
    fun_14006fb9b(xt, nrm, &kind, p2, &s, &t, q, &din, &d, &trimode, &flag, &inside);
    CHECK(near(d, 0.3));
    CHECK(near(q[0], 0.2) && near(q[1], 0.2) && near(q[2], 0.0));
    CHECK_EQ(trimode, 1);
    CHECK_EQ(inside, 0);
}

int main() { return testkit::run_all(); }
