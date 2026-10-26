// ctest-labels: p67-2 cube
// Tier-1 tests of the cfnCUBE bucket grid port (src/ifd/cube/). Expected values are worked out
// by hand from the decompiled code (docs/re/commands/OBJPOS_interference_kernels.md section 2).
// Whole-cube tests use the real CRT pow (def::rt::libm::crt_pow, 140483f60).
#include <defcmd/ifd/cube_lib.hpp>

#include <defcmd/ifd/cube.hpp>
#include <defrt/stdlib/libm.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdlib>
#include <set>
#include <vector>

using namespace def::cmd::cube;

namespace {


// Locator of `type` over the points (a hand-built point set), n divisions per axis.
struct HandLocator {
    PointSet ps{};
    Locator loc{};
    std::vector<double> xyz;
    HandLocator(std::vector<double> pts, std::int32_t nx, std::int32_t ny, std::int32_t nz)
        : xyz(std::move(pts))
    {
        fun_140405c80(&ps, 0, 1000);
        ps.count = ps.cap = static_cast<std::int32_t>(xyz.size() / 3);
        ps.data = xyz.data();
        fun_1403fed00(&loc, 1, static_cast<double>(1e-6f), 1);
        fun_1403ff890(&loc, nx, ny, nz);
        fun_1403ff970(&loc, &ps, fun_140406510(&ps), 0);
    }
    ~HandLocator() { fun_1403feec0(&loc); }
    std::int32_t cell_count(std::int32_t ix, std::int32_t iy, std::int32_t iz) const
    {
        return loc.cells_poly[ix + iy * loc.n[0] + iz * loc.n[0] * loc.n[1]].count;
    }
    std::int32_t total() const
    {
        std::int32_t s = 0;
        for (std::int32_t k = 0; k < loc.ncells; ++k) s += loc.cells_poly[k].count;
        return s;
    }
};

// The die of ik_m102_bucket_3d: 3x3x3 hex block, x 0..30, y 0..36, z at zs[0..3]; its 54
// boundary quads, bottom tiles first (id 1 + i + 3j, middle tile = 5), then top, x=0, x=30,
// y=0, y=36.
struct Die {
    std::vector<double> pts;
    std::vector<std::int32_t> polys;
    explicit Die(const double* zs)
    {
        const double xs[4] = {0, 10, 20, 30};
        const double ys[4] = {0, 12, 24, 36};
        for (int k = 0; k < 4; ++k)
            for (int j = 0; j < 4; ++j)
                for (int i = 0; i < 4; ++i) {
                    pts.push_back(xs[i]);
                    pts.push_back(ys[j]);
                    pts.push_back(zs[k]);
                }
        auto id = [](int i, int j, int k) { return 1 + i + 4 * j + 16 * k; };
        auto quad = [&](int a, int b, int c, int d) {
            polys.push_back(a);
            polys.push_back(b);
            polys.push_back(c);
            polys.push_back(d);
        };
        for (int k : {0, 3})
            for (int j = 0; j < 3; ++j)
                for (int i = 0; i < 3; ++i) quad(id(i, j, k), id(i + 1, j, k), id(i + 1, j + 1, k), id(i, j + 1, k));
        for (int i : {0, 3})
            for (int k = 0; k < 3; ++k)
                for (int j = 0; j < 3; ++j) quad(id(i, j, k), id(i, j + 1, k), id(i, j + 1, k + 1), id(i, j, k + 1));
        for (int j : {0, 3})
            for (int k = 0; k < 3; ++k)
                for (int i = 0; i < 3; ++i) quad(id(i, j, k), id(i + 1, j, k), id(i + 1, j, k + 1), id(i, j, k + 1));
    }
    std::int32_t npts() const { return static_cast<std::int32_t>(pts.size() / 3); }
    std::int32_t npoly() const { return static_cast<std::int32_t>(polys.size() / 4); }
};

void build_die(Die& d)
{
    std::int32_t one = 1, zero = 0, four = 4;
    std::int32_t n = 1;
    fun_140225640(&n, &zero);
    std::int32_t np = d.npts(), nf = d.npoly();
    fun_140225ff0(&one, &one, &one, &np, d.pts.data(), &nf, &four, d.polys.data(), &one, &zero);
}

std::vector<std::int32_t> query(double x0, double y0, double z0, double x1, double y1, double z1)
{
    double box[6] = {x0, y0, z0, x1, y1, z1};
    std::int32_t one = 1, zero = 0, nc = -1;
    fun_140228c60(&one, &one, box, &nc, &zero, &zero);
    std::vector<std::int32_t> list(static_cast<std::size_t>(nc > 0 ? nc : 0) + 1, -7);
    fun_140229f30(&one, &one, list.data(), &zero);
    list.resize(static_cast<std::size_t>(nc));
    return list;
}

void clean_all()
{
    std::int32_t zero = 0;
    fun_140225820(&zero);
}

} // namespace

