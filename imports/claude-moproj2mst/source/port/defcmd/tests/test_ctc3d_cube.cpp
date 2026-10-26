// ctest-labels: p67-7 ctc3d cube
// Tier-1 test of the p67-7 cube group: a type-3 cube (points + polygons) built from the surface
// of a 2x2x2-cell box (26 points, 24 quads), then the attribute lists, connectivity, point
// coordinates, box query + unique list, nearest point (shell search and brute force), initial
// radius and cell sizes.
#include <defcmd/ctc3d/cube.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <vector>

using namespace def::cmd::ctc3d;

namespace {

struct Surf {
    std::vector<double> x; // 3 per point
    std::vector<i32> f;    // 4 per quad, 1-based
    i32 np = 0;
    i32 nf = 0;
};

// lattice points (i, j, k) in {0,1,2}^3 on the surface of [0,2]^3 scaled by s; quads of the 6 faces
Surf make_box(double s)
{
    Surf b;
    i32 id[3][3][3];
    for (i32 i = 0; i < 3; ++i) {
        for (i32 j = 0; j < 3; ++j) {
            for (i32 k = 0; k < 3; ++k) {
                id[i][j][k] = 0;
                if (i == 1 && j == 1 && k == 1) {
                    continue;
                }
                b.x.push_back(i * s);
                b.x.push_back(j * s);
                b.x.push_back(k * s);
                id[i][j][k] = ++b.np;
            }
        }
    }
    auto quad = [&](i32 a, i32 c, i32 d, i32 e) {
        b.f.push_back(a);
        b.f.push_back(c);
        b.f.push_back(d);
        b.f.push_back(e);
        ++b.nf;
    };
    for (i32 u = 0; u < 2; ++u) {
        for (i32 v = 0; v < 2; ++v) {
            quad(id[0][u][v], id[0][u][v + 1], id[0][u + 1][v + 1], id[0][u + 1][v]); // x = 0
            quad(id[2][u][v], id[2][u + 1][v], id[2][u + 1][v + 1], id[2][u][v + 1]); // x = 2
            quad(id[u][0][v], id[u + 1][0][v], id[u + 1][0][v + 1], id[u][0][v + 1]); // y = 0
            quad(id[u][2][v], id[u][2][v + 1], id[u + 1][2][v + 1], id[u + 1][2][v]); // y = 2
            quad(id[u][v][0], id[u][v + 1][0], id[u + 1][v + 1][0], id[u + 1][v][0]); // z = 0
            quad(id[u][v][2], id[u + 1][v][2], id[u + 1][v + 1][2], id[u][v + 1][2]); // z = 2
        }
    }
    return b;
}

Surf g_box = make_box(0.5); // [0,1]^3

void build(i32 obj)
{
    i32 inst = 1, type = 3, nv = 4, base = 1, vb = 0;
    fun_140225ff0(&obj, &inst, &type, &g_box.np, g_box.x.data(), &g_box.nf, &nv, g_box.f.data(), &base, &vb);
}

void setup()
{
    i32 n = 3, vb = 0;
    fun_140225820(&vb);
    fun_140225640(&n, &vb);
    build(7);
}

} // namespace

TEST_CASE(cube_build_and_attributes)
{
    setup();
    REQUIRE(def::cmd::cube::g_cube_used == 1);
    i32 obj = 7, inst = 1;
    // connectivity and coordinates
    i32 poly = 3, conn[4] = {0, 0, 0, 0}, cnt = -1;
    fun_140227e00(&obj, &inst, &poly, conn, &cnt);
    CHECK_EQ(cnt, 4);
    for (i32 k = 0; k < 4; ++k) {
        CHECK_EQ(conn[k], g_box.f[(poly - 1) * 4 + k]);
    }
    i32 pt = 5;
    double xyz[3];
    fun_140227fc0(&obj, &inst, &pt, xyz);
    CHECK_EQ(xyz[0], g_box.x[12]);
    CHECK_EQ(xyz[2], g_box.x[14]);
    // the cube aliases the coordinates: a later move is seen
    g_box.x[12] += 0.25;
    fun_140227fc0(&obj, &inst, &pt, xyz);
    CHECK_EQ(xyz[0], g_box.x[12]);
    g_box.x[12] -= 0.25;
    // NI: init, replace with another count, get, missing field
    i32 ent = 4, f1 = 1, f2 = 2, n3 = 3, n5 = 5;
    i32 a[5] = {11, 12, 13, 14, 15};
    fun_140228690(&obj, &inst, &ent, &f1, a, &n3);
    fun_140228690(&obj, &inst, &ent, &f2, a + 1, &n3);
    fun_140228690(&obj, &inst, &ent, &f1, a, &n5);
    i32 got[8] = {0}, gc = -1;
    fun_140228830(&obj, &inst, &ent, &f1, got, &gc);
    CHECK_EQ(gc, 5);
    CHECK_EQ(got[4], 15);
    fun_140228830(&obj, &inst, &ent, &f2, got, &gc);
    CHECK_EQ(gc, 3);
    CHECK_EQ(got[0], 12);
    i32 f9 = 9;
    got[0] = 77;
    fun_140228830(&obj, &inst, &ent, &f9, got, &gc);
    CHECK_EQ(gc, 0);
    CHECK_EQ(got[0], 77);
    // NR, EI, ER
    double r[3] = {0.5, -1.25, 3.0}, rg[3] = {0, 0, 0};
    fun_140228980(&obj, &inst, &ent, &f1, r, &n3);
    fun_140228b10(&obj, &inst, &ent, &f1, rg, &gc);
    CHECK_EQ(gc, 3);
    CHECK_EQ(rg[1], -1.25);
    i32 e = 24, one = 1, v1 = -1000;
    fun_1402280d0(&obj, &inst, &e, &f2, &v1, &one);
    i32 vg = 0;
    fun_140228260(&obj, &inst, &e, &f2, &vg, &gc);
    CHECK_EQ(gc, 1);
    CHECK_EQ(vg, -1000);
    double ar = 0.25, arg = 0;
    fun_1402283b0(&obj, &inst, &e, &f2, &ar, &one);
    fun_140228540(&obj, &inst, &e, &f2, &arg, &gc);
    CHECK_EQ(arg, 0.25);
    // a missing cube gives count 0
    i32 other = 8;
    fun_140228830(&other, &inst, &ent, &f1, got, &gc);
    CHECK_EQ(gc, 0);
}

