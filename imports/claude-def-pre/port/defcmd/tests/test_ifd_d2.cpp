// ctest-labels: p67-2 ifd2d
// Tier-1 tests of the 2D interference kernel of OBJPOS modes 2/12/102/112 (src/ifd/d2/).
// Expected values are worked out by hand from the transcriptions in
// docs/re/commands/OBJPOS_interference_kernels.md section 4 (shown in the comments); where the
// value depends on the runtime's sin/cos (ps_sincos, crt_cos) the expectation is computed from
// those ported routines or checked with a tolerance and said so.
// The CRT atan/atan2 (1404865ac / 140485d6c) and the sort 1404d93ae are not ported yet; the test
// stands in for them through the interim hooks of geoutl_stubs.cpp (std::atan / std::atan2).
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defcmd/ifd/ifd2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defrt/stdlib/libm.hpp>
#include <testkit/check.hpp>

#include "../src/ifd/d2/d2.hpp"

using namespace def::cmd::ifd::d2;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

void context(double ext, double e2)
{
    reset_state();
    g_ext = ext;
    g_geoerr2 = e2;
    g_geoerr1 = e2;
}

std::uint64_t hook_ext(std::uint64_t, const std::uint64_t*, int) { return 0; }

} // namespace

// Segment (0,0)-(2,0), vel 0, dir (0,1): A = B = (0,-1). tolE 1e-10, tolG = 10 * 0.001.
TEST_CASE(segment_hit_below)
{
    context(10.0, 0.001);
    double seg[4] = {0.0, 0.0, 2.0, 0.0}, vel[4] = {0, 0, 0, 0}, dir[2] = {0.0, 1.0};
    double q[2] = {1.0, -1.0};
    // dP = (1,-1), s = (2,0): c = 0*1 - 2*(-1) = 2, d0 = c/2 = 1 > 0; a = 0 (linear);
    // b = Ay*sx = -2; t = -(c/b) = 1 > 0 -> gap 1, flag 1
    double gap = 1e10;
    std::int32_t flag = 7;
    fun_14030bba0(seg, vel, q, dir, &gap, &flag);
    CHECK_BITS(gap, 1.0);
    CHECK_EQ(flag, 1);
}

TEST_CASE(segment_back_side_and_outside)
{
    context(10.0, 0.001);
    double seg[4] = {0.0, 0.0, 2.0, 0.0}, vel[4] = {0, 0, 0, 0}, dir[2] = {0.0, 1.0};
    double gap = 5.0;
    std::int32_t flag = 7;
    double q[2] = {1.0, 1.0}; // d0 = -1 < -tolG: back side, nothing written but flag = 0
    fun_14030bba0(seg, vel, q, dir, &gap, &flag);
    CHECK_BITS(gap, 5.0);
    CHECK_EQ(flag, 0);
    double q2[2] = {3.0, -1.0}; // outside the band: c2 < 0
    fun_14030bba0(seg, vel, q2, dir, &gap, &flag);
    CHECK_BITS(gap, 5.0);
    CHECK_EQ(flag, 0);
}

TEST_CASE(segment_on_line_and_within_tolg)
{
    context(10.0, 0.001);
    double seg[4] = {0.0, 0.0, 2.0, 0.0}, vel[4] = {0, 0, 0, 0}, dir[2] = {0.0, 1.0};
    double gap = 5.0;
    std::int32_t flag = 0;
    // Q on the line: d0 = 0; b = -2 so v = (0,-1), w = vy*dPx - vx*dPy = -1 < 0 -> gap 0
    double q[2] = {1.0, 0.0};
    fun_14030bba0(seg, vel, q, dir, &gap, &flag);
    CHECK_BITS(gap, 0.0);
    CHECK_EQ(flag, 1);
    // Q 0.005 behind (d0 = -0.005 >= -tolG = -0.01): t = -((-0.01)/(-2)) < 0 -> gap 0
    gap = 5.0;
    flag = 0;
    double q2[2] = {1.0, 0.005};
    fun_14030bba0(seg, vel, q2, dir, &gap, &flag);
    CHECK_BITS(gap, 0.0);
    CHECK_EQ(flag, 1);
    // tolE >= LA and LB (dir = vel): returns at once
    double dz[2] = {0.0, 0.0};
    gap = 5.0;
    flag = 3;
    fun_14030bba0(seg, vel, q, dz, &gap, &flag);
    CHECK_BITS(gap, 5.0);
    CHECK_EQ(flag, 0);
}