TEST_CASE(division_count_thresholds)
{
    CHECK_EQ(division_count(0), 5);
    CHECK_EQ(division_count(9), 5);
    CHECK_EQ(division_count(10), 10);
    CHECK_EQ(division_count(54), 10);
    CHECK_EQ(division_count(99), 10);
    CHECK_EQ(division_count(100), 20);
    CHECK_EQ(division_count(999), 20);
    CHECK_EQ(division_count(1000), 20);
    CHECK_EQ(division_count(1999), 23);   // (int)(0.0033 * 999 = 3.2967) + 20
    CHECK_EQ(division_count(9999), 49);   // (int)(0.0033 * 8999 = 29.6967) + 20
    CHECK_EQ(division_count(10000), 50);
    CHECK_EQ(division_count(11999), 50);  // (int)(1999 * 0.0005 = 0.9995) + 50
    CHECK_EQ(division_count(30000), 60);  // (int)(20000 * 0.0005 = 10) + 50
}

TEST_CASE(divisions_clamp_and_limit)
{
    std::int32_t n[3];
    double sp = 0.0;
    // die of the bucket test: a = (30, 36, 40), bc = 10, pow(43200, 0.3333333) = 35.088...
    const double a[3] = {30.0, 36.0, 40.0};
    divisions(a, 10, 54, def::rt::libm::crt_pow(43200.0, 0.3333333), n, &sp);
    CHECK_EQ(n[0], 8);  // 30 / 3.5088 = 8.55
    CHECK_EQ(n[1], 10); // 10.26
    CHECK_EQ(n[2], 11); // 11.40
    // n < 6 -> 5, then min(n, nPoly)
    const double b[3] = {1.0, 5.9, 100.0};
    divisions(b, 5, 1000, 5.0, n, &sp); // sp = 1
    CHECK_BITS(sp, 1.0);
    CHECK_EQ(n[0], 5);
    CHECK_EQ(n[1], 5);
    CHECK_EQ(n[2], 100);
    divisions(b, 5, 3, 5.0, n, &sp);
    CHECK_EQ(n[0], 3);
    CHECK_EQ(n[2], 3);
    // bc >= 50: decrement all three until (nx ny nz) / bc^3 (int division) <= 1.1
    const double c[3] = {300.0, 300.0, 300.0};
    divisions(c, 50, 100000, 50.0, n, &sp); // 300^3 / 125000 = 216
    CHECK_EQ(n[0], 62);                     // 62^3 = 238328 -> 1; 63^3 = 250047 -> 2
    CHECK_EQ(n[1], 62);
    CHECK_EQ(n[2], 62);
    divisions(c, 20, 100000, 20.0, n, &sp); // bc < 50: no limit loop
    CHECK_EQ(n[0], 300);
}

TEST_CASE(locator_division_clamp)
{
    Locator l{};
    fun_1403ff890(&l, 0, 150, 7);
    CHECK_EQ(l.n[0], 1);
    CHECK_EQ(l.n[1], 100);
    CHECK_EQ(l.n[2], 7);
    fun_1403ff890(&l, -3, 100, 1);
    CHECK_EQ(l.n[0], 1);
    CHECK_EQ(l.n[1], 100);
    CHECK_EQ(l.n[2], 1);
}

