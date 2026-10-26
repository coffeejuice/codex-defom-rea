// ctest-labels: p67-7 ctc3d
// Tier-1 tests of the master surface helpers (group "surface"): the steps of 14004f56b after the
// cube is created (ER1/ER2, NI3/EI2, NI1, half-edges, patches, edge table, ER3, ININDP, folding,
// curves) on a unit cube surface (8 nodes, 6 quads with outward normals), checked against values
// derived by hand; plus the pure helpers 14007ea01, 14007e28d, 14005a6bf, 14006f089.
//
// The whole build runs through 14004f56b on the real cube library (cube table 140225640, attribute
// store and getters of group "cube", search kernels of group "search").
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <testkit/check.hpp>

#include <cmath>
#include <cstring>
#include <map>
#include <tuple>
#include <vector>


using namespace def::cmd::ctc3d;

namespace {

std::vector<std::vector<i32>> g_faces;  // 1-based
std::vector<std::vector<double>> g_pts; // 1-based
double g_x[24];                         // x(3, 8), aliased by the cube
i32 g_conn[24];                         // conn(4, 6), aliased by the cube

i32 g_obj = 1, g_inst = 1;

// kind 0 NI, 1 NR, 2 EI, 3 ER: the values of field fld of entity ent through the real getters
template <class T>
std::vector<T> field(int kind, int ent, int fld)
{
    T buf[64] = {};
    i32 e = ent, f = fld, c = 0;
    switch (kind) {
    case 0: fun_140228830(&g_obj, &g_inst, &e, &f, buf, &c); break;
    case 1: fun_140228b10(&g_obj, &g_inst, &e, &f, buf, &c); break;
    case 2: fun_140228260(&g_obj, &g_inst, &e, &f, buf, &c); break;
    default: fun_140228540(&g_obj, &g_inst, &e, &f, buf, &c); break;
    }
    return std::vector<T>(buf, buf + (c > 0 ? c : 0));
}

bool near(double a, double b) { return std::fabs(a - b) < 1e-12; }

// Unit cube: node n-1 = bits (x, y, z); quads counter-clockwise seen from outside.
void setup_cube()
{
    reset_image();
    dat_14077a7bc = 1;
    g_pts = {{}};
    for (int n = 0; n < 8; ++n) {
        g_pts.push_back({double(n & 1), double((n >> 1) & 1), double((n >> 2) & 1)});
    }
    g_faces = {{},           {1, 3, 4, 2}, // z = 0, normal -z
               {5, 6, 8, 7},               // z = 1, +z
               {1, 2, 6, 5},               // y = 0, -y
               {3, 7, 8, 4},               // y = 1, +y
               {1, 5, 7, 3},               // x = 0, -x
               {2, 4, 8, 6}};              // x = 1, +x
    for (int n = 1; n <= 8; ++n) {
        for (int c = 0; c < 3; ++c) {
            g_x[3 * (n - 1) + c] = g_pts[n][c];
        }
    }
    for (int p = 1; p <= 6; ++p) {
        for (int k = 0; k < 4; ++k) {
            g_conn[4 * (p - 1) + k] = g_faces[p][k];
        }
    }
}

// 14004f56b on a fresh cube table
void build(i32& flag)
{
    i32 zero = 0, three = 3, npts = 8, npoly = 6, nv = 4, base = 1;
    fun_140225820(&zero);
    fun_140225640(&three, &zero);
    flag = 0;
    fun_14004f56b(&g_obj, &g_inst, g_x, g_conn, &nv, &base, &npts, &npoly, &zero, &flag);
}

} // namespace