TEST_CASE(cube_box_query_unique_list)
{
    setup();
    i32 obj = 7, inst = 1, n = -1, exact = 0, vb = 0;
    double box[6] = {-0.1, -0.1, -0.1, 0.1, 0.1, 0.1}; // around the corner (0,0,0)
    fun_140228c60(&obj, &inst, box, &n, &exact, &vb);
    CHECK(n >= 3);
    std::vector<i32> list(64, 0);
    std::uint32_t nu = 0;
    fun_14022a5c0(&obj, &inst, list.data(), &nu, &vb);
    // the three quads at the corner (faces x = 0, y = 0, z = 0, first quad of each) are in the list
    std::vector<i32> want;
    for (i32 p = 1; p <= g_box.nf; ++p) {
        for (i32 k = 0; k < 4; ++k) {
            const i32 q = g_box.f[(p - 1) * 4 + k];
            if (g_box.x[(q - 1) * 3] == 0 && g_box.x[(q - 1) * 3 + 1] == 0 && g_box.x[(q - 1) * 3 + 2] == 0) {
                want.push_back(p);
            }
        }
    }
    CHECK_EQ(static_cast<i32>(want.size()), 3);
    for (std::uint32_t k = 1; k < nu; ++k) {
        CHECK(list[k - 1] < list[k]); // unique ascending
    }
    for (i32 w : want) {
        bool in = false;
        for (std::uint32_t k = 0; k < nu; ++k) {
            in = in || list[k] == w;
        }
        CHECK(in);
    }
    // exact query (1404040c0 + pcube): a subset of the plain one, still holding the corner quads
    i32 n2 = -1;
    exact = 1;
    fun_140228c60(&obj, &inst, box, &n2, &exact, &vb);
    CHECK(n2 <= n);
    std::uint32_t nu2 = 0;
    std::vector<i32> list2(64, 0);
    fun_14022a5c0(&obj, &inst, list2.data(), &nu2, &vb);
    CHECK(nu2 >= 3 && nu2 <= nu);
    // a box outside the cube: nothing
    double far[6] = {5, 5, 5, 6, 6, 6};
    exact = 0;
    fun_140228c60(&obj, &inst, far, &n, &exact, &vb);
    CHECK_EQ(n, 0);
}

TEST_CASE(cube_nearest_point_and_radius)
{
    setup();
    i32 obj = 7, inst = 1, vb = 0;
    // shell search (26 points >= 16) against brute force, points inside the bounds
    const double probes[][3] = {{0.1, 0.2, 0.05}, {0.9, 0.45, 0.6}, {0.47, 0.5, 0.52}, {0.3, 0.97, 0.71}, {0.02, 0.98, 0.49}};
    for (const auto& p : probes) {
        double pp[3] = {p[0], p[1], p[2]};
        double d = -1;
        std::uint32_t pt = 0;
        fun_14022d770(&obj, &inst, pp, &d, &pt, &vb);
        i32 best = 1;
        double bd = fun_140406540(pp, &g_box.x[0]);
        for (i32 i = 0; i < g_box.np; ++i) {
            const double di = fun_140406540(pp, &g_box.x[i * 3]);
            if (di < bd) {
                bd = di;
                best = i + 1;
            }
        }
        CHECK_EQ(static_cast<i32>(pt), best);
        CHECK_EQ(d, std::sqrt(bd));
        double rad = -1;
        fun_14022fcd0(&obj, &inst, pp, &rad, &vb);
        CHECK(rad > 0.0 && rad <= std::sqrt(3.0) + 1e-12);
    }
    double h[3];
    fun_140232090(&obj, &inst, h, &vb);
    CHECK(h[0] > 0 && h[1] > 0 && h[2] > 0);
    // brute force path: a cube of 8 points (one hexahedron surface)
    static double x8[24] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1};
    static i32 f8[24] = {1, 4, 3, 2, 5, 6, 7, 8, 1, 2, 6, 5, 2, 3, 7, 6, 3, 4, 8, 7, 4, 1, 5, 8};
    i32 o2 = 9, type = 3, np = 8, nf = 6, nv = 4, base = 1;
    fun_140225ff0(&o2, &inst, &type, &np, x8, &nf, &nv, f8, &base, &vb);
    double q[3] = {1.4, 0.9, -0.3};
    double d = 0;
    std::uint32_t pt = 0;
    fun_14022d770(&o2, &inst, q, &d, &pt, &vb);
    CHECK_EQ(static_cast<i32>(pt), 3);
    // clean the entity, rebuild with the attribute lists kept (1402260a0 needs an existing record)
    fun_140225cd0(&o2, &inst, &vb);
    i32 n2 = 3;
    fun_140225640(&n2, &vb);
    build(7);
    fun_1402260a0(&obj, &inst, &type, &g_box.np, g_box.x.data(), &g_box.nf, &nv, g_box.f.data(), &base, &vb);
    CHECK_EQ(def::cmd::cube::g_cube_used, 1);
}

int main() { return testkit::run_all(); }