TEST_CASE(locator_box_and_cell_size)
{
    // flat point set in z (extent 0 -> hi = lo + 1), y extent 0.0005 (< 0.001 -> hi + 0.001)
    HandLocator h({0.0, 0.0, 2.0, 4.0, 0.0005, 2.0}, 4, 2, 5);
    CHECK_BITS(h.loc.box[0], 0.0);
    CHECK_BITS(h.loc.box[1], 4.0);
    CHECK_BITS(h.loc.box[3], 0.0005 + 0.001);
    CHECK_BITS(h.loc.box[4], 2.0);
    CHECK_BITS(h.loc.box[5], 3.0);
    CHECK_BITS(h.loc.h[0], 1.0);
    CHECK_BITS(h.loc.h[1], (0.0005 + 0.001) / 2.0);
    CHECK_BITS(h.loc.h[2], 0.2);
    CHECK_EQ(h.loc.ncells, 40);
    CHECK_EQ(h.loc.offset, 1);
    CHECK_BITS(h.loc.tol, static_cast<double>(1e-6f));
    // insertion level = min(ceil(tol / min H), max n) = ceil(1e-6 / 0.00075) = 1
    CHECK_EQ(h.loc.ins_level, 1);
    CHECK(h.loc.cells_poly != nullptr);
    CHECK(h.loc.cells_a8 == nullptr);
    CHECK_EQ(h.loc.cells_poly[0].inc, 5);
    CHECK_EQ(h.loc.cells_poly[0].width, 1);
}

TEST_CASE(box_overlap_inclusive)
{
    const double b[6] = {0, 0, 0, 1, 1, 1};
    const double in[6] = {0.2, 0.2, 0.2, 0.3, 0.3, 0.3};
    const double around[6] = {-1, -1, -1, 2, 2, 2};
    const double touch[6] = {1, 0.5, 0.5, 2, 0.6, 0.6};
    const double apart_x[6] = {1.01, 0.5, 0.5, 2, 0.6, 0.6};
    const double apart_z[6] = {0.5, 0.5, -3, 0.6, 0.6, -0.0001};
    const double point[6] = {0.5, 0.5, 1, 0.5, 0.5, 1};
    CHECK_EQ(fun_140240b40(in, b), 1);
    CHECK_EQ(fun_140240b40(around, b), 1);
    CHECK_EQ(fun_140240b40(touch, b), 1);
    CHECK_EQ(fun_140240b40(apart_x, b), 0);
    CHECK_EQ(fun_140240b40(apart_z, b), 0);
    CHECK_EQ(fun_140240b40(point, b), 1);
    const double nan = std::nan("");
    const double nan_min[6] = {nan, 0.5, 0.5, 0.5, 0.6, 0.6}; // a.max in b still holds
    const double nan_all[6] = {nan, 0.5, 0.5, nan, 0.6, 0.6};
    CHECK_EQ(fun_140240b40(nan_min, b), 1);
    CHECK_EQ(fun_140240b40(nan_all, b), 0);
}

TEST_CASE(cell_list_grow_and_reset)
{
    CellList c{};
    fun_1404050b0(&c, 0, 5, 1);
    CHECK(c.data == nullptr);
    for (std::int32_t i = 0; i < 12; ++i) fun_140405540(&c, 1, &i);
    CHECK_EQ(c.count, 12);
    CHECK_EQ(c.cap, 15); // 0 -> 5 -> 10 -> 15
    CHECK_EQ(c.data[11], 11);
    CHECK_EQ(*fun_140405720(&c, 3), 2);
    fun_140405150(&c);
    CHECK_EQ(c.count, 0);
    CHECK_EQ(c.cap, 0);
    CHECK(c.data == nullptr);
    CHECK_EQ(c.inc, 5);
}