TEST_CASE(arc_span_and_angle)
{
    context(10.0, 0.001);
    double a[2] = {1.0, 0.0}, b[2] = {0.0, 1.0}, ang = 9.0;
    fun_14032beef(a, b, &ang); // atan2(1, 0) of the Fortran runtime
    CHECK_BITS(ang, def::rt::libm::ps_atan2(1.0, 0.0));
    double z[2] = {0.0, 0.0};
    ang = 9.0;
    fun_14032beef(a, z, &ang);
    CHECK_BITS(ang, 0.0);
    // quarter arc from (1,0) about (0,0), sweep pi/2: 60 deg is on it, -60 deg is not (alpha + 2pi)
    double s[2] = {1.0, 0.0}, c[2] = {0.0, 0.0}, phi = 3.1415926535898 / 2.0;
    double x1[2] = {0.5, std::sqrt(0.75)}, x2[2] = {0.5, -std::sqrt(0.75)};
    std::int32_t in = 7;
    fun_14032bd2a(s, c, &phi, x1, &in);
    CHECK_EQ(in, 1);
    fun_14032bd2a(s, c, &phi, x2, &in);
    CHECK_EQ(in, 0);
    double x3[2] = {0.6, 0.6}; // radius 0.85 != 1 by more than ext * GEOERR2 = 0.01
    fun_14032bd2a(s, c, &phi, x3, &in);
    CHECK_EQ(in, 0);
}

TEST_CASE(arc_kernel_hit)
{
    context(10.0, 0.001);
    // Ray from Q = (0.5, -2) along +y meets the unit circle at y = -sqrt(0.75) (angle -60 deg,
    // off the arc) and y = +sqrt(0.75) (on the arc): gap = 2 + sqrt(0.75) (tolerance: depends on
    // the runtime's sin/cos in the 90-degree rotation 1404d71d7)
    double s[2] = {1.0, 0.0}, c[2] = {0.0, 0.0}, phi = 3.1415926535898 / 2.0;
    double q[2] = {0.5, -2.0}, dir[2] = {0.0, 1.0}, tola = 0.01;
    double gap = 1e10;
    std::int32_t flag = 0;
    fun_1403407d7(s, c, &phi, q, dir, &gap, &tola, &flag);
    CHECK_EQ(flag, 1);
    CHECK(std::fabs(gap - (2.0 + std::sqrt(0.75))) < 1e-12);
    // ray at x = 2 misses the circle: nothing written
    double q2[2] = {2.0, -2.0};
    gap = 1e10;
    flag = 0;
    fun_1403407d7(s, c, &phi, q2, dir, &gap, &tola, &flag);
    CHECK_EQ(flag, 0);
    CHECK_BITS(gap, 1e10);
}

