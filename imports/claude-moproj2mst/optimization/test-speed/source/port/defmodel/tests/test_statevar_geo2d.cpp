// 2D helpers and Fortran curve kernels placed in the StateVar ranges (p1-09). Expected values are
// worked out by hand from the decompiled code; cases whose result depends on libm rounding are
// checked to 1e-12 (the port uses <cmath> in place of the PathScale libm, see the header).
// ctest-labels: p1-09 statevar geo2d
#include <defmodel/globals_p1_09.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <testkit/check.hpp>

#include <cmath>

using namespace def::model;
using namespace def::model::geo2d;

namespace {

bool near(double a, double b) { return std::fabs(a - b) <= 1e-12; }

struct StopCalled {};
void throwing_stop() { throw StopCalled{}; }

} // namespace

TEST_CASE(vector_helpers)
{
    const double a[2] = {1.5, -2.0};
    const double b[2] = {4.0, 0.25};
    double d[2];
    sub_vec(d, b, a);
    CHECK_BITS(d[0], 2.5);
    CHECK_BITS(d[1], 2.25);
    add_vec(d, a, b);
    CHECK_BITS(d[0], 5.5);
    CHECK_BITS(d[1], -1.75);
    copy_vec(d, a);
    CHECK_BITS(d[0], 1.5);
    CHECK_BITS(d[1], -2.0);
    double o[2];
    rotate_difference_deg(a, b, o, 0.0);   // sin 0 = 0, cos 0 = 1: exact
    CHECK_BITS(o[0], 2.5);
    CHECK_BITS(o[1], 2.25);
    rotate_difference_deg(a, b, o, 90.0);
    CHECK(near(o[0], -2.25));
    CHECK(near(o[1], 2.5));
}

TEST_CASE(polygon_area_shoelace)
{
    const double sq[8] = {0, 0, 1, 0, 1, 1, 0, 1};   // terms 0, 1, 1, 0 -> 0.5 + 0.5
    CHECK_BITS(polygon_area(4, sq), 1.0);
    const double cw[8] = {0, 0, 0, 1, 1, 1, 1, 0};
    CHECK_BITS(polygon_area(4, cw), -1.0);
    CHECK_BITS(polygon_area(0, sq), 0.0);
}

TEST_CASE(rotate_point_about_centre)
{
    double p[2] = {3.0, 5.0};
    const double c[2] = {1.0, 2.0};
    const double zero = 0.0;
    rotate_point_deg(p, &zero, c);   // (p - c) + c with integers: exact
    CHECK_BITS(p[0], 3.0);
    CHECK_BITS(p[1], 5.0);
    const double ninety = 90.0;   // angle = 90 / 45 * pi/4 = pi/2
    rotate_point_deg(p, &ninety, c);
    CHECK(near(p[0], 1.0 - 3.0));
    CHECK(near(p[1], 2.0 + 2.0));
}

TEST_CASE(lines_to_points_merges_shared_ends)
{
    reset_globals_p1_09();
    const double ents[12] = {1, 0, 0, 1, 0, 0,   // line (0,0)-(1,0)
                             1, 1, 0, 1, 1, 0};  // line (1,0)-(1,1)
    double pts[3 * 6];
    for (double& v : pts) v = -9.0;
    std::int32_t nent = 2;
    std::int32_t npts = -1;
    entities_to_points(&npts, pts, &nent, ents);
    // 4 points written; point 3 equals point 2 (distance 0 < 1e-6) and is removed, record 4 zeroed
    REQUIRE(npts == 3);
    const double want[12] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0};
    for (int i = 0; i < 12; ++i) CHECK_BITS(pts[i], want[i]);
    CHECK_BITS(pts[12], -9.0);   // beyond the written records: untouched
    // the merge set the SAVE tolerances: min(1.0 * 1e-6, 1e-6) for both
    CHECK_BITS(dat_1405f3c00, 1e-06);
    CHECK_BITS(dat_1405f3c08, 1e-06);
    CHECK_BITS(g_common_140b84720.u10, 1.0);
}