TEST_CASE(insert_without_pcube_fills_index_box)
{
    // box [0,4]^3, H = 1; quad at z = 0.5 over x 0.2..2.8, y 0.2..1.5: spans 2 / 1 / 0 -> no pcube
    HandLocator h({0, 0, 0, 4, 4, 4, 0.2, 0.2, 0.5, 2.8, 0.2, 0.5, 2.8, 1.5, 0.5, 0.2, 1.5, 0.5}, 4, 4, 4);
    const std::int32_t nodes[4] = {3, 4, -5, 6}; // abs() of the node id; offset 1
    fun_140401050(&h.loc, nodes, 4, 7, 0);
    CHECK_EQ(h.total(), 6);
    for (int ix = 0; ix <= 2; ++ix)
        for (int iy = 0; iy <= 1; ++iy) {
            CHECK_EQ(h.cell_count(ix, iy, 0), 1);
            CHECK_EQ(h.loc.cells_poly[ix + iy * 4].data[0], 7);
            CHECK_EQ(h.loc.cells_poly[ix + iy * 4].cap, 10); // re-initialised with 10 / 50
            CHECK_EQ(h.loc.cells_poly[ix + iy * 4].inc, 50);
        }
}

TEST_CASE(insert_at_max_uses_0999)
{
    // a point exactly at hi gets index n - 1 (0.999 branch), not n
    HandLocator h({0, 0, 0, 4, 4, 4}, 4, 4, 4);
    const std::int32_t nodes[4] = {2, 2, 2, 2};
    fun_140401050(&h.loc, nodes, 4, 1, 0);
    CHECK_EQ(h.total(), 1);
    CHECK_EQ(h.cell_count(3, 3, 3), 1);
}

TEST_CASE(insert_with_pcube_diagonal_plane)
{
    // box [0,6]^3, H = 1; quad in the plane z = x across the whole box: index box 6x6x6, so the
    // cells are tested with pcube (cell scaled by 1.01): exactly the cells with |ix - iz| <= 1
    HandLocator h({0, 0, 0, 6, 0, 6, 6, 6, 6, 0, 6, 0}, 6, 6, 6);
    const std::int32_t nodes[4] = {1, 2, 3, 4};
    fun_140401050(&h.loc, nodes, 4, 3, 0);
    std::int32_t bad = 0;
    for (int ix = 0; ix < 6; ++ix)
        for (int iy = 0; iy < 6; ++iy)
            for (int iz = 0; iz < 6; ++iz) {
                const int want = std::abs(ix - iz) <= 1 ? 1 : 0;
                if (h.cell_count(ix, iy, iz) != want) ++bad;
            }
    CHECK_EQ(bad, 0);
    CHECK_EQ(h.total(), 6 * 16);
    // the same quad reversed gives the same cells (winding number +1 either way)
    HandLocator r({0, 0, 0, 6, 0, 6, 6, 6, 6, 0, 6, 0}, 6, 6, 6);
    const std::int32_t rev[4] = {4, 3, 2, 1};
    fun_140401050(&r.loc, rev, 4, 3, 0);
    CHECK_EQ(r.total(), 6 * 16);
}