TEST_CASE(arc_refinement_early_cases)
{
    // cc(2) == 0 and R <= tolA: gap = cc(1) / len, status 1
    double p1[2] = {0.0, 0.0}, p2[2] = {0.0, 0.0}, cc[2] = {3.0, 0.0};
    double r = 0.5, tola = 0.5, phi = 1.0, gap = 0.0, len = 2.0;
    std::int32_t st = 9;
    fun_14034151e(p1, p2, cc, &r, &tola, &phi, &gap, &len, &st);
    CHECK_EQ(st, 1);
    CHECK_BITS(gap, 1.5);
    // cc(2) == 0, R > tolA, cc(1) > p1(1), p1(2) == 0: gap = (tolA + p1(1)) / len
    r = 2.0;
    p1[0] = 1.0;
    fun_14034151e(p1, p2, cc, &r, &tola, &phi, &gap, &len, &st);
    CHECK_EQ(st, 1);
    CHECK_BITS(gap, 0.75);
    // p1(1) > cc(1): gap = (p1(1) - tolA) / len
    p1[0] = 5.0;
    fun_14034151e(p1, p2, cc, &r, &tola, &phi, &gap, &len, &st);
    CHECK_EQ(st, 1);
    CHECK_BITS(gap, 2.25);
}

TEST_CASE(samplers_on_one_segment)
{
    context(10.0, 0.001);
    // points (0,0) -> (2,0); samples x = 0.5, 1.0, 1.5 at y = -1, dir (0,1): gap 1 each
    double pts[4] = {0.0, 0.0, 2.0, 0.0};
    std::int32_t np = 2, n = 2, obj = 1;
    double yref = -1.0, x0 = 0.5, x1 = 1.5, dir[2] = {0.0, 1.0};
    double g[3] = {0.0, 0.0, 0.0};
    std::int32_t skip0 = 1;
    fun_14033ffb8(&skip0);
    CHECK_EQ(g_skip_negative, 0);
    fun_14033ffe0(&obj, pts, &np, &yref, &x0, &x1, &n, dir, g);
    for (double v : g) CHECK_BITS(v, 1.0);
    // the same as a line record of a die curve, added on top
    double rec[6] = {1.0, 0.0, 0.0, 2.0, 0.0, 0.0};
    std::int32_t nrec = 1, err = 5;
    fun_140340273(&obj, rec, &nrec, &yref, &x0, &x1, &n, dir, g, &err);
    CHECK_EQ(err, 0);
    for (double v : g) CHECK_BITS(v, 2.0);
    // a zero-length line (< 1e-6f) gives the Fortran warning (stderr) and err = 1, g untouched
    double bad[12] = {1.0, 0.0, 0.0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0 + 1e-7, 0.0};
    nrec = 2;
    fun_140340273(&obj, bad, &nrec, &yref, &x0, &x1, &n, dir, g, &err);
    CHECK_EQ(err, 1);
    for (double v : g) CHECK_BITS(v, 2.0);
    // no piece hit: 1e10 is added
    double g2[3] = {0.0, 0.0, 0.0};
    double far = 5.0, fx0 = 3.0, fx1 = 4.0;
    fun_14033ffe0(&obj, pts, &np, &far, &fx0, &fx1, &n, dir, g2);
    for (double v : g2) CHECK_BITS(v, 1e10);
}

TEST_CASE(negative_gap_cut)
{
    context(10.0, 0.001);
    // Q (1, 0.005) lies 0.005 behind the segment: gap 0 is a hit (not below -1e-15), kept
    double pts[4] = {0.0, 0.0, 2.0, 0.0};
    std::int32_t np = 2, n = 1, obj = 1, flag0 = 0;
    double yref = 0.005, x0 = 0.5, x1 = 1.5, dir[2] = {0.0, 1.0}, g[2] = {0.0, 0.0};
    fun_14033ffb8(&flag0);
    CHECK_EQ(g_skip_negative, 1);
    fun_14033ffe0(&obj, pts, &np, &yref, &x0, &x1, &n, dir, g);
    CHECK_BITS(g[0], 0.0);
    CHECK_BITS(g[1], 0.0);
}