TEST_CASE(quarter_arc_one_piece)
{
    reset_globals_p1_09();
    const double half_pi = 1.5707963267948966;
    const double ents[6] = {2, 1, 0, 0, 0, half_pi};   // start (1,0), centre (0,0), +90 degrees
    double pts[3 * 4] = {};
    std::int32_t nent = 1;
    std::int32_t npts = 0;
    entities_to_points(&npts, pts, &nent, ents);
    REQUIRE(npts == 3);
    // start = centre + radius vector: exact
    CHECK_BITS(pts[0], 1.0);
    CHECK_BITS(pts[1], 0.0);
    CHECK_BITS(pts[2], 0.0);
    // control point: start + |tan(45 deg)| * r * tangent, r stored in the third field
    CHECK(near(pts[3], 1.0));
    CHECK(near(pts[4], 1.0));
    CHECK_BITS(pts[5], 1.0);
    // end point
    CHECK(near(pts[6], 0.0));
    CHECK(near(pts[7], 1.0));
    CHECK_BITS(pts[8], 0.0);
}

TEST_CASE(three_quarter_arc_three_pieces)
{
    reset_globals_p1_09();
    // 270 degrees: 1 piece (> 120), 2 pieces (135 > 120), 3 pieces (90): 9 points, two shared
    // ends merged -> 7
    const double ents[6] = {2, 2, 0, 0, 0, 3 * 1.5707963267948966};
    double pts[3 * 9] = {};
    std::int32_t nent = 1;
    std::int32_t npts = 0;
    entities_to_points(&npts, pts, &nent, ents);
    REQUIRE(npts == 7);
    CHECK(near(pts[3 * 6 + 0], 0.0));   // end (0, -2)
    CHECK(near(pts[3 * 6 + 1], -2.0));
    CHECK_BITS(pts[3 * 1 + 2], 2.0);    // control points carry r = 2
    CHECK_BITS(pts[3 * 3 + 2], 2.0);
    CHECK_BITS(pts[3 * 5 + 2], 2.0);
    for (int i = 21; i < 27; ++i) CHECK_BITS(pts[i], 0.0);   // freed records zeroed
}

TEST_CASE(too_large_sweep_stops)
{
    reset_globals_p1_09();
    const double ents[6] = {2, 1, 0, 0, 0, 7.0};   // 7 / 3 > 2.0944: needs a 4th piece
    double pts[3 * 16] = {};
    std::int32_t nent = 1;
    std::int32_t npts = 0;
    g_stop_1403c4504 = &throwing_stop;
    bool stopped = false;
    try {
        entities_to_points(&npts, pts, &nent, ents);
    } catch (const StopCalled&) {
        stopped = true;
    }
    g_stop_1403c4504 = nullptr;
    CHECK(stopped);
}