TEST_CASE(pcube_basic_cases)
{
    // segment through the centre / beside the cube
    const double a0[3] = {-2, 0, 0}, a1[3] = {2, 0, 0};
    const double b0[3] = {-2, 0.7, 0}, b1[3] = {2, 0.7, 0};
    CHECK_EQ(fun_140453c40(a0, a1), 1);
    CHECK_EQ(fun_140453c40(b0, b1), 0);
    // small triangle with a vertex inside: trivial accept
    const double t_in[9] = {0, 0, 0, 3, 0, 0, 0, 3, 0};
    double n[3];
    fun_140454110(n, 3, t_in);
    CHECK_EQ(fun_1404537f0(3, t_in, n, 0, 0), 1);
    // large triangle in z = 0.4, all vertices and edges outside: plane test, winding 1
    const double t_big[9] = {-10, -10, 0.4, 10, -10, 0.4, 0, 20, 0.4};
    fun_140454110(n, 3, t_big);
    CHECK_EQ(fun_140453010(3, t_big, 0), -1);
    CHECK_EQ(fun_1404537f0(3, t_big, n, 0, 0), 1);
    const double t_rev[9] = {0, 20, 0.4, 10, -10, 0.4, -10, -10, 0.4};
    fun_140454110(n, 3, t_rev);
    CHECK_EQ(fun_1404537f0(3, t_rev, n, 0, 0), 1);
    // the same at z = 0.6: every vertex above the cube -> trivial reject
    const double t_up[9] = {-10, -10, 0.6, 10, -10, 0.6, 0, 20, 0.6};
    fun_140454110(n, 3, t_up);
    CHECK_EQ(fun_1404537f0(3, t_up, n, 0, 0), 0);
    // plane z = 0.4 but the triangle beside the cube (x 2..20): edge bevel / winding 0
    const double t_side[9] = {2, -10, 0.4, 20, -10, 0.4, 2, 20, 0.4};
    fun_140454110(n, 3, t_side);
    CHECK_EQ(fun_1404537f0(3, t_side, n, 0, 0), 0);
    // unnormalised normal of a unit right triangle in z = 0
    const double t_n[9] = {0, 0, 0, 1, 0, 0, 0, 1, 0};
    fun_140454110(n, 3, t_n);
    CHECK_BITS(n[0], 0.0);
    CHECK_BITS(n[1], 0.0); // 0 + -(1*0 + 0*(-0)) = 0 + -0 = +0
    CHECK_BITS(n[2], 1.0);
}

TEST_CASE(create_build_clean_lifecycle)
{
    std::int32_t three = 3, zero = 0;
    fun_140225640(&three, &zero);
    CHECK_EQ(g_cube_size, 3);
    CHECK_EQ(g_cube_used, 0);
    CHECK_EQ(g_cubes[2].id, -1);
    CHECK_EQ(g_cubes[2].sub, -1);
    std::vector<double> pts = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1};
    std::vector<std::int32_t> polys = {1, 2, 3, 4, 1, 2, 5, 1};
    std::int32_t id = 4, sub = 2, type = 1, np = 5, nf = 2, npp = 4, off = 1;
    fun_140225ff0(&id, &sub, &type, &np, pts.data(), &nf, &npp, polys.data(), &off, &zero);
    CHECK_EQ(g_cube_used, 1);
    CHECK_EQ(g_cubes[0].id, 4);
    CHECK_EQ(g_cubes[0].sub, 2);
    CHECK(g_cubes[0].points.data == pts.data());
    CHECK(g_cubes[0].polys.data == polys.data());
    CHECK_EQ(g_cubes[0].a180.count, 5);
    CHECK_EQ(g_cubes[0].a190.count, 2);
    // 2 polygons: bc = 5; a = (1, 1, 1): sp = pow(1, ..) / 5 = 0.2, n = 5 -> min(5, nPoly) = 2
    CHECK_EQ(g_cubes[0].loc.n[0], 2);
    CHECK_EQ(g_cubes[0].loc.n[2], 2);
    // rebuilding the same (id, sub) reuses the slot
    fun_140225ff0(&id, &sub, &type, &np, pts.data(), &nf, &npp, polys.data(), &off, &zero);
    CHECK_EQ(g_cube_used, 1);
    id = 5;
    fun_140225ff0(&id, &sub, &type, &np, pts.data(), &nf, &npp, polys.data(), &off, &zero);
    CHECK_EQ(g_cube_used, 2);
    CHECK_EQ(g_cubes[1].id, 5);
    // create again: cleans the table first
    std::int32_t one = 1;
    fun_140225640(&one, &zero);
    CHECK_EQ(g_cube_size, 1);
    CHECK_EQ(g_cube_used, 0);
    clean_all();
    CHECK(g_cubes == nullptr);
    CHECK_EQ(g_cube_size, 0);
    CHECK_EQ(g_cube_used, 0);
}