TEST_CASE(area_and_shoelace)
{
    double g[3] = {0.0, 0.0, 0.0};
    std::int32_t n = 2;
    double dx = 1.0, s = 1.0;
    // ((1 + 1) * 1) * 0.5 per cell, two cells
    CHECK_BITS(fun_14033fe8e(&n, &dx, g, &s), 2.0);
    s = -1.0; // max(0, .) everywhere
    CHECK_BITS(fun_14033fe8e(&n, &dx, g, &s), 0.0);
    // unit square counter-clockwise: S = 0 - 1 - 1 + 0 = -2, returns -1
    double sq[8] = {0, 0, 1, 0, 1, 1, 0, 1};
    std::int32_t four = 4;
    CHECK_BITS(fun_14033fdb8(&four, sq), -1.0);
}

TEST_CASE(root_finder)
{
    // g = 0 on 2 cells of width 0.5: A(s) = s for s >= 0
    double g[3] = {0.0, 0.0, 0.0};
    std::int32_t n = 2, err = 9;
    double dx = 0.5, step = 0.25, atol = 0.5, s = 0.0;
    // bracket: s = 0.25 (A 0.25), s = 0.5 (A == Atol): returns there
    fun_14033f8d4(&step, &atol, g, &n, &dx, &s, &err);
    CHECK_EQ(err, 0);
    CHECK_BITS(s, 0.5);
    // Atol 0.6: bracket [0.5, 0.75], false position lands on ~0.6
    atol = 0.6;
    fun_14033f8d4(&step, &atol, g, &n, &dx, &s, &err);
    CHECK_EQ(err, 0);
    CHECK(std::fabs(s - 0.6) < 1e-12);
    // tol = 0 quirk: Atol 0, every step gives s = lo, A = 0, relative test NaN -> 100 iterations
    atol = 0.0;
    fun_14033f8d4(&step, &atol, g, &n, &dx, &s, &err);
    CHECK_EQ(err, 1);
    // no gap anywhere (gmin >= 1e10)
    double gb[3] = {1e10, 1e10, 1e10};
    atol = 0.5;
    fun_14033f8d4(&step, &atol, gb, &n, &dx, &s, &err);
    CHECK_EQ(err, 1);
}

TEST_CASE(loop_rotation)
{
    // rectangle (0,0) (2,0) (2,1) (0,1), loop 1 2 3 4 1, theta 0 (ps_sincos(0) is used as is)
    double sn = 0.0, cs = 0.0, th = 0.0;
    def::rt::libm::ps_sincos(th, &sn, &cs);
    const double xyin[8] = {0, 0, 2, 0, 2, 1, 0, 1};
    std::int32_t nodes[5] = {1, 2, 3, 4, 1};
    for (std::int32_t side = 1; side <= 2; ++side) {
        double xy[8];
        std::memcpy(xy, xyin, sizeof xy);
        double out[8] = {};
        std::int32_t range[2] = {1, 5}, count = 5;
        double area = 0.0, xmm[2] = {}, ymm[2] = {};
        std::int32_t sd = side;
        fun_14033f2bf(&sd, xy, out, range, nodes, &count, &area, xmm, ymm, &th);
        CHECK_EQ(count, 2);
        CHECK(std::fabs(area - 2.0) < 1e-12);
        CHECK_BITS(xmm[0], cs * 0.0 - sn * 0.0);
        CHECK(std::fabs(xmm[1] - 2.0) < 1e-12);
        CHECK(std::fabs(ymm[1] - 1.0) < 1e-12);
        if (side == 1) {
            // x-min node 1 (node 4 ties but lies above), x-max node 2 (node 3 ties, above): bottom
            CHECK(std::fabs(xy[0]) < 1e-12 && std::fabs(xy[1]) < 1e-12);
            CHECK(std::fabs(xy[2] - 2.0) < 1e-12 && std::fabs(xy[3]) < 1e-12);
        } else {
            // ties go to the upper nodes, then swapped: from node 3 to node 4 (top edge)
            CHECK(std::fabs(xy[0] - 2.0) < 1e-12 && std::fabs(xy[1] - 1.0) < 1e-12);
            CHECK(std::fabs(xy[2]) < 1e-12 && std::fabs(xy[3] - 1.0) < 1e-12);
        }
    }
}