TEST_CASE(corner_fillet_entities)
{
    reset_globals_p1_09();
    // (0,0) -> (1,0) with radius 0.1 -> (1,1)
    const double pts[9] = {0, 0, 0, 1, 0, 0.1, 1, 1, 0};
    double ents[6 * 4] = {};
    std::int32_t npts = 3;
    std::int32_t nent = -1;
    points_to_entities(&npts, pts, &nent, ents);
    REQUIRE(nent == 3);
    // unit directions d1 = (1,0), d2 = (0,1); cross = 1; sr = sgr = 0.1
    const double cx = (0.0 - 1.0) * 0.1 / 1.0 + 1.0;   // 0.9
    const double cy = (1.0 - 0.0) * 0.1 / 1.0 + 0.0;   // 0.1
    // line (0,0) -> E = (y1 * 0.1 + cx, cy - x1 * 0.1)
    CHECK_BITS(ents[0], 1.0);
    CHECK_BITS(ents[1], 0.0);
    CHECK_BITS(ents[2], 0.0);
    CHECK_BITS(ents[3], 0.0 * 0.1 + cx);
    CHECK_BITS(ents[4], cy - 1.0 * 0.1);
    // arc: start E, centre C, angle +acos(0)
    CHECK_BITS(ents[6], 2.0);
    CHECK_BITS(ents[7], 0.0 * 0.1 + cx);
    CHECK_BITS(ents[8], cy - 1.0 * 0.1);
    CHECK_BITS(ents[9], cx);
    CHECK_BITS(ents[10], cy);
    CHECK(near(ents[11], 1.5707963267948966));
    // line T = (y2 * 0.1 + cx, cy - x2 * 0.1) -> (1,1)
    CHECK_BITS(ents[12], 1.0);
    CHECK_BITS(ents[13], 1.0 * 0.1 + cx);
    CHECK_BITS(ents[14], cy - 0.0 * 0.1);
    CHECK_BITS(ents[15], 1.0);
    CHECK_BITS(ents[16], 1.0);
}

TEST_CASE(sharp_corners_and_merge)
{
    reset_globals_p1_09();
    // radius 0 everywhere: lines between the points; a zero-length segment merges into the
    // previous line (trim_entity_end type 1 moves its end)
    const double pts[12] = {0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 3, 0};
    double ents[6 * 4] = {};
    std::int32_t npts = 4;
    std::int32_t nent = 0;
    points_to_entities(&npts, pts, &nent, ents);
    REQUIRE(nent == 2);
    const double want[12] = {1, 0, 0, 2, 0, 0, 1, 2, 0, 2, 3, 0};
    for (int i = 0; i < 12; ++i) CHECK_BITS(ents[i], want[i]);
    CHECK_BITS(g_common_140b84720.u00, 1e-06);
    CHECK_BITS(g_common_140b84720.u08, 1e-06);
}

TEST_CASE(trim_line_and_arc_end)
{
    double p1[2] = {0, 0};
    double p2[2] = {5, 5};
    double e[2] = {3, 4};
    double ang = 9.0;
    const std::int32_t line = 1;
    trim_entity_end(p1, p2, e, &ang, &line);
    CHECK_BITS(p2[0], 3.0);
    CHECK_BITS(p2[1], 4.0);
    CHECK_BITS(ang, 9.0);
    // arc start (2,0) about (0,0); target direction (0,5): angle +90, e moved onto radius 2
    const double s[2] = {2, 0};
    double c[2] = {0, 0};
    double t[2] = {0, 5};
    const std::int32_t arc = 2;
    trim_entity_end(s, c, t, &ang, &arc);
    CHECK(near(ang, 1.5707963267948966));
    CHECK(near(t[0], 0.0));
    CHECK(near(t[1], 2.0));
    // degenerate: start at the centre -> angle 0, nothing else
    const double s0[2] = {0, 0};
    double t0[2] = {1, 1};
    trim_entity_end(s0, c, t0, &ang, &arc);
    CHECK_BITS(ang, 0.0);
    CHECK_BITS(t0[0], 1.0);
}

TEST_CASE(merge_never_checks_last_pair)
{
    reset_globals_p1_09();
    double pts[12] = {0, 0, 0, 0, 0, 7, 1, 0, 0, 1, 0, 8};
    std::int32_t n = 4;
    std::int32_t merged = -1;
    merge_close_points(&n, pts, &merged);
    // (1,2) merge: point 2 removed; then (1,2) = (0,0)-(1,0) differs; pair (2,3) is the last
    // pair and is not compared, so the duplicate (1,0) stays
    CHECK_EQ(n, 3);
    CHECK_EQ(merged, 1);
    CHECK_BITS(pts[3], 1.0);
    CHECK_BITS(pts[5], 0.0);
    CHECK_BITS(pts[8], 8.0);
}

int main() { return testkit::run_all(); }