TEST_CASE(unit_cube_surface)
{
    setup_cube();
    i32 flag = -1;
    build(flag);
    CHECK_EQ(flag, 0);
    const double nrm[7][3] = {{}, {0, 0, -1}, {0, 0, 1}, {0, -1, 0}, {0, 1, 0}, {-1, 0, 0}, {1, 0, 0}};
    for (int p = 1; p <= 6; ++p) {
        auto n = field<double>(3, p, 1);
        REQUIRE(n.size() == 3);
        for (int c = 0; c < 3; ++c) {
            CHECK(near(n[c], nrm[p][c]));
        }
        auto a = field<double>(3, p, 2);
        REQUIRE(a.size() == 1);
        CHECK(near(a[0], 1.0));
        CHECK_EQ(field<i32>(2, p, 2).at(0), 1); // EI2
        CHECK_EQ(field<i32>(2, p, 4).at(0), 1); // EI4 patch
        auto ei1 = field<i32>(2, p, 1);
        REQUIRE(ei1.size() == 8);
        for (int k = 0; k < 4; ++k) {
            // neighbour across edge k shares the nodes conn(k), conn(k+1) in reverse
            const i32 q = ei1[k];
            REQUIRE(q >= 1 && q <= 6 && q != p);
            const i32 a1 = g_faces[p][k], a2 = g_faces[p][(k + 1) % 4];
            bool rev = false;
            for (int j = 0; j < 4; ++j) {
                rev |= g_faces[q][j] == a2 && g_faces[q][(j + 1) % 4] == a1;
            }
            CHECK(rev);
            CHECK_EQ(ei1[4 + k], 2); // convex edge, ER3 = 3 pi / 2 >= 4.0122 -> class 2
        }
        auto er3 = field<double>(3, p, 3);
        REQUIRE(er3.size() == 4);
        for (int k = 0; k < 4; ++k) {
            // edge frame: (va x vb) . u = -1 for a convex edge of outward quads -> -pi/2 + 2 pi
            CHECK(std::fabs(er3[k] - 3.0 * std::acos(0.0)) < 1e-12);
        }
    }
    const double r3 = 1.0 / std::sqrt(3.0);
    for (int n = 1; n <= 8; ++n) {
        CHECK_EQ(field<i32>(0, n, 3).at(0), 1); // NI3
        auto ni1 = field<i32>(0, n, 1);
        REQUIRE(ni1.size() == 3);
        CHECK(ni1[0] < ni1[1] && ni1[1] < ni1[2]);
        CHECK_EQ(field<i32>(0, n, 2).size(), std::size_t(6)); // 3 edge + 3 face-diagonal nodes
        auto nr2 = field<double>(1, n, 2);
        CHECK(near(nr2.at(0), 0.75)); // 3 quads * 1 / 4
        auto nr1 = field<double>(1, n, 1);
        REQUIRE(nr1.size() == 4);
        const auto& x = g_pts[n];
        for (int c = 0; c < 3; ++c) {
            CHECK(std::fabs(nr1[c] - (x[c] > 0.5 ? r3 : -r3)) < 1e-12);
        }
        CHECK_EQ(nr1[3], 3.0); // three edges with ER3 > 4.398, none < 1.885
        auto nr5 = field<double>(1, n, 5);
        REQUIRE(nr5.size() == 2);
        CHECK(std::fabs(nr5[0] - std::sqrt(2.0 / 3.0)) < 1e-12); // face diagonal: 2 / sqrt(6)
        CHECK(std::fabs(nr5[1] - r3) < 1e-12);                    // edge neighbour
    }
}

TEST_CASE(angle_14007ea01)
{
    reset_image();
    double a = -1;
    const double p1[2] = {1, 0}, p2[2] = {0, 1}, p3[2] = {0, -1}, z[2] = {0, 0};
    fun_14007ea01(p1, p2, &a);
    CHECK_EQ(a, std::acos(0.0));
    fun_14007ea01(p1, p3, &a);
    CHECK_EQ(a, 0x1.921fb54442d28p+1 * 2.0 + -std::acos(0.0)); // 2 * 3.1415926535898 - pi/2
    fun_14007ea01(p1, p1, &a); // atan2(0, 1) = 0 -> 2 pi
    CHECK_EQ(a, 0x1.921fb54442d28p+1 * 2.0);
    fun_14007ea01(z, p1, &a);
    CHECK_EQ(a, 0.0);
}

TEST_CASE(bins_14007e28d)
{
    // 2 nodes, bins of m = 4 (3 entries + link): node 1 gets 7 values -> chained columns 3, 4
    i32 m = 4, nbin = 2, node = 1, get = 0;
    std::vector<i32> bins(4 * 6, 0), cnt(2, 0), out(16, 0);
    for (i32 v = 10; v < 17; ++v) {
        fun_14007e28d(&nbin, bins.data(), cnt.data(), &node, &v, &get, out.data(), &m);
    }
    CHECK_EQ(cnt[0], 7);
    CHECK_EQ(nbin, 4);
    get = 1;
    i32 dummy = 0;
    fun_14007e28d(&nbin, bins.data(), cnt.data(), &node, &dummy, &get, out.data(), &m);
    for (int i = 0; i < 7; ++i) {
        CHECK_EQ(out[i], 10 + i);
    }
}

TEST_CASE(lists_14005a6bf_14006f089)
{
    reset_image();
    i32 l[7] = {3, 1, 3, 0, 2, 1, 5};
    i32 n = 7;
    fun_14005a6bf(l, &n);
    CHECK_EQ(n, 4);
    CHECK_EQ(l[0], 3);
    CHECK_EQ(l[1], 1);
    CHECK_EQ(l[2], 2);
    CHECK_EQ(l[3], 5);
    const i32 a[5] = {4, 2, 9, 2, 7}, b[4] = {7, 2, 0, 4};
    i32 na = 5, nb = 4, no = -1, out[5] = {};
    fun_14006f089(&na, a, &nb, b, &no, out);
    CHECK_EQ(no, 3);
    CHECK_EQ(out[0], 4);
    CHECK_EQ(out[1], 2);
    CHECK_EQ(out[2], 7);
}

int main() { return testkit::run_all(); }