TEST_CASE(zero_length_rows_and_pow)
{
    def::model::OBJ_GEO2D c;
    c.curve_type = 2;
    c.n_points = 3;
    c.points = c.allocate_points();
    const double rows[18] = {1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1e-7, 0, 1, 1, 0, 2, 0, 0};
    std::memcpy(c.points, rows, sizeof rows);
    CHECK(fun_14027e990(&c));
    CHECK_EQ(c.n_points, 2);
    CHECK_BITS(c.points[6 + 3], 2.0);
    CHECK(!fun_14027e990(&c));
    CHECK_BITS(fun_1402204a0(10.0, 0), 1.0);
    CHECK_BITS(fun_1402204a0(10.0, 1), 10.0);
    CHECK_BITS(fun_1402204a0(2.0, -2), 0.25);
}

TEST_CASE(arc_bbox)
{
    double x0, x1, y0, y1;
    // degenerate sweep: x1 for all four values (as the original)
    fun_14027ab80(1.0, 5.0, 0.0, 0.0, 0.0, &x0, &x1, &y0, &y1);
    CHECK_BITS(x0, 1.0);
    CHECK_BITS(y0, 1.0);
    CHECK_BITS(y1, 1.0);
    // full circle
    fun_14027ab80(1.0, 0.0, 0.0, 0.0, 360.0, &x0, &x1, &y0, &y1);
    CHECK_BITS(x0, -1.0);
    CHECK_BITS(x1, 1.0);
    CHECK_BITS(y0, -1.0);
    CHECK_BITS(y1, 1.0);
    // quarter from (1,0) ccw to (0,1): start quadrant 0, end quadrant 0 or 1 (cos 90 ~ 0);
    // box [0..1] x [0..1] within rounding
    fun_14027ab80(1.0, 0.0, 0.0, 0.0, 90.0, &x0, &x1, &y0, &y1);
    CHECK(std::fabs(x0) < 1e-6 && x1 == 1.0 && y0 == 0.0 && std::fabs(y1 - 1.0) < 1e-6);
    // half circle from (1,0) ccw to (-1,0): quadrants 0 -> 1 or 2: ymax reaches the top (table)
    fun_14027ab80(1.0, 0.0, 0.0, 0.0, 180.0, &x0, &x1, &y0, &y1);
    CHECK(std::fabs(x0 + 1.0) < 1e-6 && x1 == 1.0 && std::fabs(y1 - 1.0) < 1e-12);
}

namespace {

// A meshed rectangle object: 4 nodes, 1 element, boundary loop 1 2 3 4 1 (counter-clockwise).
struct RectObj {
    std::vector<std::uint64_t> mem = std::vector<std::uint64_t>(sizeof(LLOBJ_DATA) / 8 + 1, 0);
    double xy[8];
    std::int32_t nodes[5] = {1, 2, 3, 4, 1};
    RectObj(std::int32_t num, double xa, double ya, double xb, double yb)
    {
        const double p[8] = {xa, ya, xb, ya, xb, yb, xa, yb};
        std::memcpy(xy, p, sizeof xy);
        void* o = obj();
        at<std::int32_t>(o, 8) = num;
        at<std::int32_t>(o, 0xc) = 1;
        at<std::int32_t>(o, 0x87c) = 1;
        at<std::int32_t>(o, 0x880) = 4;
        at<std::int32_t>(o, 0x884) = 1;
        at<double*>(o, 0x8a8) = xy;            // node set +8
        at<std::int32_t>(o, 0x8a0 + 300) = 4; // node set count
        at<std::int32_t>(o, 0xbd8) = 1;
        at<std::int32_t>(o, 0xbe0) = 5;
        at<std::int32_t*>(o, 0xbe8) = nodes;
    }
    LLOBJ_DATA* obj() { return reinterpret_cast<LLOBJ_DATA*>(mem.data()); }
};

} // namespace