TEST_CASE(result_list_sorted_with_duplicates)
{
    std::int32_t one = 1, zero = 0;
    fun_140225640(&one, &zero);
    std::vector<double> pts = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<std::int32_t> polys = {1, 2, 3, 4};
    std::int32_t type = 1, np = 4, nf = 1, npp = 4;
    fun_140225ff0(&one, &one, &type, &np, pts.data(), &nf, &npp, polys.data(), &one, &zero);
    const std::int32_t vals[6] = {5, 3, 9, 3, 1, -2};
    for (const std::int32_t v : vals) fun_140405540(&g_cubes[0].result, 1, &v);
    std::int32_t list[6] = {};
    fun_140229f30(&one, &one, list, &zero);
    const std::int32_t want[6] = {-2, 1, 3, 3, 5, 9};
    for (int i = 0; i < 6; ++i) CHECK_EQ(list[i], want[i]);
    CHECK_EQ(fun_140231d00(&vals[0], &vals[1]), 1);
    CHECK_EQ(fun_140231d00(&vals[1], &vals[3]), 0);
    CHECK_EQ(fun_140231d00(&vals[4], &vals[0]), -1);
    clean_all();
}

TEST_CASE(bucket_test_die_lowered_5)
{
    // ik_m102_bucket_3d: die z 10..50 -> grid 8 x 10 x 11 (spec section 7)
    const double zs[4] = {10, 20, 35, 50};
    Die d(zs);
    REQUIRE(d.npoly() == 54);
    build_die(d);
    const Locator& l = g_cubes[0].loc;
    CHECK_EQ(l.n[0], 8);
    CHECK_EQ(l.n[1], 10);
    CHECK_EQ(l.n[2], 11);
    CHECK_BITS(l.box[4], 10.0);
    CHECK_BITS(l.box[5], 50.0);
    // a top node of the workpiece at z = 15 (cell iz = 1): the die bottom (cell 0) is not found
    CHECK_EQ(query(15, 18, 15, 15, 18, 15).size(), 0u);
    // a side node at z = 11 (cell 0): the middle bottom tile (id 5) only
    const std::vector<std::int32_t> r = query(15, 18, 11, 15, 18, 11);
    REQUIRE(r.size() == 1u);
    CHECK_EQ(r[0], 5);
    // negative dcur: corners are min/max-ed, z 10..15 covers cells 0 and 1
    const std::vector<std::int32_t> r2 = query(15, 18, 15, 15, 18, 10);
    REQUIRE(r2.size() == 1u);
    CHECK_EQ(r2[0], 5);
    // outside the box in x: no candidates
    CHECK_EQ(query(31, 18, 11, 31, 18, 11).size(), 0u);
    // query at the box maximum (0.999 branch, index n - 1) does not leave the grid
    const std::vector<std::int32_t> r3 = query(30, 36, 50, 30, 36, 50);
    CHECK(r3.size() >= 1u);
    // a whole layer: sorted ascending, duplicates kept, all 9 bottom tiles present
    const std::vector<std::int32_t> all = query(0.5, 0.5, 11, 29.5, 35.5, 11);
    std::int32_t expect = 0;
    for (std::int32_t ix = 0; ix < 8; ++ix)
        for (std::int32_t iy = 0; iy < 10; ++iy) expect += l.cells_poly[ix + iy * 8].count;
    CHECK_EQ(static_cast<std::int32_t>(all.size()), expect);
    bool sorted = true;
    for (std::size_t i = 1; i < all.size(); ++i) sorted = sorted && all[i - 1] <= all[i];
    CHECK(sorted);
    std::set<std::int32_t> distinct(all.begin(), all.end());
    for (std::int32_t t = 1; t <= 9; ++t) CHECK(distinct.count(t) == 1);
    CHECK(distinct.size() < all.size());
    clean_all();
}

TEST_CASE(bucket_test_die_lowered_2)
{
    // ik_m102_bucket2_3d: die z 13..53; a top node at z = 15 shares cell 0 with the die bottom
    const double zs[4] = {13, 23, 38, 53};
    Die d(zs);
    build_die(d);
    CHECK_EQ(g_cubes[0].loc.n[2], 11);
    const std::vector<std::int32_t> r = query(15, 18, 15, 15, 18, 15);
    REQUIRE(r.size() == 1u);
    CHECK_EQ(r[0], 5);
    clean_all();
}

int main() { return testkit::run_all(); }