TEST_CASE(outer_loop_and_coords)
{
    def::model::llobj::ext::g_hook = hook_ext;
    RectObj r(1, 0.0, 0.0, 2.0, 1.0);
    std::int32_t idx = -5;
    CHECK(fun_1401ffa90(r.obj(), &idx));
    CHECK_EQ(idx, 1);
    CHECK(fun_1401808d0(&at<std::byte>(r.obj(), 0x8a0)) == r.xy);
    at<std::int32_t>(r.obj(), 0x87c) = 0;
    CHECK(!fun_1401ffa90(r.obj(), &idx));
    CHECK_EQ(idx, 0);
    def::model::llobj::ext::g_hook = nullptr;
}

TEST_CASE(end_to_end_two_rectangles)
{
    // Target S [0,2] x [0,1], mover T [0.5,1.5] x [3,4], direction (0,-1), mode 102 (flag 0),
    // area 10 %. theta = 2 atan(1) - atan2(-1, 0) = pi; in the rotated frame T moves +y onto S;
    // gaps: S bottom chain 2 above T's top, T top chain 0 -> g = 2 (the last sample may miss T's
    // end point, 1e10). ref = min(area S 2, area T 1) = 1, Atol = 0.1, A(s) = (s - 2) * 1 ->
    // s ~ 2.1; displacement back in the original frame ~ (0, -2.1). Checked with tolerances (the
    // CRT atan/atan2 are test stand-ins).
    def::model::llobj::ext::g_hook = hook_ext;
    reset_state();
    std::vector<std::uint64_t> model(0x2000, 0);
    void* m = model.data();
    at<double>(m, 0xdb0) = 0.001;
    at<double>(m, 0xdb8) = 0.001;
    RectObj s(1, 0.0, 0.0, 2.0, 1.0);
    RectObj t(2, 0.5, 3.0, 1.5, 4.0);
    double d[2] = {0.0, -1.0}, out[2] = {0.0, 0.0};
    const std::int32_t rc = fun_1401a1370(reinterpret_cast<DB_STEP_DATA*>(m), s.obj(), t.obj(), d,
                                          10.0, 1000, out, 0);
    CHECK_EQ(rc, 0);
    CHECK(std::fabs(out[0]) < 1e-9);
    CHECK(std::fabs(out[1] + 2.1) < 1e-3);
    CHECK_EQ(g_skip_negative, 1);
    CHECK(std::fabs(g_ext - 2.0) < 1e-12); // max(|dx|, |dy|) of the rotated target
    // the objects' own coordinates are untouched (the kernel works on copies)
    CHECK_BITS(t.xy[5], 4.0);
    // driver: same through 1401a0c70 (mover a = T, target b = S), dist = |out| < 1e9
    double tol = 10.0, dist = 0.0, disp[2] = {0.0, 0.0};
    CHECK(def::cmd::ifd::fun_1401a0c70(reinterpret_cast<DB_STEP_DATA*>(m), t.obj(), s.obj(), d, &tol,
                                       &dist, disp, 0));
    CHECK(std::fabs(dist - 2.1) < 1e-3);
    CHECK_BITS(disp[1], out[1]);
    // no overlap in x: kernel fails
    RectObj far(3, 5.0, 3.0, 6.0, 4.0);
    CHECK_EQ(fun_1401a1370(reinterpret_cast<DB_STEP_DATA*>(m), s.obj(), far.obj(), d, 10.0, 1000, out, 0), 1);
    // area 0 %: always fails (100 iterations)
    CHECK_EQ(fun_1401a1370(reinterpret_cast<DB_STEP_DATA*>(m), s.obj(), t.obj(), d, 0.0, 1000, out, 0), 1);
    def::model::llobj::ext::g_hook = nullptr;
}

int main() { return testkit::run_all(); }
